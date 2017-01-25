#include "database.h"
#include <sqlite3.h>
#include <stdexcept>
#include "query.h"

namespace verbly {

  database::database(std::string path)
  {
    if (sqlite3_open_v2(path.c_str(), &ppdb_, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
    {
      // We still have to free the resources allocated. In the event that
      // allocation failed, ppdb will be null and sqlite3_close_v2 will just
      // ignore it.
      std::string errmsg(sqlite3_errmsg(ppdb_));
      sqlite3_close_v2(ppdb_);

      throw database_error("Could not open verbly datafile", errmsg);
    }
  }

  database::database(database&& other) : database()
  {
    swap(*this, other);
  }

  database& database::operator=(database&& other)
  {
    swap(*this, other);

    return *this;
  }

  void swap(database& first, database& second)
  {
    std::swap(first.ppdb_, second.ppdb_);
  }

  database::~database()
  {
    sqlite3_close_v2(ppdb_);
  }

  query<notion> database::notions(filter where, bool random, int limit) const
  {
    return query<notion>(*this, ppdb_, std::move(where), random, limit);
  }

  query<word> database::words(filter where, bool random, int limit) const
  {
    return query<word>(*this, ppdb_, std::move(where), random, limit);
  }

  query<group> database::groups(filter where, bool random, int limit) const
  {
    return query<group>(*this, ppdb_, std::move(where), random, limit);
  }

  query<frame> database::frames(filter where, bool random, int limit) const
  {
    return query<frame>(*this, ppdb_, std::move(where), random, limit);
  }

  query<lemma> database::lemmas(filter where, bool random, int limit) const
  {
    return query<lemma>(*this, ppdb_, std::move(where), random, limit);
  }

  query<form> database::forms(filter where, bool random, int limit) const
  {
    return query<form>(*this, ppdb_, std::move(where), random, limit);
  }

  query<pronunciation> database::pronunciations(filter where, bool random, int limit) const
  {
    return query<pronunciation>(*this, ppdb_, std::move(where), random, limit);
  }

};
