#include "verbly.h"

namespace verbly {
  
  verb_query::verb_query(const data& _data) : _data(_data)
  {

  }
  
  verb_query& verb_query::limit(int _limit)
  {
    if ((_limit > 0) || (_limit == unlimited))
    {
      this->_limit = _limit;
    }
  
    return *this;
  }

  verb_query& verb_query::random()
  {
    this->_random = true;
  
    return *this;
  }
  
  verb_query& verb_query::except(const verb& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  verb_query& verb_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.get_rhymes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const verb*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const verb&>(_word));
    }
    
    return *this;
  }
  
  verb_query& verb_query::rhymes_with(rhyme _r)
  {
    _rhymes.push_back(_r);
    
    return *this;
  }
  
  verb_query& verb_query::has_pronunciation()
  {
    this->_has_prn = true;
    
    return *this;
  }
  
  verb_query& verb_query::has_rhyming_noun()
  {
    _has_rhyming_noun = true;
    
    return *this;
  }
  
  verb_query& verb_query::has_rhyming_adjective()
  {
    _has_rhyming_adjective = true;
    
    return *this;
  }
  
  verb_query& verb_query::has_rhyming_adverb()
  {
    _has_rhyming_adverb = true;
    
    return *this;
  }
  
  verb_query& verb_query::has_rhyming_verb()
  {
    _has_rhyming_verb = true;
    
    return *this;
  }
  
  verb_query& verb_query::with_stress(filter<std::vector<bool>> _arg)
  {
    _stress = _arg;
    
    return *this;
  }
  
  verb_query& verb_query::has_frames()
  {
    this->_has_frames = true;
    
    return *this;
  }
  
  std::list<verb> verb_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT verb_id, infinitive, past_tense, past_participle, ing_form, s_form FROM verbs";
    std::list<std::string> conditions;
    std::list<binding> bindings;
    
    if (_has_prn)
    {
      conditions.push_back("verb_id IN (SELECT verb_id FROM verb_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "(prerhyme != ? AND rhyme = ?)");
      std::string cond = "verb_id IN (SELECT verb_id FROM verb_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
      
      for (auto rhy : _rhymes)
      {
        bindings.emplace_back(rhy.get_prerhyme());
        bindings.emplace_back(rhy.get_rhyme());
      }
    }
    
    if (_has_rhyming_noun)
    {
      conditions.push_back("verb_id IN (SELECT a.verb_id FROM verbs AS a INNER JOIN verb_pronunciations AS curp ON curp.noun_id = a.adverb_id INNER JOIN noun_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_adjective)
    {
      conditions.push_back("verb_id IN (SELECT a.verb_id FROM verbs AS a INNER JOIN verb_pronunciations AS curp ON curp.noun_id = a.adverb_id INNER JOIN adjective_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_adverb)
    {
      conditions.push_back("verb_id IN (SELECT a.verb_id FROM verbs AS a INNER JOIN verb_pronunciations AS curp ON curp.noun_id = a.adverb_id INNER JOIN adverb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_verb)
    {
      conditions.push_back("verb_id IN (SELECT a.verb_id FROM verbs AS a INNER JOIN verb_pronunciations AS curp ON curp.noun_id = a.adverb_id INNER JOIN verb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme AND rhmp.verb_id != curp.verb_id)");
    }
    
    if (!_stress.empty())
    {
      std::stringstream cond;
      if (_stress.get_notlogic())
      {
        cond << "verb_id NOT IN";
      } else {
        cond << "verb_id IN";
      }
      
      cond << "(SELECT verb_id FROM verb_pronunciations WHERE ";
      
      std::function<std::string (filter<std::vector<bool>>, bool)> recur = [&] (filter<std::vector<bool>> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<std::vector<bool>>::type::singleton:
          {
            std::ostringstream _val;
            for (auto syl : f.get_elem())
            {
              if (syl)
              {
                _val << "1";
              } else {
                _val << "0";
              }
            }
            
            bindings.emplace_back(_val.str());
            
            if (notlogic == f.get_notlogic())
            {
              return "stress = ?";
            } else {
              return "stress != ?";
            }
          }
          
          case filter<std::vector<bool>>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<std::vector<bool>> f2) {
              return recur(f2, truelogic);
            });
            
            if (truelogic == f.get_orlogic())
            {
              return "(" + verbly::implode(std::begin(clauses), std::end(clauses), " AND ") + ")";
            } else {
              return "(" + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
            }
          }
        }
      };
      
      cond << recur(_stress, _stress.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    for (auto except : _except)
    {
      conditions.push_back("verb_id != ?");
      bindings.emplace_back(except._id);
    }
    
    if (!_has_frames)
    {
      conditions.push_back("verb_id IN (SELECT verb_id FROM verb_groups)");
    }
    
    if (!conditions.empty())
    {
      construct << " WHERE ";
      construct << verbly::implode(std::begin(conditions), std::end(conditions), " AND ");
    }
    
    if (_random)
    {
      construct << " ORDER BY RANDOM()";
    }
    
    if (_limit != unlimited)
    {
      construct << " LIMIT " << _limit;
    }
    
    sqlite3_stmt* ppstmt;
    std::string query = construct.str();
    if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
    {
      throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
    }
    
    int i = 1;
    for (auto& binding : bindings)
    {
      switch (binding.get_type())
      {
        case binding::type::integer:
        {
          sqlite3_bind_int(ppstmt, i, binding.get_integer());
          
          break;
        }
        
        case binding::type::string:
        {
          sqlite3_bind_text(ppstmt, i, binding.get_string().c_str(), binding.get_string().length(), SQLITE_TRANSIENT);
          
          break;
        }
      }
      
      i++;
    }
    
    std::list<verb> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      verb tnc {_data, sqlite3_column_int(ppstmt, 0)};
      tnc._infinitive = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
      tnc._past_tense = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
      tnc._past_participle = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 3)));
      tnc._ing_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 4)));
      tnc._s_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 5)));
      
      output.push_back(tnc);
    }
    
    sqlite3_finalize(ppstmt);
    
    for (auto& verb : output)
    {
      query = "SELECT pronunciation, prerhyme, rhyme FROM verb_pronunciations WHERE verb_id = ?";
      if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
      }
      
      sqlite3_bind_int(ppstmt, 1, verb._id);
      
      while (sqlite3_step(ppstmt) == SQLITE_ROW)
      {
        std::string pronunciation(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 0)));
        auto phonemes = verbly::split<std::list<std::string>>(pronunciation, " ");
        
        verb.pronunciations.push_back(phonemes);
        
        if ((sqlite3_column_type(ppstmt, 1) != SQLITE_NULL) && (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL))
        {
          std::string prerhyme(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
          std::string rhyming(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
          verb.rhymes.emplace_back(prerhyme, rhyming);
        }
      }
      
      sqlite3_finalize(ppstmt);
    }
    
    return output;
  }

};
