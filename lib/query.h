#ifndef QUERY_H_7CC5284C
#define QUERY_H_7CC5284C

#include <vector>
#include <stdexcept>
#include <string>
#include <list>
#include <hkutil/database.h>
#include "statement.h"
#include "order.h"

namespace verbly {

  class database_error : public std::logic_error {
  public:

    database_error(
      std::string msg,
      std::string sqlMsg) :
        std::logic_error(msg + " (" + sqlMsg + ")")
    {
    }
  };

  template <typename Object>
  class query {
  public:

    query(
      const database& db,
      hatkirby::database& ppdb,
      filter queryFilter,
      order sortOrder,
      int limit) :
        db_(db),
        ppdb_(ppdb)
    {
      if ((sortOrder.getType() == order::type::field)
        && (sortOrder.getSortField().getObject() != Object::objectType))
      {
        throw std::invalid_argument(
          "Can only sort query by a field in the result table");
      }

      statement stmt(Object::objectType, std::move(queryFilter));
      queryString_ =
        stmt.getQueryString(Object::select, std::move(sortOrder), limit);
      

      bindings_ = stmt.getBindings();
    }

    std::vector<Object> all() const
    {
      std::vector<hatkirby::row> rows =
        ppdb_.queryAll(queryString_, bindings_);

      std::vector<Object> result;

      for (hatkirby::row& r : rows)
      {
        result.emplace_back(db_, std::move(r));
      }

      return result;
    }

    Object first() const
    {
      return { db_, ppdb_.queryFirst(queryString_, bindings_) };
    }

  private:

    const database& db_;
    hatkirby::database& ppdb_;

    std::string queryString_;
    std::list<hatkirby::binding> bindings_;
  };

};

#endif /* end of include guard: QUERY_H_7CC5284C */
