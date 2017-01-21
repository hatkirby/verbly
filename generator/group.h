#ifndef GROUP_H_EDAFB5DC
#define GROUP_H_EDAFB5DC

#include <map>
#include <set>
#include <string>
#include <cassert>
#include "../lib/role.h"

namespace verbly {
  namespace generator {
    
    class frame;
    class database;
    
    class group {
    public:
      
      // Constructor
      
      group();
      
      // Mutators
      
      void setParent(const group& parent);
      
      void addRole(role r);
      
      void addFrame(const frame& f);
      
      // Accessors
      
      int getId() const
      {
        return id_;
      }
      
      bool hasParent() const
      {
        return (parent_ != nullptr);
      }
      
      const group& getParent() const
      {
        // Calling code should always call hasParent first
        assert(parent_ != nullptr);
        
        return *parent_;
      }
      
      std::set<std::string> getRoles() const;
      
      const role& getRole(std::string name) const;
      
      std::set<const frame*> getFrames() const;
      
    private:
      
      static int nextId_;
      
      const int id_;
      
      const group* parent_ = nullptr;
      std::map<std::string, role> roles_;
      std::set<const frame*> frames_;
      
      // Caches
      
      std::set<std::string> roleNames_;
      
    };
    
    // Serializer
    
    database& operator<<(database& db, const group& arg);
    
  };
};

#endif /* end of include guard: GROUP_H_EDAFB5DC */
