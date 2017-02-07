#include "binding.h"
#include <stdexcept>
#include <utility>

namespace verbly {

  binding::binding(const binding& other)
  {
    type_ = other.type_;

    switch (type_)
    {
      case type::integer:
      {
        integer_ = other.integer_;

        break;
      }

      case type::string:
      {
        new(&string_) std::string(other.string_);

        break;
      }

      case type::field:
      {
        new(&field_.table_) std::string(other.field_.table_);
        new(&field_.column_) std::string(other.field_.column_);

        break;
      }

      case type::invalid:
      {
        break;
      }
    }
  }

  binding::binding(binding&& other) : binding()
  {
    swap(*this, other);
  }

  binding& binding::operator=(binding other)
  {
    swap(*this, other);

    return *this;
  }

  void swap(binding& first, binding& second)
  {
    using type = binding::type;

    type tempType = first.type_;
    int tempInteger;
    std::string tempString;
    std::string tempTable;
    std::string tempColumn;

    switch (first.type_)
    {
      case type::integer:
      {
        tempInteger = first.integer_;

        break;
      }

      case type::string:
      {
        tempString = std::move(tempString);

        break;
      }

      case type::field:
      {
        tempTable = std::move(first.field_.table_);
        tempColumn = std::move(first.field_.column_);

        break;
      }

      case type::invalid:
      {
        break;
      }
    }

    first.~binding();

    first.type_ = second.type_;

    switch (second.type_)
    {
      case type::integer:
      {
        first.integer_ = second.integer_;

        break;
      }

      case type::string:
      {
        new(&first.string_) std::string(std::move(second.string_));

        break;
      }

      case type::field:
      {
        new(&first.field_.table_) std::string(std::move(second.field_.table_));
        new(&first.field_.column_) std::string(std::move(second.field_.column_));

        break;
      }

      case type::invalid:
      {
        break;
      }
    }

    second.~binding();

    second.type_ = tempType;

    switch (tempType)
    {
      case type::integer:
      {
        second.integer_ = tempInteger;

        break;
      }

      case type::string:
      {
        new(&second.string_) std::string(std::move(tempString));

        break;
      }

      case type::field:
      {
        new(&first.field_.table_) std::string(std::move(tempTable));
        new(&first.field_.column_) std::string(std::move(tempColumn));

        break;
      }

      case type::invalid:
      {
        break;
      }
    }
  }

  binding::~binding()
  {
    switch (type_)
    {
      case type::string:
      {
        using string_type = std::string;
        string_.~string_type();

        break;
      }

      case type::field:
      {
        using string_type = std::string;
        field_.table_.~string_type();
        field_.column_.~string_type();

        break;
      }

      case type::integer:
      case type::invalid:
      {
        break;
      }
    }
  }

  binding::binding(int arg) :
    type_(type::integer),
    integer_(arg)
  {
  }

  int binding::getInteger() const
  {
    if (type_ != type::integer)
    {
      throw std::domain_error("binding::getInteger called on non-integer binding");
    }

    return integer_;
  }

  binding::binding(std::string arg) : type_(type::string)
  {
    new(&string_) std::string(std::move(arg));
  }

  std::string binding::getString() const
  {
    if (type_ != type::string)
    {
      throw std::domain_error("binding::getString called on non-string binding");
    }

    return string_;
  }

  binding::binding(std::string table, std::string column) : type_(type::field)
  {
    new(&field_.table_) std::string(std::move(table));
    new(&field_.column_) std::string(std::move(column));
  }

  std::string binding::getTable() const
  {
    if (type_ != type::field)
    {
      throw std::domain_error("binding::getTable called on non-field binding");
    }

    return field_.table_;
  }

  std::string binding::getColumn() const
  {
    if (type_ != type::field)
    {
      throw std::domain_error("binding::getColumn called on non-field binding");
    }

    return field_.column_;
  }

};
