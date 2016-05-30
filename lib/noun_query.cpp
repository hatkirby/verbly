#include "verbly.h"

namespace verbly {
  
  noun_query::noun_query(const data& _data) : _data(_data)
  {
    
  }
  
  noun_query& noun_query::limit(int _limit)
  {
    if ((_limit > 0) || (_limit == unlimited))
    {
      this->_limit = _limit;
    }
  
    return *this;
  }

  noun_query& noun_query::random()
  {
    this->_random = true;
  
    return *this;
  }
  
  noun_query& noun_query::except(const noun& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  noun_query& noun_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.get_rhymes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const noun*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const noun&>(_word));
    }
    
    return *this;
  }
  
  noun_query& noun_query::rhymes_with(rhyme _r)
  {
    _rhymes.push_back(_r);
    
    return *this;
  }
  
  noun_query& noun_query::has_pronunciation()
  {
    this->_has_prn = true;
    
    return *this;
  }
  
  noun_query& noun_query::has_rhyming_noun()
  {
    _has_rhyming_noun = true;
    
    return *this;
  }
  
  noun_query& noun_query::has_rhyming_adjective()
  {
    _has_rhyming_adjective = true;
    
    return *this;
  }
  
  noun_query& noun_query::has_rhyming_adverb()
  {
    _has_rhyming_adverb = true;
    
    return *this;
  }
  
  noun_query& noun_query::has_rhyming_verb()
  {
    _has_rhyming_verb = true;
    
    return *this;
  }
  
  noun_query& noun_query::with_stress(filter<std::vector<bool>> _arg)
  {
    _stress = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::with_singular_form(std::string _arg)
  {
    _with_singular_form.push_back(_arg);
    
    return *this;
  }
  
  noun_query& noun_query::with_prefix(filter<std::string> _f)
  {
    _f.clean();
    _with_prefix = _f;
    
    return *this;
  }
  
  noun_query& noun_query::with_suffix(filter<std::string> _f)
  {
    _f.clean();
    _with_suffix = _f;
    
    return *this;
  }
  
  noun_query& noun_query::requires_plural_form()
  {
    _requires_plural_form = true;
    
    return *this;
  }
  
  noun_query& noun_query::with_complexity(int _arg)
  {
    _with_complexity = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::is_hypernym()
  {
    _is_hypernym = true;
    
    return *this;
  }
  
  noun_query& noun_query::hypernym_of(filter<noun> _f)
  {
    _f.clean();
    _hypernym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_hypernym_of(filter<noun> _f)
  {
    _f.clean();
    _full_hypernym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_hyponym()
  {
    _is_hyponym = true;
    
    return *this;
  }
  
  noun_query& noun_query::hyponym_of(filter<noun> _f)
  {
    _f.clean();
    _hyponym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_hyponym_of(filter<noun> _f)
  {
    _f.clean();
    _full_hyponym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_part_meronym()
  {
    _is_part_meronym = true;
    
    return *this;
  }
  
  noun_query& noun_query::part_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _part_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_part_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _full_part_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_part_holonym()
  {
    _is_part_holonym = true;
    
    return *this;
  }
  
  noun_query& noun_query::part_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _part_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_part_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _full_part_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_substance_meronym()
  {
    _is_substance_meronym = true;
    
    return *this;
  }
  
  noun_query& noun_query::substance_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _substance_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_substance_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _full_substance_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_substance_holonym()
  {
    _is_substance_holonym = true;
    
    return *this;
  }
  
  noun_query& noun_query::substance_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _substance_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_substance_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _full_substance_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_member_meronym()
  {
    _is_member_meronym = true;
    
    return *this;
  }
  
  noun_query& noun_query::member_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _member_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_member_meronym_of(filter<noun> _f)
  {
    _f.clean();
    _full_member_meronym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_member_holonym()
  {
    _is_member_holonym = true;
    
    return *this;
  }
  
  noun_query& noun_query::member_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _member_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::full_member_holonym_of(filter<noun> _f)
  {
    _f.clean();
    _full_member_holonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_proper()
  {
    _is_proper = true;
    
    return *this;
  }
  
  noun_query& noun_query::is_not_proper()
  {
    _is_not_proper = true;
    
    return *this;
  }
  
  noun_query& noun_query::is_instance()
  {
    _is_instance = true;
    
    return *this;
  }
  
  noun_query& noun_query::instance_of(filter<noun> _f)
  {
    _f.clean();
    _instance_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_class()
  {
    _is_class = true;
    
    return *this;
  }
  
  noun_query& noun_query::class_of(filter<noun> _f)
  {
    _f.clean();
    _class_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::has_synonyms()
  {
    _has_synonyms = true;
    
    return *this;
  }
  
  noun_query& noun_query::synonym_of(filter<noun> _f)
  {
    _f.clean();
    _synonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::has_antonyms()
  {
    _has_antonyms = true;
    
    return *this;
  }
  
  noun_query& noun_query::antonym_of(filter<noun> _f)
  {
    _f.clean();
    _antonym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::has_pertainym()
  {
    _has_pertainym = true;
    
    return *this;
  }
  
  noun_query& noun_query::anti_pertainym_of(filter<adjective> _f)
  {
    _f.clean();
    _anti_pertainym_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::is_attribute()
  {
    _is_attribute = true;
    
    return *this;
  }
  
  noun_query& noun_query::attribute_of(filter<adjective> _f)
  {
    _f.clean();
    _attribute_of = _f;
    
    return *this;
  }
  
  noun_query& noun_query::at_least_n_images(int _arg)
  {
    _at_least_n_images = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::with_wnid(int _arg)
  {
    _with_wnid.insert(_arg);
    
    return *this;
  }
  
  /*
  noun_query& noun_query::derived_from(const word& _w)
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
  
  noun_query& noun_query::not_derived_from(const word& _w)
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
  }*/
  
  std::list<noun> noun_query::run() const
  {
    std::stringstream construct;
    
    if (!_full_hypernym_of.empty() || !_full_hyponym_of.empty() || !_full_part_meronym_of.empty() || !_full_part_holonym_of.empty() || !_full_substance_meronym_of.empty() || !_full_substance_holonym_of.empty() || !_full_member_meronym_of.empty() || !_full_member_holonym_of.empty())
    {
      construct << "WITH RECURSIVE ";
      
      std::list<std::string> ctes;
      
      for (auto hyponym : _full_hypernym_of.uniq_flatten())
      {
        ctes.push_back("hypernym_tree_" + std::to_string(hyponym._id) + " AS (SELECT hypernym_id FROM hypernymy WHERE hyponym_id = " + std::to_string(hyponym._id) + " UNION SELECT h.hypernym_id FROM hypernym_tree_" + std::to_string(hyponym._id) + " AS t INNER JOIN hypernymy AS h ON t.hypernym_id = h.hyponym_id)");
      }
      
      for (auto hypernym : _full_hyponym_of.uniq_flatten())
      {
        ctes.push_back("hyponym_tree_" + std::to_string(hypernym._id) + " AS (SELECT hyponym_id FROM hypernymy WHERE hypernym_id = " + std::to_string(hypernym._id) + " UNION SELECT h.hyponym_id FROM hyponym_tree_" + std::to_string(hypernym._id) + " AS t INNER JOIN hypernymy AS h ON t.hyponym_id = h.hypernym_id)");
      }
      
      for (auto holonym : _full_part_meronym_of.uniq_flatten())
      {
        ctes.push_back("part_meronym_tree_" + std::to_string(holonym._id) + " AS (SELECT meronym_id FROM part_meronymy WHERE holonym_id = " + std::to_string(holonym._id) + " UNION SELECT h.meronym_id FROM part_meronym_tree_" + std::to_string(holonym._id) + " AS t INNER JOIN part_meronymy AS h ON t.meronym_id = h.holonym_id)");
      }
      
      for (auto meronym : _full_part_holonym_of.uniq_flatten())
      {
        ctes.push_back("part_holonym_tree_" + std::to_string(meronym._id) + " AS (SELECT holonym_id FROM part_meronymy WHERE meronym_id = " + std::to_string(meronym._id) + " UNION SELECT h.holonym_id FROM part_holonym_tree_" + std::to_string(meronym._id) + " AS t INNER JOIN part_meronymy AS h ON t.holonym_id = h.meronym_id)");
      }
      
      for (auto holonym : _full_substance_meronym_of.uniq_flatten())
      {
        ctes.push_back("substance_meronym_tree_" + std::to_string(holonym._id) + " AS (SELECT meronym_id FROM substance_meronymy WHERE holonym_id = " + std::to_string(holonym._id) + " UNION SELECT h.meronym_id FROM substance_meronym_tree_" + std::to_string(holonym._id) + " AS t INNER JOIN substance_meronymy AS h ON t.meronym_id = h.holonym_id)");
      }
      
      for (auto meronym : _full_substance_holonym_of.uniq_flatten())
      {
        ctes.push_back("substance_holonym_tree_" + std::to_string(meronym._id) + " AS (SELECT holonym_id FROM substance_meronymy WHERE meronym_id = " + std::to_string(meronym._id) + " UNION SELECT h.holonym_id FROM substance_holonym_tree_" + std::to_string(meronym._id) + " AS t INNER JOIN substance_meronymy AS h ON t.holonym_id = h.meronym_id)");
      }
      
      for (auto holonym : _full_member_meronym_of.uniq_flatten())
      {
        ctes.push_back("member_meronym_tree_" + std::to_string(holonym._id) + " AS (SELECT meronym_id FROM member_meronymy WHERE holonym_id = " + std::to_string(holonym._id) + " UNION SELECT h.meronym_id FROM member_meronym_tree_" + std::to_string(holonym._id) + " AS t INNER JOIN member_meronymy AS h ON t.meronym_id = h.holonym_id)");
      }
      
      for (auto meronym : _full_member_holonym_of.uniq_flatten())
      {
        ctes.push_back("member_holonym_tree_" + std::to_string(meronym._id) + " AS (SELECT holonym_id FROM member_meronymy WHERE meronym_id = " + std::to_string(meronym._id) + " UNION SELECT h.holonym_id FROM member_holonym_tree_" + std::to_string(meronym._id) + " AS t INNER JOIN member_meronymy AS h ON t.holonym_id = h.meronym_id)");
      }
      
      construct << verbly::implode(std::begin(ctes), std::end(ctes), ", ");
      construct << " ";
    }
    
    construct << "SELECT noun_id, singular, plural, wnid FROM nouns";
    std::list<std::string> conditions;
    std::list<binding> bindings;
    
    if (_has_prn)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM noun_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "(prerhyme != ? AND rhyme = ?)");
      std::string cond = "noun_id IN (SELECT noun_id FROM noun_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
      
      for (auto rhy : _rhymes)
      {
        bindings.emplace_back(rhy.get_prerhyme());
        bindings.emplace_back(rhy.get_rhyme());
      }
    }
    
    if (_has_rhyming_noun)
    {
      conditions.push_back("noun_id IN (SELECT a.noun_id FROM nouns AS a INNER JOIN noun_pronunciations AS curp ON curp.noun_id = a.noun_id INNER JOIN noun_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme AND rhmp.noun_id != curp.noun_id)");
    }
    
    if (_has_rhyming_adjective)
    {
      conditions.push_back("noun_id IN (SELECT a.noun_id FROM nouns AS a INNER JOIN noun_pronunciations AS curp ON curp.noun_id = a.noun_id INNER JOIN adjective_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_adverb)
    {
      conditions.push_back("noun_id IN (SELECT a.noun_id FROM nouns AS a INNER JOIN noun_pronunciations AS curp ON curp.noun_id = a.noun_id INNER JOIN adverb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (_has_rhyming_verb)
    {
      conditions.push_back("noun_id IN (SELECT a.noun_id FROM nouns AS a INNER JOIN noun_pronunciations AS curp ON curp.noun_id = a.noun_id INNER JOIN verb_pronunciations AS rhmp ON rhmp.prerhyme != curp.prerhyme AND rhmp.rhyme = curp.rhyme)");
    }
    
    if (!_stress.empty())
    {
      std::stringstream cond;
      if (_stress.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT noun_id FROM noun_pronunciations WHERE ";
      
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
      conditions.push_back("noun_id != ?");
      bindings.emplace_back(except._id);
    }
    
    if (!_with_singular_form.empty())
    {
      std::list<std::string> clauses(_with_singular_form.size(), "singular = ?");
      std::string cond = "(" + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
      
      for (auto form : _with_singular_form)
      {
        bindings.emplace_back(form);
      }
    }
    
    if (_requires_plural_form)
    {
      conditions.push_back("plural IS NOT NULL");
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
              return "singular LIKE ?";
            } else {
              return "singular NOT LIKE ?";
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
              return "singular LIKE ?";
            } else {
              return "singular NOT LIKE ?";
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
    
    if (_is_hypernym)
    {
      conditions.push_back("noun_id IN (SELECT hypernym_id FROM hypernymy)");
    }
    
    if (!_hypernym_of.empty())
    {
      std::stringstream cond;
      if (_hypernym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT hypernym_id FROM hypernymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "hyponym_id = ?";
            } else {
              return "hyponym_id != ?";
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
      
      cond << recur(_hypernym_of, _hypernym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_hypernym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT hypernym_id FROM hypernym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT hypernym_id FROM hypernym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_hypernym_of, false));
    }
    
    if (!_full_hyponym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT hyponym_id FROM hyponym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT hyponym_id FROM hyponym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_hyponym_of, false));
    }
    
    if (_is_hyponym)
    {
      conditions.push_back("noun_id IN (SELECT hyponym_id FROM hypernymy)");
    }
    
    if (!_hyponym_of.empty())
    {
      std::stringstream cond;
      if (_hyponym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT hyponym_id FROM hypernymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "hypernym_id = ?";
            } else {
              return "hypernym_id != ?";
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
      
      cond << recur(_hyponym_of, _hyponym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_part_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM part_meronymy)");
    }
    
    if (!_part_meronym_of.empty())
    {
      std::stringstream cond;
      if (_part_meronym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT meronym_id FROM part_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "holonym_id = ?";
            } else {
              return "holonym_id != ?";
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
      
      cond << recur(_part_meronym_of, _part_meronym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_part_meronym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT meronym_id FROM part_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT meronym_id FROM part_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_part_meronym_of, false));
    }
    
    if (_is_part_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM part_meronymy)");
    }
    
    if (!_part_holonym_of.empty())
    {
      std::stringstream cond;
      if (_part_holonym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT holonym_id FROM part_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "meronym_id = ?";
            } else {
              return "meronym_id != ?";
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
      
      cond << recur(_part_holonym_of, _part_holonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_part_holonym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT holonym_id FROM part_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT holonym_id FROM part_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_part_holonym_of, false));
    }
    
    if (_is_substance_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM substance_meronymy)");
    }
    
    if (!_substance_meronym_of.empty())
    {
      std::stringstream cond;
      if (_substance_meronym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT meronym_id FROM substance_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "holonym_id = ?";
            } else {
              return "holonym_id != ?";
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
      
      cond << recur(_substance_meronym_of, _substance_meronym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_substance_meronym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT meronym_id FROM substance_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT meronym_id FROM substance_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_substance_meronym_of, false));
    }
    
    if (_is_substance_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM substance_meronymy)");
    }
    
    if (!_substance_holonym_of.empty())
    {
      std::stringstream cond;
      if (_substance_holonym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT holonym_id FROM substance_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "meronym_id = ?";
            } else {
              return "meronym_id != ?";
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
      
      cond << recur(_substance_holonym_of, _substance_holonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_substance_holonym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT holonym_id FROM substance_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT holonym_id FROM substance_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_substance_holonym_of, false));
    }
    
    if (_is_member_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM member_meronymy)");
    }
    
    if (!_member_meronym_of.empty())
    {
      std::stringstream cond;
      if (_member_meronym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT meronym_id FROM member_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "holonym_id = ?";
            } else {
              return "holonym_id != ?";
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
      
      cond << recur(_member_meronym_of, _member_meronym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_member_meronym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT meronym_id FROM member_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT meronym_id FROM member_meronym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_member_meronym_of, false));
    }
    
    if (_is_member_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM member_meronym)");
    }
    
    if (!_member_holonym_of.empty())
    {
      std::stringstream cond;
      if (_member_holonym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT holonym_id FROM member_meronymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "meronym_id = ?";
            } else {
              return "meronym_id != ?";
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
      
      cond << recur(_member_holonym_of, _member_holonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (!_full_member_holonym_of.empty())
    {
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            if (notlogic == f.get_notlogic())
            {
              return "noun_id IN (SELECT holonym_id FROM member_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
            } else {
              return "noun_id NOT IN (SELECT holonym_id FROM member_holonym_tree_" + std::to_string(f.get_elem()._id) + ")";
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
      
      conditions.push_back(recur(_full_member_holonym_of, false));
    }
    
    if (_is_proper)
    {
      conditions.push_back("proper = 1");
    }
    
    if (_is_not_proper)
    {
      conditions.push_back("proper = 0");
    }
    
    if (_is_instance)
    {
      conditions.push_back("noun_id IN (SELECT instance_id FROM instantiation)");
    }
    
    if (!_instance_of.empty())
    {
      std::stringstream cond;
      if (_instance_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT instance_id FROM instantiation WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "class_id = ?";
            } else {
              return "class_id != ?";
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
      
      cond << recur(_instance_of, _instance_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_class)
    {
      conditions.push_back("noun_id IN (SELECT class_id FROM instantiation)");
    }
    
    if (!_class_of.empty())
    {
      std::stringstream cond;
      if (_class_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT class_id FROM instantiation WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "instance_id = ?";
            } else {
              return "instance_id != ?";
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
      
      cond << recur(_class_of, _class_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_has_synonyms)
    {
      conditions.push_back("noun_id IN (SELECT noun_2_id FROM noun_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::stringstream cond;
      if (_synonym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT noun_2_id FROM noun_synonymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "noun_1_id = ?";
            } else {
              return "noun_1_id != ?";
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
      
      cond << recur(_synonym_of, _synonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_has_antonyms)
    {
      conditions.push_back("noun_id IN (SELECT noun_2_id FROM noun_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::stringstream cond;
      if (_antonym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT noun_2_id FROM noun_antonymy WHERE ";
      
      std::function<std::string (filter<noun>, bool)> recur = [&] (filter<noun> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<noun>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "noun_1_id = ?";
            } else {
              return "noun_1_id != ?";
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
      
      cond << recur(_antonym_of, _antonym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_has_pertainym)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM pertainymy)");
    }
    
    if (!_anti_pertainym_of.empty())
    {
      std::stringstream cond;
      if (_anti_pertainym_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT noun_id FROM pertainymy WHERE ";
      
      std::function<std::string (filter<adjective>, bool)> recur = [&] (filter<adjective> f, bool notlogic) -> std::string {
        switch (f.get_type())
        {
          case filter<adjective>::type::singleton:
          {
            bindings.emplace_back(f.get_elem()._id);
            
            if (notlogic == f.get_notlogic())
            {
              return "pertainym_id = ?";
            } else {
              return "pertainym_id != ?";
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
      
      cond << recur(_anti_pertainym_of, _anti_pertainym_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_is_attribute)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM variation)");
    }
    
    if (!_attribute_of.empty())
    {
      std::stringstream cond;
      if (_attribute_of.get_notlogic())
      {
        cond << "noun_id NOT IN";
      } else {
        cond << "noun_id IN";
      }
      
      cond << "(SELECT noun_id FROM variation WHERE ";
      
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
      
      cond << recur(_attribute_of, _attribute_of.get_notlogic());
      cond << ")";
      conditions.push_back(cond.str());
    }
    
    if (_at_least_n_images != unlimited)
    {
      conditions.push_back("images >= ?");
      bindings.emplace_back(_at_least_n_images);
    }
    
    if (!_with_wnid.empty())
    {
      std::vector<std::string> clauses(_with_wnid.size(), "wnid = ?");
      std::string cond = verbly::implode(std::begin(clauses), std::end(clauses), " OR ");
      conditions.push_back("(" + cond + ")");
      
      for (auto wnid : _with_wnid)
      {
        bindings.emplace_back(wnid);
      }
    }
    
    /*
    if (!_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_derived_from_adjective.size(), "adjective_id = @DERADJ");
      std::string cond = "noun_id IN (SELECT noun_id FROM noun_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adjective.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adjective.size(), "adjective_id = @NDERADJ");
      std::string cond = "noun_id NOT IN (SELECT noun_id FROM noun_adjective_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_derived_from_adverb.size(), "adverb_id = @DERADV");
      std::string cond = "noun_id IN (SELECT noun_id FROM noun_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_adverb.empty())
    {
      std::list<std::string> clauses(_not_derived_from_adverb.size(), "adverb_id = @NDERADV");
      std::string cond = "noun_id NOT IN (SELECT noun_id FROM noun_adverb_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_derived_from_noun.size(), "noun_2_id = @DERN");
      std::string cond = "noun_id IN (SELECT noun_1_id FROM noun_noun_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_derived_from_noun.empty())
    {
      std::list<std::string> clauses(_not_derived_from_noun.size(), "noun_2_id = @NDERN");
      std::string cond = "noun_id NOT IN (SELECT noun_1_id FROM noun_noun_derivation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    */
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
    std::list<noun> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      noun tnc {_data, sqlite3_column_int(ppstmt, 0)};
      tnc._singular = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
      
      if (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL)
      {
        tnc._plural = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
      }
      
      tnc._wnid = sqlite3_column_int(ppstmt, 3);
  
      output.push_back(tnc);
    }

    sqlite3_finalize(ppstmt);
    
    for (auto& noun : output)
    {
      query = "SELECT pronunciation, prerhyme, rhyme FROM noun_pronunciations WHERE noun_id = ?";
      if (sqlite3_prepare_v2(_data.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
      {
        throw std::runtime_error(sqlite3_errmsg(_data.ppdb));
      }
      
      sqlite3_bind_int(ppstmt, 1, noun._id);
      
      while (sqlite3_step(ppstmt) == SQLITE_ROW)
      {
        std::string pronunciation(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 0)));
        auto phonemes = verbly::split<std::list<std::string>>(pronunciation, " ");
        
        noun.pronunciations.push_back(phonemes);
        
        if ((sqlite3_column_type(ppstmt, 1) != SQLITE_NULL) && (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL))
        {
          std::string prerhyme(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
          std::string rhyming(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
          
          noun.rhymes.emplace_back(prerhyme, rhyming);
        }
      }
      
      sqlite3_finalize(ppstmt);
    }

    return output;
  }

};
