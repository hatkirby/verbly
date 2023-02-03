#ifndef FORM_H_7EFBC970
#define FORM_H_7EFBC970

#include <string>
#include <set>
#include <hkutil/database.h>

namespace verbly {
  namespace generator {

    class pronunciation;

    class form {
    public:

      // Constructor

      form(std::string text, int anagram_set_id);

      // Mutators

      void addPronunciation(const pronunciation& p);

      // Accessors

      int getId() const
      {
        return id_;
      }

      std::string getText() const
      {
        return text_;
      }

      int getComplexity() const
      {
        return complexity_;
      }

      bool isProper() const
      {
        return proper_;
      }

      int getLength() const
      {
        return length_;
      }

      int getAnagramSetId() const
      {
        return anagram_set_id_;
      }

      void setReverseId(int id)
      {
        reverse_id_ = id;
      }

      int getReverseId() const
      {
        return reverse_id_;
      }

      void setFrequency(int freq)
      {
        frequency_ = freq;
      }

      int getFrequency() const
      {
        return frequency_;
      }

      std::set<const pronunciation*> getPronunciations() const
      {
        return pronunciations_;
      }

    private:

      static int nextId_;

      const int id_;
      const std::string text_;
      const int complexity_;
      const bool proper_;
      const int length_;
      const int anagram_set_id_;
      int reverse_id_ = -1;
      int frequency_ = 0;

      std::set<const pronunciation*> pronunciations_;

    };

    // Serializer

    hatkirby::database& operator<<(hatkirby::database& db, const form& arg);

  };
};

#endif /* end of include guard: FORM_H_7EFBC970 */
