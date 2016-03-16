#ifndef NOUN_H_24A03C83
#define NOUN_H_24A03C83

namespace verbly {
  
  class noun : public word {
    private:
      std::string _singular;
      std::string _plural;
      
      friend class noun_query;
      
    public:
      noun(const data& _data, int _id);
      
      std::string base_form() const;
      std::string singular_form() const;
      std::string plural_form() const;
      
      bool has_plural_form() const;
      
      noun_query hypernyms() const;
      noun_query hyponyms() const;
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
  };
  
  class noun_query {
    public:
      noun_query(const data& _data);
      
      noun_query& limit(int _limit);
      noun_query& random(bool _random);
      noun_query& except(const noun& _word);
      noun_query& rhymes_with(const word& _word);
      noun_query& has_pronunciation(bool _has_prn);
      
      noun_query& is_hypernym(bool _arg);
      noun_query& hypernym_of(const noun& _noun);
      noun_query& not_hypernym_of(const noun& _noun);
      
      noun_query& is_hyponym(bool _arg);
      noun_query& hyponym_of(const noun& _noun);
      noun_query& not_hyponym_of(const noun& _noun);
      
      noun_query& is_part_meronym(bool _arg);
      noun_query& part_meronym_of(const noun& _noun);
      noun_query& not_part_meronym_of(const noun& _noun);
      
      noun_query& is_part_holonym(bool _arg);
      noun_query& part_holonym_of(const noun& _noun);
      noun_query& not_part_holonym_of(const noun& _noun);
      
      noun_query& is_substance_meronym(bool _arg);
      noun_query& substance_meronym_of(const noun& _noun);
      noun_query& not_substance_meronym_of(const noun& _noun);
      
      noun_query& is_substance_holonym(bool _arg);
      noun_query& substance_holonym_of(const noun& _noun);
      noun_query& not_substance_holonym_of(const noun& _noun);
      
      noun_query& is_member_meronym(bool _arg);
      noun_query& member_meronym_of(const noun& _noun);
      noun_query& not_member_meronym_of(const noun& _noun);
      
      noun_query& is_member_holonym(bool _arg);
      noun_query& member_holonym_of(const noun& _noun);
      noun_query& not_member_holonym_of(const noun& _noun);
      
      noun_query& is_proper(bool _arg);
      noun_query& instance_of(const noun& _noun);
      noun_query& not_instance_of(const noun& _noun);
      
      noun_query& is_class(bool _arg);
      noun_query& class_of(const noun& _noun);
      noun_query& not_class_of(const noun& _noun);
      
      noun_query& has_synonyms(bool _arg);
      noun_query& synonym_of(const noun& _noun);
      noun_query& not_synonym_of(const noun& _noun);
      
      noun_query& has_antonyms(bool _arg);
      noun_query& antonym_of(const noun& _noun);
      noun_query& not_antonym_of(const noun& _noun);
      
      noun_query& has_pertainym(bool _arg);
      noun_query& anti_pertainym_of(const adjective& _adj);
      
      noun_query& is_attribute(bool _arg);
      noun_query& attribute_of(const adjective& _adj);
      
      std::list<noun> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<noun> _except;
      bool _has_prn = false;
      
      bool _is_hypernym = false;
      std::list<noun> _hypernym_of;
      std::list<noun> _not_hypernym_of;
      
      bool _is_hyponym = false;
      std::list<noun> _hyponym_of;
      std::list<noun> _not_hyponym_of;
      
      bool _is_part_meronym = false;
      std::list<noun> _part_meronym_of;
      std::list<noun> _not_part_meronym_of;
      
      bool _is_substance_meronym = false;
      std::list<noun> _substance_meronym_of;
      std::list<noun> _not_substance_meronym_of;
      
      bool _is_member_meronym = false;
      std::list<noun> _member_meronym_of;
      std::list<noun> _not_member_meronym_of;
      
      bool _is_part_holonym = false;
      std::list<noun> _part_holonym_of;
      std::list<noun> _not_part_holonym_of;
      
      bool _is_substance_holonym = false;
      std::list<noun> _substance_holonym_of;
      std::list<noun> _not_substance_holonym_of;
      
      bool _is_member_holonym = false;
      std::list<noun> _member_holonym_of;
      std::list<noun> _not_member_holonym_of;
      
      bool _is_proper = false;
      std::list<noun> _instance_of;
      std::list<noun> _not_instance_of;
      
      bool _is_class = false;
      std::list<noun> _class_of;
      std::list<noun> _not_class_of;
      
      bool _has_synonyms = false;
      std::list<noun> _synonym_of;
      std::list<noun> _not_synonym_of;
      
      bool _has_antonyms = false;
      std::list<noun> _antonym_of;
      std::list<noun> _not_antonym_of;
      
      bool _has_pertainym = false;
      std::list<adjective> _anti_pertainym_of;
      
      bool _is_attribute = false;
      std::list<adjective> _attribute_of;
  };
  
};

#endif /* end of include guard: NOUN_H_24A03C83 */
