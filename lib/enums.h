#ifndef ENUMS_H_260BA847
#define ENUMS_H_260BA847

namespace verbly {
    
  enum class part_of_speech {
    noun = 0,
    adjective = 1,
    adverb = 2,
    verb = 3,
    preposition = 4
  };
  
  enum class positioning {
    undefined = -1,
    predicate = 0,
    attributive = 1,
    postnominal = 2
  };
  
  enum class inflection {
    base = 0,
    plural = 1,
    comparative = 2,
    superlative = 3,
    past_tense = 4,
    past_participle = 5,
    ing_form = 6,
    s_form = 7
  };
  
  enum class object {
    undefined = -1,
    notion = 0,
    word = 1,
    group = 2,
    frame = 3,
    lemma = 4,
    form = 5,
    pronunciation = 6
  };
  
};

#endif /* end of include guard: ENUMS_H_260BA847 */
