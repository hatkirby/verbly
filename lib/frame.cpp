#include "frame.h"
#include <sqlite3.h>
#include <json.hpp>

namespace verbly {

  const object frame::objectType = object::frame;

  const std::list<std::string> frame::select = {"frame_id", "data"};

  const field frame::id = field::integerField(object::frame, "frame_id");

  const field frame::group = field::joinThrough(object::frame, "frame_id", object::group, "groups_frames", "group_id");

  frame::frame(const database& db, sqlite3_stmt* row) : db_(&db), valid_(true)
  {
    id_ = sqlite3_column_int(row, 0);

    std::string partsJsonStr(reinterpret_cast<const char*>(sqlite3_column_blob(row, 1)));
    nlohmann::json partsJson = nlohmann::json::parse(std::move(partsJsonStr));

    for (const nlohmann::json& partJson : partsJson)
    {
      part::type partType = static_cast<part::type>(partJson["type"].get<int>());

      switch (partType)
      {
        case part::type::noun_phrase:
        {
          std::set<std::string> synrestrs;
          for (const nlohmann::json& synrestrJson : partJson["synrestrs"])
          {
            synrestrs.insert(synrestrJson.get<std::string>());
          }

          parts_.push_back(part::createNounPhrase(
            partJson["role"].get<std::string>(),
            selrestr(partJson["selrestrs"]),
            std::move(synrestrs)));

          break;
        }

        case part::type::preposition:
        {
          std::vector<std::string> choices;
          for (const nlohmann::json& choiceJson : partJson["choices"])
          {
            choices.push_back(choiceJson.get<std::string>());
          }

          parts_.push_back(part::createPreposition(
            std::move(choices),
            partJson["literal"].get<bool>()));

          break;
        }

        case part::type::verb:
        {
          parts_.push_back(part::createVerb());

          break;
        }

        case part::type::adjective:
        {
          parts_.push_back(part::createAdjective());

          break;
        }

        case part::type::adverb:
        {
          parts_.push_back(part::createAdverb());

          break;
        }

        case part::type::literal:
        {
          parts_.push_back(part::createLiteral(partJson["value"].get<std::string>()));

          break;
        }

        case part::type::invalid:
        {
          throw std::domain_error("Invalid part data");
        }
      }
    }
  }

};
