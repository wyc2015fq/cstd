# java实现附件预览（openoffice+swftools+flexpaper） - z69183787的专栏 - CSDN博客
2013年12月21日 16:20:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：67177
## 先附上本人参考的文章，基于的 flexpaper版本 为 1.5，本人由于使用的是 2.1.9 ，故之后说明：
## 已经支持加载中文文件名
## [代码下载](http://download.csdn.net/detail/z69183787/6750069)
**1.概述**
主要原理
1.通过第三方工具openoffice，将word、excel、ppt、txt等文件转换为pdf文件
2.通过swfTools将pdf文件转换成swf格式的文件
3.通过FlexPaper文档组件在页面上进行展示
2.**安装包下载**
1.openoffice是Apache下的一个开放免费的文字处理软件
下载地址：[Apache oppenoffice
 官网下载 版本-3.4.1](http://www.openoffice.org/zh-cn/download/)
2.SWFTools是一组用来处理Flash的swf文件的工具包，我们使用它将pdf文件转成swf文件!
下载地址：[SWFTools官网下载 swftools-2013-04-09-1007.exe](http://www.swftools.org/download.html)
3.FlexPaper是一个开源轻量级的在浏览器上显示各种文档的组件
下载地址：[FlexPaper官网下载
 版本1.5.1](http://flexpaper.devaldi.com/download/)
4.JODConverter一个Java的OpenDocument 文件转换器，在此我们只用到它的jar包
下载地址：[JODCConverter下载](http://sourceforge.net/projects/jodconverter/files/)
3.**安装文件**
1.将所下载的文件（JODConverter除外）进行安装，盘符可随自己设定！需要注意的是在openoffice安装完成后，当我们使用它时，需将它的服务打开。在次我们需要以命令的方式打开:
打开dos窗口，进入openoffice安装盘符，输入以下代码来启动服务：
soffice -headless -accept="socket,host=127.0.0.1,port=8100;urp;" -nofirststartwizard
注意最后一个命令前边的‘—’,可不要写错！服务起不来，项目可是继续不下去的哦.
官网启动服务截图如下：
![](https://img-blog.csdn.net/20130509175206145)
本地截图：
![](https://img-blog.csdn.net/20130509175251332)
**3.开发过程**
1.新建项目，将flexpaper文件中的js文件夹(包含了flexpaper_flash_debug.js，flexpaper_flash.js,jquery.js,这三个js文件主要是预览swf文件的插件)拷贝至网站根目录;将FlexPaperViewer.swf拷贝至网站根目录下(该文件主要是用在网页中播放swf文件的播放器)，目录结构如下图：
![](https://img-blog.csdn.net/20130509175618717)
注：需创建upload文件夹
2.创建fileUpload.jsp
- <%@ page language="java"contentType="text/html; charset=UTF-8"
- pageEncoding="UTF-8"%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <title>文档在线预览系统</title>
- <style>
-     body {margin-top:100px;background:#fff;font-family: Verdana, Tahoma;}  
-     a {color:#CE4614;}  
-     #msg-box {color: #CE4614; font-size:0.9em;text-align:center;}  
-     #msg-box .logo {border-bottom:5px solid #ECE5D9;margin-bottom:20px;padding-bottom:10px;}  
-     #msg-box .title {font-size:1.4em;font-weight:bold;margin:0 0 30px 0;}  
-     #msg-box .nav {margin-top:20px;}  
- </style>
- 
- </head>
- <body>
- <divid="msg-box">
- <formname="form1"method="post"enctype="multipart/form-data"action="docUploadConvertAction.jsp">
- <divclass="title">
-             请上传要处理的文件，过程可能需要几分钟，请稍候片刻。  
- </div>
- <p>
- <inputname="file1"type="file">
- </p>
- <p>
- <inputtype="submit"name="Submit"value="上传">
- </p>
- </form>
- </div>
- </body>
- </html>
3.创建转换页docUploadConvertAction.jsp
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- 
- <%@page import="java.io.*"%>
- <%@page import="java.util.Enumeration"%>
- <%@page import="com.oreilly.servlet.MultipartRequest"%>
- <%@page import="com.oreilly.servlet.multipart.DefaultFileRenamePolicy"%>
- <%@page import="com.cectsims.util.DocConverter"%>
- <%  
- //文件上传采用cos组件上传，可更换为commons-fileupload上传，文件上传后，保存在upload文件夹  
- //获取文件上传路径  
- String saveDirectory =application.getRealPath("/")+"upload";  
- //打印上传路径信息  
- System.out.println(saveDirectory);  
- //每个文件最大50m  
- int maxPostSize = 50 * 1024 * 1024 ;  
- //采用cos缺省的命名策略，重名后加1,2,3...如果不加dfp重名将覆盖  
- DefaultFileRenamePolicy dfp = new DefaultFileRenamePolicy();  
- //response的编码为"UTF-8",同时采用缺省的文件名冲突解决策略,实现上传,如果不加dfp重名将覆盖  
- MultipartRequest multi = new MultipartRequest(request, saveDirectory, maxPostSize,"UTF-8",dfp);  
- //MultipartRequest multi = new MultipartRequest(request, saveDirectory, maxPostSize,"UTF-8");  
- //输出反馈信息  
-  Enumeration files = multi.getFileNames();  
-      while (files.hasMoreElements()) {  
-         System.err.println("ccc");  
-        String name = (String)files.nextElement();  
-        File f = multi.getFile(name);  
-        if(f!=null){  
-          String fileName = multi.getFilesystemName(name);  
-          //获取上传文件的扩展名  
-          String extName=fileName.substring(fileName.lastIndexOf(".")+1);  
-          //文件全路径  
-          String lastFileName= saveDirectory+"\\" + fileName;  
-          //获取需要转换的文件名,将路径名中的'\'替换为'/'  
-          String converfilename = saveDirectory.replaceAll("\\\\", "/")+"/"+fileName;  
-          System.out.println(converfilename);  
-          //调用转换类DocConverter,并将需要转换的文件传递给该类的构造方法  
-          DocConverter d = new DocConverter(converfilename);  
-          //调用conver方法开始转换，先执行doc2pdf()将office文件转换为pdf;再执行pdf2swf()将pdf转换为swf;  
-          d.conver();  
-          //调用getswfPath()方法，打印转换后的swf文件路径  
-          System.out.println(d.getswfPath());  
-          //生成swf相对路径，以便传递给flexpaper播放器  
-          String swfpath = "upload"+d.getswfPath().substring(d.getswfPath().lastIndexOf("/"));  
-          System.out.println(swfpath);  
-          //将相对路径放入sessio中保存  
-          session.setAttribute("swfpath", swfpath);  
-          out.println("上传的文件:"+lastFileName);  
-          out.println("文件类型"+extName);  
-          out.println("<hr>");  
-        }  
-      }  
- 
- %>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <title>Insert title here</title>
- <style>
-     body {margin-top:100px;background:#fff;font-family: Verdana, Tahoma;}  
-     a {color:#CE4614;}  
-     #msg-box {color: #CE4614; font-size:0.9em;text-align:center;}  
-     #msg-box .logo {border-bottom:5px solid #ECE5D9;margin-bottom:20px;padding-bottom:10px;}  
-     #msg-box .title {font-size:1.4em;font-weight:bold;margin:0 0 30px 0;}  
-     #msg-box .nav {margin-top:20px;}  
- </style>
- </head>
- <body>
- <div>
- <formname="viewForm"id="form_swf"action="documentView.jsp"method="POST">
- <inputtype='submit'value='预览'class='BUTTON SUBMIT'/>
- </form>
- </div>
- </body>
- </html>
4.创建查看页documentView.jsp
- <%@ page language="java"contentType="text/html; charset=UTF-8"pageEncoding="UTF-8"%>
- <%  
-     String swfFilePath=session.getAttribute("swfpath").toString();  
- %>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <scripttype="text/javascript"src="js/jquery.js"></script>
- <scripttype="text/javascript"src="js/flexpaper_flash.js"></script>
- <scripttype="text/javascript"src="js/flexpaper_flash_debug.js"></script>
- <styletype="text/css"media="screen">
-             html, body  { height:100%; }  
-             body { margin:0; padding:0; overflow:auto; }     
-             #flashContent { display:none; }  
- </style>
- 
- <title>文档在线预览系统</title>
- </head>
- <body>
- <divstyle="position:absolute;left:50px;top:10px;">
- <aid="viewerPlaceHolder"style="width:820px;height:650px;display:block"></a>
- 
- <scripttype="text/javascript">
-                 var fp = new FlexPaperViewer(     
-                          'FlexPaperViewer',  
-                          'viewerPlaceHolder', { config : {  
-                          SwfFile : escape('<%=swfFilePath%>'),  
-                          Scale : 0.6,   
-                          ZoomTransition : 'easeOut',  
-                          ZoomTime : 0.5,  
-                          ZoomInterval : 0.2,  
-                          FitPageOnLoad : true,  
-                          FitWidthOnLoad : false,  
-                          FullScreenAsMaxWindow : false,  
-                          ProgressiveLoading : false,  
-                          MinZoomSize : 0.2,  
-                          MaxZoomSize : 5,  
-                          SearchMatchAll : false,  
-                          InitViewMode : 'SinglePage',  
- 
-                          ViewModeToolsVisible : true,  
-                          ZoomToolsVisible : true,  
-                          NavToolsVisible : true,  
-                          CursorToolsVisible : true,  
-                          SearchToolsVisible : true,  
- 
-                          localeChain: 'en_US'  
-                          }});  
- </script>
- </div>
- </body>
- </html>
5.创建转换类DocConverter.java
- package com.cectsims.util;  
- import java.io.BufferedInputStream;  
- import java.io.File;  
- import java.io.IOException;  
- import java.io.InputStream;  
- 
- import com.artofsolving.jodconverter.DocumentConverter;  
- import com.artofsolving.jodconverter.openoffice.connection.OpenOfficeConnection;  
- import com.artofsolving.jodconverter.openoffice.connection.SocketOpenOfficeConnection;  
- import com.artofsolving.jodconverter.openoffice.converter.OpenOfficeDocumentConverter;  
- 
- /**
-  * doc docx格式转换
-  */
- publicclass DocConverter {  
- privatestaticfinalint environment = 1;// 环境 1：windows 2:linux
- private String fileString;// (只涉及pdf2swf路径问题)
- private String outputPath = "";// 输入路径 ，如果不设置就输出在默认的位置
- private String fileName;  
- private File pdfFile;  
- private File swfFile;  
- private File docFile;  
- 
- public DocConverter(String fileString) {  
-         ini(fileString);  
-     }  
- 
- /**
-      * 重新设置file
-      * 
-      * @param fileString
-      */
- publicvoid setFile(String fileString) {  
-         ini(fileString);  
-     }  
- 
- /**
-      * 初始化
-      * 
-      * @param fileString
-      */
- privatevoid ini(String fileString) {  
- this.fileString = fileString;  
-         fileName = fileString.substring(0, fileString.lastIndexOf("."));  
-         docFile = new File(fileString);  
-         pdfFile = new File(fileName + ".pdf");  
-         swfFile = new File(fileName + ".swf");  
-     }  
- 
- /**
-      * 转为PDF
-      * 
-      * @param file
-      */
- privatevoid doc2pdf() throws Exception {  
- if (docFile.exists()) {  
- if (!pdfFile.exists()) {  
-                 OpenOfficeConnection connection = new SocketOpenOfficeConnection(8100);  
- try {  
-                     connection.connect();  
-                     DocumentConverter converter = new OpenOfficeDocumentConverter(connection);  
-                     converter.convert(docFile, pdfFile);  
- // close the connection
-                     connection.disconnect();  
-                     System.out.println("****pdf转换成功，PDF输出：" + pdfFile.getPath()+ "****");  
-                 } catch (java.net.ConnectException e) {  
-                     e.printStackTrace();  
-                     System.out.println("****swf转换器异常，openoffice服务未启动！****");  
- throw e;  
-                 } catch (com.artofsolving.jodconverter.openoffice.connection.OpenOfficeException e) {  
-                     e.printStackTrace();  
-                     System.out.println("****swf转换器异常，读取转换文件失败****");  
- throw e;  
-                 } catch (Exception e) {  
-                     e.printStackTrace();  
- throw e;  
-                 }  
-             } else {  
-                 System.out.println("****已经转换为pdf，不需要再进行转化****");  
-             }  
-         } else {  
-             System.out.println("****swf转换器异常，需要转换的文档不存在，无法转换****");  
-         }  
-     }  
- 
- /**
-      * 转换成 swf
-      */
- @SuppressWarnings("unused")  
- privatevoid pdf2swf() throws Exception {  
-         Runtime r = Runtime.getRuntime();  
- if (!swfFile.exists()) {  
- if (pdfFile.exists()) {  
- if (environment == 1) {// windows环境处理
- try {  
-                         Process p = r.exec("D:/Program Files/SWFTools/pdf2swf.exe "+ pdfFile.getPath() + " -o "+ swfFile.getPath() + " -T 9");  
-                         System.out.print(loadStream(p.getInputStream()));  
-                         System.err.print(loadStream(p.getErrorStream()));  
-                         System.out.print(loadStream(p.getInputStream()));  
-                         System.err.println("****swf转换成功，文件输出："
-                                 + swfFile.getPath() + "****");  
- if (pdfFile.exists()) {  
-                             pdfFile.delete();  
-                         }  
- 
-                     } catch (IOException e) {  
-                         e.printStackTrace();  
- throw e;  
-                     }  
-                 } elseif (environment == 2) {// linux环境处理
- try {  
-                         Process p = r.exec("pdf2swf " + pdfFile.getPath()  
-                                 + " -o " + swfFile.getPath() + " -T 9");  
-                         System.out.print(loadStream(p.getInputStream()));  
-                         System.err.print(loadStream(p.getErrorStream()));  
-                         System.err.println("****swf转换成功，文件输出："
-                                 + swfFile.getPath() + "****");  
- if (pdfFile.exists()) {  
-                             pdfFile.delete();  
-                         }  
-                     } catch (Exception e) {  
-                         e.printStackTrace();  
- throw e;  
-                     }  
-                 }  
-             } else {  
-                 System.out.println("****pdf不存在,无法转换****");  
-             }  
-         } else {  
-             System.out.println("****swf已经存在不需要转换****");  
-         }  
-     }  
- 
- static String loadStream(InputStream in) throws IOException {  
- 
- int ptr = 0;  
-         in = new BufferedInputStream(in);  
-         StringBuffer buffer = new StringBuffer();  
- 
- while ((ptr = in.read()) != -1) {  
-             buffer.append((char) ptr);  
-         }  
- 
- return buffer.toString();  
-     }  
- /**
-      * 转换主方法
-      */
- @SuppressWarnings("unused")  
- publicboolean conver() {  
- 
- if (swfFile.exists()) {  
-             System.out.println("****swf转换器开始工作，该文件已经转换为swf****");  
- returntrue;  
-         }  
- 
- if (environment == 1) {  
-             System.out.println("****swf转换器开始工作，当前设置运行环境windows****");  
-         } else {  
-             System.out.println("****swf转换器开始工作，当前设置运行环境linux****");  
-         }  
- try {  
-             doc2pdf();  
-             pdf2swf();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
- returnfalse;  
-         }  
- 
- if (swfFile.exists()) {  
- returntrue;  
-         } else {  
- returnfalse;  
-         }  
-     }  
- 
- /**
-      * 返回文件路径
-      * 
-      * @param s
-      */
- public String getswfPath() {  
- if (swfFile.exists()) {  
-             String tempString = swfFile.getPath();  
-             tempString = tempString.replaceAll("\\\\", "/");  
- return tempString;  
-         } else {  
- return"";  
-         }  
- 
-     }  
- /**
-      * 设置输出路径
-      */
- publicvoid setOutputPath(String outputPath) {  
- this.outputPath = outputPath;  
- if (!outputPath.equals("")) {  
-             String realName = fileName.substring(fileName.lastIndexOf("/"),  
-                     fileName.lastIndexOf("."));  
- if (outputPath.charAt(outputPath.length()) == '/') {  
-                 swfFile = new File(outputPath + realName + ".swf");  
-             } else {  
-                 swfFile = new File(outputPath + realName + ".swf");  
-             }  
-         }  
-     }  
- 
- }  
6.部署发布
启动tomcat，部署当前web应用
地址栏输入http://localhost:8080/ctcesims/documentUpload.jsp 如下图：
![](https://img-blog.csdn.net/20130513093459485)
单击选择文件，选择您要上传的文档，然后单击上传，处理完成后，打印如下信息，如下图所示：
![](https://img-blog.csdn.net/20130513093615107)
单击预览按钮，就会生成预览界面，如下图：
![](https://img-blog.csdn.net/20130513093702262)
**4.常见问题**
若出现swf无法预览，请访问
[http://www.macromedia.com/support/documentation/en/flashplayer/help/settings_manager04a.html#119065](http://www.macromedia.com/support/documentation/en/flashplayer/help/settings_manager04a.html#119065)
将生成swf的文件夹设置为信任文件位置。
## 以下给出 flexpaper 2.1.9 的不同之处：初始化方式改变，若文件目录与项目目录不在一起，可将附件目录在服务器中设置为虚拟目录
```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%
	//String swfFilePath=session.getAttribute("swfpath").toString();
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/flexpaper.js"></script>
<script type="text/javascript" src="js/flexpaper_handlers.js"></script>
<style type="text/css" media="screen"> 
			html, body	{ height:100%; }
			body { margin:0; padding:0; overflow:auto; }   
			#flashContent { display:none; }
        </style> 
<title>文档在线预览系统</title>
</head>
<body> 
        <div style="position:absolute;left:50px;top:10px;">
	       <div id="documentViewer" class="flexpaper_viewer" style="width:770px;height:500px"></div>
 
	        <script type="text/javascript"> 
	        var startDocument = "Paper";
	        $('#documentViewer').FlexPaperViewer(
	                { config : {
	                    SWFFile : 'upload/ddd3.swf',
	                    Scale : 0.6,
	                    ZoomTransition : 'easeOut',
	                    ZoomTime : 0.5,
	                    ZoomInterval : 0.2,
	                    FitPageOnLoad : true,
	                    FitWidthOnLoad : false,
	                    FullScreenAsMaxWindow : false,
	                    ProgressiveLoading : false,
	                    MinZoomSize : 0.2,
	                    MaxZoomSize : 5,
	                    SearchMatchAll : false,
	                    InitViewMode : 'Portrait',
	                    RenderingOrder : 'flash',
	                    StartAtPage : '',
	                    ViewModeToolsVisible : true,
	                    ZoomToolsVisible : true,
	                    NavToolsVisible : true,
	                    CursorToolsVisible : true,
	                    SearchToolsVisible : true,
	                    WMode : 'window',
	                    localeChain: 'en_US'
	                }}
	        );
	   
	        </script>            
        </div>
</body>
</html>
```
## 最后，若需要去除打印功能及logo，可重新编译 flexpaper 的 flash文件，网上也有下载
[FlexPaper参数](http://blog.csdn.net/z69183787/article/details/18659913)
