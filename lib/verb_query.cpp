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
    for (auto rhyme : _word.rhyme_phonemes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const verb*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const verb&>(_word));
    }
    
    return *this;
  }
  
  verb_query& verb_query::has_pronunciation()
  {
    this->_has_prn = true;
    
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
    
    if (_has_prn)
    {
      conditions.push_back("verb_id IN (SELECT verb_id FROM verb_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "pronunciation LIKE @RHMPRN");
      std::string cond = "verb_id IN (SELECT verb_id FROM verb_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    for (auto except : _except)
    {
      conditions.push_back("verb_id != @EXCID");
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
      query = "SELECT pronunciation FROM verb_pronunciations WHERE verb_id = ?";
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
      }
      
      sqlite3_finalize(ppstmt);
    }
    
    return output;
  }

};
