# MySQL 中通过使用UNIQUE KEY 来控制字段值不重复的问题. - z69183787的专栏 - CSDN博客
2015年05月31日 11:31:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6274
语法：
UNIQUE KEY `test` (`xref_key`,`xref_dbname`)
UNIQUE KEY `xref_key` (`xref_key`,`xref_dbname`)
       上述语法检查xref_key与xref_dbname的组合是否是唯一值，可以设定多个字段的组合。其中，test是索引表中的字段名称。
        该方法适合用来解决多个字段相互依赖，且必须满足唯一性的条件的情况。在经常新增或删除资料表中auto_increment的primary key会造成大量的断层，这类易变动的资料表使用auto_increment并不是一个很适合的索引值，所以可以采取unique key来处理。
---------------------------------------------------
CREATE TABLE `TB_REGION` (
    `ID` bigint(20) NOT NULL auto_increment,
    `VERSION` decimal(10,0) default NULL,
    `REGION_NAME` varchar(80) NOT NULL default '',
    `REGION_DESC` varchar(255) NOT NULL default '',
    `ACTIVE_IND` tinyint(1) default NULL,
    `REMARKS` varchar(255) default NULL,
    `CREATE_BY` bigint(20) default NULL,
    `CREATE_TS` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
    `UPDATE_BY` bigint(20) default NULL,
    `UPDATE_TS` timestamp NOT NULL default '0000-00-00 00:00:00',
    `SHOWINDEX` int(11) NOT NULL default '9999',
    PRIMARY KEY    (`ID`),
**UNIQUE KEY `INDEX_REGION_NAME` (`REGION_NAME`)**
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
我使用了hibernate+spring
如果值重复,就会抛出如下异常
org.springframework.dao.DataIntegrityViolationException:
org.hibernate.exception.ConstraintViolationException:
同时,为了效验不重复的字段,我们可以遵循hibernate同时持有id字段和version字段的最佳实践,
这样会更加轻松的解决问题.
---------------------------------------------------
CREATE TABLE `aaa` (
`id` int(11) NOT NULL auto_increment,
`name` varchar(32) default NULL,
PRIMARY KEY  (`id`),
UNIQUE KEY `name` (`name`)
);
里面的ID是自增的主键，name则是不能重复的。
