#ifndef PREPOSITION_H_FF908021
#define PREPOSITION_H_FF908021

namespace verbly {
  
  class preposition_query;
  
  class preposition {
    public:
      std::string get_form() const;
      
    private:
      friend class preposition_query;
      
      std::string form;
  };
  
  class preposition_query {
    public:
      preposition_query(const data& _data);
      
      preposition_query& limit(int _limit);
      preposition_query& random();
      preposition_query& in_group(std::string _arg);
      
      std::list<preposition> run() const;
    
      const static int unlimited = -1;
    private:
      const data& _data;
      int _limit = unlimited;
      bool _random = false;
      std::list<std::string> _in_group;
  };
  
};

#endif /* end of include guard: PREPOSITION_H_FF908021 */
