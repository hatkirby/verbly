#include "word.h"
#include <sqlite3.h>
#include "form.h"
#include "util.h"
#include "database.h"
#include "query.h"

namespace verbly {
  
  const object word::objectType = object::word;
  
  const std::list<std::string> word::select = {"word_id", "notion_id", "lemma_id", "tag_count", "position", "group_id"};
  
  const field word::id = field::integerField(object::word, "word_id");
  const field word::tagCount = field::integerField(object::word, "tag_count", true);
  const field word::adjectivePosition = field::integerField(object::word, "position", true);
  
  const field word::notion = field::joinField(object::word, "notion_id", object::notion);
  const field word::lemma = field::joinField(object::word, "lemma_id", object::lemma);
  const field word::group = field::joinField(object::word, "group_id", object::group, true);
  
  const field word::antonyms = field::selfJoin(object::word, "word_id", "antonymy", "antonym_2_id", "antonym_1_id");
  
  const field word::specifications = field::selfJoin(object::word, "word_id", "specification", "general_id", "specific_id");
  const field word::generalizations = field::selfJoin(object::word, "word_id", "specification", "specific_id", "general_id");
  
  const field word::pertainyms = field::selfJoin(object::word, "word_id", "pertainymy", "noun_id", "pertainym_id");
  const field word::antiPertainyms = field::selfJoin(object::word, "word_id", "pertainymy", "pertainym_id", "noun_id");
  
  const field word::mannernyms = field::selfJoin(object::word, "word_id", "mannernymy", "adjective_id", "mannernym_id");
  const field word::antiMannernyms = field::selfJoin(object::word, "word_id", "mannernymy", "mannernym_id", "adjective_id");
  
  const field word::usageTerms = field::selfJoin(object::word, "word_id", "usage", "domain_id", "term_id");
  const field word::usageDomains = field::selfJoin(object::word, "word_id", "usage", "term_id", "domain_id");
  
  const field word::topicalTerms = field::selfJoin(object::word, "word_id", "topicality", "domain_id", "term_id");
  const field word::topicalDomains = field::selfJoin(object::word, "word_id", "topicality", "term_id", "domain_id");
  
  const field word::regionalTerms = field::selfJoin(object::word, "word_id", "regionality", "domain_id", "term_id");
  const field word::regionalDomains = field::selfJoin(object::word, "word_id", "regionality", "term_id", "domain_id");
  
  word::word(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    notionId_ = sqlite3_column_int(row, 1);
    lemmaId_ = sqlite3_column_int(row, 2);
    
    if (sqlite3_column_type(row, 3) != SQLITE_NULL)
    {
      hasTagCount_ = true;
      tagCount_ = sqlite3_column_int(row, 3);
    }
    
    if (sqlite3_column_type(row, 4) != SQLITE_NULL)
    {
      adjectivePosition_ = static_cast<positioning>(sqlite3_column_int(row, 4));
    }
    
    if (sqlite3_column_type(row, 5) != SQLITE_NULL)
    {
      hasGroup_ = true;
      groupId_ = sqlite3_column_int(row, 5);
    }
  }
  
  const notion& word::getNotion() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized word");
    }
    
    if (!notion_)
    {
      notion_ = db_->notions(notion::id == notionId_).first();
    }
    
    return notion_;
  }
  
  const lemma& word::getLemma() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized word");
    }
    
    if (!lemma_)
    {
      lemma_ = db_->lemmas(lemma::id == lemmaId_).first();
    }
    
    return lemma_;
  }
  
  const group& word::getGroup() const
  {
    if (!valid_)
    {
      throw std::domain_error("Bad access to uninitialized word");
    }
    
    if (!hasGroup_)
    {
      throw std::domain_error("Word does not have a group");
    }
   
    if (!group_)
    {
      group_ = db_->groups(group::id == groupId_).first();
    }
    
    return group_;
  }
  
  std::string word::getBaseForm() const
  {
    return getLemma().getBaseForm().getText();
  }
  
  std::vector<std::string> word::getInflections(inflection category) const
  {
    std::vector<std::string> result;
    for (const form& infl : getLemma().getInflections(category))
    {
      result.push_back(infl.getText());
    }

    return result;
  }
  
};
