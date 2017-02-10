#ifndef NOTION_H_FD1C7646
#define NOTION_H_FD1C7646

#include <stdexcept>
#include <string>
#include "field.h"
#include "filter.h"

struct sqlite3_stmt;

namespace verbly {
  
  class database;
  
  class notion {
  public:
    
    // Default constructor
    
    notion() = default;
    
    // Construct from database
    
    notion(const database& db, sqlite3_stmt* row);
    
    // Accessors
    
    bool isValid() const
    {
      return valid_;
    }
    
    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      return id_;
    }
    
    part_of_speech getPartOfSpeech() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      return partOfSpeech_;
    }
    
    bool hasWnid() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      return hasWnid_;
    }
    
    int getWnid() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      if (!hasWnid_)
      {
        throw std::domain_error("Notion has no wnid");
      }
      
      return wnid_;
    }
    
    bool hasNumOfImages() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      return hasNumOfImages_;
    }
    
    int getNumOfImages() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }
      
      if (!hasNumOfImages_)
      {
        throw std::domain_error("Notion does not have a number of images");
      }
      
      return numOfImages_;
    }
    
    // Convenience
    
    std::string getImageNetUrl() const;
    
    // Type info
    
    static const object objectType;
    
    static const std::list<std::string> select;
    
    // Query fields
    
    static const field id;
    static const field partOfSpeech;
    static const field wnid;
    static const field numOfImages;
    
    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }

      return (id == id_);
    }
    
    filter operator!() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized notion");
      }

      return (id != id_);
    }

    // Relationships with other objects
    
    static const field words;
    
    // Relationships with self
    
    static const field hypernyms;
    static const field hyponyms;
    
    static const field fullHypernyms;
    static const field fullHyponyms;
    
    static const field instances;
    static const field classes;
    
    static const field memberMeronyms;
    static const field memberHolonyms;
    
    static const field fullMemberMeronyms;
    static const field fullMemberHolonyms;
    
    static const field partMeronyms;
    static const field partHolonyms;
    
    static const field fullPartMeronyms;
    static const field fullPartHolonyms;
    
    static const field substanceMeronyms;
    static const field substanceHolonyms;
    
    static const field fullSubstanceMeronyms;
    static const field fullSubstanceHolonyms;
    
    static const field variants;
    static const field attributes;
    
    static const field similarAdjectives;
    
    static const field entails;
    static const field entailedBy;
    
    static const field causes;
    static const field effects;
    
    // Preposition group relationship
    
    class preposition_group_field {
    public:
      
      filter operator==(std::string groupName) const;
      
    private:
      
      static const field isA;
      static const field groupNameField;
    };
    
    static const preposition_group_field prepositionGroups;
    
  private:
    bool valid_ = false;
    
    int id_;
    part_of_speech partOfSpeech_;
    bool hasWnid_ = false;
    int wnid_;
    bool hasNumOfImages_ = false;
    int numOfImages_;
    
    const database* db_;
    
  };
  
};

#endif /* end of include guard: NOTION_H_FD1C7646 */
