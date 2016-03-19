#include "verbly.h"
#include <algorithm>

namespace verbly {
  
  word::word(const data& _data, int _id) : _data(_data), _id(_id)
  {
    
  }
  
  std::list<std::string> word::rhyme_phonemes() const
  {
    std::list<std::string> result;
    
    for (auto pronunciation : pronunciations)
    {
      auto phemstrt = std::find_if(std::begin(pronunciation), std::end(pronunciation), [] (std::string phoneme) {
        return phoneme.find("1") != std::string::npos;
      });
      
      std::stringstream rhymer;
      for (auto it = phemstrt; it != std::end(pronunciation); it++)
      {
        rhymer << " " << *it;
      }
      
      result.push_back(rhymer.str());
    }
    
    return result;
  }
  
  bool word::starts_with_vowel_sound() const
  {
    return std::any_of(std::begin(pronunciations), std::end(pronunciations), [] (std::list<std::string> phonemes) {
      return (phonemes.front().find_first_of("012") != std::string::npos);
    });
  }
  
};
