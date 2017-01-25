#ifndef ROLE_H_249F9A9C
#define ROLE_H_249F9A9C

#include <stdexcept>
#include <string>
#include "../lib/selrestr.h"

namespace verbly {

  class role {
  public:

    // Default constructor

    role() = default;

    // Constructor

    role(
      std::string name,
      selrestr selrestrs = {}) :
        valid_(true),
        name_(name),
        selrestrs_(selrestrs)
    {
    }

    // Accessors

    const std::string& getName() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to invalid role");
      }

      return name_;
    }

    const selrestr& getSelrestrs() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to invalid role");
      }

      return selrestrs_;
    }

  private:

    bool valid_ = false;
    std::string name_;
    selrestr selrestrs_;

  };

};

#endif /* end of include guard: ROLE_H_249F9A9C */
