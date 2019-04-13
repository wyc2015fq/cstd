
# xml中schema的complexType的用法 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:50:24[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：775


--------------------------------- xml中schema的complexType的用法----------------------------------------
### complexType
作用：定义一个复合类型，它决定了一组元素和属性值的约束和相关信息
属性：**name**
示例：
|<|xs:element|name="cat"  type="xs:string"/>
|<|xs:element|name="dog"  type="xs:string"/>
|<|xs:element|name="pets">
|<xs:complexType>
|<xs:sequence  minOccurs="0"  maxOccurs="unbounded">
|<|xs:element ref|="cat"/>
|<|xs:element ref|="dog"/>
|</xs:sequence>
|</xs:complexType>
|</xs:element>
|
**Sequence：**
|<|xs:element|name|="|student|">
|<!--|复杂数据类型，用来表示元素和元素的层级关系或属性关系|-->
|<|xs:complexType|>
|<!--|sequence|内部元素是有序的从上到下|-->
|<|xs:sequence|minOccurs|="|1|"|maxOccurs|="|3|">
|<|xs:element|name|="|name|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|gender|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|age|"|type|="|xs:int|"></|xs:element|>
|</|xs:sequence|>
|</|xs:complexType|>
|</|xs:element|>
|
All：无序的
|<?xml  version="1.0" encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">

|<|xs:element|name|="|student|">
|<!--|复杂数据类型，用来表示元素和元素的层级关系或属性关系|-->
|<|xs:complexType|>
|<!--all|内部元素是无序的，可以出现|0|或者|1|次|-->
|<|xs:all|minOccurs|="|0|"|maxOccurs|="|1|">
|<|xs:element|name|="|name|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|gender|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|age|"|type|="|xs:int|"></|xs:element|>
|</|xs:all|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:schema|>
|
Choice：
|<?xml  version="1.0" encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">
|<|xs:element|name|="|student|">
|<!--|复杂数据类型，用来表示元素和元素的层级关系或属性关系|-->
|<|xs:complexType|>
|<!--|choice|约束只能出现|choice|中的一个元素|-->
|<|xs:choice|minOccurs|="|1|"|maxOccurs|="|unbounded|">
|<|xs:element|name|="|name|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|gender|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|age|"|type|="|xs:int|"></|xs:element|>
|</|xs:choice|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:schema|>
|
![](https://img-blog.csdn.net/20180704074952263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### -------------------------------------------------------------
###### 使用scheme约束练习
|<?xml version="1.0"  encoding="GB2312"?>
|<|学生名册|>
|<|学生|学号|="1">
|<|姓名|>|张三|</|姓名|>
|<|性别|>|男|</|性别|>
|<|年龄|>20</|年龄|>
|</|学生|>
|<|学生|学号|="2">
|<|姓名|>|李四|</|姓名|>
|<|性别|>|女|</|性别|>
|<|年龄|>19</|年龄|>
|</|学生|>
|<|学生|学号|="3">
|<|姓名|>|王五|</|姓名|>
|<|性别|>|男|</|性别|>
|<|年龄|>21</|年龄|>
|</|学生|>
|</|学生名册|>
|
|<?xml  version="1.0" encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">
|<|xs:element|name|="|学生名册|">
|<|xs:complexType|>
|<|xs:sequence|minOccurs|="|1|"|maxOccurs|="|unbounded|">
|<|xs:element|name|="|学生|">
|<|xs:complexType|>
|<|xs:sequence|>
|<|xs:element|name|="|姓名|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|性别|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|年龄|"|type|="|xs:int|"></|xs:element|>
|</|xs:sequence|>
|<|xs:attribute|name|="|学号|"|type|="|xs:ID|"|use|="|required|"></|xs:attribute|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:sequence|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:schema|>
|


