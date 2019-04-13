
# xml中schema的概述 - 李昆鹏的博客 - CSDN博客


2018年07月02日 10:59:18[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：57


-------------------------------------- xml中schema的概述----------------------------------------
## Schema概述
我们学习Schema的第一目标是：参照Schema的要求可以编写XML文档；
第二目标是：可以自己来定义Schema文档。
## 2　为什么要用Schema
Ø  DTD 的局限性
DTD不遵守XML语法（写XML文档实例时候用一种语法，写DTD的时候用另外一种语法）
DTD数据类型有限（与数据库数据类型不一致）
DTD不可扩展
DTD不支持命名空间（命名冲突）
Ø  Schema的新特性
Schema基于XML语法
Schema可以用能处理XML文档的工具处理
Schema大大扩充了数据类型，可以自定义数据类型
Schema支持元素的继承—Object-Oriented’
Schema支持属性组
## 3．scheme的文档结构
![](https://img-blog.csdn.net/20180702105857755?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">
|<|xs:element|name|="|book|"|type|="|bookType|"></|xs:element|>
|<|xs:complexType|name|="|bookType|">
|<|xs:sequence|>
|<|xs:element|name|="|bookName|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|author|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|price|"|type|="|xs:float|"></|xs:element|>
|</|xs:sequence|>
|</|xs:complexType|>
|</|xs:schema|>
|
|<?xml version="1.0" encoding="UTF-8"?>
|<|book|xmlns:xsi|="|http://www.w3.org/2001/XMLSchema-instance|"|xsi:noNamespaceSchemaLocation|="|demo1.xsd|">
|<|bookName|>|天龙八部|</|bookName|>
|<|author|>|金庸|</|author|>
|<|price|>|88.88|</|price|>
|</|book|>
|


