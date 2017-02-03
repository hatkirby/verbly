#ifndef FORM_H_3A6C962C
#define FORM_H_3A6C962C

#include <list>
#include <vector>
#include <string>
#include <stdexcept>
#include "field.h"
#include "pronunciation.h"
#include "filter.h"

struct sqlite3_stmt;

namespace verbly {

  class database;

  class form {
  public:

    // Default constructor

    form() = default;

    // Construct from database

    form(const database& db, sqlite3_stmt* row);

    // Accessors

    operator bool() const
    {
      return valid_;
    }

    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return id_;
    }

    std::string getText() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return text_;
    }

    int getComplexity() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return complexity_;
    }

    bool isProper() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return proper_;
    }

    const std::vector<pronunciation>& getPronunciations() const;

    // Convenience

    bool startsWithVowelSound() const;

    // Type info

    static const object objectType;

    static const std::list<std::string> select;

    // Query fields

    static const field id;
    static const field text;
    static const field complexity;
    static const field proper;

    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return (id == id_);
    }

    // Relationships to other objects

    static const field lemmas;

    static const field pronunciations;

  private:
    bool valid_ = false;

    int id_;
    std::string text_;
    int complexity_ ;
    bool proper_;

    const database* db_;

    mutable bool initializedPronunciations_ = false;
    mutable std::vector<pronunciation> pronunciations_;

  };

};

#endif /* end of include guard: FORM_H_3A6C962C */
