#ifndef FILTER_H_932BA9C6
#define FILTER_H_932BA9C6

#include <list>
#include <string>
#include <memory>
#include "../vendor/hkutil/vendor/variant.hpp"
#include "../vendor/hkutil/hkutil/recptr.h"
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

    // Accessors

    type getType() const
    {
      return type_;
    }

    // Empty

    filter() = default;

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

    using rec_filter = hatkirby::recptr<filter>;

    struct singleton_type {
      field filterField;
      comparison filterType;

      mpark::variant<
        mpark::monostate,
        rec_filter,
        std::string,
        int,
        bool,
        field> data;
    };

    struct group_type {
      std::list<filter> children;
      bool orlogic;
    };

    struct mask_type {
      std::string name;
      bool internal;
      rec_filter subfilter;
    };

    using variant_type =
      mpark::variant<
        mpark::monostate,
        singleton_type,
        group_type,
        mask_type>;

    type type_ = type::empty;
    variant_type variant_;

  };

};

#endif /* end of include guard: FILTER_H_932BA9C6 */
