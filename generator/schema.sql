DROP TABLE IF EXISTS `verbs`;
CREATE TABLE `verbs` (
  `verb_id` INTEGER PRIMARY KEY,
  `infinitive` VARCHAR(32) NOT NULL,
  `past_tense` VARCHAR(32) NOT NULL,
  `past_participle` VARCHAR(32) NOT NULL,
  `ing_form` VARCHAR(32) NOT NULL,
  `s_form` VARCHAR(32) NOT NULL
);

DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `group_id` INTEGER PRIMARY KEY,
  `parent_id` INTEGER,
  FOREIGN KEY (`parent_id`) REFERENCES `groups`(`group_id`)
);

DROP TABLE IF EXISTS `frames`;
CREATE TABLE `frames` (
  `frame_id` INTEGER PRIMARY KEY,
  `group_id` INTEGER NOT NULL,
  `data` BLOB NOT NULL,
  FOREIGN KEY (`group_id`) REFERENCES `groups`(`group_id`)
);

DROP TABLE IF EXISTS `verb_groups`;
CREATE TABLE `verb_groups` (
  `verb_id` INTEGER NOT NULL,
  `group_id` INTEGER NOT NULL,
  FOREIGN KEY (`verb_id`) REFERENCES `verbs`(`verb_id`),
  FOREIGN KEY (`group_id`) REFERENCES `groups`(`group_id`)
);

DROP TABLE IF EXISTS `adjectives`;
CREATE TABLE `adjectives` (
  `adjective_id` INTEGER PRIMARY KEY,
  `base_form` VARCHAR(32) NOT NULL,
  `comparative` VARCHAR(32),
  `superlative` VARCHAR(32),
  `position` CHAR(1)
);

DROP TABLE IF EXISTS `adverbs`;
CREATE TABLE `adverbs` (
  `adverb_id` INTEGER PRIMARY KEY,
  `base_form` VARCHAR(32) NOT NULL,
  `comparative` VARCHAR(32),
  `superlative` VARCHAR(32)
);

DROP TABLE IF EXISTS `nouns`;
CREATE TABLE `nouns` (
  `noun_id` INTEGER PRIMARY KEY,
  `singular` VARCHAR(32) NOT NULL,
  `plural` VARCHAR(32)
);

