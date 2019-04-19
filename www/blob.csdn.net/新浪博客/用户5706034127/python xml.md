# python xml_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
**转载：[http://blog.csdn.net/yueguanghaidao/article/details/7265246](http://blog.csdn.net/yueguanghaidao/article/details/7265246)**
**一、简介**
XML（eXtensible Markup
Language）指可扩展标记语言，被设计用来传输和存储数据，已经日趋成为当前许多新生技术的核心，在不同的领域都有着不同的应用。它是web发展到一定阶段的必然产物，既具有SGML的核心特征，又有着HTML的简单特性，还具有明确和结构良好等许多新的特性。
python解析XML常见的有三种方法：一是xml.dom.*模块，它是W3C DOM API的实现，若需要处理DOM
API则该模块很适合，注意xml.dom包里面有许多模块，须区分它们间的不同；二是xml.sax.*模块，它是SAX
API的实现，这个模块牺牲了便捷性来换取速度和内存占用，SAX是一个基于事件的API，这就意味着它可以“在空中”处理庞大数量的的文档，不用完全加载进内存；三是xml.etree.ElementTree模块（简称
ET），它提供了轻量级的Python式的API，相对于DOM来说ET
快了很多，而且有很多令人愉悦的API可以使用，相对于SAX来说ET的ET.iterparse也提供了 “在空中”
的处理方式，没有必要加载整个文档到内存，ET的性能的平均值和SAX差不多，但是API的效率更高一点而且使用起来很方便。
