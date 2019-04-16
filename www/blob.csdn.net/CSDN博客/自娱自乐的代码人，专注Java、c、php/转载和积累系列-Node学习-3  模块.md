# 转载和积累系列 - Node学习 - 3. 模块 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月26日 16:04:19[initphp](https://me.csdn.net/initphp)阅读数：686标签：[function																[html																[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)




- 什么是模块

> 
nodejs的模块相当于js的文件。例如 var http = require("http") 是调用的nodejs的核心模块。一般的写好的js模块也是通过require来调用。

- 
一个简单的模块例子：


> 
module.js是模块JS文件。模块对外可访问的函数，一般用exports.xxxx，放在test/文件夹下

```java
var name;
exports.setName = function (thisName) {
	name = 	thisName;
}

exports.getName = function () {
	return name;	
}
```



index.js是调用module.js这个文件。调用后可以直接使用里面的方法。

```java
var myModule = require("./test/module");
myModule.setName("html PHP");
console.log(myModule.getName());
```



- 
调用js的对象。


> 
module.js中是一个age的对象，模块调用对象需要在module.js中加入module.exports = age

```java
var age = function () {
	var age;
	
	this.setAge = function (myAge) {
		age = myAge;	
	}	
	
	this.getAge = function () {
		return age;	
	}
}

module.exports = age;
```



index.js中调用对象需要实例化，需要new一个对象，require是加载进来这个模块，并可以使用。

```java
var myAge = require("./test/module");
myAge = new myAge();
myAge.setAge(1000000);
console.log(myAge.getAge());
```](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)




