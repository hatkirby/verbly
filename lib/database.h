#ifndef DATABASE_H_0B0A47D2
#define DATABASE_H_0B0A47D2

#include <string>
#include <exception>
#include <stdexcept>
#include <list>
#include <set>
#include "notion.h"
#include "word.h"
#include "frame.h"
#include "part.h"
#include "form.h"
#include "pronunciation.h"
#include "order.h"

struct sqlite3;

namespace verbly {

  template <typename Object>
  class query;

  class database {
  public:

    // Constructor

    explicit database(std::string path);

    // Disable copying

    database(const database& other) = delete;
    database& operator=(const database& other) = delete;

    // Move constructor and move assignment

    database(database&& other);
    database& operator=(database&& other);

    // Swap

    friend void swap(database& first, database& second);

    // Destructor

    ~database();

    // Information

    int getMajorVersion() const
    {
      return major_;
    }

    int getMinorVersion() const
    {
      return minor_;
    }

    // Queries

    query<notion> notions(filter where, order sortOrder = {}, int limit = 1) const;

    query<word> words(filter where, order sortOrder = {}, int limit = 1) const;

    query<frame> frames(filter where, order sortOrder = {}, int limit = 1) const;

    query<part> parts(filter where, order sortOrder = {}, int limit = 1) const;

    query<form> forms(filter where, order sortOrder = {}, int limit = 1) const;

    query<pronunciation> pronunciations(filter where, order sortOrder = {}, int limit = 1) const;

    std::set<std::string> selrestrs(int partId) const;

    std::set<std::string> synrestrs(int partId) const;

  private:

    database() = default;

    sqlite3* ppdb_ = nullptr;

    int major_;
    int minor_;

  };

  class database_version_mismatch : public std::logic_error {
  public:

    database_version_mismatch(int right, int wrong) :
      std::logic_error(generateMessage(right, wrong)),
      right_(right),
      wrong_(wrong)
    {
    }

    int getRightVersion() const noexcept
    {
      return right_;
    }

    int getWrongVersion() const noexcept
    {
      return wrong_;
    }

  private:

    static std::string generateMessage(int right, int wrong);

    int right_;
    int wrong_;
  };

};

#endif /* end of include guard: DATABASE_H_0B0A47D2 */
