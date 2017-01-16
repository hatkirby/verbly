#ifndef SELRESTR_H_50652FB7
#define SELRESTR_H_50652FB7

#include <list>
#include <string>
#include <json.hpp>

namespace verbly {
  namespace generator {
    
    class selrestr {
    public:
      enum class type {
        empty,
        singleton,
        group
      };
      
      // Copy and move constructors
      
      selrestr(const selrestr& other);
      selrestr(selrestr&& other);
      
      // Assignment
      
      selrestr& operator=(selrestr other);
      
      // Swap
      
      friend void swap(selrestr& first, selrestr& second);
      
      // Destructor
      
      ~selrestr();
      
      // Generic accessors
      
      type getType() const
      {
        return type_;
      }
      
      // Empty
      
      selrestr();
      
      // Singleton
      
      selrestr(std::string restriction, bool pos);
      
      std::string getRestriction() const;
      
      bool getPos() const;
      
      // Group
      
      selrestr(std::list<selrestr> children, bool orlogic);
      
      std::list<selrestr> getChildren() const;
      
      std::list<selrestr>::const_iterator begin() const;
      
      std::list<selrestr>::const_iterator end() const;
      
      bool getOrlogic() const;
      
      // Helpers
      
      nlohmann::json toJson() const;
      
    private:
      union {
        struct {
          bool pos;
          std::string restriction;
        } singleton_;
        struct {
          std::list<selrestr> children;
          bool orlogic;
        } group_;
      };
      type type_;
    };
    
  };
};

#endif /* end of include guard: SELRESTR_H_50652FB7 */
