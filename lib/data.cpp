#include "verbly.h"

namespace verbly {

  data::data(std::string datafile)
  {
    if (sqlite3_open_v2(datafile.c_str(), &ppdb, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
    {
      throw std::invalid_argument(sqlite3_errmsg(ppdb));
    }
  }

  data::data(data&& other)
  {
    ppdb = other.ppdb;
  }

  data& data::operator=(data&& other)
  {
    ppdb = other.ppdb;
  
    return *this;
  }

  data::~data()
  {
    sqlite3_close_v2(ppdb);
  }

  verb_query data::verbs() const
  {
    return verb_query(*this);
  }

  adjective_query data::adjectives() const
  {
    return adjective_query(*this);
  }

  adverb_query data::adverbs() const
  {
    return adverb_query(*this);
  }

  noun_query data::nouns() const
  {
    return noun_query(*this);
  }
  
  frame_query data::frames() const
  {
    return frame_query(*this);
  }
  
  preposition_query data::prepositions() const
  {
    return preposition_query(*this);
  }
  
  binding::type binding::get_type() const
  {
    return _type;
  }
  
  binding::binding(const binding& other)
  {
    _type = other._type;
    
    switch (_type)
    {
      case type::integer:
      {
        _integer = other._integer;
        
        break;
      }
      
      case type::string:
      {
        new(&_string) std::string(other._string);
        
        break;
      }
    }
  }
  
  binding::~binding()
  {
    switch (_type)
    {
      case type::string:
      {
        using string_type = std::string;
        _string.~string_type();
        
        break;
      }
    }
  }
  
  binding& binding::operator=(const binding& other)
  {
    this->~binding();
    
    _type = other._type;
    
    switch (_type)
    {
      case type::integer:
      {
        _integer = other._integer;
        
        break;
      }
      
      case type::string:
      {
        new(&_string) std::string(other._string);
        
        break;
      }
    }
    
    return *this;
  }
  
  binding::binding(int _arg)
  {
    _type = type::integer;
    _integer = _arg;
  }
  
  int binding::get_integer() const
  {
    assert(_type == type::integer);
    
    return _integer;
  }
  
  void binding::set_integer(int _arg)
  {
    *this = binding(_arg);
  }
  
  binding& binding::operator=(int _arg)
  {
    *this = binding(_arg);
    
    return *this;
  }
  
  binding::binding(std::string _arg)
  {
    _type = type::string;
    new(&_string) std::string(_arg);
  }
  
  std::string binding::get_string() const
  {
    assert(_type == type::string);
    
    return _string;
  }
  
  void binding::set_string(std::string _arg)
  {
    *this = binding(_arg);
  }
  
  binding& binding::operator=(std::string _arg)
  {
    *this = binding(_arg);
    
    return *this;
  }
  
};
