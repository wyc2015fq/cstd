# POI3.8和jxl读取Excel例子 - z69183787的专栏 - CSDN博客
2013年08月09日 16:15:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3337
这几天在弄java读excel的问题，以前用的是jxl.jar来读的，后来发现无法读取2007的，于是研究了一下POI，在网上下了最新版的poi-bin-3.8-beta5-20111217.zip，同时也参考了网上其它人写的代码，在jboss7服务器下成功读出，发demo吧。
使用方法：jboss服务器或tomcat服务器，将下面的jsp文件保存到某个域名下，同时在此文件所在的目录中新建一个upload目录，然后将要读取的test.xls和test.xlsx放到upload目录下，然后运行jsp文件即可看到结果：
（1）用poi.jar来读取excel2003
必须要用到的jar文件：
poi-3.8-beta5-20111217.jar
poi-ooxml-3.8-beta5-20111217.jar
poi-ooxml-schemas-3.8-beta5-20111217.jar
xmlbeans-2.3.0.jar
dom4j-1.6.1.jar
jsr173_1.0_api.jar  （如果jdk是1.5版本则需要此jar，如果是1.6以上版本则不需要）
为了方便测试，代码为jsp，如下：
**[html]**[view
 plain](http://blog.csdn.net/hegel_su/article/details/7195119#)[copy](http://blog.csdn.net/hegel_su/article/details/7195119#)
- <%@ page contentType="text/html; charset=gb2312"language="java"import="java.io.*"errorPage="" %>
- <%@ page import="java.io.*,org.apache.poi.poifs.filesystem.POIFSFileSystem,org.apache.poi.hssf.record.*,org.apache.poi.hssf.model.*,org.apache.poi.hssf.usermodel.*,org.apache.poi.hssf.util.*" %>
- <%request.setCharacterEncoding("gb2312");%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=gb2312"/>
- <title>用poi.jar来读取excel2003</title>
- </head>
- <bodybgcolor="#F2F8FF">
- 
- <%  
- try{  
-   String file = application.getRealPath("/") + "upload\\test.xls";  
-   POIFSFileSystem fs = new POIFSFileSystem(new FileInputStream(file));  
-   HSSFWorkbook xwb = new HSSFWorkbook(fs);  
-   //读取第一个sheet  
-   HSSFSheet sheet = xwb.getSheetAt(0);  
- 
-   //定义 row、cell  
-   HSSFRow row;  
-   HSSFCell cell;  
-   String output;  
-   int rowNum;  
-   int cellNum;  
- 
-   //循环输出表格中的内容  
- output = "";  
- rowNum = sheet.getPhysicalNumberOfRows();  
- 
-   for (int i = 0; i <rowNum; i++)  
-   {  
- row = sheet.getRow(i);  
- cellNum = row.getPhysicalNumberOfCells();  
-     for (int j = 0; j <cellNum; j++)  
-     {  
- cell = row.getCell(j);  
- output = output + cell.getStringCellValue() + "   ";  
-     }  
- output = output + "<br/>";  
-   }  
- 
-   out.print(output);  
- 
- }catch(Exception e){  
-   out.print(e.toString());  
- }  
- 
- %>
- 
- </body>
- </html>
（2）用poi读取excel2007的代码
所需jar文件同上
**[html]**[view
 plain](http://blog.csdn.net/hegel_su/article/details/7195119#)[copy](http://blog.csdn.net/hegel_su/article/details/7195119#)
- <%@ page contentType="text/html; charset=gb2312"language="java"import="java.io.*"errorPage="" %>
- <%@ page import="org.apache.poi.xssf.usermodel.XSSFRow,org.apache.poi.xssf.usermodel.XSSFSheet,org.apache.poi.xssf.usermodel.XSSFWorkbook,org.apache.poi.xssf.usermodel.XSSFCell" %>
- <%request.setCharacterEncoding("gb2312");%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=gb2312"/>
- <title>用poi.jar来读取excel2007</title>
- </head>
- <bodybgcolor="#F2F8FF">
- 
- <%  
- try{  
-   String file = application.getRealPath("/") + "upload\\test.xlsx";  
-   XSSFWorkbook xwb = new XSSFWorkbook(new FileInputStream(file));  
-   //读取第一个sheet  
-   XSSFSheet sheet = xwb.getSheetAt(0);  
- 
-   //定义 row、cell  
-   XSSFRow row;  
-   XSSFCell cell;  
-   String output;  
-   int rowNum;  
-   int cellNum;  
- 
-   //循环输出表格中的内容  
- output = "";  
- rowNum = sheet.getLastRowNum();  
- 
-   for (int i = 0; i <= rowNum; i++)  
-   {  
- row = sheet.getRow(i);  
- cellNum = row.getPhysicalNumberOfCells();  
-     for (int j = 0; j <cellNum; j++)  
-     {  
- cell = row.getCell(j);  
- output = output + cell.getStringCellValue() + "   ";  
-     }  
- output = output + "<br/>";  
-   }  
- 
-   out.print(output);  
- 
- }catch(Exception e){  
-   out.print(e.toString());  
- }  
- 
- %>
- 
- </body>
- </html>
（3）用jxl.jar读取excel2003的代码
需要jxl.jar即可
**[html]**[view
 plain](http://blog.csdn.net/hegel_su/article/details/7195119#)[copy](http://blog.csdn.net/hegel_su/article/details/7195119#)
- <%@ page contentType="text/html; charset=gb2312"language="java"import="java.io.*"errorPage="" %>
- <%@ page import="jxl.*" %>
- <%request.setCharacterEncoding("gb2312");%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=gb2312"/>
- <title>用jxl.jar来读取excel2003</title>
- </head>
- <bodybgcolor="#F2F8FF">
- 
- <%  
- try{  
-   String file = application.getRealPath("/") + "upload\\test.xls";  
-   Workbook workbook = Workbook.getWorkbook(new File(file));  
-   Sheet sheet = workbook.getSheet(0);  
-   Cell c1 = null;  
-   String s1 = "";  
- 
- c1 = sheet.getCell(0,0);  
- s1 = c1.getContents();  
- 
-   workbook.close();  
- 
-   out.print(s1);  
- 
- }catch(Exception e){  
-   out.print(e.toString());  
- }  
- 
- %>
- 
- </body>
- </html>
 相关jar库及代码资源下载：
[http://download.csdn.net/detail/uixor_/4016423](http://download.csdn.net/detail/uixor_/4016423)
