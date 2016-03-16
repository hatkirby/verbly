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
      adverb(const data& _data, int _id);
      
      std::string base_form() const;
      std::string comparative_form() const;
      std::string superlative_form() const;
      
      bool has_comparative_form() const;
      bool has_superlative_form() const;
      
      adverb_query antonyms() const;
      adverb_query synonyms() const;
      adjective_query anti_mannernyms() const;
  };
  
  class adverb_query {
    public:
      adverb_query(const data& _data);
      
      adverb_query& limit(int _limit);
      adverb_query& random(bool _random);
      adverb_query& except(const adverb& _word);
      adverb_query& rhymes_with(const word& _word);
      adverb_query& has_pronunciation(bool _has_prn);
      
      adverb_query& requires_comparative_form(bool _arg);
      adverb_query& requires_superlative_form(bool _arg);
      
      adverb_query& has_antonyms(bool _arg);
      adverb_query& antonym_of(const adverb& _adv);
      adverb_query& not_antonym_of(const adverb& _adv);
      
      adverb_query& has_synonyms(bool _arg);
      adverb_query& synonym_of(const adverb& _adv);
      adverb_query& not_synonym_of(const adverb& _adv);
      
      adverb_query& is_mannernymic(bool _arg);
      adverb_query& mannernym_of(const adjective& _adj);
      
      std::list<adverb> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<adverb> _except;
      bool _has_prn = false;
      
      bool _requires_comparative_form = false;
      bool _requires_superlative_form = false;
      
      bool _has_antonyms = false;
      std::list<adverb> _antonym_of;
      std::list<adverb> _not_antonym_of;
      
      bool _has_synonyms = false;
      std::list<adverb> _synonym_of;
      std::list<adverb> _not_synonym_of;
      
      bool _is_mannernymic = false;
      std::list<adjective> _mannernym_of;
  };
  
};

#endif /* end of include guard: ADVERB_H_86F8302F */
