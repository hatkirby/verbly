#ifndef FORM_H_3A6C962C
#define FORM_H_3A6C962C

#include <list>
#include <vector>
#include <string>
#include <stdexcept>
#include <hkutil/database.h>
#include "field.h"
#include "pronunciation.h"
#include "filter.h"

namespace verbly {

  class database;

  class form {
  public:

    // Default constructor

    form() = default;

    // Construct from database

    form(const database& db, hatkirby::row row);

    // Accessors

    bool isValid() const
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

    const std::string& getText() const
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

    int getLength() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return length_;
    }

    bool hasFrequency() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return hasFreq_;
    }

    bool getFrequency() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      if (!hasFreq_)
      {
        throw std::domain_error("Form does not have a frequency");
      }

      return frequency_;
    }

    const std::vector<pronunciation>& getPronunciations() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return pronunciations_;
    }

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
    static const field length;
    static const field frequency;

    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return (id == id_);
    }

    filter operator!() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized form");
      }

      return (id != id_);
    }

    // Relationships to other objects

    static field words(inflection category);

    static const field pronunciations;

    static const field anagrams;

    static const field merographs;
    static const field holographs;

  private:

    bool valid_ = false;
    int id_;
    std::string text_;
    int complexity_;
    bool proper_;
    int length_;
    bool hasFreq_ = false;
    int frequency_;
    std::vector<pronunciation> pronunciations_;
  };

};

#endif /* end of include guard: FORM_H_3A6C962C */
