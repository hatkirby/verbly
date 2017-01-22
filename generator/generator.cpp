#include "generator.h"
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <regex>
#include <dirent.h>
#include <fstream>
#include "../lib/enums.h"
#include "progress.h"
#include "../lib/selrestr.h"
#include "../lib/role.h"
#include "part.h"
#include "field.h"
#include "../lib/util.h"

namespace verbly {
  namespace generator {

    generator::generator(
      std::string verbNetPath,
      std::string agidPath,
      std::string wordNetPath,
      std::string cmudictPath,
      std::string imageNetPath,
      std::string outputPath) :
        verbNetPath_(verbNetPath),
        agidPath_(agidPath),
        wordNetPath_(wordNetPath),
        cmudictPath_(cmudictPath),
        imageNetPath_(imageNetPath),
        db_(outputPath)
    {
      // Ensure VerbNet directory exists
      DIR* dir;
      if ((dir = opendir(verbNetPath_.c_str())) == nullptr)
      {
        throw std::invalid_argument("Invalid VerbNet data directory");
      }

      closedir(dir);

      // Ensure AGID infl.txt exists
      if (!std::ifstream(agidPath_))
      {
        throw std::invalid_argument("AGID infl.txt file not found");
      }

      // Add directory separator to WordNet path
      if ((wordNetPath_.back() != '/') && (wordNetPath_.back() != '\\'))
      {
        wordNetPath_ += '/';
      }

      // Ensure WordNet tables exist
      for (std::string table : {
        "s", "sk", "ant", "at", "cls", "hyp", "ins", "mm", "mp", "ms", "per", "sa", "sim", "syntax"
      })
      {
        if (!std::ifstream(wordNetPath_ + "wn_" + table + ".pl"))
        {
          throw std::invalid_argument("WordNet " + table + " table not found");
        }
      }

      // Ensure CMUDICT file exists
      if (!std::ifstream(cmudictPath_))
      {
        throw std::invalid_argument("CMUDICT file not found");
      }

      // Ensure ImageNet urls.txt exists
      if (!std::ifstream(imageNetPath_))
      {
        throw std::invalid_argument("ImageNet urls.txt file not found");
      }
    }

    void generator::run()
    {
      // Create notions, words, lemmas, and forms from WordNet synsets
      readWordNetSynsets();

      // Reads adjective positioning WordNet data
      readAdjectivePositioning();

      // Counts the number of URLs ImageNet has per notion
      readImageNetUrls();

      // Creates a word by WordNet sense key lookup table
      readWordNetSenseKeys();

      // Creates groups and frames from VerbNet data
      readVerbNet();

      // Creates forms and inflections from AGID. To reduce the amount of forms
      // created, we do this after most lemmas that need inflecting have been
      // created through other means, and then only generate forms for
      // inflections of already-existing lemmas. The exception to this regards
      // verb lemmas. If a verb lemma in AGID either does not exist yet, or does
      // exist but is not related to any words that are related to verb notions,
      // then a notion and a word is generated and the form generation proceeds
      // as usual.
      readAgidInflections();

      // Reads in prepositions and the is_a relationship
      readPrepositions();

      // Creates pronunciations from CMUDICT. To reduce the amount of
      // pronunciations created, we do this after all forms have been created,
      // and then only generate pronunciations for already-exisiting forms.
      readCmudictPronunciations();

      // Writes the database schema
      writeSchema();

      // Dumps data to the database
      dumpObjects();

      // Populates the antonymy relationship from WordNet
      readWordNetAntonymy();

      // Populates the variation relationship from WordNet
      readWordNetVariation();

      // Populates the usage, topicality, and regionality relationships from
      // WordNet
      readWordNetClasses();

      // Populates the causality relationship from WordNet
      readWordNetCausality();

      // Populates the entailment relationship from WordNet
      readWordNetEntailment();

      // Populates the hypernymy relationship from WordNet
      readWordNetHypernymy();

      // Populates the instantiation relationship from WordNet
      readWordNetInstantiation();

      // Populates the member meronymy relationship from WordNet
      readWordNetMemberMeronymy();

      // Populates the part meronymy relationship from WordNet
      readWordNetPartMeronymy();

      // Populates the substance meronymy relationship from WordNet
      readWordNetSubstanceMeronymy();

      // Populates the pertainymy and mannernymy relationships from WordNet
      readWordNetPertainymy();

      // Populates the specification relationship from WordNet
      readWordNetSpecification();

      // Populates the adjective similarity relationship from WordNet
      readWordNetSimilarity();
    }

