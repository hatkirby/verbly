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

  query<notion> database::notions(filter where, order sortOrder, int limit) const
  {
    return query<notion>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<word> database::words(filter where, order sortOrder, int limit) const
  {
    return query<word>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<frame> database::frames(filter where, order sortOrder, int limit) const
  {
    return query<frame>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<part> database::parts(filter where, order sortOrder, int limit) const
  {
    return query<part>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<lemma> database::lemmas(filter where, order sortOrder, int limit) const
  {
    return query<lemma>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<form> database::forms(filter where, order sortOrder, int limit) const
  {
    return query<form>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<pronunciation> database::pronunciations(filter where, order sortOrder, int limit) const
  {
    return query<pronunciation>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  std::set<std::string> database::synrestrs(int partId) const
  {
    std::string queryString = "SELECT synrestr FROM synrestrs WHERE part_id = ?";

    sqlite3_stmt* ppstmt;
    if (sqlite3_prepare_v2(ppdb_, queryString.c_str(), queryString.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      std::string errorMsg = sqlite3_errmsg(ppdb_);
      sqlite3_finalize(ppstmt);

      throw database_error("Error preparing query", errorMsg);
    }

    if (sqlite3_bind_int(ppstmt, 1, partId) != SQLITE_OK)
    {
      std::string errorMsg = sqlite3_errmsg(ppdb_);
      sqlite3_finalize(ppstmt);

      throw database_error("Error binding value to query", errorMsg);
    }

    std::set<std::string> result;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      result.insert(reinterpret_cast<const char*>(sqlite3_column_blob(ppstmt, 0)));
    }

    sqlite3_finalize(ppstmt);

    return result;
  }

};
