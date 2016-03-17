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
#include "progress.h"

struct verb {
  std::string infinitive;
  std::string past_tense;
  std::string past_participle;
  std::string ing_form;
  std::string s_form;
};

struct adjective {
  std::string base;
  std::string comparative;
  std::string superlative;
};

struct noun {
  std::string singular;
  std::string plural;
};

struct group {
  std::string id;
  std::set<std::string> members;
};

std::map<std::string, group> groups;
std::map<std::string, verb> verbs;
std::map<std::string, adjective> adjectives;
std::map<std::string, noun> nouns;
std::map<int, std::map<int, int>> wn;
std::map<std::string, std::set<std::string>> pronunciations;

void print_usage()
{
  std::cout << "Verbly Datafile Generator" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "Requires exactly six arguments." << std::endl;
  std::cout << "1. The path to a VerbNet data directory." << std::endl;
  std::cout << "2. The path to a SemLink vnpbMappings file." << std::endl;
  std::cout << "3. The path to an AGID infl.txt file." << std::endl;
  std::cout << "4. The path to a WordNet prolog data directory." << std::endl;
  std::cout << "5. The path to a CMUDICT pronunciation file." << std::endl;
  std::cout << "6. Datafile output path." << std::endl;
  
  exit(1);
}

void db_error(sqlite3* ppdb, std::string)
{
  std::cout << "Error writing to output database: " << sqlite3_errmsg(ppdb) << std::endl;
  sqlite3_close_v2(ppdb);
  print_usage();
}

/*
void parse_group(xmlNodePtr top, std::string filename)
{
  xmlChar* key = xmlGetProp(top, (xmlChar*) "ID");
  if (key == 0)
  {
    std::cout << "Bad VerbNet file format: " << filename << std::endl;
    print_usage();
  }
  std::string vnid = key;
  vnid = vnid.substr(vnid.find_first_of("-")+1);
  xmlFree(key);
  
  group g;
  g.id = vnid;
  
  for (xmlNodePtr node = top->xmlChildrenNode; node != nullptr; node = node->next)
  {
    if (!xmlStrcmp(node->name, (const xmlChar*) "MEMBERS"))
    {
      for (xmlNodePtr member = node->xmlChildrenNode; member != nullptr; member = member->next)
      {
        if (!xmlStrcmp(member->name, (const xmlChar*) "MEMBER"))
        {
          key = xmlGetProp(member, (xmlChar*) "name");
          g.members.insert(key);
          xmlFree(key);
        }
      }
    } else if (!xmlStrcmp(node->name, (const xmlChar*) "FRAMES"))
    {
      for (xmlNodePtr frame = node->xmlChildrenNode; frame != nullptr; frame = frame->next)
      {
        if (!xmlStrcmp(frame->name, (const xmlChar*) "FRAME"))
        {
          for (xmlNodePtr framenode = frame->xmlChildrenNode; framenode != nullptr; framenode = framenode->next)
          {
            
          }
        }
      }
    }
  }
}*/

