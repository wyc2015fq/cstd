# SQLServer2PostgreSQL迁移过程中的几个问题 - =朝晖= - 博客园
# [SQLServer2PostgreSQL迁移过程中的几个问题](https://www.cnblogs.com/dhcn/p/7105559.html)
1、PostgreSQL 跨平台迁移工具[Migration Toolkit](http://www.enterprisedb.com/docs/en/8.4/mtkguide/Postgres_Plus_Standard_Server_Migration_Toolkit_Guide-08.htm#P101_9961)的使用指南：[http://www.enterprisedb.com/docs/en/8.4/mtkguide/Table%20of%20Contents.htm#TopOfPage](http://www.enterprisedb.com/docs/en/8.4/mtkguide/Table%20of%20Contents.htm#TopOfPage)
2、可能是程序的问题，迁移工具会把jtds驱动连接字符串认为是[Oracle](http://lib.csdn.net/base/oracle)去连接，Google以后：[http://my.oschina.net/congqian/blog/106518](http://my.oschina.net/congqian/blog/106518)，找到这个指令指定数据源类型的方式来做操作：
[Java](http://lib.csdn.net/base/java) -Dprop=toolkit.properties -jar lib/edb-migrationtoolkit.jar -sourcedbtype sqlserver -targetdbtype postgresql -allTables -targetSchema public dbo
3、用上面的工具迁移的过程中，我所发现的能够阻碍整个表迁移的问题是类型匹配问题，MSSQL的bit类型在Postgresql建表的过程中被建为boolean字段，但是数据加载的过程中却报错：完全无法把bit数据转化为boolean数据.我刚开始准备写[Python](http://lib.csdn.net/base/python)脚本迁移这几个表，后来发现问题比较单一，直接把源表里的bit类型转化为smartint类型，然后迁移过来，准备再Postgresql里面再回去。（后加）比较诡异的是其中一个表的bit类型转移过去了，但是[数据库](http://lib.csdn.net/base/mysql)的bit和Java代码中Byte类型无法兼容，还是得把字段类型改成smallini
4、接着上面问题说，3所做的数据迁移完成以后，修改了Web应用数据源配置文件，启动Web应用，做了几个前面改了类型的表的读写[测试](http://lib.csdn.net/base/softwaretest)，发现正常可用，原来之前SQLServer中bit类型自动生成的[hibernate](http://lib.csdn.net/base/javaee) ORM配置文件中bit对应的类型是java.lang.Byte,Byte是8位字节，smallint恰好也是8位，正好可以兼容可用。算是一个偶然运气事件吧。
5、再说一个问题，之前工具数据迁移的时候，系统经常提醒说UTF-8编码问题，我以为迁移过程中所有字符串数据自动会转换成UTF-8的数据，而Web应用之前是GBK编码，所以猜想是不是迁移完还得做编码集全体转换成UTF-8，但实际发现并不是这样，连之前用[python](http://lib.csdn.net/base/python)在SQLServer里面查询出来带U字头的字符串，在Postgresql里面查出来好像已经不带U字头了。
6、RHEL6.0的PostgresSQL二进制版本的安装目录是/var/lib/pgsql ,与其它平台不同的是，数据文件和配置文件都在这个目录内的子目录内。
7、数据迁移后的处理,PostgreSQL中主键ID生成机制和SQLServer中的实现机制略有不同，从SQLServer的Identity到PostgreSQL中的Sequence,工具迁移的过程中，只复制数据，主键需要自己手动(或脚本)来设置，主键字段设置好以后，还要对每个sequence的当前值根据当前表的ID最大值做一次设置。我的Web应用项目是基于Hibernate的，因为Hibernate的配置文件也要修改:1、是修改主配置文件中的SQL方言类型；2、就是把每个表的主键生成机制由identity改为sequence(现在的版本这个设置一般是由@注解机制完成)
SQL Server是这样:
```
<id name="id" column="id" type="java.lang.Integer">  
     <generator class="identity"/>  
</id>
```
改为PostgreSQL以后是这样:
```
<id name="id" column="id" type="java.lang.Integer">  
    <generator class="sequence">     
        <param name="sequence">此处是表ID对应的序列名称，你可以在AdminIII的序列目录中找到表对应序列名，一般序列的名字是Tablename_id_seq</param>     
    </generator>
```