    void generator::readWordNetSynsets()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_s.pl"));
      progress ppgs("Reading synsets from WordNet...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        std::regex relation("^s\\(([1234]\\d{8}),(\\d+),'(.+)',\\w,\\d+,(\\d+)\\)\\.$");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int synset_id = std::stoi(relation_data[1]);
        int wnum = std::stoi(relation_data[2]);
        std::string text = relation_data[3];
        int tag_count = std::stoi(relation_data[4]);
        size_t word_it;
        while ((word_it = text.find("''")) != std::string::npos)
        {
          text.erase(word_it, 1);
        }

        // The WordNet data does contain duplicates, so we need to check that we
        // haven't already created this word.
        std::pair<int, int> lookup(synset_id, wnum);
        if (!wordByWnidAndWnum_.count(lookup))
        {
          notion& synset = lookupOrCreateNotion(synset_id);
          lemma& lex = lookupOrCreateLemma(text);
          word& entry = createWord(synset, lex, tag_count);

          wordByWnidAndWnum_[lookup] = &entry;
        }
      }
    }

    void generator::readAdjectivePositioning()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_syntax.pl"));
      progress ppgs("Reading adjective positionings from WordNet...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        std::regex relation("^syntax\\((3\\d{8}),(\\d+),([ipa])p?\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int synset_id = stoi(relation_data[1]);
        int wnum = stoi(relation_data[2]);
        std::string adjpos_str = relation_data[3];

        std::pair<int, int> lookup(synset_id, wnum);
        if (wordByWnidAndWnum_.count(lookup))
        {
          word& adj = *wordByWnidAndWnum_.at(lookup);

          if (adjpos_str == "p")
          {
            adj.setAdjectivePosition(positioning::predicate);
          } else if (adjpos_str == "a")
          {
            adj.setAdjectivePosition(positioning::attributive);
          } else if (adjpos_str == "i")
          {
            adj.setAdjectivePosition(positioning::postnominal);
          } else {
            // Can't happen because of how we specified the regex.
            assert(false);
          }
        }
      }
    }

    void generator::readImageNetUrls()
    {
      // The ImageNet datafile is so large that it is unreasonable and
      // unnecessary to read it into memory; instead, we will parse each line as
      // we read it. This has the caveat that we cannot display a progress bar.
      std::cout << "Reading image counts from ImageNet..." << std::endl;

      std::ifstream file(imageNetPath_);
      if (!file)
      {
        throw std::invalid_argument("Could not find file " + imageNetPath_);
      }

      std::string line;
      while (std::getline(file, line))
      {
        if (line.back() == '\r')
        {
          line.pop_back();
        }

        std::string wnid_s = line.substr(1, 8);
        int wnid = stoi(wnid_s) + 100000000;
        if (notionByWnid_.count(wnid))
        {
          // We know that this notion has a wnid and is a noun.
          notionByWnid_.at(wnid)->incrementNumOfImages();
        }
      }
    }

