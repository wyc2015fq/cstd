# 自定义jsp标签之: TagSupport与BodyTagSupport的区别 - 零度的博客专栏 - CSDN博客
2016年05月16日 15:01:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：1715
1、 TagSupport与BodyTagSupport的区别
　　 TagSupport与BodyTagSupport的区别主要是标签处理类是否需要与标签体交互，如果不需要交互的就用TagSupport，否则如果需要交互就用BodyTagSupport。
    　交互就是标签处理类是否要读取标签体的内容和改变标签体返回的内容。
    　用TagSupport实现的标签，都可以用BodyTagSupport来实现，因为BodyTagSupport继承了TagSupport。
 2、doStartTag()，doEndTag()，doAfterBody()
     　doStartTag()方法是：在遇到标签开始时会呼叫的方法，其合法的返回值是EVAL_BODY_INCLUDE与SKIP_BODY,前者表示将显示标签间的文字，后者表示不显示标签间的文字；
   　 doEndTag()方法是：在遇到标签结束时呼叫的方法，其合法的返回值是EVAL_PAGE与 SKIP_PAGE，前者表示处理完标签后继续执行以下的JSP网页，后者是表示不处理接下来的JSP网页
   　 doAfterBody()方法是：在显示完标签间文字之后呼叫的，其返回值有EVAL_BODY_AGAIN与SKIP_BODY，前者会再显示一次标签间的文字，后者则继续执行标签处理的下一步。
　　EVAL_BODY_INCLUDE：把Body读入存在的输出流中，doStartTag()函数可用
　　EVAL_PAGE：继续处理页面，doEndTag()函数可用
　　SKIP_BODY：忽略对Body的处理，doStartTag()和doAfterBody()函数可用
　　SKIP_PAGE：忽略对余下页面的处理，doEndTag()函数可用
　　EVAL_BODY_TAG：已经废止，由EVAL_BODY_BUFFERED取代
　　EVAL_BODY_BUFFERED：申请缓冲区，由setBodyContent()函数得到的BodyContent对象来处理tag的body，如果类实现了BodyTag，那么doStartTag()可用，否则非法。
　　EVAL_BODY_BUFFERED 要将BodyContent的内容输出 如：
JspWriter w = pageContext.getOut();
  if (bodyContent != null) {
   if (w instanceof BodyContent) {
    w = ((BodyContent) w).getEnclosingWriter();
   }
  }
  String cnt = this.bodyContent.getString();
  try {
   w.write(cnt);
  } catch (IOException e) {
   e.printStackTrace();
  }
   预定的处理顺序是：doStartTag()返回SKIP_BODY,doAfterBodyTag()返回SKIP_BODY,doEndTag()返回EVAL_PAGE.
   如果继承了TagSupport之后，如果没有改写任何的方法，标签处理的执行顺序是：
   doStartTag() ->不显示文字 ->doEndTag()->执行接下来的网页
  如果您改写了doStartTag(),则必须指定返回值，如果指定了EVAL_BODY_INCLUDE,则执行顺序是
   doStartTag()->显示文字->doAfterBodyTag()->doEndTag()->执行下面的网页
display.tld 源码
xml 代码:
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE taglib PUBLIC "-//Sun Microsystems, Inc.//DTD JSP Tag Library 1.1//EN" "http://java.sun.com/j2ee/dtds/web-jsptaglibrary_1_1.dtd">
<taglib>
    <tlibversion>1.0</tlibversion>
    <jspversion>1.1</jspversion>
    <shortname>bean</shortname>
    <uri>/WEB-INF/testtag/display.tld</uri>
    <tag>
        <name>display</name>
        <tagclass>test.DisplayTag</tagclass>
        <bodycontent>JSP</bodycontent>
        <attribute>
            <name></name>
            <required></required>
            <rtexprvalue></rtexprvalue>
        </attribute>
    </tag>
</taglib>
```
DisplayTag.java 源码
java 代码:
```
package test;
import java.io.IOException;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspWriter;
import javax.servlet.jsp.tagext.TagSupport;
public class DisplayTag extends TagSupport{
    private static final long serialVersionUID = 1L;
    
      @Override
        public int doStartTag() throws JspException {
            System.out.println("doStartTag()");
            return EVAL_BODY_INCLUDE;
        }
        @Override
        public int doAfterBody() throws JspException {
            System.out.println("doAfterBody()");
            return SKIP_BODY;
        }
        
        @Override
        public int doEndTag() throws JspException {
            System.out.println("doEndTag()");
            JspWriter out = this.pageContext.getOut();
            try {
                out.print("hello12!");
            } catch (IOException e) {
                e.printStackTrace();
            }
            return super.doEndTag();
        }
}
```
index.jsp 源码
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="/WEB-INF/tld/display.tld" prefix="test" %>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<test:display>test<br></test:display>
</body>
</html>
```
注意：这里的“test”　显示在　hello 前面证明啦　它是先执行index.jsp 中标签中的内容后才执行doEndTag()
