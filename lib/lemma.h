#ifndef LEMMA_H_0A180D30
#define LEMMA_H_0A180D30

#include <stdexcept>
#include <vector>
#include <list>
#include <map>
#include "field.h"
#include "form.h"
#include "enums.h"
#include "filter.h"

struct sqlite3_stmt;

namespace verbly {
  
  class database;
  
  class lemma {
  public:
    
    // Default constructor
    
    lemma() = default;
    
    // Construct from database
    
    lemma(const database& db, sqlite3_stmt* row);
    
    // Accessors
    
    operator bool() const
    {
      return valid_;
    }
    
    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized lemma");
      }
      
      return id_;
    }
    
    const form& getBaseForm() const;
    
    bool hasInflection(inflection category) const;
    
    const std::vector<form>& getInflections(inflection category) const;
    
    // Type info
    
    static const object objectType;
    
    static const std::list<std::string> select;
    
    // Query fields
    
    static const field id;
    
    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized lemma");
      }
      
      return (id == id_);
    }
    
    // Relationships to other objects
    
    static const field word;
    
    class inflection_field {
    public:
      
      inflection_field(inflection category) : category_(category)
      {
      }
      
      const inflection getCategory() const
      {
        return category_;
      }
      
      operator filter() const;
      
    private:
      
      const inflection category_;
    };
    
    static const inflection_field form(inflection category)
    {
      return inflection_field(category);
    }
    
    friend filter operator%=(lemma::inflection_field check, filter joinCondition);
    
  private:
    
    void initializeForm(inflection category) const;
    
    bool valid_ = false;
    
    int id_;
    
    mutable std::map<inflection, std::vector<class form>> forms_;
    
    const database* db_;
    
    static const field formJoin;
    static const field inflectionCategory;
    
  };
  
};

#endif /* end of include guard: LEMMA_H_0A180D30 */
