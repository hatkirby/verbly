#ifndef UTIL_H_15DDCA2D
#define UTIL_H_15DDCA2D

#include <string>
#include <iterator>
#include <sstream>

namespace verbly {
  
  template <class InputIterator>
  std::string implode(InputIterator first, InputIterator last, std::string delimiter)
  {
    std::stringstream result;
    
    for (InputIterator it = first; it != last; it++)
    {
      if (it != first)
      {
        result << delimiter;
      }
      
      result << *it;
    }
    
    return result.str();
  }
  
  template <class Container>
  Container split(std::string input, std::string delimiter)
  {
    Container result;
    
    while (!input.empty())
    {
      int divider = input.find(" ");
      if (divider == std::string::npos)
      {
        result.push_back(input);
        
        input = "";
      } else {
        result.push_back(input.substr(0, divider));
        
        input = input.substr(divider+1);
      }
    }
    
    return result;
  }
  
};

#endif /* end of include guard: UTIL_H_15DDCA2D */
