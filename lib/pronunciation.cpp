#include "pronunciation.h"
#include <hkutil/string.h>
#include "form.h"
#include "word.h"

namespace verbly {

  const object pronunciation::objectType = object::pronunciation;

  const std::list<std::string> pronunciation::select = {"pronunciation_id", "phonemes", "syllables", "stress", "prerhyme", "rhyme"};

  const field pronunciation::id = field::integerField(object::pronunciation, "pronunciation_id");
  const field pronunciation::numOfSyllables = field::integerField(object::pronunciation, "syllables");
  const field pronunciation::stress = field::stringField(object::pronunciation, "stress");

  const field pronunciation::forms = field::joinThrough(object::pronunciation, "pronunciation_id", object::form, "forms_pronunciations", "form_id");

  const field pronunciation::prerhyme = field::stringField(object::pronunciation, "prerhyme", true);
  const field pronunciation::rhyme = field::stringField(object::pronunciation, "rhyme", true);

  const field pronunciation::anaphones = field::joinField(object::pronunciation, "anaphone_set_id", object::pronunciation);

  const field pronunciation::merophones = field::selfJoin(object::pronunciation, "pronunciation_id", "merophony", "merophone_id", "holophone_id");
  const field pronunciation::holophones = field::selfJoin(object::pronunciation, "pronunciation_id", "merophony", "holophone_id", "merophone_id");

  const field pronunciation::rhymes_field::rhymeJoin = field::joinField(object::pronunciation, "rhyme", object::pronunciation);
  const pronunciation::rhymes_field pronunciation::rhymes = {};

  pronunciation::pronunciation(
    const database& db,
    hatkirby::row row) :
      valid_(true)
  {
    id_ = mpark::get<int>(row[0]);

    phonemes_ =
      hatkirby::split<std::vector<std::string>>(
        mpark::get<std::string>(row[1]),
        " ");

    syllables_ = mpark::get<int>(row[2]);
    stress_ = mpark::get<std::string>(row[3]);

    if (!mpark::holds_alternative<std::nullptr_t>(row[5]))
    {
      hasRhyme_ = true;

      prerhyme_ = mpark::get<std::string>(row[4]);
      rhyme_ = mpark::get<std::string>(row[5]);
    }
  }

  filter pronunciation::rhymes_field::operator%=(filter joinCondition) const
  {
    return (rhymeJoin %= (
      joinCondition
      && filter(
        verbly::pronunciation::prerhyme,
        filter::comparison::field_does_not_equal,
        verbly::pronunciation::prerhyme)));
  }

  pronunciation::rhymes_field::operator filter() const
  {
    return (rhymeJoin %= filter(
      verbly::pronunciation::prerhyme,
      filter::comparison::field_does_not_equal,
      verbly::pronunciation::prerhyme));
  }

};
