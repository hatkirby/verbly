#include "group.h"
#include <sqlite3.h>
#include <json.hpp>
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

    std::string rolesJsonStr(reinterpret_cast<const char*>(sqlite3_column_blob(row, 1)));
    nlohmann::json rolesJson = nlohmann::json::parse(std::move(rolesJsonStr));
    for (const nlohmann::json& roleJson : rolesJson)
    {
      std::string roleName = roleJson["type"];
      selrestr roleSelrestr;

      if (roleJson.find("selrestrs") != roleJson.end())
      {
        roleSelrestr = selrestr(roleJson["selrestrs"]);
      }

      roles_[roleName] = role(roleName, std::move(roleSelrestr));
    }
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

  const role& group::getRole(std::string roleName) const
  {
    return roles_.at(roleName);
  }

};

