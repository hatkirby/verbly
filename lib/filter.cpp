#include "filter.h"
#include <stdexcept>
#include <map>
#include "notion.h"
#include "word.h"
#include "frame.h"
#include "part.h"
#include "lemma.h"
#include "form.h"
#include "pronunciation.h"

namespace verbly {

  filter::filter(const filter& other)
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
        new(&singleton_.filterField) field(other.singleton_.filterField);
        singleton_.filterType = other.singleton_.filterType;

        switch (singleton_.filterType)
        {
          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          {
            singleton_.intValue = other.singleton_.intValue;

            break;
          }

          case comparison::boolean_equals:
          {
            singleton_.boolValue = other.singleton_.boolValue;

            break;
          }

          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          {
            new(&singleton_.stringValue) std::string(other.singleton_.stringValue);

            break;
          }

          case comparison::is_null:
          case comparison::is_not_null:
          {
            break;
          }

          case comparison::matches:
          case comparison::does_not_match:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            new(&singleton_.join) std::unique_ptr<filter>(new filter(*other.singleton_.join));

            break;
          }
        }

        break;
      }

      case type::group:
      {
        new(&group_.children) std::list<filter>(other.group_.children);
        group_.orlogic = other.group_.orlogic;

        break;
      }
    }
  }

  filter::filter(filter&& other) : filter()
  {
    swap(*this, other);
  }

  filter& filter::operator=(filter other)
  {
    swap(*this, other);

    return *this;
  }

  void swap(filter& first, filter& second)
  {
    using type = filter::type;
    using comparison = filter::comparison;

    type tempType = first.type_;
    field tempField;
    comparison tempComparison;
    std::unique_ptr<filter> tempJoin;
    std::string tempStringValue;
    int tempIntValue;
    bool tempBoolValue;
    std::list<filter> tempChildren;
    bool tempOrlogic;

    switch (tempType)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        tempField = std::move(first.singleton_.filterField);
        tempComparison = first.singleton_.filterType;

        switch (tempComparison)
        {
          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          {
            tempIntValue = first.singleton_.intValue;

            break;
          }

          case comparison::boolean_equals:
          {
            tempBoolValue = first.singleton_.boolValue;

            break;
          }

          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          {
            tempStringValue = std::move(first.singleton_.stringValue);

            break;
          }

          case comparison::is_null:
          case comparison::is_not_null:
          {
            break;
          }

          case comparison::matches:
          case comparison::does_not_match:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            tempJoin = std::move(first.singleton_.join);

            break;
          }
        }

        break;
      }

      case type::group:
      {
        tempChildren = std::move(first.group_.children);
        tempOrlogic = first.group_.orlogic;

        break;
      }
    }

    first.~filter();

    first.type_ = second.type_;

    switch (first.type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        new(&first.singleton_.filterField) field(std::move(second.singleton_.filterField));
        first.singleton_.filterType = second.singleton_.filterType;

        switch (first.singleton_.filterType)
        {
          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          {
            first.singleton_.intValue = second.singleton_.intValue;

            break;
          }

          case comparison::boolean_equals:
          {
            first.singleton_.boolValue = second.singleton_.boolValue;

            break;
          }

          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          {
            new(&first.singleton_.stringValue) std::string(std::move(second.singleton_.stringValue));

            break;
          }

          case comparison::is_null:
          case comparison::is_not_null:
          {
            break;
          }

          case comparison::matches:
          case comparison::does_not_match:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            new(&first.singleton_.join) std::unique_ptr<filter>(std::move(second.singleton_.join));

            break;
          }
        }

        break;
      }

      case type::group:
      {
        new(&first.group_.children) std::list<filter>(std::move(second.group_.children));
        first.group_.orlogic = second.group_.orlogic;

        break;
      }
    }

    second.~filter();

    second.type_ = tempType;

    switch (second.type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        new(&second.singleton_.filterField) field(std::move(tempField));
        second.singleton_.filterType = tempComparison;

        switch (second.singleton_.filterType)
        {
          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          {
            second.singleton_.intValue = tempIntValue;

            break;
          }

          case comparison::boolean_equals:
          {
            second.singleton_.boolValue = tempBoolValue;

            break;
          }

          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          {
            new(&second.singleton_.stringValue) std::string(std::move(tempStringValue));

            break;
          }

          case comparison::is_null:
          case comparison::is_not_null:
          {
            break;
          }

          case comparison::matches:
          case comparison::does_not_match:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            new(&second.singleton_.join) std::unique_ptr<filter>(std::move(tempJoin));

            break;
          }
        }

        break;
      }

      case type::group:
      {
        new(&second.group_.children) std::list<filter>(std::move(tempChildren));
        second.group_.orlogic = tempOrlogic;

        break;
      }
    }
  }

  filter::~filter()
  {
    switch (type_)
    {
      case type::empty:
      {
        break;
      }

      case type::singleton:
      {
        singleton_.filterField.~field();

        switch (singleton_.filterType)
        {
          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          case comparison::boolean_equals:
          case comparison::is_null:
          case comparison::is_not_null:
          {
            break;
          }

          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          {
            using string_type = std::string;

            singleton_.stringValue.~string_type();

            break;
          }

          case comparison::matches:
          case comparison::does_not_match:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            using ptr_type = std::unique_ptr<filter>;

            singleton_.join.~ptr_type();

            break;
          }
        }

        break;
      }

      case type::group:
      {
        using list_type = std::list<filter>;

        group_.children.~list_type();

        break;
      }
    }
  }

  filter::filter()
  {
  }

  filter::filter(
    field filterField,
    comparison filterType,
    int filterValue) :
      type_(type::singleton)
  {
    if (filterField.getType() == field::type::integer)
    {
      switch (filterType)
      {
        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        {
          new(&singleton_.filterField) field(std::move(filterField));
          singleton_.filterType = filterType;
          singleton_.intValue = filterValue;

          break;
        }

        case comparison::boolean_equals:
        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        case comparison::is_null:
        case comparison::is_not_null:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::invalid_argument("Invalid comparison for integer field");
        }
      }
    } else {
      throw std::domain_error("Cannot match a non-integer field against an integer value");
    }
  }

  filter::filter(
    field filterField,
    comparison filterType,
    std::string filterValue) :
      type_(type::singleton)
  {
    if (filterField.getType() == field::type::string)
    {
      switch (filterType)
      {
        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        {
          new(&singleton_.filterField) field(std::move(filterField));
          singleton_.filterType = filterType;
          new(&singleton_.stringValue) std::string(std::move(filterValue));

          break;
        }

        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        case comparison::boolean_equals:
        case comparison::is_null:
        case comparison::is_not_null:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::invalid_argument("Invalid comparison for string field");
        }
      }
    } else {
      throw std::domain_error("Cannot match a non-string field against an string value");
    }
  }

  filter::filter(
    field filterField,
    comparison filterType,
    bool filterValue) :
      type_(type::singleton)
  {
    if (filterField.getType() == field::type::boolean)
    {
      switch (filterType)
      {
        case comparison::boolean_equals:
        {
          new(&singleton_.filterField) field(std::move(filterField));
          singleton_.filterType = filterType;
          singleton_.boolValue = filterValue;

          break;
        }

        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        case comparison::is_null:
        case comparison::is_not_null:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::invalid_argument("Invalid comparison for boolean field");
        }
      }
    } else {
      throw std::domain_error("Cannot match a non-boolean field against a boolean value");
    }
  }

  filter::filter(
    field filterField,
    comparison filterType) :
      type_(type::singleton)
  {
    if (filterField.isNullable())
    {
      switch (filterType)
      {
        case comparison::is_null:
        case comparison::is_not_null:
        {
          new(&singleton_.filterField) field(std::move(filterField));
          singleton_.filterType = filterType;

          break;
        }

        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        case comparison::boolean_equals:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::invalid_argument("Incorrect constructor for given comparison");
        }
      }
    } else {
      throw std::domain_error("Cannot check nullity/non-nullity of non-nullable field");
    }
  }

  filter::filter(
    field joinOn,
    comparison filterType,
    filter joinCondition) :
      type_(type::singleton)
  {
    switch (joinOn.getType())
    {
      case field::type::join:
      case field::type::join_where:
      case field::type::join_through:
      {
        switch (filterType)
        {
          case comparison::matches:
          case comparison::does_not_match:
          {
            new(&singleton_.filterField) field(std::move(joinOn));
            singleton_.filterType = filterType;
            new(&singleton_.join) std::unique_ptr<filter>(new filter(joinCondition.normalize(singleton_.filterField.getJoinObject())));

            break;
          }

          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          case comparison::boolean_equals:
          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          case comparison::is_null:
          case comparison::is_not_null:
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            throw std::invalid_argument("Incorrect constructor for given comparison");
          }
        }

        break;
      }

      case field::type::hierarchal_join:
      {
        switch (filterType)
        {
          case comparison::hierarchally_matches:
          case comparison::does_not_hierarchally_match:
          {
            new(&singleton_.filterField) field(std::move(joinOn));
            singleton_.filterType = filterType;
            new(&singleton_.join) std::unique_ptr<filter>(new filter(joinCondition.normalize(singleton_.filterField.getObject())));

            break;
          }

          case comparison::int_equals:
          case comparison::int_does_not_equal:
          case comparison::int_is_at_least:
          case comparison::int_is_greater_than:
          case comparison::int_is_at_most:
          case comparison::int_is_less_than:
          case comparison::boolean_equals:
          case comparison::string_equals:
          case comparison::string_does_not_equal:
          case comparison::string_is_like:
          case comparison::string_is_not_like:
          case comparison::is_null:
          case comparison::is_not_null:
          case comparison::matches:
          case comparison::does_not_match:
          {
            throw std::invalid_argument("Incorrect constructor for given comparison");
          }
        }

        break;
      }

      case field::type::undefined:
      case field::type::string:
      case field::type::integer:
      case field::type::boolean:
      {
        throw std::domain_error("Matching field must be a join field");
      }
    }
  }

  field filter::getField() const
  {
    if (type_ == type::singleton)
    {
      return singleton_.filterField;
    } else {
      throw std::domain_error("This filter does not have a field");
    }
  }

  filter::comparison filter::getComparison() const
  {
    if (type_ == type::singleton)
    {
      return singleton_.filterType;
    } else {
      throw std::domain_error("This filter does not have a comparison");
    }
  }

  filter filter::getJoinCondition() const
  {
    if (type_ == type::singleton)
    {
      switch (singleton_.filterType)
      {
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          return *singleton_.join;
        }

        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        case comparison::boolean_equals:
        case comparison::is_null:
        case comparison::is_not_null:
        {
          throw std::domain_error("This filter does not have a join condition");
        }
      }
    } else {
      throw std::domain_error("This filter does not have a join condition");
    }
  }

  std::string filter::getStringArgument() const
  {
    if (type_ == type::singleton)
    {
      switch (singleton_.filterType)
      {
        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        {
          return singleton_.stringValue;
        }

        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        case comparison::boolean_equals:
        case comparison::is_null:
        case comparison::is_not_null:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::domain_error("This filter does not have a string argument");
        }
      }
    } else {
      throw std::domain_error("This filter does not have a string argument");
    }
  }

  int filter::getIntegerArgument() const
  {
    if (type_ == type::singleton)
    {
      switch (singleton_.filterType)
      {
        case comparison::int_equals:
        case comparison::int_does_not_equal:
        case comparison::int_is_at_least:
        case comparison::int_is_greater_than:
        case comparison::int_is_at_most:
        case comparison::int_is_less_than:
        {
          return singleton_.intValue;
        }

        case comparison::string_equals:
        case comparison::string_does_not_equal:
        case comparison::string_is_like:
        case comparison::string_is_not_like:
        case comparison::boolean_equals:
        case comparison::is_null:
        case comparison::is_not_null:
        case comparison::matches:
        case comparison::does_not_match:
        case comparison::hierarchally_matches:
        case comparison::does_not_hierarchally_match:
        {
          throw std::domain_error("This filter does not have an integer argument");
        }
      }
    } else {
      throw std::domain_error("This filter does not have an integer argument");
    }
  }

  bool filter::getBooleanArgument() const
  {
    if ((type_ == type::singleton) && (singleton_.filterType == comparison::boolean_equals))
    {
      return singleton_.boolValue;
    } else {
      throw std::domain_error("This filter does not have a boolean argument");
    }
  }

  filter::filter(bool orlogic) : type_(type::group)
  {
    new(&group_.children) std::list<filter>();
    group_.orlogic = orlogic;
  }

  bool filter::getOrlogic() const
  {
    if (type_ == type::group)
    {
      return group_.orlogic;
    } else {
      throw std::domain_error("This filter is not a group filter");
    }
  }

  filter filter::operator+(filter condition) const
  {
    filter result(*this);
    result += std::move(condition);

    return result;
  }

  filter& filter::operator+=(filter condition)
  {
    if (type_ == type::group)
    {
      group_.children.push_back(std::move(condition));

      return *this;
    } else {
      throw std::domain_error("Children can only be added to group filters");
    }
  }

  filter::const_iterator filter::begin() const
  {
    if (type_ == type::group)
    {
      return std::begin(group_.children);
    } else {
      throw std::domain_error("This filter has no children");
    }
  }

  filter::const_iterator filter::end() const
  {
    if (type_ == type::group)
    {
      return std::end(group_.children);
    } else {
      throw std::domain_error("This filter has no children");
    }
  }

  filter filter::operator!() const
  {
    switch (type_)
    {
      case type::empty:
      {
        return {};
      }

      case type::singleton:
      {
        switch (singleton_.filterType)
        {
          case comparison::int_equals:
          {
            return filter(singleton_.filterField, comparison::int_does_not_equal, singleton_.intValue);
          }

          case comparison::int_does_not_equal:
          {
            return filter(singleton_.filterField, comparison::int_equals, singleton_.intValue);
          }

          case comparison::int_is_at_least:
          {
            return filter(singleton_.filterField, comparison::int_is_less_than, singleton_.intValue);
          }

          case comparison::int_is_greater_than:
          {
            return filter(singleton_.filterField, comparison::int_is_at_most, singleton_.intValue);
          }

          case comparison::int_is_at_most:
          {
            return filter(singleton_.filterField, comparison::int_is_greater_than, singleton_.intValue);
          }

          case comparison::int_is_less_than:
          {
            return filter(singleton_.filterField, comparison::int_is_at_least, singleton_.intValue);
          }

          case comparison::boolean_equals:
          {
            return filter(singleton_.filterField, comparison::boolean_equals, !singleton_.boolValue);
          }

          case comparison::string_equals:
          {
            return filter(singleton_.filterField, comparison::string_does_not_equal, singleton_.stringValue);
          }

          case comparison::string_does_not_equal:
          {
            return filter(singleton_.filterField, comparison::string_equals, singleton_.stringValue);
          }

          case comparison::string_is_like:
          {
            return filter(singleton_.filterField, comparison::string_is_not_like, singleton_.stringValue);
          }

          case comparison::string_is_not_like:
          {
            return filter(singleton_.filterField, comparison::string_is_like, singleton_.stringValue);
          }

          case comparison::is_null:
          {
            return filter(singleton_.filterField, comparison::is_not_null);
          }

          case comparison::is_not_null:
          {
            return filter(singleton_.filterField, comparison::is_null);
          }

          case comparison::matches:
          {
            return filter(singleton_.filterField, comparison::does_not_match, *singleton_.join);
          }

          case comparison::does_not_match:
          {
            return filter(singleton_.filterField, comparison::matches, *singleton_.join);
          }

          case comparison::hierarchally_matches:
          {
            return filter(singleton_.filterField, comparison::does_not_hierarchally_match, *singleton_.join);
          }

          case comparison::does_not_hierarchally_match:
          {
            return filter(singleton_.filterField, comparison::hierarchally_matches, *singleton_.join);
          }
        }
      }

      case type::group:
      {
        filter result(!group_.orlogic);

        for (const filter& child : group_.children)
        {
          result += !child;
        }

        return result;
      }
    }
  }

  filter& filter::operator&=(filter condition)
  {
    return (*this = (*this && std::move(condition)));
  }

  filter& filter::operator|=(filter condition)
  {
    return (*this = (*this || std::move(condition)));
  }

  filter filter::operator&&(filter condition) const
  {
    switch (type_)
    {
      case type::empty:
      {
        return condition;
      }

      case type::singleton:
      {
        filter result(false);
        result.group_.children.push_back(*this);
        result.group_.children.push_back(std::move(condition));

        return result;
      }

      case type::group:
      {
        if (group_.orlogic)
        {
          filter result(false);
          result.group_.children.push_back(*this);
          result.group_.children.push_back(std::move(condition));

          return result;
        } else {
          filter result(*this);
          result.group_.children.push_back(std::move(condition));

          return result;
        }
      }
    }
  }

  filter filter::operator||(filter condition) const
  {
    switch (type_)
    {
      case type::empty:
      {
        return condition;
      }

      case type::singleton:
      {
        filter result(true);
        result.group_.children.push_back(*this);
        result.group_.children.push_back(std::move(condition));

        return result;
      }

      case type::group:
      {
        if (!group_.orlogic)
        {
          filter result(true);
          result.group_.children.push_back(*this);
          result.group_.children.push_back(std::move(condition));

          return result;
        } else {
          filter result(*this);
          result.group_.children.push_back(std::move(condition));

          return result;
        }
      }
    }
  }

  filter filter::normalize(object context) const
  {
    {
      switch (type_)
      {
        case type::empty:
        {
          return *this;
        }

        case type::singleton:
        {
          // First, switch on the normalized context, and then switch on the
          // current context. We recursively recontextualize by using the
          // current filter as a subquery for a join such that the context of
          // the subquery is one step closer to the context of the current
          // filter, and then letting the filter constructor normalize the
          // subquery.
          switch (context)
          {
            case object::undefined:
            {
              // An undefined object indicates no participation in
              // recontexualization.
              return *this;
            }

            case object::notion:
            {
              switch (singleton_.filterField.getObject())
              {
                case object::undefined:
                case object::notion:
                {
                  return *this;
                }

                case object::word:
                case object::frame:
                case object::part:
                case object::lemma:
                case object::form:
                case object::pronunciation:
                {
                  return (verbly::notion::word %= *this);
                }
              }
            }

            case object::word:
            {
              switch (singleton_.filterField.getObject())
              {
                case object::notion:
                {
                  return (verbly::word::notion %= *this);
                }

                case object::undefined:
                case object::word:
                {
                  return *this;
                }

                case object::frame:
                case object::part:
                {
                  return (verbly::word::frame %= *this);
                }

                case object::lemma:
                case object::form:
                case object::pronunciation:
                {
                  return (verbly::word::lemma %= *this);
                }
              }

              case object::frame:
              {
                switch (singleton_.filterField.getObject())
                {
                  case object::undefined:
                  case object::frame:
                  {
                    return *this;
                  }

                  case object::notion:
                  case object::word:
                  case object::lemma:
                  case object::form:
                  case object::pronunciation:
                  {
                    return (verbly::frame::word %= *this);
                  }

                  case object::part:
                  {
                    return (verbly::frame::part() %= *this);
                  }
                }
              }

              case object::part:
              {
                switch (singleton_.filterField.getObject())
                {
                  case object::undefined:
                  case object::part:
                  {
                    return *this;
                  }

                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::lemma:
                  case object::form:
                  case object::pronunciation:
                  {
                    return (verbly::part::frame %= *this);
                  }
                }
              }

              case object::lemma:
              {
                switch (singleton_.filterField.getObject())
                {
                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::part:
                  {
                    return verbly::lemma::word %= *this;
                  }

                  case object::undefined:
                  case object::lemma:
                  {
                    return *this;
                  }

                  case object::form:
                  case object::pronunciation:
                  {
                    return (verbly::lemma::form(inflection::base) %= *this);
                  }
                }
              }

              case object::form:
              {
                switch (singleton_.filterField.getObject())
                {
                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::part:
                  case object::lemma:
                  {
                    return verbly::form::lemma %= *this;
                  }

                  case object::undefined:
                  case object::form:
                  {
                    return *this;
                  }

                  case object::pronunciation:
                  {
                    return (verbly::form::pronunciation %= *this);
                  }
                }
              }

              case object::pronunciation:
              {
                switch (singleton_.filterField.getObject())
                {
                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::part:
                  case object::lemma:
                  case object::form:
                  {
                    return verbly::pronunciation::form %= *this;
                  }

                  case object::undefined:
                  case object::pronunciation:
                  {
                    return *this;
                  }
                }
              }
            }
          }
        }

        case type::group:
        {
          filter result(group_.orlogic);
          std::map<field, filter> positiveJoins;
          std::map<field, filter> negativeJoins;

          for (const filter& child : group_.children)
          {
            filter normalized = child.normalize(context);

            // Notably, this does not attempt to merge hierarchal matches,
            // UNLESS they are positive matches being OR-d, or negative
            // matches being ANDed.
            switch (normalized.getType())
            {
              case type::singleton:
              {
                switch (normalized.getComparison())
                {
                  case comparison::matches:
                  {
                    if (!positiveJoins.count(normalized.singleton_.filterField))
                    {
                      positiveJoins[normalized.getField()] = filter(group_.orlogic);
                    }

                    positiveJoins.at(normalized.getField()) += std::move(*normalized.singleton_.join);

                    break;
                  }

                  case comparison::does_not_match:
                  {
                    if (!negativeJoins.count(normalized.singleton_.filterField))
                    {
                      negativeJoins[normalized.getField()] = filter(!group_.orlogic);
                    }

                    negativeJoins.at(normalized.getField()) += std::move(*normalized.singleton_.join);

                    break;
                  }

                  case comparison::hierarchally_matches:
                  {
                    if (group_.orlogic)
                    {
                      positiveJoins[normalized.getField()] |= std::move(*normalized.singleton_.join);
                    } else {
                      result += std::move(normalized);
                    }

                    break;
                  }

                  case comparison::does_not_hierarchally_match:
                  {
                    if (!group_.orlogic)
                    {
                      negativeJoins[normalized.getField()] |= std::move(*normalized.singleton_.join);
                    } else {
                      result += std::move(normalized);
                    }

                    break;
                  }

                  case comparison::int_equals:
                  case comparison::int_does_not_equal:
                  case comparison::int_is_at_least:
                  case comparison::int_is_greater_than:
                  case comparison::int_is_at_most:
                  case comparison::int_is_less_than:
                  case comparison::boolean_equals:
                  case comparison::string_equals:
                  case comparison::string_does_not_equal:
                  case comparison::string_is_like:
                  case comparison::string_is_not_like:
                  case comparison::is_null:
                  case comparison::is_not_null:
                  {
                    result += std::move(normalized);

                    break;
                  }
                }

                break;
              }

              case type::group:
              case type::empty:
              {
                result += std::move(normalized);

                break;
              }
            }
          }

          for (auto& mapping : positiveJoins)
          {
            const field& joinOn = mapping.first;
            filter& joinCondition = mapping.second;

            result += (joinOn %= joinCondition.normalize(joinOn.getJoinObject()));
          }

          for (auto& mapping : negativeJoins)
          {
            const field& joinOn = mapping.first;
            filter& joinCondition = mapping.second;

            result += !(joinOn %= joinCondition.normalize(joinOn.getJoinObject()));
          }

          return result;
        }
      }
    }
  }

  filter filter::compact() const
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
        filter result(group_.orlogic);
        for (const filter& child : group_.children)
        {
          filter compactedChild = child.compact();
          if (compactedChild.type_ != type::empty)
          {
            result += child;
          }
        }

        if (group_.children.empty())
        {
          result = {};
        }

        return result;
      }
    }
  }

};
