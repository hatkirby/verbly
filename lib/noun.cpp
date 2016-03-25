#include "verbly.h"
#include <set>
#include <iostream>

namespace verbly {
  
  noun::noun()
  {
    
  }
  
  noun::noun(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string noun::base_form() const
  {
    assert(_valid == true);
    
    return _singular;
  }

  std::string noun::singular_form() const
  {
    assert(_valid == true);
    
    return _singular;
  }
  
  std::string noun::plural_form() const
  {
    assert(_valid == true);
    
    return _plural;
  }

  bool noun::has_plural_form() const
  {
    assert(_valid == true);
    
    return !_plural.empty();
  }
  
  noun_query noun::hypernyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().hypernym_of(*this);
  }
  
  noun_query noun::full_hypernyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().full_hypernym_of(*this);
  }
  
  noun_query noun::hyponyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().hyponym_of(*this);
  }
  
  noun_query noun::full_hyponyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().full_hyponym_of(*this);
  }
  
  noun_query noun::part_meronyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().part_meronym_of(*this);
  }
  
  noun_query noun::part_holonyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().part_holonym_of(*this);
  }
  
  noun_query noun::substance_meronyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().substance_meronym_of(*this);
  }
  
  noun_query noun::substance_holonyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().substance_holonym_of(*this);
  }
  
  noun_query noun::member_meronyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().member_meronym_of(*this);
  }
  
  noun_query noun::member_holonyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().member_holonym_of(*this);
  }
  
  noun_query noun::classes() const
  {
    assert(_valid == true);
    
    return _data->nouns().class_of(*this);
  }
  
  noun_query noun::instances() const
  {
    assert(_valid == true);
    
    return _data->nouns().instance_of(*this);
  }
  
  noun_query noun::synonyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().synonym_of(*this);
  }
  
  noun_query noun::antonyms() const
  {
    assert(_valid == true);
    
    return _data->nouns().antonym_of(*this);
  }
  
  adjective_query noun::pertainyms() const
  {
    assert(_valid == true);
    
    return _data->adjectives().pertainym_of(*this);
  }
  
  adjective_query noun::variations() const
  {
    assert(_valid == true);
    
    return _data->adjectives().variant_of(*this);
  }
  
  bool noun::operator<(const noun& other) const
  {
    return _id < other._id;
  }

};
