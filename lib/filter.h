#ifndef FILTER_H_932BA9C6
#define FILTER_H_932BA9C6

#include <list>
#include <string>
#include <memory>
#include "field.h"
#include "enums.h"

namespace verbly {

  class filter {
  public:
    enum class type {
      empty,
      singleton,
      group,
      mask
    };

    enum class comparison {
      int_equals,
      int_does_not_equal,
      int_is_at_least,
      int_is_greater_than,
      int_is_at_most,
      int_is_less_than,
      boolean_equals,
      string_equals,
      string_does_not_equal,
      string_is_like,
      string_is_not_like,
      is_null,
      is_not_null,
      matches,
      does_not_match,
      hierarchally_matches,
      does_not_hierarchally_match,
      field_equals,
      field_does_not_equal
    };

    // Copy and move constructors

    filter(const filter& other);
    filter(filter&& other);

    // Assignment

    filter& operator=(filter other);

    // Swap

    friend void swap(filter& first, filter& second);

    // Destructor

    ~filter();

    // Accessors

    type getType() const
    {
      return type_;
    }

    // Empty

    filter();

    // Singleton

    filter(field filterField, comparison filterType, int filterValue);
    filter(field filterField, comparison filterType, std::string filterValue);
    filter(field filterField, comparison filterType, bool filterValue);
    filter(field filterField, comparison filterType);
    filter(field joinOn, comparison filterType, filter joinCondition);
    filter(field filterField, comparison filterType, field compareField);

    field getField() const;

    comparison getComparison() const;

    filter getJoinCondition() const;

    std::string getStringArgument() const;

    int getIntegerArgument() const;

    bool getBooleanArgument() const;

    field getCompareField() const;

    // Group

    explicit filter(bool orlogic);

    bool getOrlogic() const;

    filter operator+(filter condition) const;

    filter& operator+=(filter condition);

    using const_iterator = std::list<filter>::const_iterator;

    const_iterator begin() const;

    const_iterator end() const;

    // Mask

    filter(std::string name, bool internal, filter subfilter);

    const std::string& getMaskName() const;

    bool isMaskInternal() const;

    const filter& getMaskFilter() const;

    // Negation

    filter operator!() const;

    // Groupifying

    filter operator&&(filter condition) const;
    filter operator||(filter condition) const;

    filter& operator&=(filter condition);
    filter& operator|=(filter condition);

    // Maskifying

    static filter mask(std::string name, filter subfilter);

    // Utility

    filter normalize(object context) const;

    filter compact() const;

  private:
    union {
      struct {
        field filterField;
        comparison filterType;
        union {
          std::unique_ptr<filter> join;
          std::string stringValue;
          int intValue;
          bool boolValue;
          field compareField;
        };
      } singleton_;
      struct {
        std::list<filter> children;
        bool orlogic;
      } group_;
      struct {
        std::string name;
        bool internal;
        std::unique_ptr<filter> subfilter;
      } mask_;
    };
    type type_ = type::empty;

  };

};

#endif /* end of include guard: FILTER_H_932BA9C6 */
