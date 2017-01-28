#ifndef QUERY_H_7CC5284C
#define QUERY_H_7CC5284C

#include <vector>
#include <stdexcept>
#include <string>
#include <list>
#include <sqlite3.h>
#include <iostream>
#include "statement.h"
#include "binding.h"
#include "order.h"

namespace verbly {

  class database_error : public std::logic_error {
  public:

    database_error(std::string msg, std::string sqlMsg) : std::logic_error(msg + " (" + sqlMsg + ")")
    {
    }
  };

  template <typename Object>
  class query {
  public:

    query(const database& db, sqlite3* ppdb, filter queryFilter, order sortOrder, int limit) : db_(&db)
    {
      if ((sortOrder.getType() == order::type::field)
        && (sortOrder.getSortField().getObject() != Object::objectType))
      {
        throw std::invalid_argument("Can only sort query by a field in the result table");
      }

      statement stmt(Object::objectType, std::move(queryFilter));

      std::string queryString = stmt.getQueryString(Object::select, std::move(sortOrder), limit);
      std::list<binding> bindings = stmt.getBindings();

      if (sqlite3_prepare_v2(ppdb, queryString.c_str(), queryString.length(), &ppstmt_, NULL) != SQLITE_OK)
      {
        std::string errorMsg = sqlite3_errmsg(ppdb);
        sqlite3_finalize(ppstmt_);

        throw database_error("Error preparing query", errorMsg);
      }

      int i = 1;
      for (const binding& value : bindings)
      {
        switch (value.getType())
        {
          case binding::type::integer:
          {
            if (sqlite3_bind_int(ppstmt_, i, value.getInteger()) != SQLITE_OK)
            {
              std::string errorMsg = sqlite3_errmsg(ppdb);
              sqlite3_finalize(ppstmt_);

              throw database_error("Error binding value to query", errorMsg);
            }

            break;
          }

          case binding::type::string:
          {
            if (sqlite3_bind_text(ppstmt_, i, value.getString().c_str(), value.getString().length(), SQLITE_TRANSIENT) != SQLITE_OK)
            {
              std::string errorMsg = sqlite3_errmsg(ppdb);
              sqlite3_finalize(ppstmt_);

              throw database_error("Error binding value to query", errorMsg);
            }

            break;
          }

          case binding::type::invalid:
          {
            throw std::logic_error("Cannot use invalid bindings");
          }
        }

        i++;
      }
    }

    ~query()
    {
      sqlite3_finalize(ppstmt_);
    }

    std::vector<Object> all() const
    {
      std::vector<Object> result;

      while (sqlite3_step(ppstmt_) == SQLITE_ROW)
      {
        result.emplace_back(*db_, ppstmt_);
      }

      sqlite3_reset(ppstmt_);

      return result;
    }

    Object first() const
    {
      std::vector<Object> results = all();
      if (!results.empty())
      {
        return results.front();
      } else {
        throw std::logic_error("query returned empty dataset");
      }
    }

  private:
    const database* db_;
    sqlite3_stmt* ppstmt_;

  };

};

#endif /* end of include guard: QUERY_H_7CC5284C */
