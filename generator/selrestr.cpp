#include "selrestr.h"

namespace verbly {
  namespace generator {
    
    selrestr::selrestr(const selrestr& other)
    {
      type_ = other.type_;
      
      switch (type_)
      {
        case type::singleton:
        {
          singleton_.pos = other.singleton_.pos;
          new(&singleton_.restriction) std::string(other.singleton_.restriction);
          
          break;
        }
        
        case type::group:
        {
          new(&group_.children) std::list<selrestr>(other.group_.children);
          group_.orlogic = other.group_.orlogic;
          
          break;
        }
        
        case type::empty:
        {
          break;
        }
      }
    }
    
    selrestr::selrestr(selrestr&& other) : selrestr()
    {
      swap(*this, other);
    }
    
    selrestr& selrestr::operator=(selrestr other)
    {
      swap(*this, other);
      
      return *this;
    }
    
    void swap(selrestr& first, selrestr& second)
    {
      using type = selrestr::type;
      
      type tempType = first.type_;
      int tempPos;
      std::string tempRestriction;
      std::list<selrestr> tempChildren;
      bool tempOrlogic;
      
      switch (tempType)
      {
        case type::singleton:
        {
          tempPos = first.singleton_.pos;
          tempRestriction = std::move(first.singleton_.restriction);
          
          break;
        }
        
        case type::group:
        {
          tempChildren = std::move(first.group_.children);
          tempOrlogic = first.group_.orlogic;
          
          break;
        }
        
        case type::empty:
        {
          break;
        }
      }
      
      first.~selrestr();
      
      first.type_ = second.type_;
      
      switch (first.type_)
      {
        case type::singleton:
        {
          first.singleton_.pos = second.singleton_.pos;
          new(&first.singleton_.restriction) std::string(std::move(second.singleton_.restriction));
          
          break;
        }
        
        case type::group:
        {
          new(&first.group_.children) std::list<selrestr>(std::move(second.group_.children));
          first.group_.orlogic = second.group_.orlogic;
          
          break;
        }
        
        case type::empty:
        {
          break;
        }
      }
      
      second.~selrestr();
      
      second.type_ = tempType;
      
      switch (second.type_)
      {
        case type::singleton:
        {
          second.singleton_.pos = tempPos;
          new(&second.singleton_.restriction) std::string(std::move(tempRestriction));
          
          break;
        }
        
        case type::group:
        {
          new(&second.group_.children) std::list<selrestr>(std::move(tempChildren));
          second.group_.orlogic = tempOrlogic;
          
          break;
        }
        
        case type::empty:
        {
          break;
        }
      }
    }
    
    selrestr::~selrestr()
    {
      switch (type_)
      {
        case type::singleton:
        {
          using string_type = std::string;
          singleton_.restriction.~string_type();
          
          break;
        }
        
        case type::group:
        {
          using list_type = std::list<selrestr>;
          group_.children.~list_type();
          
          break;
        }
        
        case type::empty:
        {
          break;
        }
      }
    }
    
    selrestr::selrestr() : type_(type::empty)
    {
    }
    
    selrestr::selrestr(
      std::string restriction,
      bool pos) :
        type_(type::singleton)
    {
      new(&singleton_.restriction) std::string(std::move(restriction));
      singleton_.pos = pos;
    }
    
    std::string selrestr::getRestriction() const
    {
      if (type_ == type::singleton)
      {
        return singleton_.restriction;
      } else {
        throw std::domain_error("Only singleton selrestrs have restrictions");
      }
    }
    
    bool selrestr::getPos() const
    {
      if (type_ == type::singleton)
      {
        return singleton_.pos;
      } else {
        throw std::domain_error("Only singleton selrestrs have positivity flags");
      }
    }
    
    selrestr::selrestr(
      std::list<selrestr> children,
      bool orlogic) :
        type_(type::group)
    {
      new(&group_.children) std::list<selrestr>(std::move(children));
      group_.orlogic = orlogic;
    }
    
    std::list<selrestr> selrestr::getChildren() const
    {
      if (type_ == type::group)
      {
        return group_.children;
      } else {
        throw std::domain_error("Only group selrestrs have children");
      }
    }
    
    std::list<selrestr>::const_iterator selrestr::begin() const
    {
      if (type_ == type::group)
      {
        return std::begin(group_.children);
      } else {
        throw std::domain_error("Only group selrestrs have children");
      }
    }
    
    std::list<selrestr>::const_iterator selrestr::end() const
    {
      if (type_ == type::group)
      {
        return std::end(group_.children);
      } else {
        throw std::domain_error("Only group selrestrs have children");
      }
    }
    
    bool selrestr::getOrlogic() const
    {
      if (type_ == type::group)
      {
        return group_.orlogic;
      } else {
        throw std::domain_error("Only group selrestrs have logic");
      }
    }
    
    nlohmann::json selrestr::toJson() const
    {
      switch (type_)
      {
        case type::empty:
        {
          return {};
        }
        
        case type::singleton:
        {
          return {
            {"type", singleton_.restriction},
            {"pos", singleton_.pos}
          };
        }
        
        case type::group:
        {
          std::string logic;
          if (group_.orlogic)
          {
            logic = "or";
          } else {
            logic = "and";
          }
          
          std::list<nlohmann::json> children;
          std::transform(std::begin(group_.children), std::end(group_.children), std::back_inserter(children), [] (const selrestr& child) {
            return child.toJson();
          });
          
          return {
            {"logic", logic},
            {"children", children}
          };
        }
      }
    }
    
  };
};