DROP TABLE IF EXISTS `hypernymy`;
CREATE TABLE `hypernymy` (
  `hypernym_id` INTEGER NOT NULL,
  `hyponym_id` INTEGER NOT NULL,
  FOREIGN KEY (`hypernym_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`hyponym_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `instantiation`;
CREATE TABLE `instantiation` (
  `class_id` INTEGER NOT NULL,
  `instance_id` INTEGER NOT NULL,
  FOREIGN KEY (`class_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`instance_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `member_meronymy`;
CREATE TABLE `member_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  FOREIGN KEY (`meronym_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`holonym_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `part_meronymy`;
CREATE TABLE `part_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  FOREIGN KEY (`meronym_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`holonym_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `substance_meronymy`;
CREATE TABLE `substance_meronymy` (
  `meronym_id` INTEGER NOT NULL,
  `holonym_id` INTEGER NOT NULL,
  FOREIGN KEY (`meronym_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`holonym_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `variation`;
CREATE TABLE `variation` (
  `noun_id` INTEGER NOT NULL,
  `adjective_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`adjective_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `noun_antonymy`;
CREATE TABLE `noun_antonymy` (
  `noun_1_id` INTEGER NOT NULL,
  `noun_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_1_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`noun_2_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `adjective_antonymy`;
CREATE TABLE `adjective_antonymy` (
  `adjective_1_id` INTEGER NOT NULL,
  `adjective_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adjective_1_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`adjective_2_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `adverb_antonymy`;
CREATE TABLE `adverb_antonymy` (
  `adverb_1_id` INTEGER NOT NULL,
  `adverb_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adverb_1_id`) REFERENCES `adverbs`(`adverb_id`),
  FOREIGN KEY (`adverb_2_id`) REFERENCES `adverbs`(`adverb_id`)
);

DROP TABLE IF EXISTS `specification`;
CREATE TABLE `specification` (
  `general_id` INTEGER NOT NULL,
  `specific_id` INTEGER NOT NULL,
  FOREIGN KEY (`general_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`specific_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `pertainymy`;
CREATE TABLE `pertainymy` (
  `noun_id` INTEGER NOT NULL,
  `pertainym_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`pertainym_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `mannernymy`;
CREATE TABLE `mannernymy` (
  `adjective_id` INTEGER NOT NULL,
  `mannernym_id` INTEGER NOT NULL,
  FOREIGN KEY (`adjective_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`mannernym_id`) REFERENCES `adverbs`(`adverb_id`)
);

DROP TABLE IF EXISTS `noun_synonymy`;
CREATE TABLE `noun_synonymy` (
  `noun_1_id` INTEGER NOT NULL,
  `noun_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_1_id`) REFERENCES `nouns`(`nouns_id`),
  FOREIGN KEY (`noun_2_id`) REFERENCES `nouns`(`nouns_id`)
);

DROP TABLE IF EXISTS `adjective_synonymy`;
CREATE TABLE `adjective_synonymy` (
  `adjective_1_id` INTEGER NOT NULL,
  `adjective_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adjective_1_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`adjective_2_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `adverb_synonymy`;
CREATE TABLE `adverb_synonymy` (
  `adverb_1_id` INTEGER NOT NULL,
  `adverb_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adverb_1_id`) REFERENCES `adverbs`(`adverb_id`),
  FOREIGN KEY (`adverb_2_id`) REFERENCES `adverbs`(`adverb_id`)
);

DROP TABLE IF EXISTS `noun_pronunciations`;
CREATE TABLE `noun_pronunciations` (
  `noun_id` INTEGER NOT NULL,
  `pronunciation` VARCHAR(64) NOT NULL,
  FOREIGN KEY (`noun_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `verb_pronunciations`;
CREATE TABLE `verb_pronunciations` (
  `verb_id` INTEGER NOT NULL,
  `pronunciation` VARCHAR(64) NOT NULL,
  FOREIGN KEY (`verb_id`) REFERENCES `verbs`(`verb_id`)
);

DROP TABLE IF EXISTS `adjective_pronunciations`;
CREATE TABLE `adjective_pronunciations` (
  `adjective_id` INTEGER NOT NULL,
  `pronunciation` VARCHAR(64) NOT NULL,
  FOREIGN KEY (`adjective_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `adverb_pronunciations`;
CREATE TABLE `adverb_pronunciations` (
  `adverb_id` INTEGER NOT NULL,
  `pronunciation` VARCHAR(64) NOT NULL,
  FOREIGN KEY (`adverb_id`) REFERENCES `adverbs`(`adverb_id`)
);

DROP TABLE IF EXISTS `noun_noun_derivation`;
CREATE TABLE `noun_noun_derivation` (
  `noun_1_id` INTEGER NOT NULL,
  `noun_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_1_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`noun_2_id`) REFERENCES `nouns`(`noun_id`)
);

DROP TABLE IF EXISTS `noun_adjective_derivation`;
CREATE TABLE `noun_adjective_derivation` (
  `noun_id` INTEGER NOT NULL,
  `adjective_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`adjective_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `noun_adverb_derivation`;
CREATE TABLE `noun_adverb_derivation` (
  `noun_id` INTEGER NOT NULL,
  `adverb_id` INTEGER NOT NULL,
  FOREIGN KEY (`noun_id`) REFERENCES `nouns`(`noun_id`),
  FOREIGN KEY (`adverb_id`) REFERENCES `adverbs`(`adverb_id`)
);

DROP TABLE IF EXISTS `adjective_adjective_derivation`;
CREATE TABLE `adjective_adjective_derivation` (
  `adjective_1_id` INTEGER NOT NULL,
  `adjective_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adjective_1_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`adjective_2_id`) REFERENCES `adjectives`(`adjective_id`)
);

DROP TABLE IF EXISTS `adjective_adverb_derivation`;
CREATE TABLE `adjective_adverb_derivation` (
  `adjective_id` INTEGER NOT NULL,
  `adverb_id` INTEGER NOT NULL,
  FOREIGN KEY (`adjective_id`) REFERENCES `adjectives`(`adjective_id`),
  FOREIGN KEY (`adverb_id`) REFERENCES `adverbs`(`adjective_id`)
);

DROP TABLE IF EXISTS `adverb_adverb_derivation`;
CREATE TABLE `adverb_adverb_derivation` (
  `adverb_1_id` INTEGER NOT NULL,
  `adverb_2_id` INTEGER NOT NULL,
  FOREIGN KEY (`adverb_1_id`) REFERENCES `adverbs`(`adverb_id`),
  FOREIGN KEY (`adverb_2_id`) REFERENCES `adverbs`(`adverb_id`)
);
