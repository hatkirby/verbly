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
      adjective();
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
  
};

#endif /* end of include guard: ADJECTIVE_H_87B3FB75 */
