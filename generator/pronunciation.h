#ifndef PRONUNCIATION_H_584A08DD
#define PRONUNCIATION_H_584A08DD

#include <string>
#include <hkutil/database.h>
#include <stdexcept>

namespace verbly {
  namespace generator {

    class pronunciation {
    public:

      // Constructor

      pronunciation(std::string phonemes, int anaphone_set_id);

      // Accessors

      int getId() const
      {
        return id_;
      }

      std::string getPhonemes() const
      {
        return phonemes_;
      }

      bool hasRhyme() const
      {
        return !rhyme_.empty();
      }

      std::string getRhymePhonemes() const
      {
        if (rhyme_.empty())
        {
          throw std::domain_error("Pronunciation does not have a rhyme");
        }

        return rhyme_;
      }

      std::string getPrerhyme() const
      {
        if (rhyme_.empty())
        {
          throw std::domain_error("Pronunciation does not have a rhyme");
        }

        return prerhyme_;
      }

      int getSyllables() const
      {
        return syllables_;
      }

      std::string getStress() const
      {
        return stress_;
      }

      int getAnaphoneSetId() const
      {
        return anaphone_set_id_;
      }

      void setReverseId(int id)
      {
        reverse_id_ = id;
      }

      int getReverseId() const
      {
        return reverse_id_;
      }

    private:

      static int nextId_;

      const int id_;
      const std::string phonemes_;
      std::string rhyme_;
      std::string prerhyme_;
      int syllables_ = 0;
      std::string stress_;
      int anaphone_set_id_;
      int reverse_id_ = -1;

    };

    // Serializer

    hatkirby::database& operator<<(
      hatkirby::database& db,
      const pronunciation& arg);

  };
};

#endif /* end of include guard: PRONUNCIATION_H_584A08DD */
