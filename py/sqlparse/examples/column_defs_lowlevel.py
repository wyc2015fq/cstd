#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (C) 2009-2018 the sqlparse authors and contributors
# <see AUTHORS file>
#
# This example is part of python-sqlparse and is released under
# the BSD License: https://opensource.org/licenses/BSD-3-Clause
#
# Example for retrieving column definitions from a CREATE statement
# using low-level functions.

import sqlparse


def extract_definitions1(token_list):
    # assumes that token_list is a parenthesis
    definitions = []
    tmp = []
    # grab the first token, ignoring whitespace. idx=1 to skip open (
    tidx, token = token_list.token_next(1)
    while token and not token.match(sqlparse.tokens.Punctuation, ')'):
        tmp.append(token)
        # grab the next token, this times including whitespace
        tidx, token = token_list.token_next(tidx, skip_ws=False)
        # split on ",", except when on end of statement
        if token and token.match(sqlparse.tokens.Punctuation, ','):
            definitions.append(tmp)
            tmp = []
            tidx, token = token_list.token_next(tidx)
    if tmp and isinstance(tmp[0], sqlparse.sql.Identifier):
        definitions.append(tmp)
    return definitions



def extract_definitions(token_list):
    # assumes that token_list is a parenthesis
    definitions = []
    tmp = []
    # grab the first token, ignoring whitespace. idx=1 to skip open (
    tidx, token = token_list.token_next(1)
    while token and not token.match(sqlparse.tokens.Punctuation, ')'):
        tmp.append(token)
        # grab the next token, this times including whitespace
        tidx, token = token_list.token_next(tidx, skip_ws=False)
        # split on ",", except when on end of statement
        if token and token.match(sqlparse.tokens.Punctuation, ','):
            definitions.append(tmp)
            tmp = []
            tidx, token = token_list.token_next(tidx)
    if tmp:
        definitions.append(tmp)
    return definitions



def aaaa(column):
    #tt = '##'.join(str(t) for t in column[1:])
    #print('NAME: %s DEFINITION: %s' % (column[0], tt))
    li = []
    li2 = []
    print(type(column))
    for t in column:
        tt = str(t).strip()
        if len(tt)>0:
            if tt.find(',')>1:
                t1 = tt.split(',')
                if len(t1)==2:
                    a = t1[0].strip()
                    b = t1[1].strip()
                    #print(a)
                    li2.append(a)
                    #print('===========')
                    li.append(li2)
                    li2 = []
                    #print(b)
                    li2.append(b)
            else:
                #print(tt)
                li2.append(tt)
    li.append(li2)
    return li


dataTypeSet=set(['bigint','int','mediumint','smallint','tinyint','decimal', #数值类型
  'char','varchar','varbinary','text','longtext','blob',     #字符串类型
             'date','datetime','timestamp','time','year'                #时间类型
            ])

#print(dataTypeSet)

# 返回 columnList
def bbbb(li):
    ld = []
    column_position = 0
    PRIMARY_KEY = ''
    for line in li:
        d = {}
        if line[0][0]=="`":
            d['column_position'] = column_position
            column_position += 1
            d["column_name"] = line[0]
            c = line[1]
            t = c.split('(')
            i = 1
            if t[0].lower() in dataTypeSet:
                d["column_type"] = c
                d["data_type"] = t[0]
                if len(t)>1:
                    t[1] = t[1].strip(')')
                    a = t[1].split(',')
                    d["length"] = int(a[0])
                    if len(a)>1:
                        d['dot_len'] = int(a[1])
                i += 1
            while(i<len(line) and line[i].upper() in set(['UNSIGNED', 'ZEROFILL'])):
                d["column_type"] += ' ' + line[i]
                i += 1
            while (i<len(line)):
                c = line[i].upper()
                i += 1
                if c in set(['NOT NULL', 'NULL']):
                    d['is_nullable'] = c
                if c=='AUTO_INCREMENT':
                    d['is_auto_inc'] = 1
                if c=='PRIMARY':
                    PRIMARY_KEY = d["column_name"]
                    d['column_key'] = 'PRIMARY'
                if c=='COMMENT':
                    d['column_comment'] = line[i]
                    i += 1
                if c=='DEFAULT':
                    d['column_default'] = line[i]
                    i += 1
                
        else:
            i = 0
            while (i<len(line)):
                c = line[i].upper()
                i += 1
                if c=='PRIMARY':
                    if line[i].upper()=='KEY':
                        PRIMARY_KEY = line[i+1].strip('()')
                        i += 2
        if 'column_name' in d:
            ld.append(d)
            print(d)

    for d in ld:
        if 'column_name' in d  and d['column_name']==PRIMARY_KEY:
            d['column_key'] = 'PRIMARY'
    
    return ld


