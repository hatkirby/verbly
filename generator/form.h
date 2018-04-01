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

      explicit form(std::string text);

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

      std::set<const pronunciation*> pronunciations_;

    };

    // Serializer

    hatkirby::database& operator<<(hatkirby::database& db, const form& arg);

  };
};

#endif /* end of include guard: FORM_H_7EFBC970 */
