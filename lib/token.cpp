#include "verbly.h"

namespace verbly {
  
  token::type token::get_type() const
  {
    return _type;
  }
  
  int token::get_extra() const
  {
    return _extra;
  }
  
  void token::set_extra(int _arg)
  {
    _extra = _arg;
  }
  
  token::token(const token& other)
  {
    _type = other._type;
    _extra = other._extra;
    
    switch (_type)
    {
      case token::type::verb:
      {
        new(&_verb._verb) verb(other._verb._verb);
        _verb._infl = other._verb._infl;
        
        break;
      }
      
      case token::type::noun:
      {
        new(&_noun._noun) noun(other._noun._noun);
        _noun._infl = other._noun._infl;
        
        break;
      }
      
      case token::type::adjective:
      {
        new(&_adjective._adjective) adjective(other._adjective._adjective);
        _adjective._infl = other._adjective._infl;
        
        break;
      }
      
      case token::type::adverb:
      {
        new(&_adverb._adverb) adverb(other._adverb._adverb);
        _adverb._infl = other._adverb._infl;
        
        break;
      }
      
      case token::type::preposition:
      {
        new(&_preposition._preposition) preposition(other._preposition._preposition);
        
        break;
      }
      
      case token::type::fillin:
      {
        _fillin._type = other._fillin._type;
        
        break;
      }
      
      case token::type::string:
      {
        new(&_string._str) std::string(other._string._str);
        
        break;
      }
      
      case token::type::utterance:
      {
        new(&_utterance._utterance) std::list<token>(other._utterance._utterance);
        
        break;
      }
    }
  }
  
  token& token::operator=(const token& other)
  {
    this->~token();
    
    _type = other._type;
    _extra = other._extra;
    
    switch (_type)
    {
      case token::type::verb:
      {
        new(&_verb._verb) verb(other._verb._verb);
        _verb._infl = other._verb._infl;
        
        break;
      }
      
      case token::type::noun:
      {
        new(&_noun._noun) noun(other._noun._noun);
        _noun._infl = other._noun._infl;
        
        break;
      }
      
      case token::type::adjective:
      {
        new(&_adjective._adjective) adjective(other._adjective._adjective);
        _adjective._infl = other._adjective._infl;
        
        break;
      }
      
      case token::type::adverb:
      {
        new(&_adverb._adverb) adverb(other._adverb._adverb);
        _adverb._infl = other._adverb._infl;
        
        break;
      }
      
      case token::type::preposition:
      {
        new(&_preposition._preposition) preposition(other._preposition._preposition);
        
        break;
      }
      
      case token::type::fillin:
      {
        _fillin._type = other._fillin._type;
        
        break;
      }
      
      case token::type::string:
      {
        new(&_string._str) std::string(other._string._str);
        
        break;
      }
      
      case token::type::utterance:
      {
        new(&_utterance._utterance) std::list<token>(other._utterance._utterance);
        
        break;
      }
    }
    
    return *this;
  }
  
  token::~token()
  {
    switch (_type)
    {
      case token::type::verb:
      {
        _verb._verb.~verb();
        
        break;
      }
      
      case token::type::noun:
      {
        _noun._noun.~noun();
        
        break;
      }
      
      case token::type::adjective:
      {
        _adjective._adjective.~adjective();
        
        break;
      }
      
      case token::type::adverb:
      {
        _adverb._adverb.~adverb();
        
        break;
      }
      
      case token::type::preposition:
      {
        _preposition._preposition.~preposition();
        
        break;
      }
      
      case token::type::fillin:
      {
        // Nothing!
        
        break;
      }
      
      case token::type::string:
      {
        using string_type = std::string;
        _string._str.~string_type();
        
        break;
      }
      
      case token::type::utterance:
      {
        using list_type = std::list<token>;
        _utterance._utterance.~list_type();
        
        break;
      }
    }
  }
  
  bool token::is_complete() const
  {
    if (_type == token::type::utterance)
    {
      return std::all_of(std::begin(_utterance._utterance), std::end(_utterance._utterance), [] (const token& tkn) {
        return tkn.is_complete();
      });
    } else if (_type == token::type::fillin)
    {
      return false;
    } else {
      return true;
    }
  }
  