def is_create_table(parsed, tidx, token, table1):
    if token.match(sqlparse.tokens.Keyword.DDL, 'CREATE'):
        tidx, token = parsed.token_next(tidx)
        if token.match(sqlparse.tokens.Keyword, 'TABLE'):
            tidx, token = parsed.token_next(tidx)
            table1['table_name'] = str(token)
            return True
    return False

def my_token_match(parsed, tidx, token, name, table1):
    if token.match(sqlparse.tokens.Keyword, name):
        tidx, token = parsed.token_next(tidx)
        if token.match(sqlparse.tokens.Operator.Comparison, '='):
            tidx, token = parsed.token_next(tidx)
            table1[name] = str(token)
    return tidx, token


def ddlparser_create_table(SQL):
    parsed = sqlparse.parse(SQL)[0]

    table = {}
    # extract the parenthesis which holds column definitions
    tidx, token = parsed.token_next(1)

    if is_create_table(parsed, tidx, token, table):
        tidx, par = parsed.token_next_by(i=sqlparse.sql.Parenthesis)
        columns = extract_definitions(par)
        tidx, token = parsed.token_next(tidx)

        while token:
            tidx, token = my_token_match(parsed, tidx, token, 'ENGINE', table)
            tidx, token = my_token_match(parsed, tidx, token, 'CHARSET', table)
            tidx, token = my_token_match(parsed, tidx, token, 'COMMENT', table)
            tidx, token = parsed.token_next(tidx)

        if 1:
            for column in columns:
                aa = aaaa(column)
                return bbbb(aa), table
    
    return None
                

if __name__ == '__main__':
    SQL = """CREATE TABLE foo (
             id integer primary key,
             title varchar(200) not null,
             description text);"""

    SQL = """
    CREATE TABLE `sys_user` (
  `id` bigint(20)
   unsigned NOT NULL
    AUTO_INCREMENT COMMENT 'ID' PRIMARY,
  `gmt_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '建立时间',
  `gmt_modified` timestamp NOT NULL COMMENT '修改时间',
  `user_name` varchar(30) DEFAULT NULL COMMENT '用户登陆名',
  `name` varchar(32) DEFAULT NULL COMMENT '用户中文名',
  `passwd` varchar(64) DEFAULT NULL COMMENT '密码',
  `email` varchar(127) DEFAULT NULL COMMENT '邮箱',
  `emp_level` tinyint(3) unsigned DEFAULT '0' COMMENT 
  '职级 （0员工 1架构师 2部门领导）',
  `dept_id` int(10) unsigned DEFAULT '0' COMMENT
   '部门ID',
  `leader` varchar(30) DEFAULT NULL COMMENT '领导',
  `status` tinyint(3) unsigned DEFAULT NULL COMMENT '状态(1正常 0禁用)',
  `role_name` varchar(16) DEFAULT NULL COMMENT '职位（employee/ leader/dba)',
  `group_id` int(11) DEFAULT '0' COMMENT '组ID（dba组ID 1312）',
  `mobile` varchar(16) DEFAULT NULL COMMENT '手机号码',
  `post_name` varchar(32) DEFAULT NULL COMMENT '岗位',
  `last_login_time` timestamp NULL DEFAULT NULL COMMENT '最后登陆时间',
  `dingding` varchar(127) DEFAULT NULL COMMENT '钉钉',
  `user_type` tinyint(4) DEFAULT NULL COMMENT '用户类别(一般用户/审批用户)',
  `is_active` tinyint(4) DEFAULT NULL COMMENT '是否激活',
  `is_superuser` tinyint(4) DEFAULT NULL COMMENT '是否管理员',
  `auth_group` varchar(127) DEFAULT NULL COMMENT '权限组(角色id以逗号分隔)',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_sys_user_name` (`user_name`),
  KEY `FK_SYS_USER_DEPT` (`dept_id`),
  CONSTRAINT `FK_SYS_USER_DEPT` FOREIGN KEY (`dept_id`) REFERENCES `sys_department` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1105 DEFAULT CHARSET=utf8 COMMENT='用户表';
    """

    
    SQL = """
    CREATE TABLE `sys_user` (
  `id` bigint(20)
   unsigned NOT NULL
    AUTO_INCREMENT COMMENT 'ID' PRIMARY,
  `gmt_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '建立时间',
  `gmt_modified` timestamp NOT NULL COMMENT '修改时间',
  `user_name` varchar(30) DEFAULT NULL COMMENT '用户登陆名',
  `auth_group` varchar(127) DEFAULT NULL COMMENT '权限组(角色id以逗号分隔)',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_sys_user_name` (`user_name`),
  KEY `FK_SYS_USER_DEPT` (`dept_id`),
  CONSTRAINT `FK_SYS_USER_DEPT` FOREIGN KEY (`dept_id`) REFERENCES `sys_department` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1105 DEFAULT CHARSET=utf8 COMMENT='用户表';
    """

    t,table = ddlparser_create_table(SQL)
    print(t)
    print(table)

