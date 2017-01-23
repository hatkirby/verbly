#ifndef PART_H_C8F0661B
#define PART_H_C8F0661B

#include <string>
#include <vector>
#include <set>
#include "selrestr.h"

namespace verbly {

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

    static part createPreposition(std::vector<std::string> choices, bool literal);

    static part createAdjective();

    static part createAdverb();

    static part createLiteral(std::string value);

    // Default constructor

    part()
    {
    }

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

    bool nounHasSynrestr(std::string synrestr) const;

    // Preposition accessors

    std::vector<std::string> getPrepositionChoices() const;

    bool isPrepositionLiteral() const;

    // Literal accessors

    std::string getLiteralValue() const;

  private:

    // Private constructors

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
        std::vector<std::string> choices;
        bool literal;
      } preposition_;
      std::string literal_;
    };

    type type_ = type::invalid;

  };

};

#endif /* end of include guard: PART_H_C8F0661B */
