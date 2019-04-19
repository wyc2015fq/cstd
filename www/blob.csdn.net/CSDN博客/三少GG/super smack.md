# super smack - 三少GG - CSDN博客
2014年04月12日 18:31:04[三少GG](https://me.csdn.net/scut1135)阅读数：1010
[http://c20031776.blog.163.com/blog/static/68471625201341411832198/](http://c20031776.blog.163.com/blog/static/68471625201341411832198/)
与sysbench比较
优点：可以针对某个数据库指定某张表，自定义SQL语句进行测试
缺点：功能简单
sysbench：
优点：功能稍微强大，更偏向于整个系统的测试，包括CUP、内存、数据库整体性能等的测试
缺点：不能灵活配置，只支持数据库级别（databases）不支持表级别（table）的测试
总结：两个配合起来用比较好，但是嫌看说明麻烦，就自己写代码测吧！
[http://www.livingelsewhere.net/tag/super-smack/](http://www.livingelsewhere.net/tag/super-smack/)
### [Super Smack](http://www.livingelsewhere.net/2011/05/29/super-smack/)
[Super Smack](http://vegan.net/tony/supersmack/) 是一个强大的压力测试工具，支持 MySQL, PostgreSQL, Oracle。最开始的版本是由[Sasha
 Pachev](mailto:sasha@surveypro.com)写成，由[Jeremy Zawodny](http://jeremy.zawodny.com/)在维护，而现在，是[Tony Bourke](http://vegan.net/tony/supersmack/)在维护，根据 Tony Bourke 的 开发 log 来看，2005-08-30 后，super smack 就已经停止发布新的版本，但这并不妨碍我们现在继续使用它(这个工具的开发者和维护者很伟大)。
### 安装
安装有点点麻烦，主要是编译时会出现一些问题。
```
```bash
[root@localhost
 tmp]
```
```bash
#
 wget [http://vegan.net/tony/supersmack/super-smack-1.3.tar.gz](http://vegan.net/tony/supersmack/super-smack-1.3.tar.gz)
```
```bash
[root@localhost
 tmp]
```
```bash
#
 tar xvzf super-smack-1.3.tar.gz
```
```bash
[root@localhost
 super-smack-1.3]
```
```bash
#
 ./configure  --prefix=/usr/local/super-smack --with-mysql --with-mysql-lib=/usr/local/mysql/lib/mysql --with-mysql-include=/usr/local/mysql/include/mysql
```
```bash
[root@localhost
 super-smack-1.3]
```
```bash
#
 make
```
```bash
[root@localhost
 super-smack-1.3]
```
```bash
#
 make install
```
```
##### 说明
1 编译时，必须指定它所支持的数据库管理系统，否则会报如下错误。
......
configure: error:
You should include support for at least one database!
Reconfigure with one or more of:
  --with-mysql
  --with-pgsql
  --with-oracle
2 选择支持 MySQL 后, 在 MySQL 编译安装的情况下时, 也需要在编译参数中指定 MySQL 的 lib。
3 编译时，dictionary.h 和 super-smack.cc 报错：
......
dictionary.h:93: error: ‘strlen’ was not declared in this scope
super-smack.cc:126: error: ‘strlen’ was not declared in this scope
在 super smack 源代码的 `src` 目录，找到 dictionary.h,
 super-smack.cc，分别加上`#include <string.h>`。
4 编译时，query.cc 报错
query.cc:200: error: cast from ‘char*’ to ‘unsigned int’ loses precision
query.cc:200: error: cast from ‘char*’ to ‘unsigned int’ loses precision
query.cc:219: error: cast from ‘char*’ to ‘unsigned int’ loses precision
query.cc:219: error: cast from ‘char*’ to ‘unsigned int’ loses precision
在 super smack 源代码的 `src` 目录， 找到 query.cc文件，将上面指定的
 200, 219 行中的`unsigned int`改为
```
unsigned
 long
```
。
### 用其测试 MySQL 之前奏
1 看 Makefile 才知道， smack 文件是放在 `/usr/share/smacks` 这个目录下(开始并不知道源代码中有一份),
 产生的数据文件是在`/var/smack-data`目录下
```
```bash
...
```
```bash
SMACKS_DIR
 =
```
```bash
/usr/share/smacks
```
```bash
DATADIR
 =
```
```bash
/var/smack-data
```
```bash
...
```
```
2 根据 MySQL 的参数对应修改 /usr/share/smacks 目录下的 select-key.smack 和 update-select.smack 文件。包括 user, host, db, pass, 还有，最重要的 socket 路径。
3 未将 super smack 的 bin 目录加入 PATH 时，则还需：
```
```bash
gen_data_file
```
```bash
"gen-data
 -n 90000 -f %12-12s%n,%25-25s,%n,%d"
```
```bash
;
```
```bash
#改为
```
```bash
gen_data_file
```
```bash
"./gen-data
 -n 90000 -f %12-12s%n,%25-25s,%n,%d"
```
```bash
;
```
```bash
#或者直接
```
```bash
gen_data_file
```
```bash
"/usr/local/super-smack/bin/gen-data
 -n 90000 -f %12-12s%n,%25-25s,%n,%d"
```
```bash
;
```
```
4 因为在 select-key.smack 和 update-select.smack 文件中指定的数据文件是 words.dat, 而安装默认情况下 words.dat 为空文件:
```
```bash
[root@localhost
 smack-data]
```
```bash
#
 ls -al
```
```bash
total
 5320
```
```bash
drwxr-xr-x 
 2 root root    4096 May 26 18:12 .
```
```bash
drwxr-xr-x
 24 root root    4096 May 26 17:25 ..
```
```bash
-rw-r--r-- 
 1 root root 5421337 May 26 17:25 ..  http_auth.dat
```
```bash
-rw-r--r-- 
 1 root root       0 May 26 17:25 ..  words.dat
```
```
所以，需将 select-key.smack 和 update-select.smack 文件中指定的数据 words.dat 改为 http_auth.dat(其实为了统一，最好也是 http_auth.dat，因为文件中指定测试的表名是 http_atuh)。
当然，为了不修改，也可以在开始先执行如下命令：
```
```bash
[root@localhost
 bin]
```
```bash
#
 /usr/local/super-smack/bin/gen-data -n 90000 -f %12-12s%n,%25-25s,%n,%d > /var/smack-data/words.dat
```
```
5 将 /usr/share/smacks 下文件 copy 到 /usr/local/super-smack/bin/ 目录下：
```
```bash
[root@localhost
 super-smack]
```
```bash
#
 cp /usr/share/smacks/*  bin/
```
```
### 运行
```
```bash
[root@localhost
 bin]
```
```bash
#
 ./super-smack -d mysql select-key.smack 20 1000
```
```bash
Query
 Barrel Report
```
```bash
for
```
```bash
client
 smacker1
```
```bash
connect:
 max=2204ms  min=1ms avg= 221ms from 20 clients
```
```bash
Query_type     
 num_queries     max_time        min_time        q_per_s
```
```bash
select_index   
 40000   0       0       5017.26
```
```
参数：
- `-d` 指定测试的数据库管理系统的类型。
- `20` 20 个线程
- `1000` 每个线程 1000 个查询
实际上，还有`-D`参数来指定数据文件，默认路径如前面提到是` /var/smack-data`，
 这个路径需跟 select-key.smack 和 update-select.smack 指定的一致。
返回结果：
- **max=2204ms min=1ms avg= 221ms from 20 clients** 连接的最大、最小及平均花费时间。
- **q_per_s|5017.26** QPS，每秒请求处理数
- **40000**, 脚本中，对查询次数做了翻倍处理, 所以， 20 × 1000 x 2 = 40,000.
原本打算用mysql自带的mysqlslap做压力测试，可惜这工具不给力，可控制的地方不多，尤其不能够产生随机的测试语句。遂改用super smack。貌似它风评还不错。
**定义自己的测试配置文件**
要做自定义的MySQL测试，当然就必须编辑自己的配置文件（.smack）。Super Smack之所以很灵活，就在于可以在它的配置文件中自定义很多的东西。当然，随着而来的就是配置文件的相对复杂。理解它的配置文件的结构，是学会用Super Smack的关键所在。
下文将以select-key.smack为例，讲解.smack配置文件的结构。
网上也有一些相关的link：[http://imysql.cn/docs/High_Performance_MySQL/0596003064/hpmysql-CHP-3-SECT-3.html](http://imysql.cn/docs/High_Performance_MySQL/0596003064/hpmysql-CHP-3-SECT-3.html)
client "admin"
{
 user "root";
 host "localhost";
 db "test";
 pass "";
 socket "/data0/mysql/mysql.sock"; // this only applies to MySQL and is
// ignored for PostgreSQL
}
这部分定义了Super Smack连接数据库需要用到的一些信息，比如用户名，机器名，数据库名，密码等等。很容易理解，不解释。
// ensure the table exists and meets the conditions
table "http_auth"
{
  client "admin"; // connect with this client
 // if the table is not found or does not pass the checks, create it
 // with the following, dropping the old one if needed
  create "create table http_auth
    (username char(25) not null primary key,
     pass char(25),
     uid integer not null,
     gid integer not null
    )";
  min_rows "90000"; // the table must have at least that many rows
  data_file "words.dat"; // if the table is empty, load the data from
//this file
  gen_data_file "gen-data -n 90000 -f %12-12s%n,%25-25s,%n,%d";
// if the file above does not exist, generate it with the above shell command
// you can replace this command with anything that prints comma-delimited
// data to stdout, just make sure you have the right number of columns
}
这里定义了要进行测试的数据库表的信息。包括：
创建这个表所使用的sql语句。如果Super Smack发现数据库中没有这个表的话，会使用这条sql语句创建表。
注意，这里设置了client "admin"，表明是用前面定义的admin这个client来创建表。
min_rows这个参数是一个对表的约束，要求表内的记录必须达到的数量。例子中的值是90000，表示表中必须要有90000条数据。
“ data_file "words.dat";  ”是定义了一个数据文件，这个文件中的每一行都可以作为表中的一条记录。如果表http_auth没有达到90000条数据，那么Super Smack会从words.dat中读取数据然后插入到表http_auth中。
gen_data_file "gen-data -n 90000 -f %12-12s%n,%25-25s,%n,%d"; -- 自动生成测试数据，如果words.dat为空的话，Super Smack使用这条语句生成测试数据然后写入到words.dat中。
gen-data是安装Super Smack后提供的一个产生随机数据的小程序。下面解读一下它的命令格式：
-n 90000表示生成90000条记录；
-f后面跟的是记录的格式；
%S表示产生字符串，%12-12s表示产生的字符串长度在12与12之间；
%D表示生成数字，%d生成随机数；
%N %n表示可以从1开始递增的数值，可用于表中的主键，比如第一条记录的%n就会被1代替，以此类推。
上面的这句“gen-data -n 90000 -f %12-12s%n,%25-25s,%n,%d” 输出的每行是大概这样的：
josamibdnjdb3,eyhkbsombltouujdrbwcrrcgb,3,485560280
qwdqweqwdwev4,regergergftyyujeqwqwecdfr,4,239013239
rhbtjrbtywlf5,edeffkergpqqmcxvkrtmbhwer,5,233021393
默认gen-data的输出都是打印了标准输出上，可以用重定向让gen-data的输出结果写入到文件中。
需要指出的是，这里采用的例子select-key.smack是对一个表做查询测试，所以要求table中必须首先有一定量的数据。如果想做插入的测试，完全可以将min_rows设置成0，这样，Super Smack也就不需要调用gen_data_file产生数据。
//define a dictionary
dictionary "word"
{
  type "rand"; // words are retrieved in random order
  source_type "file"; // words come from a file
  source "words.dat"; // file location
  delim ","; // take the part of the line before ,
  file_size_equiv "45000"; // if the file is greater than this
//divive the real file size by this value obtaining N and take every Nth
//line skipping others. This is needed to be able to target a wide key
// range without using up too much memory with test keys
}  
//define a query
query "select_by_username"
{
  query "select * from http_auth where username = '$word'";
// $word will be substitute with the read from the 'word' dictionary
  type "select_index";
// query stats will be grouped by type
  has_result_set "y";
// the query is expected to return a result set
  parsed "y";
// the query string should be first processed by super-smack to do
// dictionary substitution
}
dictionary和query组合在一起，定义了需要的测试查询语句。
一条普通的查询语句，比如：select * from http_auth where username = 'test'; 它其实是由一个模板和一个或者几个参数构成的。select * from http_auth where username = '$word'就是这条语句的模板，而$word = test则是设置参数。
query部分定义了查询语句的模板，而dictionary则产生随机的各种参数。
query的结构很简单，最重要的就是“ query "select * from http_auth where username = '$word'"; ”，这定义了查询的模板；
dictionary的结构则非常的灵活，它可以有多种配置：
source_type "file"; -- 表示这个dictionary是一个文件的形式；
source "words.dat"; -- 表示文件名为words.dat;
delim ","; -- 表示words.dat中第一个","之前的部分是这个dictionary的内容；
type：rand -- 表示随机的从中抽取值；
把它们综合起来理解。
之前看过，words.dat的数据是这样的：
josamibdnjdb3,eyhkbsombltouujdrbwcrrcgb,3,485560280
qwdqweqwdwev4,regergergftyyujeqwqwecdfr,4,239013239
rhbtjrbtywlf5,edeffkergpqqmcxvkrtmbhwer,5,233021393
那么，这里定义的dictionary其实是读取word.dat，并将每一行的第一个字符串（比如josamibdnjdb3）作为字典内的元素，再和query组合起来，就变成了实际的查询语句：
select * from http_auth where username = 'qwdqweqwdwev4'
select * from http_auth where username = 'rhbtjrbtywlf5'
select * from http_auth where username = 'josamibdnjdb3'
type除了可以定义为rand，还可以定义为
    seq，表示Values are used sequentially；
    unique，Generate unique values using the same method as gen-data；
source_type除了可以定义为 file，还可以定义为
    list，表示a user-supplied list of words, comma-separated；
    template，表示the format to use when type is unique. For example, "jzawodn_%07d" generates values composed of jzawodn_ and a seven-digit number
举例来说，可以这样定义：
dictionary "gender" {
  type: rand
  source_type "list";
  source "male,female";
}
它表示这个gender的dictionary只有两个值，要么是male，要么是female
dictionary "name"
{
  type "unique";
  source_type "template";
  source "%15s";
}
表示name这个dictionary是一个长度为15的字符串，Super Smack将用gen-data命令产生这个字符串。
很多时候，如果你的查询模板需要多个参数的话，那么你可能需要准备多个dictionary。比如要产生如 select * from tablename where age < 20 AND id = 1234这样的查询，就需要两个dictionary，一个是age，一个是id。
当然，还有一种讨巧的方案，只需要生成一个dictionary，定义它的每一行是这样的：
Age < *** AND id = ***
而对应的query模板就是
select * from tablename where dictionary;
// define database client type
client "smacker1"
{
 user "test"; // connect as this user
 pass ""; // use this password
 host "localhost"; // connect to this host
 db "test"; // switch to this database
 socket "/data0/mysql/mysql.sock"; // this only applies to MySQL and is
// ignored for PostgreSQL
 query_barrel "2 select_by_username"; // on each round,
// run select_by_username query 2 times
}
main
 {
  smacker1.init(); // initialize the client
  smacker1.set_num_rounds($2); // second arg on the command line defines
// the number of rounds for each client 
  smacker1.create_threads($1);
// first argument on the command line defines how many client instances
// to fork. Anything after this will be done once for each client until
// you collect the threads
  smacker1.connect();
// you must connect after you fork 
  smacker1.unload_query_barrel(); // for each client fire the query barrel
// it will now do the number of rounds specified by set_num_rounds()
// on each round, query_barrel of the client is executed
  smacker1.collect_threads();
// the master thread waits for the children, each child reports the stats
// the stats are printed
  smacker1.disconnect();
// the children now disconnect and exit
 }
这里又定义了一个client，它是执行查询语句所用到的client（最早的client是创建表的client，这两个client权限应该不一样。）
query_barrel "2 select_by_username"; -- 表示这个client在一次循环中执行select_by_username这个查询两次；
main中则定义了Super Smack具体的执行流程。很简单，不解释。
http://hidba.org/?p=266
## 解析super-smack的smack文件
April 21st, 2011[hidba](http://hidba.org/?author=1)
Super-smack是一款强大的数据库压测工具，现在支持mysql和PostgreSQL两种数据库，你可以通过简单的配置文件(.smack)来生成一系列测试环境：测试数据，测试表；在测试的过程中，你可以控制客户端的并发数量以及执行频率，根据业务场景配置不同sql，以及他们的执行比率来满足我们需要的业务测试场景；
在安装完[super-smack](http://hidba.org/?p=237)工具后，我们会在smack文件目录中看到一些文件： select-key.smack , update-select.smack这些都是工具自带的配置文件，在运行Super-smack工具来压测DB之前，需要准备压测的表，压测数据，应用场景的sql语句，连接数据库的配置等信息，这些都可以在smack文件中配置完成。
我们只需要依葫芦画瓢，修改其中的配置就行了；
**$./super-smack -d mysql -D /home/mysql/xuancan/super-smack/super-data /home/mysql/xuancan/super-smack-1.3/smacks/my.smack 10 15**
**Query Barrel Report for client smacker**
**connect: max=3ms  min=0ms avg= 0ms from 10 clients**
**Query_type      num_queries     max_time        min_time        q_per_s**
**select_akey   1000    16      2       497.42**
**select_by_id    10000   0       0       4974.16**
**select_userid_aid     5000    0       0       2487.08**
**update_by_id    1500    4       0       746.12**
上面的my.smack配置文件中我配置了按照select_akey(按照akey查询) ，select_by_id (按照主键id查询)，select_userid_aid(按照userid和aid查询)，update_by_id(根据主键更新) 四种业务场景的sql，客户端有10个，每个客户端轮询执行15次的压测场景，其中我们比较关注的是qps，当然得到的测试数据还和主机服务器的配置有关，数据量相关，不能一概而论。
我们拷贝一份select-update.smack文件，并重命名为my.smack,开始编辑my.smack文件：
client “admin”
{
user “root”;
host “localhost”;
db “test”;
pass “”;
socket “/u01/mysql/run/mysql.sock”;
}
该选项是用于配置admin client，由于我们在本机上进行压测，所以使用root用户不用指定密码，如果你在远程客户端上进行压测，那么就需要指定pass密码了；socket用于指定连接mysql使用的sock文件，super-smack默认会到”/tmp/mysql.sock” or “/var/lib/mysql/mysql.sock”这两个地方去读smack文件，我们这里指定了/u01/mysql/run/mysql.sock；
table “auth”
{  client “admin”;
// if the table is not found or does not pass the checks, create it, with the following, dropping the old one if needed
create ” CREATE TABLE auth(
`id` bigint(20) NOT NULL AUTO_INCREMENT,
`userid` bigint(20) unsigned NOT NULL COMMENT ‘用户id’,
`nick` varchar(32) NOT NULL COMMENT ‘nick’,
`aid` bigint(20) unsigned NOT NULL COMMENT ‘应用id’,
`akey` varchar(256) NOT NULL,
`skey` varchar(500) NOT NULL COMMENT ‘skey’,
PRIMARY KEY (`id`),
UNIQUE KEY `ind_auth_userid` (`userid`,`aid`) USING BTREE,
KEY `ind_auth_akey` (akey) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=gbk”;
min_rows “90000″; // the table must have at least that many rows
data_file “userid.dat”; // if the table is empty, load the data from
//this file
gen_data_file “gen-data -n 90000 -f %10-12s%n,%25-25s,%n,%d”;
// if the file above does not exist, generate it with the above command
}
该选项用于定义压测的表：首先这里引用了前面定义的admin client，该表将会按照admin client的定义在test库中检查，如果该表没有，那么将会创建该表，同时我们指定了该表的最小行数，如果表中的行数没有达到min_rows，那么super-smack将会删除掉该表，重建该表；如果该表为空，将会从userid.dat中load数据到该表中，如果改文件不存在，则将用gen_data产生该文件：
%n表示可以从1开始递增的数值，可用于表中的主键
%d表示随机的产生数值
%s表示随机生产一些字母
10-12表示产生字母的范围长度
//define a dictionary
dictionary “userid”
{
type “rand”; // userid are retrieved in random order
source_type “file”; // userid come from a file
source “user.dat”; // file location
delim “,”; // take the part of the line before ,
file_size_equiv “45000″; // if the file is greater than this
//divive the real file size by this value obtaining N and take every Nth
//line skipping others
}
该数据字典用于配置sql查询条件中的值，
type：rand表示随机的从userid.dat中抽取值；
seq表示值是连续的
unique表示用gen-date产生唯一的值
source_type:file表示数据来自于磁盘的文件
list表示由用户提供带有分割符的数据（”one”,”two”,”three”）
template表示当type为unique的时候使用，比如jzawodn_%07d” generates values composed of jzawodn_ and a seven-digit number.
Source：存放在/home/mysql/xuancan/super-smack/super-data中的文件userid.dat
Delim:表示如果你使用带有分隔符的文件，delim告诉super-smack什么分隔符分隔文件
File_size_equiv：如果你的数据字典文件非常大，该选项将会很有用，如果你的文件为10k，指定file_size_equiv为1024，那么super-smack将会使用1/10的数据来测试；
备注：如果我们的查询值有多个，比如根据aid+userid来查询，那么需要定义两个数据字典：aid和userid。
**SELECT aid INTO OUTFILE “/home/mysql/xuancan/super-smack/super-data/aid.dat”**
FIELDS TERMINATED BY ‘,’
OPTIONALLY ENCLOSED BY ‘”‘
LINES TERMINATED BY “\n”
FROM auth;
**SELECT useid INTO OUTFILE “/home/mysql/xuancan/super-smack/super-data/userid.dat”**
FIELDS TERMINATED BY ‘,’
OPTIONALLY ENCLOSED BY ‘”‘
LINES TERMINATED BY “\n”
FROM auth;
**SELECT akey INTO OUTFILE “/home/mysql/xuancan/super-smack/super-data/akey.dat”**
FIELDS TERMINATED BY ‘,’
OPTIONALLY ENCLOSED BY ‘”‘
LINES TERMINATED BY “\n”
FROM auth;
//define a query
query “select_by_userid”
{
query “select * from auth where userid = ‘$userid’”;
// $word will be substitute with the read from the ‘userid’ dictionary
type “select_by_userid”;
// query stats will be grouped by type
has_result_set “y”;
// the query is expected to return a result set
parsed “y”;
// the query string should be first processed by super-smack to do
// dictionary substitution
}
query “update_by_aid”
{
query “update auth set akey=’$akey’ where aid= ‘$aid’”;
// $word will be substitute with the read from the ‘word’ dictionary
// note that the first word is not the same as the second, as each is
// a separate random draw from the dictionary
type “update_index”;
// query stats will be grouped by type
has_result_set “n”;
// the query is expected to return a result set
parsed “y”;
// the query string should be first processed by super-smack to do
// dictionary substitution
}
定义查询：query定义查询的sql，其中查询的值有刚才定义的数据字典word来获得
Type：在生成结果的时候显示的名字；
Has_result_set：如果是sql为select，则该值设置为y，若为update，则为n
Parsed：表示word的值数据字典是将该值置为y
// define database client type
client “smacker”
{
user “test”; // connect as this user
pass “test”; // use this password
host “localhost”; // connect to this host
db “test”; // switch to this database
socket “/u01/mysql/run/mysql.sock”; // this only alies to MySQL and is
// ignored for PostgreSQL
query_barrel “2 select_akey  15 select_by_id   5  select_userid_aid 10 update_by_id“; // on each round
}
与前面定义的admin client不同的是在smacker client中多定义了query_barrel，query_barrel定义了查询的顺序和执行次数，也是就是我们常说的业务场景，你的select update delete的比例是多少；
main
{
smacker.init(); // initialize the clients
smacker.set_num_rounds($2); // second arg on the command line defines
// the number of rounds for each client
smacker.create_threads($1);
// first argument on the command line defines how many client instances
// to fork. Anything after this will be done once for each client until
// you collect the threads
smacker.connect();
// you must connect after you fork
smacker.unload_query_barrel(); // for each client fire the query barrel
// it will now do the number of rounds specified by set_num_rounds()
// on each round, query_barrel of the client is executed
smacker.collect_threads();
// the master thread waits for the children, each child reports the stats
// the stats are printed
smacker.disconnect();
// the children now disconnect and exit
}
最后定义的中我们需要注意$1和$2两个参数，也就是我们调用super-smack进行压测的时候的10 15，10代表了有多少客户端同时来进行测试，15则代表了每个客户端轮询执行多少次查询；
参考：[http://imysql.cn/docs/High_Performance_MySQL/0596003064/hpmysql-CHP-3-SECT-3.html](http://imysql.cn/docs/High_Performance_MySQL/0596003064/hpmysql-CHP-3-SECT-3.html)
smack文件，类似于一个c源文件，具体包括一下几个内容
1.client，定义创建连接用到的参数，包括host，user，password，port，socket。包括两种client，admin client和普通client，admin需要具有管理权限，需要创建表以及load数据等操作
2.表定义，自定义测试表的表结构，需要指定由哪个client创建表，以及表的记录数，以及填充数据文件的位置，如果数据文件不存在，需要生成数据文件，可以自定义数据生成脚本
3.dictionary，定义了一批可选的字段，源码实现得比较简单，只提供了几种next方法，读取下一行数据，**如果当前行数据用逗号分开，只取第一个逗号前的字段**，~~其他符号分隔则取整行数据~~。所以如果一个查询里有几个字段需要从外部获取数据，就应该创建几个dictionary
4.查询，可以自定义查询的语句，查询类型(主要用于分类统计的作用)，查询语句也可以为更新操作，如update。如果是查询语句，has_result_set选项应该定义为y，否则会出现Commands out of sync错误。
5.main，测试运行的入口，一般改动不大，主要是一些client名称的改动
测试过程中创建的连接数包括：
1，表数据分析连接(select count(*) from test_table)，判断表是否已经装载了数据
2，线程数，每个线程执行的查询都只打开一个连接，与执行的次数以及每个线程执行的多少条语句无关
输出结果
输出的结果较为简单，只包含了查询的次数，最大耗时，最小耗时，每秒执行的查询会按照查询类型进行分类统计然后输出
Query_type num_queries max_time min_time q_per_s
select_index 100 697 419 14.43
select_primarykey 500 6 0 72.15
下面再给一份配置好的smack-file.smack
// this is will be used in the table section
client "admin"
{
user "test";
host "localhost";
db "test";
pass "test";
port "4331";
**// socket **"/home/netease//mysql/node-1/mysqld.sock";
 // this only applies to MySQL and is
// ignored for PostgreSQL
}
// ensure the table exists and meets the conditions
table "test_a"
{
client "admin"; // connect with this client
// if the table is not found or does not pass the checks create it
// with the following dropping the old one if needed
create "create table test_a
(Id bigint not null
UserId bigint(20) not null
UserName varchar(255) not null
primary key(Id)
index IDX_userName(userName)
)engine=inndb";
min_rows ""; // the table must have at least that many rows
data_file "/home/netease/super-smack/data/**test_f.txt**"; // if the table is empty load the data from
//this file
gen_data_file "gen-data -n -f %12-12s%n%25-25s%n%d";
// if the file above does not exist generate it with the above command
}
//define a dictionary
dictionary "word"
{
type "rand"; // words are retrieved in random order
source_type "file"; // words come from a file
source "/home/netease/super-smack/data/t**est_f_update.txt**"; // file location
delim ""; // take the part of the line before 
file_size_equiv ""; // if the file is greater than this
//divive the real file size by this value obtaining N and take every Nth
//line skipping others
}
dictionary "name"
{
type "rand"; // words are retrieved in random order
source_type "file"; // words come from a file
source "/home/netease/super-smack/data/**test_f_name.txt**"; // file location
delim ""; // take the part of the line before 
file_size_equiv ""; // if the file is greater than this
}
//define a query
query "select_by_Id"
{
query "select * from test_a where Id = '$word'";
// $word will be substitute with the read from the 'word' dictionary
type "select_index";
// query stats will be grouped by type
has_result_set "y";
// the query is expected to return a result set
parsed "y";
// the query string should be first processed by super-smack to do
// dictionary substitution
}
query "update_by_Id"
{
query "update test_a set UserName='$name' where Id = '$word'";
// $word will be substitute with the read from the 'word' dictionary
// note that the first word is not the same as the second as each is
// a separate random draw from the dictionary
type "update_primarykey";
// query stats will be grouped by type
has_result_set "n";
// the query is expected to return a result set
parsed "y";
// the query string should be first processed by super-smack to do
// dictionary substitution
}
// define database client type
client "smacker"
{
user "test"; // connect as this user
pass "test"; // use this password
host "localhost"; // connect to this host
db "test"; // switch to this database
port "4331";
// socket "/home/netease/mysql/node-1/mysqld.sock"; // this only applies to MySQL and is
// ignored for PostgreSQL
query_barrel "1 select_by_Id 1 update_by_Id"; // on each round
// run select_by_username query followed by update_by_username
}
main
{
smacker.init(); // initialize the clients
smacker.set_num_rounds($2); // second arg on the command line defines
// the number of rounds for each client
smacker.set_num_rounds($2);
smacker.create_threads($1);
// first argument on the command line defines how many client instances
// to fork. Anything after this will be done once for each client until
// you collect the threads
smacker.connect();
// you must connect after you fork
smacker.unload_query_barrel(); // for each client fire the query barrel
// it will now do the number of rounds specified by set_num_rounds()
// on each round query_barrel of the client is executed
smacker.collect_threads();
// the master thread waits for the children each child reports the stats
// the stats are printed
smacker.disconnect();
// the children now disconnect and exit
}
