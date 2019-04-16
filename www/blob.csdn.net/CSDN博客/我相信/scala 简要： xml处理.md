# scala 简要： xml处理 - 我相信...... - CSDN博客





2015年01月02日 17:35:35[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：3219








Scala提供了对xml(当然也就支持html了)的内建支持，可以用scala.xml.Elem的值表示一个XML元素。Node类是所有xml节点类型的父类，Elem类描述xml元素。要处理某个元素的属性键和值，可以用attributes属性，然后用（)来访问定键的值 ，使用循环或asAttrMap方法遍历所有属性。

内嵌的字符串会被转成Atom[String]节点，所以可在xml中包含scala代码，被内嵌的scala代码还可以继续包含XML片段，被引用的字符串当中的花括号不会被解析和求值。

NodeSeq提供了类似xpath中/,//的操作符，scala中用\,\\ 替代，可以在模式匹配中使用xml的关键字。由于scala中xml节点和节点序列是不可变的，若要修改一个节点，需创建拷贝，给出修改，在拷贝未修改的部分。RuleTransformer类的transform方法遍历给定节点的所有后代，应用所有规则，最后返回经过变换的树。

Scala中的ContructingParser是个解析器，用于加载xml，可以保留注释，CDATA和空白，用doc.dtd可以访问到DTD。

保存XML时，没有内容的元素不会被写成自结束的标签。Scala中每个元素都有一个scope属性，类型为NamespaceBinding,该类的Uri属性输出命名空间的URI。



