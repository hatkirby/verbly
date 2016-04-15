#include "verbly.h"

namespace verbly {
  
  adverb_query::adverb_query(const data& _data) : _data(_data)
  {

  }
  
  adverb_query& adverb_query::limit(int _limit)
  {
    if ((_limit > 0) || (_limit == unlimited))
    {
      this->_limit = _limit;
    }
  
    return *this;
  }

  adverb_query& adverb_query::random()
  {
    this->_random = true;
  
    return *this;
  }
  
  adverb_query& adverb_query::except(const adverb& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  adverb_query& adverb_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.rhyme_phonemes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const adverb*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const adverb&>(_word));
    }
    
    return *this;
  }
  
  adverb_query& adverb_query::has_pronunciation()
  {
    this->_has_prn = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::requires_comparative_form()
  {
    _requires_comparative_form = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::requires_superlative_form()
  {
    _requires_superlative_form = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::with_prefix(filter<std::string> _f)
  {
    _f.clean();
    _with_prefix = _f;
    
    return *this;
  }
  
  adverb_query& adverb_query::with_suffix(filter<std::string> _f)
  {
    _f.clean();
    _with_suffix = _f;
    
    return *this;
  }
  
  adverb_query& adverb_query::with_complexity(int _arg)
  {
    _with_complexity = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::has_antonyms()
  {
    _has_antonyms = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::antonym_of(filter<adverb> _f)
  {
    _f.clean();
    _antonym_of = _f;
    
    return *this;
  }
  
  adverb_query& adverb_query::has_synonyms()
  {
    _has_synonyms = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::synonym_of(filter<adverb> _f)
  {
    _f.clean();
    _synonym_of = _f;
    
    return *this;
  }
  
  adverb_query& adverb_query::is_mannernymic()
  {
    _is_mannernymic = true;
    
    return *this;
  }
  
  adverb_query& adverb_query::mannernym_of(filter<adjective> _f)
  {
    _f.clean();
    _mannernym_of = _f;
    
    return *this;
  }
  /*
  adverb_query& adverb_query::derived_from(const word& _w)
  {
    if (dynamic_cast<const adjective*>(&_w) != nullptr)
    {
      _derived_from_adjective.push_back(dynamic_cast<const adjective&>(_w));
    } else if (dynamic_cast<const adverb*>(&_w) != nullptr)
    {
      _derived_from_adverb.push_back(dynamic_cast<const adverb&>(_w));
    } else if (dynamic_cast<const noun*>(&_w) != nullptr)
    {
      _derived_from_noun.push_back(dynamic_cast<const noun&>(_w));
    }
    
    return *this;
  }
  
  adverb_query& adverb_query::not_derived_from(const word& _w)
  {
    if (dynamic_cast<const adjective*>(&_w) != nullptr)
    {
      _not_derived_from_adjective.push_back(dynamic_cast<const adjective&>(_w));
    } else if (dynamic_cast<const adverb*>(&_w) != nullptr)
    {
      _not_derived_from_adverb.push_back(dynamic_cast<const adverb&>(_w));
    } else if (dynamic_cast<const noun*>(&_w) != nullptr)
    {
      _not_derived_from_noun.push_back(dynamic_cast<const noun&>(_w));
    }
    
    return *this;
  }
  */
  std::list<adverb> adverb_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT adverb_id, base_form, comparative, superlative FROM adverbs";
    std::list<std::string> conditions;
    std::list<binding> bindings;
    
    if (_has_prn)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_id FROM adverb_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "pronunciation LIKE ?");
      std::string cond = "adverb_id IN (SELECT adverb_id FROM adverb_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
      
      for (auto rhyme : _rhymes)
      {
        bindings.emplace_back("%" + rhyme);
      }
    }
    
    for (auto except : _except)
    {
      conditions.push_back("adverb_id != ?");
      bindings.emplace_back(except._id);
    }
    
    if (_requires_comparative_form)
    {
      conditions.push_back("comparative IS NOT NULL");
    }
    
    if (_requires_superlative_form)
    {
      conditions.push_back("superlative IS NOT NULL");
    }
    
    if (!_with_prefix.empty())
    {
      std::function<std::string (filter<std::string>, bool)> recur = [&] (filter<std::string> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<std::string>::type::singleton:
          {
            bindings.emplace_back(f.get_elem() + "%");
            
            if (notlogic == f.get_notlogic())
            {
              return "base_form LIKE ?";
            } else {
              return "base_form NOT LIKE ?";
            }
          }
          
          case filter<std::string>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<std::string> f2) {
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
      
      conditions.push_back(recur(_with_prefix, false));
    }
    
    if (!_with_suffix.empty())
    {
      std::function<std::string (filter<std::string>, bool)> recur = [&] (filter<std::string> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<std::string>::type::singleton:
          {
            bindings.emplace_back("%" + f.get_elem());
            
            if (notlogic == f.get_notlogic())
            {
              return "base_form LIKE ?";
            } else {
              return "base_form NOT LIKE ?";
            }
          }
          
          case filter<std::string>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<std::string> f2) {
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
      
      conditions.push_back(recur(_with_suffix, false));
    }
    
    if (_with_complexity != unlimited)
    {
      conditions.push_back("complexity = ?");
      bindings.emplace_back(_with_complexity);
    }
    
    if (_has_antonyms)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_2_id FROM adverb_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::stringstream cond;
      if (_antonym_of.get_notlogic())
      {
        cond << "adverb_id NOT IN";
      } else {
        cond << "adverb_id IN";
      }
      
      cond << "(SELECT adverb_2_id FROM adverb_antonymy WHERE ";
      
      std::function<std::string (filter<adverb>, bool)> recur = [&] (filter<adverb> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adverb>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "adverb_1_id = ?";
            } else {
              return "adverb_1_id != ?";
            }
          }
          
          case filter<adverb>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<adverb> f2) {
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
      
      cond << recur(_antonym_of, _antonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_has_synonyms)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_2_id FROM adverb_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::stringstream cond;
      if (_antonym_of.get_notlogic())
      {
        cond << "adverb_id NOT IN";
      } else {
        cond << "adverb_id IN";
      }
      
      cond << "(SELECT adverb_2_id FROM adverb_synonymy WHERE ";
      
      std::function<std::string (filter<adverb>, bool)> recur = [&] (filter<adverb> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adverb>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "adverb_1_id = ?";
            } else {
              return "adverb_1_id != ?";
            }
          }
          
          case filter<adverb>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<adverb> f2) {
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
      
      cond << recur(_synonym_of, _synonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_mannernymic)
    {
      conditions.push_back("adverb_id IN (SELECT mannernym_id FROM mannernymy)");
    }
    
    if (!_mannernym_of.empty())
    {
      std::stringstream cond;
      if (_antonym_of.get_notlogic())
      {
        cond << "adverb_id NOT IN";
      } else {
        cond << "adverb_id IN";
      }
      
      cond << "(SELECT mannernym_id FROM mannernymy WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "adjective_id = ?";
            } else {
              return "adjective_id != ?";
            }
          }
          
          case filter<adjective>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<adjective> f2) {
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
      
      cond << recur(_mannernym_of, _mannernym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
/*    if (!_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_derived_from_adjective.size(), "adjective_id = @DERADJ");
      std::string cond = "adverb_id IN (SELECT adverb_id FROM adjective_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adjective.size(), "adjective_id = @NDERADJ");
      std::string cond = "adverb_id NOT IN (SELECT adverb_id FROM adjective_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_derived_from_adverb.size(), "adverb_2_id = @DERADV");
      std::string cond = "adverb_id IN (SELECT adverb_1_id FROM adverb_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adverb.size(), "adverb_2_id = @NDERADV");
      std::string cond = "adverb_id NOT IN (SELECT adverb_1_id FROM adverb_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_derived_from_noun.size(), "noun_id = @DERN");
      std::string cond = "adverb_id IN (SELECT adverb_id FROM noun_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_not_derived_from_noun.size(), "noun_id = @NDERN");
      std::string cond = "adverb_id NOT IN (SELECT adverb_id FROM noun_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }*/
    
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
          sqlite3_bind_text(ppstmt, i, binding.get_string().c_str(), binding.get_string().length(), SQLITE_STATIC);
          
          break;
        }
      }
      
      i++;
    }
    
    /*
    for (auto adj : _derived_from_adjective)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@DERADJ"), adj._id);
    }
    
    for (auto adj : _not_derived_from_adjective)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NDERADJ"), adj._id);
    }
    
    for (auto adv : _derived_from_adverb)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@DERADV"), adv._id);
    }
    
    for (auto adv : _not_derived_from_adverb)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NDERADV"), adv._id);
    }
    
    for (auto n : _derived_from_noun)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@DERN"), n._id);
    }
    
    for (auto n : _not_derived_from_noun)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NDERN"), n._id);
    }*/

    std::list<adverb> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      adverb tnc {_data, sqlite3_column_int(ppstmt, 0)};
      tnc._base_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
      
      if (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL)
      {
        tnc._comparative_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
      }
      
      if (sqlite3_column_type(ppstmt, 3) != SQLITE_NULL)
      {
        tnc._superlative_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 3)));
      }
  
      output.push_back(tnc);
    }

    sqlite3_finalize(ppstmt);
    
    for (auto& adverb : output)
    {
      query = "SELECT pronunciation FROM adverb_pronunciations WHERE adverb_id = ?";
      if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
      }
      
      sqlite3_bind_int(ppstmt, 1, adverb._id);
      
      while (sqlite3_step(ppstmt) == SQLITE_ROW)
      {
        std::string pronunciation(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 0)));
        auto phonemes = verbly::split<std::list<std::string>>(pronunciation, " ");
        
        adverb.pronunciations.push_back(phonemes);
      }
      
      sqlite3_finalize(ppstmt);
    }

    return output;
  }

};
