#ifndef DATA_H_C4AEC3DD
#define DATA_H_C4AEC3DD

#include "verb.h"
#include <sqlite3.h>
#include <stdexcept>

namespace verbly {
  
  class data {
    private:
      sqlite3* ppdb;
      
    public:
      class verb_query {
        public:
          const static int unlimited = -1;
          
        private:
          const data& database;
          int m_limit = unlimited;
          bool m_random = false;
          
        public:
          verb_query(const data& database) : database(database)
          {
            
          }
          
          verb_query& limit(int m_limit)
          {
            if ((m_limit > 0) || (m_limit == unlimited))
            {
              this->m_limit = m_limit;
            }
            
            return *this;
          }
          
          verb_query& random(bool m_random)
          {
            this->m_random = m_random;
            
            return *this;
          }
          
          std::list<verb> run() const
          {
            std::stringstream construct;
            construct << "SELECT verb_id, infinitive, past_tense, past_participle, ing_form, s_form FROM verbs";
            
            if (m_random)
            {
              construct << " ORDER BY RANDOM()";
            }
            
            if (m_limit != unlimited)
            {
              construct << " LIMIT " << m_limit;
            }
            
            sqlite3_stmt* ppstmt;
            std::string query = construct.str();
            if (sqlite3_prepare_v2(database.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
            {
              throw std::runtime_error(sqlite3_errmsg(database.ppdb));
            }
            
            std::list<verb> output;
            while (sqlite3_step(ppstmt) == SQLITE_ROW)
            {
              verb tnc {sqlite3_column_int(ppstmt, 0)};
              tnc.infinitive = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
              tnc.past_tense = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 2)));
              tnc.past_participle = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 3)));
              tnc.ing_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 4)));
              tnc.s_form = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 5)));
              
              output.push_back(tnc);
            }
            
            sqlite3_finalize(ppstmt);
            
            return output;
          }
          
      };
      
      class adjective_query {
        public:
          const static int unlimited = -1;
          
        private:
          const data& database;
          int m_limit = unlimited;
          bool m_random = false;
          
        public:
          adjective_query(const data& database) : database(database)
          {
            
          }
          
          adjective_query& limit(int m_limit)
          {
            if ((m_limit > 0) || (m_limit == unlimited))
            {
              this->m_limit = m_limit;
            }
            
            return *this;
          }
          
          adjective_query& random(bool m_random)
          {
            this->m_random = m_random;
            
            return *this;
          }
          
          std::list<adjective> run() const
          {
            std::stringstream construct;
            construct << "SELECT adjective_id, adjective FROM adjectives";
            
            if (m_random)
            {
              construct << " ORDER BY RANDOM()";
            }
            
            if (m_limit != unlimited)
            {
              construct << " LIMIT " << m_limit;
            }
            
            sqlite3_stmt* ppstmt;
            std::string query = construct.str();
            if (sqlite3_prepare_v2(database.ppdb, query.c_str(), query.length(), &ppstmt, NULL) != SQLITE_OK)
            {
              throw std::runtime_error(sqlite3_errmsg(database.ppdb));
            }
            
            std::list<adjective> output;
            while (sqlite3_step(ppstmt) == SQLITE_ROW)
            {
              adjective tnc {sqlite3_column_int(ppstmt, 0)};
              tnc.value = std::string(reinterpret_cast<const char*>(sqlite3_column_text(ppstmt, 1)));
              
              output.push_back(tnc);
            }
            
            sqlite3_finalize(ppstmt);
            
            return output;
          }
          
      };
      
      data(std::string datafile)
      {
        if (sqlite3_open_v2(datafile.c_str(), &ppdb, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
        {
          throw std::invalid_argument(sqlite3_errmsg(ppdb));
        }
      }
      
      data(const data& other) = delete;
      data& operator=(const data& other) = delete;
      
      data(data&& other)
      {
        ppdb = other.ppdb;
      }
      
      data& operator=(data&& other)
      {
        ppdb = other.ppdb;
        
        return *this;
      }
      
      ~data()
      {
        sqlite3_close_v2(ppdb);
      }
      
      verb_query verbs() const
      {
        return verb_query(*this);
      }
      
      adjective_query adjectives() const
      {
        return adjective_query(*this);
      }
      
  };
  
};

#endif /* end of include guard: DATA_H_C4AEC3DD */
