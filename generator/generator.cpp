#include <libxml/parser.h>
#include <iostream>
#include <dirent.h>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sqlite3.h>
#include <sstream>
#include <regex>
#include <list>
#include <algorithm>
#include <json.hpp>
#include "progress.h"
#include "../lib/util.h"

using json = nlohmann::json;

struct verb_t {
  std::string infinitive;
  std::string past_tense;
  std::string past_participle;
  std::string ing_form;
  std::string s_form;
  int id;
};

struct adjective_t {
  std::string base;
  std::string comparative;
  std::string superlative;
};

struct noun_t {
  std::string singular;
  std::string plural;
};

struct selrestr_t {
  enum class type_t {
    singleton,
    andlogic,
    orlogic,
    empty
  };
  type_t type;
  std::string restriction;
  bool pos;
  std::list<selrestr_t> subordinates;
};

struct framepart_t {
  enum class type_t {
    np,
    v,
    pp,
    adj,
    adv,
    lex
  };
  type_t type;
  std::string role;
  selrestr_t selrestrs;
  std::set<std::string> preprestrs;
  std::set<std::string> synrestrs;
  std::list<std::string> choices;
  std::string lexval;
};

struct group_t {
  std::string id;
  std::string parent;
  std::set<std::string> members;
  std::map<std::string, selrestr_t> roles;
  std::list<std::list<framepart_t>> frames;
};

struct pronunciation_t {
  std::string phonemes;
  std::string prerhyme;
  std::string rhyme;
  
  bool operator<(const pronunciation_t& other) const
  {
    return phonemes < other.phonemes;
  }
};

std::map<std::string, group_t> groups;
std::map<std::string, verb_t> verbs;
std::map<std::string, adjective_t> adjectives;
std::map<std::string, noun_t> nouns;
std::map<int, std::map<int, int>> wn;
std::map<int, int> images;
std::map<std::string, std::set<pronunciation_t>> pronunciations;

void print_usage()
{
  std::cout << "Verbly Datafile Generator" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "Requires exactly six arguments." << std::endl;
  std::cout << "1. The path to a VerbNet data directory." << std::endl;
  std::cout << "2. The path to an AGID infl.txt file." << std::endl;
  std::cout << "3. The path to a WordNet prolog data directory." << std::endl;
  std::cout << "4. The path to a CMUDICT pronunciation file." << std::endl;
  std::cout << "5. The path to an ImageNet urls.txt file." << std::endl;
  std::cout << "6. Datafile output path." << std::endl;
  
  exit(1);
}

void db_error(sqlite3* ppdb, std::string query)
{
  std::cout << "Error writing to output database: " << sqlite3_errmsg(ppdb) << std::endl;
  std::cout << query << std::endl;
  sqlite3_close_v2(ppdb);
  print_usage();
}

json export_selrestrs(selrestr_t r)
{
  if (r.type == selrestr_t::type_t::empty)
  {
    return {};
  } else if (r.type == selrestr_t::type_t::singleton)
  {
    json result;
    result["type"] = r.restriction;
    result["pos"] = r.pos;
    return result;
  } else {
    json result;
    if (r.type == selrestr_t::type_t::andlogic)
    {
      result["logic"] = "and";
    } else {
      result["logic"] = "or";
    }
    
    std::list<json> outlist;
    std::transform(std::begin(r.subordinates), std::end(r.subordinates), std::back_inserter(outlist), &export_selrestrs);
    result["children"] = outlist;
    
    return result;
  }
}

selrestr_t parse_selrestrs(xmlNodePtr top, std::string filename)
{
  selrestr_t r;
  xmlChar* key;
  
  if (!xmlStrcmp(top->name, (const xmlChar*) "SELRESTRS"))
  {
    if (xmlChildElementCount(top) == 0)
    {
      r.type = selrestr_t::type_t::empty;
    } else if (xmlChildElementCount(top) == 1)
    {
      r = parse_selrestrs(xmlFirstElementChild(top), filename);
    } else {
      r.type = selrestr_t::type_t::andlogic;
      
      if (xmlHasProp(top, (const xmlChar*) "logic"))
      {
        key = xmlGetProp(top, (const xmlChar*) "logic");
        if (!xmlStrcmp(key, (const xmlChar*) "or"))
        {
          r.type = selrestr_t::type_t::orlogic;
        }
        xmlFree(key);
      }
  
      for (xmlNodePtr selrestr = top->xmlChildrenNode; selrestr != nullptr; selrestr = selrestr->next)
      {
        if (!xmlStrcmp(selrestr->name, (const xmlChar*) "SELRESTRS") || !xmlStrcmp(selrestr->name, (const xmlChar*) "SELRESTR"))
        {
          r.subordinates.push_back(parse_selrestrs(selrestr, filename));
        }
      }
    }
  } else if (!xmlStrcmp(top->name, (const xmlChar*) "SELRESTR"))
  {
    r.type = selrestr_t::type_t::singleton;
    
    key = xmlGetProp(top, (xmlChar*) "Value");
    r.pos = (std::string((const char*)key) == "+");
    xmlFree(key);

    key = xmlGetProp(top, (xmlChar*) "type");
    r.restriction = (const char*) key;
    xmlFree(key);
  } else {
    // Invalid
    std::cout << "Bad VerbNet file format: " << filename << std::endl;
    print_usage();
  }
  
  return r;
}

