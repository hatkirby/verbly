#include "verbly.h"

namespace verbly {
  
  noun::noun(const data& _data, int _id) : word(_data, _id)
  {
    
  }
  
  std::string noun::base_form() const
  {
    return _singular;
  }

  std::string noun::singular_form() const
  {
    return _singular;
  }
  
  std::string noun::plural_form() const
  {
    return _plural;
  }

  bool noun::has_plural_form() const
  {
    return !_plural.empty();
  }
  
  noun_query noun::hypernyms() const
  {
    return _data.nouns().hypernym_of(*this);
  }
  
  noun_query noun::hyponyms() const
  {
    return _data.nouns().hyponym_of(*this);
  }
  
  noun_query noun::part_meronyms() const
  {
    return _data.nouns().part_meronym_of(*this);
  }
  
  noun_query noun::part_holonyms() const
  {
    return _data.nouns().part_holonym_of(*this);
  }
  
  noun_query noun::substance_meronyms() const
  {
    return _data.nouns().substance_meronym_of(*this);
  }
  
  noun_query noun::substance_holonyms() const
  {
    return _data.nouns().substance_holonym_of(*this);
  }
  
  noun_query noun::member_meronyms() const
  {
    return _data.nouns().member_meronym_of(*this);
  }
  
  noun_query noun::member_holonyms() const
  {
    return _data.nouns().member_holonym_of(*this);
  }
  
  noun_query noun::classes() const
  {
    return _data.nouns().class_of(*this);
  }
  
  noun_query noun::instances() const
  {
    return _data.nouns().instance_of(*this);
  }
  
  noun_query noun::synonyms() const
  {
    return _data.nouns().synonym_of(*this);
  }
  
  noun_query noun::antonyms() const
  {
    return _data.nouns().antonym_of(*this);
  }
  
  adjective_query noun::pertainyms() const
  {
    return _data.adjectives().pertainym_of(*this);
  }
  
  adjective_query noun::variations() const
  {
    return _data.adjectives().variant_of(*this);
  }
  
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

  noun_query& noun_query::random(bool _random)
  {
    this->_random = _random;
  
    return *this;
  }
  
  noun_query& noun_query::except(const noun& _word)
  {
    _except.push_back(_word);
    
    return *this;
  }
  
  noun_query& noun_query::rhymes_with(const word& _word)
  {
    for (auto rhyme : _word.rhyme_phonemes())
    {
      _rhymes.push_back(rhyme);
    }
    
    if (dynamic_cast<const noun*>(&_word) != nullptr)
    {
      _except.push_back(dynamic_cast<const noun&>(_word));
    }
    
    return *this;
  }
  
  noun_query& noun_query::has_pronunciation(bool _has_prn)
  {
    this->_has_prn = _has_prn;
    
    return *this;
  }
  
