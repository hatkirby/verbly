#include "verbly.h"

namespace verbly {
  
  adjective::adjective()
  {
    
  }
  
  adjective::adjective(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string adjective::base_form() const
  {
    assert(_valid == true);
    
    return _base_form;
  }
  
  std::string adjective::comparative_form() const
  {
    assert(_valid == true);
    
    return _comparative_form;
  }
  
  std::string adjective::superlative_form() const
  {
    assert(_valid == true);
    
    return _superlative_form;
  }
  
  adjective::positioning adjective::position() const
  {
    assert(_valid == true);
    
    return _position;
  }
  
  bool adjective::has_comparative_form() const
  {
    assert(_valid == true);
    
    return !_comparative_form.empty();
  }
  
  bool adjective::has_superlative_form() const
  {
    assert(_valid == true);
    
    return !_superlative_form.empty();
  }
  
  bool adjective::has_position() const
  {
    assert(_valid == true);
    
    return _position != adjective::positioning::undefined;
  }
  
  adjective_query adjective::antonyms() const
  {
    assert(_valid == true);
    
    return _data->adjectives().antonym_of(*this);
  }
  
  adjective_query adjective::synonyms() const
  {
    assert(_valid == true);
    
    return _data->adjectives().synonym_of(*this);
  }
  
  adjective_query adjective::generalizations() const
  {
    assert(_valid == true);
    
    return _data->adjectives().generalization_of(*this);
  }
  
  adjective_query adjective::specifications() const
  {
    assert(_valid == true);
    
    return _data->adjectives().specification_of(*this);
  }
  
  noun_query adjective::anti_pertainyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().anti_pertainym_of(*this);
  }
  
  adverb_query adjective::mannernyms() const
  {
    assert(_valid == true);
    
    return _data->adverbs().mannernym_of(*this);
  }
  
  noun_query adjective::attributes() const
  {
    assert(_valid == true);
    
    return _data->nouns().attribute_of(*this);
  }

};
