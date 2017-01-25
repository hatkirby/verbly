#include "part.h"
#include <stdexcept>

namespace verbly {
  namespace generator {

    part part::createNounPhrase(std::string role, selrestr selrestrs, std::set<std::string> synrestrs)
    {
      part p(type::noun_phrase);

      new(&p.noun_phrase_.role) std::string(std::move(role));
      new(&p.noun_phrase_.selrestrs) selrestr(std::move(selrestrs));
      new(&p.noun_phrase_.synrestrs) std::set<std::string>(std::move(synrestrs));

      return p;
    }

    part part::createVerb()
    {
      return part(type::verb);
    }

    part part::createPreposition(std::set<std::string> choices, bool literal)
    {
      part p(type::preposition);

      new(&p.preposition_.choices) std::set<std::string>(std::move(choices));
      p.preposition_.literal = literal;

      return p;
    }

    part part::createAdjective()
    {
      return part(type::adjective);
    }

    part part::createAdverb()
    {
      return part(type::adverb);
    }

    part part::createLiteral(std::string value)
    {
      part p(type::literal);

      new(&p.literal_) std::string(std::move(value));

      return p;
    }

    part::part(const part& other)
    {
      type_ = other.type_;

      switch (type_)
      {
        case type::noun_phrase:
        {
          new(&noun_phrase_.role) std::string(other.noun_phrase_.role);
          new(&noun_phrase_.selrestrs) selrestr(other.noun_phrase_.selrestrs);
          new(&noun_phrase_.synrestrs) std::set<std::string>(other.noun_phrase_.synrestrs);

          break;
        }

        case type::preposition:
        {
          new(&preposition_.choices) std::set<std::string>(other.preposition_.choices);
          preposition_.literal = other.preposition_.literal;

          break;
        }

        case type::literal:
        {
          new(&literal_) std::string(other.literal_);

          break;
        }

        case type::verb:
        case type::adjective:
        case type::adverb:
        case type::invalid:
        {
          break;
        }
      }
    }

    part::part(part&& other) : part()
    {
      swap(*this, other);
    }

    part& part::operator=(part other)
    {
      swap(*this, other);

      return *this;
    }

    void swap(part& first, part& second)
    {
      using type = part::type;

      type tempType = first.type_;
      std::string tempRole;
      selrestr tempSelrestrs;
      std::set<std::string> tempSynrestrs;
      std::set<std::string> tempChoices;
      bool tempPrepLiteral;
      std::string tempLiteralValue;

      switch (tempType)
      {
        case type::noun_phrase:
        {
          tempRole = std::move(first.noun_phrase_.role);
          tempSelrestrs = std::move(first.noun_phrase_.selrestrs);
          tempSynrestrs = std::move(first.noun_phrase_.synrestrs);

          break;
        }

        case type::preposition:
        {
          tempChoices = std::move(first.preposition_.choices);
          tempPrepLiteral = first.preposition_.literal;

          break;
        }

        case type::literal:
        {
          tempLiteralValue = std::move(first.literal_);

          break;
        }

        case type::verb:
        case type::adjective:
        case type::adverb:
        case type::invalid:
        {
          break;
        }
      }

      first.~part();

      first.type_ = second.type_;

      switch (first.type_)
      {
        case type::noun_phrase:
        {
          new(&first.noun_phrase_.role) std::string(std::move(second.noun_phrase_.role));
          new(&first.noun_phrase_.selrestrs) selrestr(std::move(second.noun_phrase_.selrestrs));
          new(&first.noun_phrase_.synrestrs) std::set<std::string>(std::move(second.noun_phrase_.synrestrs));

          break;
        }

        case type::preposition:
        {
          new(&first.preposition_.choices) std::set<std::string>(std::move(second.preposition_.choices));
          first.preposition_.literal = second.preposition_.literal;

          break;
        }

        case type::literal:
        {
          new(&first.literal_) std::string(std::move(second.literal_));

          break;
        }

        case type::verb:
        case type::adjective:
        case type::adverb:
        case type::invalid:
        {
          break;
        }
      }

      second.~part();

      second.type_ = tempType;

      switch (second.type_)
      {
        case type::noun_phrase:
        {
          new(&second.noun_phrase_.role) std::string(std::move(tempRole));
          new(&second.noun_phrase_.selrestrs) selrestr(std::move(tempSelrestrs));
          new(&second.noun_phrase_.synrestrs) std::set<std::string>(std::move(tempSynrestrs));

          break;
        }

        case type::preposition:
        {
          new(&second.preposition_.choices) std::set<std::string>(std::move(tempChoices));
          second.preposition_.literal = tempPrepLiteral;

          break;
        }

        case type::literal:
        {
          new(&second.literal_) std::string(std::move(tempLiteralValue));

          break;
        }

        case type::verb:
        case type::adjective:
        case type::adverb:
        case type::invalid:
        {
          break;
        }
      }
    }

    part::~part()
    {
      switch (type_)
      {
        case type::noun_phrase:
        {
          using string_type = std::string;
          using set_type = std::set<std::string>;

          noun_phrase_.role.~string_type();
          noun_phrase_.selrestrs.~selrestr();
          noun_phrase_.synrestrs.~set_type();

          break;
        }

        case type::preposition:
        {
          using set_type = std::set<std::string>;

          preposition_.choices.~set_type();

          break;
        }

        case type::literal:
        {
          using string_type = std::string;

          literal_.~string_type();

          break;
        }

        case type::verb:
        case type::adjective:
        case type::adverb:
        case type::invalid:
        {
          break;
        }
      }
    }

    std::string part::getNounRole() const
    {
      if (type_ == type::noun_phrase)
      {
        return noun_phrase_.role;
      } else {
        throw std::domain_error("part::getNounRole is only valid for noun phrase parts");
      }
    }

    selrestr part::getNounSelrestrs() const
    {
      if (type_ == type::noun_phrase)
      {
        return noun_phrase_.selrestrs;
      } else {
        throw std::domain_error("part::getNounSelrestrs is only valid for noun phrase parts");
      }
    }

    std::set<std::string> part::getNounSynrestrs() const
    {
      if (type_ == type::noun_phrase)
      {
        return noun_phrase_.synrestrs;
      } else {
        throw std::domain_error("part::getNounSynrestrs is only valid for noun phrase parts");
      }
    }

    std::set<std::string> part::getPrepositionChoices() const
    {
      if (type_ == type::preposition)
      {
        return preposition_.choices;
      } else {
        throw std::domain_error("part::getPrepositionChoices is only valid for preposition parts");
      }
    }

    bool part::isPrepositionLiteral() const
    {
      if (type_ == type::preposition)
      {
        return preposition_.literal;
      } else {
        throw std::domain_error("part::isPrepositionLiteral is only valid for preposition parts");
      }
    }

    std::string part::getLiteralValue() const
    {
      if (type_ == type::literal)
      {
        return literal_;
      } else {
        throw std::domain_error("part::getLiteralValue is only valid for literal parts");
      }
    }

  };
};
