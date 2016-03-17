#include "verbly.h"

namespace verbly {

  data::data(std::string datafile)
  {
    if (sqlite3_open_v2(datafile.c_str(), &ppdb, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
    {
      throw std::invalid_argument(sqlite3_errmsg(ppdb));
    }
  }

  data::data(data&& other)
  {
    ppdb = other.ppdb;
  }

  data& data::operator=(data&& other)
  {
    ppdb = other.ppdb;
  
    return *this;
  }

  data::~data()
  {
    sqlite3_close_v2(ppdb);
  }

  verb_query data::verbs() const
  {
    return verb_query(*this);
  }

  adjective_query data::adjectives() const
  {
    return adjective_query(*this);
  }

  adverb_query data::adverbs() const
  {
    return adverb_query(*this);
  }

  noun_query data::nouns() const
  {
    return noun_query(*this);
  }

};
