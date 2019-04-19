# XSLT中用normalize-space函数来清除元素的前后空格 - 左直拳的马桶_日用桶 - CSDN博客
2006年05月17日 18:18:00[左直拳](https://me.csdn.net/leftfist)阅读数：7042标签：[xslt																[xsl																[stylesheet																[xml																[c#																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=stylesheet&t=blog)](https://so.csdn.net/so/search/s.do?q=xsl&t=blog)](https://so.csdn.net/so/search/s.do?q=xslt&t=blog)
个人分类：[XML](https://blog.csdn.net/leftfist/article/category/94499)
XSLT中用normalize-space函数来清除元素的前后空格
 左直拳
按照我的理解，XML文件存储数据，XSLT负责显示这些数据。同样的XML文件，结合不同的XSLT，结果呈现出来的风格、样式可以大相径庭。
可能也是由于功能强大的原因，XSLT有许多语法、函数，但介绍的资料似乎很少，应用起来常常费心劳力。
现在有一个问题，从XML中获取的数据，在XSLT中进行分析之前怎样祛除数据前后的空格？如：
<xsl:choose>
<xsl:when test=".[a='']" >
<p>元素a为空</p>
</xsl:when>
<xsl:when test=".[b='']" >
<p>元素b为空</p>
</xsl:when>
<xsl:otherwise>
<p>元素a、b都不为空</p>
</xsl:otherwise>
</xsl:choose>
结果，对于a、b表面上看似乎为空的数据，如<a></a><b> </b>，全都输出了元素a、b都不为空这样的字样。很显然，在比较的时候没有删除元素里前后的空格。
要怎样才能删除这些空格？如果在C#或者其他什么语言里，早一个”Trim”飞过去把它搞定了。
辛辛苦苦跑到网上搜索一翻，发现可以用normalize-space这个函数。
可我怎么试都没有成功，说是不支持normalize-space。后来看了看XSLT的文件头，差点吐血，原来我用的是老版本的命名空间：
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">
改成新版本的命名空间：
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
问题解决了。
<xsl:choose>
<xsl:when test="normalize-space(a)=''" >
<p>元素a为空</p>
</xsl:when>
<xsl:when test="normalize-space(b)=''" >
<p>元素b为空</p>
</xsl:when>
<xsl:otherwise>
<p>元素a、b都不为空</p>
</xsl:otherwise>
</xsl:choose>
