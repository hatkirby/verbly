#include "verbly.h"
#include <json.hpp>

using json = nlohmann::json;

namespace verbly {

  frame_query::frame_query(const data& _data) : _data(_data)
  {
    
  }
  
  frame_query& frame_query::for_verb(const verb& _v)
  {
    _for_verb.push_back(_v);
    
    return *this;
  }
  
  frame::selrestr parse_selrestr(const json data)
  {
    if (data.find("children") != data.end())
    {
      std::list<frame::selrestr> children;
      std::transform(std::begin(data["children"]), std::end(data["children"]), std::back_inserter(children), &parse_selrestr);
      
      return frame::selrestr{children, data["logic"] == "or"};
    } else if (data.find("type") != data.end())
    {
      return frame::selrestr{data["type"].get<std::string>(), data["pos"].get<bool>()};
    } else {
      return frame::selrestr{};
    }
  }
  
  std::list<frame> frame_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT frames.data, groups.data FROM frames INNER JOIN groups ON frames.group_id = groups.group_id";
    
    if (!_for_verb.empty())
    {
      std::list<std::string> clauses(_for_verb.size(), "verb_id = @VERID");
      construct << " WHERE frames.group_id IN (SELECT group_id FROM verb_groups WHERE ";
      construct << verbly::implode(std::begin(clauses), std::end(clauses), " OR ");
      construct << ")";
    }
    
    sqlite3_stmt* ppstmt;
    std::string query = construct.str();
    if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
    }
    
    for (auto verb : _for_verb)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@VERID"), verb._id);
    }
    
    std::list<frame> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      frame f;
      
      std::string fdatat(reinterpret_cast<const char*>(sqlite3_column_blob(ppstmt, 0)));
      const json fdata = json::parse(fdatat);
      for (const auto& part : fdata)
      {
        frame::part p;
        
        if (part["type"] == "np")
        {
          p._type = frame::part::type::noun_phrase;
          new(&p._noun_phrase.role) std::string(part["role"].get<std::string>());
          new(&p._noun_phrase.selrestrs) frame::selrestr(parse_selrestr(part["selrestrs"]));
          new(&p._noun_phrase.synrestrs) std::set<std::string>();
          for (auto synrestr : part["synrestrs"])
          {
            p._noun_phrase.synrestrs.insert(synrestr.get<std::string>());
          }
        } else if (part["type"] == "pp")
        {
          if (!part["values"].empty())
          {
            p._type = frame::part::type::literal_preposition;
            new(&p._literal_preposition.choices) std::vector<std::string>();
            for (auto choice : part["values"])
            {
              p._literal_preposition.choices.push_back(choice.get<std::string>());
            }
          } else if (!part["preprestrs"].empty())
          {
            p._type = frame::part::type::selection_preposition;
            new(&p._selection_preposition.preprestrs) std::vector<std::string>();
            for (auto preprestr : part["preprestrs"])
            {
              p._selection_preposition.preprestrs.push_back(preprestr.get<std::string>());
            }
          }
        } else if (part["type"] == "v")
        {
          p._type = frame::part::type::verb;
        } else if (part["type"] == "adj")
        {
          p._type = frame::part::type::adjective;
        } else if (part["type"] == "adv")
        {
          p._type = frame::part::type::adverb;
        } else if (part["type"] == "lex")
        {
          p._type = frame::part::type::literal;
          new(&p._literal.lexval) std::string(part["value"].get<std::string>());
        }
        
        f._parts.push_back(p);
      }
      
      std::string rdatat(reinterpret_cast<const char*>(sqlite3_column_blob(ppstmt, 1)));
      const json rdata = json::parse(rdatat);
      for (const auto& role : rdata)
      {
        std::string rt = role["type"];
        frame::selrestr rs;
        
        if (role.find("selrestrs") != role.end())
        {
          rs = parse_selrestr(role["selrestrs"]);
        }
        
        f._roles[rt] = rs;
      }
      
      output.push_back(f);
    }
    
    sqlite3_finalize(ppstmt);
    
    return output;
  }

};
