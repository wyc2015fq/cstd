# XSD使用总结（三） - fanyun的博客 - CSDN博客
2019年03月24日 22:02:29[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：24
## 一、复合元素介绍
### 1、什么是复合元素？
复合元素指包含其他元素及/或属性的 XML 元素。
有四种类型的复合元素：
- 空元素
- 包含其他元素的元素
- 仅包含文本的元素
- 包含元素和文本的元素
注释：上述元素均可包含属性！
### 2、如何定义复合元素？
类似属性定义和内容限定，定义复合元素同样有两种方式
1. 直接对"employee"元素进行声明，就像这样：
```
<xs:element name="employee">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="firstname" type="xs:string"/>
      <xs:element name="lastname" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
</xs:element>
```
假如您使用上面所描述的方法，那么仅有 "employee" 可使用所规定的复合类型。请注意其子元素，"firstname" 以及 "lastname"，被包围在指示器 <sequence>中。这意味着子元素必须以它们被声明的次序出现。
2. "employee" 元素可以使用 type 属性，这个属性的作用是引用要使用的复合类型的名称：
```
<xs:element name="employee" type="personinfo"/>
<xs:complexType name="personinfo">
  <xs:sequence>
    <xs:element name="firstname" type="xs:string"/>
    <xs:element name="lastname" type="xs:string"/>
  </xs:sequence>
</xs:complexType>
```
## 二、指示器
通过指示器，我们可以控制在文档中使用元素的方式。xsd有七种指示器：
Order 指示器：
- All
- Choice
- Sequence
Occurrence 指示器：
- maxOccurs
- minOccurs
Group 指示器：
- Group name
- attributeGroup name
### 1、Order 指示器
Order 指示器用于定义元素的顺序。
All 
<all> 指示器规定子元素可以按照任意顺序出现，且每个子元素必须只出现一次。 
Choice 
<choice> 指示器规定对应的XML可出现<choice>声明中的某个子元素；
实例：
```
<xs:element name="project">
        <xs:complexType>
            <xs:choice >
                <xs:element name="a"/>
                <xs:element name="b"/>
            </xs:choice>
        </xs:complexType>
    </xs:element>
```
那么此时对应的xml中project元素仅能包含<a/>或<b/>。 
Sequence 
<sequence> 规定子元素必须按照特定的顺序出现，默认每个仅能出现一次。
### 2、Occurrence 指示器
Occurrence 指示器用于定义某个元素出现的频率。
注释：对于所有的 "Order" 和 "Group" 指示器（any、all、choice、sequence、group name 以及 group reference），其中的 maxOccurs 以及 minOccurs 的默认值均为 1。
maxOccurs 
<maxOccurs> 指示器可规定某个元素可出现的最大次数。 
minOccurs 
<minOccurs> 指示器可规定某个元素能够出现的最小次数。
提示：如需使某个元素的出现次数不受限制，请使用 maxOccurs="unbounded" 这个声明：
### 3、Group 指示器
元素组
实例：
```
<xs:group name="persongroup">
  <xs:sequence>
    <xs:element name="firstname" type="xs:string"/>
    <xs:element name="lastname" type="xs:string"/>
    <xs:element name="birthday" type="xs:date"/>
  </xs:sequence>
</xs:group>
<xs:element name="person" type="personinfo"/>
<xs:complexType name="personinfo">
  <xs:sequence>
    <xs:group ref="persongroup"/>
    <xs:element name="country" type="xs:string"/>
  </xs:sequence>
</xs:complexType>
```
属性组
属性组通过 attributeGroup 声明来进行定义：
```
<xs:attributeGroup name="personattrgroup">
  <xs:attribute name="firstname" type="xs:string"/>
  <xs:attribute name="lastname" type="xs:string"/>
  <xs:attribute name="birthday" type="xs:date"/>
</xs:attributeGroup>
<xs:element name="person">
  <xs:complexType>
    <xs:attributeGroup ref="personattrgroup"/>
  </xs:complexType>
</xs:element>
```
## 三、复合元素
### 1、空元素
空的复合元素不能包含内容，只能含有属性。
一个空的 XML 元素：
`<product prodid="1345" />`
 对应的XSD
```
<xs:element name="product">
  <xs:complexType>
    <xs:attribute name="prodid" type="xs:positiveInteger"/>
  </xs:complexType>
</xs:element>
```
### 2、仅含元素
XML 元素，"person"，仅包含其他的元素：
```
<person>
<firstname>John</firstname>
<lastname>Smith</lastname>
</person>
```
您可在 schema 中这样定义 "person" 元素：
```
<xs:element name="person">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="firstname" type="xs:string"/>
      <xs:element name="lastname" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>
</xs:element>
```
### 3、仅含文本
仅含文本的复合元素可包含文本和属性。
这里有一个 XML 元素的例子，"shoesize"，其中仅包含文本：
`<shoesize country="france">35</shoesize>`
下面这个例子声明了一个复合类型，其内容被定义为整数值，并且 "shoesize" 元素含有名为 "country" 的属性：
```
<xs:element name="shoesize">
  <xs:complexType>
    <xs:simpleContent>
      <xs:extension base="xs:integer">
        <xs:attribute name="country" type="xs:string" />
      </xs:extension>
    </xs:simpleContent>
  </xs:complexType>
</xs:element>
```
### 4、混合内容
混合的复合类型可包含属性、元素以及文本。
XML 元素，"letter"，含有文本以及其他元素：
```
<letter>
Dear Mr.<name>John Smith</name>.
Your order <orderid>1032</orderid>
will be shipped on <shipdate>2001-07-13</shipdate>.
</letter>
```
下面这个 schema 声明了这个 "letter" 元素：
```
<xs:element name="letter">
  <xs:complexType mixed="true">
    <xs:sequence>
      <xs:element name="name" type="xs:string"/>
      <xs:element name="orderid" type="xs:positiveInteger"/>
      <xs:element name="shipdate" type="xs:date"/>
    </xs:sequence>
  </xs:complexType>
</xs:element>
```
## 总结
定义复合元素的时候，刚接触到人可能会对元素（extension、simpleContent、complexContent、simpleType、complexType）的使用产生混淆，这里简要归纳一下希望对大家有帮助：
使用extension扩展的功能都可以在complexType内完成，但是当我们可能或需要在不同的地方对同一块复杂或者简单的类型（complexType or simpleType）进行重复引用时，我们可以把需要重用的代码块单独出来然后使用extension进行引用。
extension的父元素是simpleContent或者complexContent，当我们扩展的是简单类型时，比如属性、文本，应使用simpleContent，而当扩展的是复杂类型时，比如元素，则应使用complexContent。
