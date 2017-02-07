#include "form.h"
#include <sqlite3.h>
#include <algorithm>
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

  const field form::pronunciations = field::joinThrough(object::form, "form_id", object::pronunciation, "forms_pronunciations", "pronunciation_id");

  field form::words(inflection category)
  {
    return field::joinThroughWhere(object::form, "form_id", object::word, "lemmas_forms", "lemma_id", "category", static_cast<int>(category));
  }

  form::form(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    text_ = std::string(reinterpret_cast<const char*>(sqlite3_column_text(row, 1)));
    complexity_ = sqlite3_column_int(row, 2);
    proper_ = (sqlite3_column_int(row, 3) == 1);
  }

  const std::vector<pronunciation>& form::getPronunciations() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized form");
    }

    if (!initializedPronunciations_)
    {
      pronunciations_ = db_->pronunciations(pronunciation::forms %= *this, pronunciation::id, -1).all();
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

    const std::vector<pronunciation>& pronunciations = getPronunciations();
    if (!pronunciations.empty())
    {
      return std::any_of(std::begin(pronunciations), std::end(pronunciations), [] (const pronunciation& p) {
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
