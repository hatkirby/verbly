#include "frame.h"

namespace verbly {
  namespace generator {

    int frame::nextId_ = 0;

    frame::frame() : id_(nextId_++)
    {
    }

    frame frame::duplicate(const frame& other)
    {
      frame result;

      for (const part& p : other.parts_)
      {
        result.push_back(part::duplicate(p));
      }

      return result;
    }

    void frame::push_back(part fp)
    {
      parts_.push_back(std::move(fp));
    }

  };
};
