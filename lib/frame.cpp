#include "frame.h"
#include <sqlite3.h>

namespace verbly {
  
  const object frame::objectType = object::frame;
  
  const std::list<std::string> frame::select = {"frame_id", "data"};
  
  const field frame::id = field::integerField(object::frame, "frame_id");
  
  const field frame::group = field::joinThrough(object::frame, "frame_id", object::group, "groups_frames", "group_id");
  
  frame::frame(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    
    // TODO: Initialize frame data from row.
  }
  
};
