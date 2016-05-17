#ifndef TOKEN_H_AD62C505
#define TOKEN_H_AD62C505

namespace verbly {
  
  class token {
    public:
      enum class type {
        verb,
        noun,
        adjective,
        adverb,
        preposition,
        fillin,
        utterance,
        string
      };
      
      enum class verb_inflection {
        infinitive,
        past_tense,
        past_participle,
        s_form,
        ing_form
      };
      
      enum class noun_inflection {
        singular,
        plural
      };
      
      enum class adjective_inflection {
        base,
        comparative,
        superlative
      };
      
      enum class adverb_inflection {
        base,
        comparative,
        superlative
      };
      
      enum class fillin_type {
        generic,
        noun_phrase,
        adjective_phrase,
        adverb_phrase,
        participle_phrase,
        infinitive_phrase
      };
      
      type get_type() const;
      
      int get_extra() const;
      void set_extra(int _arg);
      
      token(const token& other);
      token& operator=(const token& other);
      ~token();
      
      bool is_complete() const;
      std::string compile() const;
      
      // Verb
      token(verb _verb);
      token(verb _verb, verb_inflection _infl);
      token& operator=(verb _verb);
      verb get_verb() const;
      void set_verb(verb _verb);
      verb_inflection get_verb_inflection() const;
      void set_verb_inflection(verb_inflection _infl);
      
      // Noun
      token(noun _noun);
      token(noun _noun, noun_inflection _infl);
      token& operator=(noun _noun);
      noun get_noun() const;
      void set_noun(noun _noun);
      noun_inflection get_noun_inflection() const;
      void set_noun_inflection(noun_inflection _infl);
      
      // Adjective
      token(adjective _adjective);
      token(adjective _adjective, adjective_inflection _infl);
      token& operator=(adjective _adjective);
      adjective get_adjective() const;
      void set_adjective(adjective _adjective);
      adjective_inflection get_adjective_inflection() const;
      void set_adjective_inflection(adjective_inflection _infl);
      
      // Adverb
      token(adverb _adverb);
      token(adverb _adverb, adverb_inflection _infl);
      token& operator=(adverb _adverb);
      adverb get_adverb() const;
      void set_adverb(adverb _adverb);
      adverb_inflection get_adverb_inflection() const;
      void set_adverb_inflection(adverb_inflection _infl);
      
      // Preposition
      token(preposition _preposition);
      token& operator=(preposition _preposition);
      preposition get_preposition() const;
      void set_preposition(preposition _preposition);
      
      // Fillin
      token(fillin_type _ft);
      token& operator=(fillin_type _ft);
      fillin_type get_fillin_type() const;
      void set_fillin_type(fillin_type _ft);
      
      // Utterance
      typedef std::list<token>::iterator iterator;
      
      token();
      token(std::initializer_list<token> _init);
      iterator begin();
      iterator end();
      token& operator<<(token _tkn);
      void push_back(token _tkn);
      void insert(iterator before, token _tkn);
      void replace(iterator torepl, token _tkn);
      void erase(iterator toer);
      
      // String
      token(std::string _str);
      token& operator=(std::string _str);
      std::string get_string() const;
      void set_string(std::string _str);
      
    private:
      type _type;
      int _extra = 0;
      union {
        struct {
          verb _verb;
          verb_inflection _infl;
        } _verb;
        struct {
          noun _noun;
          noun_inflection _infl;
        } _noun;
        struct {
          adjective _adjective;
          adjective_inflection _infl;
        } _adjective;
        struct {
          adverb _adverb;
          adverb_inflection _infl;
        } _adverb;
        struct {
          preposition _preposition;
        } _preposition;
        struct {
          fillin_type _type;
        } _fillin;
        struct {
          std::string _str;
        } _string;
        struct {
          std::list<token> _utterance;
        } _utterance;
      };
  };
  
  std::ostream& operator<<(std::ostream& os, token::type _type);

};

#endif /* end of include guard: TOKEN_H_AD62C505 */
