#ifndef GROUP_H_EDAFB5DC
#define GROUP_H_EDAFB5DC

#include <map>
#include <set>
#include <string>
#include <cassert>
#include <list>
#include <hkutil/database.h>
#include "role.h"

namespace verbly {
  namespace generator {

    class frame;

    class group {
    public:

      // Constructor

      group();

      explicit group(const group& parent);

      // Mutators

      void addRole(role r);

      void addFrame(frame f);

      // Accessors

      int getId() const
      {
        return id_;
      }

      const std::set<std::string>& getRoles() const
      {
        return roleNames_;
      }

      bool hasRole(std::string name) const;

      const role& getRole(std::string name) const;

      const std::list<frame>& getFrames() const
      {
        return frames_;
      }

    private:

      static int nextId_;

      const int id_;

      std::map<std::string, role> roles_;
      std::list<frame> frames_;

      // Caches

      std::set<std::string> roleNames_;

    };

    // Serializer

    hatkirby::database& operator<<(hatkirby::database& db, const group& arg);

  };
};

#endif /* end of include guard: GROUP_H_EDAFB5DC */
