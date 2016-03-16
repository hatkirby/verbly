#ifndef WORD_H_8FC89498
#define WORD_H_8FC89498

namespace verbly {
  
  class adjective_query;
  class verb_query;
  class adverb_query;
  
  template <class T>
  class query;
  
  class word {
    protected:
      const data& _data;
      int _id;
      
      std::list<std::list<std::string>> pronunciations;
      
      word(const data& _data, int _id);
      
      friend class adjective_query;
      friend class verb_query;
      friend class noun_query;
      friend class adverb_query;
    
    public:
      virtual std::string base_form() const = 0;
      
      std::list<std::string> rhyme_phonemes() const;
  };
  
};

#endif /* end of include guard: WORD_H_8FC89498 */
