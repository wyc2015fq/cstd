# 在html属性中使用xsl变量 - z69183787的专栏 - CSDN博客
2014年02月20日 16:11:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1784
在使用xsl把xml转换成html中，有时候 html元素属性的值需要从xml文档中取
比如：<a>的href属性 和<image> src属性
此时需要用到xsl中的变量：<xsl:variable>
声明变量： <xsl:variable name="path" select="urlAddress"/>
name是后续要用到的变量名称 select是从xml中取值的表达式
1.在html中使用变量：
<a href="{$path}"><xsl:value-of select ="title"/></a>
<a>元素的href属性值来之刚定义的变量 需要加大括号
而innerText属性则来自xml中另外一个元素：title
2.继续在xsl中使用变量：
<h3> <xsl:value-of select="$path" /> </h3>
在XSL指令属性中使用诸如**select**的变量，是该规则的一种例外情况
不需要加大括号
