#include "database.h"
#include <sqlite3.h>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <sstream>
#include "field.h"
#include "../lib/util.h"

namespace verbly {
  namespace generator {

    sqlite3_error::sqlite3_error(
      const std::string& what,
      const std::string& db_err) :
        what_(what + " (" + db_err + ")"),
        db_err_(db_err)
    {
    }

    const char* sqlite3_error::what() const noexcept
    {
      return what_.c_str();
    }

    const char* sqlite3_error::db_err() const noexcept
    {
      return db_err_.c_str();
    }

    database::database(std::string path)
    {
      // If there is already a file at this path, overwrite it.
      if (std::ifstream(path))
      {
        if (std::remove(path.c_str()))
        {
          throw std::logic_error("Could not overwrite file at path");
        }
      }

      if (sqlite3_open_v2(path.c_str(), &ppdb_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
      {
        // We still have to free the resources allocated. In the event that
        // allocation failed, ppdb will be null and sqlite3_close_v2 will just
        // ignore it.
        std::string errmsg(sqlite3_errmsg(ppdb_));
        sqlite3_close_v2(ppdb_);

        throw sqlite3_error("Could not create output datafile", errmsg);
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

    void database::runQuery(std::string query)
    {
      // This can only happen when doing bad things with move semantics.
      assert(ppdb_ != nullptr);

      sqlite3_stmt* ppstmt;

      if (sqlite3_prepare_v2(ppdb_, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw sqlite3_error("Error writing to database", sqlite3_errmsg(ppdb_));
      }

      int result = sqlite3_step(ppstmt);
      sqlite3_finalize(ppstmt);

      if (result != SQLITE_DONE)
      {
        throw sqlite3_error("Error writing to database", sqlite3_errmsg(ppdb_));
      }
    }

    void database::insertIntoTable(std::string table, std::list<field> fields)
    {
      // This can only happen when doing bad things with move semantics.
      assert(ppdb_ != nullptr);

      // This shouldn't happen.
      assert(!fields.empty());

      std::list<std::string> fieldNames;
      std::list<std::string> qs;
      for (field& f : fields)
      {
        fieldNames.push_back(f.getName());
        qs.push_back("?");
      }

      std::ostringstream query;
      query << "INSERT INTO ";
      query << table;
      query << " (";
      query << implode(std::begin(fieldNames), std::end(fieldNames), ", ");
      query << ") VALUES (";
      query << implode(std::begin(qs), std::end(qs), ", ");
      query << ")";

      std::string query_str = query.str();

      sqlite3_stmt* ppstmt;

      if (sqlite3_prepare_v2(ppdb_, query_str.c_str(), query_str.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw sqlite3_error("Error writing to database", sqlite3_errmsg(ppdb_));
      }

      int i = 1;
      for (field& f : fields)
      {
        switch (f.getType())
        {
          case field::type::integer:
          {
            sqlite3_bind_int(ppstmt, i, f.getInteger());

            break;
          }

          case field::type::string:
          {
            sqlite3_bind_text(ppstmt, i, f.getString().c_str(), f.getString().length(), SQLITE_TRANSIENT);

            break;
          }

          case field::type::invalid:
          {
            // Fields can only be invalid when doing bad things with move semantics.
            assert(false);

            break;
          }
        }

        i++;
      }

      int result = sqlite3_step(ppstmt);
      sqlite3_finalize(ppstmt);

      if (result != SQLITE_DONE)
      {
        throw sqlite3_error("Error writing to database", sqlite3_errmsg(ppdb_));
      }
    }

  };
};
