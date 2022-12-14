#include "part.h"
#include <stdexcept>

namespace verbly {
  namespace generator {

    int part::nextId_ = 0;

    part part::createNounPhrase(std::string role, std::set<std::string> selrestrs, std::set<std::string> synrestrs)
    {
      return part(type::noun_phrase, noun_phrase_type {
        .role = std::move(role),
        .selrestrs = std::move(selrestrs),
        .synrestrs = std::move(synrestrs)
      });
    }

    part part::createVerb()
    {
      return part(type::verb);
    }

    part part::createPreposition(std::set<std::string> choices, bool literal)
    {
      return part(type::preposition, preposition_type {
        .choices = std::move(choices),
        .literal = literal
      });
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
      return part(type::literal, std::move(value));
    }

    part part::duplicate(const part& other)
    {
      return part(other.type_, other.variant_);
    }

    std::string part::getNounRole() const
    {
      if (type_ == type::noun_phrase)
      {
        return std::get<noun_phrase_type>(variant_).role;
      } else {
        throw std::domain_error("part::getNounRole is only valid for noun phrase parts");
      }
    }

    std::set<std::string> part::getNounSelrestrs() const
    {
      if (type_ == type::noun_phrase)
      {
        return std::get<noun_phrase_type>(variant_).selrestrs;
      } else {
        throw std::domain_error("part::getNounSelrestrs is only valid for noun phrase parts");
      }
    }

    std::set<std::string> part::getNounSynrestrs() const
    {
      if (type_ == type::noun_phrase)
      {
        return std::get<noun_phrase_type>(variant_).synrestrs;
      } else {
        throw std::domain_error("part::getNounSynrestrs is only valid for noun phrase parts");
      }
    }

    std::set<std::string> part::getPrepositionChoices() const
    {
      if (type_ == type::preposition)
      {
        return std::get<preposition_type>(variant_).choices;
      } else {
        throw std::domain_error("part::getPrepositionChoices is only valid for preposition parts");
      }
    }

    bool part::isPrepositionLiteral() const
    {
      if (type_ == type::preposition)
      {
        return std::get<preposition_type>(variant_).literal;
      } else {
        throw std::domain_error("part::isPrepositionLiteral is only valid for preposition parts");
      }
    }

    std::string part::getLiteralValue() const
    {
      if (type_ == type::literal)
      {
        return std::get<literal_type>(variant_);
      } else {
        throw std::domain_error("part::getLiteralValue is only valid for literal parts");
      }
    }

  };
};
