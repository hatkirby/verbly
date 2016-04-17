#ifndef WORD_H_8FC89498
#define WORD_H_8FC89498

namespace verbly {
  
  class rhyme {
    public:
      rhyme(std::string prerhyme, std::string phonemes);
      
      std::string get_prerhyme() const;
      std::string get_rhyme() const;
      
      bool operator==(const rhyme& other) const;
      
    private:
      std::string _prerhyme;
      std::string _rhyme;
  };
  
  class word {
    protected:
      const data* _data;
      int _id;
      bool _valid = false;
      
      std::list<std::list<std::string>> pronunciations;
      std::list<rhyme> rhymes;
      
      word();
      word(const data& _data, int _id);
      
      friend class adjective_query;
      friend class verb_query;
      friend class noun_query;
      friend class adverb_query;
      friend class frame_query;
      friend class preposition_query;
    
    public:
      virtual std::string base_form() const = 0;
      
      std::list<rhyme> get_rhymes() const;
      bool starts_with_vowel_sound() const;
  };
  
};

#endif /* end of include guard: WORD_H_8FC89498 */
