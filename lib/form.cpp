#include "form.h"
#include <algorithm>
#include "filter.h"
#include "database.h"
#include "query.h"

namespace verbly {

  const object form::objectType = object::form;

  const std::list<std::string> form::select = {"form_id", "form", "complexity", "proper", "length"};

  const field form::id = field::integerField(object::form, "form_id");
  const field form::text = field::stringField(object::form, "form");
  const field form::complexity = field::integerField(object::form, "complexity");
  const field form::proper = field::booleanField(object::form, "proper");
  const field form::length = field::integerField(object::form, "length");

  const field form::pronunciations = field::joinThrough(object::form, "form_id", object::pronunciation, "forms_pronunciations", "pronunciation_id");

  field form::words(inflection category)
  {
    return field::joinThroughWhere(object::form, "form_id", object::word, "lemmas_forms", "lemma_id", "category", static_cast<int>(category));
  }

  form::form(const database& db, hatkirby::row row) : valid_(true)
  {
    id_ = std::get<int>(row[0]);
    text_ = std::get<std::string>(row[1]);
    complexity_ = std::get<int>(row[2]);
    proper_ = (std::get<int>(row[3]) == 1);
    length_ = std::get<int>(row[4]);

    pronunciations_ = db.pronunciations(*this, pronunciation::id, -1).all();
  }

  bool form::startsWithVowelSound() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized form");
    }

    if (!pronunciations_.empty())
    {
      return std::any_of(
        std::begin(pronunciations_),
        std::end(pronunciations_),
        [] (const pronunciation& p) {
          return p.getPhonemes().front().find_first_of("012") !=
            std::string::npos;
        });
    } else {
      // If the word is not in CMUDICT, fall back to checking whether the first
      // letter is a vowel. Not perfect but will work in most cases.
      char ch = std::tolower(text_.front());
      return (ch == 'a') ||
             (ch == 'e') ||
             (ch == 'i') ||
             (ch == 'o') ||
             (ch == 'u');
    }
  }

};
