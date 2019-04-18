# XPath详解，总结 - z69183787的专栏 - CSDN博客
2016年11月17日 15:09:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7076
经常在工作中会使用到XPath的相关知识，但每次总会在一些关键的地方不记得或不太清楚，所以免不了每次总要查一些零碎的知识，感觉即很烦又浪费时间，所以对XPath归纳及总结一下。
在这篇文章中你将能学习到：
- XPath简介
- XPath 路径表达式详解
- XPath在DOM，XSLT及XQuery中的应用
XPath简介
XPath是W3C的一个标准。它最主要的目的是为了在XML1.0或XML1.1文档节点树中定位节点所设计。目前有XPath1.0和XPath2.0两个版本。其中Xpath1.0是1999年成为W3C标准，而XPath2.0标准的确立是在2007年。W3C关于XPath的英文详细文档请见：[http://www.w3.org/TR/xpath20/](http://www.w3.org/TR/xpath20/) 。
XPath是一种表达式语言，它的返回值可能是节点，节点集合，原子值，以及节点和原子值的混合等。XPath2.0是XPath1.0的超集。它是对XPath1.0的扩展，它可以支持更加丰富的数据类型，并且XPath2.0保持了对XPath1.0的相对很好的向后兼容性，几乎所有的XPath2.0的返回结果都可以和XPath1.0保持一样。另外XPath2.0也是XSLT2.0和XQuery1.0的用于查询定位节点的主表达式语言。XQuery1.0是对XPath2.0的扩展。关于在XSLT和XQuery中使用XPath表达式定位节点的知识在后面的实例中会有所介绍。
在学习XPath之前你应该对XML的节点，元素，属性，原子值（文本），处理指令，注释，根节点（文档节点），命名空间以及对节点间的关系如：父（Parent），子（Children），兄弟（Sibling），先辈（Ancestor），后代（Descendant）等概念有所了解。这里不在说明。
XPath路径表达式
在本小节下面的内容中你将可以学习到：
- 路径表达式语法
- 相对/绝对路径
- 表达式上下文
- 谓词（筛选表达式）及轴的概念
- 运算符及特殊字符
- 常用表达式实例
- 函数及说明
这里给出一个实例Xml文件。下面的说明及实例都是基于该XML文件。
- 
路径表达式语法：
- 路径 = 相对路径 | 绝对路径
- XPath路径表达式 = 步进表达式 | 相对路径 "/"步进表达式。
- 步进表达式=轴 节点测试 谓词
说明：
- 其中轴表示步进表达式选择的节点和当前上下文节点间的树状关系（层次关系），节点测试指定步进表达式选择的节点名称扩展名，谓词即相当于过滤表达式以进一步过滤细化节点集。
- 谓词可以是0个或多个。多个多个谓词用逻辑操作符and， or连接。取逻辑非用not()函数。
请看一个典型的XPath查询表达式：/messages/message//child::node()[@id=0]，其中/messages/message是路径（绝对路径以"/"开始），child::是轴表示在子节点下选择，node()是节点测试表示选择所有的节点。[@id=0]是谓词，表示选择所有有属性id并且值为0的节点。
- 相对路径与绝对路径：
如果"/"处在XPath表达式开头则表示文档根元素，（表达式中间作为分隔符用以分割每一个步进表达式）如：/messages/message/subject是一种绝对路径表示法，它表明是从文档根开始查找节点。假设当前节点是在第一个message节点【/messages/message[1]】，则路径表达式subject（路径前没有"/"）这种表示法称为相对路径，表明从当前节点开始查找。具体请见下面所述的"表达式上下文"。
- 表达式上下文（Context）:
上下文其实表示一种环境。以明确当前XPath路径表达式处在什么样的环境下执行。例如同样一个路径表达式处在对根节点操作的环境和处在对某一个特定子节点操作的环境下执行所获得的结果可能是完全不一样的。也就是说XPath路径表达式计算结果取决于它所处的上下文。
XPath上下文基本有以下几种：
- 
当前节点(./)：
如./sender表示选择当前节点下的sender节点集合（等同于下面所讲的"特定元素"，如：sender）
- 
父节点(../)：
如../sender表示选择当前节点的父节点下的sender节点集合
- 
根元素（/）：
如/messages表示选择从文档根节点下的messages节点集合.
- 
根节点（/*）：
这里的*是代表所有节点，但是根元素只有一个，所以这里表示根节点。/*的返回结果和/messages返回的结果一样都是messages节点。
- 
递归下降（//）:
如当前上下文是messages节点。则//sender将返回以下结果：
/messages//sender :
<sender>gkt1980@gmail.com</sender>
<sender>111@gmail.com</sender>
<sender>333@gmail.com</sender>
/messages/message[1]//sender:
<sender>gkt1980@gmail.com</sender>
<sender>111@gmail.com</sender>
我们可以看出XPath表达式返回的结果是：从当前节点开始递归步进搜索当前节点下的所有子节点找到满足条件的节点集。
- 
特定元素
如sender：表示选择当前节点下的sender节点集合，等同于（./sender）
注意：在执行XPath时一定要注意上下文。即当前是在哪个节点下执行XPath表达式。这在XMLDOM中很重要。如：在XMLDOM中的selectNodes,selectSingleNode方法的参数都是一个XPath表达式，此时这个XPath表达式的执行上下文就是调用这个方法的节点及它所在的环境。更多信息请参见：[http://www.w3.org/TR/xpath20/](http://www.w3.org/TR/xpath20/)
- 谓词（筛选表达式）及轴的概念：
XPath的谓词即筛选表达式，类似于SQL的where子句.
|轴名称|结果|
|----|----|
|ancestor|选取当前节点的所有先辈（父、祖父等）|
|ancestor-or-self|选取当前节点的所有先辈（父、祖父等）以及当前节点本身|
|attribute|选取当前节点的所有属性|
|child|选取当前节点的所有子元素。|
|descendant|选取当前节点的所有后代元素（子、孙等）。|
|descendant-or-self|选取当前节点的所有后代元素（子、孙等）以及当前节点本身。|
|following|选取文档中当前节点的结束标签之后的所有节点。|
|namespace|选取当前节点的所有命名空间节点|
|parent|选取当前节点的父节点。|
|preceding|直到所有这个节点的父辈节点，顺序选择每个父辈节点前的所有同级节点|
|preceding-sibling|选取当前节点之前的所有同级节点。|
|self|选取当前节点。|
- 运算符及特殊字符：
|运算符/特殊字符|说明|
|----|----|
|/|此路径运算符出现在模式开头时，表示应从根节点选择。|
|//|从当前节点开始递归下降，此路径运算符出现在模式开头时，表示应从根节点递归下降。|
|.|当前上下文。|
|..|当前上下文节点父级。|
|*|通配符；选择所有元素节点与元素名无关。（不包括文本，注释，指令等节点，如果也要包含这些节点请用node()函数）|
|@|属性名的前缀。|
|@*|选择所有属性，与名称无关。|
|:|命名空间分隔符；将命名空间前缀与元素名或属性名分隔。|
|( )|括号运算符(优先级最高)，强制运算优先级。|
|[ ]|应用筛选模式（即谓词，包括"过滤表达式"和"轴（向前/向后）"）。|
|[ ]|下标运算符；用于在集合中编制索引。|
|||两个节点集合的联合，如：//messages/message/to | //messages/message/cc|
|-|减法。|
|div，|浮点除法。|
|and, or|逻辑运算。|
|mod|求余。|
|not()|逻辑非|
|=|等于|
|！=|不等于|
|特殊比较运算符|< 或者 <<= 或者 <=> 或者 >>= 或者 >=需要转义的时候必须使用转义的形式，如在XSLT中，而在XMLDOM的scripting中不需要转义。|
- 
常用表达式实例：
|/|Document Root文档根.|
|----|----|
|/*|选择文档根下面的所有元素节点，即根节点（XML文档只有一个根节点）|
|/node()|根元素下所有的节点（包括文本节点，注释节点等）|
|/text()|查找文档根节点下的所有文本节点|
|/messages/message|messages节点下的所有message节点|
|/messages/message[1]|messages节点下的第一个message节点|
|/messages/message[1]/self::node()|第一个message节点（self轴表示自身，node()表示选择所有节点）|
|/messages/message[1]/node()|第一个message节点下的所有子节点|
|/messages/message[1]/*[last()]|第一个message节点的最后一个子节点|
|/messages/message[1]/[last()]|Error，谓词前必须是节点或节点集|
|/messages/message[1]/node()[last()]|第一个message节点的最后一个子节点|
|/messages/message[1]/text()|第一个message节点的所有子节点|
|/messages/message[1]//text()|第一个message节点下递归下降查找所有的文本节点（无限深度）|
|/messages/message[1] /child::node()/messages/message[1] /node()/messages/message[position()=1]/node()//message[@id=1] /node()|第一个message节点下的所有子节点|
|//message[@id=1] //child::node()|递归所有子节点（无限深度）|
|//message[position()=1]/node()|选择id=1的message节点以及id=0的message节点|
|/messages/message[1] /parent::*|Messages节点|
|/messages/message[1]/body/attachments/parent::node()/messages/message[1]/body/attachments/parent::* /messages/message[1]/body/attachments/..|attachments节点的父节点。父节点只有一个,所以node()和* 返回结果一样。（..也表示父节点. 表示自身节点）|
|//message[@id=0]/ancestor::*|Ancestor轴表示所有的祖辈，父，祖父等。向上递归|
|//message[@id=0]/ancestor-or-self::*|向上递归,包含自身|
|//message[@id=0]/ancestor::node()|对比使用*,多一个文档根元素(Document root)|
|/messages/message[1]/descendant::node()//messages/message[1]//node()|递归下降查找message节点的所有节点|
|/messages/message[1]/sender/following::*|查找第一个message节点的sender节点后的所有同级节点，并对每一个同级节点递归向下查找。|
|//message[@id=1]/sender/following-sibling::*|查找id=1的message节点的sender节点的所有后续的同级节点。|
|//message[@id=1]/datetime/@date|查找id=1的message节点的datetime节点的date属性|
|//message[@id=1]/datetime[@date]//message/datetime[attribute::date]|查找id=1的message节点的所有含有date属性的datetime节点|
|//message[datetime]|查找所有含有datetime节点的message节点|
|//message/datetime/attribute::*//message/datetime/attribute::node()//message/datetime/@*|返回message节点下datetime节点的所有属性节点|
|//message/datetime[attribute::*]//message/datetime[attribute::node()]//message/datetime[@*]//message/datetime[@node()]|选择所有含有属性的datetime节点|
|//attribute::*|选择根节点下的所有属性节点|
|//message[@id=0]/body/preceding::node()|顺序选择body节点所在节点前的所有同级节点。（查找顺序为：先找到body节点的顶级节点（根节点）,得到根节点标签前的所有同级节点，执行完成后继续向下一级，顺序得到该节点标签前的所有同级节点，依次类推。）注意：查找同级节点是顺序查找，而不是递归查找。|
|//message[@id=0]/body/preceding-sibling::node()|顺序查找body标签前的所有同级节点。（和上例一个最大的区别是：不从最顶层开始到body节点逐层查找。我们可以理解成少了一个循环，而只查找当前节点前的同级节点）|
|//message[@id=1]//*[namespace::amazon]|查找id=1的所有message节点下的所有命名空间为amazon的节点。|
|//namespace::*|文档中的所有的命名空间节点。（包括默认命名空间xmlns:xml）|
|//message[@id=0]//books/*[local-name()='book']|选择books下的所有的book节点，注意：由于book节点定义了命名空间<amazone:book>.若写成//message[@id=0]//books/book则查找不出任何节点。|
|//message[@id=0]//books/*[local-name()='book' and namespace-uri()='http://www.amazon.com/books/schema']|选择books下的所有的book节点，(节点名和命名空间都匹配)|
|//message[@id=0]//books/*[local-name()='book'][year>2006]|选择year节点值>2006的book节点|
|//message[@id=0]//books/*[local-name()='book'][1]/year>2006|指示第一个book节点的year节点值是否大于2006.返回xs:boolean: true|
- 函数及说明：
值得欣喜的是XPath函数和XSLT，XQuery等共享函数库，函数库为我们提供了功能丰富的各种函数的调用，我们也可以自定义自己的函数。这里不再对每个函数的用法逐一说明，英文好点的朋友直接去看看w3关于XPath函数的介绍吧：[http://www.w3.org/TR/xquery-operators](http://www.w3.org/TR/xquery-operators) 。中文的可以参考这个网站,[http://www.w3school.com.cn/xpath/xpath_functions.asp](http://www.w3school.com.cn/xpath/xpath_functions.asp)
XPath在DOM,XSLT及XQuery中的应用
DOM：
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>XPath Test</title>
</head>
<body>
<script language="javascript" type="text/javascript">
var xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
xmlDoc.async="false";
xmlDoc.load("messages.xml");
xmlDoc.setProperty("SelectionLanguage", "XPath");
    var sPath = "/messages/message[1]//books/*[local-name()='book']";
var bookNodes = xmlDoc.selectNodes(sPath);
document.write("<ul>");
for ( var i = 0; i < bookNodes.length; i++) {
document.write("<li>" + bookNodes[i].childNodes[0].text + "</li>");
}
document.write("</ul>");
</script>
</body>
</html>
注意：
我们若使用new ActiveXObject("Microsoft.XMLDOM")则需要注意的是：因为早期的XMLDOM的SelectionLanguage属性默认是正则表达式，不是XPath语言。所以需要指定这样一条语句xmlDoc.setProperty("SelectionLanguage", "XPath"); 以支持XPath查询表达式。.
若没有指定SelectionLanguage属性值为XPath则要注意以下情况：
- 数组下标从0开始（我们知道在XPath查询表达式中数组下标是从1开始的）
- 不支持在XPath查询表达式中使用XPath函数。
XSLT：
见：我的另外一篇关于如何使用XSLT的一个小示范[http://www.cnblogs.com/ktgu/archive/2008/12/14/1354890.html](http://www.cnblogs.com/ktgu/archive/2008/12/14/1354890.html)
XQuery：
XQuery查询表达式：
xquery version "1.0";
<ul>
{
let $i := 0
for $x in doc("C:\Users\Administrator\Desktop\messages.xml")//message[@id=0]//books/*[local-name()='book']
where $x/year>2006
order by $x/year descending
return <li>{ data($x/name) } </li>
}
</ul>
返回结果：
<ul>
    <li>Microsoft Visual C# 2008 Step by Step </li>
    <li>Professional C# 2008 </li>
</ul>
这里只是非常简单的介绍一下应用，XPath的其他应用场合和应用技巧大家自己总结吧，若有好的体验，觉得很爽的话，别忘了告诉哥们一下啊，大家共同提高：）
