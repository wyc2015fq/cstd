
# xml中DTD属性的约束 - 李昆鹏的博客 - CSDN博客


2018年07月02日 10:58:22[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：297


---------------------------------------xml中DTD属性的约束---------------------------------------------
## DTD属性定义

### 1　属性定义的格式
Attribute List
<!ATTLIST 元素名
属性名1 属性类型 设置说明
属性名2 属性类型 设置说明
...>
<!ATTLISTstudent number CDATA \#REQUIRED>表示student元素的number为属性名，CDATA文本类型，这个属性是必须的。
最常见的属性类型：CDATA，表示文本类型；
最常见的设置说明1：\#REQUIRED，表示属性是必须的。
最常见的设置说明2：\#IMPLIED，表示属性是可选的。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT students  (student)+>
|<!ELEMENT  student (name,age,gender)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ELEMENT  gender (\#PCDATA)>
|<!ATTLIST  student stuno CDATA \#IMPLIED>
|]>
|<|students|>
|<|student|stuno|="|01|">
|<|name|></|name|>
|<|age|></|age|>
|<|gender|></|gender|>
|</|student|>
|</|students|>
|

### 2　属性类型
l  CDATA：属性值为任意文本数据；
l  Enumerated：属性值必须是枚举列表中的一个；
l  ID：属性值必须是唯一的，并且属性值不能以数字开头；

### 3　CDATA属性类型
CDATA，即Character Data（字符数据）。表示属性的类型为字符类型！
<!ATTLISTstudent number CDATA \#REQUIRED>表示student元素的number属性是字符数据类型，并且是必须属性。
<studentnumber=”czbk_1001”>

### 4　Enumerated属性类型
Enumerated不是关键字，定义枚举类型的属性需要给出枚举列表。当属性值为枚举类型时，那么这个属性的取值必须是枚举列表中的一个值。
<!ATTLISTstudent sex (male | female) \#IMPLIED> 表示student的sex属性取值必须是male或者是female。并且这个属性是可选的。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student)+>
|<!ELEMENT  student (name,age)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ATTLIST  student stuno CDATA \#IMPLIED>
|<!ATTLIST  student gender (|男||女) \#REQUIRED>
|]>
|<|students|>
|<|student|stuno|="|01|"|gender|="|男|"|>
|<|name|></|name|>
|<|age|></|age|>
|</|student|>
|</|students|>
|

### 5　ID属性类型
一个元素最多只能有一个ID 属性，ID属性用来表示元素唯一性的唯一标识。ID属性就相当与元素的身份证号，必须是唯一标识！
如果把student元素的number属性设定为ID类型，那么每个student元素的number属性值必须是唯一的，并且ID类型的属性值不能以数字开头。
<!ATTLISTstudent number ID \#REQUIRED> 表示student的number属性值是ID类型，这说明student元素的number属性值必须是唯一的，不能和其他student的number属性值相同。
<studentnumber=”czbk_1001”/>
<studentnumber=”czbk_1002”/>
**注意：不能以数字开头。**
如果<a>元素有一个ID属性a
如果<b>元素有一个ID属性b
<a a=”abc”/>
<b b=”abc”/>
上面也是错误的，因为ID属性的值是不可以相同的。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student)+>
|<!ELEMENT  student (name,age)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ATTLIST  student stuno ID \#REQUIRED>
|<!ATTLIST  student gender (|男||女) \#REQUIRED>
|]>
|<|students|>
|<|student|stuno|="|a_1|"|gender|="|男|"|>
|<|name|></|name|>
|<|age|></|age|>
|</|student|>
|<|student|stuno|="|a_2|"|gender|="|男|"|>
|<|name|></|name|>
|<|age|></|age|>
|</|student|>
|</|students|>
|

### 6　设置说明
l  \#REQUIRED：表示属性是必须的；
l  \#IMPLIED：表示属性是可选的，即这个属性可以不给出；

