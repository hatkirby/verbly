#include "word.h"
#include "form.h"
#include "util.h"
#include "database.h"
#include "query.h"

namespace verbly {

  const object word::objectType = object::word;

  const std::list<std::string> word::select = {"word_id", "notion_id", "lemma_id", "tag_count", "position", "group_id"};

  const field word::id = field::integerField(object::word, "word_id");
  const field word::tagCount = field::integerField(object::word, "tag_count", true);
  const field word::adjectivePosition = field::integerField(object::word, "position", true);

  const field word::notions = field::joinField(object::word, "notion_id", object::notion);
  const field word::frames = field::joinField(object::word, "group_id", object::frame, true);

  const field word::antonyms = field::selfJoin(object::word, "word_id", "antonymy", "antonym_2_id", "antonym_1_id");

  const field word::specifications = field::selfJoin(object::word, "word_id", "specification", "general_id", "specific_id");
  const field word::generalizations = field::selfJoin(object::word, "word_id", "specification", "specific_id", "general_id");

  const field word::pertainyms = field::selfJoin(object::word, "word_id", "pertainymy", "noun_id", "pertainym_id");
  const field word::antiPertainyms = field::selfJoin(object::word, "word_id", "pertainymy", "pertainym_id", "noun_id");

  const field word::mannernyms = field::selfJoin(object::word, "word_id", "mannernymy", "adjective_id", "mannernym_id");
  const field word::antiMannernyms = field::selfJoin(object::word, "word_id", "mannernymy", "mannernym_id", "adjective_id");

  const field word::usageTerms = field::selfJoin(object::word, "word_id", "usage", "domain_id", "term_id");
  const field word::usageDomains = field::selfJoin(object::word, "word_id", "usage", "term_id", "domain_id");

  const field word::topicalTerms = field::selfJoin(object::word, "word_id", "topicality", "domain_id", "term_id");
  const field word::topicalDomains = field::selfJoin(object::word, "word_id", "topicality", "term_id", "domain_id");

  const field word::regionalTerms = field::selfJoin(object::word, "word_id", "regionality", "domain_id", "term_id");
  const field word::regionalDomains = field::selfJoin(object::word, "word_id", "regionality", "term_id", "domain_id");

  const word::synonyms_field word::synonyms = {};

  field word::forms(inflection category)
  {
    return field::joinThroughWhere(object::word, "lemma_id", object::form, "lemmas_forms", "form_id", "category", static_cast<int>(category));
  }

  word::word(const database& db, hatkirby::row row) : db_(&db), valid_(true)
  {
    id_ = std::get<int>(row[0]);

    notion_ = db.notions(notion::id == std::get<int>(row[1])).first();

    if (!std::holds_alternative<std::nullptr_t>(row[3]))
    {
      hasTagCount_ = true;
      tagCount_ = std::get<int>(row[3]);
    }

    if (!std::holds_alternative<std::nullptr_t>(row[4]))
    {
      adjectivePosition_ = static_cast<positioning>(std::get<int>(row[4]));
    }

    if (!std::holds_alternative<std::nullptr_t>(row[5]))
    {
      frames_ = db.frames(*this).all();
    }
  }

  const form& word::getBaseForm() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized word");
    }

    if (!forms_.count(inflection::base))
    {
      initializeForm(inflection::base);
    }

    return forms_.at(inflection::base).front();
  }

  bool word::hasInflection(inflection category) const
  {
    return !getInflections(category).empty();
  }

  const std::vector<form>& word::getInflections(inflection category) const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized word");
    }

    if (!forms_.count(category))
    {
      initializeForm(category);
    }

    return forms_.at(category);
  }

  void word::initializeForm(inflection infl) const
  {
    if (!db_)
    {
      throw std::domain_error("Database not present");
    }

    forms_[infl] = db_->forms(form::words(infl) %= *this, verbly::form::id, -1).all();
  }

  filter word::synonyms_field::operator%=(filter joinCondition) const
  {
    return (verbly::word::notions %=
      filter("synonyms", true,
        (verbly::notion::words %= (
          std::move(joinCondition)
          && (filter(
            verbly::word::id,
            filter::comparison::field_does_not_equal,
            verbly::word::id))))));
  }

  word::synonyms_field::operator filter() const
  {
    return (verbly::word::notions %=
      filter("synonyms", true,
        (verbly::notion::words %= filter(
          verbly::word::id,
          filter::comparison::field_does_not_equal,
          verbly::word::id))));
  }

};
