#ifndef DATA_H_C4AEC3DD
#define DATA_H_C4AEC3DD

#include <sqlite3.h>

namespace verbly {
  
  class data;
  class word;
  class adjective;
  class noun;
  class verb;
  class adverb;
  class frame;
  class adjective_query;
  class adverb_query;
  class noun_query;
  class verb_query;
  class frame_query;
  class preposition_query;
  
  class data {
    private:
      sqlite3* ppdb;
      
      friend class adjective_query;
      friend class noun_query;
      friend class verb_query;
      friend class adverb_query;
      friend class frame_query;
      friend class preposition_query;
      
    public:
      data(std::string datafile);
      
      data(const data& other) = delete;
      data& operator=(const data& other) = delete;
      
      data(data&& other);
      data& operator=(data&& other);
      
      ~data();
      
      verb_query verbs() const;
      adjective_query adjectives() const;
      adverb_query adverbs() const;
      noun_query nouns() const;
      frame_query frames() const;
      preposition_query prepositions() const;
      
  };
  
  template <class T>
  class filter {
    public:
      enum class type {
        singleton,
        group
      };
      
      typedef filter<T> value_type;
      
      type get_type() const
      {
        return _type;
      }
      
      filter(const filter<T>& other)
      {
        _type = other._type;
        _notlogic = other._notlogic;
        
        switch (_type)
        {
          case type::singleton:
          {
            new(&_singleton.elem) T(other._singleton.elem);
            
            break;
          }
          
          case type::group:
          {
            new(&_group.elems) std::list<filter<T>>(other._group.elems);
            _group.orlogic = other._group.orlogic;
            
            break;
          }
        }
      }
      
      filter<T>& operator=(const filter<T>& other)
      {
        this->~filter();
        
        _type = other._type;
        _notlogic = other._notlogic;
        
        switch (_type)
        {
          case type::singleton:
          {
            new(&_singleton.elem) T(other._singleton.elem);
            
            break;
          }
          
          case type::group:
          {
            new(&_group.elems) std::list<filter<T>>(other._group.elems);
            _group.orlogic = other._group.orlogic;
            
            break;
          }
        }
        
        return *this;
      }
      
      ~filter()
      {
        switch (_type)
        {
          case type::singleton:
          {
            _singleton.elem.~T();
            
            break;
          }
          
          case type::group:
          {
            using list_type = std::list<filter<T>>;
            _group.elems.~list_type();
            
            break;
          }
        }
      }
      
      bool get_notlogic() const
      {
        return _notlogic;
      }
      
      void set_notlogic(bool _nl)
      {
        _notlogic = _nl;
      }
      
      std::list<T> inorder_flatten() const
      {
        std::list<T> result;
        
        if (_type == type::singleton)
        {
          result.push_back(_singleton.elem);
        } else if (_type == type::group)
        {
          for (auto elem : _group.elems)
          {
            auto l = elem.inorder_flatten();
            result.insert(std::end(result), std::begin(l), std::end(l));
          }
        }
        
        return result;
      }
      
      std::set<T> uniq_flatten() const
      {
        std::set<T> result;
        
        if (_type == type::singleton)
        {
          result.insert(_singleton.elem);
        } else if (_type == type::group)
        {
          for (auto elem : _group.elems)
          {
            auto l = elem.uniq_flatten();
            result.insert(std::begin(l), std::end(l));
          }
        }
        
        return result;
      }
      
      void clean()
      {
        if (_type == type::group)
        {
          std::list<typename std::list<filter<T>>::iterator> toremove;
          for (auto it = _group.elems.begin(); it != _group.elems.end(); it++)
          {
            it->clean();
            
            if (it->get_type() == type::group)
            {
              if (it->_group.elems.size() == 0)
              {
                toremove.push_back(it);
              } else if (it->_group.elems.size() == 1)
              {
                bool truelogic = it->_notlogic != it->_group.elems.front()._notlogic;
                filter<T> e = it->_group.elems.front();
                *it = e;
                it->_notlogic = truelogic;
              }
            }
          }
          
          for (auto rem : toremove)
          {
            _group.elems.erase(rem);
          }
          
          if (_group.elems.size() == 1)
          {
            bool truelogic = _notlogic != _group.elems.front()._notlogic;
            filter<T> e = _group.elems.front();
            *this = e;
            _notlogic = truelogic;
          }
        }
      }
      
      // Singleton
      filter(T _elem, bool _notlogic = false) : _type(type::singleton)
      {
        new(&_singleton.elem) T(_elem);
        this->_notlogic = _notlogic;
      }
      
      filter<T>& operator=(T _elem)
      {
        *this = filter<T>{_elem};
        
        return *this;
      }
      
      T get_elem() const
      {
        assert(_type == type::singleton);
        
        return _singleton.elem;
      }
      
      void set_elem(T _elem)
      {
        assert(_type == type::singleton);
        
        _singleton.elem = _elem;
      }
      
      // Group
      typedef typename std::list<filter<T>>::iterator iterator;
      
      filter() : _type(type::group)
      {
        new(&_group.elems) std::list<filter<T>>();
        _group.orlogic = false;
      }
      
      filter(std::initializer_list<filter<T>> _init) : _type(type::group)
      {
        new(&_group.elems) std::list<filter<T>>(_init);
        _group.orlogic = false;
      }
      
      iterator begin()
      {
        assert(_type == type::group);
        
        return _group.elems.begin();
      }
      
      iterator end()
      {
        assert(_type == type::group);
        
        return _group.elems.end();
      }
      
      filter<T>& operator<<(filter<T> _elem)
      {
        assert(_type == type::group);
        
        _group.elems.push_back(_elem);
        
        return *this;
      }
      
      void push_back(filter<T> _elem)
      {
        assert(_type == type::group);
        
        _group.elems.push_back(_elem);
      }
      
      bool get_orlogic() const
      {
        assert(_type == type::group);
        
        return _group.orlogic;
      }
      
      void set_orlogic(bool _ol)
      {
        assert(_type == type::group);
        
        _group.orlogic = _ol;
      }
      
      bool empty() const
      {
        if (_type == type::group)
        {
          return _group.elems.empty();
        } else {
          return false;
        }
      }
      
      int size() const
      {
        assert(_type == type::group);
        
        return _group.elems.size();
      }
      
    private:
      type _type;
      bool _notlogic = false;
      union {
        struct {
          T elem;
        } _singleton;
        struct {
          std::list<filter<T>> elems;
          bool orlogic;
        } _group;
      };
  };
  
  class binding {
    public:
      enum class type {
        integer,
        string
      };
  
      type get_type() const;
      binding(const binding& other);
      ~binding();
      binding& operator=(const binding& other);
    
      // Integer
      binding(int _arg);
      int get_integer() const;
      void set_integer(int _arg);
      binding& operator=(int _arg);
    
      // String
      binding(std::string _arg);
      std::string get_string() const;
      void set_string(std::string _arg);
      binding& operator=(std::string _arg);
    
    private:
      union {
        int _integer;
        std::string _string;
      };
      type _type;
  };
  
};

#endif /* end of include guard: DATA_H_C4AEC3DD */
