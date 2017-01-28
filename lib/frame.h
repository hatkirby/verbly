#ifndef FRAME_H_EA29065A
#define FRAME_H_EA29065A

#include <stdexcept>
#include <list>
#include "field.h"
#include "filter.h"
#include "part.h"

struct sqlite3_stmt;

namespace verbly {

  class database;

  class frame {
  public:

    // Default constructor

    frame() = default;

    // Construct from database

    frame(const database& db, sqlite3_stmt* row);

    // Accessors

    operator bool() const
    {
      return valid_;
    }

    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized frame");
      }

      return id_;
    }

    int getLength() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized frame");
      }

      return length_;
    }

    const std::vector<part>& getParts() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized frame");
      }

      return parts_;
    }

    // Type info

    static const object objectType;

    static const std::list<std::string> select;

    // Query fields

    static const field id;

    static const field length;

    operator filter() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized frame");
      }

      return (id == id_);
    }

    // Relationships to other objects

    static const field word;

    static field part();
    static field part(int index);

  private:
    bool valid_ = false;

    int id_;
    int groupId_;
    int length_;
    std::vector<class part> parts_;

    const database* db_;

  };

};

#endif /* end of include guard: FRAME_H_EA29065A */
