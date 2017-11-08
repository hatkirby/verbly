#include "token.h"
#include <stdexcept>
#include <algorithm>
#include "util.h"

namespace verbly {

  token::token(const token& other)
  {
    type_ = other.type_;

    switch (type_)
    {
      case type::word:
      {
        new(&word_.word_) word(other.word_.word_);
        word_.category_ = other.word_.category_;

        break;
      }

      case type::literal:
      {
        new(&literal_) std::string(other.literal_);

        break;
      }

      case type::part:
      {
        new(&part_) part(other.part_);

        break;
      }

      case type::fillin:
      {
        new(&fillin_) std::set<std::string>(other.fillin_);

        break;
      }

      case type::utterance:
      {
        new(&utterance_) std::list<token>(other.utterance_);

        break;
      }

      case type::transform:
      {
        transform_.type_ = other.transform_.type_;
        new(&transform_.strParam_) std::string(other.transform_.strParam_);
        new(&transform_.strParam2_) std::string(other.transform_.strParam2_);
        transform_.casingParam_ = other.transform_.casingParam_;
        new(&transform_.inner_) std::unique_ptr<token>(new token(*other.transform_.inner_));

        break;
      }
    }
  }

  token::token(token&& other) : token()
  {
    swap(*this, other);
  }

  token& token::operator=(token other)
  {
    swap(*this, other);

    return *this;
  }

  void swap(token& first, token& second)
  {
    using type = token::type;
    using transform_type = token::transform_type;
    using casing = token::casing;

    type tempType = first.type_;
    word tempWord;
    inflection tempCategory;
    std::string tempLiteral;
    part tempPart;
    std::set<std::string> tempFillin;
    std::list<token> tempUtterance;
    transform_type tempTransformType;
    std::string tempTransformStrParam;
    std::string tempTransformStrParam2;
    casing tempTransformCasingParam;
    std::unique_ptr<token> tempTransformInner;

    switch (tempType)
    {
      case type::word:
      {
        tempWord = std::move(first.word_.word_);
        tempCategory = first.word_.category_;

        break;
      }

      case type::literal:
      {
        tempLiteral = std::move(first.literal_);

        break;
      }

      case type::part:
      {
        tempPart = std::move(first.part_);

        break;
      }

      case type::fillin:
      {
        tempFillin = std::move(first.fillin_);

        break;
      }

      case type::utterance:
      {
        tempUtterance = std::move(first.utterance_);

        break;
      }

      case type::transform:
      {
        tempTransformType = first.transform_.type_;
        tempTransformStrParam = std::move(first.transform_.strParam_);
        tempTransformStrParam2 = std::move(first.transform_.strParam2_);
        tempTransformCasingParam = first.transform_.casingParam_;
        tempTransformInner = std::move(first.transform_.inner_);

        break;
      }
    }

    first.~token();

    first.type_ = second.type_;

    switch (first.type_)
    {
      case type::word:
      {
        new(&first.word_.word_) word(std::move(second.word_.word_));
        first.word_.category_ = second.word_.category_;

        break;
      }

      case type::literal:
      {
        new(&first.literal_) std::string(std::move(second.literal_));

        break;
      }

      case type::part:
      {
        new(&first.part_) part(std::move(second.part_));

        break;
      }

      case type::fillin:
      {
        new(&first.fillin_) std::set<std::string>(std::move(second.fillin_));

        break;
      }

      case type::utterance:
      {
        new(&first.utterance_) std::list<token>(std::move(second.utterance_));

        break;
      }

      case type::transform:
      {
        first.transform_.type_ = second.transform_.type_;
        new(&first.transform_.strParam_) std::string(std::move(second.transform_.strParam_));
        new(&first.transform_.strParam2_) std::string(std::move(second.transform_.strParam2_));
        first.transform_.casingParam_ = second.transform_.casingParam_;
        new(&first.transform_.inner_) std::unique_ptr<token>(std::move(second.transform_.inner_));

        break;
      }
    }

    second.~token();

    second.type_ = tempType;

    switch (second.type_)
    {
      case type::word:
      {
        new(&second.word_.word_) word(std::move(tempWord));
        second.word_.category_ = tempCategory;

        break;
      }

      case type::literal:
      {
        new(&second.literal_) std::string(std::move(tempLiteral));

        break;
      }

      case type::part:
      {
        new(&second.part_) part(std::move(tempPart));

        break;
      }

      case type::fillin:
      {
        new(&second.fillin_) std::set<std::string>(std::move(tempFillin));

        break;
      }

      case type::utterance:
      {
        new(&second.utterance_) std::list<token>(std::move(tempUtterance));

        break;
      }

      case type::transform:
      {
        second.transform_.type_ = tempTransformType;
        new(&second.transform_.strParam_) std::string(std::move(tempTransformStrParam));
        new(&second.transform_.strParam2_) std::string(std::move(tempTransformStrParam2));
        second.transform_.casingParam_ = tempTransformCasingParam;
        new(&second.transform_.inner_) std::unique_ptr<token>(std::move(tempTransformInner));

        break;
      }
    }
  }

