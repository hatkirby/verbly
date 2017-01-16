#include "notion.h"
#include <string>
#include <list>
#include "database.h"
#include "field.h"

namespace verbly {
  namespace generator {

    int notion::nextId_ = 0;

    notion::notion(
      part_of_speech partOfSpeech) :
        id_(nextId_++),
        partOfSpeech_(partOfSpeech)
    {
    }

    notion::notion(
      part_of_speech partOfSpeech,
      int wnid) :
        id_(nextId_++),
        partOfSpeech_(partOfSpeech),
        wnid_(wnid),
        hasWnid_(true)
    {
    }

    void notion::incrementNumOfImages()
    {
      // Calling code should always call hasWnid and check that the notion is a noun first.
      assert(hasWnid_ && (partOfSpeech_ == part_of_speech::noun));
      
      numOfImages_++;
    }
    
    void notion::setPrepositionGroups(std::list<std::string> groups)
    {
      // Calling code should always check that the notion is a preposition first.
      assert(partOfSpeech_ == part_of_speech::preposition);
      
      prepositionGroups_ = groups;
    }

    database& operator<<(database& db, const notion& arg)
    {
      // First, serialize the notion
      {
        std::list<field> fields;

        fields.emplace_back("notion_id", arg.getId());
        fields.emplace_back("part_of_speech", static_cast<int>(arg.getPartOfSpeech()));

        if (arg.hasWnid())
        {
          fields.emplace_back("wnid", arg.getWnid());
        
          if (arg.getPartOfSpeech() == part_of_speech::noun)
          {
            fields.emplace_back("images", arg.getNumOfImages());
          }
        }

        db.insertIntoTable("notions", std::move(fields));
      }
      
      // Next, serialize the is_a relationship if this is a preposition
      if (arg.getPartOfSpeech() == part_of_speech::preposition)
      {
        for (std::string group : arg.getPrepositionGroups())
        {
          std::list<field> fields;
          
          fields.emplace_back("notion_id", arg.getId());
          fields.emplace_back("groupname", group);
          
          db.insertIntoTable("is_a", std::move(fields));
        }
      }

      return db;
    }

  };
};
