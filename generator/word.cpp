#include "word.h"
#include <list>
#include <string>
#include "notion.h"
#include "lemma.h"
#include "group.h"

namespace verbly {
  namespace generator {

    int word::nextId_ = 0;

    word::word(
      notion& n,
      lemma& l) :
        id_(nextId_++),
        notion_(n),
        lemma_(l)
    {
    }

    word::word(
      notion& n,
      lemma& l,
      int tagCount) :
        id_(nextId_++),
        notion_(n),
        lemma_(l),
        tagCount_(tagCount),
        hasTagCount_(true)
    {
    }

    void word::setAdjectivePosition(positioning adjectivePosition)
    {
      adjectivePosition_ = adjectivePosition;
    }

    void word::setVerbGroup(const group& verbGroup)
    {
      verbGroup_ = &verbGroup;
    }

    hatkirby::database& operator<<(hatkirby::database& db, const word& arg)
    {
      std::list<hatkirby::column> fields;

      fields.emplace_back("word_id", arg.getId());
      fields.emplace_back("notion_id", arg.getNotion().getId());
      fields.emplace_back("lemma_id", arg.getLemma().getId());

      if (arg.hasTagCount())
      {
        fields.emplace_back("tag_count", arg.getTagCount());
      }

      if ((arg.getNotion().getPartOfSpeech() == part_of_speech::adjective)
        && (arg.getAdjectivePosition() != positioning::undefined))
      {
        fields.emplace_back("position",
          static_cast<int>(arg.getAdjectivePosition()));
      }

      if ((arg.getNotion().getPartOfSpeech() == part_of_speech::verb)
        && (arg.hasVerbGroup()))
      {
        fields.emplace_back("group_id", arg.getVerbGroup().getId());
      }

      db.insertIntoTable("words", std::move(fields));

      return db;
    }

  };
};
