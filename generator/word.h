#ifndef WORD_H_91F99D46
#define WORD_H_91F99D46

#include <cassert>
#include <hkutil/database.h>
#include "../lib/enums.h"

namespace verbly {
  namespace generator {

    class notion;
    class lemma;
    class group;

    class word {
    public:

      // Constructors

      word(notion& n, lemma& l);

      word(notion& n, lemma& l, int tagCount);

      // Mutators

      void setAdjectivePosition(positioning adjectivePosition);

      void setVerbGroup(const group& verbGroup);

      // Accessors

      int getId() const
      {
        return id_;
      }

      notion& getNotion()
      {
        return notion_;
      }

      const notion& getNotion() const
      {
        return notion_;
      }

      lemma& getLemma()
      {
        return lemma_;
      }

      const lemma& getLemma() const
      {
        return lemma_;
      }

      bool hasTagCount() const
      {
        return hasTagCount_;
      }

      int getTagCount() const
      {
        // Calling code should always call hasTagCount first.
        assert(hasTagCount_);

        return tagCount_;
      }

      positioning getAdjectivePosition() const
      {
        return adjectivePosition_;
      }

      bool hasVerbGroup() const
      {
        return (verbGroup_ != nullptr);
      }

      const group& getVerbGroup() const
      {
        // Calling code should always call hasVerbGroup first.
        assert(verbGroup_ != nullptr);

        return *verbGroup_;
      }

    private:

      static int nextId_;

      const int id_;
      notion& notion_;
      lemma& lemma_;
      const int tagCount_ = 0;
      const bool hasTagCount_ = false;

      positioning adjectivePosition_ = positioning::undefined;
      const group* verbGroup_ = nullptr;

    };

    // Serializer

    hatkirby::database& operator<<(hatkirby::database& db, const word& arg);

  };
};

#endif /* end of include guard: WORD_H_91F99D46 */
