# webservice初次相识之XML限制文件DTD和Schema - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年04月25日 23:02:37[boonya](https://me.csdn.net/boonya)阅读数：2510








前言：这里备注一些webservice学习之前的基础，讲解编写XML文件的两种限制文件DTD和Schema。

第一部分：DTD

1、DTD的作用：
 定义一个 XML 文档的合法元素，为 XML 文档声明合法的元素、属性、实体以及 CDATA 部分。
 2、编写DTD与XML的分离
 （1）、编写DTD文件human.dtd

```
<?xml version="1.0" encoding="UTF-8"?>
  <!ELEMENT human (kind)>
  <!ELEMENT kind (area+)>
  <!ELEMENT area (name,spid)>
  <!ELEMENT name (#PCDATA)>
  <!ELEMENT spid (#PCDATA)>
```
（2）、根据DTD编写XML文件
 在XML内部引用一个外部声明 的DTD

```
<?xml version="1.0" encoding="UTF-8"?>
 <!DOCTYPE human SYSTEM "human.dtd">
 <human>
     <kind>
        <area>
          <name>BLACK</name>
          <spid>AFRICA</spid>
        </area>
        <area>
          <name>WHITE</name>
          <spid>AMERICA</spid>
        </area>
     </kind>
 </human>
```
3、Schema和DTD的关系和区别
 关系：
 XML Schema 用于定义 XML 文档的合法元素，类似 DTD。
 XML Schema 是基于 XML 的 DTD 替代物。
 区别：

 与 DTD 不同，XML Schema 支持数据类型和命名空间。

第二部分：SChema

1、什么是 XML Schema：
  XML Schema 的作用是定义 XML 文档的合法构建模块，类似 DTD。（--W3CSchool）
2、学习背景：
  了解了：HTML / XHTML ，  XML 以及 XML 命名空间 和 DTD
  -----解决问题：
  通过一种更加合理的基于XML语法的方式来编写XML的限制文件。
  使用命名空间支持多个名称相同的元素（防止元素冲突）。
  schema可以很好地完成对java或者所有对象的修饰并且提供了大量的数据类型；
3、XML schema的特性：
  XML Schema 的重要特性是，它们由 XML 编写
  XML Schema 是可扩展的，因为它们由 XML 编写
  XML Schema 最重要的能力之一就是对数据类型的支持 
4、XML Schema可以做什么：
  定义可出现在文档中的元素 
  定义可出现在文档中的属性 
  定义哪个元素是子元素 
  定义子元素的次序 
  定义子元素的数目 
  定义元素是否为空，或者是否可包含文本 
  定义元素和属性的数据类型 
  定义元素和属性的默认值以及固定值 
  --->它应用于XML文件,schema是继承自DTD的，所以上面的限制有些是同DTD一致的，
  而Schema解除了DTD的不足之处（如：重名），并解决了数据类型（提供大量数据类型）。
5、XML Schema 可保护数据通信
  当数据从发送方被发送到接受方时，其要点是双方应有关于内容的相同的“期望值”。
  通过 XML Schema，发送方可以用一种接受方能够明白的方式来描述数据。
  一种数据，比如 "03-11-2004"，在某些国家被解释为11月3日，而在另一些国家为当作3月11日。
  但是一个带有数据类型的 XML 元素，比如：<date type="date">2004-03-11</date>，可确保
  对内容一致的理解，这是因为 XML 的数据类型 "date" 要求的格式是 "YYYY-MM-DD"。（--W3CSchool）
 --->注意：Schema会自动检测数据的格式。
6、编写一个形式良好的XML的条件：
 必须以 XML 声明开头 
 必须拥有唯一的根元素 
 开始标签必须与结束标签相匹配 
 元素对大小写敏感 
 所有的元素都必须关闭 
 所有的元素都必须正确地嵌套 
 必须对特殊字符使用实体 
  --->注意：即使文档的形式良好，仍然不能保证它们不会包含错误，并且这些错误可能会产生严重的后果
7、一个XML Schema的xsd文件,note.xsd：


```
<?xml version="1.0"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
      targetNamespace="http://www.w3school.com.cn"
      xmlns="http://www.w3school.com.cn"
      elementFormDefault="qualified">
 <xs:element name="note">
    <xs:complexType>
      <xs:sequence>
<xs:element name="to" type="xs:string"/>
<xs:element name="from" type="xs:string"/>
<xs:element name="heading" type="xs:string"/>
<xs:element name="body" type="xs:string"/>
      </xs:sequence>
    </xs:complexType>
 </xs:element>
</xs:schema>
```

8、引用xsd文件：


```
<?xml version="1.0"?>
<note
  xmlns="http://www.w3school.com.cn"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 <!--引用xsd文件 -->
  xsi:schemaLocation="http://www.w3school.com.cn note.xsd">
  <to>George</to>
  <from>John</from>
  <heading>Reminder</heading>
  <body>Don't forget the meeting!</body>
</note>
```

9、完善xsd文件：
 创建简单元素：<xs:element name="xxx" type="yyy"/>
 声明属性：<xs:attribute name="xxx" type="yyy"/>
 常用数据类型：xs:string ，xs:decimal， xs:integer， xs:boolean， xs:date ，xs:time 
 定义数据限制：
 enumeration 定义可接受值的一个列表 
 fractionDigits 定义所允许的最大的小数位数。必须大于等于0。 
 length 定义所允许的字符或者列表项目的精确数目。必须大于或等于0。 
 maxExclusive 定义数值的上限。所允许的值必须小于此值。 
 maxInclusive 定义数值的上限。所允许的值必须小于或等于此值。 
 maxLength 定义所允许的字符或者列表项目的最大数目。必须大于或等于0。 
 minExclusive 定义数值的下限。所允许的值必需大于此值。 
 minInclusive 定义数值的下限。所允许的值必需大于或等于此值。 
 minLength 定义所允许的字符或者列表项目的最小数目。必须大于或等于0。 
 pattern 定义可接受的字符的精确序列。 
 totalDigits 定义所允许的阿拉伯数字的精确位数。必须大于0。 
 whiteSpace 定义空白字符（换行、回车、空格以及制表符）的处理方式。 
10、数据限制实例：
枚举类型：


```
<xs:element name="gender">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="male|female"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```

数值范围：

```
<xs:element name="age">
<xs:simpleType>
  <xs:restriction base="xs:integer">
    <xs:minInclusive value="0"/>
    <xs:maxInclusive value="120"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```

11、命名空间：
 如上10中的实例：xs就是命名空间。
 定义命名空间用xmlns:xs，xmlns为默认命名空间。




