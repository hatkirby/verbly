#include "form.h"
#include <algorithm>
#include <list>
#include "database.h"
#include "field.h"
#include "pronunciation.h"

namespace verbly {
  namespace generator {

    int form::nextId_ = 0;

    form::form(std::string text) :
      id_(nextId_++),
      text_(text),
      complexity_(std::count(std::begin(text), std::end(text), ' ') + 1),
      proper_(std::any_of(std::begin(text), std::end(text), std::isupper))
    {
    }

    void form::addPronunciation(const pronunciation& p)
    {
      pronunciations_.insert(&p);
    }

    database& operator<<(database& db, const form& arg)
    {
      // Serialize the form first.
      {
        std::list<field> fields;
        fields.emplace_back("form_id", arg.getId());
        fields.emplace_back("form", arg.getText());
        fields.emplace_back("complexity", arg.getComplexity());
        fields.emplace_back("proper", arg.isProper());

        db.insertIntoTable("forms", std::move(fields));
      }

      // Then, serialize the form/pronunciation relationship.
      for (const pronunciation* p : arg.getPronunciations())
      {
        std::list<field> fields;
        fields.emplace_back("form_id", arg.getId());
        fields.emplace_back("pronunciation_id", p->getId());

        db.insertIntoTable("forms_pronunciations", std::move(fields));
      }

      return db;
    }

  };
};
