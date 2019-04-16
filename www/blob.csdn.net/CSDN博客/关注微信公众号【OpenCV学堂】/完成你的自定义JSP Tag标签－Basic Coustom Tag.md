# 完成你的自定义JSP Tag标签－Basic Coustom Tag - 关注微信公众号【OpenCV学堂】 - CSDN博客





2009年12月19日 10:05:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：1272标签：[jsp																[basic																[class																[tomcat																[string																[encoding](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)
个人分类：[J2EE](https://blog.csdn.net/jia20003/article/category/871633)





说明：基于Eclipse 3.2 + Tomcat 4.0编译完成

1.完成你自己的Tag Class，javax.servlet.jsp中提供一个Tag接口类

TagSupport 和 BodyTagSupport两个类，你自己的Tag Class必须

extends其中一个。本例中HrefTag Class extends TagSupport类，并

完成了doStratTag方法的重写，主要代码如下：


|public int doStartTag(){JspWriter ōut = pageContext.getOut();  String value = getValue();  String code = getCode();  System.out.println("value = " + value);  System.out.println("code = " + code);try{   if(code != null && value != null) {    out.print("<a href=");    out.print("'");    out.print(value);    out.print("'");    out.print(">");    out.print(code);    out.print("</a>");   }  }catch(IOException e){   System.out.println("Error in Tag:" +  e.getMessage());  }  return(SKIP_BODY); }|
|----|

对于一个Tag Class来说，想要正确的工作，必需有相应的TLD文件，本例为

test-taglib.tld，定义如下：


|<?xml version="1.0" encoding="ISO-8859-1" ?><!DOCTYPE taglibPUBLIC "-//Sun Microsystems, Inc.//DTD JSP Tag Library 1.1//EN""[http://java.sun.com/j2ee/dtds/web-jsptaglibrary_1_1.dtd](http://java.sun.com/j2ee/dtds/web-jsptaglibrary_1_1.dtd)"><taglib> <tag>  <name>href</name>  <tagclass>com.test.tag.HrefTag</tagclass>  <bodycontent>jsp</bodycontent>  <info>Simplest example: href the page </info>  <attribute>   <name>code</name>   <required>true</required>   <rtexprvalue>false</rtexprvalue>  </attribute>  <attribute>   <name>value</name>   <required>true</required>   <rtexprvalue>false</rtexprvalue>  </attribute> </tag></taglib>|
|----|

作为jsp页面要引用一个Tag，必需导入它对应的Tag声明，告诉系统怎么

去找到和地位到这个TLD文件，再根据TLD文件去寻找对应的Java文件

jsp页面声明如下：


|<%@ taglib uri="simpleTag" prefix="test" %>|
|----|

页面中应用Tag如下：


|<li><test:href code="Click here" value="[http://www.sina.com.cn](http://www.sina.com.cn/)" />|
|----|

本文中由于在页面声明Taglib时候使用的uri是相对路径，因此要在

web.xml中加上taglib的声明,如下：


|<taglib>   <taglib-uri>simpleTag</taglib-uri>   <taglib-location>              /WEB-INF/tld/test-taglib.tld   </taglib-location></taglib>|
|----|

说一下我的目录结构：

jspTagSample

/src/com.test.tag.HrefTag

/jsp/index.jsp

/jsp/web-inf/web.xml

/jsp/web-inf/tld/test-taglib.tld

/build.xml

最后是程序的效果，点击Click here将会超连接到新浪主页



[](http://jysq.net/batch.download.php?aid=12110)

学习本例时候，可能遇到的常见问题：

页面请求时候遇到：Unable to compile clas for JSP

对策：把在build路径中要加上：%tomcat_home%/common/lib/*.jar

对应的build.xml文件为：

<path id="libraries">

        <fileset dir="${build}">

            <include name="*.jar"/>

        </fileset>

</path>](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)




