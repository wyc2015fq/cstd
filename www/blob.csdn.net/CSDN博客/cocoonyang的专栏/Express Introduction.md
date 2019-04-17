# Express  Introduction - cocoonyang的专栏 - CSDN博客





2017年03月25日 11:22:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：275
个人分类：[nodejs](https://blog.csdn.net/cocoonyang/article/category/1301193)












1)  Create a directory named as d:/foo

2) Create a file name as package.json



```
{
  "name": "Your app name",
  "description": "A concisense description of the app",
  "version": "0.0.1",
  "private": true,
  "dependencies": {
    "express": "4.x"
  }
}
```




3) In the directory d:/foo, run 



```java
npm install
```
which will install Express and its dependencies.




4) Create a init JavaScript file, named as, for example, 'app.js'



```java
var express = require('express');
var app = express();

app.use(express.static(__dirname + '/public'));

app.listen(8080);
```


5) Run app.js 



```java
node app.js
```


Done!






