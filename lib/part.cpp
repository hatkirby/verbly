#include "part.h"
#include <stdexcept>
#include <hkutil/string.h>
#include "database.h"

namespace verbly {

  const object part::objectType = object::part;

  const std::list<std::string> part::select = {"part_id", "frame_id", "part_index", "type", "role", "prepositions", "preposition_literality", "literal_value"};

  const field part::index = field::integerField(object::part, "part_index");
  const field part::type = field::integerField(object::part, "type");

  const field part::role = field::stringField(object::part, "role", true);

  const field part::frames = field::joinField(object::part, "frame_id", object::frame);

  const field part::selrestr_field::selrestrJoin = field::joinField(object::part, "part_id", "selrestrs");
  const field part::selrestr_field::selrestrField = field::stringField("selrestrs", "selrestr");

  const field part::synrestr_field::synrestrJoin = field::joinField(object::part, "part_id", "synrestrs");
  const field part::synrestr_field::synrestrField = field::stringField("synrestrs", "synrestr");

  const part::selrestr_field part::selrestrs = {};
  const part::synrestr_field part::synrestrs = {};

  part part::createNounPhrase(
    std::string role,
    std::set<std::string> selrestrs,
    std::set<std::string> synrestrs)
  {
    return part {
      part_type::noun_phrase,
      np_type {
        std::move(role),
        std::move(selrestrs),
        std::move(synrestrs)
      }
    };
  }

  part part::createVerb()
  {
    return part(part_type::verb);
  }

  part part::createPreposition(std::vector<std::string> choices, bool literal)
  {
    return part {
      part_type::preposition,
      prep_type {
        std::move(choices),
        literal
      }
    };
  }

  part part::createAdjective()
  {
    return part(part_type::adjective);
  }

  part part::createAdverb()
  {
    return part(part_type::adverb);
  }

  part part::createLiteral(std::string value)
  {
    return part {
      part_type::literal,
      std::move(value)
    };
  }

  part::part(const database& db, hatkirby::row row)
  {
    int id = std::get<int>(row[0]);

    type_ = static_cast<part_type>(std::get<int>(row[3]));

    switch (type_)
    {
      case part_type::noun_phrase:
      {
        variant_ = np_type {
          std::get<std::string>(row[4]),
          db.selrestrs(id),
          db.synrestrs(id)
        };

        break;
      }

      case part_type::preposition:
      {
        hatkirby::blob_type raw =
          std::get<hatkirby::blob_type>(row[5]);

        std::string serializedChoices(
          std::begin(raw),
          std::end(raw));

        variant_ = prep_type {
          hatkirby::split<std::vector<std::string>>(
            std::move(serializedChoices),
            ","),
          (std::get<int>(row[6]) == 1)
        };

        break;
      }

      case part_type::literal:
      {
        variant_ = std::get<std::string>(row[7]);

        break;
      }

      case part_type::verb:
      case part_type::adjective:
      case part_type::adverb:
      case part_type::invalid:
      {
        break;
      }
    }
  }

  const std::string& part::getNounRole() const
  {
    if (type_ != part_type::noun_phrase)
    {
      throw std::domain_error("part is not a noun phrase");
    }

    return std::get<np_type>(variant_).role;
  }

  const std::set<std::string>& part::getNounSelrestrs() const
  {
    if (type_ != part_type::noun_phrase)
    {
      throw std::domain_error("part is not a noun phrase");
    }

    return std::get<np_type>(variant_).selrestrs;
  }

  const std::set<std::string>& part::getNounSynrestrs() const
  {
    if (type_ != part_type::noun_phrase)
    {
      throw std::domain_error("part is not a noun phrase");
    }

    return std::get<np_type>(variant_).synrestrs;
  }

  bool part::nounHasSynrestr(std::string synrestr) const
  {
    if (type_ != part_type::noun_phrase)
    {
      throw std::domain_error("part is not a noun phrase");
    }

    return std::get<np_type>(variant_).synrestrs.count(synrestr);
  }

  const std::vector<std::string>& part::getPrepositionChoices() const
  {
    if (type_ != part_type::preposition)
    {
      throw std::domain_error("part is not a preposition");
    }

    return std::get<prep_type>(variant_).choices;
  }

  bool part::isPrepositionLiteral() const
  {
    if (type_ != part_type::preposition)
    {
      throw std::domain_error("part is not a preposition");
    }

    return std::get<prep_type>(variant_).literal;
  }

  const std::string& part::getLiteralValue() const
  {
    if (type_ != part_type::literal)
    {
      throw std::domain_error("part is not a literal");
    }

    return std::get<std::string>(variant_);
  }

  filter part::synrestr_field::operator%=(std::string synrestr) const
  {
    return (synrestrJoin %= (synrestrField == synrestr));
  }

};
