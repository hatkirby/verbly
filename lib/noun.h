#ifndef NOUN_H_24A03C83
#define NOUN_H_24A03C83

namespace verbly {
  
  class noun : public word {
    private:
      std::string _singular;
      std::string _plural;
      
      friend class noun_query;
      
    public:
      noun();
      noun(const data& _data, int _id);
      
      std::string base_form() const;
      std::string singular_form() const;
      std::string plural_form() const;
      
      bool has_plural_form() const;
      
      noun_query hypernyms() const;
      noun_query full_hypernyms() const;
      noun_query hyponyms() const;
      noun_query full_hyponyms() const;
      noun_query part_meronyms() const;
      noun_query part_holonyms() const;
      noun_query substance_meronyms() const;
      noun_query substance_holonyms() const;
      noun_query member_meronyms() const;
      noun_query member_holonyms() const;
      noun_query classes() const;
      noun_query instances() const;
      noun_query synonyms() const;
      noun_query antonyms() const;
      adjective_query pertainyms() const;
      adjective_query variations() const;
      
      bool operator<(const noun& other) const;
  };
  
};

#endif /* end of include guard: NOUN_H_24A03C83 */
