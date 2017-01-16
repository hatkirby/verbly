#include "statement.h"
#include <sstream>
#include <utility>
#include "filter.h"
#include "util.h"
#include "notion.h"
#include "word.h"
#include "group.h"
#include "frame.h"
#include "lemma.h"
#include "form.h"
#include "pronunciation.h"

namespace verbly {
  
  statement::statement(
    object context,
    filter queryFilter) :
      statement(getTableForContext(context), queryFilter.normalize(context))
  {
  }
  
  std::string statement::getQueryString(std::list<std::string> select, bool random, int limit) const
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
          cteStream << cte.getCondition().toSql();
        }
        
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
      queryStream << topCondition_.toSql();
    }
    
    if (random)
    {
      queryStream << " ORDER BY RANDOM()";
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
    std::string tableName,
    filter clause,
    int nextTableId,
    int nextWithId) :
      nextTableId_(nextTableId),
      nextWithId_(nextWithId),
      topTable_(instantiateTable(std::move(tableName))),
      topCondition_(parseFilter(std::move(clause)))
  {
  }
  
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
          {
            std::string joinTableName;
            if (clause.getField().hasTable())
            {
              joinTableName = clause.getField().getTable();
            } else {
              joinTableName = getTableForContext(clause.getField().getJoinObject());
            }

            statement joinStmt(
              joinTableName,
              clause.getJoinCondition().normalize(clause.getField().getJoinObject()),
              nextTableId_,
              nextWithId_);
            
            std::string joinTable = joinStmt.topTable_;
            condition curCond = integrate(std::move(joinStmt));
            
            bool outer = false;
            if (clause.getComparison() == filter::comparison::does_not_match)
            {
              outer = true;
              
              curCond &= condition(joinTable, clause.getField().getColumn(), true);
            }
            
            joins_.emplace_back(outer, joinTableName, topTable_, clause.getField().getColumn(), joinTable, clause.getField().getColumn());
            
            return curCond;
          }
        
          case field::type::join_through:
          {
            statement joinStmt(
              getTableForContext(clause.getField().getJoinObject()),
              clause.getJoinCondition().normalize(clause.getField().getJoinObject()),
              nextTableId_,
              nextWithId_);
              
            std::string joinTable = joinStmt.topTable_;
            std::string throughTable = instantiateTable(clause.getField().getTable());
            condition curCond = integrate(std::move(joinStmt));
            
            bool outer = false;
            if (clause.getComparison() == filter::comparison::does_not_match)
            {
              outer = true;
              
              curCond &= condition(throughTable, clause.getField().getJoinColumn(), true);
            }
            
            joins_.emplace_back(outer, clause.getField().getTable(), topTable_, clause.getField().getColumn(), throughTable, clause.getField().getJoinColumn());
            joins_.emplace_back(false, getTableForContext(clause.getField().getJoinObject()), throughTable, clause.getField().getForeignJoinColumn(), joinTable, clause.getField().getForeignColumn());
            
            return curCond;
          }
        
          case field::type::hierarchal_join:
          {
            std::string withName = std::string(clause.getField().getTable()) + "_tree_" + std::to_string(nextWithId_++);
            std::string withInstName = instantiateTable(withName);
            
            bool outer = false;
            if (clause.getComparison() == filter::comparison::does_not_hierarchally_match)
            {
              outer = true;
            }
            
            joins_.emplace_back(outer, withName, topTable_, clause.getField().getColumn(), withInstName, clause.getField().getColumn());
            
            statement withStmt(
              getTableForContext(clause.getField().getObject()),
              clause.getJoinCondition().normalize(clause.getField().getObject()),
              nextTableId_,
              nextWithId_);
              
            for (auto& w : withStmt.withs_)
            {
              withs_.push_back(std::move(w));
            }
              
            nextTableId_ = withStmt.nextTableId_;
            nextWithId_ = withStmt.nextWithId_;
            
            withs_.emplace_back(
              withName,
              clause.getField(),
              std::move(withStmt.tables_),
              std::move(withStmt.topTable_),
              std::move(withStmt.topCondition_),
              std::move(withStmt.joins_));

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
  
  statement::condition statement::integrate(statement subStmt)
  {
    for (auto& mapping : subStmt.tables_)
    {
      tables_[mapping.first] = mapping.second;
    }
        
    for (auto& j : subStmt.joins_)
    {
      joins_.push_back(j);
    }
    
    for (auto& w : subStmt.withs_)
    {
      withs_.push_back(w);
    }

    nextTableId_ = subStmt.nextTableId_;
    nextWithId_ = subStmt.nextWithId_;
    
    return subStmt.topCondition_;
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
  }
  
  statement::condition::condition(
    std::string table,
    std::string column,
    comparison comp,
    binding value) :
      type_(type::singleton)
  {
    new(&singleton_.table_) std::string(std::move(table));
    new(&singleton_.column_) std::string(std::move(column));
    singleton_.comparison_ = comp;
    new(&singleton_.value_) binding(std::move(value));
  }
  
  std::string statement::condition::toSql() const
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
            return singleton_.table_ + "." + singleton_.column_ + " = ?";
          }
          
          case comparison::does_not_equal:
          {
            return singleton_.table_ + "." + singleton_.column_ + " != ?";
          }
          
          case comparison::is_greater_than:
          {
            return singleton_.table_ + "." + singleton_.column_ + " > ?";
          }
          
          case comparison::is_at_most:
          {
            return singleton_.table_ + "." + singleton_.column_ + " <= ?";
          }
          
          case comparison::is_less_than:
          {
            return singleton_.table_ + "." + singleton_.column_ + " < ?";
          }
          
          case comparison::is_at_least:
          {
            return singleton_.table_ + "." + singleton_.column_ + " >= ?";
          }
          
          case comparison::is_like:
          {
            return singleton_.table_ + "." + singleton_.column_ + " LIKE ?";
          }
          
          case comparison::is_not_like:
          {
            return singleton_.table_ + "." + singleton_.column_ + " NOT LIKE ?";
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
          clauses.push_back(cond.toSql());
        }
        
        return implode(std::begin(clauses), std::end(clauses), group_.orlogic_ ? " OR " : " AND ");
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
        return {singleton_.value_};
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
  
};
