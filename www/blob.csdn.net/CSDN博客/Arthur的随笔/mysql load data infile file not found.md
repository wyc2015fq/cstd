# mysql load data infile file not found - Arthur的随笔 - CSDN博客
2010年12月13日 13:39:00[largetalk](https://me.csdn.net/largetalk)阅读数：2859
解决方法：
gelmjw@voyager:~$ sudo aa-complain /usr/sbin/mysqld
[sudo] password for gelmjw:
Setting /usr/sbin/mysqld to complain mode.
gelmjw@voyager:~$ sudo /etc/init.d/apparmor reload
Reloading AppArmor profiles : done.
gelmjw@voyager:~$ 
another mysql load date error fix:
when i load data form file as:
`load data infile '/tmp/typee.sql' into table asset_assettype fields terminated by '|' lines terminated by '\n';`it will be report a error: 
ERROR 1045 (28000): Access denied for user 'adaptive'@'%' (using password: YES)
to fix it, should use statement as:
`load data local infile '/tmp/typee.sql' into table asset_assettype fields terminated by '|' lines terminated by '\n';`
When you try to export data to a file you are getting an access denied error message. Even if you grant the user with full privileges on the database you must Grant Global Permission to the specified user and set File Privileges to Yes as well in order to export
 data to a file.
mysql> select * INTO OUTFILE ‘/tmp/file.out’ from TABLE;
“ERROR 1045 (28000): Access denied for user ‘db_user’@'localhost’ (using password: YES)”
**How To:**
mysql -u root -p
> 
1. use mysql;
2. update user set File_priv = ‘Y’ where User = ‘db_user’;
3. FLUSH PRIVILEGES;