  std::string token::compile() const
  {
    switch (_type)
    {
      case token::type::verb:
      {
        switch (_verb._infl)
        {
          case token::verb_inflection::infinitive: return _verb._verb.infinitive_form();
          case token::verb_inflection::past_tense: return _verb._verb.past_tense_form();
          case token::verb_inflection::past_participle: return _verb._verb.past_participle_form();
          case token::verb_inflection::ing_form: return _verb._verb.ing_form();
          case token::verb_inflection::s_form: return _verb._verb.s_form();
        }
      }
      
      case token::type::noun:
      {
        switch (_noun._infl)
        {
          case token::noun_inflection::singular: return _noun._noun.singular_form();
          case token::noun_inflection::plural: return _noun._noun.plural_form();
        }
      }
      
      case token::type::adjective:
      {
        switch (_adjective._infl)
        {
          case token::adjective_inflection::base: return _adjective._adjective.base_form();
          case token::adjective_inflection::comparative: return _adjective._adjective.comparative_form();
          case token::adjective_inflection::superlative: return _adjective._adjective.superlative_form();
        }
      }
      
      case token::type::adverb:
      {
        switch (_adverb._infl)
        {
          case token::adverb_inflection::base: return _adverb._adverb.base_form();
          case token::adverb_inflection::comparative: return _adverb._adverb.comparative_form();
          case token::adverb_inflection::superlative: return _adverb._adverb.superlative_form();
        }
      }
      
      case token::type::preposition: return _preposition._preposition.get_form();
      case token::type::string: return _string._str;
      
      case token::type::fillin:
      {
        throw std::runtime_error("Cannot compile a fillin token.");
      }
      
      case token::type::utterance:
      {
        std::list<std::string> compiled;
        std::transform(std::begin(_utterance._utterance), std::end(_utterance._utterance), std::back_inserter(compiled), [] (token tkn) {
          return tkn.compile();
        });
        
        return verbly::implode(std::begin(compiled), std::end(compiled), " ");
      }
    }
  }
  
  token::token(verb _verb) : _type(type::verb)
  {
    new(&this->_verb._verb) verb(_verb);
    this->_verb._infl = verb_inflection::infinitive;
  }
  
  token::token(verb _verb, verb_inflection _infl) : token(_verb)
  {
    this->_verb._infl = _infl;
  }
  
  token& token::operator=(verb _verb)
  {
    *this = token{_verb};
    
    return *this;
  }
  
  verb token::get_verb() const
  {
    assert(_type == type::verb);
    
    return _verb._verb;
  }
  
  void token::set_verb(verb _verb)
  {
    assert(_type == type::verb);
    
    this->_verb._verb = _verb;
  }
  
  token::verb_inflection token::get_verb_inflection() const
  {
    assert(_type == type::verb);
    
    return _verb._infl;
  }
  
  void token::set_verb_inflection(verb_inflection _infl)
  {
    assert(_type == type::verb);
    
    _verb._infl = _infl;
  }
  
  token::token(noun _noun) : _type(type::noun)
  {
    new(&this->_noun._noun) noun(_noun);
    this->_noun._infl = noun_inflection::singular;
  }
  
  token::token(noun _noun, noun_inflection _infl) : token(_noun)
  {
    this->_noun._infl = _infl;
  }
  
  token& token::operator=(noun _noun)
  {
    *this = token{_noun};
    
    return *this;
  }
  
  noun token::get_noun() const
  {
    assert(_type == type::noun);
    
    return _noun._noun;
  }
  
  void token::set_noun(noun _noun)
  {
    assert(_type == type::noun);
    
    this->_noun._noun = _noun;
  }
  
  token::noun_inflection token::get_noun_inflection() const
  {
    assert(_type == type::noun);
    
    return _noun._infl;
  }
  
  void token::set_noun_inflection(noun_inflection _infl)
  {
    assert(_type == type::noun);
    
    _noun._infl = _infl;
  }
  
  token::token(adjective _adjective) : _type(type::adjective)
  {
    new(&this->_adjective._adjective) adjective(_adjective);
    this->_adjective._infl = adjective_inflection::base;
  }
  
  token::token(adjective _adjective, adjective_inflection _infl) : token(_adjective)
  {
    this->_adjective._infl = _infl;
  }
  
  token& token::operator=(adjective _adjective)
  {
    *this = token{_adjective};
    
    return *this;
  }
  
  adjective token::get_adjective() const
  {
    assert(_type == type::adjective);
    
    return _adjective._adjective;
  }
  
