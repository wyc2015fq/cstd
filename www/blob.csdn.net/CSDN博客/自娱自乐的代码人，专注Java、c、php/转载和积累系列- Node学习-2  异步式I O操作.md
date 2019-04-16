# 转载和积累系列 -  Node学习 - 2. 异步式I/O操作 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月25日 14:15:25[initphp](https://me.csdn.net/initphp)阅读数：1855标签：[io																[function																[数据库																[网络																[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)




- 阻塞式I/O：

> 
程序在执行过程中，遇到比较耗时的操作，例如读取数据库、网络交互等，只有等这些操作执行完毕后，程序才继续往下执行，例如PHP就是典型的阻塞式I/O操作。



- 
异步式I/O：


> 
针对所有I/O操作时，不采用阻塞的策略，会继续执行下面的程序语句。Node就是异步式的。



- 
代码，执行文件读取操作，有同步IO和异步IO两种方法：


```java
var fs = require("fs");
var filename = 'hello.txt';
fs.readFile(filename, 'utf-8', function (error, data) {
	if (error) {
		console.log("error");	
	} else {
		console.log('yb IO' + data);	
	}										 
});
var datas = fs.readFileSync(filename, 'utf-8');
console.log('tb IO' + datas);

console.log("end");
```
- 打印结果

![](https://img-my.csdn.net/uploads/201207/25/1343197476_8900.jpg)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)




