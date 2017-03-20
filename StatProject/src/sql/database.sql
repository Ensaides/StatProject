CREATE SCHEMA IF NOT EXISTS `pantherhackers` DEFAULT CHARACTER SET utf8;

CREATE TABLE IF NOT EXISTS `pantherhackers`.`users` (
	  `uid` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	  `name` VARCHAR(128) NULL,
	  PRIMARY KEY (`uid`),
	  UNIQUE INDEX `idusers_UNIQUE` (`uid` ASC));

CREATE TABLE IF NOT EXISTS `pantherhackers`.`typeforms` (
	`TypeformID` VARCHAR(16) NOT NULL,
	`APIKey` VARCHAR(64) NOT NULL,
	`Name` VARCHAR(64) NOT NULL,
	PRIMARY KEY (`TypeformID`),
	UNIQUE INDEX `TypeformID_UNIQUE` (`TypeformID` ASC));