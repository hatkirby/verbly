#include "notion.h"
#include <sqlite3.h>
#include <sstream>

namespace verbly {
  
  const object notion::objectType = object::notion;
  
  const std::list<std::string> notion::select = {"notion_id", "part_of_speech", "wnid", "images"};
  
  const field notion::id = field::integerField(object::notion, "notion_id");
  const field notion::partOfSpeech = field::integerField(object::notion, "part_of_speech");
  const field notion::wnid = field::integerField(object::notion, "wnid", true);
  const field notion::numOfImages = field::integerField(object::notion, "images", true);
  
  const field notion::word = field::joinField(object::notion, "word_id", object::word);

  const field notion::hypernyms = field::selfJoin(object::notion, "notion_id", "hypernymy", "hyponym_id", "hypernym_id");
  const field notion::hyponyms = field::selfJoin(object::notion, "notion_id", "hypernymy", "hypernym_id", "hyponym_id");

  const field notion::fullHypernyms = field::hierarchalSelfJoin(object::notion, "notion_id", "hypernymy", "hyponym_id", "hypernym_id");
  const field notion::fullHyponyms = field::hierarchalSelfJoin(object::notion, "notion_id", "hypernymy", "hypernym_id", "hyponym_id");

  const field notion::instances = field::selfJoin(object::notion, "notion_id", "instantiation", "class_id", "instance_id");
  const field notion::classes = field::selfJoin(object::notion, "notion_id", "instantiation", "instance_id", "class_id");

  const field notion::memberMeronyms = field::selfJoin(object::notion, "notion_id", "member_meronymy", "holonym_id", "meronym_id");
  const field notion::memberHolonyms = field::selfJoin(object::notion, "notion_id", "member_meronymy", "meronym_id", "holonym_id");

  const field notion::fullMemberMeronyms = field::hierarchalSelfJoin(object::notion, "notion_id", "member_meronymy", "holonym_id", "meronym_id");
  const field notion::fullMemberHolonyms = field::hierarchalSelfJoin(object::notion, "notion_id", "member_meronymy", "meronym_id", "holonym_id");

  const field notion::partMeronyms = field::selfJoin(object::notion, "notion_id", "part_meronymy", "holonym_id", "meronym_id");
  const field notion::partHolonyms = field::selfJoin(object::notion, "notion_id", "part_meronymy", "meronym_id", "holonym_id");

  const field notion::fullPartMeronyms = field::hierarchalSelfJoin(object::notion, "notion_id", "part_meronymy", "holonym_id", "meronym_id");
  const field notion::fullPartHolonyms = field::hierarchalSelfJoin(object::notion, "notion_id", "part_meronymy", "meronym_id", "holonym_id");

  const field notion::substanceMeronyms = field::selfJoin(object::notion, "notion_id", "substance_meronymy", "holonym_id", "meronym_id");
  const field notion::substanceHolonyms = field::selfJoin(object::notion, "notion_id", "substance_meronymy", "meronym_id", "holonym_id");

  const field notion::fullSubstanceMeronyms = field::hierarchalSelfJoin(object::notion, "notion_id", "substance_meronymy", "holonym_id", "meronym_id");
  const field notion::fullSubstanceHolonyms = field::hierarchalSelfJoin(object::notion, "notion_id", "substance_meronymy", "meronym_id", "holonym_id");

  const field notion::variants = field::selfJoin(object::notion, "notion_id", "variation", "noun_id", "adjective_id");
  const field notion::attributes = field::selfJoin(object::notion, "notion_id", "variation", "adjective_id", "noun_id");

  const field notion::similarAdjectives = field::selfJoin(object::notion, "notion_id", "similarity", "adjective_2_id", "adjective_1_id");

  const field notion::entails = field::selfJoin(object::notion, "notion_id", "entailment", "given_id", "entailment_id");
  const field notion::entailedBy = field::selfJoin(object::notion, "notion_id", "entailment", "entailment_id", "given_id");

  const field notion::causes = field::selfJoin(object::notion, "notion_id", "causality", "effect_id", "cause_id");
  const field notion::effects = field::selfJoin(object::notion, "notion_id", "causality", "cause_id", "effect_id");
  
  const notion::preposition_group_field prepositionGroup = {};
  
  const field notion::preposition_group_field::isA = field::joinField(object::notion, "notion_id", "is_a");
  const field notion::preposition_group_field::groupNameField = field::stringField("is_a", "groupname");
  
  notion::notion(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    partOfSpeech_ = static_cast<part_of_speech>(sqlite3_column_int(row, 1));
    
    if (sqlite3_column_type(row, 2) != SQLITE_NULL)
    {
      hasWnid_ = true;
      wnid_ = sqlite3_column_int(row, 2);
    }
    
    if (sqlite3_column_type(row, 3) != SQLITE_NULL)
    {
      hasNumOfImages_ = true;
      numOfImages_ = sqlite3_column_int(row, 3);
    }
  }
  
  std::string notion::getImageNetUrl() const
  {
    std::stringstream url;
    url << "http://www.image-net.org/api/text/imagenet.synset.geturls?wnid=n";
    url.width(8);
    url.fill('0');
    url << (getWnid() % 100000000);
    return url.str();
  }
  
  filter notion::preposition_group_field::operator==(std::string groupName) const
  {
    return (isA %= (groupNameField == groupName));
  }
  
};
