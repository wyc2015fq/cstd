# JSON语法简介 介绍 json - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月12日 10:12:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：345
JSON 指的是 JavaScript 对象表示法（JavaScript Object Notation），类似 XML，但比 XML 更小、更快，更易解析。
## 实例
**{**"employees": **[****{**"firstName":"Bill" , "lastName":"Gates"**}**,**{**"firstName":"George" , "lastName":"Bush"**}**,**{**"firstName":"Thomas" , "lastName":"Carter"**}****]****}**
这个 employee 对象是一个包含 3 个员工记录（对象）的数组。
## 规则
- 数据在名称/值对中
- 数据由逗号分隔
- 花括号保存对象
- 方括号保存数组
其中名称/值对为字段名称（在双引号中）后面跟一个冒号，然后是值
"firstName" : "John"
JSON 值可以是：
- 数字（整数或浮点数）
- 字符串（在双引号中）
- 逻辑值（true 或 false）
- 数组（在方括号中）
- 对象（在花括号中）
- null
JSON 对象在花括号中书写： 对象可以包含多个名称/值对：
**{**"firstName":"John" , "lastName":"Doe"**}**
JSON 数组在方括号中书写： 数组可包含多个对象：
**{**"employees": **[****{**"firstName":"John" , "lastName":"Doe"**}**,**{**"firstName":"Anna" , "lastName":"Smith"**}**,**{**"firstName":"Peter" , "lastName":"Jones"**}****]****}**
在上面的例子中，对象 "employees" 是一个包含三个对象的数组。每个对象代表一条关于某人（有姓和名）的记录。
## 艺搜参考
[http://www.w3school.com.cn/json/index.asp](http://www.w3school.com.cn/json/index.asp)
[http://www.json.org/json-zh.html](http://www.json.org/json-zh.html)
