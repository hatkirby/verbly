#ifndef FIELD_H_43258321
#define FIELD_H_43258321

#include "enums.h"
#include <stdexcept>
#include <tuple>

namespace verbly {

  class filter;

  class field {
  public:
    enum class type {
      undefined,
      string,
      integer,
      boolean,
      join,
      join_where,
      join_through,
      join_through_where,
      hierarchal_join
    };

    // Default constructor

    field()
    {
    }

    // Static factories

    static field stringField(
      object obj,
      const char* name,
      bool nullable = false)
    {
      return field(obj, type::string, name, nullable);
    }

    static field stringField(
      const char* table,
      const char* name,
      bool nullable = false)
    {
      return field(object::undefined, type::string, name, nullable, table);
    }

    static field integerField(
      object obj,
      const char* name,
      bool nullable = false)
    {
      return field(obj, type::integer, name, nullable);
    }

    static field integerField(
      const char* table,
      const char* name,
      bool nullable = false)
    {
      return field(object::undefined, type::integer, name, nullable, table);
    }

    static field booleanField(
      object obj,
      const char* name,
      bool nullable = false)
    {
      return field(obj, type::boolean, name, nullable);
    }

    static field booleanField(
      const char* table,
      const char* name,
      bool nullable = false)
    {
      return field(object::undefined, type::boolean, name, nullable, table);
    }

    static field joinField(
      object obj,
      const char* name,
      object joinWith,
      bool nullable = false)
    {
      return field(obj, type::join, name, nullable, 0, joinWith);
    }

    static field joinField(
      object obj,
      const char* name,
      const char* table,
      bool nullable = false)
    {
      return field(obj, type::join, name, nullable, table);
    }

    static field joinWhere(
      object obj,
      const char* name,
      object joinWith,
      const char* conditionColumn,
      int conditionValue,
      bool nullable = false)
    {
      return field(obj, type::join_where, name, nullable, 0, joinWith, 0, 0, 0, conditionColumn, conditionValue);
    }

    static field joinThrough(
      object obj,
      const char* name,
      object joinWith,
      const char* joinTable,
      const char* foreignColumn)
    {
      return field(obj, type::join_through, name, true, joinTable, joinWith, foreignColumn, name, foreignColumn);
    }

    static field joinThrough(
      object obj,
      const char* name,
      object joinWith,
      const char* joinTable,
      const char* foreignColumn,
      const char* joinColumn,
      const char* foreignJoinColumn)
    {
      return field(obj, type::join_through, name, true, joinTable, joinWith, foreignColumn, joinColumn, foreignJoinColumn);
    }

    static field joinThroughWhere(
      object obj,
      const char* name,
      object joinWith,
      const char* joinTable,
      const char* foreignColumn,
      const char* conditionColumn,
      int conditionValue,
      bool nullable = false)
    {
      return field(obj, type::join_through_where, name, nullable, joinTable, joinWith, foreignColumn, name, foreignColumn, conditionColumn, conditionValue);
    }

    static field selfJoin(
      object obj,
      const char* name,
      const char* joinTable,
      const char* joinColumn,
      const char* foreignJoinColumn)
    {
      return field(obj, type::join_through, name, true, joinTable, obj, name, joinColumn, foreignJoinColumn);
    }

    static field hierarchalSelfJoin(
      object obj,
      const char* name,
      const char* joinTable,
      const char* joinColumn,
      const char* foreignJoinColumn)
    {
      return field(obj, type::hierarchal_join, name, true, joinTable, obj, name, joinColumn, foreignJoinColumn);
    }

    // Accessors

    object getObject() const
    {
      return object_;
    }

    type getType() const
    {
      return type_;
    }

    bool isJoin() const
    {
      return ((type_ == type::join)
        || (type_ == type::join_where)
        || (type_ == type::join_through)
        || (type_ == type::join_through_where)
        || (type_ == type::hierarchal_join));
    }

    const char* getColumn() const
    {
      return column_;
    }

    bool isNullable() const
    {
      return nullable_;
    }

    bool hasTable() const
    {
      return (table_ != 0);
    }

    const char* getTable() const
    {
      return table_;
    }

    // Joins

    object getJoinObject() const
    {
      return (type_ == type::hierarchal_join)
        ? object_
        : ((type_ == type::join) || (type_ == type::join_where) || (type_ == type::join_through) || (type_ == type::join_through_where))
        ? joinObject_
        : throw std::domain_error("Non-join fields don't have join objects");
    }

    // Many-to-many joins

    const char* getForeignColumn() const
    {
      // We ignore hierarchal joins because they are always self joins.
      return ((type_ == type::join_through) || (type_ == type::join_through_where))
        ? foreignColumn_
        : throw std::domain_error("Only many-to-many join fields have a foreign column");
    }

