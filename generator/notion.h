#ifndef NOTION_H_221DE2BC
#define NOTION_H_221DE2BC

#include <cassert>
#include <list>
#include <string>
#include "../lib/enums.h"

namespace verbly {
  namespace generator {

    class notion {
    public:

      // Constructors

      explicit notion(part_of_speech partOfSpeech);

      notion(part_of_speech partOfSpeech, int wnid);

      // Mutators

      void incrementNumOfImages();

      void setPrepositionGroups(std::list<std::string> groups);

      // Accessors

      int getId() const
      {
        return id_;
      }

      part_of_speech getPartOfSpeech() const
      {
        return partOfSpeech_;
      }

      bool hasWnid() const
      {
        return hasWnid_;
      }

      int getWnid() const
      {
        // Calling code should always call hasWnid first.
        assert(hasWnid_);

        return wnid_;
      }

      int getNumOfImages() const
      {
        // Calling code should always call hasWnid and check that the notion is a noun first.
        assert(hasWnid_ && (partOfSpeech_ == part_of_speech::noun));

        return numOfImages_;
      }

      std::list<std::string> getPrepositionGroups() const
      {
        // Calling code should always check that the notion is a preposition first.
        assert(partOfSpeech_ == part_of_speech::preposition);

        return prepositionGroups_;
      }

    private:

      static int nextId_;

      const int id_;
      const part_of_speech partOfSpeech_;
      const int wnid_ = 0;
      const bool hasWnid_ = false;

      int numOfImages_ = 0;
      std::list<std::string> prepositionGroups_;

    };

    // Serializer

    database& operator<<(database& db, const notion& arg);

  };
};

#endif /* end of include guard: NOTION_H_221DE2BC */
