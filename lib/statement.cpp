#include "statement.h"
#include <sstream>
#include <utility>
#include "filter.h"
#include "util.h"
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

      queryStream << implode(std::begin(ctes), std::end(ctes), ", ");
      queryStream << " ";
    }

    std::list<std::string> realSelect;
    for (std::string& s : select)
    {
      realSelect.push_back(topTable_ + "." + s);
    }

    queryStream << "SELECT ";
    queryStream << implode(std::begin(realSelect), std::end(realSelect), ", ");
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

  std::list<binding> statement::getBindings() const
  {
    std::list<binding> result;

    for (const with& w : withs_)
    {
      for (binding value : w.getCondition().flattenBindings())
      {
        result.push_back(std::move(value));
      }
    }

    for (binding value : topCondition_.flattenBindings())
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
                return condition(topTable_, clause.getField().getColumn(), true);
              }

              case filter::comparison::is_not_null:
              {
                return condition(topTable_, clause.getField().getColumn(), false);
              }

              case filter::comparison::int_equals:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::equals, clause.getIntegerArgument());
              }

              case filter::comparison::int_does_not_equal:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::does_not_equal, clause.getIntegerArgument());
              }

              case filter::comparison::int_is_at_least:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_at_least, clause.getIntegerArgument());
              }

              case filter::comparison::int_is_greater_than:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_greater_than, clause.getIntegerArgument());
              }

              case filter::comparison::int_is_at_most:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_at_most, clause.getIntegerArgument());
              }

              case filter::comparison::int_is_less_than:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_less_than, clause.getIntegerArgument());
              }

              case filter::comparison::boolean_equals:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::equals, clause.getBooleanArgument() ? 1 : 0);
              }

              case filter::comparison::string_equals:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::equals, clause.getStringArgument());
              }

              case filter::comparison::string_does_not_equal:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::does_not_equal, clause.getStringArgument());
              }

              case filter::comparison::string_is_like:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_like, clause.getStringArgument());
              }

              case filter::comparison::string_is_not_like:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::is_not_like, clause.getStringArgument());
              }

              case filter::comparison::field_equals:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::equals, {"", clause.getCompareField().getColumn()}, clause.getCompareField().getObject());
              }

              case filter::comparison::field_does_not_equal:
              {
                return condition(topTable_, clause.getField().getColumn(), condition::comparison::does_not_equal, {"", clause.getCompareField().getColumn()}, clause.getCompareField().getObject());
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

  statement::condition::condition(const condition& other)
  {
    type_ = other.type_;

    switch (type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        new(&singleton_.table_) std::string(other.singleton_.table_);
        new(&singleton_.column_) std::string(other.singleton_.column_);
        singleton_.comparison_ = other.singleton_.comparison_;
        new(&singleton_.value_) binding(other.singleton_.value_);
        singleton_.parentObject_ = other.singleton_.parentObject_;

        break;
      }

      case type::group:
      {
        new(&group_.children_) std::list<condition>(other.group_.children_);
        group_.orlogic_ = other.group_.orlogic_;

        break;
      }
    }
  }

  statement::condition::condition(condition&& other) : condition()
  {
    swap(*this, other);
  }

  statement::condition& statement::condition::operator=(condition other)
  {
    swap(*this, other);

    return *this;
  }

  void swap(statement::condition& first, statement::condition& second)
  {
    using type = statement::condition::type;
    using condition = statement::condition;

    type tempType = first.type_;
    std::string tempTable;
    std::string tempColumn;
    condition::comparison tempComparison;
    binding tempBinding;
    object tempParentObject;
    std::list<condition> tempChildren;
    bool tempOrlogic;

    switch (tempType)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        tempTable = std::move(first.singleton_.table_);
        tempColumn = std::move(first.singleton_.column_);
        tempComparison = first.singleton_.comparison_;
        tempBinding = std::move(first.singleton_.value_);
        tempParentObject = first.singleton_.parentObject_;

        break;
      }

      case type::group:
      {
        tempChildren = std::move(first.group_.children_);
        tempOrlogic = first.group_.orlogic_;

        break;
      }
    }

    first.~condition();

    first.type_ = second.type_;

    switch (first.type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        new(&first.singleton_.table_) std::string(std::move(second.singleton_.table_));
        new(&first.singleton_.column_) std::string(std::move(second.singleton_.column_));
        first.singleton_.comparison_ = second.singleton_.comparison_;
        new(&first.singleton_.value_) binding(std::move(second.singleton_.value_));
        first.singleton_.parentObject_ = second.singleton_.parentObject_;

        break;
      }

      case type::group:
      {
        new(&first.group_.children_) std::list<condition>(std::move(second.group_.children_));
        first.group_.orlogic_ = second.group_.orlogic_;

        break;
      }
    }

    second.~condition();

    second.type_ = tempType;

    switch (second.type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        new(&second.singleton_.table_) std::string(std::move(tempTable));
        new(&second.singleton_.column_) std::string(std::move(tempColumn));
        second.singleton_.comparison_ = tempComparison;
        new(&second.singleton_.value_) binding(std::move(tempBinding));
        second.singleton_.parentObject_ = tempParentObject;

        break;
      }

      case type::group:
      {
        new(&second.group_.children_) std::list<condition>(std::move(tempChildren));
        second.group_.orlogic_ = tempOrlogic;

        break;
      }
    }
  }

  statement::condition::~condition()
  {
    switch (type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        using string_type = std::string;

        singleton_.table_.~string_type();
        singleton_.column_.~string_type();
        singleton_.value_.~binding();

        break;
      }

      case type::group:
      {
        using list_type = std::list<condition>;

        group_.children_.~list_type();

        break;
      }
    }
  }

  statement::condition::condition() : type_(type::empty)
  {
  }

  statement::condition::condition(
    std::string table,
    std::string column,
    bool isNull) :
      type_(type::singleton)
  {
    new(&singleton_.table_) std::string(std::move(table));
    new(&singleton_.column_) std::string(std::move(column));

    if (isNull)
    {
      singleton_.comparison_ = comparison::is_null;
    } else {
      singleton_.comparison_ = comparison::is_not_null;
    }

    singleton_.parentObject_ = object::undefined;
  }

  statement::condition::condition(
    std::string table,
    std::string column,
    comparison comp,
    binding value,
    object parentObject) :
      type_(type::singleton)
  {
    new(&singleton_.table_) std::string(std::move(table));
    new(&singleton_.column_) std::string(std::move(column));
    singleton_.comparison_ = comp;
    new(&singleton_.value_) binding(std::move(value));
    singleton_.parentObject_ = parentObject;
  }

  std::string statement::condition::toSql(bool toplevel, bool debug) const
  {
    switch (type_)
    {
      case type::empty:
      {
        return "";
      }

      case type::singleton:
      {
        switch (singleton_.comparison_)
        {
          case comparison::equals:
          {
            if (debug)
            {
              switch (singleton_.value_.getType())
              {
                case binding::type::string:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " = \"" + singleton_.value_.getString() + "\"";
                }

                case binding::type::integer:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " = " + std::to_string(singleton_.value_.getInteger());
                }

                case binding::type::field:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " = " + singleton_.value_.getTable() + "." + singleton_.value_.getColumn();
                }

                case binding::type::invalid:
                {
                  throw std::logic_error("Invalid binding in statement generation");
                }
              }
            } else {
              if (singleton_.value_.getType() == binding::type::field)
              {
                return singleton_.table_ + "." + singleton_.column_ + " = " + singleton_.value_.getTable() + "." + singleton_.value_.getColumn();
              } else {
                return singleton_.table_ + "." + singleton_.column_ + " = ?";
              }
            }
          }

          case comparison::does_not_equal:
          {
            if (debug)
            {
              switch (singleton_.value_.getType())
              {
                case binding::type::string:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " != \"" + singleton_.value_.getString() + "\"";
                }

                case binding::type::integer:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " != " + std::to_string(singleton_.value_.getInteger());
                }

                case binding::type::field:
                {
                  return singleton_.table_ + "." + singleton_.column_ + " != " + singleton_.value_.getTable() + "." + singleton_.value_.getColumn();
                }

                case binding::type::invalid:
                {
                  throw std::logic_error("Invalid binding in statement generation");
                }
              }
            } else {
              if (singleton_.value_.getType() == binding::type::field)
              {
                return singleton_.table_ + "." + singleton_.column_ + " != " + singleton_.value_.getTable() + "." + singleton_.value_.getColumn();
              } else {
                return singleton_.table_ + "." + singleton_.column_ + " != ?";
              }
            }
          }

          case comparison::is_greater_than:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " > " + std::to_string(singleton_.value_.getInteger());
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " > ?";
            }
          }

          case comparison::is_at_most:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " <= " + std::to_string(singleton_.value_.getInteger());
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " <= ?";
            }
          }

          case comparison::is_less_than:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " < " + std::to_string(singleton_.value_.getInteger());
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " < ?";
            }
          }

          case comparison::is_at_least:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " >= " + std::to_string(singleton_.value_.getInteger());
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " >= ?";
            }
          }

          case comparison::is_like:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " LIKE \"" + singleton_.value_.getString() + "\"";
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " LIKE ?";
            }
          }

          case comparison::is_not_like:
          {
            if (debug)
            {
              return singleton_.table_ + "." + singleton_.column_ + " NOT LIKE \"" + singleton_.value_.getString() + "\"";
            } else {
              return singleton_.table_ + "." + singleton_.column_ + " NOT LIKE ?";
            }
          }

          case comparison::is_not_null:
          {
            return singleton_.table_ + "." + singleton_.column_ + " IS NOT NULL";
          }

          case comparison::is_null:
          {
            return singleton_.table_ + "." + singleton_.column_ + " IS NULL";
          }
        }
      }

      case type::group:
      {
        std::list<std::string> clauses;
        for (const condition& cond : group_.children_)
        {
          clauses.push_back(cond.toSql(false, debug));
        }

        if (clauses.empty())
        {
          return "";
        } else if (clauses.size() == 1)
        {
          return clauses.front();
        } else {
          std::string result = implode(std::begin(clauses), std::end(clauses), group_.orlogic_ ? " OR " : " AND ");

          if (toplevel)
          {
            return result;
          } else {
            return "(" + result + ")";
          }
        }
      }
    }
  }

  std::list<binding> statement::condition::flattenBindings() const
  {
    switch (type_)
    {
      case type::empty:
      {
        return {};
      }

      case type::singleton:
      {
        if (singleton_.value_.getType() == binding::type::field)
        {
          return {};
        } else {
          switch (singleton_.comparison_)
          {
            case comparison::equals:
            case comparison::does_not_equal:
            case comparison::is_greater_than:
            case comparison::is_at_most:
            case comparison::is_less_than:
            case comparison::is_at_least:
            case comparison::is_like:
            case comparison::is_not_like:
            {
              return {singleton_.value_};
            }

            case comparison::is_not_null:
            case comparison::is_null:
            {
              return {};
            }
          }
        }
      }

      case type::group:
      {
        std::list<binding> bindings;
        for (const condition& cond : group_.children_)
        {
          for (binding value : cond.flattenBindings())
          {
            bindings.push_back(std::move(value));
          }
        }

        return bindings;
      }
    }
  }

  statement::condition::condition(bool orlogic) : type_(type::group)
  {
    new(&group_.children_) std::list<condition>();
    group_.orlogic_ = orlogic;
  }

  statement::condition& statement::condition::operator+=(condition n)
  {
    if (type_ == type::group)
    {
      group_.children_.push_back(std::move(n));

      return *this;
    } else {
      throw std::domain_error("Cannot add condition to non-group condition");
    }
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

  const std::list<statement::condition>& statement::condition::getChildren() const
  {
    if (type_ == type::group)
    {
      return group_.children_;
    } else {
      throw std::domain_error("Cannot get children of non-group condition");
    }
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
        condition result(group_.orlogic_);

        for (const condition& child : group_.children_)
        {
          condition newChild = child.flatten();

          if ((newChild.type_ == type::group) && (newChild.group_.orlogic_ == group_.orlogic_))
          {
            for (condition subChild : std::move(newChild.group_.children_))
            {
              result += std::move(subChild);
            }
          } else {
            result += std::move(newChild);
          }
        }

        return result;
      }
    }
  }

  statement::condition statement::condition::resolveCompareFields(object context, std::string tableName) const
  {
    switch (type_)
    {
      case type::empty:
      {
        return *this;
      }

      case type::singleton:
      {
        if ((singleton_.parentObject_ != object::undefined) && (singleton_.parentObject_ == context))
        {
          return condition(singleton_.table_, singleton_.column_, singleton_.comparison_, {tableName, singleton_.value_.getColumn()});
        } else {
          return *this;
        }
      }

      case type::group:
      {
        condition result(group_.orlogic_);
        for (const condition& cond : group_.children_)
        {
          result += cond.resolveCompareFields(context, tableName);
        }

        return result;
      }
    }
  }

};
