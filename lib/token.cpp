#include "verbly.h"

namespace verbly {
  
  token::token(token::type _type) : _type(_type)
  {
    
  }
  
  token::type token::token_type() const
  {
    return _type;
  }
  
  verb_token::verb_token(const class verb& _verb) : token(token::type::verb), _verb(&_verb)
  {
    
  }
  
  const class verb& verb_token::verb() const
  {
    return *_verb;
  }
  
  verb_token& verb_token::inflect(verb_token::inflection infl)
  {
    _inflection = infl;
    return *this;
  }
  
  bool verb_token::complete() const
  {
    return true;
  }
  
  std::string verb_token::compile() const
  {
    switch (_inflection)
    {
      case inflection::infinitive: return _verb->infinitive_form();
      case inflection::past_tense: return _verb->past_tense_form();
      case inflection::past_participle: return _verb->past_participle_form();
      case inflection::ing_form: return _verb->ing_form();
      case inflection::s_form: return _verb->s_form();
    }
  }
  
  token* verb_token::copy() const
  {
    return new verb_token(*this);
  }
  
};
