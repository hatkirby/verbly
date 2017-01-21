#ifndef PART_H_FB54F361
#define PART_H_FB54F361

#include <string>
#include <set>
#include "../lib/selrestr.h"

namespace verbly {
  namespace generator {
    
    class part {
    public:
      enum class type {
        invalid = -1,
        noun_phrase = 0,
        verb = 1,
        preposition = 2,
        adjective = 3,
        adverb = 4,
        literal = 5
      };
      
      // Static factories
      
      static part createNounPhrase(std::string role, selrestr selrestrs, std::set<std::string> synrestrs);
      
      static part createVerb();
      
      static part createPreposition(std::set<std::string> choices, bool literal);
      
      static part createAdjective();
      
      static part createAdverb();
      
      static part createLiteral(std::string value);
      
      // Copy and move constructors
      
      part(const part& other);
      
      part(part&& other);
      
      // Assignment
      
      part& operator=(part other);
      
      // Swap
      
      friend void swap(part& first, part& second);
      
      // Destructor
      
      ~part();
      
      // General accessors
      
      type getType() const
      {
        return type_;
      }
      
      // Noun phrase accessors
      
      std::string getNounRole() const;
      
      selrestr getNounSelrestrs() const;
      
      std::set<std::string> getNounSynrestrs() const;
      
      // Preposition accessors
      
      std::set<std::string> getPrepositionChoices() const;
      
      bool isPrepositionLiteral() const;
      
      // Literal accessors
      
      std::string getLiteralValue() const;
      
    private:
      
      // Private constructors
      
      part()
      {
      }
      
      part(type t) : type_(t)
      {
      }
      
      // Data
      
      union {
        struct {
          std::string role;
          selrestr selrestrs;
          std::set<std::string> synrestrs;
        } noun_phrase_;
        struct {
          std::set<std::string> choices;
          bool literal;
        } preposition_;
        std::string literal_;
      };
      
      type type_ = type::invalid;
      
    };
    
  };
};

#endif /* end of include guard: PART_H_FB54F361 */
