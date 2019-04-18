# java dom4j操作xml - z69183787的专栏 - CSDN博客
2013年11月04日 21:59:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：870标签：[dom4j](https://so.csdn.net/so/search/s.do?q=dom4j&t=blog)
个人分类：[Dom4j																[Xml解析-Xpath](https://blog.csdn.net/z69183787/article/category/6525508)](https://blog.csdn.net/z69183787/article/category/2229189)
SAXReader reader = **new** SAXReader();    
Document doc = reader.read(...);    
List childNodes = doc.selectNodes("//Config/Child/ChildNode");    
**for**(Object obj:childNodes) {    
      Node childNode = (Node)obj;    
      String name = childNode.valueOf("@name");    
      String text = childNode.getText();    
}      
**一.Document 对象相关**
      1. 读取XML文件,获得document对象.   
SAXReader reader = **new** SAXReader();   
            Document   document = reader.read(**new** File("input.xml"));   
2.解析XML形式的文本,得 到document对象.   
            String text = "<members></members>";   
            Document document = DocumentHelper.parseText(text);   
3.主动创建document 对象.   
            Document document = DocumentHelper.createDocument();   
            Element root = document.addElement("members");// 创建根节点 
**二.节点相关**
1.获取文档的根节点.   
            Element rootElm = document.getRootElement();   
2.取得某节点的单个子节 点.   
Element memberElm=root.element("member");// "member"是节点名 
3.取得节点的文字   
String text=memberElm.getText(); 也可以用:   
String text=root.elementText("name");这个是取得根 节点下的name字节点的文字.   
4.取得某节点下名为"member"的所有字节点并 进行遍历.   
List nodes = rootElm.elements("member");   
**for** (Iterator it = nodes.iterator(); it.hasNext();) {   
                  Element elm = (Element) it.next();   
// do something 
            }   
5.对某节点下的所有子节点进 行遍历.   
**for**(Iterator it=root.elementIterator();it.hasNext();){   
                  Element element = (Element) it.next();   
// do something 
            }   
6.在某节点下添加子节 点.   
Element ageElm = newMemberElm.addElement("age");   
7.设置节点文字.   
            ageElm.setText("29");   
8.删除某节点.   
            parentElm.remove(childElm);// childElm是待删除的节点,parentElm是其父节 点 
9. 添加一个CDATA节点.   
     Element contentElm = infoElm.addElement("content");   
            contentElm.addCDATA(diary.getContent());  
contentElm.getText(); // 特别说明：获取节点的CDATA值与获取节点的值是一个方法  
contentElm.clearContent(); //清除节点中的内容，CDATA亦可  
**三. 属性相关.**
1.取得某节点下的某属性   
            Element root=document.getRootElement();       
            Attribute attribute=root.attribute("size");// 属性名name
2.取得属性的文字   
// 也可以用
            String text=attribute.getText();   
// 这个是取得根节点下name字节点的属性firstname的值:
            String text2=root.element("name").attributeValue("firstname");
3.遍历某节点的所有属 性   
            Element root=document.getRootElement();        
**for**(Iterator it=root.attributeIterator();it.hasNext();){    
                 Attribute attribute = (Attribute) it.next();    
                 String text=attribute.getText();    
                 System.out.println(text);    
            }
4.设置某节点的属性和文 字.   
            newMemberElm.addAttribute("name", "sitinspring");    
5.设置属性的文字   
           Attribute attribute=root.attribute("name");    
           attribute.setText("sitinspring");    
6.删除某属性    
           Attribute attribute=root.attribute("size");// 属性名name 
           root.remove(attribute);    
**四.将文档写入XML文件.**
1.文档中全为英文,不设置编 码,直接写入的形式.   
            XMLWriter writer = **new** XMLWriter(**new** FileWriter("output.xml"));    
            writer.write(document);    
            writer.close();    
2.文档中含有中文,设置编码 格式写入的形式.   
            OutputFormat format = OutputFormat.createPrettyPrint();    
            format.setEncoding("GBK");    // 指定XML编码         
            XMLWriter writer = **new** XMLWriter(**new** FileWriter("output.xml"),format);                    
            writer.write(document);    
            writer.close();    
**五.字符串与XML的转换    **1.将字符串转化为 XML   
            String text = "<members> <member>sitinspring</member> </members>";    
            Document document = DocumentHelper.parseText(text);    
2.将文档或节点的XML转化 为字符串.   
            SAXReader reader = **new** SAXReader();    
            Document   document = reader.read(**new** File("input.xml"));                
            Element root=document.getRootElement();                    
            String docXmlText=document.asXML();    
            String rootXmlText=root.asXML();    
            Element memberElm=root.element("member");    
            String memberXmlText=memberElm.asXML();      
      {    ** dom4j API 包含一个解析 XML 文档的工具。本文中将使用这个解析器创建一个示例 XML 文档。清单 **1** 显示了这个示例 XML 文档，catalog.xml。**   } 
      清单 1. 示例 XML 文档（catalog.xml）     
            <?xml version="1.0" encoding="UTF-8"?>     
            <catalog>     
                  <!--An XML Catalog-->     
                  <?target instruction?>    
                  <journal title="XML Zone" publisher="IBM developerWorks">     
                        <article level="Intermediate" date="December-2001">    
                               <title>Java configuration with XML Schema</title>     
                               <author>     
                                     <firstname>Marcello</firstname>     
                                     <lastname>Vitaletti</lastname>     
                               </author>    
                         </article>    
                  </journal>     
            </catalog>      
然 后使用同一个解析器修改 catalog.xml，清单 2 是修改后的 XML 文档，catalog-
 modified.xml。   
      清单 2. 修改后的 XML 文档
 （catalog-modified.xml）    
            <?xml version="1.0" encoding="UTF-8"?>     
            <catalog>     
                  <!--An XML catalog-->     
                  <?target instruction?>    
                  <journal title="XML Zone" publisher="IBM developerWorks">     
                        <article level="Introductory" date="October-2002">    
                              <title>Create flexible and extensible XML schemas</title>   
                              <author>     
                                    <firstname>Ayesha</firstname>     
                                    <lastname>Malik</lastname>     
                               </author>     
                        </article> 
                  </journal>
            </catalog>    
与 W3C DOM API 相
 比，使用 dom4j 所包含的解析器的好处是 dom4j 拥有本地的 XPath 支持。DOM 解析器不支持使用 XPath 选择节点。  
六.xpath语法
## 选取节点
XPath 使用路径表达式在 XML 文档中选取节点。节点是通过沿着路径或者 step 来选取的。
### 下 面列出了最有用的路径表达式：
|表达式|描述|
|----|----|
|nodename|选取此节点的所有子节点|
|/|从根节点选取|
|//|从匹配选择的当前节点选择文档中的节点，而不考虑它们的位置|
|.|选取当前节点|
|..|选取当前节点的父节点|
|@|选取属性|
### 实例
在下面 的表格中，我们已列出了一些路径表达式以及表达式的结果：
|路径表达式|结 果|
|----|----|
|bookstore|选取 bookstore 元素的所有子节点|
|/bookstore|选取根元素 bookstore注释：假如路径起始于正斜杠( / )，则此路径始终代表到某元素的绝对路径！|
|bookstore/book|选 取所有属于 bookstore 的子元素的 book 元素。|
|//book|选取所有 book 子元素，而不管它们在文档中的位置。|
|bookstore//book|选择所有 属于 bookstore 元素的后代的 book 元素，而不管它们位于 bookstore 之下的什么位置。|
|//@lang|选取所有名为 lang 的属性。|
## 谓语 （Predicates）
谓语用来查找某个特定的节点或者包含某个指定的值的节点。
谓语被嵌在方括号中。
### 实 例
在下面的表格中，我们列出了带有谓语的一些路径表达式，以及表达式的结果：
|路 径表达式|结果|
|----|----|
|/bookstore/book[1]|选取属于 bookstore 子元素的第一个 book 元素。|
|/bookstore/book[last()]|选取属于 bookstore 子元素的最后一个 book 元素。|
|/bookstore/book[last()-1]|选取属于 bookstore 子元素的倒数第二个 book 元素。|
|/bookstore/book[position()<3]|选取最前面的两个属于 bookstore 元素的子元素的 book 元素。|
|//title[@lang]|选取所有拥有名为 lang 的属性的 title 元素。|
|//title[@lang='eng']|选取所有 title 元素，且这些元素拥有值为 eng 的 lang 属性。|
|/bookstore/book[price>35.00]|选取所有 bookstore 元素的 book 元素，且其中的 price 元素的值须大于 35.00。|
|/bookstore/book[price>35.00]/title|选取所有 bookstore 元素中的 book 元素的 title 元素，且其中的 price 元素的值须大于 35.00。|
## 选取未知节点
XPath 通配符可用来选取未知的 XML 元素。
|通配符|描述|
|----|----|
|*|匹配任何元素节点|
|@*|匹配任何属性节点|
|node()|匹配任何类型的节点|
### 实例
在下面的表格中，我们列出了一些路径表达式，以及这些表达式的结果：
|路径表达式|结果|
|----|----|
|/bookstore/*|选取 bookstore 元素的所有子节点|
|//*|选取文档中的所有元素|
|//title[@*]|选取所有带有属性的 title 元素。|
## 选取若干路径
通过在路径表达式中使用“|”运算符，您可以选取若干个路径。
### 实 例
在下面的表格中，我们列出了一些路径表达式，以及这些表达式的结果：
|路 径表达式|结果|
|----|----|
|//book/title | //book/price|选 取所有 book 元素的 title 和 price 元素。|
|//title | //price|选取所有文档中的 title 和 price 元素。|
|/bookstore/book/title | //price|选取所有属于 bookstore 元素的 book 元素的 title 元素，以及文档中所有的 price 元素。|
## XPath 轴
轴可定义某个相对于当前节点的节点集。
|轴名称|结果|
|----|----|
|ancestor|选取当前节点的所有先辈（父、祖父等）|
|ancestor-or-self|选取当前节点的所有先辈（父、祖父等）以及当前节点本身|
|attribute|选 取当前节点的所有属性|
|child|选取当前节点的所有子元素。|
|descendant|选取当前节点的所有后代元素（子、孙等）。|
|descendant-or-self|选取当前节点的所有后代元素（子、孙等）以及当前节点本身。|
|following|选 取文档中当前节点的结束标签之后的所有节点。|
|namespace|选取当前节点的所有命名 空间节点|
|parent|选取当前节点的父节点。|
|preceding|选取文档中当前节点的开始标签之前的所有节点。|
|preceding-sibling|选 取当前节点之前的所有同级节点。|
|self|选取当前节点。|
## 位置路径表达式
位置路径可以是绝对的，也可以是相对的。
绝对路径起始于正斜杠( / )，而相对路径不会这样。在两种情况中，位置路径均包括一个或多个步，每个步均被斜杠分割：
### 绝对位置路径：
/step/step/...
### 相对位置路径：
step/step/...
每个步均根据当前节点集之中的节点来进行计算。
### 步（step）包括：
- 轴（axis）
定义所选节点与当前节点之间的树关系- 节 点测试（node-test）
识别某个轴内部的节点- 零个或者更多谓语（predicate）
更深 入地提炼所选的节点集
### 步的语法：
轴名称::节点测试[谓语]
### 实例
|例子|结果|
|----|----|
|child::book|选取所有属于当前节点的子元素的 book 节点|
|attribute::lang|选 取当前节点的 lang 属性|
|child::*|选取当前节点的所有子元素|
|attribute::*|选取当前节点的所有属性|
|child::text()|选取当前节点的所有文本子节点|
|child::node()|选取当前节点的所有 子节点|
|descendant::book|选取当前节点的所有 book 后代|
|ancestor::book|选择当前节点的所有 book 先辈|
|ancestor-or-self::book|选取当前节点的所有book先辈以及当前节点（假如此节点是book节点的 话）|
|child::*/child::price|选取当前节点的所有 price 孙。|
## XPath 运算符
下面列出了可用在 XPath 表达式中的运算符：
|运算符|描述|实例|返回值|
|----|----|----|----|
|||计算两个节点集|//book | //cd|返回所有带有 book 和 ck 元素的节点集|
|+|加法|6 + 4|10|
|-|减法|6 - 4|2|
|*|乘法|6 * 4|24|
|div|除法|8 div 4|2|
|=|等于|price=9.80|如果 price 是 9.80，则返回 true。如 果 price 是 9.90，则返回 fasle。|
|!=|不等于|price!=9.80|如果 price 是 9.90，则返回 true。如果 price 是 9.80，则返回 fasle。|
|<|小于|price<9.80|如 果 price 是 9.00，则返回 true。如果 price 是 9.90，则返回 fasle。|
|<=|小于或等于|price<=9.80|如果 price 是 9.00，则返回 true。如果 price 是 9.90，则返回 fasle。|
|>|大于|price>9.80|如果 price 是 9.90，则返回 true。如果 price 是 9.80，则返回 fasle。|
|>=|大于或等于|price>=9.80|如果 price 是 9.90，则返回 true。如果 price 是 9.70，则返回 fasle。|
|or|或|price=9.80 or price=9.70|如果 price 是 9.80，则返回 true。如果 price 是 9.50，则返回 fasle。|
|and|与|price>9.00 and price<9.90|如果 price 是 9.80，则返回 true。如果 price 是 8.50，则返回 fasle。|
|mod|计算除法的余数|5 mod 2|1|
【注】Exception in thread "main" java.lang.NoClassDefFoundError: org/jaxen/JaxenException
下载 jaxen-1.1.1.jar包
