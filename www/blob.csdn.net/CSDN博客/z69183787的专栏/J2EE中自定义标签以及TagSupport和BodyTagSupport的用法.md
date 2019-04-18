# J2EE中自定义标签以及TagSupport和BodyTagSupport的用法 - z69183787的专栏 - CSDN博客
2015年06月28日 17:59:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1011

**StringAppendTags**
package cn.xy.myTag;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;
public class StringAppendTags extends **TagSupport**
{
 private static final long serialVersionUID = 1L;
 public StringAppendTags()
 {
 }
 @Override
 public int doStartTag() throws JspException
 {
  return 1;
 }
}
**StringAppendTag**
package cn.xy.myTag;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.BodyContent;
import javax.servlet.jsp.tagext.BodyTagSupport;
/**
 * 标签一般执行顺序:doStartTag()->显示文字->doAfterBodyTag()->doEndTag()->执行下面的网页
 * @author xy
 * 
 */
public class StringAppendTag extends **BodyTagSupport**
{
 private static final long serialVersionUID = 1L;
 /**
  * 值处理过后存储在pageContext中的键
  */
 private String key;
 /**
  * 标签开始时调用该方法
  */
 public int doStartTag() throws JspException
 {
  // 处理标签体
  return EVAL_BODY_BUFFERED;
 }
 /**
  * 处理标签体方法
  */
 public int doAfterBody() throws JspException
 {
  // 标签体对象
  BodyContent body = getBodyContent();
  // 标签体中字符串内容
  StringBuffer bodyContent = new StringBuffer(body.getString());
  // 清除标签体中内容
  body.clearBody();
  // 将处理过后的变量保存在pageContext对象
  this.pageContext.setAttribute(key, bodyContent.append("xy").toString());
  // 结束对标签体的处理
  return SKIP_BODY;
 }
 public String getKey()
 {
  return key;
 }
 public void setKey(String key)
 {
  this.key = key;
 }
}
**xy-tags.tld**
<?xml version="1.0" encoding="UTF-8"?>
<taglib version="2.1" xmlns="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee)"
 xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"
 xsi:schemaLocation="[http://java.sun.com/xml/ns/javaee](http://java.sun.com/xml/ns/javaee) web-jsptaglibrary_2_1.xsd">
 <tlib-version>1.0</tlib-version>
 <short-name>xy</short-name>
 <uri>/xy-tags</uri>
 <tag>
  <name>StringAppenders</name>
  <tag-class>**cn.xy.myTag.StringAppendTags**</tag-class>
  <body-content>JSP</body-content>
 </tag>
 <tag>
  <name>StringAppender</name>
  <tag-class>**cn.xy.myTag.StringAppendTag**</tag-class>
  <body-content>tagdependent</body-content>
  <variable>
   <name-from-attribute>key</name-from-attribute>
   <variable-class>java.lang.String</variable-class>
   <scope>AT_BEGIN</scope>
  </variable>
  <attribute>
   <name>key</name>
   <required>true</required>
  </attribute>
 </tag>
</taglib>
**用法**
<body>
 <x:StringAppenders>
  <x:StringAppender key="str1">I am </x:StringAppender>
  <x:StringAppender key="str2">I am </x:StringAppender>
  <x:StringAppender key="str3">I am </x:StringAppender>
 </x:StringAppenders>
 字符串添标签的结果 ${ str1 } | ${pageScope.str1 } <br> 
 字符串添标签的结果 ${ str2 } | ${pageScope.str2 } <br> 
 字符串添标签的结果 ${ str3 } | ${pageScope.str3 } <br>
</body>
**结果**
字符串添标签的结果 I am xy | I am xy 
字符串添标签的结果 I am xy | I am xy 
字符串添标签的结果 I am xy | I am xy
