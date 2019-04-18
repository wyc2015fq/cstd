# oracle的用户授权问题：如何把一个用户的所有表的读写权限授予另一个用户 - z69183787的专栏 - CSDN博客
2014年01月15日 10:37:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21924
select 'Grant all on '||table_name||'to user2 ;' from all_tables 
where owner = upper(user1); 
然后执行该语句的结果就可以了 
