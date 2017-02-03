#include "field.h"
#include "filter.h"

namespace verbly {

  filter field::operator==(int value) const
  {
    return filter(*this, filter::comparison::int_equals, value);
  }

  filter field::operator!=(int value) const
  {
    return filter(*this, filter::comparison::int_does_not_equal, value);
  }

  filter field::operator<(int value) const
  {
    return filter(*this, filter::comparison::int_is_less_than, value);
  }

  filter field::operator<=(int value) const
  {
    return filter(*this, filter::comparison::int_is_at_most, value);
  }

  filter field::operator>(int value) const
  {
    return filter(*this, filter::comparison::int_is_greater_than, value);
  }

  filter field::operator>=(int value) const
  {
    return filter(*this, filter::comparison::int_is_at_least, value);
  }

  filter field::operator==(part_of_speech value) const
  {
    return filter(*this, filter::comparison::int_equals, static_cast<int>(value));
  }

  filter field::operator==(positioning value) const
  {
    return filter(*this, filter::comparison::int_equals, static_cast<int>(value));
  }

  filter field::operator==(inflection value) const
  {
    return filter(*this, filter::comparison::int_equals, static_cast<int>(value));
  }

  filter field::operator==(part_type value) const
  {
    return filter(*this, filter::comparison::int_equals, static_cast<int>(value));
  }

  filter field::operator!=(part_of_speech value) const
  {
    return filter(*this, filter::comparison::int_does_not_equal, static_cast<int>(value));
  }

  filter field::operator!=(positioning value) const
  {
    return filter(*this, filter::comparison::int_does_not_equal, static_cast<int>(value));
  }

  filter field::operator!=(inflection value) const
  {
    return filter(*this, filter::comparison::int_does_not_equal, static_cast<int>(value));
  }

  filter field::operator!=(part_type value) const
  {
    return filter(*this, filter::comparison::int_does_not_equal, static_cast<int>(value));
  }

  filter field::operator==(bool value) const
  {
    return filter(*this, filter::comparison::boolean_equals, value);
  }

  filter field::operator==(std::string value) const
  {
    return filter(*this, filter::comparison::string_equals, std::move(value));
  }

  filter field::operator!=(std::string value) const
  {
    return filter(*this, filter::comparison::string_does_not_equal, std::move(value));
  }

  filter field::operator%=(std::string value) const
  {
    return filter(*this, filter::comparison::string_is_like, std::move(value));
  }

  filter field::operator==(const char* value) const
  {
    return filter(*this, filter::comparison::string_equals, std::string(value));
  }

  filter field::operator!=(const char* value) const
  {
    return filter(*this, filter::comparison::string_does_not_equal, std::string(value));
  }

  filter field::operator%=(const char* value) const
  {
    return filter(*this, filter::comparison::string_is_like, std::string(value));
  }

  field::operator filter() const
  {
    if (isJoin())
    {
      return filter(*this, filter::comparison::matches, filter());
    } else {
      return filter(*this, filter::comparison::is_not_null);
    }
  }

  filter field::operator!() const
  {
    if (isJoin())
    {
      return filter(*this, filter::comparison::does_not_match, filter());
    } else {
      return filter(*this, filter::comparison::is_null);
    }
  }

  filter field::operator%=(filter joinCondition) const
  {
    if (type_ == type::hierarchal_join)
    {
      return filter(*this, filter::comparison::hierarchally_matches, std::move(joinCondition));
    } else {
      return filter(*this, filter::comparison::matches, std::move(joinCondition));
    }
  }

};
