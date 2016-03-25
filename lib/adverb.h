#ifndef ADVERB_H_86F8302F
#define ADVERB_H_86F8302F

namespace verbly {
  
  class adverb : public word {
    private:
      std::string _base_form;
      std::string _comparative_form;
      std::string _superlative_form;
      
      friend class adverb_query;
      
    public:
      adverb();
      adverb(const data& _data, int _id);
      
      std::string base_form() const;
      std::string comparative_form() const;
      std::string superlative_form() const;
      
      bool has_comparative_form() const;
      bool has_superlative_form() const;
      
      adverb_query antonyms() const;
      adverb_query synonyms() const;
      adjective_query anti_mannernyms() const;
      
      adverb_query& derived_from(const word& _w);
      adverb_query& not_derived_from(const word& _w);
  };
  
};

#endif /* end of include guard: ADVERB_H_86F8302F */
