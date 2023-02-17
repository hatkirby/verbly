#include "database.h"
#include <sstream>
#include "query.h"
#include "version.h"

namespace verbly {

  database::database(
    std::string path) :
      ppdb_(std::move(path), hatkirby::dbmode::read)
  {
    hatkirby::row version =
      ppdb_.queryFirst("SELECT major, minor FROM version");

    major_ = std::get<int>(version[0]);
    minor_ = std::get<int>(version[1]);

    if (major_ != DATABASE_MAJOR_VERSION)
    {
      throw database_version_mismatch(DATABASE_MAJOR_VERSION, major_);
    }
  }

  query<notion> database::notions(filter where, order sortOrder, int limit) const
  {
    return query<notion>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<word> database::words(filter where, order sortOrder, int limit) const
  {
    return query<word>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<frame> database::frames(filter where, order sortOrder, int limit) const
  {
    return query<frame>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<part> database::parts(filter where, order sortOrder, int limit) const
  {
    return query<part>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<form> database::forms(filter where, order sortOrder, int limit) const
  {
    return query<form>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  query<pronunciation> database::pronunciations(filter where, order sortOrder, int limit) const
  {
    return query<pronunciation>(*this, ppdb_, std::move(where), std::move(sortOrder), limit);
  }

  std::set<std::string> database::selrestrs(int partId) const
  {
    std::vector<hatkirby::row> rows =
      ppdb_.queryAll(
        "SELECT selrestr FROM selrestrs WHERE part_id = ?",
        { partId });

    std::set<std::string> result;

    for (hatkirby::row& r : rows)
    {
      result.emplace(std::move(std::get<std::string>(r[0])));
    }

    return result;
  }

  std::set<std::string> database::synrestrs(int partId) const
  {
    std::vector<hatkirby::row> rows =
      ppdb_.queryAll(
        "SELECT synrestr FROM synrestrs WHERE part_id = ?",
        { partId });

    std::set<std::string> result;

    for (hatkirby::row& r : rows)
    {
      result.emplace(std::move(std::get<std::string>(r[0])));
    }

    return result;
  }

  void database::setTimeout(int ms)
  {
    ppdb_.setTimeout(ms);
  }

  std::string database_version_mismatch::generateMessage(int right, int wrong)
  {
    std::ostringstream msgbuilder;
    msgbuilder << "Incompatible database version: needed ";
    msgbuilder << right;
    msgbuilder << ", found ";
    msgbuilder << wrong;

    return msgbuilder.str();
  }

};
