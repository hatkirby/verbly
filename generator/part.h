#ifndef PART_H_FB54F361
#define PART_H_FB54F361

#include <string>
#include <set>
#include <variant>
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

      struct noun_phrase_type {
        std::string role;
        std::set<std::string> selrestrs;
        std::set<std::string> synrestrs;
      };

      struct preposition_type {
        std::set<std::string> choices;
        bool literal;
      };

      using literal_type = std::string;

      using variant_type =
        std::variant<
          std::monostate,
          noun_phrase_type,
          preposition_type,
          literal_type>;

      static int nextId_;

      int id_;

      // Private constructors

      part()
      {
      }

      part(type t, variant_type variant = {}) :
        id_(nextId_++),
        type_(t),
        variant_(std::move(variant))
      {
        bool valid_type = false;
        switch (type_)
        {
          case part_type::noun_phrase:
          {
            valid_type = std::holds_alternative<noun_phrase_type>(variant_);
            break;
          }
          case part_type::preposition:
          {
            valid_type = std::holds_alternative<preposition_type>(variant_);
            break;
          }
          case part_type::literal:
          {
            valid_type = std::holds_alternative<literal_type>(variant_);
            break;
          }
          case part_type::invalid:
          case part_type::verb:
          case part_type::adjective:
          case part_type::adverb:
          {
            valid_type = std::holds_alternative<std::monostate>(variant_);
            break;
          }
        }
        if (!valid_type)
        {
          throw std::invalid_argument("Invalid variant provided for part");
        }
      }

      // Data

      variant_type variant_;

      type type_ = type::invalid;

    };

  };
};

#endif /* end of include guard: PART_H_FB54F361 */
