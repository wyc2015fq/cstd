# Nodejs 与MySQL交互（felixge/node-mysql） - z69183787的专栏 - CSDN博客
2016年05月10日 11:39:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：547
### 目录
简介和安装 测试[MySQL](http://www.it165.net/database/dbmy/) 认识一下Connection
 Options MYSQL CURD 插入 更新 查询 删除 Nodejs 调用带out参数的存储过程，并得到out参数返回值 结束数据库连接两种方法和区别 连接池Pooling connections 创建 其它连接池配置选项 释放 使用示例 断线重连 其它...
### 简介和安装
　　Node.js与[MySQL](http://www.it165.net/database/dbmy/)交互操作有很多库，具体可以在 https://www.npmjs.org/search?q=[MySQL](http://lib.csdn.net/base/14) 
 查看。
　　我选择了felixge/node-mysql，用的人比较多，先随大溜看看它的使用，**暂时没有太过纠结于各库之间的执行性能问题**，对其它库有研究的筒子也可以分享一下性能要求较高时的选择^_^! 
　　地址：https://github.com/felixge/node-mysql
　　　　   https://www.npmjs.org/package/mysql
> 
This is a node.js driver for mysql. It is written in JavaScript, does not require compiling, and is 100% MIT licensed
　　用纯JavaScript实现。MIT 100%!
**安装**
> <code class="lang-bash">npm install mysql</code>
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807110.png)
 　注意：安装前先把目录cd到node.exe所在目录下，这样执行安装命令时，会找到目录下node_modules，并安装在此目录下，否则使用mysql时，你会出现 Error: Cannot find module 'mysql' 
### 测试MySQL
　　MySQL版本：5.5
　　在官方示例的基础下修改一下并测试：
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807112.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``);  ``//调用MySQL模块`
`02.`
`03.``//创建一个connection`
`04.`
```
var
 connection = mysql.createConnection({
```
`05.`
```
host    
 :
```
`'192.168.0.200'``,       ``//主机`
`06.`
```
user    
 :
```
`'root'``,               ``//MySQL认证用户名`
`07.`
```
pass<a
 href=
```
`"[http://www.it165.net/edu/ebg/](http://www.it165.net/edu/ebg/)"``target=``"_blank"``class``=``"keylink"`
```
>word</a>
 :
```
`'abcd'`
```
,       
 //MySQL认证用户密码
```
`08.``port: ``'3306'``,                   ``//端口号`
`09.``});`
`10.``//创建一个connection`
`11.``connection.connect(function(err){`
`12.``if``(err){       `
`13.``console.log(`
```
'[query]
 - :'
```
`+err);`
`14.``return``;`
`15.``}`
`16.``console.log(`
```
'[connection
 connect]  succeed!'
```
`);`
`17.``}); `
`18.``//执行SQL语句`
`19.``connection.query(`
```
'SELECT
 1 + 1 AS solution'
```
```
,
 function(err, rows, fields) {
```
`20.``if`
```
(err)
 {
```
`21.``console.log(`
```
'[query]
 - :'
```
`+err);`
`22.``return``;`
`23.``}`
`24.``console.log(`
```
'The
 solution is: '
```
`, rows[``0``].solution); `
`25.``}); `
`26.``//关闭connection`
`27.``connection.end(function(err){`
`28.``if``(err){       `
`29.``return``;`
`30.``}`
`31.``console.log(`
```
'[connection
 end] succeed!'
```
`);`
`32.``});`
测试示例源码
**成功执行！**
### 认识一下Connection Options
　　要想创建一个数据库连接，先就要认识清楚Options
host：主机地址 （默认：localhost）
　　user：用户名
　　pass[word](http://www.it165.net/edu/ebg/)：密码
　　port：端口号 （默认：3306）
　　database：数据库名
　　charset：连接字符集（默认：'UTF8_GENERAL_CI'，注意字符集的字母都要大写）
　　localAddress：此IP用于TCP连接（可选）
　　socketPath：连接到unix域路径，当使用 host 和 port 时会被忽略
　　timezone：时区（默认：'local'）
　　connectTimeout：连接超时（默认：不限制；单位：毫秒）
　　stringifyObjects：是否序列化对象（默认：'false' ；与安全相关https://github.com/felixge/node-mysql/issues/501）
　　typeCast：是否将列值转化为本地JavaScript类型值 （默认：true）
　　queryFormat：自定义query语句格式化方法 https://github.com/felixge/node-mysql#custom-format
　　supportBigNumbers：数据库支持bigint或decimal类型列时，需要设此option为true （默认：false）
　　bigNumberStrings：supportBigNumbers和bigNumberStrings启用 强制bigint或decimal列以JavaScript字符串类型返回（默认：false）
　　dateStrings：强制timestamp,datetime,data类型以字符串类型返回，而不是JavaScript Date类型（默认：false）
　　debug：开启调试（默认：false）
　　multipleStatements：是否许一个query中有多个MySQL语句 （默认：false）
　　flags：用于修改连接标志，更多详情：https://github.com/felixge/node-mysql#connection-flags
　　ssl：使用ssl参数（与crypto.createCredenitals参数格式一至）或一个包含ssl配置文件名称的字符串，目前只捆绑Amazon RDS的配置文件
　　其它：
　　可以使用URL形式的加接字符串，不多介绍了，不太喜欢那种格式，觉得可读性差，也易出错，想了解的可以去主页上看。
### MYSQL CURD
 　　首先创建一个测试数据库nodesample，在数据库中建一个userinfo表
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
CREATE
 DATABASE IF NOT EXISTS nodesample CHARACTER SET UTF8;
```
`02.`
`03.`
```
USE
 nodesample;
```
`04.`
`05.`
```
SET
 FOREIGN_KEY_CHECKS=
```
`0``;`
`06.`
`07.`
```
DROP
 TABLE IF EXISTS `userinfo`;
```
`08.`
```
CREATE
 TABLE `userinfo` (
```
`09.```Id` ``int``(``11`
```
)
 NOT NULL AUTO_INCREMENT COMMENT
```
`'主键'``,`
`10.`
```
`UserName`
 varchar(
```
`64`
```
)
 NOT NULL COMMENT
```
`'用户名'``,`
`11.`
```
`UserPass`
 varchar(
```
`64`
```
)
 NOT NULL COMMENT
```
`'用户密码'``,`
`12.`
```
PRIMARY
 KEY (`Id`)
```
`13.`
```
)
 ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT=
```
`'用户信息表'``;`
**1.向UserInfo表中插入一条数据**
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807118.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``); `
`02.`
`03.`
```
var
 connection = mysql.createConnection({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'``,`
`09.``});`
`10.`
`11.``connection.connect();`
`12.`
`13.`
```
var 
 userAddSql =
```
```
'INSERT
 INTO userinfo(Id,UserName,UserPass) VALUES(0,?,?)'
```
`;`
`14.`
```
var 
 userAddSql_Params = [
```
`'Wilson'``, ``'abcd'``];`
`15.``//增`
`16.`
```
connection.query(userAddSql,userAddSql_Params,function
 (err, result) {
```
`17.``if``(err){`
`18.``console.log(`
```
'[INSERT
 ERROR] - '
```
`,err.message);`
`19.``return``;`
`20.``}       `
`21.`
`22.``console.log(``'--------------------------INSERT----------------------------'``);`
`23.`
```
//console.log('INSERT
 ID:',result.insertId);
```
`24.``console.log(`
```
'INSERT
 ID:'
```
`,result);       `
`25.``console.log('-----------------------------------------------------------------`
`26.`
`27.``'); `
`28.``});`
`29.`
`30.``connection.end();`
插入示例源码
> 
从执行结果可以看出，result中包含一些有用的信息，affectedRows（受影响的行数） insertId（插入的主键ID）等等。。。
有受影响的行数和插入数据的ID，就可以很方便进行后续的一些操作（比如判断是否成功或者继续根据插入数据的ID进行其它操作）
　　下面，我去数据库服务器中使用Navicate for MySQL工具查询一下UserInfo表
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807121.png)
　　插入成功！
**2.更新1操作中插入的数据信息**
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807122.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``); `
`02.`
`03.`
```
var
 connection = mysql.createConnection({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'``,`
`09.``});`
`10.`
`11.``connection.connect();`
`12.`
`13.`
```
var
 userModSql =
```
```
'UPDATE
 userinfo SET UserName = ?,UserPass = ? WHERE Id = ?'
```
`;`
`14.`
```
var
 userModSql_Params = [
```
`'钟慰'``, ``'5678'``,``1``];`
`15.``//改`
`16.`
```
connection.query(userModSql,userModSql_Params,function
 (err, result) {
```
`17.``if``(err){`
`18.``console.log(`
```
'[UPDATE
 ERROR] - '
```
`,err.message);`
`19.``return``;`
`20.``}       `
`21.``console.log(``'--------------------------UPDATE----------------------------'``);`
`22.``console.log(`
```
'UPDATE
 affectedRows'
```
`,result.affectedRows);`
`23.``console.log('-----------------------------------------------------------------`
`24.`
`25.``');`
`26.``});`
`27.`
`28.``connection.end();`
更新示例源码
　　从返回受影响行数来看，更新应该是正确的，下面我们用一个SELECT查询示例去验证更新是否成功！
**3.查询**
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807123.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``); `
`02.`
`03.`
```
var
 connection = mysql.createConnection({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'``,`
`09.``});`
`10.`
`11.``connection.connect();`
`12.`
`13.`
```
var 
 userGetSql =
```
```
'SELECT
 * FROM userinfo'
```
`;`
`14.``//查`
`15.`
```
connection.query(userGetSql,function
 (err, result) {
```
`16.``if``(err){`
`17.``console.log(`
```
'[SELECT
 ERROR] - '
```
`,err.message);`
`18.``return``;`
`19.``}       `
`20.`
`21.``console.log(``'--------------------------SELECT----------------------------'``);`
`22.``console.log(result);       `
`23.``console.log('-----------------------------------------------------------------`
`24.`
`25.``'); `
`26.``});`
`27.`
`28.``connection.end();`
查询示例源码
> 
从查询出来的结果可以看出，result返回了一个JSON格式的数据，同时表示第二步中更新是成功!
**4.删除**
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807124.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``); `
`02.`
`03.`
```
var
 connection = mysql.createConnection({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'``,`
`09.``});`
`10.`
`11.``connection.connect();`
`12.`
`13.`
```
var 
 userDelSql =
```
```
'DELETE
 FROM userinfo'
```
`;`
`14.``//删`
`15.`
```
connection.query(userDelSql,function
 (err, result) {
```
`16.``if``(err){`
`17.``console.log(`
```
'[DELETE
 ERROR] - '
```
`,err.message);`
`18.``return``;`
`19.``}       `
`20.`
`21.``console.log(``'--------------------------DELETE----------------------------'``);`
`22.``console.log(`
```
'DELETE
 affectedRows'
```
`,result.affectedRows);`
`23.``console.log('-----------------------------------------------------------------`
`24.`
`25.``'); `
`26.``});`
`27.`
`28.``connection.end();`
删除示例源码
　　去数据库服务器里查询看一下
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808125.png)
> 
查询userinfo表中已没有数据，删除成功！
　　到此Node.js结合MySQL的增、删、改、查操作应会部完成了！
### Nodejs 调用带out参数的存储过程，并得到out参数返回值
　　Nodejs调用存储过程没什么特别，只是调用带out参数存储过程并得到out参数返回值可能有些人比较疑惑，下面用个示例来介绍一下
　　先创建一个带out参数的存储过程 P_UserInfo
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
--
 ----------------------------
```
`02.`
```
--
 Procedure structure
```
`for```P_UserInfo``
`03.`
```
--
 ----------------------------
```
`04.`
```
DROP
 PROCEDURE IF EXISTS `P_UserInfo`;
```
`05.`
```
DELIMITER
 ;;
```
`06.`
```
CREATE
 DEFINER=`root`@`localhost` PROCEDURE `P_UserInfo`(IN ExtId INT,IN ExtUserName VARCHAR(
```
`64`
```
),IN
 ExtUserPass VARCHAR(
```
`64`
```
),OUT
 ExtReturnVal INT)
```
`07.`
```
TOP:
 BEGIN
```
`08.`
```
DECLARE
 EXIT HANDLER FOR SQLEXCEPTION
```
`09.``BEGIN`
`10.``ROLLBACK;`
`11.`
```
SET
 ExtReturnVal =
```
`0`
```
; 
 -- Failed
```
`12.``END;`
`13.`
`14.`
```
START
 TRANSACTION;
```
`15.`
`16.`
```
INSERT
 INTO userinfo(Id,UserName,UserPass) VALUES(ExtId,ExtUserName,ExtUserPass);
```
`17.`
`18.`
```
SET
 ExtReturnVal =
```
`1``;`
`19.`
```
SELECT
 ExtReturnVal;
```
`20.``COMMIT;`
`21.``END`
`22.``;;`
`23.`
```
DELIMITER
 ;
```
　　下面，来写个示例来调用
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808126.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql  = require(
```
`'mysql'``); `
`02.`
`03.`
```
var
 connection = mysql.createConnection({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'`
`09.``});`
`10.`
`11.``connection.connect();`
`12.`
`13.`
```
var 
 userProc =
```
```
'call
 P_UserInfo(?,?,?,@ExtReturnVal);'
```
`;`
`14.`
```
var
 userProc_Params = [
```
`0``,`
```
'Wilson
 Z'
```
`,``'abcd'``];`
`15.``//调用存储过程`
`16.`
```
connection.query(userProc,userProc_Params,function
 (err, retsult) {
```
`17.``if``(err){`
`18.``console.log(`
```
'[EXEC
 PROC ERROR] - '
```
`,err.message);`
`19.``return``;`
`20.``}       `
`21.`
`22.``console.log(``'--------------------------PROC----------------------------'``);`
`23.``console.log(retsult);      `
`24.``console.log(retsult[``0``][``0``].ExtReturnVal);`
`25.``console.log('-----------------------------------------------------------------`
`26.`
`27.``'); `
`28.``});`
`29.`
`30.``connection.end();`
调用存储过程得到out参数示例源码
　　去数据为中看一下
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808127.png)
> 
在表中正确插入此数据，而且正确的得到了out参数的值，细心的可能会发现我存储过程中在SET ExtReturnVal = 1表示成功后，多了一句SELECT ExtReturnVal;
这样就可以通地查询得到out参数的值了！（有别的方法可以分享一下^_^!）
### 结束数据库连接两种方法和区别
　　前面的示例中我在结尾处都会调用一个connection.end()方法,这个方法connection.connect()对应，一个开始，一个结束！
　　结束连接其实有两种方法end()，destory()；
end()
　　end()方法在queries都结束后执行，end()方法接收一个回调函数，queries执行出错，仍然后结束连接，错误会返回给回调函数err参数，可以在回调函数中处理！
destory()
　　比较暴力，没有回调函数，即刻执行，不管queries是否完成！
### 连接池Pooling connections
**1.连接池的创建，使用createPool方法，options和createConntion一致，可以监听connection事件**。
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql = require(
```
`'mysql'``);`
`02.`
`03.``//创建连接池`
`04.`
```
var
 pool  = mysql.createPool({
```
`05.`
```
host    
 :
```
`'192.168.0.200'``,`
`06.`
```
user    
 :
```
`'root'``,`
`07.`
```
password
 :
```
`'abcd'`
`08.``});`
`09.`
`10.``//监听connection事件`
`11.``pool.on(``'connection'`
```
,
 function(connection) {
```
`12.``connection.query(`
```
'SET
 SESSION auto_increment_increment=1'
```
`);`
`13.``});`
　　连接池可以直接使用，也可以共享一个连接或管理多个连接（引用官方示例）
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.``//直接使用`
`02.``pool.query(`
```
'SELECT
 1 + 1 AS solution'
```
```
,
 function(err, rows, fields) {
```
`03.``if``(err) ``throw``err;`
`04.`
`05.``console.log(`
```
'The
 solution is: '
```
`, rows[``0``].solution);`
`06.``});`
`07.`
`08.``//共享`
`09.`
```
pool.getConnection(function(err,
 connection) {
```
`10.`
```
//
 connected! (unless `err` is set)
```
`11.``});`
**2.其它连接池配置选项**
　　waitForConnections
 　　当连接池没有连接或超出最大限制时，设置为true且会把连接放入队列，设置为false会返回error
　　connectionLimit
 　　连接数限制，默认：10
　　queueLimit
 　　最大连接请求队列限制，设置为0表示不限制，默认：0
**3.释放**
　　调用connection.release()方法，会把连接放回连接池，等待其它使用者使用!
**4.使用示例**
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808128.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql = require(
```
`'mysql'``);`
`02.`
`03.`
```
var
 pool  = mysql.createPool({
```
`04.`
```
host    
 :
```
`'192.168.0.200'``,      `
`05.`
```
user    
 :
```
`'root'``,             `
`06.`
```
password
 :
```
`'abcd'``,      `
`07.``port: ``'3306'``,                  `
`08.``database: ``'nodesample'`
`09.``});`
`10.`
`11.`
```
pool.getConnection(function(err,
 connection) {
```
`12.`
`13.``connection.query( `
```
'SELECT
 * FROM userinfo;'
```
```
,
 function(err, result) {
```
`14.``console.log(result);`
`15.``connection.release();`
`16.``});`
`17.`
`18.``connection.query( `
```
'SELECT
 * FROM userinfo;'
```
```
,
 function(err, result) {
```
`19.`
`20.``console.log(result);`
`21.``connection.release();`
`22.`
`23.``});`
`24.``});`
连接池使用示例源码
　　在实际开发过程中，应该还是使用连接池的方式比较好！
### 断线重连
　　数据库可以因为各种原因导致连接不上，这种就必须有重连接机制！
　　主要判断errorcode:PROTOCOL_CONNECTION_LOST 
　　1.首先去数据库服务器停止MySQL服务
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808129.png)
　　2.运行断线重连代码
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808130.png)
> 
代码中在error事件中，判断返回errorcode是否为：PROTOCOL_CONNECTION_LOST ，如果是用setTimeout定时2秒重连！
从执行结果可以看出，大约2-3秒会输出重连输出信息，不断在尝试重新连接！
　　3.去数据为服务器，开启mysql服务器，再看看执行结果
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808131.png)
> 
当数据库服务器mysql服务重新启动后，执行结果输出连接成功，不再输出断线重连日志^_^!
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql = require(
```
`'mysql'``);`
`02.`
```
var
 db_config = {
```
`03.`
```
host    
 :
```
`'192.168.0.200'``,      `
`04.`
```
user    
 :
```
`'root'``,             `
`05.`
```
password
 :
```
`'abcd'``,      `
`06.``port: ``'3306'``,                  `
`07.``database: ``'nodesample'`
`08.``};`
`09.`
`10.`
```
var
 connection;
```
`11.`
```
function
 handleDisconnect() {
```
`12.`
```
connection
 = mysql.createConnection(db_config);
```
`13.`
```
connection.connect(function(err)
 {
```
`14.``if`
```
(err)
 {
```
`15.``console.log(``'进行断线重连：'``+ ``new``Date());`
`16.``setTimeout(handleDisconnect, ``2000``);   ``//2秒重连一次`
`17.``return``;`
`18.``}        `
`19.``console.log(``'连接成功'``); `
`20.``});                                                                          `
`21.``connection.on(``'error'`
```
,
 function(err) {
```
`22.``console.log(`
```
'db
 error'
```
`, err);`
`23.``if`
```
(err.code
 ===
```
`'PROTOCOL_CONNECTION_LOST'`
```
)
 {
```
`24.``handleDisconnect();                        `
`25.``} ``else``{                                     `
`26.``throw``err;                                `
`27.``}`
`28.``});`
`29.``}`
`30.``handleDisconnect();`
断线重连示例源码
### 其它...
**１. escape()**
　　防止SQL注入，可以使用pool.escape()和connect.escape()，不多说了，自已可以试试，我提供一个示例
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192808132.png)
![\](http://www.it165.net/uploadfile/files/2014/0414/20140414192807114.gif)
[view
 source](http://www.it165.net/database/html/201404/5954.html#viewSource)[print](http://www.it165.net/database/html/201404/5954.html#printSource)[?](http://www.it165.net/database/html/201404/5954.html#about)
`01.`
```
var
 mysql = require(
```
`'mysql'``);`
`02.`
`03.`
```
var
 pool = mysql.createPool({
```
`04.``host: ``'192.168.0.200'``,    `
`05.``user: ``'root'``,`
`06.``password:``'abcd'``,`
`07.``port:``'3306'``,`
`08.``database:``'nodesample'`
`09.``});`
`10.`
`11.``pool.getConnection(function(err,connection){`
`12.`
`13.``connection.query(`
```
'SELECT
 * FROM userinfo WHERE id = '
```
`+ `
```
'5
 OR ID = 6'
```
`,function(err,result){`
`14.``//console.log(err);`
`15.``console.log(result);`
`16.``connection.release();`
`17.``});`
`18.`
`19.``connection.query(`
```
'SELECT
 * FROM userinfo WHERE id = '
```
```
+
 pool.escape(
```
```
'5
 OR ID = 6'
```
`) ,function(err,result){`
`20.``//console.log(err);`
`21.``console.log(result);`
`22.``connection.release();`
`23.``});`
`24.``})`
escape() 示例源码
> 
结果可以看出，第1个query拼接条件可以被执行，而通过escape方法转义后的忽略了后面的拼接的部分！
大家可以看到我前面用的？占位的方式，简单的试了一下，好处并没有这种危险，这里就不提供示例了，在我上面提供的代码上改一下就可以试出来^_^!
**2.mysql.escapeId(identifier)**
　　如果不能信任由用户提示的SQL标识符（数据库名，列名，表名），可以使用此方法，官方提供有示例（最常见的是通过列名来排序什么的...） 　　
**3.mysql.format**
准备查询，该函数会选择合适的转义方法转义参数
 　  ....
　　还有一些安全相关的方法可以自行查看一下官方说明
**本次也未讲到连接池集群，以后有时间再把这次漏掉的一些补一篇当提高篇吧，现在基础的和MySQL交互应该问题不大了^_^!**
