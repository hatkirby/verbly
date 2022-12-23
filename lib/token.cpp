#include "token.h"
#include <stdexcept>
#include <algorithm>
#include <hkutil/string.h>

namespace verbly {

  bool token::isComplete() const
  {
    switch (type_)
    {
      case type::word:
      case type::literal:
      {
        return true;
      }

      case type::part:
      case type::fillin:
      {
        return false;
      }

      case type::utterance:
      {
        const utterance_type& utterance = std::get<utterance_type>(variant_);

        return std::all_of(
          std::begin(utterance),
          std::end(utterance),
          [] (const token& tkn) {
            return tkn.isComplete();
          });
      }

      case type::transform:
      {
        const transform_type& transform = std::get<transform_type>(variant_);

        return transform.inner->isComplete();
      }
    }
  }

  std::string token::compile() const
  {
    return compileHelper(" ", false, casing::normal);
  }

  std::string token::compileHelper(
    std::string separator,
    bool indefiniteArticle,
    casing capitalization) const
  {
    switch (type_)
    {
      case type::word:
      {
        const word_type& w = std::get<word_type>(variant_);

        const form& wordForm = w.value.getInflections(w.category).front();

        std::string result = wordForm.getText();

        if (indefiniteArticle)
        {
          if (wordForm.startsWithVowelSound())
          {
            result = "an " + result;
          } else {
            result = "a " + result;
          }
        }

        if (capitalization == casing::capitalize)
        {
          if (std::isalpha(result[0]))
          {
            result[0] = std::toupper(result[0]);
          }
        } else if (capitalization == casing::title_case)
        {
          std::list<std::string> swords =
            hatkirby::split<std::list<std::string>>(result, " ");

          for (std::string& sword : swords)
          {
            if (std::isalpha(sword[0]))
            {
              sword[0] = std::toupper(sword[0]);
            }
          }

          result = hatkirby::implode(std::begin(swords), std::end(swords), " ");
        } else if (capitalization == casing::all_caps)
        {
          for (char& ch : result)
          {
            if (std::isalpha(ch))
            {
              ch = std::toupper(ch);
            }
          }
        }

        return result;
      }

      case type::literal:
      {
        std::string result = std::get<literal_type>(variant_);

        if (indefiniteArticle && std::isalpha(result[0]))
        {
          char canon = std::tolower(result[0]);
          if ((canon == 'a') || (canon == 'e') || (canon == 'i')
            || (canon == 'o') || (canon == 'u'))
          {
            result = "an " + result;
          } else {
            result = "a " + result;
          }
        }

        if (capitalization == casing::capitalize)
        {
          if (std::isalpha(result[0]))
          {
            result[0] = std::toupper(result[0]);
          }
        } else if (capitalization == casing::title_case)
        {
          std::list<std::string> swords =
            hatkirby::split<std::list<std::string>>(result, " ");

          for (std::string& sword : swords)
          {
            if (std::isalpha(sword[0]))
            {
              sword[0] = std::toupper(sword[0]);
            }
          }

          result = hatkirby::implode(std::begin(swords), std::end(swords), " ");
        } else if (capitalization == casing::all_caps)
        {
          for (char& ch : result)
          {
            if (std::isalpha(ch))
            {
              ch = std::toupper(ch);
            }
          }
        }

        return result;
      }

      case type::part:
      case type::fillin:
      {
        throw std::domain_error("Cannot compile incomplete token");
      }

      case type::utterance:
      {
        const utterance_type& utterance = std::get<utterance_type>(variant_);

        bool first = true;
        std::list<std::string> compiled;
        for (const token& tkn : utterance)
        {
          casing propagateCasing = capitalization;
          if ((capitalization == casing::capitalize) && (!first))
          {
            propagateCasing = casing::normal;
          }

          compiled.push_back(
            tkn.compileHelper(" ",
              first && indefiniteArticle,
              propagateCasing));

          first = false;
        }

        return hatkirby::implode(
          std::begin(compiled),
          std::end(compiled),
          separator);
      }

      case type::transform:
      {
        const transform_type& transform = std::get<transform_type>(variant_);

        switch (transform.type)
        {
          case transform_mode::separator:
          {
            return transform.inner->compileHelper(
              transform.strParam,
              indefiniteArticle,
              capitalization);
          }

          case transform_mode::punctuation:
          {
            return transform.inner->compileHelper(
              separator,
              indefiniteArticle,
              capitalization) + transform.strParam;
          }

          case transform_mode::indefinite_article:
          {
            return transform.inner->compileHelper(
              separator,
              true,
              capitalization);
          }

          case transform_mode::capitalize:
          {
            return transform.inner->compileHelper(
              separator,
              indefiniteArticle,
              transform.casingParam);
          }

          case transform_mode::quote:
          {
            return transform.strParam +
              transform.inner->compileHelper(
                separator,
                indefiniteArticle,
                capitalization) +
              transform.strParam2;
          }
        }
      }
    }
  }

