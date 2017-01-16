#ifndef PRONUNCIATION_H_C68F86B0
#define PRONUNCIATION_H_C68F86B0

#include <stdexcept>
#include <vector>
#include <string>
#include "field.h"
#include "filter.h"

struct sqlite3_stmt;

namespace verbly {
  
  class form;
  class lemma;
  class word;
  class database;
  
  class pronunciation {
  public:
    
    // Default constructor
    
    pronunciation() = default;
    
    // Construct from database
    
    pronunciation(const database& db, sqlite3_stmt* row);
    
    // Accessors
    
    operator bool() const
    {
      return valid_;
    }
    
    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      return id_;
    }
    
    const std::vector<std::string>& getPhonemes() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      return phonemes_;
    }
    
    int getSyllables() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      return syllables_;
    }
    
    std::string getStress() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      return stress_;
    }
    
    bool hasRhyme() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      return hasRhyme_;
    }
    
    std::string getPrerhyme() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      if (!hasRhyme_)
      {
        throw std::domain_error("This pronunciation has no rhyme");
      }
      
      return prerhyme_;
    }
    
    std::string getRhyme() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized pronunciation");
      }
      
      if (!hasRhyme_)
      {
        throw std::domain_error("This pronunciation has no rhyme");
      }
      
      return rhyme_;
    }
    
    // Type info
    
    static const object objectType;
    
    static const std::list<std::string> select;
    
    // Query fields
    
    static const field id;
    static const field numOfSyllables;
    static const field stress;
    
    operator filter() const
    {
      return (id == id_);
    }
    
    static filter rhymesWith(const pronunciation& arg);
    static filter rhymesWith(const class form& arg);
    static filter rhymesWith(const lemma& arg);
    static filter rhymesWith(const word& arg);
    
    // Relationships to other objects
    
    static const field form;
    
  private:
    bool valid_ = false;
    
    int id_;
    std::vector<std::string> phonemes_;
    int syllables_;
    std::string stress_;
    bool hasRhyme_ = false;
    std::string prerhyme_;
    std::string rhyme_;
    
    const database* db_;
    
    static const field prerhyme;
    static const field rhyme;
    
  };
  
};

#endif /* end of include guard: PRONUNCIATION_H_C68F86B0 */