  token::~token()
  {
    switch (type_)
    {
      case type::word:
      {
        word_.word_.~word();

        break;
      }

      case type::literal:
      {
        using string_type = std::string;
        literal_.~string_type();

        break;
      }

      case type::part:
      {
        part_.~part();

        break;
      }

      case type::fillin:
      {
        using set_type = std::set<std::string>;
        fillin_.~set_type();

        break;
      }

      case type::utterance:
      {
        using list_type = std::list<token>;
        utterance_.~list_type();

        break;
      }

      case type::transform:
      {
        using string_type = std::string;
        using ptr_type = std::unique_ptr<token>;

        transform_.strParam_.~string_type();
        transform_.strParam2_.~string_type();
        transform_.inner_.~ptr_type();

        break;
      }
    }
  }

  bool token::isComplete() const
  {
    switch (type_)
    {
      case type::word: return true;
      case type::literal: return true;
      case type::part: return false;
      case type::fillin: return false;
      case type::utterance: return std::all_of(std::begin(utterance_), std::end(utterance_), [] (const token& tkn) {
        return tkn.isComplete();
      });
      case type::transform: return transform_.inner_->isComplete();
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
        const form& wordForm = word_.word_.getInflections(word_.category_)
          .front();

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
            split<std::list<std::string>>(result, " ");

          for (std::string& sword : swords)
          {
            if (std::isalpha(sword[0]))
            {
              sword[0] = std::toupper(sword[0]);
            }
          }

          result = implode(std::begin(swords), std::end(swords), " ");
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
        std::string result = literal_;

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
            split<std::list<std::string>>(result, " ");

          for (std::string& sword : swords)
          {
            if (std::isalpha(sword[0]))
            {
              sword[0] = std::toupper(sword[0]);
            }
          }

          result = implode(std::begin(swords), std::end(swords), " ");
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

      case type::part: throw std::domain_error("Cannot compile incomplete token");
      case type::fillin: throw std::domain_error("Cannot compile incomplete token");

      case type::utterance:
      {
        bool first = true;
        std::list<std::string> compiled;
        for (const token& tkn : utterance_)
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

        return implode(std::begin(compiled), std::end(compiled), separator);
      }

      case type::transform:
      {
        switch (transform_.type_)
        {
          case transform_type::separator:
          {
            return transform_.inner_->compileHelper(
              transform_.strParam_, indefiniteArticle, capitalization);
          }

          case transform_type::punctuation:
          {
            return transform_.inner_->compileHelper(
              separator, indefiniteArticle, capitalization)
              + transform_.strParam_;
          }

          case transform_type::indefinite_article:
          {
            return transform_.inner_->compileHelper(
              separator, true, capitalization);
          }

          case transform_type::capitalize:
          {
            return transform_.inner_->compileHelper(
              separator,
              indefiniteArticle,
              transform_.casingParam_);
          }

          case transform_type::quote:
          {
            return transform_.strParam_ +
              transform_.inner_->compileHelper(
                separator,
                indefiniteArticle,
                capitalization) +
              transform_.strParam2_;
          }
        }
      }
    }
  }

