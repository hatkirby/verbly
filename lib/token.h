#ifndef TOKEN_H_AD62C505
#define TOKEN_H_AD62C505

#include <ostream>
#include <string>
#include <list>
#include <set>
#include <variant.hpp>
#include <hkutil/recptr.h>
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
        utterance,
        transform
      };

      // Accessors

      type getType() const
      {
        return type_;
      }

      bool isComplete() const;

      std::string compile() const;

      bool isEmpty() const
      {
        return (type_ == type::utterance &&
          mpark::get<utterance_type>(variant_).empty());
      }

      // Word

      token(word arg, inflection category = inflection::base);

      const word& getWord() const;

      token inflect(inflection category) const;

      // Literal

      token(std::string arg);
      token(const char* arg);

      const std::string& getLiteral() const;

      // Part

      token(part arg);

      const part& getPart() const;

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
      token(std::initializer_list<token> pieces);

      iterator begin();
      const_iterator begin() const;

      iterator end();
      const_iterator end() const;

      token& operator<<(token arg);

      // Transform

      enum class casing {
        normal,
        capitalize,
        all_caps,
        title_case
      };

      static token separator(std::string param, token inner);
      static token punctuation(std::string param, token inner);
      static token indefiniteArticle(token inner);
      static token capitalize(casing param, token inner);
      static token quote(std::string open, std::string close, token inner);

      token& getInnerToken();
      const token& getInnerToken() const;

    private:

      std::string compileHelper(
        std::string separator,
        bool indefiniteArticle,
        casing capitalization) const;

      enum class transform_mode {
        separator,
        punctuation,
        indefinite_article,
        capitalize,
        quote
      };

      token(
        transform_mode type,
        std::string param1,
        std::string param2,
        token inner);

      token(
        transform_mode type,
        casing param,
        token inner);

      struct word_type {
        word value;
        inflection category;
      };

      using literal_type = std::string;

      using fillin_type = std::set<std::string>;

      using utterance_type = std::list<token>;

      struct transform_type {
        transform_mode type;
        std::string strParam;
        std::string strParam2;
        casing casingParam;
        hatkirby::recptr<token> inner;
      };

      using variant_type =
        mpark::variant<
          word_type,
          literal_type,
          part,
          fillin_type,
          utterance_type,
          transform_type>;

      type type_;
      variant_type variant_;
  };

  std::ostream& operator<<(std::ostream& os, token::type type);

};

#endif /* end of include guard: TOKEN_H_AD62C505 */
