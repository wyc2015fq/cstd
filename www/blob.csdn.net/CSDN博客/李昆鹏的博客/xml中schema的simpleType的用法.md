
# xml中schema的simpleType的用法 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:47:51[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：259


-----------------------------------------------xml中schema的simpleType的用法-------------------------------------------------
### simpleType
作用：定义一个简单类型，它决定了元素和属性值的约束和相关信息
属性：**name ：自定义属性的类型**
内容：应用已经存在的简单类型，三种方式：
**restrict**→限定一个范围
**list**→从列表中选择
|<?xml  version="1.0" encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">
|<|xs:element|name|="|student|">
|<|xs:complexType|>
|<|xs:sequence|>
|<|xs:element|name|="|name|"|type|="|xs:string|"></|xs:element|>
|<|xs:element|name|="|age|"|type|="|xs:int|"></|xs:element|>
|<|xs:element|name|="|gender|"|type|="|genderst|"></|xs:element|>
|<|xs:element|name|="|favor|"|type|="|favorst|"></|xs:element|>
|</|xs:sequence|>
|<|xs:attributeGroup|ref|="|studattr|"></|xs:attributeGroup|>
|</|xs:complexType|>
|</|xs:element|>
|<|xs:attributeGroup|name|="|studattr|">
|<|xs:attribute|name|="|studno|"|type|="|xs:int|"|use|="|required|"></|xs:attribute|>
|<|xs:attribute|name|="|score|"|type|="|scorest|"|use|="|required|"></|xs:attribute|>
|</|xs:attributeGroup|>
|<!--|0|到|100|的数值约束|-->
|<|xs:simpleType|name|="|scorest|">
|<|xs:restriction|base|="|xs:int|">
|<|xs:minInclusive|value|="|0|"></|xs:minInclusive|>
|<|xs:maxInclusive|value|="|100|"></|xs:maxInclusive|>
|</|xs:restriction|>
|</|xs:simpleType|>
|<!--|给性别设置枚举类型的约束，只能填写男或者女|-->
|<|xs:simpleType|name|="|genderst|">
|<|xs:restriction|base|="|xs:string|">
|<|xs:enumeration|value|="|男|"></|xs:enumeration|>
|<|xs:enumeration|value|="|女|"></|xs:enumeration|>
|</|xs:restriction|>
|</|xs:simpleType|>
|<!--|目录约束，favor|元素中只能填写字符串目录，并且用空格分割开来|-->
|<|xs:simpleType|name|="|favorst|">
|<|xs:list|itemType|="|xs:string|"></|xs:list|>
|</|xs:simpleType|>
|</|xs:schema|>
|
|<?xml version="1.0"  encoding="UTF-8"?>
|<|student|studentNo|="|1|"|score|="|100|"|xmlns:xsi|="|http://www.w3.org/2001/XMLSchema-instance|"|xsi:noNamespaceSchemaLocation|="|demo5.xsd|">
|<|name|>|张三|</|name|>
|<|age|>|20|</|age|>
|<|gender|>|男|</|gender|>
|<|favor|>|足球 男球|</|favor|>
|</|student|>
|
![](https://img-blog.csdn.net/20180704074738528?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


