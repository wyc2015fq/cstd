# Json学习 - BlueSky - CSDN博客
2016年01月19日 11:11:31[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：283
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)
## 首先说下 JSON 的五点语法：
- 
数组（Array）用方括号(“[]”)表示。
- 
对象（Object）用大括号（”{}”）表示。
- 
名称/值对（name/value）之间用冒号（”：”）隔开。
- 
名称（name）置于双引号中，值（value）有字符串、数值、布尔值、null、对象和数组。
- 
并列的数据之间用逗号（“,”）分隔
```
{}  //空对象
{"name":"Geoff Lui"} 
{"name":"Mary", "age": 10}
{
    "name":"Alice",
    "age":14,
    "mother":{                //对象里包含对象
        "name":"Gwen",
        "age":40
    }
}
```
两个并列的数据之间用逗号（“,”）隔开，注意两点：
- 
使用英文的逗号（“,”），不要用中文的逗号（“，”）
- 
最后一个“名称/值对“之后不要加逗号
- 解析
不同的编程语言都提供了解析 JSON 字符串的方法，在这里主要讲解 JavaScript 中的解析方法。主要有三种：
- 
使用 eval()
- 
使用 JSON.parse()
- 
使用第三方库，例如 JQuery 等
## 序列化
- 
序列化，指将
 JavaScript 值转化为 JSON 字符串的过程。
- 
JSON.stringify()
 能够将 JavaScript 值转换成 JSON 字符串。JSON.stringify() 生成的字符串可以用 JSON.parse() 再还原成 JavaScript 值。
- 
