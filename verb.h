#ifndef VERB_H_BCC929AD
#define VERB_H_BCC929AD

#include <vector>

namespace verbly {
  
  /*class frame_part {
    
  };
  
  class frame {
    private:
      std::list<frame_part> content;
      std::map<std::string, std::vector<std::list<frame_part>::iterator>> predicates;
      
    public:
      frame(std::list<frame_part> content) : content(content)
      {
        
      }
      
      std::unique_ptr<token> make_utterance() const
      {
        
      }
  };*/
  
  enum class conjugation {
    present_participle,
    past_participle,
    infinitive
  };
  
  class verb {
    private:
      int id;
      
    public:
      verb(int id) : id(id)
      {
        
      }
      
      std::string infinitive;
      std::string past_tense;
      std::string past_participle;
      std::string ing_form;
      std::string s_form;
      //std::vector<frame> frames;
      
      std::string conjugate(conjugation infl) const
      {
        switch (infl)
        {
          case conjugation::infinitive: return infinitive;
          case conjugation::past_participle: return past_participle;
          case conjugation::present_participle: return ing_form;
        }
      }
  };
  
};

#include "token.h"

#endif /* end of include guard: VERB_H_BCC929AD */
