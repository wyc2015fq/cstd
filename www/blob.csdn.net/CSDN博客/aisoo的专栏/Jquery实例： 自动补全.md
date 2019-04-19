# Jquery实例： 自动补全 - aisoo的专栏 - CSDN博客
2009年12月17日 22:56:00[aisoo](https://me.csdn.net/aisoo)阅读数：655
1 jqueryauto.js
var highlightindex = -1;
var timeoutId;
$("document").ready(function(){
 var wordInput = $("#word");
 var wordInputOffset = wordInput.offset();
 $("#auto").hide().css("border", "1px black solid").css("position", "absolute")
      .css("top", wordInputOffset.top + wordInput.height() + 4 + "px")
      .css("left", wordInputOffset.left + "px").width(wordInput.width() + 2);
 wordInput.keyup(function(event) {
  var myEvent = event || window.event;
  var keyCode = myEvent.keyCode;
  // 输入的字母，或者是退格键或Delete键
  if (keyCode >= 65 && keyCode <= 90 || keyCode == 8 || keyCode == 46) {
   var wordText = $("#word").val();
   var autoNode = $("#auto"); 
   if (wordText != "") {
    clearTimeout(timeoutId);
    // 对于服务器端进行交互延迟500ms， 避免快速打字造成频繁请求
    timeoutId = setTimeout(function() {
     $.post("AjaxAutoComlete", {word : wordText}, function(data) {
      var jqueryObj = $(data);
      var wordNodes = jqueryObj.find("word");
      autoNode.html("");
      wordNodes.each(function(i) {
       var wordNode = $(this);
       var newDivNode = $("<div>").attr("id", i);
       newDivNode.html(wordNode.text()).appendTo(autoNode);
       // 增加鼠标进入事件， 高亮节点
       newDivNode.mouseover(function(){
        // 将原来高亮节点取消高亮
        if (highlightindex != -1) {
         $("#auto").children("div").eq(highlightindex).css("background-color", "white");
        }
        // 记录新的高亮索引
        highlightindex = $(this).attr("id");
        $(this).css("background-color", "red"); 
       });
       // 增加鼠标移出事件
       newDivNode.mouseout(function() {
        $(this).css("background-color", "white"); 
       });
       // 增加鼠标点击事件
       newDivNode.click(function() {
        var comText = $("#auto").hide().children("div").eq(highlightindex).text();
        highlightindex = -1;
        $("#word").val(comText); 
       });
      })
      if (wordNodes.length > 0) {
       autoNode.show();
      } else {
       autoNode.hide();
       highlightindex = -1;
      }
     }, "xml")
    },500);
   } else {
    autoNode.hide();
    highlightindex = -1
   }
  // 如果输入的向下40或向上38
  } else if (keyCode == 38 || keyCode == 40) {
   if (keyCode == 38) {
    var autoNodes = $("#auto").children("div");
    // 如果原来存在高亮节点， 则将背景色改成白色
    if (highlightindex != -1) { 
     autoNodes.eq(highlightindex).css("background-color", "white");
     highlightindex--;
    } else {
     highlightindex = autoNodes.length - 1;
    }
    // 如果修改索引值以后index变成-1，则将索引值指向最后一个元素
    if (highlightindex == -1) {
     highlightindex = autoNodes.length - 1;
    }
    // 让现在高亮的内容变成红色
    autoNodes.eq(highlightindex).css("background-color", "red");
   }
   if (keyCode == 40) {
    var autoNodes = $("#auto").children("div");
    // 如果原来存在高亮节点， 则将背景色改成白色
    if (highlightindex != -1) { 
     autoNodes.eq(highlightindex).css("background-color", "white");
    }
    highlightindex++;
    // 如果修改索引值以后index变成-1，则将索引值指向最后一个元素
    if (highlightindex == autoNodes.length) {
     highlightindex = 0;
    }
    // 让现在高亮的内容变成红色
    autoNodes.eq(highlightindex).css("background-color", "red");
   }
  // 如果输入的是回车
  } else if (keyCode == 13){
   // 下拉框有高亮内容
   if (highlightindex != -1) {
    var comText = $("#auto").hide().children("div").eq(highlightindex).text();
    highlightindex = -1;
    $("#word").val(comText);
   // 下拉框无高亮内容
   } else {
    $("#auto").hide()
    // 让文本框失去焦点
    $("#word").get(0).blur();
   }   
  }
 });
 $("input[type='button']").click(function(){
 });
});
2 JqueryAutoComplete.html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
 <script type="text/javascript" src="js/jquery.js"></script>
 <script type="text/javascript" src="js/jqueryauto.js"></script>
  <title>My JSP </title>
 </head>
 <body>
  Jquery实例： 自动补全<input type="text" id="word" />
  <input type="button" value="提交"/>
  <div id="auto"></div>
 </body>
</html>
3  wordxml.jsp
<%@ page language="java" contentType="text/xml; charset=UTF-8"%>
<%
 String word = request.getAttribute("word").toString();
%>
<words>
 <% if ("absolute".startsWith(word)) {%>
  <word>absolute</word>
 <%}%>
 <% if ("anyone".startsWith(word)) {%>
  <word>anyone</word>
 <%}%>
 <% if ("anything".startsWith(word)) {%>
  <word>anything</word>
 <%}%>
 <% if ("apple".startsWith(word)) {%>
  <word>apple</word>
 <%}%>
 <% if ("absolute".startsWith(word)) {%>
  <word>absolute</word>
 <%}%>
 <% if ("abandon".startsWith(word)) {%>
  <word>abandon</word>
 <%}%>
 <% if ("breach".startsWith(word)) {%>
  <word>breach</word>
 <%}%>
 <% if ("break".startsWith(word)) {%>
  <word>break</word>
 <%}%>
 <% if ("boolean".startsWith(word)) {%>
  <word>boolean</word>
 <%}%>
</words>
4 AjaxAutoComlete.java
   package ajax;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class AjaxAutoComlete extends HttpServlet {
 public void doGet(HttpServletRequest request, HttpServletResponse response)
   throws ServletException, IOException {
   String word = request.getParameter("word");
   request.setAttribute("word", word);
   request.getRequestDispatcher("wordxml.jsp").forward(request, response);
 }
 public void doPost(HttpServletRequest request, HttpServletResponse response)
   throws ServletException, IOException {
  doGet(request,response);
 }
}
5 web.xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.4" 
 xmlns="[http://java.sun.com/xml/ns/j2ee](http://java.sun.com/xml/ns/j2ee)" 
 xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)" 
 xsi:schemaLocation="[http://java.sun.com/xml/ns/j2ee](http://java.sun.com/xml/ns/j2ee)
[http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd](http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd)">
  <servlet>
    <servlet-name>AjaxAutoComlete</servlet-name>
    <servlet-class>ajax.AjaxAutoComlete</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>AjaxAutoComlete</servlet-name>
    <url-pattern>/AjaxAutoComlete</url-pattern>
  </servlet-mapping>
</web-app>
