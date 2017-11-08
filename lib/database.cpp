#include "database.h"
#include <sqlite3.h>
#include <stdexcept>
#include <sstream>
#include "query.h"
#include "version.h"

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

    std::string queryString = "SELECT major, minor FROM version";

    sqlite3_stmt* ppstmt;
    if (sqlite3_prepare_v2(ppdb_, queryString.c_str(), queryString.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      std::string errorMsg = sqlite3_errmsg(ppdb_);
      sqlite3_finalize(ppstmt);

      throw database_error("Error reading database version", errorMsg);
    }

    if (sqlite3_step(ppstmt) != SQLITE_ROW)
    {
      std::string errorMsg = sqlite3_errmsg(ppdb_);
      sqlite3_finalize(ppstmt);

      throw database_error("Error reading database version", errorMsg);
    }

    major_ = sqlite3_column_int(ppstmt, 0);
    minor_ = sqlite3_column_int(ppstmt, 1);

    sqlite3_finalize(ppstmt);

    if (major_ != DATABASE_MAJOR_VERSION)
    {
      throw database_version_mismatch(DATABASE_MAJOR_VERSION, major_);
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

  query<form> database::forms(filter where, order sortOrder, int limit) const
  {
    return query<form>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<pronunciation> database::pronunciations(filter where, order sortOrder, int limit) const
  {
    return query<pronunciation>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  std::set<std::string> database::selrestrs(int partId) const
  {
    std::string queryString = "SELECT selrestr FROM selrestrs WHERE part_id = ?";

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

  std::string database_version_mismatch::generateMessage(int right, int wrong)
  {
    std::ostringstream msgbuilder;
    msgbuilder << "Incompatible database version: needed ";
    msgbuilder << right;
    msgbuilder << ", found ";
    msgbuilder << wrong;

    return msgbuilder.str();
  }

};
