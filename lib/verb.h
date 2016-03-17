#ifndef VERB_H_BCC929AD
#define VERB_H_BCC929AD

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
  
  class verb : public word {
    private:
      std::string _infinitive;
      std::string _past_tense;
      std::string _past_participle;
      std::string _ing_form;
      std::string _s_form;
      
      friend class verb_query;
      
    public:
      verb(const data& _data, int _id);
      
      std::string base_form() const;
      std::string infinitive_form() const;
      std::string past_tense_form() const;
      std::string past_participle_form() const;
      std::string ing_form() const;
      std::string s_form() const;
  };
  
  class verb_query {
    public:
      verb_query(const data& _data);
      
      verb_query& limit(int _limit);
      verb_query& random(bool _random);
      verb_query& except(const verb& _word);
      verb_query& rhymes_with(const word& _word);
      verb_query& has_pronunciation(bool _has_prn);
      
      std::list<verb> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<verb> _except;
      bool _has_prn = false;
  };
  
};

#endif /* end of include guard: VERB_H_BCC929AD */
