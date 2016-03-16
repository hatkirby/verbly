#ifndef DATA_H_C4AEC3DD
#define DATA_H_C4AEC3DD

#include <sqlite3.h>
#include <stdexcept>

namespace verbly {
  
  class data;
  class word;
  class adjective;
  class noun;
  class verb;
  class adverb;
  class adjective_query;
  class adverb_query;
  class noun_query;
  class verb_query;
  
  class data {
    private:
      sqlite3* ppdb;
      
      friend class adjective_query;
      friend class noun_query;
      friend class verb_query;
      friend class adverb_query;
      
    public:
      data(std::string datafile);
      
      data(const data& other) = delete;
      data& operator=(const data& other) = delete;
      
      data(data&& other);
      data& operator=(data&& other);
      
      ~data();
      
      verb_query verbs() const;
      adjective_query adjectives() const;
      adverb_query adverbs() const;
      noun_query nouns() const;
      
  };
  
};

#endif /* end of include guard: DATA_H_C4AEC3DD */
