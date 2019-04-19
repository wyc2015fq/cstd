# node.js中的require - 左直拳的马桶_日用桶 - CSDN博客
2014年12月05日 17:36:34[左直拳](https://me.csdn.net/leftfist)阅读数：1794
初初接触node.js，印象最深刻的就是开头密密麻麻的require了。
这是什么东西？
其实也没啥大惊小怪的。require就是为了引用别的js文件，利于模块化编程，重用、以及避免过多代码挤在同一个文件里，就像.net的using，java的import，c++的#include一样。
对于这个require，有以下一些要点：
1、require可以是系统模块、第三方模块、自己写的JS文件，甚至有可能是一个目录！
```java
1、require("http");//系统模块
2、require("socket.io");//第三方模块
3、require("./login");//自定义文件
4、require("./user");//同层目录并无一个叫user.js的文件，但有一个[user]文件夹，其下有许多js，其中有一个index.js，再在index.js里require文件夹下其他JS
```
对于第4种，会这样使用：
/user/下有文件
index.js
login.js
regist.js
那么在index.js下会这样写：
```java
exports.login = require("./login");
exports.regist = require("./regist");
```
然后 /app.js中这样写：
```java
var user = require("./user");
user.login();
user.regist();
```
其实1、2这种方式，也是引用目录级别的
2、require返回的不一定就是函数，也有可能是其他javascript对象
通常，require返回的都是函数。但这也不是固定的，也有可能是字符串、数组、JSON，凡是javascript对象，皆有可能。关键是要看被引用的那个文件，将什么东西赋给了module.exports
