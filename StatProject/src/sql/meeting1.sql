CREATE TABLE IF NOT EXISTS `pantherhackers`.`meeting1` (
	  `uid` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	  `name` VARCHAR(128) NULL,
	  `year` VARCHAR(128) NULL,
	  `major` VARCHAR(128) NULL,
	  PRIMARY KEY (`uid`),
	  UNIQUE INDEX `idusers_UNIQUE` (`uid` ASC));