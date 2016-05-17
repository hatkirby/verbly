#include "verbly.h"

namespace verbly {

  adjective_query::adjective_query(const data& _data) : _data(_data)
  {

  }
  
  adjective_query& adjective_query::limit(int _limit)
  {
    if ((_limit > 0) || (_limit == unlimited))
    {
      this->_limit = _limit;
    }
  
    return *this;
  }

  adjective_query& adjective_query::random()
  {
    this->_random = true;
  
    return *this;
  }
  
  adjective_query& adjective_query::except(const adjective& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  adjective_query& adjective_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.get_rhymes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const adjective*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const adjective&>(_word));
    }
    
    return *this;
  }
  
  adjective_query& adjective_query::rhymes_with(rhyme _r)
  {
    _rhymes.push_back(_r);
    
    return *this;
  }
  
  adjective_query& adjective_query::has_pronunciation()
  {
    this->_has_prn = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::has_rhyming_noun()
  {
    _has_rhyming_noun = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::has_rhyming_adjective()
  {
    _has_rhyming_adjective = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::has_rhyming_adverb()
  {
    _has_rhyming_adverb = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::has_rhyming_verb()
  {
    _has_rhyming_verb = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::with_prefix(filter<std::string> _f)
  {
    _f.clean();
    _with_prefix = _f;
    
    return *this;
  }
  
  adjective_query& adjective_query::with_suffix(filter<std::string> _f)
  {
    _f.clean();
    _with_suffix = _f;
    
    return *this;
  }
  
  adjective_query& adjective_query::with_complexity(int _arg)
  {
    _with_complexity = _arg;
    
    return *this;
  }
  
  adjective_query& adjective_query::requires_comparative_form()
  {
    _requires_comparative_form = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::requires_superlative_form()
  {
    _requires_superlative_form = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::position(adjective::positioning pos)
  {
    _position = pos;
    
    return *this;
  }
  
  adjective_query& adjective_query::is_variant()
  {
    this->_is_variant = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::variant_of(filter<noun> _f)
  {
    _f.clean();
    _variant_of = _f;
    
    return *this;
  }
    
  adjective_query& adjective_query::has_antonyms()
  {
    this->_is_antonymic = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::antonym_of(filter<adjective> _f)
  {
    _f.clean();
    _antonym_of = _f;
    
    return *this;
  }
    
  adjective_query& adjective_query::has_synonyms()
  {
    this->_is_synonymic = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::synonym_of(filter<adjective> _f)
  {
    _f.clean();
    _synonym_of = _f;
    
    return *this;
  }

  adjective_query& adjective_query::is_generalization()
  {
    this->_is_generalization = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::generalization_of(filter<adjective> _f)
  {
    _f.clean();
    _generalization_of = _f;
    
    return *this;
  }
  
  adjective_query& adjective_query::is_specification()
  {
    this->_is_specification = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::specification_of(filter<adjective> _f)
  {
    _f.clean();
    _specification_of = _f;
    
    return *this;
  }
  
  adjective_query& adjective_query::is_pertainymic()
  {
    this->_is_pertainymic = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::pertainym_of(filter<noun> _f)
  {
    _f.clean();
    _pertainym_of = _f;
    
    return *this;
  }
  
  adjective_query& adjective_query::is_mannernymic()
  {
    this->_is_mannernymic = true;
    
    return *this;
  }
  
  adjective_query& adjective_query::anti_mannernym_of(filter<adverb> _f)
  {
    _f.clean();
    _anti_mannernym_of = _f;
    
    return *this;
  }
  /*
  adjective_query& adjective_query::derived_from(const word& _w)
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
  
  adjective_query& adjective_query::not_derived_from(const word& _w)
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
  std::list<adjective> adjective_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT adjective_id, base_form, comparative, superlative, position FROM adjectives";
    std::list<std::string> conditions;
    std::list<binding> bindings;
    
    if (_has_prn)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM adjective_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "(prerhyme != ? AND rhyme = ?)");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM adjective_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
      
      for (auto rhy : _rhymes)
      {
        bindings.emplace_back(rhy.get_prerhyme());
        bindings.emplace_back(rhy.get_rhyme());
      }
    }
    
    if (_has_rhyming_noun)
    {
      conditions.push_back("adjective_id IN (SELECT a.adjective_id FROM adjectives AS a INNER JOIN adjective_pronunciations AS curp ON curp.adjective_id = a.adjective_id INNER JOIN noun_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_adjective)
    {
      conditions.push_back("adjective_id IN (SELECT a.adjective_id FROM adjectives AS a INNER JOIN adjective_pronunciations AS curp ON curp.adjective_id = a.adjective_id INNER JOIN adjective_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme AND rhmp.adjective_id != curp.adjective_id)");
    }
    
    if (_has_rhyming_adverb)
    {
      conditions.push_back("adjective_id IN (SELECT a.adjective_id FROM adjectives AS a INNER JOIN adjective_pronunciations AS curp ON curp.adjective_id = a.adjective_id INNER JOIN adverb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_verb)
    {
      conditions.push_back("adjective_id IN (SELECT a.adjective_id FROM adjectives AS a INNER JOIN adjective_pronunciations AS curp ON curp.adjective_id = a.adjective_id INNER JOIN verb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    for (auto except : _except)
    {
      conditions.push_back("adjective_id != ?");
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
    
    switch (_position)
    {
      case adjective::positioning::predicate: conditions.push_back("position = 'p'"); break;
      case adjective::positioning::attributive: conditions.push_back("position = 'a'"); break;
      case adjective::positioning::postnominal: conditions.push_back("position = 'i'"); break;
      case adjective::positioning::undefined: break;
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
    
    if (_is_variant)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM variation)");
    }
    
    if (!_variant_of.empty())
    {
      std::stringstream cond;
      if (_variant_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT adjective_id FROM variation WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "noun_id = ?";
            } else {
              return "noun_id != ?";
            }
          }
          
          case filter<noun>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<noun> f2) {
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
      
      cond << recur(_variant_of, _variant_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_antonymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_2_id FROM adjective_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::stringstream cond;
      if (_antonym_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT adjective_2_id FROM adjective_antonymy WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "adjective_1_id = ?";
            } else {
              return "adjective_1_id != ?";
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
      
      cond << recur(_antonym_of, _antonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_synonymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_2_id FROM adjective_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::stringstream cond;
      if (_synonym_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT adjective_2_id FROM adjective_synonymy WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "adjective_1_id = ?";
            } else {
              return "adjective_1_id != ?";
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
      
      cond << recur(_synonym_of, _synonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_generalization)
    {
      conditions.push_back("adjective_id IN (SELECT general_id FROM specification)");
    }
    
    if (!_generalization_of.empty())
    {
      std::stringstream cond;
      if (_generalization_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT general_id FROM specification WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "specific_id = ?";
            } else {
              return "specific_id != ?";
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
      
      cond << recur(_generalization_of, _generalization_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_specification)
    {
      conditions.push_back("adjective_id IN (SELECT specific_id FROM specification)");
    }
    
    if (!_specification_of.empty())
    {
      std::stringstream cond;
      if (_specification_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT specific_id FROM specification WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "general_id = ?";
            } else {
              return "general_id != ?";
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
      
      cond << recur(_specification_of, _specification_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_pertainymic)
    {
      conditions.push_back("adjective_id IN (SELECT pertainym_id FROM pertainymy)");
    }
    
    if (!_pertainym_of.empty())
    {
      std::stringstream cond;
      if (_pertainym_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT pertainym_id FROM pertainymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "noun_id = ?";
            } else {
              return "noun_id != ?";
            }
          }
          
          case filter<noun>::type::group:
          {
            bool truelogic = notlogic != f.get_notlogic();
            
            std::list<std::string> clauses;
            std::transform(std::begin(f), std::end(f), std::back_inserter(clauses), [&] (filter<noun> f2) {
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
      
      cond << recur(_pertainym_of, _pertainym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_mannernymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM mannernymy)");
    }
    
    if (!_anti_mannernym_of.empty())
    {
      std::stringstream cond;
      if (_anti_mannernym_of.get_notlogic())
      {
        cond << "adjective_id NOT IN";
      } else {
        cond << "adjective_id IN";
      }
      
      cond << "(SELECT adjective_id FROM mannernymy WHERE ";
      
      std::function<std::string (filter<adverb>, bool)> recur = [&] (filter<adverb> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adverb>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "mannernym_id = ?";
            } else {
              return "mannernym_id != ?";
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
      
      cond << recur(_anti_mannernym_of, _anti_mannernym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
/*    
    if (!_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_derived_from_adjective.size(), "adjective_2_id = @DERADJ");
      std::string cond = "adjective_id IN (SELECT adjective_1_id FROM adjective_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adjective.size(), "adjective_2_id = @NDERADJ");
      std::string cond = "adjective_id NOT IN (SELECT adjective_1_id FROM adjective_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_derived_from_adverb.size(), "adverb_id = @DERADV");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM adjective_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adverb.size(), "adverb_id = @NDERADV");
      std::string cond = "adjective_id NOT IN (SELECT adjective_id FROM adjective_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_derived_from_noun.size(), "noun_id = @DERN");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM noun_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_not_derived_from_noun.size(), "noun_id = @NDERN");
      std::string cond = "adjective_id NOT IN (SELECT adjective_id FROM noun_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
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
          sqlite3_bind_text(ppstmt, i, binding.get_string().c_str(), binding.get_string().length(), SQLITE_TRANSIENT);
          
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
    }
*/
    std::list<adjective> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      adjective tnc {_data, sqlite3_column_int(ppstmt, 0)};
      tnc._base_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
      
      if (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL)
      {
        tnc._comparative_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
      }
      
      if (sqlite3_column_type(ppstmt, 3) != SQLITE_NULL)
      {
        tnc._superlative_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 3)));
      }
      
      if (sqlite3_column_type(ppstmt, 4) != SQLITE_NULL)
      {
        std::string adjpos(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 4)));
        if (adjpos == "p")
        {
          tnc._position = adjective::positioning::predicate;
        } else if (adjpos == "a")
        {
          tnc._position = adjective::positioning::attributive;
        } else if (adjpos == "i")
        {
          tnc._position = adjective::positioning::postnominal;
        }
      }
  
      output.push_back(tnc);
    }

    sqlite3_finalize(ppstmt);
    
    for (auto& adjective : output)
    {
      query = "SELECT pronunciation, prerhyme, rhyme FROM adjective_pronunciations WHERE adjective_id = ?";
      if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
      }
      
      sqlite3_bind_int(ppstmt, 1, adjective._id);
      
      while (sqlite3_step(ppstmt) == SQLITE_ROW)
      {
        std::string pronunciation(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 0)));
        auto phonemes = verbly::split<std::list<std::string>>(pronunciation, " ");
        
        adjective.pronunciations.push_back(phonemes);
        
        if ((sqlite3_column_type(ppstmt, 1) != SQLITE_NULL) && (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL))
        {
          std::string prerhyme(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
          std::string rhyming(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
          adjective.rhymes.emplace_back(prerhyme, rhyming);
        }
      }
      
      sqlite3_finalize(ppstmt);
    }

    return output;
  }

};
