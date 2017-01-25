#include "form.h"
#include <sqlite3.h>
#include "filter.h"
#include "database.h"
#include "query.h"
#include "util.h"

namespace verbly {

  const object form::objectType = object::form;

  const std::list<std::string> form::select = {"form_id", "form", "complexity", "proper"};

  const field form::id = field::integerField(object::form, "form_id");
  const field form::text = field::stringField(object::form, "form");
  const field form::complexity = field::integerField(object::form, "complexity");
  const field form::proper = field::booleanField(object::form, "proper");

  const field form::pronunciation = field::joinThrough(object::form, "form_id", object::pronunciation, "forms_pronunciations", "pronunciation_id");

  const field form::lemmaJoin = field::joinField(object::form, "form_id", object::lemma);
  const field form::inflectionCategory = field::integerField("lemmas_forms", "category");

  form::form(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    text_ = std::string(reinterpret_cast<const char*>(sqlite3_column_text(row, 1)));
    complexity_ = sqlite3_column_int(row, 2);
    proper_ = (sqlite3_column_int(row, 3) == 1);
  }

  filter operator%=(form::inflection_field check, filter joinCondition)
  {
    return (form::lemmaJoin %= (joinCondition && (form::inflectionCategory == check.getCategory())));
  }

  form::inflection_field::operator filter() const
  {
    return (form::lemmaJoin %= (form::inflectionCategory == category_));
  }

  const std::vector<pronunciation>& form::getPronunciations() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized form");
    }

    if (!initializedPronunciations_)
    {
      pronunciations_ = db_->pronunciations(pronunciation::form %= *this, false, -1).all();
      initializedPronunciations_ = true;
    }

    return pronunciations_;
  }

  bool form::startsWithVowelSound() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized form");
    }

    const std::vector<verbly::pronunciation>& pronunciations = getPronunciations();
    if (!pronunciations.empty())
    {
      return std::any_of(std::begin(pronunciations), std::end(pronunciations), [] (const verbly::pronunciation& p) {
        return p.getPhonemes().front().find_first_of("012") != std::string::npos;
      });
    } else {
      // If the word is not in CMUDICT, fall back to checking whether the first letter is a vowel.
      // Not perfect but will work in most cases.
      char ch = std::tolower(text_.front());
      return (ch == 'a') || (ch == 'e') || (ch == 'i') || (ch == 'o') || (ch == 'u');
    }
  }

};
