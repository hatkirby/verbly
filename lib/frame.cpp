#include "verbly.h"

namespace verbly {
  
  frame::selrestr::type frame::selrestr::get_type() const
  {
    return _type;
  }
  
  frame::selrestr::selrestr(const selrestr& other)
  {
    _type = other._type;
    
    switch (_type)
    {
      case frame::selrestr::type::singleton:
      {
        _singleton.pos = other._singleton.pos;
        new(&_singleton.restriction) std::string(other._singleton.restriction);
        
        break;
      }
      
      case frame::selrestr::type::group:
      {
        new(&_group.children) std::list<selrestr>(other._group.children);
        _group.orlogic = other._group.orlogic;
        
        break;
      }
      
      case frame::selrestr::type::empty:
      {
        // Nothing!
        
        break;
      }
    }
  }
  
  frame::selrestr::~selrestr()
  {
    switch (_type)
    {
      case frame::selrestr::type::singleton:
      {
        using string_type = std::string;
        _singleton.restriction.~string_type();
        
        break;
      }
      
      case frame::selrestr::type::group:
      {
        using list_type = std::list<selrestr>;
        _group.children.~list_type();
        
        break;
      }
      
      case frame::selrestr::type::empty:
      {
        // Nothing!
        
        break;
      }
    }
  }
  
  frame::selrestr& frame::selrestr::operator=(const selrestr& other)
  {
    this->~selrestr();
    
    _type = other._type;
    
    switch (_type)
    {
      case frame::selrestr::type::singleton:
      {
        _singleton.pos = other._singleton.pos;
        new(&_singleton.restriction) std::string(other._singleton.restriction);
        
        break;
      }
      
      case frame::selrestr::type::group:
      {
        new(&_group.children) std::list<selrestr>(other._group.children);
        _group.orlogic = other._group.orlogic;
        
        break;
      }
      
      case frame::selrestr::type::empty:
      {
        // Nothing!
        
        break;
      }
    }
    
    return *this;
  }
  
  frame::selrestr::selrestr() : _type(frame::selrestr::type::empty)
  {
    
  }
  
  frame::selrestr::selrestr(std::string restriction, bool pos) : _type(frame::selrestr::type::singleton)
  {
    new(&_singleton.restriction) std::string(restriction);
    _singleton.pos = pos;
  }
  
  std::string frame::selrestr::get_restriction() const
  {
    assert(_type == frame::selrestr::type::singleton);
    
    return _singleton.restriction;
  }
  
  bool frame::selrestr::get_pos() const
  {
    assert(_type == frame::selrestr::type::singleton);
    
    return _singleton.pos;
  }
  
  frame::selrestr::selrestr(std::list<selrestr> children, bool orlogic) : _type(frame::selrestr::type::group)
  {
    new(&_group.children) std::list<selrestr>(children);
    _group.orlogic = orlogic;
  }
  
  std::list<frame::selrestr> frame::selrestr::get_children() const
  {
    assert(_type == frame::selrestr::type::group);
    
    return _group.children;
  }
  
  std::list<frame::selrestr>::const_iterator frame::selrestr::begin() const
  {
    assert(_type == frame::selrestr::type::group);
    
    return _group.children.begin();
  }
  
  std::list<frame::selrestr>::const_iterator frame::selrestr::end() const
  {
    assert(_type == frame::selrestr::type::group);
    
    return _group.children.end();
  }
  
  bool frame::selrestr::get_orlogic() const
  {
    assert(_type == frame::selrestr::type::group);
    
    return _group.orlogic;
  }
  
  frame::part::type frame::part::get_type() const
  {
    return _type;
  }
  
  frame::part::part()
  {
    
  }
  
  frame::part::part(const part& other)
  {
    _type = other._type;
    
    switch (_type)
    {
      case frame::part::type::noun_phrase:
      {
        new(&_noun_phrase.role) std::string(other._noun_phrase.role);
        new(&_noun_phrase.selrestrs) selrestr(other._noun_phrase.selrestrs);
        new(&_noun_phrase.synrestrs) std::set<std::string>(other._noun_phrase.synrestrs);
        
        break;
      }
      
      case frame::part::type::literal_preposition:
      {
        new(&_literal_preposition.choices) std::vector<std::string>(other._literal_preposition.choices);
        
        break;
      }
      
      case frame::part::type::selection_preposition:
      {
        new(&_selection_preposition.preprestrs) std::vector<std::string>(other._selection_preposition.preprestrs);
        
        break;
      }
      
      case frame::part::type::literal:
      {
        new(&_literal.lexval) std::string(other._literal.lexval);
        
        break;
      }
      
      default:
      {
        // Nothing!
        
        break;
      }
    }
  }
  
  frame::part::~part()
  {
    switch (_type)
    {
      case frame::part::type::noun_phrase:
      {
        using string_type = std::string;
        using set_type = std::set<std::string>;
        
        _noun_phrase.role.~string_type();
        _noun_phrase.selrestrs.~selrestr();
        _noun_phrase.synrestrs.~set_type();
        
        break;
      }
      
      case frame::part::type::literal_preposition:
      {
        using vector_type = std::vector<std::string>;
        _literal_preposition.choices.~vector_type();
        
        break;
      }
      
      case frame::part::type::selection_preposition:
      {
        using vector_type = std::vector<std::string>;
        _selection_preposition.preprestrs.~vector_type();
        
        break;
      }
      
      case frame::part::type::literal:
      {
        using string_type = std::string;
        _literal.lexval.~string_type();
        
        break;
      }
      
      default:
      {
        // Nothing!
        
        break;
      }
    }
  }
  
  std::string frame::part::get_role() const
  {
    assert(_type == frame::part::type::noun_phrase);
    
    return _noun_phrase.role;
  }
  
  frame::selrestr frame::part::get_selrestrs() const
  {
    assert(_type == frame::part::type::noun_phrase);
    
    return _noun_phrase.selrestrs;
  }
  
  std::set<std::string> frame::part::get_synrestrs() const
  {
    assert(_type == frame::part::type::noun_phrase);
    
    return _noun_phrase.synrestrs;
  }
  
  std::vector<std::string> frame::part::get_choices() const
  {
    assert(_type == frame::part::type::literal_preposition);
    
    return _literal_preposition.choices;
  }
  
  std::vector<std::string> frame::part::get_preprestrs() const
  {
    assert(_type == frame::part::type::selection_preposition);
    
    return _selection_preposition.preprestrs;
  }
  
  std::string frame::part::get_literal() const
  {
    assert(_type == frame::part::type::literal);
    
    return _literal.lexval;
  }
  
  std::vector<frame::part> frame::parts() const
  {
    return _parts;
  }
  
  std::map<std::string, frame::selrestr> frame::roles() const
  {
    return _roles;
  }
  
};
