#include "lemma.h"
#include <list>
#include <stdexcept>
#include "form.h"

namespace verbly {
  namespace generator {

    int lemma::nextId_ = 0;

    lemma::lemma(const form& baseForm) :
      id_(nextId_++),
      baseForm_(baseForm)
    {
      inflections_[inflection::base] = {&baseForm};
    }

    void lemma::addInflection(inflection type, const form& f)
    {
      if (type == inflection::base)
      {
        throw std::invalid_argument("There can only be one base form");
      }

      inflections_[type].insert(&f);
    }

    std::set<const form*> lemma::getInflections(inflection type) const
    {
      if (inflections_.count(type))
      {
        return inflections_.at(type);
      } else {
        return {};
      }
    }

    hatkirby::database& operator<<(hatkirby::database& db, const lemma& arg)
    {
      for (inflection type : {
        inflection::base,
        inflection::plural,
        inflection::comparative,
        inflection::superlative,
        inflection::past_tense,
        inflection::past_participle,
        inflection::ing_form,
        inflection::s_form})
      {
        for (const form* f : arg.getInflections(type))
        {
          db.insertIntoTable(
            "lemmas_forms",
            {
              { "lemma_id", arg.getId() },
              { "form_id", f->getId() },
              { "category", static_cast<int>(type) }
            });
        }
      }

      return db;
    }

  };
};
