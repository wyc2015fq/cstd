
# xml中schema的element元素 - 李昆鹏的博客 - CSDN博客


2018年07月02日 11:07:48[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：156


---------------------------------------------xml中schema的element元素------------------------------------------------
### element
作用：声明一个元素
属性：
Name：元素的名称
Type：元素的类型
ref：引用已定义的元素
minOccurs ：至少出现的次数
maxOccurs ：最多出现的次数
fixed：元素的值固定成指定值
default：
|<|xs:element|name="cat"type="xs:string"/>
|<|xs:element|name="dog"type="xs:string"/>
|<|xs:element|name="pets">
|<xs:complexType>
|<xs:sequence  minOccurs="0"maxOccurs="unbounded">
|<|xs:element ref|="cat"/>
|<|xs:element ref|="dog"/>
|</xs:sequence>
|</xs:complexType>
|</xs:element>
|
例子：
|<?xml version="1.0"  encoding="UTF-8"?>
|<|xs:schema|xmlns:xs|="|http://www.w3.org/2001/XMLSchema|"|elementFormDefault|="|qualified|"|attributeFormDefault|="|unqualified|">
|<!--|内部创建子元素
|<xs:element  name="pets">
|<xs:complexType>
|<xs:sequence>
|<xs:element  name="dog" type="xs:string"></xs:element>
|<xs:element  name="cat" type="xs:string"></xs:element>
|</xs:sequence>
|</xs:complexType>
|</xs:element>
|-->
|<!--|外部引用子元素
|<xs:element  name="dog" type="xs:string"></xs:element>
|<xs:element  name="cat" type="xs:string"></xs:element>
|<xs:element  name="pets">
|<xs:complexType>
|<xs:sequence>
|<xs:element  ref="dog"></xs:element>
|<xs:element  ref="cat"></xs:element>
|</xs:sequence>
|</xs:complexType>
|</xs:element>
|-->
|<!--|设置子元素出现的次数，sequence|中的元素必须要成对出现，
|minOccurs|表示最小出现的次数，maxOccurs|表示最多出现的次数
|<xs:element  name="pets">
|<xs:complexType>
|<xs:sequence  minOccurs="1" maxOccurs="unbounded">
|<xs:element  name="dog" type="xs:string"></xs:element>
|<xs:element  name="cat" type="xs:string"></xs:element>
|</xs:sequence>
|</xs:complexType>
|</xs:element>
|-->
|<!--|fixed|给元素指定的值不能改变，default|给元素默认的值，可以改变|-->
|<|xs:element|name|="|pets|">
|<|xs:complexType|>
|<|xs:sequence|>
|<|xs:element|name|="|dog|"|type|="|xs:string|"|fixed|="|旺财|"></|xs:element|>
|<|xs:element|name|="|cat|"|type|="|xs:string|"|default|="|喵喵|"></|xs:element|>
|</|xs:sequence|>
|</|xs:complexType|>
|</|xs:element|>
|</|xs:schema|>
|
|<?xml version="1.0"  encoding="UTF-8"?>
|<|pets|xmlns:xsi|="|http://www.w3.org/2001/XMLSchema-instance|"|xsi:noNamespaceSchemaLocation|="|demo4.xsd|">
|<|dog|>|旺财|</|dog|>
|<|cat|>|咪咪|</|cat|>
|<|dog|>|旺财1|</|dog|>
|<|cat|>|咪咪2|</|cat|>
|</|pets|>
|


