#include "verbly.h"

namespace verbly {
  
  adverb::adverb(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string adverb::base_form() const
  {
    return _base_form;
  }
  
  std::string adverb::comparative_form() const
  {
    return _comparative_form;
  }
  
  std::string adverb::superlative_form() const
  {
    return _superlative_form;
  }
    
  bool adverb::has_comparative_form() const
  {
    return !_comparative_form.empty();
  }
  
  bool adverb::has_superlative_form() const
  {
    return !_superlative_form.empty();
  }
  
  adverb_query adverb::antonyms() const
  {
    return _data.adverbs().antonym_of(*this);
  }
  
  adverb_query adverb::synonyms() const
  {
    return _data.adverbs().synonym_of(*this);
  }
  
  adjective_query adverb::anti_mannernyms() const
  {
    return _data.adjectives().anti_mannernym_of(*this);
  }
  
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

  adverb_query& adverb_query::random(bool _random)
  {
    this->_random = _random;
  
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
  
  adverb_query& adverb_query::has_pronunciation(bool _has_prn)
  {
    this->_has_prn = _has_prn;
    
    return *this;
  }
  
  adverb_query& adverb_query::requires_comparative_form(bool _arg)
  {
    _requires_comparative_form = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::requires_superlative_form(bool _arg)
  {
    _requires_superlative_form = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::has_antonyms(bool _arg)
  {
    _has_antonyms = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::antonym_of(const adverb& _adv)
  {
    _antonym_of.push_back(_adv);
    
    return *this;
  }
  
  adverb_query& adverb_query::not_antonym_of(const adverb& _adv)
  {
    _not_antonym_of.push_back(_adv);
    
    return *this;
  }
  
  adverb_query& adverb_query::has_synonyms(bool _arg)
  {
    _has_synonyms = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::synonym_of(const adverb& _adv)
  {
    _synonym_of.push_back(_adv);
    
    return *this;
  }
  
  adverb_query& adverb_query::not_synonym_of(const adverb& _adv)
  {
    _not_synonym_of.push_back(_adv);
    
    return *this;
  }
  
  adverb_query& adverb_query::is_mannernymic(bool _arg)
  {
    _is_mannernymic = _arg;
    
    return *this;
  }
  
  adverb_query& adverb_query::mannernym_of(const adjective& _adj)
  {
    _mannernym_of.push_back(_adj);
    
    return *this;
  }
  
  std::list<adverb> adverb_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT adverb_id, base_form, comparative, superlative FROM adverbs";
    std::list<std::string> conditions;
    
    if (_has_prn)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_id FROM adverb_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "pronunciation LIKE @RHMPRN");
      std::string cond = "adverb_id IN (SELECT adverb_id FROM adverb_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    for (auto except : _except)
    {
      conditions.push_back("adverb_id != @EXCID");
    }
    
    if (_requires_comparative_form)
    {
      conditions.push_back("comparative IS NOT NULL");
    }
    
    if (_requires_superlative_form)
    {
      conditions.push_back("superlative IS NOT NULL");
    }
    
    if (_has_antonyms)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_2_id FROM adverb_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::list<std::string> clauses(_antonym_of.size(), "adverb_1_id = @ANTID");
      std::string cond = "adverb_id IN (SELECT adverb_2_id FROM adverb_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_antonym_of.empty())
    {
      std::list<std::string> clauses(_not_antonym_of.size(), "adverb_1_id = @NANTID");
      std::string cond = "adverb_id NOT IN (SELECT adverb_2_id FROM adverb_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_has_synonyms)
    {
      conditions.push_back("adverb_id IN (SELECT adverb_2_id FROM adverb_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::list<std::string> clauses(_synonym_of.size(), "adverb_1_id = @SYNID");
      std::string cond = "adverb_id IN (SELECT adverb_2_id FROM adverb_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_synonym_of.empty())
    {
      std::list<std::string> clauses(_not_synonym_of.size(), "adverb_1_id = @NSYNID");
      std::string cond = "adverb_id NOT IN (SELECT adverb_2_id FROM adverb_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_mannernymic)
    {
      conditions.push_back("adverb_id IN (SELECT mannernym_id FROM mannernymy)");
    }
    
    if (!_mannernym_of.empty())
    {
      std::list<std::string> clauses(_mannernym_of.size(), "adjective_id = @AMANID");
      std::string cond = "adverb_id IN (SELECT mannernym_id FROM mannernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
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
    
    if (!_rhymes.empty())
    {
      int i = 0;
      for (auto rhyme : _rhymes)
      {
        std::string rhymer = "%" + rhyme;
        sqlite3_bind_text(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@RHMPRN"), rhymer.c_str(), rhymer.length(), SQLITE_STATIC);
        
        i++;
      }
    }
    
    for (auto except : _except)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@EXCID"), except._id);
    }
    
    for (auto antonym : _antonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@ANTID"), antonym._id);
    }
    
    for (auto antonym : _not_antonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NANTID"), antonym._id);
    }
    
    for (auto synonym : _synonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@SYNID"), synonym._id);
    }
    
    for (auto synonym : _not_synonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NSYNID"), synonym._id);
    }
    
    for (auto adj : _mannernym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@AMANID"), adj._id);
    }

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
