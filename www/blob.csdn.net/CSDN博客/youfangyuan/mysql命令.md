# mysql命令 - youfangyuan - CSDN博客
2012年05月08日 16:33:59[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1343标签：[mysql																[html5																[null																[数据库																[server																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=html5&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[mysql](https://blog.csdn.net/youfangyuan/article/category/1142217)
                
1.下载并安装
    mysql 安装文件:mysql-5.5.19-win32.msi
    mysql 源代码:mysql-5.5.19.zip
    mysql 客户端安装文件:mysql-workbench-gpl-5.2.36-win32.msi
2.启动 mysql 服务 
    配置文件：
        复制my-small.ini，my-huge.ini其中一个 为 my.ini  文件,端口可以修改
    cd C:\Program Files\MySQL\MySQL Server 5.5\bin
    C:\Program Files\MySQL\MySQL Server 5.5\bin>mysqld.exe
    监听端口:3306
    mysqld --verbose --help
    mysqld --console
    mysqld --debug
    参考: http://blog.csdn.net/yhawaii/article/details/7069968
3. 登陆
    cd C:\Program Files\MySQL\MySQL Server 5.5\bin
    初始密码为空，所以直接回车可以进入
    mysql.exe -u root -p
     mysql.exe -u root -p -h
4.管理
    修改默认密码
    mysqladmin -u root password 'yourpassword'
    cd C:\Program Files\MySQL\MySQL Server 5.5\bin
    mysqladmin.exe version
    mysqladmin.exe status
    关闭
    mysqladmin.exe -u root shutdown
5.创建表/测试    
        create table test (
        id int(5) not null,
        name varchar(40) not null,
        email varchar(50) not null,
        telephone int(12) null);
    查看当前数据库: show databases;
    选择数据库: use mysql;
    创建数据库：create database mydata;
    创建表：
        create table address (
        id int(5) not null,
        name varchar(40) not null,
        email varchar(50) not null,
        telephone int(12) null);
    插入数据：
        insert into address values (1,'Nighthawk','nighthawk@163.net',92384092);
    显示表数据：
        show full fields from address;
        describe address;
        select * from address;
.用户管理
    http://hi.baidu.com/fwso/blog/item/658c00555bdd1cc5b645aee0.html
    select host,user,password from user;        
    创建用户：
        create user 'test'@'%' identified by 'test123';
    用户授权:
        grant all on *.* to 'test'@'%';
        grant select,insert,update,delete on [db_name].* to [username]@[ipadd] identified by '[password]';
        show grants for test;
6.windows 编译mysql
    下载Cmake cmake-2.8.6-win32-x86.exe
    http://cmake.org/cmake/resources/software.html 
    http://dev.mysql.com/doc/refman/5.5/en/windows-source-build.html
    帮助文档
    http://hi.baidu.com/phenixnote/blog/item/4dfd5aecf703f85378f055ef.html
    E:/Study/mysql/mysql-5.5.19
    .问题1 sql_locale.cc 文件格式问题
        sql_locale.cc 在编译之前保存为 dos u8 格式
    .问题2 js脚本默认打开方式
        编译时js脚本问题需要修改js文件默认打开方式，参考 fullreg.reg       
        HKEY_CLASSES_ROOT 为 注册表第一个根节点
        [HKEY_CLASSES_ROOT\.js]
        @="JSFile"
    执行后产生MySQL.sln等sln文件 ，既有工程可以使用
7.perl 链接mysql数据库
    安装 dbi dbi-mysql,进入Perl Package Manager(命令行ppm)
    http://search.cpan.org/dist/DBD-mysql/lib/Bundle/DBD/mysql.pm 下载 DBD-mysql-4.020.tar.gz
    安装 DBD-mysql-4.020.tar.gz
    perl Makefile.PL --with-mysql="C:\Program Files\MySQL\MySQL Server 5.5"
    windows 命令直接安装：
    ppm  install DBI
    ppm install DBD::mysql
8. 数据库导出/导入
    mysqldump -uroot -p mydata > mydata.sql 
    mysql -uroot -p db_html5 < db_html5.sql
    mysql -uroot db_html5 < db_html5.sql
    mysql -uflash -pflash2011 db_html5 < db_html5.sql
    mysql -h 10.135.11.113 -P3300 -umtt -pmtt@sn 
    mysqldump -h 10.135.11.113 -P3300 -umtt -pmtt@sn  db_appCenter > db_appcenter.sql
    mysqldump -h -opt 10.135.11.113 -P3300 -umtt -pmtt@sn  db_appCenter > db_appcenter.sql
    mysqldump --opt --single-transaction -default-charset=utf8 -h 10.135.11.113 -P3300 -umtt -pmtt@sn  db_appCenter > db_appcenter.sql
    mysqldump --single-transaction -default-charset=utf8 -h 10.135.11.113 -P3300 -umtt -pmtt@sn  db_appCenter > db_appcenter.sql
    mysqldump -default-charset=utf8 -h 10.135.11.113 -P3300 -umtt -pmtt@sn  db_appCenter > db_appcenter.sql
    // -d 仅导出表结构
    mysqldump --single-transaction -h 10.135.11.113 -P3300 -umtt -pmtt@sn  -d db_test_appCenter > db_appcenter.sql
    // -t 仅导出表数据 -c 表示带字段名
    mysqldump --single-transaction  --extended-insert=false -c -h 10.135.11.113 -P3300 -umtt -pmtt@sn  -t db_test_appCenter > db_appcenter.sql
    //全倒出
    mysqldump --single-transaction  --extended-insert=false -c -h 10.135.11.113 -P3300 -umtt -pmtt@sn  db_test_appCenter > db_appcenter.sql
9. sql语句
    查询前 3 行记录
    select * from t_type_info where 1=1 limit 3;
    select * from t_app where 1=1 limit 3;
    查询表的索引
    show index from mytable;   
    SHOW CREATE TABLE table_name
    修改表，添加union
    alter table test add unique u_a(a);
10. 全文搜索
