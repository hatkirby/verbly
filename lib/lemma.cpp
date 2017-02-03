#include "lemma.h"
#include <sqlite3.h>
#include "database.h"
#include "query.h"

namespace verbly {

  const object lemma::objectType = object::lemma;

  const std::list<std::string> lemma::select = {"lemma_id"};

  const field lemma::id = field::integerField(object::lemma, "lemma_id");
  const field lemma::inflectionCategory = field::integerField(object::lemma, "category");

  const field lemma::words = field::joinField(object::lemma, "lemma_id", object::word);

  field lemma::forms(inflection category)
  {
    return field::joinWhere(object::lemma, "form_id", object::form, inflectionCategory, static_cast<int>(category));
  }

  lemma::lemma(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
  }

  const form& lemma::getBaseForm() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized lemma");
    }

    if (!forms_.count(inflection::base))
    {
      initializeForm(inflection::base);
    }

    return forms_.at(inflection::base).front();
  }

  bool lemma::hasInflection(inflection category) const
  {
    return !getInflections(category).empty();
  }

  const std::vector<form>& lemma::getInflections(inflection category) const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized lemma");
    }

    if (!forms_.count(category))
    {
      initializeForm(category);
    }

    return forms_.at(category);
  }

  void lemma::initializeForm(inflection infl) const
  {
    forms_[infl] = db_->forms(form::lemmas %= ((inflectionCategory == infl) && *this), verbly::form::id, -1).all();
  }

};
