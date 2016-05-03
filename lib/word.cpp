#include "verbly.h"
#include <algorithm>

namespace verbly {
  
  rhyme::rhyme(std::string prerhyme, std::string phonemes) : _prerhyme(prerhyme), _rhyme(phonemes)
  {
    
  }
  
  std::string rhyme::get_prerhyme() const
  {
    return _prerhyme;
  }
  
  std::string rhyme::get_rhyme() const
  {
    return _rhyme;
  }
  
  bool rhyme::operator==(const rhyme& other) const
  {
    return std::tie(_prerhyme, _rhyme) == std::tie(other._prerhyme, other._rhyme);
  }
  
  word::word()
  {
    
  }
  
  word::word(const data& _data, int _id) : _data(&_data), _id(_id), _valid(true)
  {
    
  }
  
  std::list<rhyme> word::get_rhymes() const
  {
    assert(_valid == true);
    
    return rhymes;
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
