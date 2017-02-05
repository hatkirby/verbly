#ifndef PART_H_C8F0661B
#define PART_H_C8F0661B

#include <string>
#include <vector>
#include <set>
#include <list>
#include "field.h"
#include "filter.h"
#include "enums.h"

struct sqlite3_stmt;

namespace verbly {

  class database;

  class part {
  public:

    // Static factories

    static part createNounPhrase(std::string role, std::set<std::string> selrestrs, std::set<std::string> synrestrs);

    static part createVerb();

    static part createPreposition(std::vector<std::string> choices, bool literal);

    static part createAdjective();

    static part createAdverb();

    static part createLiteral(std::string value);

    // Default constructor

    part()
    {
    }

    // Construct from database

    part(const database& db, sqlite3_stmt* row);

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

    operator bool() const
    {
      return (type_ != part_type::invalid);
    }

    part_type getType() const
    {
      return type_;
    }

    // Noun phrase accessors

    std::string getNounRole() const;

    std::set<std::string> getNounSelrestrs() const;

    std::set<std::string> getNounSynrestrs() const;

    bool nounHasSynrestr(std::string synrestr) const;

    // Preposition accessors

    std::vector<std::string> getPrepositionChoices() const;

    bool isPrepositionLiteral() const;

    // Literal accessors

    std::string getLiteralValue() const;

    // Type info

    static const object objectType;

    static const std::list<std::string> select;

    // Query fields

    static const field index;
    static const field type;

    static const field role;

    // Relationships to other objects

    static const field frames;

    // Noun selrestr and synrestr relationships

    class selrestr_field {
    public:

      filter operator%=(std::string selrestr) const;

    private:

      static const field selrestrJoin;
      static const field selrestrField;
    };

    static const selrestr_field selrestrs;
    
    class synrestr_field {
    public:

      filter operator%=(std::string synrestr) const;

    private:

      static const field synrestrJoin;
      static const field synrestrField;
    };

    static const synrestr_field synrestrs;

  private:

    // Private constructors

    part(part_type t) : type_(t)
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
        std::vector<std::string> choices;
        bool literal;
      } preposition_;
      std::string literal_;
    };

    part_type type_ = part_type::invalid;

  };

};

#endif /* end of include guard: PART_H_C8F0661B */
