#include "verbly.h"

namespace verbly {
  
  adjective::adjective(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string adjective::base_form() const
  {
    return _base_form;
  }
  
  std::string adjective::comparative_form() const
  {
    return _comparative_form;
  }
  
  std::string adjective::superlative_form() const
  {
    return _superlative_form;
  }
  
  adjective::positioning adjective::position() const
  {
    return _position;
  }
  
  bool adjective::has_comparative_form() const
  {
    return !_comparative_form.empty();
  }
  
  bool adjective::has_superlative_form() const
  {
    return !_superlative_form.empty();
  }
  
  bool adjective::has_position() const
  {
    return _position != adjective::positioning::undefined;
  }
  
  adjective_query adjective::antonyms() const
  {
    return _data.adjectives().antonym_of(*this);
  }
  
  adjective_query adjective::synonyms() const
  {
    return _data.adjectives().synonym_of(*this);
  }
  
  adjective_query adjective::generalizations() const
  {
    return _data.adjectives().generalization_of(*this);
  }
  
  adjective_query adjective::specifications() const
  {
    return _data.adjectives().specification_of(*this);
  }
  
  noun_query adjective::anti_pertainyms() const
  {
    return _data.nouns().anti_pertainym_of(*this);
  }
  
  adverb_query adjective::mannernyms() const
  {
    return _data.adverbs().mannernym_of(*this);
  }
  
  noun_query adjective::attributes() const
  {
    return _data.nouns().attribute_of(*this);
  }
  
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

  adjective_query& adjective_query::random(bool _random)
  {
    this->_random = _random;
  
    return *this;
  }
  
  adjective_query& adjective_query::except(const adjective& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  adjective_query& adjective_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.rhyme_phonemes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const adjective*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const adjective&>(_word));
    }
    
