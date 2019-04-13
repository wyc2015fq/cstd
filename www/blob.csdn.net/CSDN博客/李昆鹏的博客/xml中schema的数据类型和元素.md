
# xml中schema的数据类型和元素 - 李昆鹏的博客 - CSDN博客


2018年07月02日 11:03:06[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：285


----------------------------------xml中schema的数据类型和元素--------------------------------------
## scheme的数据类型
Ø**简单类型**
内置的数据类型（**built-indata types**）
•       基本的数据类型
![](https://img-blog.csdn.net/20180702110001108?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
•       扩展的数据类型
![](https://img-blog.csdn.net/20180702110008381?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
用户自定义数据类型（通过**simpleType**定义）
数据类型的特性
![](https://img-blog.csdn.net/20180702110015932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø**复杂类型（通过complexType定义）**
**----------------------------------------------------------------------------------**
## .
## 元素
### 1.Schema
作用：包含已经定义的schema
用法：<xs:schema>：xs是命名空间的别名，也可以自定义，但是建议使用默认的。
属性：
xmlns：约束当前xml的命名空间（必有的）
targetNamespace
**只要是xml****的schema方式约束都以schema作为根结点**
**带有命名空间的schema**
|<?xml  version="1.0" encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|"
|targetNamespace|="|http://txjava.cn|"
|>
|<|xs:element|name|="|book|">
|<|xs:complexType|>
|<|xs:sequence|>
|<|xs:element|name|="|bookName|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|author|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|price|"|type|="|xs:float|"></|xs:element|>
|</|xs:sequence|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:schema|>
|
Xml的实例：
|<?xml  version="1.0" encoding="UTF-8"?>
|<|book|xmlns|="|http://txjava.cn|"|xmlns:xsi|="|http://www.w3.org/2001/XMLSchema-instance|"|xsi:schemaLocation|="|http://txjava.cn demo2.xsd|">
|<|bookName|>|神雕侠侣|</|bookName|>
|<|author|>|金庸|</|author|>
|<|price|>|25.4|</|price|>
|</|book|>
|
xsi:schemaLocation：引用的命名空间+“”约束文件的全地址
语法：
<根结点xmlns=”要引入的schema的命名空间”
xmlns:xsi=“http://www.w3.org/2001/XMLSchema-instance”
xsi:schemaLocation=”命名空间 约束文件的位置” >
|<beans xmlns="http://www.springframework.org/schema/beans"
|xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
|xsi:schemaLocation="http://www.springframework.org/schema/beans
|http://www.springframework.org/schema/beans/spring-beans-3.2.xsd">
|
在eclipse中加载schema约束的步骤
![](https://img-blog.csdn.net/20180702110205802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018070211022621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


