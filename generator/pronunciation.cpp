#include "pronunciation.h"
#include <list>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <hkutil/string.h>

namespace verbly {
  namespace generator {

    int pronunciation::nextId_ = 0;

    pronunciation::pronunciation(std::string phonemes, int anaphone_set_id) :
      id_(nextId_++),
      phonemes_(phonemes),
      anaphone_set_id_(anaphone_set_id)
    {
      auto phonemeList =
        hatkirby::split<std::list<std::string>>(phonemes, " ");

      std::list<std::string>::iterator rhymeStart =
        std::find_if(
          std::begin(phonemeList),
          std::end(phonemeList),
          [] (std::string phoneme) {
            return phoneme.find("1") != std::string::npos;
          });

      // Rhyme detection
      if (rhymeStart != std::end(phonemeList))
      {
        std::list<std::string> rhymePhonemes;

        std::transform(
          rhymeStart,
          std::end(phonemeList),
          std::back_inserter(rhymePhonemes),
          [] (std::string phoneme) {
            std::string naked;

            std::remove_copy_if(
              std::begin(phoneme),
              std::end(phoneme),
              std::back_inserter(naked),
              [] (char ch) {
                return std::isdigit(ch);
              });

            return naked;
          });

        rhyme_ =
          hatkirby::implode(
            std::begin(rhymePhonemes),
            std::end(rhymePhonemes),
            " ");

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

    hatkirby::database& operator<<(
      hatkirby::database& db,
      const pronunciation& arg)
    {
      std::list<hatkirby::column> fields;

      fields.emplace_back("pronunciation_id", arg.getId());
      fields.emplace_back("phonemes", arg.getPhonemes());
      fields.emplace_back("syllables", arg.getSyllables());
      fields.emplace_back("stress", arg.getStress());
      fields.emplace_back("anaphone_set_id", arg.getAnaphoneSetId());
      fields.emplace_back("reverse_pronunciation_id", arg.getReverseId());

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
