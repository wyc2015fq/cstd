# XML中DTD,XSD的区别与应用 - z69183787的专栏 - CSDN博客
2014年08月14日 20:15:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1234
    XML我们并不陌生，在企业级应用中有很广的用途。具体就不再说，下面介绍一下DTD,XSD的区别并以XSD为例看spring中定义与使用。
1.DTD(Documnet Type Definition)
DTD即文档类型定义，是一种XML约束模式语言，是XML文件的验证机制,属于XML文件组成的一部分。
DTD 是一种保证XML文档格式正确的有效方法，可以通过比较XML文档和DTD文件来看文档是否符合规范，元素和标签使用是否正确。 一个 DTD文档包含：元素的定义规则，元素间关系的定义规则，元素可使用的属性，可使用的实体或符号规则。
DTD和XSD相比：DTD 是使用非 XML 语法编写的。
DTD 不可扩展,不支持命名空间,只提供非常有限的数据类型 .
2.XSD(XML Schemas Definition) 
XML Schema语言也就是XSD。XML Schema描述了XML文档的结构。 
可以用一个指定的XML Schema来验证某个XML文档，以检查该XML文档是否符合其要求。文档设计者可以通过XML Schema指定一个XML文档所允许的结构和内容，并可据此检查一个XML文档是否是有效的。XML Schema本身是一个XML文档，它符合XML语法结构。可以用通用的XML解析器解析它。
一个XML Schema会定义：文档中出现的元素、文档中出现的属性、子元素、子元素的数量、子元素的顺序、元素是否为空、元素和属性的数据类型、元素或属性的默认 和固定值。
XSD是DTD替代者的原因，一是据将来的条件可扩展，二是比DTD丰富和有用，三是用XML书写，四是支持数据类型，五是支持命名空间。
XML Schema的优点:
1) XML Schema基于XML,没有专门的语法 
2) XML Schema可以象其他XML文件一样解析和处理 
3) XML Schema比DTD提供了更丰富的数据类型.
4) XML Schema提供可扩充的数据模型。 
5) XML Schema支持综合命名空间 
6) XML Schema支持属性组。
3.spring中spring-beans-2.5.xsd
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8" standalone="no"?>  
-     <xsd:schema xmlns="http://www.springframework.org/schema/beans"
-             xmlns:xsd="http://www.w3.org/2001/XMLSchema"
-             targetNamespace="http://www.springframework.org/schema/beans">  
- 
-         <!-- base types -->  
-         <xsd:complexType name="identifiedType"abstract="true">         
-             <xsd:attribute name="id" type="xsd:ID">  
-             </xsd:attribute>  
-         </xsd:complexType>  
- 
-     </xsd:schema>  
(1)schema元素为所有XML Schema模式文档的根元素，它用于声明该XML文档是一个XML Schema文档。该模式文档本身也一个XML文档，自己的名称空间为xmlns:xsd="http://www.w3.org/2001/XMLSchema"，其中xsd为其前缀。
(2)Schema中targetNamespace定义了Schema定义出的新元素与属性的名称空间。也就是它是对所定义的XML模型进行限制：限制定义出的元素，或者属性的名称空间.
4.Spring中配置文件对XSD文件的使用
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <beans xmlns="http://www.springframework.org/schema/beans"
-         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-         xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd>
</beans>
在使用XML Schema 文档对XML实例文档进行检验，除了要声明名称空间外(xmlns="http://www.springframework.org/schema/beans")，还必须指定该名称空间所对应的XML Schema文档的存储位置.
通过schemaLocation属性来指定名称空间所对应的XML Schema文档的存储位置，它包含两个部分,一部分是名称空间的URI,第二部分就是该名称空间所标识的XML Schema文件位置或URL地址
（xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd）
