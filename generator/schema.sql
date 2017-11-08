CREATE TABLE `version` (
  `major` INTEGER NOT NULL,
  `minor` INTEGER NOT NULL
);

CREATE TABLE `notions` (
  `notion_id` INTEGER PRIMARY KEY,
  `part_of_speech` SMALLINT NOT NULL,
  `wnid` INTEGER,
  `images` INTEGER
);

CREATE UNIQUE INDEX `notion_by_wnid` ON `notions`(`wnid`);

CREATE TABLE `hypernymy` (
  `hypernym_id` INTEGER NOT NULL,
  `hyponym_id` INTEGER NOT NULL
);

CREATE INDEX `hyponym_of` ON `hypernymy`(`hypernym_id`,`hyponym_id`);
CREATE INDEX `hypernym_of` ON `hypernymy`(`hyponym_id`,`hypernym_id`);

CREATE TABLE `instantiation` (
  `class_id` INTEGER NOT NULL,
  `instance_id` INTEGER NOT NULL
);

CREATE INDEX `instance_of` ON `instantiation`(`class_id`,`instance_id`);
CREATE INDEX `class_of` ON `instantiation`(`instance_id`,`class_id`);

CREATE TABLE `member_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL
);

CREATE INDEX `member_holonym_of` ON `member_meronymy`(`meronym_id`,`holonym_id`);
CREATE INDEX `member_meronym_of` ON `member_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `part_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL
);

CREATE INDEX `part_holonym_of` ON `part_meronymy`(`meronym_id`,`holonym_id`);
CREATE INDEX `part_meronym_of` ON `part_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `substance_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL
);

CREATE INDEX `substance_holonym_of` ON `substance_meronymy`(`meronym_id`,`holonym_id`);
CREATE INDEX `substance_meronym_of` ON `substance_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `variation` (
  `noun_id` INTEGER NOT NULL,
  `adjective_id` INTEGER NOT NULL
);

CREATE INDEX `variant_of` ON `variation`(`noun_id`,`adjective_id`);
CREATE INDEX `attribute_of` ON `variation`(`adjective_id`,`noun_id`);

CREATE TABLE `similarity` (
  `adjective_1_id` INTEGER NOT NULL,
  `adjective_2_id` INTEGER NOT NULL
);

CREATE INDEX `similar_to` ON `similarity`(`adjective_1_id`,`adjective_2_id`);

CREATE TABLE `is_a` (
  `notion_id` INTEGER NOT NULL,
  `groupname` VARCHAR(32) NOT NULL
);

CREATE TABLE `entailment` (
  `given_id` INTEGER NOT NULL,
  `entailment_id` INTEGER NOT NULL
);

CREATE INDEX `entailment_of` ON `entailment`(`given_id`,`entailment_id`);
CREATE INDEX `entailed_by` ON `entailment`(`entailment_id`,`given_id`);

CREATE TABLE `causality` (
  `cause_id` INTEGER NOT NULL,
  `effect_id` INTEGER NOT NULL
);

CREATE INDEX `effect_of` ON `causality`(`cause_id`,`effect_id`);
CREATE INDEX `cause_of` ON `causality`(`effect_id`,`cause_id`);

CREATE TABLE `words` (
  `word_id` INTEGER PRIMARY KEY,
  `notion_id` INTEGER NOT NULL,
  `lemma_id` INTEGER NOT NULL,
  `tag_count` INTEGER,
  `position` SMALLINT,
  `group_id` INTEGER
);

CREATE INDEX `notions_lemmas` ON `words`(`notion_id`,`lemma_id`);
CREATE INDEX `lemmas_notions` ON `words`(`lemma_id`,`notion_id`);
CREATE INDEX `group_words` ON `words`(`group_id`);

CREATE TABLE `antonymy` (
  `antonym_1_id` INTEGER NOT NULL,
  `antonym_2_id` INTEGER NOT NULL
);

CREATE INDEX `antonym_of` ON `antonymy`(`antonym_1_id`,`antonym_2_id`);

CREATE TABLE `specification` (
  `general_id` INTEGER NOT NULL,
  `specific_id` INTEGER NOT NULL
);

CREATE INDEX `specification_of` ON `specification`(`general_id`,`specific_id`);
CREATE INDEX `generalization_of` ON `specification`(`specific_id`,`general_id`);

