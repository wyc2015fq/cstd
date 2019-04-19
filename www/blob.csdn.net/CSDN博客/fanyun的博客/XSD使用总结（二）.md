# XSD使用总结（二） - fanyun的博客 - CSDN博客
2019年03月24日 22:02:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：11
## 一、XSD 简易元素
XML Schema 可定义 XML 文件的元素。
简易元素指那些只包含文本的元素。它不会包含任何其他的元素或属性。
XSD元素：
![](https://img-blog.csdnimg.cn/20190324195507532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
## XSD Restrictions/Facets for Datatypes
![](https://img-blog.csdnimg.cn/20190324195555119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
### 1、什么是简易元素？
简易元素指那些仅包含文本的元素。它不会包含任何其他的元素或属性。
不过，“仅包含文本”这个限定却很容易造成误解。文本有很多类型。它可以是 XML Schema 定义中包括的类型中的一种（布尔、字符串、数据等等），或者它也可以是您自行定义的定制类型。
### 2、定义简易元素
定义简易元素的语法：
`<xs:element name="xxx" type="yyy"/>`
 此处 xxx 指元素的名称，yyy 指元素的数据类型。XML Schema 拥有很多内建的数据类型。
最常用的类型是：
- xs:string
- xs:decimal
- xs:integer
- xs:boolean
- xs:date
- xs:time
例子：
这是一些 XML 元素：
```
<lastname>Smith</lastname>
<age>28</age>
<dateborn>1980-03-27</dateborn>
```
 这是相应的简易元素定义：
```
<xs:element name="lastname" type="xs:string"/>
<xs:element name="age" type="xs:integer"/>
<xs:element name="dateborn" type="xs:date"/>
```
### 3、简易元素的默认值和固定值
简易元素可拥有指定的默认值或固定值。
当没有其他的值被规定时，默认值就会自动分配给元素。
在下面的例子中，缺省值是 "red"：
`<xs:element name="color" type="xs:string" default="red"/>`
 固定值同样会自动分配给元素，并且您无法规定另外一个值。
在下面的例子中，固定值是 "red"：
```
<xs:element name="color" type="xs:string" fixed="red"/>
```
## 二、XSD 属性
 所有的属性均作为简易类型来声明。 
### 1、什么是属性？
简易元素无法拥有属性。假如某个元素拥有属性，它就会被当作某种复合类型，复合元素将在下节中进行介绍。但是属性本身总是作为简易类型被声明的。
### 2、如何声明属性？
定义属性的语法是：
`<xs:attribute name="xxx" type="yyy"/>`
 在此处，xxx 指属性名称，yyy 则规定属性的数据类型。XML Schema 拥有很多内建的数据类型。
 实例
这是带有属性的 XML 元素：
`<lastname lang="EN">Smith</lastname>`
这是对应的属性定义：
```
<xs:attribute name="lang" type="xs:string"/>
```
### 3、属性的默认值和固定值
属性可拥有指定的默认值或固定值。
当没有其他的值被规定时，默认值就会自动分配给元素。
在下面的例子中，缺省值是 "EN"：
`<xs:attribute name="lang" type="xs:string" default="EN"/>`
固定值同样会自动分配给元素，并且您无法规定另外的值。
在下面的例子中，固定值是 "EN"：
```
<xs:attribute name="lang" type="xs:string" fixed="EN"/>
```
### 4、可选的和必需的属性
在缺省的情况下，属性是可选的。如需规定属性为必选，请使用 "use" 属性：
```
<xs:attribute name="lang" type="xs:string" use="required"/>
```
### 5、内建数据类型
因为属性本身是作为简易类型被声明的，所以属性的内建数据类型根元素是simpleType。
具体使用可以在[参考手册](http://www.w3school.com.cn/schema/schema_elements_ref.asp)中查阅，就不一一细说。
实例：
```
<xs:element name="person">
        <xs:complexType>
            <xs:attribute type="t_sex" name="sex"/>
        </xs:complexType>
    </xs:element>
    
    <xs:simpleType name="t_sex">
        <xs:restriction>
            <xs:pattern value="man|woman"/>
        </xs:restriction>
    </xs:simpleType>
```
## 三、对内容的限定
限定（restriction）用于为 XML 元素或者属性定义可接受的值。对 XML 元素的限定被称为 facet。
假如 XML 元素的类型是 "xs:date"，而其包含的内容是类似 "Hello World" 的字符串，元素将不会（通过）验证。 
### 1、数据类型的限定
|限定|描述|
|----|----|
|enumeration|定义可接受值的一个列表```<xs:enumeration value="a"/><xs:enumeration value="b"/><xs:enumeration value="c"/>```|
|fractionDigits|定义所允许的最大的小数位数。必须大于等于0。```<xs:restriction base="xs:decimal">    <xs:fractionDigits value="2"/></xs:restriction>```|
|length|定义所允许的字符或者列表项目的精确数目。必须大于或等于0。`<xs:length value="8"/>`|
|maxExclusive|定义数值的上限。所允许的值必须小于此值。`<xs:maxExclusive value="8"/>`|
|maxInclusive|定义数值的上限。所允许的值必须小于或等于此值。|
|maxLength|定义所允许的字符或者列表项目的最大数目。必须大于或等于0。|
|minExclusive|定义数值的下限。所允许的值必需大于此值。|
|minInclusive|定义数值的下限。所允许的值必需大于或等于此值。|
|minLength|定义所允许的字符或者列表项目的最小数目。必须大于或等于0。|
|pattern|定义可接受的字符的精确序列。类似正则表达式`<xs:pattern value="[a-zA-Z0-9]{8}"/>`|
|totalDigits|定义所允许的阿拉伯数字的精确位数。必须大于0。|
|whiteSpace|定义空白字符（换行、回车、空格以及制表符）的处理方式。`preserve - 不会移除任何空白字符``replace - 移除所有空白字符（换行、回车、空格以及制表符）``collapse - 移除所有空白字符（换行、回车、空格以及制表符会被替换为空格，开头和结尾的空格会被移除，而多个连续的空格会被缩减为一个单一的空格）``<xs:whiteSpace value="preserve"/>`|
### 2、两种不同的写法
下面的例子定义了带有一个限定的名为 "car" 的元素。可接受的值只有：Audi, Golf, BMW：
```
<xs:element name="car">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:enumeration value="Audi"/>
    <xs:enumeration value="Golf"/>
    <xs:enumeration value="BMW"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 上面的例子也可以被写为：
<xs:element name="car" type="carType"/> <xs:simpleType name="carType"> <xs:restriction base="xs:string"> <xs:enumeration value="Audi"/> <xs:enumeration value="Golf"/> <xs:enumeration value="BMW"/> </xs:restriction> </xs:simpleType>
 注释：在这种情况下，类型 "carType" 可被其他元素使用，因为它不是 "car" 元素的组成部分。
### 3、部分类型的限定示例
### 对值的限定
下面的例子定义了带有一个限定且名为 "age" 的元素。age 的值不能低于 0 或者高于 120：
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
### 对一组值的限定
如需把 XML 元素的内容限制为一组可接受的值，我们要使用枚举约束（enumeration constraint）。
下面的例子定义了带有一个限定的名为 "car" 的元素。可接受的值只有：Audi, Golf, BMW：
```
<xs:element name="car">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:enumeration value="Audi"/>
    <xs:enumeration value="Golf"/>
    <xs:enumeration value="BMW"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
### 对一系列值的限定
如需把 XML 元素的内容限制定义为一系列可使用的数字或字母，我们要使用模式约束（pattern constraint）。
下面的例子定义了带有一个限定的名为 "letter" 的元素。可接受的值只有小写字母 a - z 其中的一个：
```
<xs:element name="letter">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="[a-z]"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
下一个例子定义了带有一个限定的名为 "initials" 的元素。可接受的值是大写字母 A - Z 其中的三个：
```
<xs:element name="initials">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="[A-Z][A-Z][A-Z]"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
下一个例子也定义了带有一个限定的名为 "initials" 的元素。可接受的值是大写或小写字母 a - z 其中的三个：
```
<xs:element name="initials">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="[a-zA-Z][a-zA-Z][a-zA-Z]"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 下一个例子定义了带有一个限定的名为 "choice 的元素。可接受的值是字母 x, y 或 z 中的一个：
```
<xs:element name="choice">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="[xyz]"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 下一个例子定义了带有一个限定的名为 "prodid" 的元素。可接受的值是五个阿拉伯数字的一个序列，且每个数字的范围是 0-9：
```
<xs:element name="prodid">
<xs:simpleType>
  <xs:restriction base="xs:integer">
    <xs:pattern value="[0-9][0-9][0-9][0-9][0-9]"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
### 对一系列值的其他限定
下面的例子定义了带有一个限定的名为 "letter" 的元素。可接受的值是 a - z 中零个或多个字母：
```
<xs:element name="letter">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="([a-z])*"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 下面的例子定义了带有一个限定的名为 "letter" 的元素。可接受的值是一对或多对字母，每对字母由一个小写字母后跟一个大写字母组成。举个例子，"sToP"将会通过这种模式的验证，但是 "Stop"、"STOP" 或者 "stop" 无法通过验证：
```
<xs:element name="letter">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="([a-z][A-Z])+"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 下面的例子定义了带有一个限定的名为 "gender" 的元素。可接受的值是 male 或者 female：
```
<xs:element name="gender">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="male|female"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
 下面的例子定义了带有一个限定的名为 "password" 的元素。可接受的值是由 8 个字符组成的一行字符，这些字符必须是大写或小写字母 a - z 亦或数字 0 - 9：
```
<xs:element name="password">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:pattern value="[a-zA-Z0-9]{8}"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
### 对空白字符的限定
如需规定对空白字符（whitespace characters）的处理方式，我们需要使用 whiteSpace 限定。
下面的例子定义了带有一个限定的名为 "address" 的元素。这个 whiteSpace 限定被设置为 "preserve"，这意味着 XML 处理器不会移除任何空白字符：
```
<xs:element name="address">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:whiteSpace value="preserve"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
这个例子也定义了带有一个限定的名为 "address" 的元素。这个 whiteSpace 限定被设置为 "replace"，这意味着 XML 处理器将移除所有空白字符（换行、回车、空格以及制表符）：
```
<xs:element name="address">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:whiteSpace value="replace"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
这个例子也定义了带有一个限定的名为 "address" 的元素。这个 whiteSpace 限定被设置为 "collapse"，这意味着 XML 处理器将移除所有空白字符（换行、回车、空格以及制表符会被替换为空格，开头和结尾的空格会被移除，而多个连续的空格会被缩减为一个单一的空格）：
```
<xs:element name="address">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:whiteSpace value="collapse"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
### 对长度的限定
如需限制元素中值的长度，我们需要使用 length、maxLength 以及 minLength 限定。
本例定义了带有一个限定且名为 "password" 的元素。其值必须精确到 8 个字符：
```
<xs:element name="password">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:length value="8"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
这个例子也定义了带有一个限定的名为 "password" 的元素。其值最小为 5 个字符，最大为 8 个字符：
```
<xs:element name="password">
<xs:simpleType>
  <xs:restriction base="xs:string">
    <xs:minLength value="5"/>
    <xs:maxLength value="8"/>
  </xs:restriction>
</xs:simpleType>
</xs:element>
```