group_t& parse_group(xmlNodePtr top, std::string filename)
{
  xmlChar* key = xmlGetProp(top, (xmlChar*) "ID");
  if (key == 0)
  {
    std::cout << "Bad VerbNet file format: " << filename << std::endl;
    print_usage();
  }
  std::string vnid = (const char*)key;
  vnid = vnid.substr(vnid.find_first_of("-")+1);
  xmlFree(key);
  
  group_t g;
  g.id = vnid;
  
  for (xmlNodePtr node = top->xmlChildrenNode; node != nullptr; node = node->next)
  {
    if (!xmlStrcmp(node->name, (const xmlChar*) "SUBCLASSES"))
    {
      for (xmlNodePtr subclass = node->xmlChildrenNode; subclass != nullptr; subclass = subclass->next)
      {
        if (!xmlStrcmp(subclass->name, (const xmlChar*) "VNSUBCLASS"))
        {
          auto& sg = parse_group(subclass, filename);
          sg.parent = vnid;
          
          for (auto member : sg.members)
          {
            g.members.insert(member);
          }
          
          // The schema requires that subclasses appear after role definitions, so we can do this now
          for (auto role : g.roles)
          {
            if (sg.roles.count(role.first) == 0)
            {
              sg.roles[role.first] = role.second;
            }
          }
        }
      }
    } else if (!xmlStrcmp(node->name, (const xmlChar*) "MEMBERS"))
    {
      for (xmlNodePtr member = node->xmlChildrenNode; member != nullptr; member = member->next)
      {
        if (!xmlStrcmp(member->name, (const xmlChar*) "MEMBER"))
        {
          key = xmlGetProp(member, (xmlChar*) "name");
          g.members.insert((const char*)key);
          xmlFree(key);
        }
      }
    } else if (!xmlStrcmp(node->name, (const xmlChar*) "THEMROLES"))
    {
      for (xmlNodePtr role = node->xmlChildrenNode; role != nullptr; role = role->next)
      {
        if (!xmlStrcmp(role->name, (const xmlChar*) "THEMROLE"))
        {
          selrestr_t r;
          r.type = selrestr_t::type_t::empty;
          
          key = xmlGetProp(role, (const xmlChar*) "type");
          std::string type = (const char*)key;
          xmlFree(key);
          
          for (xmlNodePtr rolenode = role->xmlChildrenNode; rolenode != nullptr; rolenode = rolenode->next)
          {
            if (!xmlStrcmp(rolenode->name, (const xmlChar*) "SELRESTRS"))
            {
              r = parse_selrestrs(rolenode, filename);
            }
          }
          
          g.roles[type] = r;
        }
      }
    } else if (!xmlStrcmp(node->name, (const xmlChar*) "FRAMES"))
    {
      for (xmlNodePtr frame = node->xmlChildrenNode; frame != nullptr; frame = frame->next)
      {
        if (!xmlStrcmp(frame->name, (const xmlChar*) "FRAME"))
        {
          std::list<framepart_t> f;
          
          for (xmlNodePtr framenode = frame->xmlChildrenNode; framenode != nullptr; framenode = framenode->next)
          {
            if (!xmlStrcmp(framenode->name, (const xmlChar*) "SYNTAX"))
            {
              for (xmlNodePtr syntaxnode = framenode->xmlChildrenNode; syntaxnode != nullptr; syntaxnode = syntaxnode->next)
              {
                framepart_t fp;
                
                if (!xmlStrcmp(syntaxnode->name, (const xmlChar*) "NP"))
                {
                  fp.type = framepart_t::type_t::np;
                  
                  key = xmlGetProp(syntaxnode, (xmlChar*) "value");
                  fp.role = (const char*)key;
                  xmlFree(key);
                  
                  fp.selrestrs.type = selrestr_t::type_t::empty;
                  
                  for (xmlNodePtr npnode = syntaxnode->xmlChildrenNode; npnode != nullptr; npnode = npnode->next)
                  {
                    if (!xmlStrcmp(npnode->name, (const xmlChar*) "SYNRESTRS"))
                    {
                      for (xmlNodePtr synrestr = npnode->xmlChildrenNode; synrestr != nullptr; synrestr = synrestr->next)
                      {
                        if (!xmlStrcmp(synrestr->name, (const xmlChar*) "SYNRESTR"))
                        {
                          key = xmlGetProp(synrestr, (xmlChar*) "type");
                          fp.synrestrs.insert(std::string((const char*)key));
                          xmlFree(key);
                        }
                      }
                    }
                  
                    if (!xmlStrcmp(npnode->name, (const xmlChar*) "SELRESTRS"))
                    {
                      fp.selrestrs = parse_selrestrs(npnode, filename);
                    }
                  }
                } else if (!xmlStrcmp(syntaxnode->name, (xmlChar*) "VERB"))
                {
                  fp.type = framepart_t::type_t::v;
                } else if (!xmlStrcmp(syntaxnode->name, (xmlChar*) "PREP"))
                {
                  fp.type = framepart_t::type_t::pp;
                  
                  if (xmlHasProp(syntaxnode, (xmlChar*) "value"))
                  {
                    key = xmlGetProp(syntaxnode, (xmlChar*) "value");
                    std::string choices = (const char*)key;
                    xmlFree(key);
                  
                    fp.choices = verbly::split<std::list<std::string>>(choices, " ");
                  }
                  
                  for (xmlNodePtr npnode = syntaxnode->xmlChildrenNode; npnode != nullptr; npnode = npnode->next)
                  {
                    if (!xmlStrcmp(npnode->name, (const xmlChar*) "SELRESTRS"))
                    {
                      for (xmlNodePtr synrestr = npnode->xmlChildrenNode; synrestr != nullptr; synrestr = synrestr->next)
                      {
                        if (!xmlStrcmp(synrestr->name, (const xmlChar*) "SELRESTR"))
                        {
                          key = xmlGetProp(synrestr, (xmlChar*) "type");
                          fp.preprestrs.insert(std::string((const char*)key));
                          xmlFree(key);
                        }
                      }
                    }
                  }
                } else if (!xmlStrcmp(syntaxnode->name, (xmlChar*) "ADJ"))
                {
                  fp.type = framepart_t::type_t::adj;
                } else if (!xmlStrcmp(syntaxnode->name, (xmlChar*) "ADV"))
                {
                  fp.type = framepart_t::type_t::adv;
                } else if (!xmlStrcmp(syntaxnode->name, (xmlChar*) "LEX"))
                {
                  fp.type = framepart_t::type_t::lex;
                  
                  key = xmlGetProp(syntaxnode, (xmlChar*) "value");
                  fp.lexval = (const char*)key;
                  xmlFree(key);
                } else {
                  continue;
                }
                
                f.push_back(fp);
              }
              
              g.frames.push_back(f);
            }
          }
        }
      }
    }
  }
  
  groups[vnid] = g;
  
  return groups[vnid];
}

