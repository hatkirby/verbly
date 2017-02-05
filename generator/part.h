#ifndef PART_H_FB54F361
#define PART_H_FB54F361

#include <string>
#include <set>
#include "../lib/enums.h"

namespace verbly {

  namespace generator {

    class part {
    public:

      using type = part_type;

      // Static factories

      static part createNounPhrase(std::string role, std::set<std::string> selrestrs, std::set<std::string> synrestrs);

      static part createVerb();

      static part createPreposition(std::set<std::string> choices, bool literal);

      static part createAdjective();

      static part createAdverb();

      static part createLiteral(std::string value);

      // Duplication

      static part duplicate(const part& other);

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

      int getId() const
      {
        return id_;
      }

      type getType() const
      {
        return type_;
      }

      // Noun phrase accessors

      std::string getNounRole() const;

      std::set<std::string> getNounSelrestrs() const;

      std::set<std::string> getNounSynrestrs() const;

      // Preposition accessors

      std::set<std::string> getPrepositionChoices() const;

      bool isPrepositionLiteral() const;

      // Literal accessors

      std::string getLiteralValue() const;

    private:

      static int nextId_;

      int id_;

      // Private constructors

      part()
      {
      }

      part(type t) :
        id_(nextId_++),
        type_(t)
      {
      }

      // Data

      union {
        struct {
          std::string role;
          std::set<std::string> selrestrs;
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
