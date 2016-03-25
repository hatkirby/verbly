#ifndef FRAME_H_9A5D90FE
#define FRAME_H_9A5D90FE

namespace verbly {
  
  class frame_query;

  class frame {
    public:
      class selrestr {
        public:
          enum class type {
            empty,
            singleton,
            group
          };
          
          type get_type() const;
          selrestr(const selrestr& other);
          ~selrestr();
          selrestr& operator=(const selrestr& other);
          
          // Empty
          selrestr();
          
          // Singleton
          selrestr(std::string restriction, bool pos);
          std::string get_restriction() const;
          bool get_pos() const;
          
          // Group
          selrestr(std::list<selrestr> children, bool orlogic);
          std::list<selrestr> get_children() const;
          std::list<selrestr>::const_iterator begin() const;
          std::list<selrestr>::const_iterator end() const;
          bool get_orlogic() const;
          
        private:
          union {
            struct {
              bool pos;
              std::string restriction;
            } _singleton;
            struct {
              std::list<selrestr> children;
              bool orlogic;
            } _group;
          };
          type _type;
      };
      
      class part {
        public:
          enum class type {
            noun_phrase,
            verb,
            literal_preposition,
            selection_preposition,
            adjective,
            adverb,
            literal
          };
          
          type get_type() const;
          part(const part& other);
          ~part();
          
          // Noun phrase
          std::string get_role() const;
          selrestr get_selrestrs() const;
          std::set<std::string> get_synrestrs() const;
          
          // Literal preposition
          std::vector<std::string> get_choices() const;
          
          // Selection preposition
          std::vector<std::string> get_preprestrs() const;
          
          // Literal
          std::string get_literal() const;
          
        private:
          friend class frame_query;
          
          part();
          
          union {
            struct {
              std::string role;
              selrestr selrestrs;
              std::set<std::string> synrestrs;
            } _noun_phrase;
            struct {
              std::vector<std::string> choices;
            } _literal_preposition;
            struct {
              std::vector<std::string> preprestrs;
            } _selection_preposition;
            struct {
              std::string lexval;
            } _literal;
          };
          type _type;
      };
      
      std::vector<part> parts() const;
      std::map<std::string, selrestr> roles() const;
      
    private:
      friend class frame_query;
      
      std::vector<part> _parts;
      std::map<std::string, selrestr> _roles;
  };

};

#endif /* end of include guard: FRAME_H_9A5D90FE */
