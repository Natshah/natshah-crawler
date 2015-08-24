
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

--
-- Create schema natshahdb
--

CREATE DATABASE IF NOT EXISTS natshahdb;
USE natshahdb;

--
-- Definition of table `emm_domains`
--

DROP TABLE IF EXISTS `emm_domains`;
CREATE TABLE `emm_domains` (
  `id` int(15) NOT NULL AUTO_INCREMENT,
  `domain` varchar(900) NOT NULL,
  `crawled` tinyint(1) NOT NULL,
  `done` tinyint(1) NOT NULL,
  `archived` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index_domain` (`domain`(333)),
  KEY `index_crawled` (`crawled`) USING BTREE,
  KEY `index_done` (`done`) USING BTREE,
  KEY `index_archived` (`archived`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;


--
-- Definition of table `emm_emails`
--

DROP TABLE IF EXISTS `emm_emails`;
CREATE TABLE `emm_emails` (
  `id` int(50) NOT NULL AUTO_INCREMENT,
  `email` varchar(900) NOT NULL,
  `indexed` tinyint(1) unsigned NOT NULL,
  `done` tinyint(1) unsigned NOT NULL,
  `archived` tinyint(1) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index_email` (`email`(333)),
  KEY `index_indexed` (`indexed`) USING BTREE,
  KEY `index_done` (`done`) USING BTREE,
  KEY `index_archived` (`archived`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=2777 DEFAULT CHARSET=utf8;

--
-- Definition of table `emm_urls`
--

DROP TABLE IF EXISTS `emm_urls`;
CREATE TABLE `emm_urls` (
  `id` int(50) NOT NULL AUTO_INCREMENT,
  `url` varchar(900) NOT NULL,
  `data` text NOT NULL,
  `crawled` tinyint(1) NOT NULL,
  `done` tinyint(1) NOT NULL,
  `archived` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  KEY `index_crawled` (`crawled`,`id`) USING BTREE,
  KEY `index_done` (`done`,`id`) USING BTREE,
  KEY `index_archived` (`archived`,`id`) USING BTREE,
  KEY `index_url` (`url`(333)) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=241801 DEFAULT CHARSET=utf8;