int main(int argc, char** argv)
{
  if (argc != 7)
  {
    print_usage();
  }
  
  // VerbNet data
  std::cout << "Reading verb frames..." << std::endl;
  
  DIR* dir;
  if ((dir = opendir(argv[1])) == nullptr)
  {
    std::cout << "Invalid VerbNet data directory." << std::endl;
    
    print_usage();
  }
  
  struct dirent* ent;
  while ((ent = readdir(dir)) != nullptr)
  {
    std::string filename(argv[1]);
    if (filename.back() != '/')
    {
      filename += '/';
    }
    
    filename += ent->d_name;
    //std::cout << ent->d_name << std::endl;
    
    if (filename.rfind(".xml") != filename.size() - 4)
    {
      continue;
    }
    
    xmlDocPtr doc = xmlParseFile(filename.c_str());
    if (doc == nullptr)
    {
      std::cout << "Error opening " << filename << std::endl;
      print_usage();
    }
    
    xmlNodePtr top = xmlDocGetRootElement(doc);
    if ((top == nullptr) || (xmlStrcmp(top->name, (xmlChar*) "VNCLASS")))
    {
      std::cout << "Bad VerbNet file format: " << filename << std::endl;
      print_usage();
    }
    
    parse_group(top, filename);
  }
  
  closedir(dir);
  
  // Get verbs from AGID
  std::cout << "Reading inflections..." << std::endl;
  
  std::ifstream agidfile(argv[2]);
  if (!agidfile.is_open())
  {
    std::cout << "Could not open AGID file: " << argv[2] << std::endl;
    print_usage();
  }
  
  for (;;)
  {
    std::string line;
    if (!getline(agidfile, line))
    {
      break;
    }
    
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    int divider = line.find_first_of(" ");
    std::string word = line.substr(0, divider);
    line = line.substr(divider+1);
    char type = line[0];
    
    if (line[1] == '?')
    {
      line.erase(0, 4);
    } else {
      line.erase(0, 3);
    }

    std::vector<std::string> forms;
    while (!line.empty())
    {
      std::string inflection;
      if ((divider = line.find(" | ")) != std::string::npos)
      {
        inflection = line.substr(0, divider);
        line = line.substr(divider + 3);
      } else {
        inflection = line;
        line = "";
      }
  
      if ((divider = inflection.find_first_of(",?")) != std::string::npos)
      {
        inflection = inflection.substr(0, divider);
      }
  
      forms.push_back(inflection);
    }
    
    switch (type)
    {
      case 'V':
      {
        verb_t v;
        v.infinitive = word;
        if (forms.size() == 4)
        {
          v.past_tense = forms[0];
          v.past_participle = forms[1];
          v.ing_form = forms[2];
          v.s_form = forms[3];
        } else if (forms.size() == 3)
        {
          v.past_tense = forms[0];
          v.past_participle = forms[0];
          v.ing_form = forms[1];
          v.s_form = forms[2];
        } else if (forms.size() == 8)
        {
          // As of AGID 2014.08.11, this is only "to be"
          v.past_tense = forms[0];
          v.past_participle = forms[2];
          v.ing_form = forms[3];
          v.s_form = forms[4];
        } else {
          // Words that don't fit the cases above as of AGID 2014.08.11:
          // - may and shall do not conjugate the way we want them to
          // - methinks only has a past tense and is an outlier
          // - wit has five forms, and is archaic/obscure enough that we can ignore it for now
          std::cout << "Ignoring verb \"" << word << "\" due to non-standard number of forms." << std::endl;
        }
    
        verbs[word] = v;
        
        break;
      }
      
      case 'A':
      {
        adjective_t adj;
        adj.base = word;
        if (forms.size() == 2)
        {
          adj.comparative = forms[0];
          adj.superlative = forms[1];
        } else {
          // As of AGID 2014.08.11, this is only "only", which has only the form "onliest"
          std::cout << "Ignoring adjective/adverb \"" << word << "\" due to non-standard number of forms." << std::endl;
        }
        
        adjectives[word] = adj;
        
        break;
      }
      
      case 'N':
      {
        noun_t n;
        n.singular = word;
        if (forms.size() == 1)
        {
          n.plural = forms[0];
        } else {
          // As of AGID 2014.08.11, this is non-existent.
          std::cout << "Ignoring noun \"" << word << "\" due to non-standard number of forms." << std::endl;
        }
        
        nouns[word] = n;
        
        break;
      }
    }
  }
  
  // Pronounciations
  std::cout << "Reading pronunciations..." << std::endl;
  
  std::ifstream pronfile(argv[4]);
  if (!pronfile.is_open())
  {
    std::cout << "Could not open CMUDICT file: " << argv[4] << std::endl;
    print_usage();
  }
  
  for (;;)
  {
    std::string line;
    if (!getline(pronfile, line))
    {
      break;
    }
    
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    std::regex phoneme("([A-Z][^ \\(]*)(?:\\(\\d+\\))?  ([A-Z 0-9]+)");
    std::smatch phoneme_data;
    if (std::regex_search(line, phoneme_data, phoneme))
    {
      std::string canonical(phoneme_data[1]);
      std::transform(std::begin(canonical), std::end(canonical), std::begin(canonical), ::tolower);
      
      std::string phonemes = phoneme_data[2];
      auto phoneme_set = verbly::split<std::list<std::string>>(phonemes, " ");
      auto phemstrt = std::find_if(std::begin(phoneme_set), std::end(phoneme_set), [] (std::string phoneme) {
        return phoneme.find("1") != std::string::npos;
      });
      
      pronunciation_t p;
      p.phonemes = phonemes;
      if (phemstrt != std::end(phoneme_set))
      {
        std::stringstream rhymer;
        for (auto it = phemstrt; it != std::end(phoneme_set); it++)
        {
          std::string naked;
          std::remove_copy_if(std::begin(*it), std::end(*it), std::back_inserter(naked), [] (char ch) {
            return isdigit(ch);
          });
          
          if (it != phemstrt)
          {
            rhymer << " ";
          }
          
          rhymer << naked;
        }
        
        p.rhyme = rhymer.str();
        
        if (phemstrt != std::begin(phoneme_set))
        {
          phemstrt--;
          p.prerhyme = *phemstrt;
        } else {
          p.prerhyme = "";
        }
      } else {
        p.prerhyme = "";
        p.rhyme = "";
      }
      
      pronunciations[canonical].insert(p);
    }
  }
  
  // Images
  std::cout << "Reading images..." << std::endl;
  
  std::ifstream imagefile(argv[5]);
  if (!imagefile.is_open())
  {
    std::cout << "Could not open ImageNet file: " << argv[5] << std::endl;
    print_usage();
  }
  
  for (;;)
  {
    std::string line;
    if (!getline(imagefile, line))
    {
      break;
    }
    
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    std::string wnid_s = line.substr(1, 8);
    int wnid = stoi(wnid_s) + 100000000;
    images[wnid]++;
  }
  
  imagefile.close();
  
  // Start writing output
  std::cout << "Writing schema..." << std::endl;
  
  sqlite3* ppdb;
  if (sqlite3_open_v2(argv[6], &ppdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
  {
    std::cout << "Error opening output datafile: " << sqlite3_errmsg(ppdb) << std::endl;
    print_usage();
  }
  
  std::ifstream schemafile("schema.sql");
  if (!schemafile.is_open())
  {
    std::cout << "Could not find schema file" << std::endl;
    print_usage();
  }
  
  std::stringstream schemabuilder;
  for (;;)
  {
    std::string line;
    if (!getline(schemafile, line))
    {
      break;
    }
    
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    schemabuilder << line << std::endl;
  }
  
  std::string schema = schemabuilder.str();
  while (!schema.empty())
  {
    std::string query;
    int divider = schema.find(";");
    if (divider != std::string::npos)
    {
      query = schema.substr(0, divider+1);
      schema = schema.substr(divider+2);
    } else {
      break;
    }
    
    sqlite3_stmt* schmstmt;
    if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &schmstmt, NULL) != SQLITE_OK)
    {
      db_error(ppdb, query);
    }
  
    if (sqlite3_step(schmstmt) != SQLITE_DONE)
    {
      db_error(ppdb, query);
    }
  
    sqlite3_finalize(schmstmt);
  }
  
  std::cout << "Writing prepositions..." << std::endl;
  std::ifstream prepfile("prepositions.txt");
  if (!prepfile.is_open())
  {
    std::cout << "Could not find prepositions file" << std::endl;
    print_usage();
  }
  
  for (;;)
  {
    std::string line;
    if (!getline(prepfile, line))
    {
      break;
    }
    
    if (line.back() == '\r')
    {
      line.pop_back();
    }
    
    std::regex relation("^([^:]+): (.+)");
    std::smatch relation_data;
    std::regex_search(line, relation_data, relation);
    std::string prep = relation_data[1];
    std::list<std::string> groups = verbly::split<std::list<std::string>>(relation_data[2], ", ");
    
    std::string query("INSERT INTO prepositions (form) VALUES (?)");
    sqlite3_stmt* ppstmt;
    
    if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      db_error(ppdb, query);
    }
    
    sqlite3_bind_text(ppstmt, 1, prep.c_str(), prep.length(), SQLITE_TRANSIENT);
    
    if (sqlite3_step(ppstmt) != SQLITE_DONE)
    {
      db_error(ppdb, query);
    }
    
    sqlite3_finalize(ppstmt);
    
    query = "SELECT last_insert_rowid()";
    if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      db_error(ppdb, query);
    }
    
    if (sqlite3_step(ppstmt) != SQLITE_ROW)
    {
      db_error(ppdb, query);
    }
    
    int rowid = sqlite3_column_int(ppstmt, 0);
    sqlite3_finalize(ppstmt);
    
    for (auto group : groups)
    {
      query = "INSERT INTO preposition_groups (preposition_id, groupname) VALUES (?, ?)";
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_bind_int(ppstmt, 1, rowid);
      sqlite3_bind_text(ppstmt, 2, group.c_str(), group.length(), SQLITE_TRANSIENT);
      
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_finalize(ppstmt);
    }
  }
  
  
  {
    progress ppgs("Writing verbs...", verbs.size());
    for (auto& mapping : verbs)
    {
      sqlite3_stmt* ppstmt;
      std::string query("INSERT INTO verbs (infinitive, past_tense, past_participle, ing_form, s_form) VALUES (?, ?, ?, ?, ?)");
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_text(ppstmt, 1, mapping.second.infinitive.c_str(), mapping.second.infinitive.length(), SQLITE_TRANSIENT);
      sqlite3_bind_text(ppstmt, 2, mapping.second.past_tense.c_str(), mapping.second.past_tense.length(), SQLITE_TRANSIENT);
      sqlite3_bind_text(ppstmt, 3, mapping.second.past_participle.c_str(), mapping.second.past_participle.length(), SQLITE_TRANSIENT);
      sqlite3_bind_text(ppstmt, 4, mapping.second.ing_form.c_str(), mapping.second.ing_form.length(), SQLITE_TRANSIENT);
      sqlite3_bind_text(ppstmt, 5, mapping.second.s_form.c_str(), mapping.second.s_form.length(), SQLITE_TRANSIENT);
    
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_finalize(ppstmt);
      
      std::string canonical(mapping.second.infinitive);
      std::transform(std::begin(canonical), std::end(canonical), std::begin(canonical), ::tolower);
      if (pronunciations.count(canonical) == 1)
      {
        query = "SELECT last_insert_rowid()";
        if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
        {
          db_error(ppdb, query);
        }
    
        if (sqlite3_step(ppstmt) != SQLITE_ROW)
        {
          db_error(ppdb, query);
        }
    
        int rowid = sqlite3_column_int(ppstmt, 0);
    
        sqlite3_finalize(ppstmt);
        
        mapping.second.id = rowid;
        
        for (auto pronunciation : pronunciations[canonical])
        {
          if (!pronunciation.rhyme.empty())
          {
            query = "INSERT INTO verb_pronunciations (verb_id, pronunciation, prerhyme, rhyme) VALUES (?, ?, ?, ?)";
          } else {
            query = "INSERT INTO verb_pronunciations (verb_id, pronunciation) VALUES (?, ?)";
          }
          
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, rowid);
          sqlite3_bind_text(ppstmt, 2, pronunciation.phonemes.c_str(), pronunciation.phonemes.length(), SQLITE_TRANSIENT);
          
          if (!pronunciation.rhyme.empty())
          {
            sqlite3_bind_text(ppstmt, 3, pronunciation.prerhyme.c_str(), pronunciation.prerhyme.length(), SQLITE_TRANSIENT);
            sqlite3_bind_text(ppstmt, 4, pronunciation.rhyme.c_str(), pronunciation.rhyme.length(), SQLITE_TRANSIENT);
          }
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
      
      ppgs.update();
    }
  }
  
  {
    progress ppgs("Writing verb frames...", groups.size());
    for (auto& mapping : groups)
    {
      std::list<json> roledatal;
      std::transform(std::begin(mapping.second.roles), std::end(mapping.second.roles), std::back_inserter(roledatal), [] (std::pair<std::string, selrestr_t> r) {
        json role;
        role["type"] = r.first;
        role["selrestrs"] = export_selrestrs(r.second);
        
        return role;
      });
      
      json roledata(roledatal);
      std::string rdm = roledata.dump();
      
      sqlite3_stmt* ppstmt;
      std::string query("INSERT INTO groups (data) VALUES (?)");
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_bind_blob(ppstmt, 1, rdm.c_str(), rdm.size(), SQLITE_TRANSIENT);
      
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_finalize(ppstmt);
      
      query = "SELECT last_insert_rowid()";
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
      
      if (sqlite3_step(ppstmt) != SQLITE_ROW)
      {
        db_error(ppdb, query);
      }
      
      int gid = sqlite3_column_int(ppstmt, 0);
      sqlite3_finalize(ppstmt);
      
      for (auto frame : mapping.second.frames)
      {
        std::list<json> fdatap;
        std::transform(std::begin(frame), std::end(frame), std::back_inserter(fdatap), [] (framepart_t& fp) {
          json part;
          
          switch (fp.type)
          {
            case framepart_t::type_t::np:
            {
              part["type"] = "np";
              part["role"] = fp.role;
              part["selrestrs"] = export_selrestrs(fp.selrestrs);
              part["synrestrs"] = fp.synrestrs;
              
              break;
            }
            
            case framepart_t::type_t::pp:
            {
              part["type"] = "pp";
              part["values"] = fp.choices;
              part["preprestrs"] = fp.preprestrs;
              
              break;
            }
            
            case framepart_t::type_t::v:
            {
              part["type"] = "v";
              
              break;
            }
            
            case framepart_t::type_t::adj:
            {
              part["type"] = "adj";
              
              break;
            }
            
            case framepart_t::type_t::adv:
            {
              part["type"] = "adv";
              
              break;
            }
            
            case framepart_t::type_t::lex:
            {
              part["type"] = "lex";
              part["value"] = fp.lexval;
              
              break;
            }
          }
          
          return part;
        });
        
        json fdata(fdatap);
        std::string marshall = fdata.dump();
        
        query = "INSERT INTO frames (group_id, data) VALUES (?, ?)";
        if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
        {
          db_error(ppdb, query);
        }
        
        sqlite3_bind_int(ppstmt, 1, gid);
        sqlite3_bind_blob(ppstmt, 2, marshall.c_str(), marshall.length(), SQLITE_TRANSIENT);
        
        if (sqlite3_step(ppstmt) != SQLITE_DONE)
        {
          db_error(ppdb, query);
        }
        
        sqlite3_finalize(ppstmt);
      }
      
      for (auto member : mapping.second.members)
      {
        if (verbs.count(member) == 1)
        {
          auto& v = verbs[member];
          
          query = "INSERT INTO verb_groups (verb_id, group_id) VALUES (?, ?)";
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, v.id);
          sqlite3_bind_int(ppstmt, 2, gid);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
      
      ppgs.update();
    }
  }
  
  // Get nouns/adjectives/adverbs from WordNet
  // Useful relations:
  // - s: master list
  // - ant: antonymy (e.g. happy/sad, sad/happy, happiness/sadness)
  // - at: variation (e.g. a measurement can be standard or nonstandard)
  // - der: derivation (e.g. happy/happily, happily/happy)
  // - hyp: hypernymy/hyponymy (e.g. color/red, color/blue)
  // - ins: instantiation (do we need this? let's see)
  // - mm: member meronymy/holonymy (e.g. family/mother, family/child)
  // - mp: part meronymy/holonymy (e.g. wheel/spoke, wheel/tire)
  // - ms: substance meronymy/holonymy (e.g. tire/rubber, doorstop/rubber)
  // - per: pertainymy (e.g. something that is Alaskan pertains to Alaska)
  //        mannernymy (e.g. something done quickly is done in a manner that is quick)
  // - sa: specification (e.g. inaccurate (general) can mean imprecise or incorrect (specific))
  // - sim: synonymy (e.g. cheerful/happy, happy/cheerful)
  // - syntax: positioning flags for some adjectives
  std::string wnpref {argv[3]};
  if (wnpref.back() != '/')
  {
    wnpref += '/';
  }
  
  // s table
  {
    std::ifstream wnsfile(wnpref + "wn_s.pl");
    if (!wnsfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }
  
    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnsfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing nouns, adjectives, and adverbs...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^s\\(([134]\\d{8}),(\\d+),'(.+)',\\w,\\d+,\\d+\\)\\.$");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
    
      int synset_id = stoi(relation_data[1]);
      int wnum = stoi(relation_data[2]);
      std::string word = relation_data[3];
      size_t word_it;
      while ((word_it = word.find("''")) != std::string::npos)
      {
        word.erase(word_it, 1);
      }
    
      std::string query;
      switch (synset_id / 100000000)
      {
        case 1: // Noun
        {
          if (nouns.count(word) == 1)
          {
            query = "INSERT INTO nouns (singular, proper, complexity, images, wnid, plural) VALUES (?, ?, ?, ?, ?, ?)";
          } else {
            query = "INSERT INTO nouns (singular, proper, complexity, images, wnid) VALUES (?, ?, ?, ?, ?)";
          }
        
          break;
        }
      
        case 2: // Verb
        {
          // Ignore
        
          break;
        }
      
        case 3: // Adjective
        {
          if (adjectives.count(word) == 1)
          {
            query = "INSERT INTO adjectives (base_form, complexity, comparative, superlative) VALUES (?, ?, ?, ?)";
          } else {
            query = "INSERT INTO adjectives (base_form, complexity) VALUES (?, ?)";
          }
        
          break;
        }
      
        case 4: // Adverb
        {
          if (adjectives.count(word) == 1)
          {
            query = "INSERT INTO adverbs (base_form, complexity, comparative, superlative) VALUES (?, ?, ?, ?)";
          } else {
            query = "INSERT INTO adverbs (base_form, complexity) VALUES (?, ?)";
          }
        
          break;
        }
      }
    
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_text(ppstmt, 1, word.c_str(), word.length(), SQLITE_TRANSIENT);
      switch (synset_id / 100000000)
      {
        case 1: // Noun
        {
          sqlite3_bind_int(ppstmt, 2, (std::any_of(std::begin(word), std::end(word), [] (char ch) {
            return isupper(ch);
          }) ? 1 : 0));
          
          sqlite3_bind_int(ppstmt, 3, verbly::split<std::list<std::string>>(word, " ").size());
          sqlite3_bind_int(ppstmt, 4, images[synset_id]);
          sqlite3_bind_int(ppstmt, 5, synset_id);
          
          if (nouns.count(word) == 1)
          {
            sqlite3_bind_text(ppstmt, 6, nouns[word].plural.c_str(), nouns[word].plural.length(), SQLITE_TRANSIENT);
          }
          
          break;
        }
        
        case 3: // Adjective
        case 4: // Adverb
        {
          sqlite3_bind_int(ppstmt, 2, verbly::split<std::list<std::string>>(word, " ").size());
          
          if (adjectives.count(word) == 1)
          {
            sqlite3_bind_text(ppstmt, 3, adjectives[word].comparative.c_str(), adjectives[word].comparative.length(), SQLITE_TRANSIENT);
            sqlite3_bind_text(ppstmt, 4, adjectives[word].superlative.c_str(), adjectives[word].superlative.length(), SQLITE_TRANSIENT);
          }
          
          break;
        }
      }
    
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_finalize(ppstmt);
    
      query = "SELECT last_insert_rowid()";
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      if (sqlite3_step(ppstmt) != SQLITE_ROW)
      {
        db_error(ppdb, query);
      }
    
      int rowid = sqlite3_column_int(ppstmt, 0);
      wn[synset_id][wnum] = rowid;
    
      sqlite3_finalize(ppstmt);
      
      std::string canonical(word);
      std::transform(std::begin(canonical), std::end(canonical), std::begin(canonical), ::tolower);
      if (pronunciations.count(canonical) == 1)
      {
        for (auto pronunciation : pronunciations[canonical])
        {
          switch (synset_id / 100000000)
          {
            case 1: // Noun
            {
              if (!pronunciation.rhyme.empty())
              {
                query = "INSERT INTO noun_pronunciations (noun_id, pronunciation, prerhyme, rhyme) VALUES (?, ?, ?, ?)";
              } else {
                query = "INSERT INTO noun_pronunciations (noun_id, pronunciation) VALUES (?, ?)";
              }
              
              break;
            }
            
            case 3: // Adjective
            {
              if (!pronunciation.rhyme.empty())
              {
                query = "INSERT INTO adjective_pronunciations (adjective_id, pronunciation, prerhyme, rhyme) VALUES (?, ?, ?, ?)";
              } else {
                query = "INSERT INTO adjective_pronunciations (adjective_id, pronunciation) VALUES (?, ?)";
              }
              
              break;
            }
            
            case 4: // Adverb
            {
              if (!pronunciation.rhyme.empty())
              {
                query = "INSERT INTO adverb_pronunciations (adverb_id, pronunciation, prerhyme, rhyme) VALUES (?, ?, ?, ?)";
              } else {
                query = "INSERT INTO adverb_pronunciations (adverb_id, pronunciation) VALUES (?, ?)";
              }
              
              break;
            }
          }

          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
        
          sqlite3_bind_int(ppstmt, 1, rowid);
          sqlite3_bind_text(ppstmt, 2, pronunciation.phonemes.c_str(), pronunciation.phonemes.length(), SQLITE_TRANSIENT);
          
          if (!pronunciation.rhyme.empty())
          {
            sqlite3_bind_text(ppstmt, 3, pronunciation.prerhyme.c_str(), pronunciation.prerhyme.length(), SQLITE_TRANSIENT);
            sqlite3_bind_text(ppstmt, 4, pronunciation.rhyme.c_str(), pronunciation.rhyme.length(), SQLITE_TRANSIENT);
          }
        
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
        
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // While we're working on s
  {
    progress ppgs("Writing word synonyms...", wn.size());
    for (auto sense : wn)
    {
      ppgs.update();
      
      for (auto word1 : sense.second)
      {
        for (auto word2 : sense.second)
        {
          if (word1 != word2)
          {
            std::string query;
            switch (sense.first / 100000000)
            {
              case 1: // Noun
              {
                query = "INSERT INTO noun_synonymy (noun_1_id, noun_2_id) VALUES (?, ?)";
        
                break;
              }
      
              case 2: // Verb
              {
                // Ignore
        
                break;
              }
      
              case 3: // Adjective
              {
                query = "INSERT INTO adjective_synonymy (adjective_1_id, adjective_2_id) VALUES (?, ?)";
        
                break;
              }
      
              case 4: // Adverb
              {
                query = "INSERT INTO adverb_synonymy (adverb_1_id, adverb_2_id) VALUES (?, ?)";
        
                break;
              }
            }
            
            sqlite3_stmt* ppstmt;
            if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
            {
              db_error(ppdb, query);
            }
    
            sqlite3_bind_int(ppstmt, 1, word1.second);
            sqlite3_bind_int(ppstmt, 2, word2.second);
    
            if (sqlite3_step(ppstmt) != SQLITE_DONE)
            {
              db_error(ppdb, query);
            }
    
            sqlite3_finalize(ppstmt);
          }
        }
      }
    }
  }
  
  // ant table
  {
    std::ifstream wnantfile(wnpref + "wn_ant.pl");
    if (!wnantfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnantfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing antonyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^ant\\(([134]\\d{8}),(\\d+),([134]\\d{8}),(\\d+)\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
    
      int synset_id_1 = stoi(relation_data[1]);
      int wnum_1 = stoi(relation_data[2]);
      int synset_id_2 = stoi(relation_data[3]);
      int wnum_2 = stoi(relation_data[4]);
    
      std::string query;
      switch (synset_id_1 / 100000000)
      {
        case 1: // Noun
        {
          query = "INSERT INTO noun_antonymy (noun_1_id, noun_2_id) VALUES (?, ?)";
        
          break;
        }
      
        case 2: // Verb
        {
          // Ignore
        
          break;
        }
      
        case 3: // Adjective
        {
          query = "INSERT INTO adjective_antonymy (adjective_1_id, adjective_2_id) VALUES (?, ?)";
        
          break;
        }
      
        case 4: // Adverb
        {
          query = "INSERT INTO adverb_antonymy (adverb_1_id, adverb_2_id) VALUES (?, ?)";
        
          break;
        }
      }
    
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_int(ppstmt, 1, wn[synset_id_1][wnum_1]);
      sqlite3_bind_int(ppstmt, 2, wn[synset_id_2][wnum_2]);
    
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_finalize(ppstmt);
    }
  }
  
  // at table
  {
    std::ifstream wnatfile(wnpref + "wn_at.pl");
    if (!wnatfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnatfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing variations...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^at\\((1\\d{8}),(3\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO variation (noun_id, adjective_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // der table
  {
    std::ifstream wnderfile(wnpref + "wn_der.pl");
    if (!wnderfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }
    
    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnderfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing morphological derivation...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^der\\(([134]\\d{8}),(\\d+),([134]\\d{8}),(\\d+)\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int wnum_1 = stoi(relation_data[2]);
      int synset_id_2 = stoi(relation_data[3]);
      int wnum_2 = stoi(relation_data[4]);
      std::string query;
      switch (synset_id_1 / 100000000)
      {
        case 1: // Noun
        {
          switch (synset_id_2 / 100000000)
          {
            case 1: // Noun
            {
              query = "INSERT INTO noun_noun_derivation (noun_1_id, noun_2_id) VALUES (?, ?)";
              break;
            }
            
            case 3: // Adjective
            {
              query = "INSERT INTO noun_adjective_derivation (noun_id, adjective_id) VALUES (?, ?)";
              break;
            }
            
            case 4: // Adverb
            {
              query = "INSERT INTO noun_adverb_derivation (noun_id, adverb_id) VALUES (?, ?)";
              break;
            }
          }
          
          break;
        }
        
        case 3: // Adjective
        {
          switch (synset_id_2 / 100000000)
          {
            case 1: // Noun
            {
              query = "INSERT INTO noun_adjective_derivation (adjective_id, noun_id) VALUES (?, ?)";
              break;
            }
            
            case 3: // Adjective
            {
              query = "INSERT INTO adjective_adjective_derivation (adjective_id, adjective_id) VALUES (?, ?)";
              break;
            }
            
            case 4: // Adverb
            {
              query = "INSERT INTO adjective_adverb_derivation (adjective_id, adverb_id) VALUES (?, ?)";
              break;
            }
          }
          
          break;
        }
        
        case 4: // Adverb
        {
          switch (synset_id_2 / 100000000)
          {
            case 1: // Noun
            {
              query = "INSERT INTO noun_adverb_derivation (adverb_id, noun_id) VALUES (?, ?)";
              break;
            }
            
            case 3: // Adjective
            {
              query = "INSERT INTO adjective_adverb_derivation (adverb_id, adjective_id) VALUES (?, ?)";
              break;
            }
            
            case 4: // Adverb
            {
              query = "INSERT INTO adverb_adverb_derivation (adverb_1_id, adverb_2_id) VALUES (?, ?)";
              break;
            }
          }
          
          break;
        }
      }
      
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_bind_int(ppstmt, 1, wn[synset_id_1][wnum_1]);
      sqlite3_bind_int(ppstmt, 2, wn[synset_id_2][wnum_2]);
      
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_finalize(ppstmt);
    }
  }
  
  // hyp table
  {
    std::ifstream wnhypfile(wnpref + "wn_hyp.pl");
    if (!wnhypfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnhypfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing hypernyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^hyp\\((1\\d{8}),(1\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO hypernymy (hyponym_id, hypernym_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // ins table
  {
    std::ifstream wninsfile(wnpref + "wn_ins.pl");
    if (!wninsfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wninsfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing instantiations...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^ins\\((1\\d{8}),(1\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO instantiation (instance_id, class_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // mm table
  {
    std::ifstream wnmmfile(wnpref + "wn_mm.pl");
    if (!wnmmfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnmmfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing member meronyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^mm\\((1\\d{8}),(1\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO member_meronymy (holonym_id, meronym_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // ms table
  {
    std::ifstream wnmsfile(wnpref + "wn_ms.pl");
    if (!wnmsfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnmsfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing substance meronyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^ms\\((1\\d{8}),(1\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO substance_meronymy (holonym_id, meronym_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // mm table
  {
    std::ifstream wnmpfile(wnpref + "wn_mp.pl");
    if (!wnmpfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnmpfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing part meronyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^mp\\((1\\d{8}),(1\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO part_meronymy (holonym_id, meronym_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // per table
  {
    std::ifstream wnperfile(wnpref + "wn_per.pl");
    if (!wnperfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnperfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing pertainyms and mannernyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^per\\(([34]\\d{8}),(\\d+),([13]\\d{8}),(\\d+)\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int wnum_1 = stoi(relation_data[2]);
      int synset_id_2 = stoi(relation_data[3]);
      int wnum_2 = stoi(relation_data[4]);
      std::string query;
      switch (synset_id_1 / 100000000)
      {
        case 3: // Adjective
        {
          // This is a pertainym, the second word should be a noun
          // Technically it can be an adjective but we're ignoring that
          if (synset_id_2 / 100000000 != 1)
          {
            continue;
          }
          
          query = "INSERT INTO pertainymy (pertainym_id, noun_id) VALUES (?, ?)";
          
          break;
        }
        
        case 4: // Adverb
        {
          // This is a mannernym, the second word should be an adjective
          if (synset_id_2 / 100000000 != 3)
          {
            continue;
          }
          
          query = "INSERT INTO mannernymy (mannernym_id, adjective_id) VALUES (?, ?)";
          
          break;
        }
      }
      
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_int(ppstmt, 1, wn[synset_id_1][wnum_1]);
      sqlite3_bind_int(ppstmt, 2, wn[synset_id_2][wnum_2]);
    
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_finalize(ppstmt);
    }
  }
  
  // sa table
  {
    std::ifstream wnsafile(wnpref + "wn_sa.pl");
    if (!wnsafile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnsafile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing specifications...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^per\\((3\\d{8}),(\\d+),(3\\d{8}),(\\d+)\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int wnum_1 = stoi(relation_data[2]);
      int synset_id_2 = stoi(relation_data[3]);
      int wnum_2 = stoi(relation_data[4]);
      std::string query("INSERT INTO specification (general_id, specific_id) VALUES (?, ?)");
      
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_int(ppstmt, 1, wn[synset_id_1][wnum_1]);
      sqlite3_bind_int(ppstmt, 2, wn[synset_id_2][wnum_2]);
    
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_finalize(ppstmt);
    }
  }

  // sim table
  {
    std::ifstream wnsimfile(wnpref + "wn_sim.pl");
    if (!wnsimfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnsimfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing sense synonyms...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^sim\\((3\\d{8}),(3\\d{8})\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id_1 = stoi(relation_data[1]);
      int synset_id_2 = stoi(relation_data[2]);
      std::string query("INSERT INTO adjective_synonymy (adjective_1_id, adjective_2_id) VALUES (?, ?)");
      
      for (auto mapping1 : wn[synset_id_1])
      {
        for (auto mapping2 : wn[synset_id_2])
        {
          sqlite3_stmt* ppstmt;
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, mapping1.second);
          sqlite3_bind_int(ppstmt, 2, mapping2.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_reset(ppstmt);
          sqlite3_clear_bindings(ppstmt);
          
          sqlite3_bind_int(ppstmt, 1, mapping2.second);
          sqlite3_bind_int(ppstmt, 2, mapping1.second);
          
          if (sqlite3_step(ppstmt) != SQLITE_DONE)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_finalize(ppstmt);
        }
      }
    }
  }
  
  // syntax table
  {
    std::ifstream wnsyntaxfile(wnpref + "wn_syntax.pl");
    if (!wnsyntaxfile.is_open())
    {
      std::cout << "Invalid WordNet data directory." << std::endl;
      print_usage();
    }

    std::list<std::string> lines;
    for (;;)
    {
      std::string line;
      if (!getline(wnsyntaxfile, line))
      {
        break;
      }
    
      if (line.back() == '\r')
      {
        line.pop_back();
      }
      
      lines.push_back(line);
    }
    
    progress ppgs("Writing adjective syntax markers...", lines.size());
    for (auto line : lines)
    {
      ppgs.update();
      
      std::regex relation("^syntax\\((3\\d{8}),(\\d+),([ipa])p?\\)\\.");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
      
      int synset_id = stoi(relation_data[1]);
      int wnum = stoi(relation_data[2]);
      std::string syn = relation_data[3];
      std::string query("UPDATE adjectives SET position = ? WHERE adjective_id = ?");
      
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.size(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_bind_text(ppstmt, 1, syn.c_str(), 1, SQLITE_TRANSIENT);
      sqlite3_bind_int(ppstmt, 2, wn[synset_id][wnum]);
      
      if (sqlite3_step(ppstmt) != SQLITE_DONE)
      {
        db_error(ppdb, query);
      }
      
      sqlite3_finalize(ppstmt);
    }
  }
  
  sqlite3_close_v2(ppdb);
  
  std::cout << "Done." << std::endl;
}
