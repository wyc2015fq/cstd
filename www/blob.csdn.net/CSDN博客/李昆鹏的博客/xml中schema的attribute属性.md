
# xml中schema的attribute属性 - 李昆鹏的博客 - CSDN博客


2018年07月02日 11:05:40[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：720


-------------------------------------- xml中schema的attribute属性----------------------------------------
### Attribute
作用：声明一个属性
属性：
Name：属性名
Type：属性类型
ref：外部属性的引用
use：是否是必填
示例：
![](https://img-blog.csdn.net/20180702110447621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
单属性定义：
|<xs:element  name="student">
|<xs:complexType>
|<xs:sequence>
|<xs:element  name="name" type="xs:string"></xs:element>
|<xs:element  name="age" type="xs:int"></xs:element>
|<xs:element  name="gender" type="xs:string"></xs:element>
|</xs:sequence>
|<xs:attribute  name="studno" type="xs:int"  use="required"></xs:attribute>
|</xs:complexType>
|</xs:element>
|-->
|<!--
|<xs:element  name="student">
|<xs:complexType>
|<xs:sequence>
|<xs:element  name="name" type="xs:string"></xs:element>
|<xs:element  name="age" type="xs:int"></xs:element>
|<xs:element  name="gender" type="xs:string"></xs:element>
|</xs:sequence>
|<xs:attribute   ref="studno"></xs:attribute>
|</xs:complexType>
|</xs:element>
|<xs:attribute name="studno"  type="xs:int"></xs:attribute>
|
attributeGroup：定义一组属性。
|<|xs:element|name|="|student|">
|<|xs:complexType|>
|<|xs:sequence|>
|<|xs:element|name|="|name|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|age|"|type|="|xs:int|"></|xs:element|>
|<|xs:element|name|="|gender|"|type|="|xs:string|"></|xs:element|>
|</|xs:sequence|>
|<|xs:attributeGroup|ref|="|studattr|"></|xs:attributeGroup|>
|</|xs:complexType|>
|</|xs:element|>
|<|xs:attributeGroup|name|="|studattr|">
|<|xs:attribute|name|="|studno|"|type|="|xs:int|"|use|="|required|"></|xs:attribute|>
|<|xs:attribute|name|="|score|"|type|="|xs:int|"|use|="|required|"></|xs:attribute|>
|</|xs:attributeGroup|>
|
|<?xml version="1.0"  encoding="UTF-8"?>
|<|student|studentNo|="|1|"|score|="|88|"|xmlns:xsi|="|http://www.w3.org/2001/XMLSchema-instance|"|xsi:noNamespaceSchemaLocation|="|demo3.xsd|">
|<|name|>|张三|</|name|>
|<|age|>|20|</|age|>
|<|gender|>|男|</|gender|>
|</|student|>
|


