#ifndef DATABASE_H_0B0A47D2
#define DATABASE_H_0B0A47D2

#include <string>
#include <exception>
#include <list>

struct sqlite3;

namespace verbly {
  namespace generator {

    class field;

    class sqlite3_error : public std::exception {
    public:

      sqlite3_error(const std::string& what, const std::string& db_err);

      const char* what() const noexcept override;
      const char* db_err() const noexcept;

    private:
      std::string what_;
      std::string db_err_;

    };

    class database {
    public:

      // Constructor

      explicit database(std::string path);

      // Disable copying

      database(const database& other) = delete;
      database& operator=(const database& other) = delete;

      // Move constructor and move assignment

      database(database&& other);
      database& operator=(database&& other);

      // Swap

      friend void swap(database& first, database& second);

      // Destructor

      ~database();

      // Actions

      void runQuery(std::string query);

      void insertIntoTable(std::string table, std::list<field> fields);

    private:

      database()
      {
      }

      sqlite3* ppdb_ = nullptr;

    };

  };
};

#endif /* end of include guard: DATABASE_H_0B0A47D2 */
