#include "token.h"
#include <stdexcept>
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

    type tempType = first.type_;
    word tempWord;
    inflection tempCategory;
    std::string tempLiteral;
    part tempPart;
    std::set<std::string> tempFillin;
    std::list<token> tempUtterance;

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
    }
  }

  std::string token::compile() const
  {
    switch (type_)
    {
      case type::word: return word_.word_.getInflections(word_.category_).front();
      case type::literal: return literal_;
      case type::part: throw std::domain_error("Cannot compile incomplete token");
      case type::fillin: throw std::domain_error("Cannot compile incomplete token");

      case type::utterance:
      {
        std::list<std::string> compiled;
        for (const token& tkn : utterance_)
        {
          compiled.push_back(tkn.compile());
        }

        return implode(std::begin(compiled), std::end(compiled), " ");
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

  std::ostream& operator<<(std::ostream& os, token::type type)
  {
    switch (type)
    {
      case token::type::word: return os << "word";
      case token::type::literal: return os << "literal";
      case token::type::part: return os << "part";
      case token::type::fillin: return os << "fillin";
      case token::type::utterance: return os << "utterance";
    }
  }

};
