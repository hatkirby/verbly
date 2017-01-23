#include "frame.h"
#include "database.h"
#include "field.h"

namespace verbly {
  namespace generator {

    int frame::nextId_ = 0;

    frame::frame() : id_(nextId_++)
    {
    }

    void frame::push_back(part fp)
    {
      parts_.push_back(std::move(fp));
    }

    database& operator<<(database& db, const frame& arg)
    {
      std::list<field> fields;
      fields.emplace_back("frame_id", arg.getId());

      nlohmann::json jsonParts;
      for (const part& p : arg)
      {
        nlohmann::json jsonPart;
        jsonPart["type"] = static_cast<int>(p.getType());

        switch (p.getType())
        {
          case part::type::noun_phrase:
          {
            jsonPart["role"] = p.getNounRole();
            jsonPart["selrestrs"] = p.getNounSelrestrs().toJson();
            jsonPart["synrestrs"] = p.getNounSynrestrs();

            break;
          }

          case part::type::preposition:
          {
            jsonPart["choices"] = p.getPrepositionChoices();
            jsonPart["literal"] = p.isPrepositionLiteral();

            break;
          }

          case part::type::literal:
          {
            jsonPart["value"] = p.getLiteralValue();

            break;
          }

          case part::type::verb:
          case part::type::adjective:
          case part::type::adverb:
          {
            break;
          }

          case part::type::invalid:
          {
            // Invalid parts should not be serialized.
            assert(false);

            break;
          }
        }

        jsonParts.push_back(std::move(jsonPart));
      }

      fields.emplace_back("data", jsonParts.dump());

      db.insertIntoTable("frames", std::move(fields));

      return db;
    }

  };
};
