#ifndef FRAME_H_26770FF1
#define FRAME_H_26770FF1

#include <list>
#include "part.h"

namespace verbly {
  namespace generator {

    class database;

    class frame {
    public:

      // Aliases

      using const_iterator = std::list<part>::const_iterator;

      // Constructor

      frame();

      // Mutators

      void push_back(part fp);

      // Accessors

      int getId() const
      {
        return id_;
      }

      const_iterator begin() const
      {
        return std::begin(parts_);
      }

      const_iterator end() const
      {
        return std::end(parts_);
      }

    private:

      static int nextId_;

      const int id_;

      std::list<part> parts_;

    };

    database& operator<<(database& db, const frame& arg);

  };
};

#endif /* end of include guard: FRAME_H_26770FF1 */
