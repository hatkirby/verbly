#ifndef ADVERB_QUERY_H_CA13CCDD
#define ADVERB_QUERY_H_CA13CCDD

namespace verbly {
  
  class adverb_query {
    public:
      adverb_query(const data& _data);
      
      adverb_query& limit(int _limit);
      adverb_query& random();
      adverb_query& except(const adverb& _word);
      adverb_query& rhymes_with(const word& _word);
      adverb_query& rhymes_with(rhyme _r);
      adverb_query& has_pronunciation();
      adverb_query& has_rhyming_noun();
      adverb_query& has_rhyming_adjective();
      adverb_query& has_rhyming_adverb();
      adverb_query& has_rhyming_verb();
      
      adverb_query& requires_comparative_form();
      adverb_query& requires_superlative_form();
      
      adverb_query& with_prefix(filter<std::string> _f);
      adverb_query& with_suffix(filter<std::string> _f);
      
      adverb_query& with_complexity(int _arg);
      
      adverb_query& has_antonyms();
      adverb_query& antonym_of(filter<adverb> _f);
      
      adverb_query& has_synonyms();
      adverb_query& synonym_of(filter<adverb> _f);
      
      adverb_query& is_mannernymic();
      adverb_query& mannernym_of(filter<adjective> _f);
      
/*      adverb_query& derived_from(const word& _w);
      adverb_query& not_derived_from(const word& _w);*/
      
      std::list<adverb> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<rhyme> _rhymes;
      std::list<adverb> _except;
      bool _has_prn = false;
      bool _has_rhyming_noun = false;
      bool _has_rhyming_adjective = false;
      bool _has_rhyming_adverb = false;
      bool _has_rhyming_verb = false;
      
      bool _requires_comparative_form = false;
      bool _requires_superlative_form = false;
      
      filter<std::string> _with_prefix;
      filter<std::string> _with_suffix;
      
      int _with_complexity = unlimited;
      
      bool _has_antonyms = false;
      filter<adverb> _antonym_of;
      
      bool _has_synonyms = false;
      filter<adverb> _synonym_of;
      
      bool _is_mannernymic = false;
      filter<adjective> _mannernym_of;
      
/*      std::list<adjective> _derived_from_adjective;
      std::list<adjective> _not_derived_from_adjective;
      std::list<adverb> _derived_from_adverb;
      std::list<adverb> _not_derived_from_adverb;
      std::list<noun> _derived_from_noun;
      std::list<noun> _not_derived_from_noun;*/
  };
  
};

#endif /* end of include guard: ADVERB_QUERY_H_CA13CCDD */
