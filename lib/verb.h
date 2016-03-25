#ifndef VERB_H_BCC929AD
#define VERB_H_BCC929AD

namespace verbly {
  
  class frame_query;
  
  class verb : public word {
    private:
      std::string _infinitive;
      std::string _past_tense;
      std::string _past_participle;
      std::string _ing_form;
      std::string _s_form;
      
      friend class verb_query;
      
    public:
      verb();
      verb(const data& _data, int _id);
      
      std::string base_form() const;
      std::string infinitive_form() const;
      std::string past_tense_form() const;
      std::string past_participle_form() const;
      std::string ing_form() const;
      std::string s_form() const;
      
      frame_query frames() const;
  };
  
};

#endif /* end of include guard: VERB_H_BCC929AD */
