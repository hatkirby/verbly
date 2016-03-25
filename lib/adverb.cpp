#include "verbly.h"

namespace verbly {
  
  adverb::adverb()
  {
    
  }
  
  adverb::adverb(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string adverb::base_form() const
  {
    assert(_valid == true);
    
    return _base_form;
  }
  
  std::string adverb::comparative_form() const
  {
    assert(_valid == true);
    
    return _comparative_form;
  }
  
  std::string adverb::superlative_form() const
  {
    assert(_valid == true);
    
    return _superlative_form;
  }
    
  bool adverb::has_comparative_form() const
  {
    assert(_valid == true);
    
    return !_comparative_form.empty();
  }
  
  bool adverb::has_superlative_form() const
  {
    assert(_valid == true);
    
    return !_superlative_form.empty();
  }
  
  adverb_query adverb::antonyms() const
  {
    assert(_valid == true);
    
    return _data->adverbs().antonym_of(*this);
  }
  
  adverb_query adverb::synonyms() const
  {
    assert(_valid == true);
    
    return _data->adverbs().synonym_of(*this);
  }
  
  adjective_query adverb::anti_mannernyms() const
  {
    assert(_valid == true);
    
    return _data->adjectives().anti_mannernym_of(*this);
  }
  
};
