#ifndef GROUP_H_BD6933C0
#define GROUP_H_BD6933C0

#include <stdexcept>
#include <list>
#include <vector>
#include "field.h"
#include "filter.h"

struct sqlite3_stmt;

namespace verbly {
  
  class database;
  class frame;
  
  class group {
  public:
    
    // Default constructor
    
    group() = default;
    
    // Construct from database
    
    group(const database& db, sqlite3_stmt* row);
    
    // Accessors
    
    operator bool() const
    {
      return valid_;
    }
    
    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized group");
      }
      
      return id_;
    }
    
    const std::vector<frame>& getFrames() const;
    
    // Type info
    
    static const object objectType;
    
    static const std::list<std::string> select;
    
    // Query fields
    
    static const field id;
    
    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized group");
      }
      
      return (id == id_);
    }
    
    // Relationships to other objects
    
    static const field frame;
    
    static const field word;
    
  private:
    bool valid_ = false;
    
    int id_;
    
    const database* db_;
    
    mutable bool initializedFrames_ = false;
    mutable std::vector<class frame> frames_;
    
  };
  
};

#endif /* end of include guard: GROUP_H_BD6933C0 */
