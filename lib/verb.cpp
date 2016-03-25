#include "verbly.h"

namespace verbly {
  
  verb::verb()
  {
    
  }
  
  verb::verb(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string verb::base_form() const
  {
    assert(_valid == true);
    
    return _infinitive;
  }
  
  std::string verb::infinitive_form() const
  {
    assert(_valid == true);
    
    return _infinitive;
  }
  
  std::string verb::past_tense_form() const
  {
    assert(_valid == true);
    
    return _past_tense;
  }
  
  std::string verb::past_participle_form() const
  {
    assert(_valid == true);
    
    return _past_participle;
  }
  
  std::string verb::ing_form() const
  {
    assert(_valid == true);
    
    return _ing_form;
  }
  
  std::string verb::s_form() const
  {
    assert(_valid == true);
    
    return _s_form;
  }
  
  frame_query verb::frames() const
  {
    assert(_valid == true);
    
    return _data->frames().for_verb(*this);
  }
    
};
