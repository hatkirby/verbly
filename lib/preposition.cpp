#include "verbly.h"

namespace verbly {
  
  std::string preposition::get_form() const
  {
    return form;
  }
  
  preposition_query::preposition_query(const data& _data) : _data(_data)
  {
    
  }
  
  preposition_query& preposition_query::limit(int _limit)
  {
    this->_limit = _limit;
    
    return *this;
  }
  
  preposition_query& preposition_query::random()
  {
    _random = true;
    
    return *this;
  }
  
  preposition_query& preposition_query::in_group(std::string _arg)
  {
    _in_group.push_back(_arg);
    
    return *this;
  }
  
  std::list<preposition> preposition_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT form FROM prepositions";
    
    if (!_in_group.empty())
    {
      std::list<std::string> clauses(_in_group.size(), "groupname = @GNAME");
      construct << " WHERE preposition_id IN (SELECT preposition_id FROM preposition_groups WHERE ";
      construct << verbly::implode(std::begin(clauses), std::end(clauses), " OR ");
      construct << ")";
    }
    
    if (_random)
    {
      construct << " ORDER BY RANDOM()";
    }
    
    if (_limit != unlimited)
    {
      construct << " LIMIT " << _limit;
    }
    
    sqlite3_stmt* ppstmt;
    std::string query = construct.str();
    if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
    }
    
    for (auto& group : _in_group)
    {
      sqlite3_bind_text(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@GNAME"), group.c_str(), group.length(), SQLITE_STATIC);
    }
    
    std::list<preposition> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      preposition pp;
      pp.form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 0)));
      
      output.push_back(pp);
    }
    
    return output;
  }

};
