
# YAML 与 front-matter - Zhang's Wikipedia - CSDN博客


2018年03月21日 22:18:26[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1303个人分类：[misc](https://blog.csdn.net/lanchunhui/article/category/6071672)



## 1. YAML
类似 Linux：Linux is not UniX，YAML：YAML ain’t markup language，是一种递归缩写，是一个可读性高并且容易被人类阅读，容易和脚本语言交互，用来表达资料序列的编程语言。
写法：
单行缩进：
house:family:{name:Doe, parents: [John, Jane], children: [Paul, Mark, Simone]}address:{number:34, street: Main Street, city: Nowheretown, zipcode:12345}
多行缩进：连续的项目通过减号“-”来表示，map 结构里面的key/value对用冒号“:”来分隔
house:
  family:
    name: Doe
    parents:
      -John-Janechildren:
      -Paul-Mark-Simoneaddress:
    number: 34
    street: Main Street
    city: Nowheretown
    zipcode: 12345
## 2. front-matter：置于文件头部的一些东东
Front-matter 是文件最上方以 — 分隔（三个连续短横）的区域，用于指定个别文件的变量，举例来说：
```python
title: Hello World
```
```python
date: 2013/7/13 20:46:25
---
```
## 3. Typora 下的 YAML front matter
typora-root-url：表示文中所插入引用的图片等页面元素所参考依据的路径（如下代码使用的是相对路径，此 md 文件所在的同级目录）：
---typora-root-url: images
---

