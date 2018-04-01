#include "notion.h"

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
      if (!hasWnid_ || (partOfSpeech_ != part_of_speech::noun))
      {
        throw std::domain_error("Notion is not a noun with wnid");
      }

      numOfImages_++;
    }

    void notion::setPrepositionGroups(std::list<std::string> groups)
    {
      if (partOfSpeech_ != part_of_speech::preposition)
      {
        throw std::domain_error("Notion is not a preposition");
      }

      prepositionGroups_ = groups;
    }

    hatkirby::database& operator<<(hatkirby::database& db, const notion& arg)
    {
      // First, serialize the notion
      {
        std::list<hatkirby::column> fields;

        fields.emplace_back("notion_id", arg.getId());
        fields.emplace_back("part_of_speech",
          static_cast<int>(arg.getPartOfSpeech()));

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
          db.insertIntoTable(
            "is_a",
            {
              { "notion_id", arg.getId() },
              { "groupname", group }
            });
        }
      }

      return db;
    }

  };
};
