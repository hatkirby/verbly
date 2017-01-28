#ifndef ORDER_H_0EC669D5
#define ORDER_H_0EC669D5

#include <stdexcept>
#include "field.h"

namespace verbly {

  class order {
  public:
    enum class type {
      random,
      field
    };

    // Type

    type getType() const
    {
      return type_;
    }

    // Random

    order() : type_(type::random)
    {
    }

    // Field

    order(
      field arg,
      bool asc = true) :
        type_(type::field),
        sortField_(std::move(arg)),
        ascending_(asc)
    {
    }

    field getSortField() const
    {
      if (type_ != type::field)
      {
        throw std::domain_error("Invalid access to non-field order");
      }

      return sortField_;
    }

    bool isAscending() const
    {
      if (type_ != type::field)
      {
        throw std::domain_error("Invalid access to non-field order");
      }

      return ascending_;
    }

  private:
    type type_;
    field sortField_;
    bool ascending_;

  };

};

#endif /* end of include guard: ORDER_H_0EC669D5 */
