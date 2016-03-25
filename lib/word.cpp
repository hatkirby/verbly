#include "verbly.h"
#include <algorithm>

namespace verbly {
  
  word::word()
  {
    
  }
  
  word::word(const data& _data, int _id) : _data(&_data), _id(_id), _valid(true)
  {
    
  }
  
  std::list<std::string> word::rhyme_phonemes() const
  {
    assert(_valid == true);
    
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
    assert(_valid == true);
    
    if (pronunciations.size() > 0)
    {
      return std::any_of(std::begin(pronunciations), std::end(pronunciations), [] (std::list<std::string> phonemes) {
        return (phonemes.front().find_first_of("012") != std::string::npos);
      });
    } else {
      // If the word is not in CMUDICT, fall back to checking whether the first letter is a vowel
      // Not perfect but will work in most cases
      char ch = tolower(base_form().front());
      return (ch == 'a') || (ch == 'e') || (ch == 'i') || (ch == 'o') || (ch == 'u');
    }
  }
  
};
