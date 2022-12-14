#ifndef PART_H_C8F0661B
#define PART_H_C8F0661B

#include <string>
#include <vector>
#include <set>
#include <list>
#include <hkutil/database.h>
#include <variant>
#include "field.h"
#include "filter.h"
#include "enums.h"

namespace verbly {

  class database;

  class part {
  public:

    // Static factories

    static part createNounPhrase(
      std::string role,
      std::set<std::string> selrestrs,
      std::set<std::string> synrestrs);

    static part createVerb();

    static part createPreposition(
      std::vector<std::string> choices,
      bool literal);

    static part createAdjective();

    static part createAdverb();

    static part createLiteral(std::string value);

    // Construct from database

    part(const database& db, hatkirby::row row);

    // General accessors

    part_type getType() const
    {
      return type_;
    }

    // Noun phrase accessors

    const std::string& getNounRole() const;

    const std::set<std::string>& getNounSelrestrs() const;

    const std::set<std::string>& getNounSynrestrs() const;

    bool nounHasSynrestr(std::string synrestr) const;

    // Preposition accessors

    const std::vector<std::string>& getPrepositionChoices() const;

    bool isPrepositionLiteral() const;

    // Literal accessors

    const std::string& getLiteralValue() const;

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

    // Data

    struct np_type {
      std::string role;
      std::set<std::string> selrestrs;
      std::set<std::string> synrestrs;
    };

    struct prep_type {
      std::vector<std::string> choices;
      bool literal;
    };

    using variant_type =
      std::variant<
        std::monostate,
        np_type,
        prep_type,
        std::string>;

    variant_type variant_;

    part_type type_ = part_type::invalid;

    // Private constructors

    part(part_type t, variant_type v = {}) : type_(t), variant_(v)
    {
    }

  };

};

#endif /* end of include guard: PART_H_C8F0661B */
