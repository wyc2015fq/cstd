# weblogic文件下载不弹出窗口 - z69183787的专栏 - CSDN博客
2014年07月24日 20:14:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1938
有这样一个文件下载连接如下：
<a href="文件名.xls" >download</a>
在tomcat下，点download的时候，可以弹出文件下载框，但是在weblogic下却直接把该文件打开了。
原因是：weblogic是根据HTTP Header 的文件类型来判断是否弹出下载框的，默认的文件类型是contentType="text/html"，所以是自动打开的。而tomcat是默认把文件都作为二进制文件，所以弹出对话框。
那如何让weblogic能弹出对话框呢，示例如下：
**test1.jsp 内容如下：**
<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<script type="text/javascript">
  function returnTo(url){
   window.location= 'test2.jsp?filename='+encodeURI(url);
  }
</script>
//注意：encodeURI 函数可以很好的解决utf-8编码下常见的 奇数个汉语最后一个为乱码的问题！
<title></title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</head>
<body>
<a href="javascript:returnTo('文件名.xls')" >download</a>
</body>
</html>
** 一：采用RequestDispatcher的方式进行**
**test2.jsp 内容如下：**
<%@ page language="java"  pageEncoding="utf-8"%>
<%   
  String filename = request.getParameter("filename");
 response.setContentType("application/x-download");//设置为下载application/x-download
filename = new String(filename.getBytes("ISO-8859-1"), "utf-8"); //解决get请求传递参数为乱码的问题。
String display = java.net.URLEncoder.encode(filename,"utf-8"); //解决文件下载框汉语文件名为乱码的问题。
  response.addHeader("Content-Disposition","attachment;filename=" + display); 
  try   
  {   
  RequestDispatcher dis = application.getRequestDispatcher("/"+filename);   
  if(dis!= null)   
  {   
  dis.forward(request,response);   
  }   
  response.flushBuffer();   
  }   
  catch(Exception e)   
  {   
  e.printStackTrace();   
  }   
  finally   
  {   
  }   
%>
**二：采用文件流输出的方式下载**
**test2.jsp 内容如下：**
<%@ page language="java" pageEncoding="utf-8"%>
<%
 out.clear(); 
 response.reset(); 
 response.setContentType("application/x-download");
 String filename = request.getParameter("filename");
 response.setContentType("application/x-download");
 filename = new String(filename.getBytes("ISO-8859-1"), "utf-8");
 String display = java.net.URLEncoder.encode(filename, "utf-8");
 response.addHeader("Content-Disposition", "attachment;filename="
   + display); // 
String path = application.getRealPath(filename);//取得物理路径
 java.io.OutputStream outp = null;
 java.io.FileInputStream in = null;
 try {
  outp = response.getOutputStream();
  in = new java.io.FileInputStream(path);
  byte[] b = new byte[1024];
  int i = 0;
  while ((i = in.read(b)) > 0) {
   outp.write(b, 0, i);
  }
 } catch (Exception e) {
  System.out.println("Error!");
  e.printStackTrace();
 } finally {
  if (in != null) {
   in.close();
   in = null;
  }
 }
%>