    const char* getJoinColumn() const
    {
      return ((type_ == type::join_through) || (type_ == type::join_through_where) || (type_ == type::hierarchal_join))
        ? joinColumn_
        : throw std::domain_error("Only many-to-many join fields have a join column");
    }

    const char* getForeignJoinColumn() const
    {
      return ((type_ == type::join_through) || (type_ == type::join_through_where) || (type_ == type::hierarchal_join))
        ? foreignJoinColumn_
        : throw std::domain_error("Only many-to-many join fields have a foreign join column");
    }

    // Condition joins

    const char* getConditionColumn() const
    {
      if ((type_ == type::join_where) || (type_ == type::join_through_where))
      {
        return conditionColumn_;
      } else {
        throw std::domain_error("Only condition join fields have a condition column");
      }
    }

    int getConditionValue() const
    {
      return ((type_ == type::join_where) || (type_ == type::join_through_where))
        ? conditionValue_
        : throw std::domain_error("Only condition join fields have a condition value");
    }

    // Ordering

    bool operator<(const field& other) const
    {
      // For the most part, (object, column) uniquely identifies fields.
      // However, there do exist a number of relationships from an object to
      // itself, such as notion hypernymy/hyponymy. Hypernymy and hyponymy have
      // the same object (notion), the same column (notion_id), and the same
      // table (hypernymy); however, they have different join columns. For
      // condition joins, the condition field and condition value are also
      // significant.
      return std::tie(object_, column_, table_, joinColumn_, conditionColumn_, conditionValue_)
        < std::tie(other.object_, other.column_, other.table_, other.joinColumn_, other.conditionColumn_, other.conditionValue_);
    }

    // Equality

    bool operator==(const field& other) const
    {
      // See operator<() for documentation.
      return std::tie(object_, column_, table_, joinColumn_, conditionColumn_, conditionValue_)
        == std::tie(other.object_, other.column_, other.table_, other.joinColumn_, other.conditionColumn_, other.conditionValue_);
    }

    // Filter construction

    filter operator==(int value) const; // Integer equality
    filter operator!=(int value) const; // Integer inequality
    filter operator<(int value) const; // Integer is less than
    filter operator<=(int value) const; // Integer is at most
    filter operator>(int value) const; // Integer is greater than
    filter operator>=(int value) const; // Integer is at least

    filter operator==(part_of_speech value) const; // Part of speech equality
    filter operator==(positioning value) const; // Adjective positioning equality
    filter operator==(inflection value) const; // Inflection category equality
    filter operator==(part_type value) const; // Verb frame part type equality

    filter operator!=(part_of_speech value) const; // Part of speech inequality
    filter operator!=(positioning value) const; // Adjective positioning inequality
    filter operator!=(inflection value) const; // Inflection category inequality
    filter operator!=(part_type value) const; // Verb frame part type inequality

    filter operator==(bool value) const; // Boolean equality

    filter operator==(std::string value) const; // String equality
    filter operator!=(std::string value) const; // String inequality
    filter operator%=(std::string value) const; // String matching

    filter operator==(const char* value) const; // String equality
    filter operator!=(const char* value) const; // String inequality
    filter operator%=(const char* value) const; // String matching

    operator filter() const; // Non-nullity
    filter operator!() const; // Nullity

    filter operator%=(filter joinCondition) const; // Join

  private:

    // Constructor

    field(
      object obj,
      type datatype,
      const char* column,
      bool nullable = false,
      const char* table = 0,
      object joinObject = object::undefined,
      const char* foreignColumn = 0,
      const char* joinColumn = 0,
      const char* foreignJoinColumn = 0,
      const char* conditionColumn = 0,
      int conditionValue = 0) :
        object_(obj),
        type_(datatype),
        column_(column),
        nullable_(nullable),
        table_(table),
        joinObject_(joinObject),
        foreignColumn_(foreignColumn),
        joinColumn_(joinColumn),
        foreignJoinColumn_(foreignJoinColumn),
        conditionColumn_(conditionColumn),
        conditionValue_(conditionValue)
    {
    }

    // General
    object object_ = object::undefined;
    type type_ = type::undefined;
    const char* column_ = 0;
    const char* table_ = 0;

    // Non-joins and belongs-to joins
    bool nullable_ = false;

    // Joins
    object joinObject_ = object::undefined;

    // Many-to-many joins
    const char* foreignColumn_ = 0;
    const char* joinColumn_ = 0;
    const char* foreignJoinColumn_ = 0;

    // Condition joins
    const char* conditionColumn_ = 0;
    int conditionValue_ = 0;

  };

};

#endif /* end of include guard: FIELD_H_43258321 */
