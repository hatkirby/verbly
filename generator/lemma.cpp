#include "lemma.h"
#include <list>
#include <cassert>
#include "field.h"
#include "database.h"
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
      // There can only be one base form.
      assert(type != inflection::base);

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

    database& operator<<(database& db, const lemma& arg)
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
          std::list<field> fields;
          fields.emplace_back("lemma_id", arg.getId());
          fields.emplace_back("form_id", f->getId());
          fields.emplace_back("category", static_cast<int>(type));

          db.insertIntoTable("lemmas_forms", std::move(fields));
        }
      }

      return db;
    }

  };
};
