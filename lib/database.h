#ifndef DATABASE_H_0B0A47D2
#define DATABASE_H_0B0A47D2

#include <string>
#include <exception>
#include <list>
#include <set>
#include "notion.h"
#include "word.h"
#include "frame.h"
#include "part.h"
#include "lemma.h"
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

    // Queries

    query<notion> notions(filter where, order sortOrder = {}, int limit = 1) const;

    query<word> words(filter where, order sortOrder = {}, int limit = 1) const;

    query<frame> frames(filter where, order sortOrder = {}, int limit = 1) const;

    query<part> parts(filter where, order sortOrder = {}, int limit = 1) const;

    query<lemma> lemmas(filter where, order sortOrder = {}, int limit = 1) const;

    query<form> forms(filter where, order sortOrder = {}, int limit = 1) const;

    query<pronunciation> pronunciations(filter where, order sortOrder = {}, int limit = 1) const;

    std::set<std::string> synrestrs(int partId) const;

  private:

    database() = default;

    sqlite3* ppdb_ = nullptr;

  };

};

#endif /* end of include guard: DATABASE_H_0B0A47D2 */
