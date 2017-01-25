#ifndef BINDING_H_CAE0B18E
#define BINDING_H_CAE0B18E

#include <string>

namespace verbly {
  namespace generator {

    class field {
      public:
        enum class type {
          invalid,
          integer,
          string
        };

        // Copy and move constructors

        field(const field& other);
        field(field&& other);

        // Assignment

        field& operator=(field other);

        // Swap

        friend void swap(field& first, field& second);

        // Destructor

        ~field();

        // Generic accessors

        type getType() const
        {
          return type_;
        }

        std::string getName() const
        {
          return name_;
        }

        // Integer

        field(std::string name, int arg);

        int getInteger() const;

        // String

        field(std::string name, std::string arg);

        std::string getString() const;

      private:

        field()
        {
        }

        union {
          int integer_;
          std::string string_;
        };

        type type_ = type::invalid;
        std::string name_;
    };

  };
};

#endif /* end of include guard: BINDING_H_CAE0B18E */
