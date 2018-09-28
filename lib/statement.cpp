#include "statement.h"
#include <sstream>
#include <utility>
#include <hkutil/string.h>
#include "filter.h"
#include "order.h"

namespace verbly {

  statement::statement(
    object context,
    filter queryFilter) :
      statement(context, getTableForContext(context), queryFilter.compact().normalize(context))
  {
  }

  std::string statement::getQueryString(std::list<std::string> select, order sortOrder, int limit, bool debug) const
  {
    std::stringstream queryStream;

    if (!withs_.empty())
    {
      queryStream << "WITH RECURSIVE ";

      std::list<std::string> ctes;
      for (const with& cte : withs_)
      {
        std::stringstream cteStream;
        cteStream << cte.getIdentifier();
        cteStream << " AS (SELECT ";
        cteStream << cte.getTopTable();
        cteStream << ".* FROM ";
        cteStream << cte.getTableForId(cte.getTopTable());
        cteStream << " AS ";
        cteStream << cte.getTopTable();

        for (const join& j : cte.getJoins())
        {
          cteStream << " ";
          cteStream << j;
        }

        if (cte.getCondition().getType() != condition::type::empty)
        {
          cteStream << " WHERE ";
          cteStream << cte.getCondition().flatten().toSql(true, debug);
        }

        if (cte.isRecursive())
        {
          cteStream << " UNION SELECT l.* FROM ";
          cteStream << cte.getIdentifier();
          cteStream << " AS t INNER JOIN ";
          cteStream << cte.getField().getTable();
          cteStream << " AS j ON t.";
          cteStream << cte.getField().getColumn();
          cteStream << " = j.";
          cteStream << cte.getField().getForeignJoinColumn();
          cteStream << " INNER JOIN ";
          cteStream << cte.getTableForId(cte.getTopTable());
          cteStream << " AS l ON j.";
          cteStream << cte.getField().getJoinColumn();
          cteStream << " = l.";
          cteStream << cte.getField().getColumn();
        }

        cteStream << ")";

        ctes.push_back(cteStream.str());
      }

      queryStream << hatkirby::implode(std::begin(ctes), std::end(ctes), ", ");
      queryStream << " ";
    }

    std::list<std::string> realSelect;
    for (std::string& s : select)
    {
      realSelect.push_back(topTable_ + "." + s);
    }

    queryStream << "SELECT ";
    queryStream << hatkirby::implode(std::begin(realSelect), std::end(realSelect), ", ");
    queryStream << " FROM ";
    queryStream << tables_.at(topTable_);
    queryStream << " AS ";
    queryStream << topTable_;

    for (const join& j : joins_)
    {
      queryStream << " ";
      queryStream << j;
    }

    if (topCondition_.getType() != condition::type::empty)
    {
      queryStream << " WHERE ";
      queryStream << topCondition_.flatten().toSql(true, debug);
    }

    queryStream << " GROUP BY ";
    queryStream << topTable_;
    queryStream << ".";
    queryStream << select.front();

    queryStream << " ORDER BY ";

    switch (sortOrder.getType())
    {
      case order::type::random:
      {
        queryStream << "RANDOM()";

        break;
      }

      case order::type::field:
      {
        queryStream << topTable_;
        queryStream << ".";
        queryStream << sortOrder.getSortField().getColumn();

        break;
      }
    }

    if (limit > 0)
    {
      queryStream << " LIMIT ";
      queryStream << limit;
    }

    return queryStream.str();
  }

  std::list<hatkirby::binding> statement::getBindings() const
  {
    std::list<hatkirby::binding> result;

    for (const with& w : withs_)
    {
      for (hatkirby::binding value : w.getCondition().flattenBindings())
      {
        result.push_back(std::move(value));
      }
    }

    for (hatkirby::binding value : topCondition_.flattenBindings())
    {
      result.push_back(std::move(value));
    }

    return result;
  }

  statement::statement(
    object context,
    std::string tableName,
    filter clause,
    int nextTableId,
    int nextWithId) :
      context_(context),
      nextTableId_(nextTableId),
      nextWithId_(nextWithId),
      topTable_(instantiateTable(std::move(tableName))),
      topCondition_(parseFilter(std::move(clause)))
  {
  }

