#ifndef VERB_QUERY_H_34E5A679
#define VERB_QUERY_H_34E5A679

namespace verbly {

  class verb_query {
    public:
      verb_query(const data& _data);
      
      verb_query& limit(int _limit);
      verb_query& random();
      verb_query& except(const verb& _word);
      verb_query& rhymes_with(const word& _word);
      verb_query& rhymes_with(rhyme _r);
      verb_query& has_pronunciation();
      verb_query& has_rhyming_noun();
      verb_query& has_rhyming_adjective();
      verb_query& has_rhyming_adverb();
      verb_query& has_rhyming_verb();
      verb_query& with_stress(filter<std::vector<bool>> _arg);
      
      verb_query& has_frames();
      
      std::list<verb> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<rhyme> _rhymes;
      std::list<verb> _except;
      bool _has_prn = false;
      bool _has_frames = false;
      bool _has_rhyming_noun = false;
      bool _has_rhyming_adjective = false;
      bool _has_rhyming_adverb = false;
      bool _has_rhyming_verb = false;
      filter<std::vector<bool>> _stress;
  };

};

#endif /* end of include guard: VERB_QUERY_H_34E5A679 */
