#include "group.h"
#include <stdexcept>
#include <list>
#include <hkutil/string.h>
#include "frame.h"

namespace verbly {
  namespace generator {

    int group::nextId_ = 0;

    group::group() : id_(nextId_++)
    {
    }

    group::group(const group& parent) :
      id_(nextId_++),
      roles_(parent.roles_),
      roleNames_(parent.roleNames_)
    {
      for (const frame& f : parent.frames_)
      {
        frames_.push_back(frame::duplicate(f));
      }
    }

    void group::addRole(role r)
    {
      std::string name = r.getName();
      roles_[name] = std::move(r);
      roleNames_.insert(std::move(name));
    }

    void group::addFrame(frame f)
    {
      frames_.push_back(std::move(f));
    }

    bool group::hasRole(std::string name) const
    {
      // Rarely, a noun phrase part may use a role that is not defined in the
      // group. See confess-37.10 "NP V NP ADJ".
      return (roles_.count(name) == 1);
    }

    const role& group::getRole(std::string name) const
    {
      return roles_.at(name);
    }

    hatkirby::database& operator<<(hatkirby::database& db, const group& arg)
    {
      // Serialize each frame
      for (const frame& f : arg.getFrames())
      {
        // First, serialize the group/frame relationship
        db.insertIntoTable(
          "frames",
          {
            { "frame_id", f.getId() },
            { "group_id", arg.getId() },
            { "length", f.getLength() }
          });

        // Then, serialize the frame parts in the context of the group
        for (int partIndex = 0; partIndex < f.getLength(); partIndex++)
        {
          const part& p = f[partIndex];

          std::list<hatkirby::column> fields;
          fields.emplace_back("part_id", p.getId());
          fields.emplace_back("frame_id", f.getId());
          fields.emplace_back("part_index", partIndex);
          fields.emplace_back("type", static_cast<int>(p.getType()));

          switch (p.getType())
          {
            case part::type::noun_phrase:
            {
              fields.emplace_back("role", p.getNounRole());

              // Short interlude to serialize the selrestrs
              std::set<std::string> partSelrestrs = p.getNounSelrestrs();
              if (partSelrestrs.empty() && arg.hasRole(p.getNounRole()))
              {
                partSelrestrs = arg.getRole(p.getNounRole()).getSelrestrs();
              }

              for (const std::string& s : partSelrestrs)
              {
                db.insertIntoTable(
                  "selrestrs",
                  {
                    { "part_id", p.getId() },
                    { "selrestr", s }
                  });
              }

              // Short interlude to serialize the synrestrs
              for (const std::string& s : p.getNounSynrestrs())
              {
                db.insertIntoTable(
                  "synrestrs",
                  {
                    { "part_id", p.getId() },
                    { "synrestr", s }
                  });
              }

              break;
            }

            case part::type::preposition:
            {
              std::set<std::string> setChoices = p.getPrepositionChoices();

              std::string serializedChoices =
                hatkirby::implode(
                  std::begin(setChoices),
                  std::end(setChoices),
                  ",");

              fields.emplace_back("prepositions", std::move(serializedChoices));

              fields.emplace_back("preposition_literality",
                p.isPrepositionLiteral() ? 1 : 0);

              break;
            }

            case part::type::literal:
            {
              fields.emplace_back("literal_value", p.getLiteralValue());

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

          db.insertIntoTable("parts", std::move(fields));
        }
      }

      return db;
    }

  };
};