  void token::set_adjective(adjective _adjective)
  {
    assert(_type == type::adjective);
    
    this->_adjective._adjective = _adjective;
  }
  
  token::adjective_inflection token::get_adjective_inflection() const
  {
    assert(_type == type::adjective);
    
    return _adjective._infl;
  }
  
  void token::set_adjective_inflection(adjective_inflection _infl)
  {
    assert(_type == type::adjective);
    
    _adjective._infl = _infl;
  }
  
  token::token(adverb _adverb) : _type(type::adverb)
  {
    new(&this->_adverb._adverb) adverb(_adverb);
    this->_adverb._infl = adverb_inflection::base;
  }
  
  token::token(adverb _adverb, adverb_inflection _infl) : token(_adverb)
  {
    this->_adverb._infl = _infl;
  }
  
  token& token::operator=(adverb _adverb)
  {
    *this = token{_adverb};
    
    return *this;
  }
  
  adverb token::get_adverb() const
  {
    assert(_type == type::adverb);
    
    return _adverb._adverb;
  }
  
  void token::set_adverb(adverb _adverb)
  {
    assert(_type == type::adverb);
    
    this->_adverb._adverb = _adverb;
  }
  
  token::adverb_inflection token::get_adverb_inflection() const
  {
    assert(_type == type::adverb);
    
    return _adverb._infl;
  }
  
  void token::set_adverb_inflection(adverb_inflection _infl)
  {
    assert(_type == type::adverb);
    
    _adverb._infl = _infl;
  }
  
  token::token(preposition _preposition) : _type(type::preposition)
  {
    new(&this->_preposition._preposition) preposition(_preposition);
  }
  
  token& token::operator=(preposition _preposition)
  {
    *this = token{_preposition};
    
    return *this;
  }
  
  preposition token::get_preposition() const
  {
    assert(_type == type::preposition);
    
    return _preposition._preposition;
  }
  
  void token::set_preposition(preposition _preposition)
  {
    assert(_type == type::preposition);
    
    this->_preposition._preposition = _preposition;
  }
  
  token::token(fillin_type _ft) : _type(type::fillin)
  {
    _fillin._type = _ft;
  }
  
  token& token::operator=(fillin_type _ft)
  {
    *this = token{_ft};
    
    return *this;
  }
  
  token::fillin_type token::get_fillin_type() const
  {
    assert(_type == type::fillin);
    
    return _fillin._type;
  }
  
  void token::set_fillin_type(fillin_type _ft)
  {
    assert(_type == type::fillin);
    
    _fillin._type = _ft;
  }
  
  token::token() : _type(type::utterance)
  {
    new(&_utterance._utterance) std::list<token>();
  }
  
  token::token(std::initializer_list<token> _init) : _type(type::utterance)
  {
    new(&_utterance._utterance) std::list<token>(_init);
  }
  
  token::iterator token::begin()
  {
    assert(_type == type::utterance);
    
    return _utterance._utterance.begin();
  }
  
  token::iterator token::end()
  {
    assert(_type == type::utterance);
    
    return _utterance._utterance.end();
  }
  
  token& token::operator<<(token _tkn)
  {
    assert(_type == type::utterance);
    
    _utterance._utterance.push_back(_tkn);
    
    return *this;
  }
  
  void token::push_back(token _tkn)
  {
    assert(_type == type::utterance);
    
    _utterance._utterance.push_back(_tkn);
  }
  
  void token::insert(iterator before, token _tkn)
  {
    assert(_type == type::utterance);
    
    _utterance._utterance.insert(before, _tkn);
  }
  
  void token::replace(iterator torepl, token _tkn)
  {
    assert(_type == type::utterance);
    
    _utterance._utterance.insert(torepl, _tkn);
    _utterance._utterance.erase(torepl);
  }
  
  void token::erase(iterator toer)
  {
    assert(_type == type::utterance);
    
    _utterance._utterance.erase(toer);
  }
  
  token::token(std::string _str) : _type(type::string)
  {
    new(&_string._str) std::string(_str);
  }
  
  token& token::operator=(std::string _str)
  {
    *this = token{_str};
    
    return *this;
  }
  
  std::string token::get_string() const
  {
    assert(_type == type::string);
    
    return _string._str;
  }
  
  void token::set_string(std::string _str)
  {
    assert(_type == type::string);
    
    _string._str = _str;
  }
  
};
