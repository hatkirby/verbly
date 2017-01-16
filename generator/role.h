#ifndef ROLE_H_249F9A9C
#define ROLE_H_249F9A9C

#include "selrestr.h"

namespace verbly {
  namespace generator {
    
    class role {
    public:
      
      // Mutators
      
      void setSelrestrs(selrestr selrestrs)
      {
        selrestrs_ = selrestrs;
      }
      
      // Accessors
      
      const selrestr& getSelrestrs() const
      {
        return selrestrs_;
      }
      
    private:
      
      selrestr selrestrs_;
      
    };
    
  };
};

#endif /* end of include guard: ROLE_H_249F9A9C */
