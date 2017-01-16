#ifndef WORD_H_DF91B1B4
#define WORD_H_DF91B1B4

#include <stdexcept>
#include <map>
#include "field.h"
#include "filter.h"
#include "notion.h"
#include "lemma.h"
#include "group.h"

struct sqlite3_stmt;

namespace verbly {
  
  class database;
  
  class word {
  public:
    
    // Default constructor
    
    word() = default;
    
    // Construct from database
    
    word(const database& db, sqlite3_stmt* row);
    
    // Accessors
    
    operator bool() const
    {
      return valid_;
    }
    
    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }
      
      return id_;
    }
    
    bool hasTagCount() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }
      
      return hasTagCount_;
    }
    
    int getTagCount() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }
      
      if (!hasTagCount_)
      {
        throw std::domain_error("Word has no tag count");
      }
      
      return tagCount_;
    }
    
    bool hasAdjectivePositioning() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }
      
      return (adjectivePosition_ != positioning::undefined);
    }
    
    positioning getAdjectivePosition() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }
      
      if (adjectivePosition_ == positioning::undefined)
      {
        throw std::domain_error("Word has no adjective position");
      }
      
      return adjectivePosition_;
    }
    
    const notion& getNotion() const;
    
    const lemma& getLemma() const;
    
    // Convenience accessors
    
    std::string getBaseForm() const;
  
    std::list<std::string> getInflections(inflection infl) const;
    
    // Type info
    
    static const object objectType;
    
    static const std::list<std::string> select;
    
    // Query fields
    
    static const field id;
    static const field tagCount;
    static const field adjectivePosition;
    
    operator filter() const
    {
      return (id == id_);
    }
    
    // Relationships with other objects
    
    static const field notion;
    static const field lemma;
    static const field group;
    
    // Relationships with self
    
    static const field antonyms;
    
    static const field specifications;
    static const field generalizations;
    
    static const field pertainyms;
    static const field antiPertainyms;
    
    static const field mannernyms;
    static const field antiMannernyms;
    
    static const field usageTerms;
    static const field usageDomains;
    
    static const field topicalTerms;
    static const field topicalDomains;
    
    static const field regionalTerms;
    static const field regionalDomains;
    
  private:
    bool valid_ = false;
    
    int id_;
    bool hasTagCount_ = false;
    int tagCount_;
    positioning adjectivePosition_ = positioning::undefined;
    int notionId_;
    int lemmaId_;
    bool hasGroup_ = false;
    int groupId_;
    
    const database* db_;
    
    mutable class notion notion_;
    mutable class lemma lemma_;
    mutable class group group_;
    
  };
  
};

#endif /* end of include guard: WORD_H_DF91B1B4 */