CREATE TABLE `pertainymy` (
  `noun_id` INTEGER NOT NULL,
  `pertainym_id` INTEGER NOT NULL
);

CREATE INDEX `pertainym_of` ON `pertainymy`(`noun_id`,`pertainym_id`);
CREATE INDEX `anti_pertainym_of` ON `pertainymy`(`pertainym_id`,`noun_id`);

CREATE TABLE `mannernymy` (
  `adjective_id` INTEGER NOT NULL,
  `mannernym_id` INTEGER NOT NULL
);

CREATE INDEX `mannernym_of` ON `mannernymy`(`adjective_id`,`mannernym_id`);
CREATE INDEX `anti_mannernym_of` ON `mannernymy`(`mannernym_id`,`adjective_id`);

CREATE TABLE `usage` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL
);

CREATE INDEX `usage_term_of` ON `usage`(`domain_id`,`term_id`);
CREATE INDEX `usage_domain_of` ON `usage`(`term_id`,`domain_id`);

CREATE TABLE `topicality` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL
);

CREATE INDEX `topical_term_of` ON `topicality`(`domain_id`,`term_id`);
CREATE INDEX `topical_domain_of` ON `topicality`(`term_id`,`domain_id`);

CREATE TABLE `regionality` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL
);

CREATE INDEX `regional_term_of` ON `regionality`(`domain_id`,`term_id`);
CREATE INDEX `regional_domain_of` ON `regionality`(`term_id`,`domain_id`);

CREATE TABLE `forms` (
  `form_id` INTEGER PRIMARY KEY,
  `form` VARCHAR(32) NOT NULL,
  `complexity` SMALLINT NOT NULL,
  `proper` SMALLINT NOT NULL,
  `length` SMALLINT NOT NULL
);

CREATE UNIQUE INDEX `form_by_string` ON `forms`(`form`);

CREATE TABLE `lemmas_forms` (
  `lemma_id` INTEGER NOT NULL,
  `form_id` INTEGER NOT NULL,
  `category` SMALLINT NOT NULL
);

CREATE INDEX `form_of` ON `lemmas_forms`(`lemma_id`,`category`,`form_id`);
CREATE INDEX `lemma_of` ON `lemmas_forms`(`form_id`,`category`,`lemma_id`);

CREATE TABLE `pronunciations` (
  `pronunciation_id` INTEGER PRIMARY KEY,
  `phonemes` VARCHAR(64) NOT NULL,
  `prerhyme` VARCHAR(8),
  `rhyme` VARCHAR(64),
  `syllables` INTEGER NOT NULL,
  `stress` VARCHAR(64) NOT NULL
);

CREATE INDEX `rhymes_with` ON `pronunciations`(`rhyme`,`prerhyme`);

CREATE TABLE `forms_pronunciations` (
  `form_id` INTEGER NOT NULL,
  `pronunciation_id` INTEGER NOT NULL
);

CREATE INDEX `pronunciation_of` ON `forms_pronunciations`(`form_id`,`pronunciation_id`);
CREATE INDEX `spelling_of` ON `forms_pronunciations`(`pronunciation_id`,`form_id`);

CREATE TABLE `frames` (
  `frame_id` INTEGER NOT NULL,
  `group_id` INTEGER NOT NULL,
  'length' INTEGER NOT NULL
);

CREATE INDEX `frames_in` ON `frames`(`group_id`);

CREATE TABLE `parts` (
  `part_id` INTEGER PRIMARY KEY,
  `frame_id` INTEGER NOT NULL,
  `part_index` INTEGER NOT NULL,
  `type` INTEGER NOT NULL,
  `role` VARCHAR(16),
  `prepositions` BLOB,
  `preposition_literality` SMALLINT,
  `literal_value` VARCHAR(64)
);

CREATE INDEX `parts_of` ON `parts`(`frame_id`);
CREATE UNIQUE INDEX `part_by_frame_index` ON `parts`(`frame_id`, `part_index`);

CREATE TABLE `synrestrs` (
  `part_id` INTEGER NOT NULL,
  `synrestr` VARCHAR(32) NOT NULL
);

CREATE INDEX `synrestrs_for` ON `synrestrs`(`part_id`);

CREATE TABLE `selrestrs` (
  `part_id` INTEGER NOT NULL,
  `selrestr` VARCHAR(32) NOT NULL
);

CREATE INDEX `selrestrs_for` ON `selrestrs`(`part_id`);
