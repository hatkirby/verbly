#include "field.h"
#include <stdexcept>
#include <utility>

namespace verbly {
  namespace generator {

    field::field(const field& other)
    {
      type_ = other.type_;
      name_ = other.name_;

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

        case type::invalid:
        {
          break;
        }
      }
    }

    field::field(field&& other) : field()
    {
      swap(*this, other);
    }

    field& field::operator=(field other)
    {
      swap(*this, other);

      return *this;
    }

    void swap(field& first, field& second)
    {
      using type = field::type;

      type tempType = first.type_;
      std::string tempName = std::move(first.name_);
      int tempInteger;
      std::string tempString;

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

        case type::invalid:
        {
          break;
        }
      }

      first.~field();

      first.type_ = second.type_;
      first.name_ = std::move(second.name_);

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

        case type::invalid:
        {
          break;
        }
      }

      second.~field();

      second.type_ = tempType;
      second.name_ = std::move(tempName);

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

        case type::invalid:
        {
          break;
        }
      }
    }

    field::~field()
    {
      switch (type_)
      {
        case type::string:
        {
          using string_type = std::string;
          string_.~string_type();

          break;
        }

        case type::integer:
        case type::invalid:
        {
          break;
        }
      }
    }

    field::field(
      std::string name,
      int arg) :
        type_(type::integer),
        name_(name),
        integer_(arg)
    {
    }

    int field::getInteger() const
    {
      if (type_ != type::integer)
      {
        throw std::domain_error("field::getInteger called on non-integer field");
      }

      return integer_;
    }

    field::field(
      std::string name,
      std::string arg) :
        type_(type::string),
        name_(name)
    {
      new(&string_) std::string(arg);
    }

    std::string field::getString() const
    {
      if (type_ != type::string)
      {
        throw std::domain_error("field::getString called on non-string field");
      }

      return string_;
    }

  };
};
