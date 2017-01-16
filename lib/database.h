#ifndef DATABASE_H_0B0A47D2
#define DATABASE_H_0B0A47D2

#include <string>
#include <exception>
#include <list>
#include "notion.h"
#include "word.h"
#include "group.h"
#include "frame.h"
#include "lemma.h"
#include "form.h"
#include "pronunciation.h"

struct sqlite3;

namespace verbly {
  
  template <typename Object>
  class query;

  class database {
  public:

    // Constructor

    explicit database(std::string path);

    // Disable copying

    database(const database& other) = delete;
    database& operator=(const database& other) = delete;

    // Move constructor and move assignment

    database(database&& other);
    database& operator=(database&& other);

    // Swap

    friend void swap(database& first, database& second);

    // Destructor

    ~database();
    
    // Queries
    
    query<notion> notions(filter where, bool random = true, int limit = 1) const;
    
    query<word> words(filter where, bool random = true, int limit = 1) const;
    
    query<group> groups(filter where, bool random = true, int limit = 1) const;
    
    query<frame> frames(filter where, bool random = true, int limit = 1) const;
    
    query<lemma> lemmas(filter where, bool random = true, int limit = 1) const;
    
    query<form> forms(filter where, bool random = true, int limit = 1) const;
    
    query<pronunciation> pronunciations(filter where, bool random = true, int limit = 1) const;

  private:

    database() = default;

    sqlite3* ppdb_ = nullptr;

  };

};

#endif /* end of include guard: DATABASE_H_0B0A47D2 */
