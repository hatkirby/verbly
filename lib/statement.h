#ifndef STATEMENT_H_29F51659
#define STATEMENT_H_29F51659

#include <string>
#include <list>
#include <map>
#include <set>
#include "binding.h"
#include "enums.h"
#include "field.h"
#include "filter.h"

namespace verbly {

  class filter;

  class statement {
  public:

    statement(object context, filter queryFilter);

    std::string getQueryString(std::list<std::string> select, bool random, int limit) const;

    std::list<binding> getBindings() const;

  private:

    class join {
    public:

      join(
        bool outer,
        std::string foreignTableName,
        std::string joinTable,
        std::string joinColumn,
        std::string foreignTable,
        std::string foreignColumn) :
          outer_(outer),
          foreignTableName_(std::move(foreignTableName)),
          joinTable_(std::move(joinTable)),
          joinColumn_(std::move(joinColumn)),
          foreignTable_(std::move(foreignTable)),
          foreignColumn_(std::move(foreignColumn))
      {
      }

      bool isOuterJoin() const
      {
        return outer_;
      }

      const std::string& getForeignTableName() const
      {
        return foreignTableName_;
      }

      const std::string& getJoinTable() const
      {
        return joinTable_;
      }

      const std::string& getJoinColumn() const
      {
        return joinColumn_;
      }

      const std::string& getForeignTable() const
      {
        return foreignTable_;
      }

      const std::string& getForeignColumn() const
      {
        return foreignColumn_;
      }

    private:
      bool outer_ = false;
      std::string foreignTableName_;
      std::string joinTable_;
      std::string joinColumn_;
      std::string foreignTable_;
      std::string foreignColumn_;

    };

    friend std::ostream& operator<<(std::ostream& oss, const join& j);

    class condition {
    public:
      enum class type {
        empty,
        singleton,
        group
      };

      enum class comparison {
        equals,
        does_not_equal,
        is_greater_than,
        is_at_most,
        is_less_than,
        is_at_least,
        is_like,
        is_not_like,
        is_not_null,
        is_null
      };

      // Copy and move constructors

      condition(const condition& other);
      condition(condition&& other);

      // Assignment

      condition& operator=(condition other);

      // Swap

      friend void swap(condition& first, condition& second);

      // Destructor

      ~condition();

      // Accessors

      type getType() const
      {
        return type_;
      }

      // Empty

      condition();

      // Singleton

      condition(std::string table, std::string column, bool isNull);

      condition(std::string table, std::string column, comparison comp, binding value);

      // Group

      explicit condition(bool orlogic);

      condition& operator+=(condition n);

      condition& operator&=(condition n);

      const std::list<condition>& getChildren() const;

      // Utility

      std::string toSql() const;

      std::list<binding> flattenBindings() const;

    private:
      union {
        struct {
          std::string table_;
          std::string column_;
          comparison comparison_;
          binding value_;
        } singleton_;
        struct {
          std::list<condition> children_;
          bool orlogic_;
        } group_;
      };
      type type_;
    };

    friend void swap(condition& first, condition& second);

    class with {
    public:

      with(
        std::string identifier,
        field f,
        std::map<std::string, std::string> tables,
        std::string topTable,
        condition where,
        std::list<join> joins,
        bool recursive) :
          identifier_(std::move(identifier)),
          field_(f),
          tables_(std::move(tables)),
          topTable_(std::move(topTable)),
          topCondition_(std::move(where)),
          joins_(std::move(joins)),
          recursive_(recursive)
      {
      }

      const std::string& getIdentifier() const
      {
        return identifier_;
      }

      field getField() const
      {
        return field_;
      }

      std::string getTableForId(std::string identifier) const
      {
        return tables_.at(identifier);
      }

      const std::string& getTopTable() const
      {
        return topTable_;
      }

      const condition& getCondition() const
      {
        return topCondition_;
      }

      const std::list<join>& getJoins() const
      {
        return joins_;
      }

      bool isRecursive() const
      {
        return recursive_;
      }

    private:
      std::string identifier_;
      field field_;
      std::map<std::string, std::string> tables_;
      std::string topTable_;
      condition topCondition_;
      std::list<join> joins_;
      bool recursive_;

    };

    static constexpr const char* getTableForContext(object context)
    {
      return (context == object::notion) ? "notions"
        : (context == object::word) ? "words"
        : (context == object::group) ? "groups"
        : (context == object::frame) ? "frames"
        : (context == object::lemma) ? "lemmas_forms"
        : (context == object::form) ? "forms"
        : (context == object::pronunciation) ? "pronunciations"
        : throw std::domain_error("Provided context has no associated table");
    }

    static const std::list<field> getSelectForContext(object context);

    statement(std::string tableName, filter clause, int nextTableId = 0, int nextWithId = 0);

    condition parseFilter(filter queryFilter);

    std::string instantiateTable(std::string name);

    std::string instantiateWith(std::string name);

    condition integrate(statement subStmt, bool cte = false);

    int nextTableId_;
    int nextWithId_;

    std::map<std::string, std::string> tables_;
    std::string topTable_;
    std::list<join> joins_;
    std::list<with> withs_;
    condition topCondition_;

  };

};

#endif /* end of include guard: STATEMENT_H_29F51659 */
