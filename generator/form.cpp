#include "form.h"
#include <algorithm>
#include <list>
#include <cctype>
#include "pronunciation.h"

namespace verbly {
  namespace generator {

    int form::nextId_ = 0;

    form::form(std::string text) :
      id_(nextId_++),
      text_(text),
      complexity_(std::count(std::begin(text), std::end(text), ' ') + 1),
      proper_(std::any_of(std::begin(text), std::end(text), ::isupper)),
      length_(text.length())
    {
    }

    void form::addPronunciation(const pronunciation& p)
    {
      pronunciations_.insert(&p);
    }

    hatkirby::database& operator<<(hatkirby::database& db, const form& arg)
    {
      // Serialize the form first.
      {
        db.insertIntoTable(
          "forms",
          {
            { "form_id", arg.getId() },
            { "form", arg.getText() },
            { "complexity", arg.getComplexity() },
            { "proper", arg.isProper() },
            { "length", arg.getLength() }
          });
      }

      // Then, serialize the form/pronunciation relationship.
      for (const pronunciation* p : arg.getPronunciations())
      {
        db.insertIntoTable(
          "forms_pronunciations",
          {
            { "form_id", arg.getId() },
            { "pronunciation_id", p->getId() }
          });
      }

      return db;
    }

  };
};
