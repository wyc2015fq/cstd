
# XPath查询 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:57:57[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：540


---------------------------------------------XPath查询--------------------------------------------
#### 1　什么是XPath
XPath即为XML路径语言（XMLPath Language），它是一种用来确定XML文档中某部分位置的语言。XPath基于XML的树状结构，提供在数据结构树中找寻节点的能力。起初 XPath 的提出的初衷是将其作为一个通用的、介于XPointer与XSL间的语法模型。但是 XPath 很快的被开发者采用来当作小型查询语言。

#### 2　DOM4J对XPath的支持
在DOM4J中，Node接口中的三个方法最为常用：
l  List selectNodes(String xpathExpression)：在当前节点中查找满足XPath表达式的所有子节点；
l  Node selectSingleNode(String xpathExpression)：在当前节点中查找满足XPath表达式的第一个子节点；
l  String valueOf(String xpathExpression)：在当前节点中查找满足XPath表达式的第一个子节点的文本内容；
|Document doc = reader.read(new  FileInputStream("person.xml"));
|Element  root = doc.getRootElement();
|List<Element>  eList = root.selectNodes("/persons");
|List<Element>  eList1 = root.selectNodes("/persons/person/name");

|List<Element>  eList2 = root.selectNodes("//age");
|List<Element>  eList3 = root.selectNodes("//person/age");

|List<Element>  eList4 = root.selectNodes("/*/*/address");
|List<Element>  eList5 = root.selectNodes("//*");
|List<Element>  eList6 = root.selectNodes("/persons/person[1]");
|List<Element>  eList7 = root.selectNodes("/persons/person[last()]");

|List<Element>  eList8 = root.selectNodes("//@id");
|List<Element>  eList9 = root.selectNodes("//person[@id]");
|List<Element>  eList10 = root.selectNodes("//person[@*]");
|List<Element>  eList11 = root.selectNodes("//*[not(@*)]");

|List<Element>  eList12 = root.selectNodes("//person[@id='person1']");
|
|package cn.tx.dom4j;
|import java.io.FileInputStream;
|import java.io.IOException;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.Element;
|import org.dom4j.io.SAXReader;
|public class SelectXML {
|/**
|* 读取xml文件的方法,使用XPath获得元素
|*/
|public  static void readXML() {
|//创建xml文档读取dom4j的对象
|SAXReader  reader = new SAXReader();
|//创建文档流
|FileInputStream  in = null;
|try  {
|in  = new FileInputStream("src/student.xml");
|//读取文档，返回文档对象
|Document  doc = reader.read(in);
|//获得文档对象的根元素
|Element  root = doc.getRootElement();
|//获得students元素
|List<Element>  eList = root.selectNodes("/students");
|//获得students下的所有子元素
|List<Element>  stuList = root.selectNodes("/students/student");
|//获得student下的name元素
|List<Element>  nameList = root.selectNodes("/students/student/name");
|//使用“//”查出所有name的元素
|List<Element>  nameList1 = root.selectNodes("//name");
|//使用“//”+“/”查出student下的score元素,无论层级关系只要满足双斜线后面的元素就都查询出来
|List<Element>  scoreList = root.selectNodes("//student/score");
|//使用“ * ”查询student下所有的元素
|List<Element>  stuList1 = root.selectNodes("//student/*");
|//使用/*/*/name 查出两级元素下所有的name元素
|List<Element>  nameList2 = root.selectNodes("/*/*/name");
|//获得指定索引的元素
|//获得students下的第一个student元素
|List<Element>  stuList2 = root.selectNodes("/students/student[1]");
|//获得students下最后一个student元素
|List<Element>  stuList3 = root.selectNodes("/students/student[last()]");
|//获得students下带有gender属性的元素，通过属性来筛选上一层中带有指定属性的元素
|List<Element>  stuList4 = root.selectNodes("/students/student/@gender");
|//获得同级元素下指定的属性元素
|List<Element>  stuList5 = root.selectNodes("/students/student[@gender]");
|//获得同级元素下所有带属性的元素
|List<Element>  stuList6 = root.selectNodes("/students/student[@*]");
|//获得同级元素下所有不带属性的元素
|List<Element>  stuList7 = root.selectNodes("/students/student[not(@*)]");
|//获得指定属性等于指定值得元素
|List<Element>  stuList8 = root.selectNodes("/students/student[@stuNo='5']");
|//获得使用normalize-space()函数去掉属性值得前后空格后，指定属性等于指定值得元素
|List<Element>  stuList9 =  root.selectNodes("/students/student[normalize-space(@stuNo)='6']");
|//使用count()函数获得含有一个name元素的上级元素元素
|List<Element>  stuList10 = root.selectNodes("/students//*[count(name)=1]");
|//获得含有任意三个子元素的上级元素
|List<Element>  stuList11 = root.selectNodes("/students//*[count(*)=3]");
|//使用name()函数获得元素名为score的元素,name()函数等同于//加函数名
|List<Element>  stuList12 = root.selectNodes("//*[name()='score']");
|//使用starts-with(name(),元素名开头字母)获得以指定字母开头的元素
|List<Element>  stuList13 = root.selectNodes("//*[starts-with(name(),'a')]");
|//使用contains(name(),元素名所包含的字母)函数获得包含指定字母的元素名的元素
|List<Element>  stuList14 =  root.selectNodes("/students/student/name[contains(name(),'m')]");
|//获得元素名的字符长度等于4的元素，如果想要大于和小于字符长度判断只需要改变符号即可
|List<Element>  stuList15 = root.selectNodes("//*[string-length(name())='4']");
|System.out.println(stuList15);
|}  catch (Exception e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(in  != null) {
|in.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|


