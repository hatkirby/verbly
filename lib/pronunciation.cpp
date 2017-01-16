#include "pronunciation.h"
#include <sqlite3.h>
#include "form.h"
#include "lemma.h"
#include "word.h"
#include "util.h"

namespace verbly {
  
  const object pronunciation::objectType = object::pronunciation;
  
  const std::list<std::string> pronunciation::select = {"pronunciation_id", "phonemes", "syllables", "stress", "prerhyme", "rhyme"};
  
  const field pronunciation::id = field::integerField(object::pronunciation, "pronunciation_id");
  const field pronunciation::numOfSyllables = field::integerField(object::pronunciation, "syllables");
  const field pronunciation::stress = field::stringField(object::pronunciation, "stress");
  
  const field pronunciation::form = field::joinThrough(object::pronunciation, "pronunciation_id", object::form, "forms_pronunciations", "form_id");
  
  const field pronunciation::prerhyme = field::stringField(object::pronunciation, "prerhyme", true);
  const field pronunciation::rhyme = field::stringField(object::pronunciation, "rhyme", true);
  
  pronunciation::pronunciation(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);
    
    std::string phonemesStr(reinterpret_cast<const char*>(sqlite3_column_text(row, 1)));
    phonemes_ = split<std::vector<std::string>>(phonemesStr, " ");
    
    syllables_ = sqlite3_column_int(row, 2);
    stress_ = std::string(reinterpret_cast<const char*>(sqlite3_column_text(row, 3)));
    
    if (sqlite3_column_type(row, 5) != SQLITE_NULL)
    {
      hasRhyme_ = true;
      
      prerhyme_ = std::string(reinterpret_cast<const char*>(sqlite3_column_text(row, 4)));
      rhyme_ = std::string(reinterpret_cast<const char*>(sqlite3_column_text(row, 5)));
    }
  }
  
  filter pronunciation::rhymesWith(const pronunciation& arg)
  {
    return (prerhyme != arg.getPrerhyme()) && (rhyme == arg.getRhyme());
  }
  
  /*filter pronunciation::rhymesWith(const class form& arg)
  {
    filter result;
    
    for (const pronunciation& p : arg.getPronunciations())
    {
      result |= rhymesWith(p);
    }
    
    return result;
  }
  
  filter pronunciation::rhymesWith(const lemma& arg)
  {
    return rhymesWith(arg.getBaseForm());
  }
  
  filter pronunciation::rhymesWith(const word& arg)
  {
    return rhymesWith(arg.getLemma());
  }*/
  
};
