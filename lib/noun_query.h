#ifndef NOUN_QUERY_H_5DE51DD7
#define NOUN_QUERY_H_5DE51DD7

namespace verbly {
  
  class noun_query {
    public:
      noun_query(const data& _data);
      
      noun_query& limit(int _limit);
      noun_query& random();
      noun_query& except(const noun& _word);
      noun_query& rhymes_with(const word& _word);
      noun_query& has_pronunciation();
      
      noun_query& with_singular_form(std::string _arg);
      noun_query& with_prefix(filter<std::string> _f);
      noun_query& with_suffix(filter<std::string> _f);
      
      noun_query& with_complexity(int _arg);
      
      noun_query& is_hypernym();
      noun_query& hypernym_of(filter<noun> _f);
      noun_query& full_hypernym_of(filter<noun> _f);
      
      noun_query& is_hyponym();
      noun_query& hyponym_of(filter<noun> _f);
      noun_query& full_hyponym_of(filter<noun> _f);
      
      noun_query& is_part_meronym();
      noun_query& part_meronym_of(filter<noun> _f);
      noun_query& full_part_meronym_of(filter<noun> _f);
      
      noun_query& is_part_holonym();
      noun_query& part_holonym_of(filter<noun> _f);
      noun_query& full_part_holonym_of(filter<noun> _f);
      
      noun_query& is_substance_meronym();
      noun_query& substance_meronym_of(filter<noun> _f);
      noun_query& full_substance_meronym_of(filter<noun> _f);
      
      noun_query& is_substance_holonym();
      noun_query& substance_holonym_of(filter<noun> _f);
      noun_query& full_substance_holonym_of(filter<noun> _f);
      
      noun_query& is_member_meronym();
      noun_query& member_meronym_of(filter<noun> _f);
      noun_query& full_member_meronym_of(filter<noun> _f);
      
      noun_query& is_member_holonym();
      noun_query& member_holonym_of(filter<noun> _f);
      noun_query& full_member_holonym_of(filter<noun> _f);
      
      noun_query& is_proper();
      noun_query& is_not_proper();
      
      noun_query& is_instance();
      noun_query& instance_of(filter<noun> _f);
      
      noun_query& is_class();
      noun_query& class_of(filter<noun> _f);
      
      noun_query& has_synonyms();
      noun_query& synonym_of(filter<noun> _f);
      
      noun_query& has_antonyms();
      noun_query& antonym_of(filter<noun> _f);
      
      noun_query& has_pertainym();
      noun_query& anti_pertainym_of(filter<adjective> _f);
      
      noun_query& is_attribute();
      noun_query& attribute_of(filter<adjective> _f);
      
/*      noun_query& derived_from(const word& _w);
      noun_query& not_derived_from(const word& _w);*/
      
      std::list<noun> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<noun> _except;
      bool _has_prn = false;
      
      std::list<std::string> _with_singular_form;
      filter<std::string> _with_prefix;
      filter<std::string> _with_suffix;
      
      int _with_complexity = unlimited;
      
      bool _is_hypernym = false;
      filter<noun> _hypernym_of;
      filter<noun> _full_hypernym_of;
      
      bool _is_hyponym = false;
      filter<noun> _hyponym_of;
      filter<noun> _full_hyponym_of;
      
      bool _is_part_meronym = false;
      filter<noun> _part_meronym_of;
      filter<noun> _full_part_meronym_of;
      
      bool _is_substance_meronym = false;
      filter<noun> _substance_meronym_of;
      filter<noun> _full_substance_meronym_of;
      
      bool _is_member_meronym = false;
      filter<noun> _member_meronym_of;
      filter<noun> _full_member_meronym_of;
      
      bool _is_part_holonym = false;
      filter<noun> _part_holonym_of;
      filter<noun> _full_part_holonym_of;
      
      bool _is_substance_holonym = false;
      filter<noun> _substance_holonym_of;
      filter<noun> _full_substance_holonym_of;
      
      bool _is_member_holonym = false;
      filter<noun> _member_holonym_of;
      filter<noun> _full_member_holonym_of;
      
      bool _is_proper = false;
      bool _is_not_proper = false;
      
      bool _is_instance = false;
      filter<noun> _instance_of;
      
      bool _is_class = false;
      filter<noun> _class_of;
      
      bool _has_synonyms = false;
      filter<noun> _synonym_of;
      
      bool _has_antonyms = false;
      filter<noun> _antonym_of;
      
      bool _has_pertainym = false;
      filter<adjective> _anti_pertainym_of;
      
      bool _is_attribute = false;
      filter<adjective> _attribute_of;
      
/*      std::list<adjective> _derived_from_adjective;
      std::list<adjective> _not_derived_from_adjective;
      std::list<adverb> _derived_from_adverb;
      std::list<adverb> _not_derived_from_adverb;
      std::list<noun> _derived_from_noun;
      std::list<noun> _not_derived_from_noun;*/
  };
  
};

#endif /* end of include guard: NOUN_QUERY_H_5DE51DD7 */
