#ifndef BINDING_H_CAE0B18E
#define BINDING_H_CAE0B18E

#include <string>

namespace verbly {

  class binding {
    public:
      enum class type {
        invalid,
        integer,
        string,
        field
      };

      // Default constructor

      binding()
      {
      }

      // Copy and move constructors

      binding(const binding& other);
      binding(binding&& other);

      // Assignment

      binding& operator=(binding other);

      // Swap

      friend void swap(binding& first, binding& second);

      // Destructor

      ~binding();

      // Generic accessors

      type getType() const
      {
        return type_;
      }

      // Integer

      binding(int arg);

      int getInteger() const;

      // String

      binding(std::string arg);

      std::string getString() const;

      // Field

      binding(std::string table, std::string column);

      std::string getTable() const;
      std::string getColumn() const;

    private:

      union {
        int integer_;
        std::string string_;
        struct {
          std::string table_;
          std::string column_;
        } field_;
      };

      type type_ = type::invalid;
  };

};

#endif /* end of include guard: BINDING_H_CAE0B18E */
