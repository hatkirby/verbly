#ifndef TOKEN_H_AD62C505
#define TOKEN_H_AD62C505

#include <ostream>
#include <string>
#include <list>
#include <set>
#include "enums.h"
#include "word.h"
#include "part.h"

namespace verbly {

  class token {
    public:
      enum class type {
        word,
        literal,
        part,
        fillin,
        utterance
      };

      // Copy & move constructors

      token(const token& other);
      token(token&& other);

      // Assignment operator

      token& operator=(token other);

      // Swap

      friend void swap(token& first, token& second);

      // Destructor

      ~token();

      // Accessors

      type getType() const
      {
        return type_;
      }

      bool isComplete() const;

      std::string compile() const;

      // Word

      token(word arg, inflection category = inflection::base);

      const word& getWord() const;

      token inflect(inflection category) const;

      // Literal

      token(std::string arg);
      token(const char* arg);

      std::string getLiteral() const;

      // Part

      token(part arg);

      part getPart() const;

      // Fillin

      token(std::set<std::string> synrestrs);

      const std::set<std::string>& getSynrestrs() const;

      bool hasSynrestr(std::string synrestr) const;

      void addSynrestr(std::string synrestr);

      // Utterance

      using iterator = std::list<token>::iterator;
      using const_iterator = std::list<token>::const_iterator;

      token();
      token(std::vector<part> parts);

      iterator begin();
      const_iterator begin() const;

      iterator end();
      const_iterator end() const;

      token& operator<<(token arg);

    private:
      union {
        struct {
          word word_;
          inflection category_;
        } word_;
        std::string literal_;
        part part_;
        std::set<std::string> fillin_;
        std::list<token> utterance_;
      };
      type type_;
  };

  std::ostream& operator<<(std::ostream& os, token::type type);

};

#endif /* end of include guard: TOKEN_H_AD62C505 */
