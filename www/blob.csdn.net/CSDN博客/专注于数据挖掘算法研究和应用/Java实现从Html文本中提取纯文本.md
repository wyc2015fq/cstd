# Java实现从Html文本中提取纯文本 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月05日 17:04:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：16990
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









1、应用场景：从一份html文件中或从String（是html内容）中提取纯文本，去掉网页标签；

2、代码一：replaceAll搞定



```java
//从html中提取纯文本
	public static String StripHT(String strHtml) {
		 String txtcontent = strHtml.replaceAll("</?[^>]+>", ""); //剔出<html>的标签  
         txtcontent = txtcontent.replaceAll("<a>\\s*|\t|\r|\n</a>", "");//去除字符串中的空格,回车,换行符,制表符  
         return txtcontent;
    }
```


3、代码二：正则表达式搞定



```java
//从html中提取纯文本
	public static String Html2Text(String inputString) {
		String htmlStr = inputString; // 含html标签的字符串
		String textStr = "";
		java.util.regex.Pattern p_script;
		java.util.regex.Matcher m_script;
		java.util.regex.Pattern p_style;
		java.util.regex.Matcher m_style;
		java.util.regex.Pattern p_html;
		java.util.regex.Matcher m_html;
		try {
			String regEx_script = "<[\\s]*?script[^>]*?>[\\s\\S]*?<[\\s]*?\\/[\\s]*?script[\\s]*?>"; // 定义script的正则表达式{或<script[^>]*?>[\\s\\S]*?<\\/script>
	        String regEx_style = "<[\\s]*?style[^>]*?>[\\s\\S]*?<[\\s]*?\\/[\\s]*?style[\\s]*?>"; // 定义style的正则表达式{或<style[^>]*?>[\\s\\S]*?<\\/style>
	        String regEx_html = "<[^>]+>"; // 定义HTML标签的正则表达式
	        p_script = Pattern.compile(regEx_script, Pattern.CASE_INSENSITIVE);
	        m_script = p_script.matcher(htmlStr);
	        htmlStr = m_script.replaceAll(""); // 过滤script标签
	        p_style = Pattern.compile(regEx_style, Pattern.CASE_INSENSITIVE);
	        m_style = p_style.matcher(htmlStr);
	        htmlStr = m_style.replaceAll(""); // 过滤style标签
	        p_html = Pattern.compile(regEx_html, Pattern.CASE_INSENSITIVE);
	        m_html = p_html.matcher(htmlStr);
	        htmlStr = m_html.replaceAll(""); // 过滤html标签
	        textStr = htmlStr;
	    } catch (Exception e) {System.err.println("Html2Text: " + e.getMessage()); }
		//剔除空格行
		textStr=textStr.replaceAll("[ ]+", " ");
		textStr=textStr.replaceAll("(?m)^\\s*$(\\n|\\r\\n)", "");
		return textStr;// 返回文本字符串
	}
```


3、代码三：HTMLEditorKit.ParserCallback搞定，Java自带的类



```java
package com.util;

import java.io.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.*;

public class Html2Text extends HTMLEditorKit.ParserCallback {
	 StringBuffer s;

	 public Html2Text() {}

	 public void parse(Reader in) throws IOException {
	   s = new StringBuffer();
	   ParserDelegator delegator = new ParserDelegator();
	   // the third parameter is TRUE to ignore charset directive
	   delegator.parse(in, this, Boolean.TRUE);
	 }

	 public void handleText(char[] text, int pos) {
	   s.append(text);
	 }

	 public String getText() {
	   return s.toString();
	 }

	 public static void main (String[] args) {
	   try {
	     // the HTML to convert
		 //Reader in=new StringReader("string");	
	     FileReader in = new FileReader("java-new.html");
	     Html2Text parser = new Html2Text();
	     parser.parse(in);
	     in.close();
	     System.out.println(parser.getText());
	   }
	   catch (Exception e) {
	     e.printStackTrace();
	   }
	 }
}
```




