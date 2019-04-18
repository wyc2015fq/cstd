# xslt语法之---position()函数 - z69183787的专栏 - CSDN博客
2014年02月20日 16:37:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2642
# position()函数——返回节点位置
语法：position()
参数：无
返回值：整数
用途：该函数用于返回上下文节点在正在处理的节点集合中的位置。
以下示例程序演示了position()函数的用法。
XML源文档如下所示。
**[xhtml]**[view plain](http://blog.csdn.net/lwkaoyan/article/details/5742568#)[copy](http://blog.csdn.net/lwkaoyan/article/details/5742568#)
- <?xmlversion="1.0"?>
- <root>
- <epos="1">001</e>
- <epos="2">002</e>
- <epos="3">003</e>
- <epos="4">004</e>
- <epos="5">005</e>
- </root>
XSLT文档如下所示。
**[xhtml]**[view plain](http://blog.csdn.net/lwkaoyan/article/details/5742568#)[copy](http://blog.csdn.net/lwkaoyan/article/details/5742568#)
- <?xmlversion='1.0'?>
- <xsl:stylesheetversion="1.0"xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
- <xsl:templatematch="/">
- <xsl:for-eachselect="root/e">
-       第<xsl:value-ofselect="position()"/>个e元素：<xsl:value-ofselect="."/>
- </xsl:for-each>
- </xsl:template>
- </xsl:stylesheet>
输出结果如下所示。
第1个e元素：001
第2个e元素：002
第3个e元素：003
第4个e元素：004
第5个e元素：005
