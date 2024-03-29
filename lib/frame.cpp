#include "frame.h"
#include "database.h"
#include "query.h"

namespace verbly {

  const object frame::objectType = object::frame;

  const std::list<std::string> frame::select = {"frame_id", "group_id", "length"};

  const field frame::id = field::integerField(object::frame, "frame_id");
  const field frame::length = field::integerField(object::frame, "length");

  const field frame::words = field::joinField(object::frame, "group_id", object::word);

  field frame::parts()
  {
    return field::joinField(object::frame, "frame_id", object::part);
  }

  field frame::parts(int index)
  {
    return field::joinWhere(object::frame, "frame_id", object::part, "part_index", index);
  }

  frame::frame(const database& db, hatkirby::row row) : valid_(true)
  {
    id_ = std::get<int>(row[0]);
    groupId_ = std::get<int>(row[1]);
    length_ = std::get<int>(row[2]);

    parts_ = db.parts(*this, verbly::part::index, -1).all();
  }

};
