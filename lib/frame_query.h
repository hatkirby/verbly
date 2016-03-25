#ifndef FRAME_QUERY_H_334B9D47
#define FRAME_QUERY_H_334B9D47

namespace verbly {

  class frame_query {
    public:
      frame_query(const data& _data);
      
      frame_query& for_verb(const verb& _v);
      
      std::list<frame> run() const;
      
    private:
      const data& _data;
      std::list<verb> _for_verb;      
  };

};

#endif /* end of include guard: FRAME_QUERY_H_334B9D47 */
