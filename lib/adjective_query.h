#ifndef ADJECTIVE_QUERY_H_05E590FD
#define ADJECTIVE_QUERY_H_05E590FD

namespace verbly {
  
  class adjective_query {
    public:
      adjective_query(const data& _data);
      
      adjective_query& limit(int _limit);
      adjective_query& random();
      adjective_query& except(const adjective& _word);
      adjective_query& rhymes_with(const word& _word);
      adjective_query& rhymes_with(rhyme _r);
      adjective_query& has_pronunciation();
      adjective_query& has_rhyming_noun();
      adjective_query& has_rhyming_adjective();
      adjective_query& has_rhyming_adverb();
      adjective_query& has_rhyming_verb();
      
      adjective_query& requires_comparative_form();
      adjective_query& requires_superlative_form();
      adjective_query& position(adjective::positioning pos);
      
      adjective_query& with_prefix(filter<std::string> _f);
      adjective_query& with_suffix(filter<std::string> _f);
      
      adjective_query& with_complexity(int _arg);
      
      adjective_query& is_variant();
      adjective_query& variant_of(filter<noun> _f);
      
      adjective_query& has_antonyms();
      adjective_query& antonym_of(filter<adjective> _f);
      
      adjective_query& has_synonyms();
      adjective_query& synonym_of(filter<adjective> _f);
      
      adjective_query& is_generalization();
      adjective_query& generalization_of(filter<adjective> _f);
      
      adjective_query& is_specification();
      adjective_query& specification_of(filter<adjective> _f);
      
      adjective_query& is_pertainymic();
      adjective_query& pertainym_of(filter<noun> _f);
      
      adjective_query& is_mannernymic();
      adjective_query& anti_mannernym_of(filter<adverb> _f);
      
/*      adjective_query& derived_from(const word& _w);
      adjective_query& not_derived_from(const word& _w);*/
      
      std::list<adjective> run() const;
      
      const static int unlimited = -1;

    protected:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<rhyme> _rhymes;
      std::list<adjective> _except;
      bool _has_prn = false;
      bool _has_rhyming_noun = false;
      bool _has_rhyming_adjective = false;
      bool _has_rhyming_adverb = false;
      bool _has_rhyming_verb = false;
      
      bool _requires_comparative_form = false;
      bool _requires_superlative_form = false;
      adjective::positioning _position = adjective::positioning::undefined;
      
      filter<std::string> _with_prefix;
      filter<std::string> _with_suffix;
      
      int _with_complexity = unlimited;
      
      bool _is_variant = false;
      filter<noun> _variant_of;
            
      bool _is_antonymic = false;
      filter<adjective> _antonym_of;
      
      bool _is_synonymic = false;
      filter<adjective> _synonym_of;
      
      bool _is_generalization = false;
      filter<adjective> _generalization_of;
      
      bool _is_specification = false;
      filter<adjective> _specification_of;
      
      bool _is_pertainymic = false;
      filter<noun> _pertainym_of;
      
      bool _is_mannernymic = false;
      filter<adverb> _anti_mannernym_of;
      
/*      std::list<adjective> _derived_from_adjective;
      std::list<adjective> _not_derived_from_adjective;
      std::list<adverb> _derived_from_adverb;
      std::list<adverb> _not_derived_from_adverb;
      std::list<noun> _derived_from_noun;
      std::list<noun> _not_derived_from_noun;*/
  };
  
};

#endif /* end of include guard: ADJECTIVE_QUERY_H_05E590FD */