  token::token(
    word arg,
    inflection category) :
      type_(type::word),
      variant_(word_type { std::move(arg), category })
  {
  }

  const word& token::getWord() const
  {
    if (type_ != type::word)
    {
      throw std::domain_error("Token is not a word");
    }

    return std::get<word_type>(variant_).value;
  }

  token token::inflect(inflection category) const
  {
    if (type_ != type::word)
    {
      throw std::domain_error("Token is not a word");
    }

    return {
      std::get<word_type>(variant_).value,
      category
    };
  }

  token::token(
    std::string arg) :
      type_(type::literal),
      variant_(std::move(arg))
  {
  }

  token::token(
    const char* arg) :
      token(std::string(arg))
  {
  }

  const std::string& token::getLiteral() const
  {
    if (type_ != type::literal)
    {
      throw std::domain_error("Token is not a literal");
    }

    return std::get<literal_type>(variant_);
  }

  token::token(
    part arg) :
      type_(type::part),
      variant_(std::move(arg))
  {
  }

 const part& token::getPart() const
  {
    if (type_ != type::part)
    {
      throw std::domain_error("Token is not a part");
    }

    return std::get<part>(variant_);
  }

  token::token(
    std::set<std::string> synrestrs) :
      type_(type::fillin),
      variant_(std::move(synrestrs))
  {
  }

  const std::set<std::string>& token::getSynrestrs() const
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    return std::get<fillin_type>(variant_);
  }

  bool token::hasSynrestr(std::string synrestr) const
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    return std::get<fillin_type>(variant_).count(synrestr);
  }

  void token::addSynrestr(std::string synrestr)
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    fillin_type& fillin = std::get<fillin_type>(variant_);
    fillin.insert(std::move(synrestr));
  }

  token::token() :
    type_(type::utterance),
    variant_(utterance_type {})
  {
  }

  token::token(
    std::vector<part> parts) :
      type_(type::utterance),
      variant_(utterance_type { std::begin(parts), std::end(parts) })
  {
  }

  token::token(
    std::initializer_list<token> parts) :
      type_(type::utterance),
      variant_(utterance_type { std::begin(parts), std::end(parts) })
  {
  }

  token::iterator token::begin()
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::begin(std::get<utterance_type>(variant_));
  }

  token::const_iterator token::begin() const
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::begin(std::get<utterance_type>(variant_));
  }

  token::iterator token::end()
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::end(std::get<utterance_type>(variant_));
  }

  token::const_iterator token::end() const
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::end(std::get<utterance_type>(variant_));
  }

  token& token::operator<<(token arg)
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    utterance_type& utterance = std::get<utterance_type>(variant_);
    utterance.push_back(std::move(arg));

    return *this;
  }

  token token::separator(std::string param, token inner)
  {
    return token(transform_mode::separator, std::move(param), "", std::move(inner));
  }

  token token::punctuation(std::string param, token inner)
  {
    return token(transform_mode::punctuation, std::move(param), "", std::move(inner));
  }

  token token::indefiniteArticle(token inner)
  {
    return token(transform_mode::indefinite_article, "", "", std::move(inner));
  }

  token token::capitalize(casing param, token inner)
  {
    return token(transform_mode::capitalize, param, std::move(inner));
  }

  token token::quote(std::string opening, std::string closing, token inner)
  {
    return token(
      transform_mode::quote,
      std::move(opening),
      std::move(closing),
      std::move(inner));
  }

  token& token::getInnerToken()
  {
    if (type_ != type::transform)
    {
      throw std::domain_error("Invalid access on non-tranform token");
    }

    return *std::get<transform_type>(variant_).inner;
  }

  const token& token::getInnerToken() const
  {
    if (type_ != type::transform)
    {
      throw std::domain_error("Invalid access on non-tranform token");
    }

    return *std::get<transform_type>(variant_).inner;
  }

  token::token(
    transform_mode type,
    std::string param1,
    std::string param2,
    token inner) :
      type_(type::transform),
      variant_(transform_type {
        type,
        std::move(param1),
        std::move(param2),
        casing::normal,
        new token(std::move(inner))
      })
  {
  }

  token::token(
    transform_mode type,
    casing param,
    token inner) :
      type_(type::transform),
      variant_(transform_type {
        type,
        {},
        {},
        param,
        new token(std::move(inner))
      })
  {
  }

  std::ostream& operator<<(std::ostream& os, token::type type)
  {
    switch (type)
    {
      case token::type::word: return os << "word";
      case token::type::literal: return os << "literal";
      case token::type::part: return os << "part";
      case token::type::fillin: return os << "fillin";
      case token::type::utterance: return os << "utterance";
      case token::type::transform: return os << "transform";
    }
  }

};
