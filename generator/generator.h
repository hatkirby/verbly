#ifndef GENERATOR_H_5B61CBC5
#define GENERATOR_H_5B61CBC5

#include <string>
#include <map>
#include <list>
#include <set>
#include <libxml/parser.h>
#include "database.h"
#include "notion.h"
#include "word.h"
#include "lemma.h"
#include "form.h"
#include "pronunciation.h"
#include "group.h"
#include "frame.h"

namespace verbly {

  enum class part_of_speech;
  class selrestr;

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
        std::string outputPath);

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

      void dumpObjects();

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

      // Helpers

      std::list<std::string> readFile(std::string path);

      inline part_of_speech partOfSpeechByWnid(int wnid);

      notion& createNotion(part_of_speech partOfSpeech);

      notion& lookupOrCreateNotion(int wnid);

      lemma& lookupOrCreateLemma(std::string base_form);

      form& lookupOrCreateForm(std::string text);

      template <typename... Args> word& createWord(Args&&... args);

      void createGroup(xmlNodePtr top, const group* parent = nullptr);

      selrestr parseSelrestr(xmlNodePtr top);

      // Input

      std::string verbNetPath_;
      std::string agidPath_;
      std::string wordNetPath_;
      std::string cmudictPath_;
      std::string imageNetPath_;

      // Output

      database db_;

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
      std::map<std::string, form*> formByText_;

      // Caches

      std::map<std::string, word*> wnSenseKeys_;

    };

  };
};

#endif /* end of include guard: GENERATOR_H_5B61CBC5 */
