#include "group.h"
#include <stdexcept>
#include <list>
#include <json.hpp>
#include "database.h"
#include "field.h"
#include "frame.h"

namespace verbly {
  namespace generator {
    
    int group::nextId_ = 0;
    
    group::group() : id_(nextId_++)
    {
    }
    
    void group::setParent(const group& parent)
    {
      // Adding a group to itself is nonsensical.
      assert(&parent != this);
      
      parent_ = &parent;
    }
    
    void group::addRole(role r)
    {
      std::string name = r.getName();
      roles_[name] = std::move(r);
      roleNames_.insert(std::move(name));
    }
    
    void group::addFrame(const frame& f)
    {
      frames_.insert(&f);
    }
    
    std::set<std::string> group::getRoles() const
    {
      std::set<std::string> fullRoles = roleNames_;
      
      if (hasParent())
      {
        for (std::string name : getParent().getRoles())
        {
          fullRoles.insert(name);
        }
      }
      
      return fullRoles;
    }
    
    const role& group::getRole(std::string name) const
    {
      if (roles_.count(name))
      {
        return roles_.at(name);
      } else if (hasParent())
      {
        return getParent().getRole(name);
      } else {
        throw std::invalid_argument("Specified role not found in verb group");
      }
    }
    
    std::set<const frame*> group::getFrames() const
    {
      std::set<const frame*> fullFrames = frames_;
      
      if (hasParent())
      {
        for (const frame* f : getParent().getFrames())
        {
          fullFrames.insert(f);
        }
      }
      
      return fullFrames;
    }
    
    database& operator<<(database& db, const group& arg)
    {
      // Serialize the group first
      {
        std::list<field> fields;
        fields.emplace_back("group_id", arg.getId());
        
        nlohmann::json jsonRoles;
        for (std::string name : arg.getRoles())
        {
          const role& r = arg.getRole(name);
          
          nlohmann::json jsonRole;
          jsonRole["type"] = name;
          jsonRole["selrestrs"] = r.getSelrestrs().toJson();
          
          jsonRoles.emplace_back(std::move(jsonRole));
        }

        fields.emplace_back("data", jsonRoles.dump());
        
        db.insertIntoTable("groups", std::move(fields));
      }
      
      // Then, serialize the group/frame relationship
      for (const frame* f : arg.getFrames())
      {
        std::list<field> fields;
        
        fields.emplace_back("group_id", arg.getId());
        fields.emplace_back("frame_id", f->getId());
        
        db.insertIntoTable("groups_frames", std::move(fields));
      }
      
      return db;
    }
    
  };
};
