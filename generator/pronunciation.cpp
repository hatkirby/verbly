#include "pronunciation.h"
#include <list>
#include <algorithm>
#include <cctype>
#include <iterator>
#include "database.h"
#include "field.h"
#include "../lib/util.h"

namespace verbly {
  namespace generator {

    int pronunciation::nextId_ = 0;

    pronunciation::pronunciation(std::string phonemes) :
      id_(nextId_++),
      phonemes_(phonemes)
    {
      auto phonemeList = split<std::list<std::string>>(phonemes, " ");

      auto rhymeStart = std::find_if(std::begin(phonemeList), std::end(phonemeList), [] (std::string phoneme) {
        return phoneme.find("1") != std::string::npos;
      });

      // Rhyme detection
      if (rhymeStart != std::end(phonemeList))
      {
        std::list<std::string> rhymePhonemes;

        std::transform(rhymeStart, std::end(phonemeList), std::back_inserter(rhymePhonemes), [] (std::string phoneme) {
          std::string naked;

          std::remove_copy_if(std::begin(phoneme), std::end(phoneme), std::back_inserter(naked), [] (char ch) {
            return std::isdigit(ch);
          });

          return naked;
        });

        rhyme_ = implode(std::begin(rhymePhonemes), std::end(rhymePhonemes), " ");

        if (rhymeStart != std::begin(phonemeList))
        {
          prerhyme_ = *std::prev(rhymeStart);
        }
      }

      // Syllable/stress
      for (std::string phoneme : phonemeList)
      {
        if (std::isdigit(phoneme.back()))
        {
          // It's a vowel!
          syllables_++;

          if (phoneme.back() == '1')
          {
            stress_.push_back('1');
          } else {
            stress_.push_back('0');
          }
        }
      }
    }

    database& operator<<(database& db, const pronunciation& arg)
    {
      std::list<field> fields;

      fields.emplace_back("pronunciation_id", arg.getId());
      fields.emplace_back("phonemes", arg.getPhonemes());
      fields.emplace_back("syllables", arg.getSyllables());
      fields.emplace_back("stress", arg.getStress());

      if (arg.hasRhyme())
      {
        fields.emplace_back("rhyme", arg.getRhymePhonemes());
        fields.emplace_back("prerhyme", arg.getPrerhyme());
      }

      db.insertIntoTable("pronunciations", std::move(fields));

      return db;
    }

  };
};