  token::token(word arg, inflection category) : type_(type::word)
  {
    new(&word_.word_) word(std::move(arg));
    word_.category_ = category;
  }

  const word& token::getWord() const
  {
    if (type_ != type::word)
    {
      throw std::domain_error("Token is not a word");
    }

    return word_.word_;
  }

  token token::inflect(inflection category) const
  {
    if (type_ != type::word)
    {
      throw std::domain_error("Token is not a word");
    }

    return token(word_.word_, category);
  }

  token::token(std::string arg) : type_(type::literal)
  {
    new(&literal_) std::string(std::move(arg));
  }

  token::token(const char* arg) : token(std::string(arg))
  {
  }

  std::string token::getLiteral() const
  {
    if (type_ != type::literal)
    {
      throw std::domain_error("Token is not a literal");
    }

    return literal_;
  }

  token::token(part arg) : type_(type::part)
  {
    new(&part_) part(std::move(arg));
  }

  part token::getPart() const
  {
    if (type_ != type::part)
    {
      throw std::domain_error("Token is not a part");
    }

    return part_;
  }

  token::token(std::set<std::string> synrestrs) : type_(type::fillin)
  {
    new(&fillin_) std::set<std::string>(std::move(synrestrs));
  }

  const std::set<std::string>& token::getSynrestrs() const
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    return fillin_;
  }

  bool token::hasSynrestr(std::string synrestr) const
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    return (fillin_.count(synrestr) == 1);
  }

  void token::addSynrestr(std::string synrestr)
  {
    if (type_ != type::fillin)
    {
      throw std::domain_error("Token is not a fillin");
    }

    fillin_.insert(std::move(synrestr));
  }

  token::token() : type_(type::utterance)
  {
    new(&utterance_) std::list<token>();
  }

  token::token(std::vector<part> parts) : type_(type::utterance)
  {
    new(&utterance_) std::list<token>(std::begin(parts), std::end(parts));
  }

  token::token(std::initializer_list<token> parts) : type_(type::utterance)
  {
    new(&utterance_) std::list<token>(std::move(parts));
  }

  token::iterator token::begin()
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::begin(utterance_);
  }

  token::const_iterator token::begin() const
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::begin(utterance_);
  }

  token::iterator token::end()
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::end(utterance_);
  }

  token::const_iterator token::end() const
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    return std::end(utterance_);
  }

  token& token::operator<<(token arg)
  {
    if (type_ != type::utterance)
    {
      throw std::domain_error("Token is not an utterance");
    }

    utterance_.push_back(std::move(arg));

    return *this;
  }

  token token::separator(std::string param, token inner)
  {
    return token(transform_type::separator, std::move(param), "", std::move(inner));
  }

  token token::punctuation(std::string param, token inner)
  {
    return token(transform_type::punctuation, std::move(param), "", std::move(inner));
  }

  token token::indefiniteArticle(token inner)
  {
    return token(transform_type::indefinite_article, "", "", std::move(inner));
  }

  token token::capitalize(casing param, token inner)
  {
    return token(transform_type::capitalize, param, std::move(inner));
  }

  token token::quote(std::string opening, std::string closing, token inner)
  {
    return token(
      transform_type::quote,
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

    return *transform_.inner_;
  }

  const token& token::getInnerToken() const
  {
    if (type_ != type::transform)
    {
      throw std::domain_error("Invalid access on non-tranform token");
    }

    return *transform_.inner_;
  }

  token::token(
    transform_type type,
    std::string param1,
    std::string param2,
    token inner) :
      type_(type::transform)
  {
    transform_.type_ = type;
    new(&transform_.strParam_) std::string(std::move(param1));
    new(&transform_.strParam2_) std::string(std::move(param2));
    new(&transform_.inner_) std::unique_ptr<token>(new token(std::move(inner)));
  }

  token::token(
    transform_type type,
    casing param,
    token inner) :
      type_(type::transform)
  {
    transform_.type_ = type;
    new(&transform_.strParam_) std::string();
    new(&transform_.strParam2_) std::string();
    transform_.casingParam_ = param;
    new(&transform_.inner_) std::unique_ptr<token>(new token(std::move(inner)));
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
