#ifndef GENERATOR_H_5B61CBC5
#define GENERATOR_H_5B61CBC5

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <set>
#include <libxml/parser.h>
#include <hkutil/database.h>
#include <filesystem>
#include "notion.h"
#include "word.h"
#include "lemma.h"
#include "form.h"
#include "pronunciation.h"
#include "group.h"
#include "frame.h"

namespace verbly {

  enum class part_of_speech;

  namespace generator {

    class generator {
    public:

      // Constructor

      generator(
        std::string verbNetPath,
        std::string agidPath,
        std::string wordNetPath,
        std::string cmudictPath,
        std::string imageNetPath,
        std::string outputPath,
        std::string imageNetOutput);

      // Action

      void run();

    private:

      // Subroutines

      void readWordNetSynsets();

      void readAdjectivePositioning();

      void readImageNetUrls();

      void readWordNetSenseKeys();

      void readVerbNet();

      void readAgidInflections();

      void readPrepositions();

      void readCmudictPronunciations();

      void writeSchema();

      void writeVersion();

      void dumpObjects();

      void writeMerography();

      void writeMerophony();

      void readWordNetAntonymy();

      void readWordNetVariation();

      void readWordNetClasses();

      void readWordNetCausality();

      void readWordNetEntailment();

      void readWordNetHypernymy();

      void readWordNetInstantiation();

      void readWordNetMemberMeronymy();

      void readWordNetPartMeronymy();

      void readWordNetSubstanceMeronymy();

      void readWordNetPertainymy();

      void readWordNetSpecification();

      void readWordNetSimilarity();

      void analyzeDatabase();

      // Helpers

      std::list<std::string> readFile(std::string path, bool uniq = false);

      inline part_of_speech partOfSpeechByWnid(int wnid);

      notion& createNotion(part_of_speech partOfSpeech);

      notion& lookupOrCreateNotion(int wnid);

      lemma& lookupOrCreateLemma(std::string base_form);

      form& lookupOrCreateForm(std::string text);

      template <typename... Args> word& createWord(Args&&... args);

      void createGroup(xmlNodePtr top, const group* parent = nullptr);

      // Input

      std::string verbNetPath_;
      std::string agidPath_;
      std::string wordNetPath_;
      std::string cmudictPath_;
      std::string imageNetPath_;

      // Output

      hatkirby::database db_;
      std::filesystem::path imageNetOutput_;

      // Data

      std::list<notion> notions_;
      std::list<word> words_;
      std::list<lemma> lemmas_;
      std::list<form> forms_;
      std::list<pronunciation> pronunciations_;
      std::list<group> groups_;

      // Indexes

      std::map<int, notion*> notionByWnid_;
      std::map<int, std::set<word*>> wordsByWnid_;
      std::map<std::pair<int, int>, word*> wordByWnidAndWnum_;
      std::map<std::string, std::set<word*>> wordsByBaseForm_;
      std::map<std::string, lemma*> lemmaByBaseForm_;
      std::unordered_map<std::string, form*> formByText_;
      std::map<std::string, pronunciation*> pronunciationByPhonemes_;
      std::unordered_map<std::string, pronunciation*> pronunciationByBlankPhonemes_;
      std::map<std::string, int> anagramSets_;
      std::map<std::string, int> anaphoneSets_;

      // Caches

      std::map<std::string, word*> wnSenseKeys_;

    };

  };
};

#endif /* end of include guard: GENERATOR_H_5B61CBC5 */