int main(int argc, char** argv)
{
  if (argc != 7)
  {
    print_usage();
  }
  
  /*DIR* dir;
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
  
  closedir(dir);*/
  
  // Get verbs from AGID
  std::cout << "Reading inflections..." << std::endl;
  
  std::ifstream agidfile(argv[3]);
  if (!agidfile.is_open())
  {
    std::cout << "Could not open AGID file: " << argv[3] << std::endl;
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
        verb v;
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
        adjective adj;
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
        noun n;
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
  
  std::ifstream pronfile(argv[5]);
  if (!pronfile.is_open())
  {
    std::cout << "Could not open CMUDICT file: " << argv[5] << std::endl;
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
      
      pronunciations[canonical].insert(phoneme_data[2]);
    }
  }
  
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
    
      sqlite3_bind_text(ppstmt, 1, mapping.second.infinitive.c_str(), mapping.second.infinitive.length(), SQLITE_STATIC);
      sqlite3_bind_text(ppstmt, 2, mapping.second.past_tense.c_str(), mapping.second.past_tense.length(), SQLITE_STATIC);
      sqlite3_bind_text(ppstmt, 3, mapping.second.past_participle.c_str(), mapping.second.past_participle.length(), SQLITE_STATIC);
      sqlite3_bind_text(ppstmt, 4, mapping.second.ing_form.c_str(), mapping.second.ing_form.length(), SQLITE_STATIC);
      sqlite3_bind_text(ppstmt, 5, mapping.second.s_form.c_str(), mapping.second.s_form.length(), SQLITE_STATIC);
    
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
        
        for (auto pronunciation : pronunciations[canonical])
        {
          query = "INSERT INTO verb_pronunciations (verb_id, pronunciation) VALUES (?, ?)";
          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
          
          sqlite3_bind_int(ppstmt, 1, rowid);
          sqlite3_bind_text(ppstmt, 2, pronunciation.c_str(), pronunciation.length(), SQLITE_STATIC);
          
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
  std::string wnpref {argv[4]};
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
      
      std::regex relation("^s\\(([134]\\d{8}),(\\d+),'([\\w ]+)',");
      std::smatch relation_data;
      if (!std::regex_search(line, relation_data, relation))
      {
        continue;
      }
    
      int synset_id = stoi(relation_data[1]);
      int wnum = stoi(relation_data[2]);
      std::string word = relation_data[3];
    
      std::string query;
      switch (synset_id / 100000000)
      {
        case 1: // Noun
        {
          if (nouns.count(word) == 1)
          {
            query = "INSERT INTO nouns (singular, plural) VALUES (?, ?)";
          } else {
            query = "INSERT INTO nouns (singular) VALUES (?)";
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
            query = "INSERT INTO adjectives (base_form, comparative, superlative) VALUES (?, ?, ?)";
          } else {
            query = "INSERT INTO adjectives (base_form) VALUES (?)";
          }
        
          break;
        }
      
        case 4: // Adverb
        {
          if (adjectives.count(word) == 1)
          {
            query = "INSERT INTO adverbs (base_form, comparative, superlative) VALUES (?, ?, ?)";
          } else {
            query = "INSERT INTO adverbs (base_form) VALUES (?)";
          }
        
          break;
        }
      }
    
      sqlite3_stmt* ppstmt;
      if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        db_error(ppdb, query);
      }
    
      sqlite3_bind_text(ppstmt, 1, word.c_str(), word.length(), SQLITE_STATIC);
      switch (synset_id / 100000000)
      {
        case 1: // Noun
        {
          if (nouns.count(word) == 1)
          {
            sqlite3_bind_text(ppstmt, 2, nouns[word].plural.c_str(), nouns[word].plural.length(), SQLITE_STATIC);
          }
          
          break;
        }
        
        case 3: // Adjective
        case 4: // Adverb
        {
          if (adjectives.count(word) == 1)
          {
            sqlite3_bind_text(ppstmt, 2, adjectives[word].comparative.c_str(), adjectives[word].comparative.length(), SQLITE_STATIC);
            sqlite3_bind_text(ppstmt, 3, adjectives[word].superlative.c_str(), adjectives[word].superlative.length(), SQLITE_STATIC);
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
              query = "INSERT INTO noun_pronunciations (noun_id, pronunciation) VALUES (?, ?)";
              
              break;
            }
            
            case 3: // Adjective
            {
              query = "INSERT INTO adjective_pronunciations (adjective_id, pronunciation) VALUES (?, ?)";
              
              break;
            }
            
            case 4: // Adverb
            {
              query = "INSERT INTO adverb_pronunciations (adverb_id, pronunciation) VALUES (?, ?)";
              
              break;
            }
          }

          if (sqlite3_prepare_v2(ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
          {
            db_error(ppdb, query);
          }
        
          sqlite3_bind_int(ppstmt, 1, rowid);
          sqlite3_bind_text(ppstmt, 2, pronunciation.c_str(), pronunciation.length(), SQLITE_STATIC);
        
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
      
      sqlite3_bind_text(ppstmt, 1, syn.c_str(), 1, SQLITE_STATIC);
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