    return *this;
  }
  
  adjective_query& adjective_query::has_pronunciation(bool _has_prn)
  {
    this->_has_prn = _has_prn;
    
    return *this;
  }
  
  adjective_query& adjective_query::is_variant(bool _is_variant)
  {
    this->_is_variant = _is_variant;
    
    return *this;
  }
  
  adjective_query& adjective_query::variant_of(const noun& _noun)
  {
    _variant_of.push_back(_noun);
    
    return *this;
  }
  
  adjective_query& adjective_query::not_variant_of(const noun& _noun)
  {
    _not_variant_of.push_back(_noun);
    
    return *this;
  }
  
  adjective_query& adjective_query::has_antonyms(bool _is_antonymic)
  {
    this->_is_antonymic = _is_antonymic;
    
    return *this;
  }
  
  adjective_query& adjective_query::antonym_of(const adjective& _adj)
  {
    _antonym_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::not_antonym_of(const adjective& _adj)
  {
    _not_antonym_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::has_synonyms(bool _is_synonymic)
  {
    this->_is_synonymic = _is_synonymic;
    
    return *this;
  }
  
  adjective_query& adjective_query::synonym_of(const adjective& _adj)
  {
    _synonym_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::not_synonym_of(const adjective& _adj)
  {
    _not_synonym_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::is_generalization(bool _is_generalization)
  {
    this->_is_generalization = _is_generalization;
    
    return *this;
  }
  
  adjective_query& adjective_query::generalization_of(const adjective& _adj)
  {
    _generalization_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::not_generalization_of(const adjective& _adj)
  {
    _not_generalization_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::is_specification(bool _is_specification)
  {
    this->_is_specification = _is_specification;
    
    return *this;
  }
  
  adjective_query& adjective_query::specification_of(const adjective& _adj)
  {
    _specification_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::not_specification_of(const adjective& _adj)
  {
    _not_specification_of.push_back(_adj);
    
    return *this;
  }
  
  adjective_query& adjective_query::is_pertainymic(bool _is_pertainymic)
  {
    this->_is_pertainymic = _is_pertainymic;
    
    return *this;
  }
  
  adjective_query& adjective_query::pertainym_of(const noun& _noun)
  {
    _pertainym_of.push_back(_noun);
    
    return *this;
  }
  
  adjective_query& adjective_query::is_mannernymic(bool _is_mannernymic)
  {
    this->_is_mannernymic = _is_mannernymic;
    
    return *this;
  }
  
  adjective_query& adjective_query::anti_mannernym_of(const adverb& _adv)
  {
    _anti_mannernym_of.push_back(_adv);
    
    return *this;
  }
  
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
  
  std::list<adjective> adjective_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT adjective_id, base_form, comparative, superlative, position FROM adjectives";
    std::list<std::string> conditions;
    
    if (_has_prn)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM adjective_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "pronunciation LIKE @RHMPRN");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM adjective_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    for (auto except : _except)
    {
      conditions.push_back("adjective_id != @EXCID");
    }
    
    if (_requires_comparative_form)
    {
      conditions.push_back("comparative IS NOT NULL");
    }
    
    if (_requires_superlative_form)
    {
      conditions.push_back("superlative IS NOT NULL");
    }
    
    if (_position != adjective::positioning::undefined)
    {
      switch (_position)
      {
        case adjective::positioning::predicate: conditions.push_back("position = 'p'"); break;
        case adjective::positioning::attributive: conditions.push_back("position = 'a'"); break;
        case adjective::positioning::postnominal: conditions.push_back("position = 'i'"); break;
      }
    }
    
    if (_is_variant)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM variation)");
    }
    
    if (!_variant_of.empty())
    {
      std::list<std::string> clauses(_variant_of.size(), "noun_id = @ATTRID");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM variation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_variant_of.empty())
    {
      std::list<std::string> clauses(_not_variant_of.size(), "noun_id = @NATTRID");
      std::string cond = "adjective_id NOT IN (SELECT adjective_id FROM variation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_antonymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_2_id FROM adjective_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::list<std::string> clauses(_antonym_of.size(), "adjective_1_id = @ANTID");
      std::string cond = "adjective_id IN (SELECT adjective_2_id FROM adjective_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_antonym_of.empty())
    {
      std::list<std::string> clauses(_not_antonym_of.size(), "adjective_1_id = @NANTID");
      std::string cond = "adjective_id NOT IN (SELECT adjective_2_id FROM adjective_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_synonymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_2_id FROM adjective_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::list<std::string> clauses(_synonym_of.size(), "adjective_1_id = @SYNID");
      std::string cond = "adjective_id IN (SELECT adjective_2_id FROM adjective_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_synonym_of.empty())
    {
      std::list<std::string> clauses(_not_synonym_of.size(), "adjective_1_id = @NSYNID");
      std::string cond = "adjective_id NOT IN (SELECT adjective_2_id FROM adjective_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_generalization)
    {
      conditions.push_back("adjective_id IN (SELECT general_id FROM specification)");
    }
    
    if (!_generalization_of.empty())
    {
      std::list<std::string> clauses(_generalization_of.size(), "specific_id = @SPECID");
      std::string cond = "adjective_id IN (SELECT general_id FROM specification WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_generalization_of.empty())
    {
      std::list<std::string> clauses(_not_generalization_of.size(), "specific_id = @NSPECID");
      std::string cond = "adjective_id NOT IN (SELECT general_id FROM specification WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_specification)
    {
      conditions.push_back("adjective_id IN (SELECT specific_id FROM specification)");
    }
    
    if (!_specification_of.empty())
    {
      std::list<std::string> clauses(_specification_of.size(), "general_id = @GENID");
      std::string cond = "adjective_id IN (SELECT specific_id FROM specification WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_specification_of.empty())
    {
      std::list<std::string> clauses(_not_specification_of.size(), "general_id = @NGENID");
      std::string cond = "adjective_id NOT IN (SELECT specific_id FROM specification WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_pertainymic)
    {
      conditions.push_back("adjective_id IN (SELECT pertainym_id FROM pertainymy)");
    }
    
    if (!_pertainym_of.empty())
    {
      std::list<std::string> clauses(_pertainym_of.size(), "noun_id = @APERID");
      std::string cond = "adjective_id IN (SELECT pertainym_id FROM pertainymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_mannernymic)
    {
      conditions.push_back("adjective_id IN (SELECT adjective_id FROM mannernymy)");
    }
    
    if (!_anti_mannernym_of.empty())
    {
      std::list<std::string> clauses(_anti_mannernym_of.size(), "mannernym_id = @MANID");
      std::string cond = "adjective_id IN (SELECT adjective_id FROM mannernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
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
    
    for (auto attribute : _variant_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@ATTRID"), attribute._id);
    }
    
    for (auto attribute : _not_variant_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NATTRID"), attribute._id);
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
    
    for (auto specific : _generalization_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@SPECID"), specific._id);
    }
    
    for (auto specific : _not_generalization_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NSPECID"), specific._id);
    }
    
    for (auto general : _specification_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@GENID"), general._id);
    }
    
    for (auto general : _not_specification_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NGENID"), general._id);
    }
    
    for (auto n : _pertainym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@APERID"), n._id);
    }
    
    for (auto mannernym : _anti_mannernym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@MANID"), mannernym._id);
    }
    
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
      query = "SELECT pronunciation FROM adjective_pronunciations WHERE adjective_id = ?";
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
      }
      
      sqlite3_finalize(ppstmt);
    }

    return output;
  }

};
