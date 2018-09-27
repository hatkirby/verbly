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
  `hyponym_id` INTEGER NOT NULL,
  PRIMARY KEY (`hypernym_id`,`hyponym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_hypernymy` ON `hypernymy`(`hyponym_id`,`hypernym_id`);

CREATE TABLE `instantiation` (
  `class_id` INTEGER NOT NULL,
  `instance_id` INTEGER NOT NULL,
  PRIMARY KEY (`class_id`,`instance_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_instantiation` ON `instantiation`(`instance_id`,`class_id`);

CREATE TABLE `member_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  PRIMARY KEY (`meronym_id`,`holonym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_member_meronymy` ON `member_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `part_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  PRIMARY KEY (`meronym_id`,`holonym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_part_meronymy` ON `part_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `substance_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  PRIMARY KEY (`meronym_id`,`holonym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_substance_meronymy` ON `substance_meronymy`(`holonym_id`,`meronym_id`);

CREATE TABLE `variation` (
  `noun_id` INTEGER NOT NULL,
  `adjective_id` INTEGER NOT NULL,
  PRIMARY KEY (`noun_id`,`adjective_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_variation` ON `variation`(`adjective_id`,`noun_id`);

CREATE TABLE `similarity` (
  `adjective_1_id` INTEGER NOT NULL,
  `adjective_2_id` INTEGER NOT NULL,
  PRIMARY KEY (`adjective_1_id`,`adjective_2_id`)
) WITHOUT ROWID;

CREATE TABLE `is_a` (
  `notion_id` INTEGER NOT NULL,
  `groupname` VARCHAR(32) NOT NULL,
  PRIMARY KEY (`notion_id`,`groupname`)
) WITHOUT ROWID;

CREATE TABLE `entailment` (
  `given_id` INTEGER NOT NULL,
  `entailment_id` INTEGER NOT NULL,
  PRIMARY KEY (`given_id`,`entailment_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_entailment` ON `entailment`(`entailment_id`,`given_id`);

CREATE TABLE `causality` (
  `cause_id` INTEGER NOT NULL,
  `effect_id` INTEGER NOT NULL,
  PRIMARY KEY (`cause_id`,`effect_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_causality` ON `causality`(`effect_id`,`cause_id`);

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
  `antonym_2_id` INTEGER NOT NULL,
  PRIMARY KEY (`antonym_1_id`,`antonym_2_id`)
) WITHOUT ROWID;

CREATE TABLE `specification` (
  `general_id` INTEGER NOT NULL,
  `specific_id` INTEGER NOT NULL,
  PRIMARY KEY (`general_id`,`specific_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_specification` ON `specification`(`specific_id`,`general_id`);

CREATE TABLE `pertainymy` (
  `noun_id` INTEGER NOT NULL,
  `pertainym_id` INTEGER NOT NULL,
  PRIMARY KEY (`noun_id`,`pertainym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_pertainymy` ON `pertainymy`(`pertainym_id`,`noun_id`);

CREATE TABLE `mannernymy` (
  `adjective_id` INTEGER NOT NULL,
  `mannernym_id` INTEGER NOT NULL,
  PRIMARY KEY (`adjective_id`,`mannernym_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_mannernymy` ON `mannernymy`(`mannernym_id`,`adjective_id`);

CREATE TABLE `usage` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL,
  PRIMARY KEY (`domain_id`,`term_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_usage` ON `usage`(`term_id`,`domain_id`);

CREATE TABLE `topicality` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL,
  PRIMARY KEY (`domain_id`,`term_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_topicality` ON `topicality`(`term_id`,`domain_id`);

CREATE TABLE `regionality` (
  `domain_id` INTEGER NOT NULL,
  `term_id` INTEGER NOT NULL,
  PRIMARY KEY (`domain_id`,`term_id`)
) WITHOUT ROWID;

CREATE INDEX `reverse_regionality` ON `regionality`(`term_id`,`domain_id`);

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
  `category` SMALLINT NOT NULL,
  PRIMARY KEY (`lemma_id`,`category`,`form_id`)
) WITHOUT ROWID;

CREATE INDEX `forms_lemmas` ON `lemmas_forms`(`form_id`,`category`,`lemma_id`);

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
  `pronunciation_id` INTEGER NOT NULL,
  PRIMARY KEY (`form_id`,`pronunciation_id`)
) WITHOUT ROWID;

CREATE INDEX `pronunciations_forms` ON `forms_pronunciations`(`pronunciation_id`,`form_id`);

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
