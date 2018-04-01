#ifndef FRAME_H_26770FF1
#define FRAME_H_26770FF1

#include <list>
#include <vector>
#include "part.h"

namespace verbly {
  namespace generator {

    class frame {
    public:

      // Aliases

      using const_iterator = std::list<part>::const_iterator;

      // Constructor

      frame();

      // Duplication

      static frame duplicate(const frame& other);

      // Mutators

      void push_back(part fp);

      // Accessors

      int getId() const
      {
        return id_;
      }

      int getLength() const
      {
        return parts_.size();
      }

      const part& operator[](int index) const
      {
        return parts_.at(index);
      }

    private:

      static int nextId_;

      const int id_;

      std::vector<part> parts_;

    };

  };
};

#endif /* end of include guard: FRAME_H_26770FF1 */
