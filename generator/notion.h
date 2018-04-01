#ifndef NOTION_H_221DE2BC
#define NOTION_H_221DE2BC

#include <list>
#include <string>
#include <hkutil/database.h>
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
        if (!hasWnid_)
        {
          throw std::domain_error("Notion does not have wnid");
        }

        return wnid_;
      }

      int getNumOfImages() const
      {
        if (!hasWnid_ || (partOfSpeech_ != part_of_speech::noun))
        {
          throw std::domain_error("Notion is not a noun with wnid");
        }

        return numOfImages_;
      }

      std::list<std::string> getPrepositionGroups() const
      {
        if (partOfSpeech_ != part_of_speech::preposition)
        {
          throw std::domain_error("Notion is not a preposition");
        }

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

    hatkirby::database& operator<<(hatkirby::database& db, const notion& arg);

  };
};

#endif /* end of include guard: NOTION_H_221DE2BC */
