#ifndef LEMMA_H_D73105A7
#define LEMMA_H_D73105A7

#include <string>
#include <map>
#include <set>
#include "../lib/enums.h"

namespace verbly {
  namespace generator {

    class database;
    class form;

    class lemma {
    public:

      // Constructors

      explicit lemma(const form& baseForm);

      // Mutators

      void addInflection(inflection type, const form& f);

      // Accessors

      int getId() const
      {
        return id_;
      }

      const form& getBaseForm() const
      {
        return baseForm_;
      }

      std::set<const form*> getInflections(inflection type) const;

    private:

      static int nextId_;

      const int id_;
      const form& baseForm_;

      std::map<inflection, std::set<const form*>> inflections_;

    };

    // Serializer

    database& operator<<(database& db, const lemma& arg);

  };
};

#endif /* end of include guard: LEMMA_H_D73105A7 */
