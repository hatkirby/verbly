#ifndef WORD_H_DF91B1B4
#define WORD_H_DF91B1B4

#include <stdexcept>
#include <map>
#include "field.h"
#include "filter.h"
#include "notion.h"
#include "frame.h"
#include "form.h"

struct sqlite3_stmt;

namespace verbly {

  class database;

  class word {
  public:

    // Default constructor

    word() = default;

    // Construct from database

    word(const database& db, sqlite3_stmt* row);

    // Accessors

    bool isValid() const
    {
      return valid_;
    }

    int getId() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }

      return id_;
    }

    bool hasTagCount() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }

      return hasTagCount_;
    }

    int getTagCount() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }

      if (!hasTagCount_)
      {
        throw std::domain_error("Word has no tag count");
      }

      return tagCount_;
    }

    bool hasAdjectivePositioning() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }

      return (adjectivePosition_ != positioning::undefined);
    }

    positioning getAdjectivePosition() const
    {
      if (!valid_)
      {
        throw std::domain_error("Bad access to uninitialized word");
      }

      if (adjectivePosition_ == positioning::undefined)
      {
        throw std::domain_error("Word has no adjective position");
      }

      return adjectivePosition_;
    }

    const notion& getNotion() const;

    bool hasFrames() const;

    const std::vector<frame>& getFrames() const;

    const form& getBaseForm() const;

    bool hasInflection(inflection category) const;

    const std::vector<form>& getInflections(inflection category) const;



    // Type info

    static const object objectType;

    static const std::list<std::string> select;

    // Query fields

    static const field id;
    static const field tagCount;
    static const field adjectivePosition;

    operator filter() const
    {
      return (id == id_);
    }

    // Relationships with other objects

    static const field notions;
    static const field frames;

    static field forms(inflection category);

    // Relationships with self

    static const field antonyms;

    static const field specifications;
    static const field generalizations;

    static const field pertainyms;
    static const field antiPertainyms;

    static const field mannernyms;
    static const field antiMannernyms;

    static const field usageTerms;
    static const field usageDomains;

    static const field topicalTerms;
    static const field topicalDomains;

    static const field regionalTerms;
    static const field regionalDomains;

  private:

    void initializeForm(inflection category) const;
    void initializeFrames() const;

    bool valid_ = false;

    int id_;
    bool hasTagCount_ = false;
    int tagCount_;
    positioning adjectivePosition_ = positioning::undefined;
    int notionId_;
    int lemmaId_;
    bool hasGroup_ = false;
    int groupId_;

    const database* db_;

    mutable notion notion_;
    mutable bool initializedFrames_ = false;
    mutable std::vector<frame> frames_;
    mutable std::map<inflection, std::vector<form>> forms_;

  };

};

#endif /* end of include guard: WORD_H_DF91B1B4 */
