#include "group.h"
#include <sqlite3.h>
#include "frame.h"
#include "database.h"
#include "query.h"

namespace verbly {
  
  const object group::objectType = object::group;
  
  const std::list<std::string> group::select = {"group_id", "data"};
  
  const field group::id = field::integerField(object::group, "group_id");
  
  const field group::frame = field::joinThrough(object::group, "group_id", object::frame, "groups_frames", "frame_id");
  const field group::word = field::joinField(object::group, "group_id", object::word);
  
  group::group(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    
    // TODO: Initialize role data from row.
  }
  
  const std::vector<frame>& group::getFrames() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized group");
    }
    
    if (!initializedFrames_)
    {
      frames_ = db_->frames(frame::group %= *this, false, -1).all();
      
      initializedFrames_ = true;
    }
    
    return frames_;
  }
  
};

