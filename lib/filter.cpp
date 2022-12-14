#include "filter.h"
#include <stdexcept>
#include <map>
#include "notion.h"
#include "word.h"
#include "frame.h"
#include "part.h"
#include "form.h"
#include "pronunciation.h"

namespace verbly {

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
          variant_ = singleton_type
            {
              std::move(filterField),
              filterType,
              filterValue
            };

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
        case comparison::field_equals:
        case comparison::field_does_not_equal:
        {
          throw std::invalid_argument("Invalid comparison for integer field");
        }
      }
    } else {
      throw std::domain_error(
        "Cannot match a non-integer field against an integer value");
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
          variant_ = singleton_type
            {
              std::move(filterField),
              filterType,
              std::move(filterValue)
            };

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
        case comparison::field_equals:
        case comparison::field_does_not_equal:
        {
          throw std::invalid_argument("Invalid comparison for string field");
        }
      }
    } else {
      throw std::domain_error(
        "Cannot match a non-string field against an string value");
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
          variant_ = singleton_type
            {
              std::move(filterField),
              filterType,
              filterValue
            };

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
        case comparison::field_equals:
        case comparison::field_does_not_equal:
        {
          throw std::invalid_argument("Invalid comparison for boolean field");
        }
      }
    } else {
      throw std::domain_error(
        "Cannot match a non-boolean field against a boolean value");
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
          variant_ = singleton_type
            {
              std::move(filterField),
              filterType
            };

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
        case comparison::field_equals:
        case comparison::field_does_not_equal:
        {
          throw std::invalid_argument(
            "Incorrect constructor for given comparison");
        }
      }
    } else {
      throw std::domain_error(
        "Cannot check nullity/non-nullity of non-nullable field");
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
      case field::type::join_through_where:
      {
        switch (filterType)
        {
          case comparison::matches:
          case comparison::does_not_match:
          {
            variant_ = singleton_type
              {
                std::move(joinOn),
                filterType,
                rec_filter(new filter(std::move(joinCondition)))
              };

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
          case comparison::field_equals:
          case comparison::field_does_not_equal:
          {
            throw std::invalid_argument(
              "Incorrect constructor for given comparison");
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
            variant_ = singleton_type
              {
                std::move(joinOn),
                filterType,
                rec_filter(new filter(std::move(joinCondition)))
              };

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
          case comparison::field_equals:
          case comparison::field_does_not_equal:
          {
            throw std::invalid_argument(
              "Incorrect constructor for given comparison");
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

  filter::filter(
    field filterField,
    comparison filterType,
    field compareField) :
      type_(type::singleton)
  {
    switch (filterType)
    {
      case comparison::field_equals:
      case comparison::field_does_not_equal:
      {
        if (filterField.getType() != compareField.getType())
        {
          throw std::invalid_argument("Cannot compare two fields of different types");
        }

        if (filterField.isJoin())
        {
          throw std::domain_error("Cannot compare join fields");
        }

        variant_ = singleton_type
          {
            std::move(filterField),
            filterType,
            std::move(compareField)
          };

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
      case comparison::hierarchally_matches:
      case comparison::does_not_hierarchally_match:
      {
        throw std::domain_error("Incorrect constructor for given comparison");
      }
    }
  }

  field filter::getField() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have a field");
    }

    return std::get<singleton_type>(variant_).filterField;
  }

  filter::comparison filter::getComparison() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have a comparison");
    }

    return std::get<singleton_type>(variant_).filterType;
  }

  filter filter::getJoinCondition() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have a join condition");
    }

    const singleton_type& ss = std::get<singleton_type>(variant_);

    switch (ss.filterType)
    {
      case comparison::matches:
      case comparison::does_not_match:
      case comparison::hierarchally_matches:
      case comparison::does_not_hierarchally_match:
      {
        return *std::get<rec_filter>(ss.data);
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
      case comparison::field_equals:
      case comparison::field_does_not_equal:
      {
        throw std::domain_error("This filter does not have a join condition");
      }
    }
  }

  std::string filter::getStringArgument() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have a string argument");
    }

    const singleton_type& ss = std::get<singleton_type>(variant_);

    switch (ss.filterType)
    {
      case comparison::string_equals:
      case comparison::string_does_not_equal:
      case comparison::string_is_like:
      case comparison::string_is_not_like:
      {
        return std::get<std::string>(ss.data);
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
      case comparison::field_equals:
      case comparison::field_does_not_equal:
      {
        throw std::domain_error("This filter does not have a string argument");
      }
    }
  }

  int filter::getIntegerArgument() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have an integer argument");
    }

    const singleton_type& ss = std::get<singleton_type>(variant_);

    switch (ss.filterType)
    {
      case comparison::int_equals:
      case comparison::int_does_not_equal:
      case comparison::int_is_at_least:
      case comparison::int_is_greater_than:
      case comparison::int_is_at_most:
      case comparison::int_is_less_than:
      {
        return std::get<int>(ss.data);
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
      case comparison::field_equals:
      case comparison::field_does_not_equal:
      {
        throw std::domain_error(
          "This filter does not have an integer argument");
      }
    }
  }

  bool filter::getBooleanArgument() const
  {
    if ((type_ != type::singleton) ||
      (std::get<singleton_type>(variant_).filterType !=
        comparison::boolean_equals))
    {
      throw std::domain_error("This filter does not have a boolean argument");
    }

    return std::get<bool>(std::get<singleton_type>(variant_).data);
  }

  field filter::getCompareField() const
  {
    if (type_ != type::singleton)
    {
      throw std::domain_error("This filter does not have a compare field");
    }

    const singleton_type& ss = std::get<singleton_type>(variant_);

    switch (ss.filterType)
    {
      case comparison::field_equals:
      case comparison::field_does_not_equal:
      {
        return std::get<field>(ss.data);

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
      case comparison::hierarchally_matches:
      case comparison::does_not_hierarchally_match:
      {
        throw std::domain_error("This filter doesn't have a compare field");
      }
    }

  }

  filter::filter(bool orlogic) :
    type_(type::group),
    variant_(group_type {{}, orlogic})
  {
  }

  bool filter::getOrlogic() const
  {
    if (type_ != type::group)
    {
      throw std::domain_error("This filter is not a group filter");
    }

    return std::get<group_type>(variant_).orlogic;
  }

  filter filter::operator+(filter condition) const
  {
    filter result(*this);
    result += std::move(condition);

    return result;
  }

  filter& filter::operator+=(filter condition)
  {
    if (type_ != type::group)
    {
      throw std::domain_error("Children can only be added to group filters");
    }

    std::get<group_type>(variant_).children.push_back(std::move(condition));

    return *this;
  }

  filter::const_iterator filter::begin() const
  {
    if (type_ != type::group)
    {
      throw std::domain_error("This filter has no children");
    }

    return std::begin(std::get<group_type>(variant_).children);
  }

  filter::const_iterator filter::end() const
  {
    if (type_ != type::group)
    {
      throw std::domain_error("This filter has no children");
    }

    return std::end(std::get<group_type>(variant_).children);
  }

  filter::filter(
    std::string name,
    bool internal,
    filter subfilter) :
      type_(type::mask),
      variant_(mask_type {
        std::move(name),
        internal,
        rec_filter(new filter(std::move(subfilter)))
      })
  {
  }

  const std::string& filter::getMaskName() const
  {
    if (type_ != type::mask)
    {
      throw std::domain_error("This filter is not a mask filter");
    }

    return std::get<mask_type>(variant_).name;
  }

  bool filter::isMaskInternal() const
  {
    if (type_ != type::mask)
    {
      throw std::domain_error("This filter is not a mask filter");
    }

    return std::get<mask_type>(variant_).internal;
  }

  const filter& filter::getMaskFilter() const
  {
    if (type_ != type::mask)
    {
      throw std::domain_error("This filter is not a mask filter");
    }

    return *std::get<mask_type>(variant_).subfilter;
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
        const singleton_type& ss = std::get<singleton_type>(variant_);

        switch (ss.filterType)
        {
          case comparison::int_equals:
          {
            return filter(
              ss.filterField,
              comparison::int_does_not_equal,
              std::get<int>(ss.data));
          }

          case comparison::int_does_not_equal:
          {
            return filter(
              ss.filterField,
              comparison::int_equals,
              std::get<int>(ss.data));
          }

          case comparison::int_is_at_least:
          {
            return filter(
              ss.filterField,
              comparison::int_is_less_than,
              std::get<int>(ss.data));
          }

          case comparison::int_is_greater_than:
          {
            return filter(
              ss.filterField,
              comparison::int_is_at_most,
              std::get<int>(ss.data));
          }

          case comparison::int_is_at_most:
          {
            return filter(
              ss.filterField,
              comparison::int_is_greater_than,
              std::get<int>(ss.data));
          }

          case comparison::int_is_less_than:
          {
            return filter(
              ss.filterField,
              comparison::int_is_at_least,
              std::get<int>(ss.data));
          }

          case comparison::boolean_equals:
          {
            return filter(
              ss.filterField,
              comparison::boolean_equals,
              !std::get<int>(ss.data));
          }

          case comparison::string_equals:
          {
            return filter(
              ss.filterField,
              comparison::string_does_not_equal,
              std::get<std::string>(ss.data));
          }

          case comparison::string_does_not_equal:
          {
            return filter(
              ss.filterField,
              comparison::string_equals,
              std::get<std::string>(ss.data));
          }

          case comparison::string_is_like:
          {
            return filter(
              ss.filterField,
              comparison::string_is_not_like,
              std::get<std::string>(ss.data));
          }

          case comparison::string_is_not_like:
          {
            return filter(
              ss.filterField,
              comparison::string_is_like,
              std::get<std::string>(ss.data));
          }

          case comparison::is_null:
          {
            return filter(
              ss.filterField,
              comparison::is_not_null);
          }

          case comparison::is_not_null:
          {
            return filter(
              ss.filterField,
              comparison::is_null);
          }

          case comparison::matches:
          {
            return filter(
              ss.filterField,
              comparison::does_not_match,
              *std::get<rec_filter>(ss.data));
          }

          case comparison::does_not_match:
          {
            return filter(
              ss.filterField,
              comparison::matches,
              *std::get<rec_filter>(ss.data));
          }

          case comparison::hierarchally_matches:
          {
            return filter(
              ss.filterField,
              comparison::does_not_hierarchally_match,
              *std::get<rec_filter>(ss.data));
          }

          case comparison::does_not_hierarchally_match:
          {
            return filter(
              ss.filterField,
              comparison::hierarchally_matches,
              *std::get<rec_filter>(ss.data));
          }

          case comparison::field_equals:
          {
            return filter(
              ss.filterField,
              comparison::field_does_not_equal,
              std::get<field>(ss.data));
          }

          case comparison::field_does_not_equal:
          {
            return filter(
              ss.filterField,
              comparison::field_equals,
              std::get<field>(ss.data));
          }
        }
      }

      case type::group:
      {
        const group_type& gg = std::get<group_type>(variant_);

        filter result(!gg.orlogic);

        for (const filter& child : gg.children)
        {
          result += !child;
        }

        return result;
      }

      case type::mask:
      {
        const mask_type& mm = std::get<mask_type>(variant_);

        return {mm.name, mm.internal, !*mm.subfilter};
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
      case type::mask:
      {
        filter result(false);

        group_type& gg = std::get<group_type>(result.variant_);

        gg.children.push_back(*this);
        gg.children.push_back(std::move(condition));

        return result;
      }

      case type::group:
      {
        const group_type& og = std::get<group_type>(variant_);

        if (og.orlogic)
        {
          filter result(false);

          group_type& gg = std::get<group_type>(result.variant_);

          gg.children.push_back(*this);
          gg.children.push_back(std::move(condition));

          return result;
        } else {
          filter result(*this);

          group_type& gg = std::get<group_type>(result.variant_);

          gg.children.push_back(std::move(condition));

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
      case type::mask:
      {
        filter result(true);

        group_type& gg = std::get<group_type>(result.variant_);

        gg.children.push_back(*this);
        gg.children.push_back(std::move(condition));

        return result;
      }

      case type::group:
      {
        const group_type& og = std::get<group_type>(variant_);

        if (!og.orlogic)
        {
          filter result(true);

          group_type& gg = std::get<group_type>(result.variant_);

          gg.children.push_back(*this);
          gg.children.push_back(std::move(condition));

          return result;
        } else {
          filter result(*this);

          group_type& gg = std::get<group_type>(result.variant_);

          gg.children.push_back(std::move(condition));

          return result;
        }
      }
    }
  }

  filter filter::mask(std::string name, filter subfilter)
  {
    return {std::move(name), false, std::move(subfilter)};
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
          const singleton_type& ss = std::get<singleton_type>(variant_);

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
              switch (ss.filterField.getObject())
              {
                case object::undefined:
                case object::notion:
                {
                  return *this;
                }

                case object::word:
                case object::frame:
                case object::part:
                case object::form:
                case object::pronunciation:
                {
                  return (verbly::notion::words %= normalize(object::word));
                }
              }
            }

            case object::word:
            {
              switch (ss.filterField.getObject())
              {
                case object::notion:
                {
                  return (verbly::word::notions %= normalize(object::notion));
                }

                case object::undefined:
                case object::word:
                {
                  return *this;
                }

                case object::frame:
                case object::part:
                {
                  return (verbly::word::frames %= normalize(object::frame));
                }

                case object::form:
                case object::pronunciation:
                {
                  return (verbly::word::forms(inflection::base) %=
                    normalize(object::form));
                }
              }

              case object::frame:
              {
                switch (ss.filterField.getObject())
                {
                  case object::undefined:
                  case object::frame:
                  {
                    return *this;
                  }

                  case object::notion:
                  case object::word:
                  case object::form:
                  case object::pronunciation:
                  {
                    return (verbly::frame::words %= normalize(object::word));
                  }

                  case object::part:
                  {
                    return (verbly::frame::parts() %= normalize(object::part));
                  }
                }
              }

              case object::part:
              {
                switch (ss.filterField.getObject())
                {
                  case object::undefined:
                  case object::part:
                  {
                    return *this;
                  }

                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::form:
                  case object::pronunciation:
                  {
                    return (verbly::part::frames %= normalize(object::frame));
                  }
                }
              }

              case object::form:
              {
                switch (ss.filterField.getObject())
                {
                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::part:
                  {
                    return (verbly::form::words(inflection::base) %=
                      normalize(object::word));
                  }

                  case object::undefined:
                  case object::form:
                  {
                    return *this;
                  }

                  case object::pronunciation:
                  {
                    return (verbly::form::pronunciations %=
                      normalize(object::pronunciation));
                  }
                }
              }

              case object::pronunciation:
              {
                switch (ss.filterField.getObject())
                {
                  case object::notion:
                  case object::word:
                  case object::frame:
                  case object::part:
                  case object::form:
                  {
                    return verbly::pronunciation::forms %=
                      normalize(object::form);
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
          const group_type& gg = std::get<group_type>(variant_);

          filter result(gg.orlogic);
          std::map<field, filter> positiveJoins;
          std::map<field, filter> negativeJoins;
          std::map<std::tuple<std::string, bool>, filter> masks;

          for (const filter& child : gg.children)
          {
            filter normalized = child.normalize(context);

            // Notably, this does not attempt to merge hierarchal matches,
            // UNLESS they are positive matches being OR-d, or negative
            // matches being ANDed.
            switch (normalized.getType())
            {
              case type::singleton:
              {
                singleton_type& normSing =
                  std::get<singleton_type>(normalized.variant_);

                switch (normalized.getComparison())
                {
                  case comparison::matches:
                  {
                    if (!positiveJoins.count(normSing.filterField))
                    {
                      positiveJoins[normalized.getField()] = filter(gg.orlogic);
                    }

                    positiveJoins.at(normalized.getField()) +=
                      std::move(*std::get<rec_filter>(normSing.data));

                    break;
                  }

                  case comparison::does_not_match:
                  {
                    if (!negativeJoins.count(normSing.filterField))
                    {
                      negativeJoins[normalized.getField()] =
                        filter(!gg.orlogic);
                    }

                    negativeJoins.at(normalized.getField()) +=
                      std::move(*std::get<rec_filter>(normSing.data));

                    break;
                  }

                  case comparison::hierarchally_matches:
                  {
                    if (gg.orlogic)
                    {
                      positiveJoins[normalized.getField()] |=
                        std::move(*std::get<rec_filter>(normSing.data));
                    } else {
                      result += std::move(normalized);
                    }

                    break;
                  }

                  case comparison::does_not_hierarchally_match:
                  {
                    if (!gg.orlogic)
                    {
                      negativeJoins[normalized.getField()] |=
                        std::move(*std::get<rec_filter>(normSing.data));
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
                  case comparison::field_equals:
                  case comparison::field_does_not_equal:
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

              case type::mask:
              {
                mask_type& normMask =
                  std::get<mask_type>(normalized.variant_);

                auto maskId =
                  std::tie(
                    normMask.name,
                    normMask.internal);

                if (!masks.count(maskId))
                {
                  masks[maskId] = filter(gg.orlogic);
                }

                masks.at(maskId) += std::move(*normMask.subfilter);

                break;
              }
            }
          }

          for (auto& mapping : positiveJoins)
          {
            const field& joinOn = mapping.first;
            filter& joinCondition = mapping.second;

            result +=
              (joinOn %= joinCondition.normalize(joinOn.getJoinObject()));
          }

          for (auto& mapping : negativeJoins)
          {
            const field& joinOn = mapping.first;
            filter& joinCondition = mapping.second;

            result +=
              !(joinOn %= joinCondition.normalize(joinOn.getJoinObject()));
          }

          for (auto& mapping : masks)
          {
            const std::string& name = std::get<0>(mapping.first);
            bool internal = std::get<1>(mapping.first);
            filter& subfilter = mapping.second;

            result += {name, internal, std::move(subfilter)};
          }

          return result;
        }

        case type::mask:
        {
          const mask_type& mm = std::get<mask_type>(variant_);

          return {
            mm.name,
            mm.internal,
            mm.subfilter->normalize(context)};
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
        const group_type& gg = std::get<group_type>(variant_);

        filter result(gg.orlogic);
        for (const filter& child : gg.children)
        {
          filter compactedChild = child.compact();
          if (compactedChild.type_ != type::empty)
          {
            result += child;
          }
        }

        group_type& resGroup = std::get<group_type>(result.variant_);

        if (resGroup.children.empty())
        {
          result = {};
        } else if (resGroup.children.size() == 1)
        {
          filter tempChild = std::move(resGroup.children.front());

          result = std::move(tempChild);
        }

        return result;
      }

      case type::mask:
      {
        const mask_type& mm = std::get<mask_type>(variant_);

        filter subfilter = mm.subfilter->compact();

        if (subfilter.type_ == type::empty)
        {
          return {};
        } else {
          return {
            mm.name,
            mm.internal,
            std::move(subfilter)};
        }
      }
    }
  }

};
