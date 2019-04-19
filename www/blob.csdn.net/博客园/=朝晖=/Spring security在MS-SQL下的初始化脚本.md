# Spring security在MS-SQL下的初始化脚本 - =朝晖= - 博客园
# [Spring security在MS-SQL下的初始化脚本](https://www.cnblogs.com/dhcn/p/7100137.html)
```sql;gutter
-- create table users(  
 --     username nvarchar(50) not null primary key,  
 --     password nvarchar(50) not null,  
 --     enabled bit not null);  
 --create table authorities (  
--      username nvarchar(50) not null,  
--      authority nvarchar(50) not null,  
--      constraint fk_authorities_users foreign key(username) references users(username));  
--      create unique index ix_auth_username on authorities (username,authority);  
create table groups (  
  [id] [bigint] IDENTITY (0, 1) NOT NULL primary key,   
  group_name nvarchar(50) not null);  
create table group_authorities (  
  group_id bigint not null,   
  authority nvarchar(50) not null,   
  constraint fk_group_authorities_group foreign key(group_id) references groups(id));  
create table group_members (  
   [id] [bigint] IDENTITY (0, 1) NOT NULL primary key,   
  username nvarchar(50) not null,   
  group_id bigint not null,   
  constraint fk_group_members_group foreign key(group_id) references groups(id));
```
