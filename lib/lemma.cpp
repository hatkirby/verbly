#include "lemma.h"
#include <sqlite3.h>
#include "database.h"
#include "query.h"

namespace verbly {

  const object lemma::objectType = object::lemma;

  const std::list<std::string> lemma::select = {"lemma_id"};

  const field lemma::id = field::integerField(object::lemma, "lemma_id");

  const field lemma::word = field::joinField(object::lemma, "lemma_id", object::word);

  const field lemma::formJoin = field::joinField(object::lemma, "form_id", object::form);
  const field lemma::inflectionCategory = field::integerField(object::lemma, "category");

  filter operator%=(lemma::inflection_field check, filter joinCondition)
  {
    return (lemma::formJoin %= joinCondition) && (lemma::inflectionCategory == check.getCategory());
  }

  lemma::inflection_field::operator filter() const
  {
    return (lemma::inflectionCategory == category_);
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
    forms_[infl] = db_->forms(form::lemma(infl) %= *this, false, -1).all();
  }

};