    void generator::readWordNetSenseKeys()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_sk.pl"));
      progress ppgs("Reading sense keys from WordNet...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        // We only actually need to lookup verbs by sense key so we'll just
        // ignore everything that isn't a verb.
        std::regex relation("^sk\\((2\\d{8}),(\\d+),'(.+)'\\)\\.$");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int synset_id = stoi(relation_data[1]);
        int wnum = stoi(relation_data[2]);
        std::string sense_key = relation_data[3];

        // We are treating this mapping as injective, which is not entirely
        // accurate. First, the WordNet table contains duplicate rows, so those
        // need to be ignored. More importantly, a small number of sense keys
        // (one for each letter of the Latin alphabet, plus 9 other words) each
        // map to two different words in the same synset which differ only by
        // capitalization. Luckily, none of these exceptions are verbs, so we
        // can pretend that the mapping is injective.
        if (!wnSenseKeys_.count(sense_key))
        {
          std::pair<int, int> lookup(synset_id, wnum);
          if (wordByWnidAndWnum_.count(lookup))
          {
            wnSenseKeys_[sense_key] = wordByWnidAndWnum_.at(lookup);
          }
        }
      }
    }

    void generator::readVerbNet()
    {
      std::cout << "Reading frames from VerbNet..." << std::endl;

      DIR* dir;
      if ((dir = opendir(verbNetPath_.c_str())) == nullptr)
      {
        throw std::invalid_argument("Invalid VerbNet data directory");
      }

      struct dirent* ent;
      while ((ent = readdir(dir)) != nullptr)
      {
        std::string filename(verbNetPath_);

        if (filename.back() != '/')
        {
          filename += '/';
        }

        filename += ent->d_name;

        if (filename.rfind(".xml") != filename.size() - 4)
        {
          continue;
        }

        xmlDocPtr doc = xmlParseFile(filename.c_str());
        if (doc == nullptr)
        {
          throw std::logic_error("Error opening " + filename);
        }

        xmlNodePtr top = xmlDocGetRootElement(doc);
        if ((top == nullptr) || (xmlStrcmp(top->name, reinterpret_cast<const xmlChar*>("VNCLASS"))))
        {
          throw std::logic_error("Bad VerbNet file format: " + filename);
        }

        try
        {
          createGroup(top);
        } catch (const std::exception& e)
        {
          std::throw_with_nested(std::logic_error("Error parsing VerbNet file: " + filename));
        }
      }

      closedir(dir);
    }

    void generator::readAgidInflections()
    {
      std::list<std::string> lines(readFile(agidPath_));
      progress ppgs("Reading inflections from AGID...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        int divider = line.find_first_of(" ");
        std::string infinitive = line.substr(0, divider);
        line = line.substr(divider+1);
        char type = line[0];

        if (line[1] == '?')
        {
          line.erase(0, 4);
        } else {
          line.erase(0, 3);
        }

        if (!lemmaByBaseForm_.count(infinitive) && (type != 'V'))
        {
          continue;
        }

        lemma& curLemma = lookupOrCreateLemma(infinitive);

        std::vector<std::list<std::string>> agidForms;
        for (std::string inflForms : split<std::list<std::string>>(line, " | "))
        {
          std::list<std::string> forms;

          for (std::string inflForm : split<std::list<std::string>>(std::move(inflForms), ", "))
          {
            int sympos = inflForm.find_first_of("~<!? ");
            if (sympos != std::string::npos)
            {
              inflForm = inflForm.substr(0, sympos);
            }

            forms.push_back(std::move(inflForm));
          }

          agidForms.push_back(std::move(forms));
        }

        std::map<inflection, std::list<std::string>> mappedForms;
        switch (type)
        {
          case 'V':
          {
            if (agidForms.size() == 4)
            {
              mappedForms[inflection::past_tense] = agidForms[0];
              mappedForms[inflection::past_participle] = agidForms[1];
              mappedForms[inflection::ing_form] = agidForms[2];
              mappedForms[inflection::s_form] = agidForms[3];
            } else if (agidForms.size() == 3)
            {
              mappedForms[inflection::past_tense] = agidForms[0];
              mappedForms[inflection::past_participle] = agidForms[0];
              mappedForms[inflection::ing_form] = agidForms[1];
              mappedForms[inflection::s_form] = agidForms[2];
            } else if (agidForms.size() == 8)
            {
              // As of AGID 2014.08.11, this is only "to be"
              mappedForms[inflection::past_tense] = agidForms[0];
              mappedForms[inflection::past_participle] = agidForms[2];
              mappedForms[inflection::ing_form] = agidForms[3];
              mappedForms[inflection::s_form] = agidForms[4];
            } else {
              // Words that don't fit the cases above as of AGID 2014.08.11:
              // - may and shall do not conjugate the way we want them to
              // - methinks only has a past tense and is an outlier
              // - wit has five forms, and is archaic/obscure enough that we can ignore it for now
              std::cout << " Ignoring verb \"" << infinitive << "\" due to non-standard number of forms." << std::endl;
            }

            // For verbs in particular, we sometimes create a notion and a word
            // from inflection data. Specifically, if there are not yet any
            // verbs existing that have the same infinitive form. "Yet" means
            // that this verb appears in the AGID data but not in either WordNet
            // or VerbNet.
            if (!wordsByBaseForm_.count(infinitive)
              || !std::any_of(std::begin(wordsByBaseForm_.at(infinitive)), std::end(wordsByBaseForm_.at(infinitive)), [] (word* w) {
                return w->getNotion().getPartOfSpeech() == part_of_speech::verb;
              }))
            {
              notion& n = createNotion(part_of_speech::verb);
              createWord(n, curLemma);
            }

            break;
          }

          case 'A':
          {
            if (agidForms.size() == 2)
            {
              mappedForms[inflection::comparative] = agidForms[0];
              mappedForms[inflection::superlative] = agidForms[1];
            } else {
              // As of AGID 2014.08.11, this is only "only", which has only the form "onliest"
              std::cout << " Ignoring adjective/adverb \"" << infinitive << "\" due to non-standard number of forms." << std::endl;
            }

            break;
          }

          case 'N':
          {
            if (agidForms.size() == 1)
            {
              mappedForms[inflection::plural] = agidForms[0];
            } else {
              // As of AGID 2014.08.11, this is non-existent.
              std::cout << " Ignoring noun \"" << infinitive << "\" due to non-standard number of forms." << std::endl;
            }

            break;
          }
        }

        // Compile the forms we have mapped.
        for (auto mapping : std::move(mappedForms))
        {
          for (std::string infl : std::move(mapping.second))
          {
            curLemma.addInflection(mapping.first, lookupOrCreateForm(std::move(infl)));
          }
        }
      }
    }

    void generator::readPrepositions()
    {
      std::list<std::string> lines(readFile("prepositions.txt"));
      progress ppgs("Reading prepositions...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        std::regex relation("^([^:]+): (.+)");
        std::smatch relation_data;
        std::regex_search(line, relation_data, relation);
        std::string prep = relation_data[1];
        auto groups = split<std::list<std::string>>(relation_data[2], ", ");

        notion& n = createNotion(part_of_speech::preposition);
        lemma& l = lookupOrCreateLemma(prep);
        word& w = createWord(n, l);

        n.setPrepositionGroups(groups);
      }
    }

    void generator::readCmudictPronunciations()
    {
      std::list<std::string> lines(readFile(cmudictPath_));
      progress ppgs("Reading pronunciations from CMUDICT...", lines.size());

      for (std::string line : lines)
      {
        ppgs.update();

        std::regex phoneme("([A-Z][^ \\(]*)(?:\\(\\d+\\))?  ([A-Z 0-9]+)");
        std::smatch phoneme_data;
        if (std::regex_search(line, phoneme_data, phoneme))
        {
          std::string canonical(phoneme_data[1]);
          std::transform(std::begin(canonical), std::end(canonical), std::begin(canonical), ::tolower);

          if (!formByText_.count(canonical))
          {
            continue;
          }

          std::string phonemes = phoneme_data[2];
          pronunciations_.emplace_back(phonemes);
          pronunciation& p = pronunciations_.back();
          formByText_.at(canonical)->addPronunciation(p);
        }
      }
    }

    void generator::writeSchema()
    {
      std::ifstream file("schema.sql");
      if (!file)
      {
        throw std::invalid_argument("Could not find database schema");
      }

      std::ostringstream schemaBuilder;
      std::string line;
      while (std::getline(file, line))
      {
        if (line.back() == '\r')
        {
          line.pop_back();
        }

        schemaBuilder << line;
      }

      std::string schema = schemaBuilder.str();
      auto queries = split<std::list<std::string>>(schema, ";");
      progress ppgs("Writing database schema...", queries.size());
      for (std::string query : queries)
      {
        if (!queries.empty())
        {
          db_.runQuery(query);
        }

        ppgs.update();
      }
    }

    void generator::dumpObjects()
    {
      {
        progress ppgs("Writing notions...", notions_.size());

        for (notion& n : notions_)
        {
          db_ << n;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing words...", words_.size());

        for (word& w : words_)
        {
          db_ << w;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing lemmas...", lemmas_.size());

        for (lemma& l : lemmas_)
        {
          db_ << l;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing forms...", forms_.size());

        for (form& f : forms_)
        {
          db_ << f;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing pronunciations...", pronunciations_.size());

        for (pronunciation& p : pronunciations_)
        {
          db_ << p;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing verb groups...", groups_.size());

        for (group& g : groups_)
        {
          db_ << g;

          ppgs.update();
        }
      }

      {
        progress ppgs("Writing verb frames...", frames_.size());

        for (frame& f : frames_)
        {
          db_ << f;

          ppgs.update();
        }
      }
    }

    void generator::readWordNetAntonymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_ant.pl"));
      progress ppgs("Writing antonyms...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^ant\\(([134]\\d{8}),(\\d+),([134]\\d{8}),(\\d+)\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        std::pair<int, int> lookup1(std::stoi(relation_data[1]), std::stoi(relation_data[2]));
        std::pair<int, int> lookup2(std::stoi(relation_data[3]), std::stoi(relation_data[4]));

        if (wordByWnidAndWnum_.count(lookup1) && wordByWnidAndWnum_.count(lookup2))
        {
          word& word1 = *wordByWnidAndWnum_.at(lookup1);
          word& word2 = *wordByWnidAndWnum_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("antonym_1_id", word1.getId());
          fields.emplace_back("antonym_2_id", word2.getId());

          db_.insertIntoTable("antonymy", std::move(fields));
        }
      }
    }

    void generator::readWordNetVariation()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_at.pl"));
      progress ppgs("Writing variation...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^at\\((1\\d{8}),(3\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("noun_id", notion1.getId());
          fields.emplace_back("adjective_id", notion2.getId());

          db_.insertIntoTable("variation", std::move(fields));
        }
      }
    }

    void generator::readWordNetClasses()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_cls.pl"));
      progress ppgs("Writing usage, topicality, and regionality...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^cls\\(([134]\\d{8}),(\\d+),(1\\d{8}),(\\d+),([tur])\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        std::pair<int, int> lookup1(std::stoi(relation_data[1]), std::stoi(relation_data[2]));
        std::pair<int, int> lookup2(std::stoi(relation_data[3]), std::stoi(relation_data[4]));
        std::string class_type = relation_data[5];

        std::string table_name;
        if (class_type == "t")
        {
          table_name += "topicality";
        } else if (class_type == "u")
        {
          table_name += "usage";
        } else if (class_type == "r")
        {
          table_name += "regionality";
        }

        std::list<int> leftJoin;
        std::list<int> rightJoin;

        if ((lookup1.second == 0) && (wordsByWnid_.count(lookup1.first)))
        {
          std::transform(std::begin(wordsByWnid_.at(lookup1.first)), std::end(wordsByWnid_.at(lookup1.first)), std::back_inserter(leftJoin), [] (word* w) {
            return w->getId();
          });
        } else if (wordByWnidAndWnum_.count(lookup1)) {
          leftJoin.push_back(wordByWnidAndWnum_.at(lookup1)->getId());
        }

        if ((lookup2.second == 0) && (wordsByWnid_.count(lookup2.first)))
        {
          std::transform(std::begin(wordsByWnid_.at(lookup2.first)), std::end(wordsByWnid_.at(lookup2.first)), std::back_inserter(rightJoin), [] (word* w) {
            return w->getId();
          });
        } else if (wordByWnidAndWnum_.count(lookup2)) {
          rightJoin.push_back(wordByWnidAndWnum_.at(lookup2)->getId());
        }

        for (int word1 : leftJoin)
        {
          for (int word2 : rightJoin)
          {
            std::list<field> fields;
            fields.emplace_back("term_id", word1);
            fields.emplace_back("domain_id", word2);

            db_.insertIntoTable(table_name, std::move(fields));
          }
        }
      }
    }

    void generator::readWordNetCausality()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_cs.pl"));
      progress ppgs("Writing causality...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^cs\\((2\\d{8}),(2\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("effect_id", notion1.getId());
          fields.emplace_back("cause_id", notion2.getId());

          db_.insertIntoTable("causality", std::move(fields));
        }
      }
    }

    void generator::readWordNetEntailment()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_ent.pl"));
      progress ppgs("Writing entailment...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^ent\\((2\\d{8}),(2\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("given_id", notion1.getId());
          fields.emplace_back("entailment_id", notion2.getId());

          db_.insertIntoTable("entailment", std::move(fields));
        }
      }
    }

    void generator::readWordNetHypernymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_hyp.pl"));
      progress ppgs("Writing hypernymy...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^hyp\\(([12]\\d{8}),([12]\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("hyponym_id", notion1.getId());
          fields.emplace_back("hypernym_id", notion2.getId());

          db_.insertIntoTable("hypernymy", std::move(fields));
        }
      }
    }

    void generator::readWordNetInstantiation()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_ins.pl"));
      progress ppgs("Writing instantiation...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^ins\\((1\\d{8}),(1\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("instance_id", notion1.getId());
          fields.emplace_back("class_id", notion2.getId());

          db_.insertIntoTable("instantiation", std::move(fields));
        }
      }
    }

    void generator::readWordNetMemberMeronymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_mm.pl"));
      progress ppgs("Writing member meronymy...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^mm\\((1\\d{8}),(1\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("holonym_id", notion1.getId());
          fields.emplace_back("meronym_id", notion2.getId());

          db_.insertIntoTable("member_meronymy", std::move(fields));
        }
      }
    }

    void generator::readWordNetPartMeronymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_mp.pl"));
      progress ppgs("Writing part meronymy...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^mp\\((1\\d{8}),(1\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("holonym_id", notion1.getId());
          fields.emplace_back("meronym_id", notion2.getId());

          db_.insertIntoTable("part_meronymy", std::move(fields));
        }
      }
    }

    void generator::readWordNetSubstanceMeronymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_ms.pl"));
      progress ppgs("Writing substance meronymy...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^ms\\((1\\d{8}),(1\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("holonym_id", notion1.getId());
          fields.emplace_back("meronym_id", notion2.getId());

          db_.insertIntoTable("substance_meronymy", std::move(fields));
        }
      }
    }

    void generator::readWordNetPertainymy()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_per.pl"));
      progress ppgs("Writing pertainymy and mannernymy...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^per\\(([34]\\d{8}),(\\d+),([13]\\d{8}),(\\d+)\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        std::pair<int, int> lookup1(std::stoi(relation_data[1]), std::stoi(relation_data[2]));
        std::pair<int, int> lookup2(std::stoi(relation_data[3]), std::stoi(relation_data[4]));

        if (wordByWnidAndWnum_.count(lookup1) && wordByWnidAndWnum_.count(lookup2))
        {
          word& word1 = *wordByWnidAndWnum_.at(lookup1);
          word& word2 = *wordByWnidAndWnum_.at(lookup2);

          if (word1.getNotion().getPartOfSpeech() == part_of_speech::adjective)
          {
            std::list<field> fields;
            fields.emplace_back("pertainym_id", word1.getId());
            fields.emplace_back("noun_id", word2.getId());

            db_.insertIntoTable("pertainymy", std::move(fields));
          } else if (word1.getNotion().getPartOfSpeech() == part_of_speech::adverb)
          {
            std::list<field> fields;
            fields.emplace_back("mannernym_id", word1.getId());
            fields.emplace_back("adjective_id", word2.getId());

            db_.insertIntoTable("mannernymy", std::move(fields));
          }
        }
      }
    }

    void generator::readWordNetSpecification()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_sa.pl"));
      progress ppgs("Writing specifications...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^sa\\((23\\d{8}),(\\d+),(23\\d{8}),(\\d+)\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        std::pair<int, int> lookup1(std::stoi(relation_data[1]), std::stoi(relation_data[2]));
        std::pair<int, int> lookup2(std::stoi(relation_data[3]), std::stoi(relation_data[4]));

        if (wordByWnidAndWnum_.count(lookup1) && wordByWnidAndWnum_.count(lookup2))
        {
          word& word1 = *wordByWnidAndWnum_.at(lookup1);
          word& word2 = *wordByWnidAndWnum_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("general_id", word1.getId());
          fields.emplace_back("specific_id", word2.getId());

          db_.insertIntoTable("specification", std::move(fields));
        }
      }
    }

    void generator::readWordNetSimilarity()
    {
      std::list<std::string> lines(readFile(wordNetPath_ + "wn_sim.pl"));
      progress ppgs("Writing adjective similarity...", lines.size());
      for (auto line : lines)
      {
        ppgs.update();

        std::regex relation("^sim\\((3\\d{8}),(3\\d{8})\\)\\.");
        std::smatch relation_data;
        if (!std::regex_search(line, relation_data, relation))
        {
          continue;
        }

        int lookup1 = std::stoi(relation_data[1]);
        int lookup2 = std::stoi(relation_data[2]);

        if (notionByWnid_.count(lookup1) && notionByWnid_.count(lookup2))
        {
          notion& notion1 = *notionByWnid_.at(lookup1);
          notion& notion2 = *notionByWnid_.at(lookup2);

          std::list<field> fields;
          fields.emplace_back("adjective_1_id", notion1.getId());
          fields.emplace_back("adjective_2_id", notion2.getId());

          db_.insertIntoTable("similarity", std::move(fields));
        }
      }
    }

    std::list<std::string> generator::readFile(std::string path)
    {
      std::ifstream file(path);
      if (!file)
      {
        throw std::invalid_argument("Could not find file " + path);
      }

      std::list<std::string> lines;
      std::string line;
      while (std::getline(file, line))
      {
        if (line.back() == '\r')
        {
          line.pop_back();
        }

        lines.push_back(line);
      }

      return lines;
    }

    part_of_speech generator::partOfSpeechByWnid(int wnid)
    {
      switch (wnid / 100000000)
      {
        case 1: return part_of_speech::noun;
        case 2: return part_of_speech::verb;
        case 3: return part_of_speech::adjective;
        case 4: return part_of_speech::adverb;
        default: throw std::domain_error("Invalid WordNet synset ID: " + std::to_string(wnid));
      }
    }

    notion& generator::createNotion(part_of_speech partOfSpeech)
    {
      notions_.emplace_back(partOfSpeech);

      return notions_.back();
    }

    notion& generator::lookupOrCreateNotion(int wnid)
    {
      if (!notionByWnid_.count(wnid))
      {
        notions_.emplace_back(partOfSpeechByWnid(wnid), wnid);
        notionByWnid_[wnid] = &notions_.back();
      }

      return *notionByWnid_.at(wnid);
    }

    lemma& generator::lookupOrCreateLemma(std::string base_form)
    {
      if (!lemmaByBaseForm_.count(base_form))
      {
        lemmas_.emplace_back(lookupOrCreateForm(base_form));
        lemmaByBaseForm_[base_form] = &lemmas_.back();
      }

      return *lemmaByBaseForm_.at(base_form);
    }

    form& generator::lookupOrCreateForm(std::string text)
    {
      if (!formByText_.count(text))
      {
        forms_.emplace_back(text);
        formByText_[text] = &forms_.back();
      }

      return *formByText_[text];
    }

    template <typename... Args> word& generator::createWord(Args&&... args)
    {
      words_.emplace_back(std::forward<Args>(args)...);
      word& w = words_.back();

      wordsByBaseForm_[w.getLemma().getBaseForm().getText()].insert(&w);

      if (w.getNotion().hasWnid())
      {
        wordsByWnid_[w.getNotion().getWnid()].insert(&w);
      }

      return w;
    }

    group& generator::createGroup(xmlNodePtr top)
    {
      groups_.emplace_back();
      group& grp = groups_.back();

      xmlChar* key;

      for (xmlNodePtr node = top->xmlChildrenNode; node != nullptr; node = node->next)
      {
        if (!xmlStrcmp(node->name, reinterpret_cast<const xmlChar*>("SUBCLASSES")))
        {
          for (xmlNodePtr subclass = node->xmlChildrenNode; subclass != nullptr; subclass = subclass->next)
          {
            if (!xmlStrcmp(subclass->name, reinterpret_cast<const xmlChar*>("VNSUBCLASS")))
            {
              try
              {
                group& subgrp = createGroup(subclass);
                subgrp.setParent(grp);
              } catch (const std::exception& e)
              {
                key = xmlGetProp(subclass, reinterpret_cast<const xmlChar*>("ID"));

                if (key == nullptr)
                {
                  std::throw_with_nested(std::logic_error("Error parsing IDless subgroup"));
                } else {
                  std::string subgroupId(reinterpret_cast<const char*>(key));
                  xmlFree(key);

                  std::throw_with_nested(std::logic_error("Error parsing subgroup " + subgroupId));
                }
              }
            }
          }
        } else if (!xmlStrcmp(node->name, reinterpret_cast<const xmlChar*>("MEMBERS")))
        {
          for (xmlNodePtr member = node->xmlChildrenNode; member != nullptr; member = member->next)
          {
            if (!xmlStrcmp(member->name, reinterpret_cast<const xmlChar*>("MEMBER")))
            {
              key = xmlGetProp(member, reinterpret_cast<const xmlChar*>("wn"));
              std::string wnSenses(reinterpret_cast<const char*>(key));
              xmlFree(key);

              auto wnSenseKeys = split<std::list<std::string>>(wnSenses, " ");
              if (!wnSenseKeys.empty())
              {
                std::list<std::string> tempKeys;

                std::transform(std::begin(wnSenseKeys), std::end(wnSenseKeys), std::back_inserter(tempKeys), [] (std::string sense) {
                  return sense + "::";
                });

                std::list<std::string> filteredKeys;

                std::remove_copy_if(std::begin(tempKeys), std::end(tempKeys), std::back_inserter(filteredKeys), [&] (std::string sense) {
                  return !wnSenseKeys_.count(sense);
                });

                wnSenseKeys = std::move(filteredKeys);
              }

              if (!wnSenseKeys.empty())
              {
                for (std::string sense : wnSenseKeys)
                {
                  word& wordSense = *wnSenseKeys_[sense];
                  wordSense.setVerbGroup(grp);
                }
              } else {
                key = xmlGetProp(member, reinterpret_cast<const xmlChar*>("name"));
                std::string memberName(reinterpret_cast<const char*>(key));
                xmlFree(key);

                notion& n = createNotion(part_of_speech::verb);
                lemma& l = lookupOrCreateLemma(memberName);
                word& w = createWord(n, l);

                w.setVerbGroup(grp);
              }
            }
          }
        } else if (!xmlStrcmp(node->name, reinterpret_cast<const xmlChar*>("THEMROLES")))
        {
          for (xmlNodePtr roletopnode = node->xmlChildrenNode; roletopnode != nullptr; roletopnode = roletopnode->next)
          {
            if (!xmlStrcmp(roletopnode->name, reinterpret_cast<const xmlChar*>("THEMROLE")))
            {
              key = xmlGetProp(roletopnode, reinterpret_cast<const xmlChar*>("type"));
              std::string roleName = reinterpret_cast<const char*>(key);
              xmlFree(key);

              selrestr roleSelrestrs;
              for (xmlNodePtr rolenode = roletopnode->xmlChildrenNode; rolenode != nullptr; rolenode = rolenode->next)
              {
                if (!xmlStrcmp(rolenode->name, reinterpret_cast<const xmlChar*>("SELRESTRS")))
                {
                  roleSelrestrs = parseSelrestr(rolenode);
                }
              }

              grp.addRole({roleName, std::move(roleSelrestrs)});
            }
          }
        } else if (!xmlStrcmp(node->name, reinterpret_cast<const xmlChar*>("FRAMES")))
        {
          for (xmlNodePtr frametopnode = node->xmlChildrenNode; frametopnode != nullptr; frametopnode = frametopnode->next)
          {
            if (!xmlStrcmp(frametopnode->name, reinterpret_cast<const xmlChar*>("FRAME")))
            {
              frames_.emplace_back();
              frame& fr = frames_.back();

              for (xmlNodePtr framenode = frametopnode->xmlChildrenNode; framenode != nullptr; framenode = framenode->next)
              {
                if (!xmlStrcmp(framenode->name, reinterpret_cast<const xmlChar*>("SYNTAX")))
                {
                  for (xmlNodePtr syntaxnode = framenode->xmlChildrenNode; syntaxnode != nullptr; syntaxnode = syntaxnode->next)
                  {
                    if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("NP")))
                    {
                      key = xmlGetProp(syntaxnode, reinterpret_cast<const xmlChar*>("value"));
                      std::string partRole = reinterpret_cast<const char*>(key);
                      xmlFree(key);

                      selrestr partSelrestrs;
                      std::set<std::string> partSynrestrs;

                      for (xmlNodePtr npnode = syntaxnode->xmlChildrenNode; npnode != nullptr; npnode = npnode->next)
                      {
                        if (!xmlStrcmp(npnode->name, reinterpret_cast<const xmlChar*>("SYNRESTRS")))
                        {
                          for (xmlNodePtr synrestr = npnode->xmlChildrenNode; synrestr != nullptr; synrestr = synrestr->next)
                          {
                            if (!xmlStrcmp(synrestr->name, reinterpret_cast<const xmlChar*>("SYNRESTR")))
                            {
                              key = xmlGetProp(synrestr, reinterpret_cast<const xmlChar*>("type"));
                              partSynrestrs.insert(reinterpret_cast<const char*>(key));
                              xmlFree(key);
                            }
                          }
                        }

                        if (!xmlStrcmp(npnode->name, reinterpret_cast<const xmlChar*>("SELRESTRS")))
                        {
                          partSelrestrs = parseSelrestr(npnode);
                        }
                      }

                      fr.push_back(part::createNounPhrase(std::move(partRole), std::move(partSelrestrs), std::move(partSynrestrs)));
                    } else if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("VERB")))
                    {
                      fr.push_back(part::createVerb());
                    } else if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("PREP")))
                    {
                      std::set<std::string> partChoices;
                      bool partLiteral;

                      if (xmlHasProp(syntaxnode, reinterpret_cast<const xmlChar*>("value")))
                      {
                        partLiteral = true;

                        key = xmlGetProp(syntaxnode, reinterpret_cast<const xmlChar*>("value"));
                        std::string choicesStr = reinterpret_cast<const char*>(key);
                        xmlFree(key);

                        for (std::string choice : split<std::list<std::string>>(choicesStr, " "))
                        {
                          int chloc;
                          while ((chloc = choice.find_first_of("_")) != std::string::npos)
                          {
                            choice.replace(chloc, 1, " ");
                          }

                          partChoices.insert(std::move(choice));
                        }
                      } else {
                        partLiteral = false;

                        for (xmlNodePtr npnode = syntaxnode->xmlChildrenNode; npnode != nullptr; npnode = npnode->next)
                        {
                          if (!xmlStrcmp(npnode->name, reinterpret_cast<const xmlChar*>("SELRESTRS")))
                          {
                            for (xmlNodePtr synrestr = npnode->xmlChildrenNode; synrestr != nullptr; synrestr = synrestr->next)
                            {
                              if (!xmlStrcmp(synrestr->name, reinterpret_cast<const xmlChar*>("SELRESTR")))
                              {
                                key = xmlGetProp(synrestr, reinterpret_cast<const xmlChar*>("type"));
                                partChoices.insert(reinterpret_cast<const char*>(key));
                                xmlFree(key);
                              }
                            }
                          }
                        }
                      }

                      fr.push_back(part::createPreposition(std::move(partChoices), partLiteral));
                    } else if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("ADJ")))
                    {
                      fr.push_back(part::createAdjective());
                    } else if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("ADV")))
                    {
                      fr.push_back(part::createAdverb());
                    } else if (!xmlStrcmp(syntaxnode->name, reinterpret_cast<const xmlChar*>("LEX")))
                    {
                      key = xmlGetProp(syntaxnode, reinterpret_cast<const xmlChar*>("value"));
                      std::string literalValue = reinterpret_cast<const char*>(key);
                      xmlFree(key);

                      fr.push_back(part::createLiteral(literalValue));
                    } else {
                      continue;
                    }
                  }

                  grp.addFrame(fr);
                }
              }
            }
          }
        }
      }

      return grp;
    }

    selrestr generator::parseSelrestr(xmlNodePtr top)
    {
      xmlChar* key;

      if (!xmlStrcmp(top->name, reinterpret_cast<const xmlChar*>("SELRESTRS")))
      {
        if (xmlChildElementCount(top) == 0)
        {
          return {};
        } else if (xmlChildElementCount(top) == 1)
        {
          return parseSelrestr(xmlFirstElementChild(top));
        } else {
          bool orlogic = false;
          if (xmlHasProp(top, reinterpret_cast<const xmlChar*>("logic")))
          {
            key = xmlGetProp(top, reinterpret_cast<const xmlChar*>("logic"));
            if (!xmlStrcmp(key, reinterpret_cast<const xmlChar*>("or")))
            {
              orlogic = true;
            }

            xmlFree(key);
          }

          std::list<selrestr> children;
          for (xmlNodePtr selrestr = top->xmlChildrenNode; selrestr != nullptr; selrestr = selrestr->next)
          {
            if (!xmlStrcmp(selrestr->name, reinterpret_cast<const xmlChar*>("SELRESTRS"))
              || !xmlStrcmp(selrestr->name, reinterpret_cast<const xmlChar*>("SELRESTR")))
            {
              children.push_back(parseSelrestr(selrestr));
            }
          }

          return selrestr(children, orlogic);
        }
      } else if (!xmlStrcmp(top->name, reinterpret_cast<const xmlChar*>("SELRESTR")))
      {
        key = xmlGetProp(top, reinterpret_cast<const xmlChar*>("Value"));
        bool selPos = (std::string(reinterpret_cast<const char*>(key)) == "+");
        xmlFree(key);

        key = xmlGetProp(top, reinterpret_cast<const xmlChar*>("type"));
        std::string selRestriction = reinterpret_cast<const char*>(key);
        xmlFree(key);

        return selrestr(selRestriction, selPos);
      } else {
        throw std::logic_error("Badly formatted selrestr");
      }
    }

  };
};
