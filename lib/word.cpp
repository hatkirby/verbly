#include "verbly.h"

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
  
};
