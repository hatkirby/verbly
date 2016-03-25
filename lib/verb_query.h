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
      verb_query& has_pronunciation();
      
      verb_query& has_frames();
      
      std::list<verb> run() const;
      
      const static int unlimited = -1;
      
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _rhymes;
      std::list<verb> _except;
      bool _has_prn = false;
      bool _has_frames = false;
  };

};

#endif /* end of include guard: VERB_QUERY_H_34E5A679 */
