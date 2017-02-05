#ifndef ROLE_H_249F9A9C
#define ROLE_H_249F9A9C

#include <stdexcept>
#include <string>
#include <set>

namespace verbly {

  class role {
  public:

    // Default constructor

    role() = default;

    // Constructor

    role(
      std::string name,
      std::set<std::string> selrestrs = {}) :
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

    const std::set<std::string>& getSelrestrs() const
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
    std::set<std::string> selrestrs_;

  };

};

#endif /* end of include guard: ROLE_H_249F9A9C */
