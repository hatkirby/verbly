#ifndef ADJECTIVE_H_87B3FB75
#define ADJECTIVE_H_87B3FB75

namespace verbly {
  
  class adjective_query;
  class adverb_query;
  class noun_query;
  
  class adjective : public word {
    public:
      enum class positioning {
        undefined,
        predicate,
        attributive,
        postnominal
      };
    
    private:
      std::string _base_form;
      std::string _comparative_form;
      std::string _superlative_form;
      positioning _position = positioning::undefined;
      
      friend class adjective_query;
      
    public:
      adjective(const data& _data, int _id);
      
      std::string base_form() const;
      std::string comparative_form() const;
      std::string superlative_form() const;
      positioning position() const;
      
      bool has_comparative_form() const;
      bool has_superlative_form() const;
      bool has_position() const;
      
      adjective_query antonyms() const;
      adjective_query synonyms() const;
      adjective_query generalizations() const;
      adjective_query specifications() const;
      noun_query anti_pertainyms() const;
      adverb_query mannernyms() const;
      noun_query attributes() const;
  };
  
  class adjective_query {
    public:
      adjective_query(const data& _data);
      
      adjective_query& limit(int _limit);
      adjective_query& random(bool _random);
      adjective_query& except(const adjective& _word);
      adjective_query& rhymes_with(const word& _word);
      adjective_query& has_pronunciation(bool _has_prn);
      
      adjective_query& requires_comparative_form(bool req);
      adjective_query& requires_superlative_form(bool req);
      adjective_query& position(adjective::positioning pos);
      
      adjective_query& is_variant(bool _is_variant);
      adjective_query& variant_of(const noun& _noun);
      adjective_query& not_variant_of(const noun& _noun);
      
      adjective_query& has_antonyms(bool _is_antonymic);
      adjective_query& antonym_of(const adjective& _adj);
      adjective_query& not_antonym_of(const adjective& _adj);
      
      adjective_query& has_synonyms(bool _is_synonymic);
      adjective_query& synonym_of(const adjective& _adj);
      adjective_query& not_synonym_of(const adjective& _adj);
      
      adjective_query& is_generalization(bool _is_generalization);
      adjective_query& generalization_of(const adjective& _adj);
      adjective_query& not_generalization_of(const adjective& _adj);
      
      adjective_query& is_specification(bool _is_specification);
      adjective_query& specification_of(const adjective& _adj);
      adjective_query& not_specification_of(const adjective& _adj);
      
      adjective_query& is_pertainymic(bool _is_pertainymic);
      adjective_query& pertainym_of(const noun& _noun);
      
      adjective_query& is_mannernymic(bool _is_mannernymic);
      adjective_query& anti_mannernym_of(const adverb& _adv);
      
      std::list<adjective> run() const;
      
      const static int unlimited = -1;

    protected:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<adjective> _except;
      bool _has_prn = false;
      
      bool _requires_comparative_form = false;
      bool _requires_superlative_form = false;
      adjective::positioning _position = adjective::positioning::undefined;
      
      bool _is_variant = false;
      std::list<noun> _variant_of;
      std::list<noun> _not_variant_of;
      
      bool _is_antonymic = false;
      std::list<adjective> _antonym_of;
      std::list<adjective> _not_antonym_of;
      
      bool _is_synonymic = false;
      std::list<adjective> _synonym_of;
      std::list<adjective> _not_synonym_of;
      
      bool _is_generalization = false;
      std::list<adjective> _generalization_of;
      std::list<adjective> _not_generalization_of;
      
      bool _is_specification = false;
      std::list<adjective> _specification_of;
      std::list<adjective> _not_specification_of;
      
      bool _is_pertainymic = false;
      std::list<noun> _pertainym_of;
      
      bool _is_mannernymic = false;
      std::list<adverb> _anti_mannernym_of;
  };
  
};

#endif /* end of include guard: ADJECTIVE_H_87B3FB75 */