  noun_query& noun_query::is_hypernym(bool _arg)
  {
    _is_hypernym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::hypernym_of(const noun& _noun)
  {
    _hypernym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_hypernym_of(const noun& _noun)
  {
    _not_hypernym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_hyponym(bool _arg)
  {
    _is_hyponym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::hyponym_of(const noun& _noun)
  {
    _hyponym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_hyponym_of(const noun& _noun)
  {
    _not_hyponym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_part_meronym(bool _arg)
  {
    _is_part_meronym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::part_meronym_of(const noun& _noun)
  {
    _part_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_part_meronym_of(const noun& _noun)
  {
    _not_part_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_part_holonym(bool _arg)
  {
    _is_part_holonym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::part_holonym_of(const noun& _noun)
  {
    _part_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_part_holonym_of(const noun& _noun)
  {
    _not_part_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_substance_meronym(bool _arg)
  {
    _is_substance_meronym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::substance_meronym_of(const noun& _noun)
  {
    _substance_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_substance_meronym_of(const noun& _noun)
  {
    _not_substance_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_substance_holonym(bool _arg)
  {
    _is_substance_holonym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::substance_holonym_of(const noun& _noun)
  {
    _substance_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_substance_holonym_of(const noun& _noun)
  {
    _not_substance_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_member_meronym(bool _arg)
  {
    _is_member_meronym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::member_meronym_of(const noun& _noun)
  {
    _member_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_member_meronym_of(const noun& _noun)
  {
    _not_member_meronym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_member_holonym(bool _arg)
  {
    _is_member_holonym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::member_holonym_of(const noun& _noun)
  {
    _member_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_member_holonym_of(const noun& _noun)
  {
    _not_member_holonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_proper(bool _arg)
  {
    _is_proper = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::instance_of(const noun& _noun)
  {
    _instance_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_instance_of(const noun& _noun)
  {
    _not_instance_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::is_class(bool _arg)
  {
    _is_class = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::class_of(const noun& _noun)
  {
    _class_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_class_of(const noun& _noun)
  {
    _not_class_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::has_synonyms(bool _arg)
  {
    _has_synonyms = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::synonym_of(const noun& _noun)
  {
    _synonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_synonym_of(const noun& _noun)
  {
    _not_synonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::has_antonyms(bool _arg)
  {
    _has_antonyms = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::antonym_of(const noun& _noun)
  {
    _antonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::not_antonym_of(const noun& _noun)
  {
    _not_antonym_of.push_back(_noun);
    
    return *this;
  }
  
  noun_query& noun_query::has_pertainym(bool _arg)
  {
    _has_pertainym = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::anti_pertainym_of(const adjective& _adj)
  {
    _anti_pertainym_of.push_back(_adj);
    
    return *this;
  }
  
  noun_query& noun_query::is_attribute(bool _arg)
  {
    _is_attribute = _arg;
    
    return *this;
  }
  
  noun_query& noun_query::attribute_of(const adjective& _adj)
  {
    _attribute_of.push_back(_adj);
    
    return *this;
  }
  
  std::list<noun> noun_query::run() const
  {
    std::stringstream construct;
    construct << "SELECT noun_id, singular, plural FROM nouns";
    std::list<std::string> conditions;
    
    if (_has_prn)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM noun_pronunciations)");
    }
    
    if (!_rhymes.empty())
    {
      std::list<std::string> clauses(_rhymes.size(), "pronunciation LIKE @RHMPRN");
      std::string cond = "noun_id IN (SELECT noun_id FROM noun_pronunciations WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    for (auto except : _except)
    {
      conditions.push_back("noun_id != @EXCID");
    }
    
    if (_is_hypernym)
    {
      conditions.push_back("noun_id IN (SELECT hypernym_id FROM hypernymy)");
    }
    
    if (!_hypernym_of.empty())
    {
      std::list<std::string> clauses(_hypernym_of.size(), "hyponym_id = @HYPO");
      std::string cond = "noun_id IN (SELECT hypernym_id FROM hypernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_hypernym_of.empty())
    {
      std::list<std::string> clauses(_not_hypernym_of.size(), "hyponym_id = @NHYPO");
      std::string cond = "noun_id NOT IN (SELECT hypernym_id FROM hypernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_hyponym)
    {
      conditions.push_back("noun_id IN (SELECT hyponym_id FROM hypernymy)");
    }
    
    if (!_hyponym_of.empty())
    {
      std::list<std::string> clauses(_hyponym_of.size(), "hypernym_id = @HYPER");
      std::string cond = "noun_id IN (SELECT hyponym_id FROM hypernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_hyponym_of.empty())
    {
      std::list<std::string> clauses(_not_hyponym_of.size(), "hypernym_id = @NHYPER");
      std::string cond = "noun_id NOT IN (SELECT hyponym_id FROM hypernymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_part_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM part_meronymy)");
    }
    
    if (!_part_meronym_of.empty())
    {
      std::list<std::string> clauses(_part_meronym_of.size(), "holonym_id = @PHOLO");
      std::string cond = "noun_id IN (SELECT meronym_id FROM part_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_part_meronym_of.empty())
    {
      std::list<std::string> clauses(_not_part_meronym_of.size(), "holonym_id = @NPHOLO");
      std::string cond = "noun_id NOT IN (SELECT meronym_id FROM part_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_part_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM part_meronymy)");
    }
    
    if (!_part_holonym_of.empty())
    {
      std::list<std::string> clauses(_part_holonym_of.size(), "meronym_id = @PMERO");
      std::string cond = "noun_id IN (SELECT holonym_id FROM part_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_part_holonym_of.empty())
    {
      std::list<std::string> clauses(_not_part_holonym_of.size(), "meronym_id = @NPMERO");
      std::string cond = "noun_id NOT IN (SELECT holonym_id FROM part_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_substance_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM substance_meronymy)");
    }
    
    if (!_substance_meronym_of.empty())
    {
      std::list<std::string> clauses(_substance_meronym_of.size(), "holonym_id = @SHOLO");
      std::string cond = "noun_id IN (SELECT meronym_id FROM substance_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_substance_meronym_of.empty())
    {
      std::list<std::string> clauses(_not_substance_meronym_of.size(), "holonym_id = @NSHOLO");
      std::string cond = "noun_id NOT IN (SELECT meronym_id FROM substance_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_substance_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM substance_meronymy)");
    }
    
    if (!_substance_holonym_of.empty())
    {
      std::list<std::string> clauses(_substance_holonym_of.size(), "meronym_id = @SMERO");
      std::string cond = "noun_id IN (SELECT holonym_id FROM substance_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_substance_holonym_of.empty())
    {
      std::list<std::string> clauses(_not_substance_holonym_of.size(), "meronym_id = @NSMERO");
      std::string cond = "noun_id NOT IN (SELECT holonym_id FROM substance_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_member_meronym)
    {
      conditions.push_back("noun_id IN (SELECT meronym_id FROM member_meronymy)");
    }
    
    if (!_member_meronym_of.empty())
    {
      std::list<std::string> clauses(_member_meronym_of.size(), "holonym_id = @MHOLO");
      std::string cond = "noun_id IN (SELECT meronym_id FROM member_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_member_meronym_of.empty())
    {
      std::list<std::string> clauses(_not_member_meronym_of.size(), "holonym_id = @NMHOLO");
      std::string cond = "noun_id NOT IN (SELECT meronym_id FROM member_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_member_holonym)
    {
      conditions.push_back("noun_id IN (SELECT holonym_id FROM member_meronym)");
    }
    
    if (!_member_holonym_of.empty())
    {
      std::list<std::string> clauses(_member_holonym_of.size(), "meronym_id = @MMERO");
      std::string cond = "noun_id IN (SELECT holonym_id FROM member_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_member_holonym_of.empty())
    {
      std::list<std::string> clauses(_not_member_holonym_of.size(), "meronym_id = @NMMERO");
      std::string cond = "noun_id NOT IN (SELECT holonym_id FROM member_meronymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_proper)
    {
      conditions.push_back("noun_id IN (SELECT instance_id FROM instantiation)");
    }
    
    if (!_instance_of.empty())
    {
      std::list<std::string> clauses(_instance_of.size(), "class_id = @CLSID");
      std::string cond = "noun_id IN (SELECT instance_id FROM instantiation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_instance_of.empty())
    {
      std::list<std::string> clauses(_not_instance_of.size(), "class_id = @NCLSID");
      std::string cond = "noun_id NOT IN (SELECT instance_id FROM instantiation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_class)
    {
      conditions.push_back("noun_id IN (SELECT class_id FROM instantiation)");
    }
    
    if (!_class_of.empty())
    {
      std::list<std::string> clauses(_class_of.size(), "instance_id = @INSID");
      std::string cond = "noun_id IN (SELECT class_id FROM instantiation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_class_of.empty())
    {
      std::list<std::string> clauses(_not_class_of.size(), "instance_id = @NINSID");
      std::string cond = "noun_id NOT IN (SELECT class_id FROM instantiation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_has_synonyms)
    {
      conditions.push_back("noun_id IN (SELECT adjective_2_id FROM adjective_synonymy)");
    }
    
    if (!_synonym_of.empty())
    {
      std::list<std::string> clauses(_synonym_of.size(), "adjective_1_id = @SYNID");
      std::string cond = "noun_id IN (SELECT adjective_2_id FROM adjective_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_synonym_of.empty())
    {
      std::list<std::string> clauses(_not_synonym_of.size(), "adjective_1_id = @NSYNID");
      std::string cond = "noun_id NOT IN (SELECT adjective_2_id FROM adjective_synonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_has_antonyms)
    {
      conditions.push_back("noun_id IN (SELECT adjective_2_id FROM adjective_antonymy)");
    }
    
    if (!_antonym_of.empty())
    {
      std::list<std::string> clauses(_antonym_of.size(), "adjective_1_id = @ANTID");
      std::string cond = "noun_id IN (SELECT adjective_2_id FROM adjective_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (!_not_antonym_of.empty())
    {
      std::list<std::string> clauses(_not_antonym_of.size(), "adjective_1_id = @NANTID");
      std::string cond = "noun_id NOT IN (SELECT adjective_2_id FROM adjective_antonymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_has_pertainym)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM pertainymy)");
    }
    
    if (!_anti_pertainym_of.empty())
    {
      std::list<std::string> clauses(_anti_pertainym_of.size(), "pertainym_id = @PERID");
      std::string cond = "noun_id IN (SELECT noun_id FROM pertainymy WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
      conditions.push_back(cond);
    }
    
    if (_is_attribute)
    {
      conditions.push_back("noun_id IN (SELECT noun_id FROM variation)");
    }
    
    if (!_attribute_of.empty())
    {
      std::list<std::string> clauses(_attribute_of.size(), "adjective_id = @VALID");
      std::string cond = "noun_id IN (SELECT noun_id FROM variation WHERE " + verbly::implode(std::begin(clauses), std::end(clauses), " OR ") + ")";
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
    
    for (auto hyponym : _hypernym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@HYPO"), hyponym._id);
    }
    
    for (auto hyponym : _not_hypernym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NHYPO"), hyponym._id);
    }
    
    for (auto hypernym : _hyponym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@HYPER"), hypernym._id);
    }
    
    for (auto hypernym : _not_hyponym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NHYPER"), hypernym._id);
    }
    
    for (auto holonym : _part_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@PHOLO"), holonym._id);
    }
    
    for (auto holonym : _not_part_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NPHOLO"), holonym._id);
    }
    
    for (auto meronym : _part_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@PMERO"), meronym._id);
    }
    
    for (auto meronym : _not_part_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NPMERO"), meronym._id);
    }
    
    for (auto holonym : _substance_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@SHOLO"), holonym._id);
    }
    
    for (auto holonym : _not_substance_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NSHOLO"), holonym._id);
    }
    
    for (auto meronym : _substance_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@SMERO"), meronym._id);
    }
    
    for (auto meronym : _not_substance_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NSMERO"), meronym._id);
    }
    
    for (auto holonym : _member_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@MHOLO"), holonym._id);
    }
    
    for (auto holonym : _not_member_meronym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NMHOLO"), holonym._id);
    }
    
    for (auto meronym : _member_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@MMERO"), meronym._id);
    }
    
    for (auto meronym : _not_member_holonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NMMERO"), meronym._id);
    }
    
    for (auto cls : _instance_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@CLSID"), cls._id);
    }
    
    for (auto cls : _not_instance_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NCLSID"), cls._id);
    }
    
    for (auto inst : _class_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@INSID"), inst._id);
    }
    
    for (auto inst : _not_class_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NINSID"), inst._id);
    }
    
    for (auto synonym : _synonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@SYNID"), synonym._id);
    }
    
    for (auto synonym : _not_synonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NSYNID"), synonym._id);
    }
    
    for (auto antonym : _antonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@ANTID"), antonym._id);
    }
    
    for (auto antonym : _not_antonym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@NANTID"), antonym._id);
    }
    
    for (auto pertainym : _anti_pertainym_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@PERID"), pertainym._id);
    }
    
    for (auto value : _attribute_of)
    {
      sqlite3_bind_int(ppstmt, sqlite3_bind_parameter_index(ppstmt, "@VALID"), value._id);
    }

    std::list<noun> output;
    while (sqlite3_step(ppstmt) == SQLITE_ROW)
    {
      noun tnc {_data, sqlite3_column_int(ppstmt, 0)};
      tnc._singular = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
      
      if (sqlite3_column_type(ppstmt, 2) != SQLITE_NULL)
      {
        tnc._plural = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
      }
  
      output.push_back(tnc);
    }

    sqlite3_finalize(ppstmt);
    
    for (auto& noun : output)
    {
      query = "SELECT pronunciation FROM noun_pronunciations WHERE noun_id = ?";
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
      }
      
      sqlite3_finalize(ppstmt);
    }

    return output;
  }

};