  /**
   * This function recursively parses the query's filter condition. It is not
   * idempotent. It returns a condition object representing the passed filter,
   * but it also modifies the statement object, specifically by adding any joins
   * and CTEs that may be required to represent the passed filter. This may also
   * involve instantiating tables.
   */
  statement::condition statement::parseFilter(filter clause)
  {
    switch (clause.getType())
    {
      case filter::type::empty:
      {
        return {};
      }

      case filter::type::singleton:
      {
        switch (clause.getField().getType())
        {
          case field::type::undefined:
          {
            return {};
          }

          // For primitive type filters, all we need to do is translate the
          // filter object directly into a condition object. No joins are
          // necessary.
          case field::type::string:
          case field::type::integer:
          case field::type::boolean:
          {
            switch (clause.getComparison())
            {
              case filter::comparison::is_null:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  true
                };
              }

              case filter::comparison::is_not_null:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  false
                };
              }

              case filter::comparison::int_equals:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::equals,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::int_does_not_equal:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::does_not_equal,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::int_is_at_least:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_at_least,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::int_is_greater_than:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_greater_than,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::int_is_at_most:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_at_most,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::int_is_less_than:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_less_than,
                  clause.getIntegerArgument()
                };
              }

              case filter::comparison::boolean_equals:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::equals,
                  clause.getBooleanArgument() ? 1 : 0
                };
              }

              case filter::comparison::string_equals:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::equals,
                  clause.getStringArgument()
                };
              }

              case filter::comparison::string_does_not_equal:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::does_not_equal,
                  clause.getStringArgument()
                };
              }

              case filter::comparison::string_is_like:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_like,
                  clause.getStringArgument()
                };
              }

              case filter::comparison::string_is_not_like:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::is_not_like,
                  clause.getStringArgument()
                };
              }

              case filter::comparison::field_equals:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::equals,
                  field_binding {"", clause.getCompareField().getColumn()},
                  clause.getCompareField().getObject()
                };
              }

              case filter::comparison::field_does_not_equal:
              {
                return {
                  topTable_,
                  clause.getField().getColumn(),
                  condition::comparison::does_not_equal,
                  field_binding {"", clause.getCompareField().getColumn()},
                  clause.getCompareField().getObject()
                };
              }

              case filter::comparison::matches:
              case filter::comparison::does_not_match:
              case filter::comparison::hierarchally_matches:
              case filter::comparison::does_not_hierarchally_match:
              {
                throw std::logic_error("Invalid comparison type for field");
              }
            }
          }

          case field::type::join:
          case field::type::join_where:
          {
            // First, figure out what table we need to join against.
            std::string joinTableName;
            object joinContext = object::undefined;
            if (clause.getField().hasTable())
            {
              joinTableName = clause.getField().getTable();
            } else {
              joinContext = clause.getField().getJoinObject();
              joinTableName = getTableForContext(joinContext);
            }

            filter joinCondition = clause.getJoinCondition();

            // If this is a condition join, we need to add the field join
            // condition to the clause.
            if (clause.getField().getType() == field::type::join_where)
            {
              joinCondition &= (field::integerField(joinTableName.c_str(), clause.getField().getConditionColumn()) == clause.getField().getConditionValue());
            }

            // Instantiate a CTE in case we need one. This is necessary because
            // the nextWithId is propagated in the instantiation of the joinStmt
            // below.
            std::string withName = instantiateWith(joinTableName);

            // Recursively parse the subquery, and therefore obtain an
            // instantiated table to join against, as well as any joins or CTEs
            // that the subquery may require to function.
            statement joinStmt(
              joinContext,
              joinTableName,
              std::move(joinCondition).normalize(clause.getField().getJoinObject()),
              nextTableId_,
              nextWithId_);

            std::string joinTable = joinStmt.topTable_;

            if (clause.getComparison() == filter::comparison::does_not_match)
            {
              // If the comparison is actually a negative filter, we can't just
              // integrate the subquery statement into this statement and then
              // join against it. Even if we LEFT JOIN against the subquery's
              // top level table and then condition on the join column being
              // NULL, if that table joins against any other table, the query
              // will return zero results. Instead, we create a non-recursive
              // CTE that represents the subquery, then LEFT JOIN against it and
              // condition on the join column being NULL as before.
              std::string withInstName = instantiateTable(withName);

              // LEFT JOIN against the CTE.
              joins_.emplace_back(
                true,
                withName,
                topTable_,
                clause.getField().getColumn(),
                withInstName,
                clause.getField().getColumn());

              // All CTEs have to be in the main statement, so integrate any
              // CTEs that our subquery uses. Also, retrieve the table mapping,
              // joins list, and subquery condition, and use them to create the
              // CTE.
              std::map<std::string, std::string> cteTables = std::move(joinStmt.tables_);
              std::list<join> cteJoins = std::move(joinStmt.joins_);
              condition cteCondition = integrate(std::move(joinStmt), true);

              withs_.emplace_back(
                std::move(withName),
                clause.getField(),
                std::move(cteTables),
                std::move(joinTable),
                std::move(cteCondition),
                std::move(cteJoins),
                false);

              // Condition on the join column being NULL, which causes the query
              // to only return results that do not match the subquery.
              return condition(std::move(withInstName), clause.getField().getColumn(), true);
            } else {
              // INNER JOIN against the top table of the subquery.
              joins_.emplace_back(
                false,
                std::move(joinTableName),
                topTable_,
                clause.getField().getColumn(),
                std::move(joinTable),
                clause.getField().getColumn());

              // Integrate the subquery's table mappings, joins, and CTEs into
              // this statement, and return the subquery condition as our
              // condition.
              return integrate(std::move(joinStmt));
            }
          }

          case field::type::join_through:
          case field::type::join_through_where:
          {
            // Instantiate a CTE in case we need one. This is necessary because
            // the nextWithId is propagated in the instantiation of the joinStmt
            // below.
            std::string withName = instantiateWith(clause.getField().getTable());

            // Recursively parse the subquery, and therefore obtain an
            // instantiated table to join against, as well as any joins or CTEs
            // that the subquery may require to function.
            statement joinStmt(
              clause.getField().getJoinObject(),
              getTableForContext(clause.getField().getJoinObject()),
              clause.getJoinCondition().normalize(clause.getField().getJoinObject()),
              nextTableId_,
              nextWithId_);

            std::string joinTable = joinStmt.topTable_;

            if (clause.getComparison() == filter::comparison::does_not_match)
            {
              // If the comparison is actually a negative filter, we can't just
              // integrate the subquery statement into this statement and then
              // join against it. Even if we LEFT JOIN against the subquery's
              // through table and then condition on the join column being NULL,
              // the query will return zero results because the through table
              // joins against the subquery's top level table. Instead, we
              // create a non-recursive CTE that represents the through table
              // joined against the subquery, then LEFT JOIN against it and
              // condition on the join column being NULL as before.
              std::string withInstName = instantiateTable(withName);

              // LEFT JOIN against the CTE.
              joins_.emplace_back(
                true,
                withName,
                topTable_,
                clause.getField().getColumn(),
                withInstName,
                clause.getField().getJoinColumn());

              // Modify the substatement such that the through table is the top
              // table, and such that it joins against the previous top table.
              std::string throughTable = joinStmt.instantiateTable(clause.getField().getTable());
              joinStmt.joins_.emplace_back(
                false,
                getTableForContext(clause.getField().getJoinObject()),
                throughTable,
                clause.getField().getForeignJoinColumn(),
                std::move(joinTable),
                clause.getField().getForeignColumn());

              joinStmt.topTable_ = throughTable;

              // All CTEs have to be in the main statement, so integrate any
              // CTEs that our subquery uses. Also, retrieve the table mapping,
              // joins list, and subquery condition, and use them to create the
              // CTE.
              std::map<std::string, std::string> cteTables = std::move(joinStmt.tables_);
              std::list<join> cteJoins = std::move(joinStmt.joins_);
              condition cteCondition = integrate(std::move(joinStmt), true);

              // If this is a condition join, add the condition.
              if (clause.getField().getType() == field::type::join_through_where)
              {
                cteCondition &=
                  condition(
                    throughTable,
                    clause.getField().getConditionColumn(),
                    condition::comparison::equals,
                    clause.getField().getConditionValue());
              }

              withs_.emplace_back(
                std::move(withName),
                clause.getField(),
                std::move(cteTables),
                std::move(throughTable),
                std::move(cteCondition),
                std::move(cteJoins),
                false);

              // Condition on the join column being NULL, which causes the query
              // to only return results that do not match the subquery.
              return condition(std::move(withInstName), clause.getField().getJoinColumn(), true);
            } else {
              // Instantiate the through table.
              std::string throughTable = instantiateTable(clause.getField().getTable());

              // INNER JOIN against the through table.
              joins_.emplace_back(
                false,
                clause.getField().getTable(),
                topTable_,
                clause.getField().getColumn(),
                throughTable,
                clause.getField().getJoinColumn());

              // INNER JOIN from the through table to the top table of the subquery.
              joins_.emplace_back(
                false,
                getTableForContext(clause.getField().getJoinObject()),
                throughTable,
                clause.getField().getForeignJoinColumn(),
                std::move(joinTable),
                clause.getField().getForeignColumn());

              // Integrate the subquery's table mappings, joins, and CTEs into
              // this statement, and return the subquery condition as our
              // condition.
              condition resultCond = integrate(std::move(joinStmt));

              // If this is a condition join, add the condition.
              if (clause.getField().getType() == field::type::join_through_where)
              {
                resultCond &=
                  condition(
                    throughTable,
                    clause.getField().getConditionColumn(),
                    condition::comparison::equals,
                    clause.getField().getConditionValue());
              }

              return std::move(resultCond);
            }
          }

          case field::type::hierarchal_join:
          {
            // Create a recursive CTE that represents the results of the subquery.
            std::string withName = instantiateWith(clause.getField().getTable());
            std::string withInstName = instantiateTable(withName);

            // If we are matching against the subquery, we INNER JOIN with the
            // CTE. If we are negatively matching the subquery, we LEFT JOIN
            // with the CTE.
            bool outer = false;
            if (clause.getComparison() == filter::comparison::does_not_hierarchally_match)
            {
              outer = true;
            }

            // Join against the CTE.
            joins_.emplace_back(
              outer,
              withName,
              topTable_,
              clause.getField().getColumn(),
              withInstName,
              clause.getField().getColumn());

            // Recursively parse the subquery in order to create the CTE.
            statement withStmt(
              clause.getField().getObject(),
              getTableForContext(clause.getField().getObject()),
              clause.getJoinCondition().normalize(clause.getField().getObject()),
              nextTableId_,
              nextWithId_);

            // All CTEs have to be in the main statement, so integrate any CTEs
            // that our subquery uses. Also, retrieve the table mapping, joins
            // list, and subquery condition, and use them to create the CTE.
            std::string cteTopTable = std::move(withStmt.topTable_);
            std::map<std::string, std::string> cteTables = std::move(withStmt.tables_);
            std::list<join> cteJoins = std::move(withStmt.joins_);
            condition cteCondition = integrate(std::move(withStmt), true);

            withs_.emplace_back(
              std::move(withName),
              clause.getField(),
              std::move(cteTables),
              std::move(cteTopTable),
              std::move(cteCondition),
              std::move(cteJoins),
              true);

            // If we are matching against the subquery, no condition is
            // necessary. If we are negatively matching the subquery, we
            // condition on the join column being NULL.
            if (clause.getComparison() == filter::comparison::does_not_hierarchally_match)
            {
              return condition(withInstName, clause.getField().getColumn(), true);
            } else {
              return {};
            }
          }
        }
      }

      case filter::type::group:
      {
        condition grp(clause.getOrlogic());

        for (const filter& child : clause)
        {
          condition newChild = parseFilter(child);
          if (newChild.getType() != condition::type::empty)
          {
            grp += std::move(newChild);
          }
        }

        if (grp.getChildren().empty())
        {
          grp = {};
        }

        return grp;
      }

      case filter::type::mask:
      {
        condition result = parseFilter(clause.getMaskFilter());

        if (result.getType() == condition::type::empty)
        {
          return {};
        } else {
          return result;
        }
      }
    }
  }

  std::string statement::instantiateTable(std::string name)
  {
    std::string identifier = name + "_" + std::to_string(nextTableId_++);
    tables_[identifier] = name;

    return identifier;
  }

  std::string statement::instantiateWith(std::string name)
  {
    return name + "_tree_" + std::to_string(nextWithId_++);
  }

  /**
   * This method integrates the parts of a recursively generated statement into
   * this statement. This is used because filters are recursive objects, but
   * statements need to be flat to be compiled into a SQL query. Thus, all CTEs
   * have to be in the main statement, and all table mappings & joins that
   * aren't part of a CTE have to be in the main statement as well. Finally, we
   * need to copy up the next ID fields in order to properly prevent ID reuse.
   */
  statement::condition statement::integrate(statement subStmt, bool cte)
  {
    if (!cte)
    {
      for (auto& mapping : subStmt.tables_)
      {
        tables_[mapping.first] = mapping.second;
      }

      for (auto& j : subStmt.joins_)
      {
        joins_.push_back(j);
      }
    }

    for (auto& w : subStmt.withs_)
    {
      withs_.push_back(w);
    }

    nextTableId_ = subStmt.nextTableId_;
    nextWithId_ = subStmt.nextWithId_;

    return subStmt.topCondition_.resolveCompareFields(context_, topTable_);
  }

  std::ostream& operator<<(std::ostream& oss, const statement::join& j)
  {
    if (j.isOuterJoin())
    {
      oss << "LEFT";
    } else {
      oss << "INNER";
    }

    return oss
      << " JOIN "
      << j.getForeignTableName()
      << " AS "
      << j.getForeignTable()
      << " ON "
      << j.getForeignTable()
      << "."
      << j.getForeignColumn()
      << " = "
      << j.getJoinTable()
      << "."
      << j.getJoinColumn();
  }

  statement::condition::condition(
    std::string table,
    std::string column,
    bool isNull) :
      type_(type::singleton),
      variant_(singleton_type {
        std::move(table),
        std::move(column),
        isNull ? comparison::is_null : comparison::is_not_null,
        {},
        object::undefined
      })
  {
  }

  statement::condition::condition(
    std::string table,
    std::string column,
    comparison comp,
    binding value,
    object parentObject) :
      type_(type::singleton),
      variant_(singleton_type {
        std::move(table),
        std::move(column),
        comp,
        std::move(value),
        parentObject
      })
  {
  }

  std::string statement::condition::toSql(bool toplevel, bool debug) const
  {
    std::ostringstream sql;

    switch (type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        const singleton_type& singleton = mpark::get<singleton_type>(variant_);

        sql << singleton.table << "." << singleton.column;

        switch (singleton.comparison)
        {
          case comparison::equals:
          case comparison::does_not_equal:
          {
            if (singleton.comparison == comparison::equals)
            {
              sql << " = ";
            } else {
              sql << " != ";
            }

            if (mpark::holds_alternative<field_binding>(singleton.value))
            {
              sql << std::get<0>(mpark::get<field_binding>(singleton.value))
                << "."
                << std::get<1>(mpark::get<field_binding>(singleton.value));
            } else if (debug)
            {
              if (mpark::holds_alternative<std::string>(singleton.value))
              {
                sql << "\"" << mpark::get<std::string>(singleton.value) << "\"";
              }
              else if (mpark::holds_alternative<int>(singleton.value))
              {
                sql << mpark::get<int>(singleton.value);
              }
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_greater_than:
          {
            sql << " > ";

            if (debug)
            {
              sql << mpark::get<int>(singleton.value);
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_at_most:
          {
            sql << " <= ";

            if (debug)
            {
              sql << mpark::get<int>(singleton.value);
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_less_than:
          {
            sql << " < ";

            if (debug)
            {
              sql << mpark::get<int>(singleton.value);
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_at_least:
          {
            sql << " >= ";

            if (debug)
            {
              sql << mpark::get<int>(singleton.value);
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_like:
          {
            sql << " LIKE ";

            if (debug)
            {
              sql << "\"" << mpark::get<std::string>(singleton.value) << "\"";
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_not_like:
          {
            sql << " NOT LIKE ";

            if (debug)
            {
              sql << "\"" << mpark::get<std::string>(singleton.value) << "\"";
            } else {
              sql << "?";
            }

            break;
          }

          case comparison::is_not_null:
          {
            sql << " IS NOT NULL";

            break;
          }

          case comparison::is_null:
          {
            sql << " IS NULL";

            break;
          }
        }

        break;
      }

      case type::group:
      {
        const group_type& group = mpark::get<group_type>(variant_);

        std::list<std::string> clauses;
        for (const condition& cond : group.children)
        {
          clauses.push_back(cond.toSql(false, debug));
        }

        if (clauses.size() == 1)
        {
          sql << clauses.front();
        } else if (!clauses.empty())
        {
          if (!toplevel)
          {
            sql << "(";
          }

          sql <<
            hatkirby::implode(
              std::begin(clauses),
              std::end(clauses),
              group.orlogic ? " OR " : " AND ");

          if (!toplevel)
          {
            sql << ")";
          }
        }

        break;
      }
    }

    return sql.str();
  }

  std::list<hatkirby::binding> statement::condition::flattenBindings() const
  {
    switch (type_)
    {
      case type::empty:
      {
        return {};
      }

      case type::singleton:
      {
        const singleton_type& singleton = mpark::get<singleton_type>(variant_);

        if (mpark::holds_alternative<std::string>(singleton.value))
        {
          return {{ mpark::get<std::string>(singleton.value) }};
        } else if (mpark::holds_alternative<int>(singleton.value))
        {
          return {{ mpark::get<int>(singleton.value) }};
        } else {
          return {};
        }
      }

      case type::group:
      {
        const group_type& group = mpark::get<group_type>(variant_);

        std::list<hatkirby::binding> bindings;
        for (const condition& cond : group.children)
        {
          for (hatkirby::binding value : cond.flattenBindings())
          {
            bindings.push_back(std::move(value));
          }
        }

        return bindings;
      }
    }
  }

  statement::condition::condition(
    bool orlogic) :
      type_(type::group),
      variant_(group_type { {}, orlogic })
  {
  }

  statement::condition& statement::condition::operator+=(condition n)
  {
    if (type_ != type::group)
    {
      throw std::domain_error("Cannot add condition to non-group condition");
    }

    group_type& group = mpark::get<group_type>(variant_);
    group.children.emplace_back(std::move(n));

    return *this;
  }

  statement::condition& statement::condition::operator&=(condition n)
  {
    switch (type_)
    {
      case type::empty:
      {
        *this = std::move(n);

        break;
      }

      case type::singleton:
      {
        condition grp(false);
        grp += *this;
        grp += std::move(n);

        *this = grp;

        break;
      }

      case type::group:
      {
        *this += std::move(n);

        break;
      }
    }

    return *this;
  }

  const std::list<statement::condition>& statement::condition::getChildren()
    const
  {
    if (type_ != type::group)
    {
      throw std::domain_error("Cannot get children of non-group condition");
    }

    const group_type& group = mpark::get<group_type>(variant_);

    return group.children;
  }

  statement::condition statement::condition::flatten() const
  {
    switch (type_)
    {
      case type::empty:
      case type::singleton:
      {
        return *this;
      }

      case type::group:
      {
        const group_type& group = mpark::get<group_type>(variant_);

        condition result(group.orlogic);

        for (const condition& child : group.children)
        {
          condition newChild = child.flatten();

          if (newChild.type_ == type::group)
          {
            group_type& childGroup =
              mpark::get<group_type>(newChild.variant_);

            if (childGroup.orlogic == group.orlogic)
            {
              for (condition subChild : std::move(childGroup.children))
              {
                result += std::move(subChild);
              }
            } else {
              result += std::move(newChild);
            }
          } else {
            result += std::move(newChild);
          }
        }

        return result;
      }
    }
  }

  statement::condition statement::condition::resolveCompareFields(
    object context,
    std::string tableName) const
  {
    switch (type_)
    {
      case type::empty:
      {
        return *this;
      }

      case type::singleton:
      {
        const singleton_type& singleton = mpark::get<singleton_type>(variant_);

        if (singleton.parentObject != object::undefined &&
            singleton.parentObject == context)
        {
          return {
            singleton.table,
            singleton.column,
            singleton.comparison,
            field_binding {
              tableName,
              std::get<1>(mpark::get<field_binding>(singleton.value))
            }
          };
        } else {
          return *this;
        }
      }

      case type::group:
      {
        const group_type& group = mpark::get<group_type>(variant_);

        condition result(group.orlogic);
        for (const condition& cond : group.children)
        {
          result += cond.resolveCompareFields(context, tableName);
        }

        return result;
      }
    }
  }

};
