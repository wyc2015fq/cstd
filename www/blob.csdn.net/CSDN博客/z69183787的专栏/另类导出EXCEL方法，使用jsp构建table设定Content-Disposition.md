# 另类导出EXCEL方法，使用jsp构建table设定Content-Disposition - z69183787的专栏 - CSDN博客
2014年10月30日 11:42:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1718
基本方法：在jsp中构建类似 excel 的table格式，并设置 requestHeader标识，该文件以excel下载。
```
<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page contentType="application/vnd.ms-excel; charset=utf-8" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<!-- <meta http-equiv="Content-Type" content="application/vnd.ms-excel; charset=utf-8" /> -->
    <title></title>
    
    <style type="text/css">
	table{
		border: 0.5pt solid #000000;
	}    
	table th{
		border: 0.5pt solid #000000;
	}
	table td{
		border: 0.5pt solid #000000;
	}
  	a{
    	color: #000000;
    	text-decoration: none;
    }
    </style>
</head>
<body> 
<%
response.setContentType("application/vnd.ms-excel");
String titlename="招标需fsdfds求表";
  response.setHeader("Content-Disposition", "attachment;filename="+new String((titlename).getBytes("gbk"),"iso8859-1")+".xlsx");
%>
<center>
		<h2>大大大大fsdfsd大大</h2>
		<div>打算打萨达萨达是</div>
		<div id="maingrid" style="margin:0; padding:0">
			<table><tr><td>21321312</td></tr></table>
			</div>
</center>
</body>
</html>
```
