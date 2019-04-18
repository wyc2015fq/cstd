# struts2导出excel笔记 - z69183787的专栏 - CSDN博客
2016年02月25日 17:01:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1081
l例子很简单，就一个Action类和一个Service类
**1.所需jar包**
struts2需要jar包：commons-logging.jar、freemarker-2.3.8.jar、ognl-2.6.11.jar、struts2-core-2.0.11.1.jar、xwork-2.0.4.jar
excel导出：jxl.jar
**2.配置**
Web.xml代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
-   .....            
-              <filter>  
-     <filter-name>struts2</filter-name>  
-     <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>  
- </filter>  
- <filter-mapping>  
-     <filter-name>struts2</filter-name>  
-     <url-pattern>/*</url-pattern>  
- </filter-mapping>  
-  ......  
Struts.xml代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8"?>  
- <!DOCTYPE struts PUBLIC "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN""http://struts.apache.org/dtds/struts-2.0.dtd" >  
- <struts>  
-     <package name="platform-default" extends="struts-default">  
-         <action name="excel" class="action.ExcelAction">  
-             <result name="excel" type="stream">  
-                 <param name="contentType">  
-                     application/vnd.ms-excel  
-                 </param>  
-                 <param name="inputName">excelStream</param>  
-                 <param name="contentDisposition">  
-                     filename="export.xls"
-                 </param>  
-                 <param name="bufferSize">1024</param>  
-             </result>  
-         </action>  
-     </package>  
- </struts>  
**3.Action实现**
Excelaction代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- package action;  
- 
- import java.io.InputStream;  
- import service.IExcelService;  
- import service.impl.ExcelServiceImpl;  
- public class ExcelAction {  
- 
-     InputStream excelStream;  
- 
-     public String execute(){  
-         IExcelService es = new ExcelServiceImpl();  
-         excelStream = es.getExcelInputStream();  
-         return "excel";  
-     }  
-     //get set...  
- }  
**4.Service实现**
a.接口
Iexcelservice代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- package service;  
- import java.io.InputStream;  
- public interface IExcelService {  
-     InputStream getExcelInputStream();  
- }  
b.实现类
Excelserviceimpl.java代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- package service.impl;  
- 
- import java.io.ByteArrayInputStream;  
- import java.io.ByteArrayOutputStream;  
- import java.io.InputStream;  
- import java.io.OutputStream;  
- 
- import jxl.Workbook;  
- import jxl.write.WritableSheet;  
- import jxl.write.WritableWorkbook;  
- import service.IExcelService;  
- 
- public class ExcelServiceImpl implements IExcelService {  
- 
-     @Override  
-     public InputStream getExcelInputStream() {  
-         //将OutputStream转化为InputStream  
-         ByteArrayOutputStream out = new ByteArrayOutputStream();  
-         putDataOnOutputStream(out);  
-         return new ByteArrayInputStream(out.toByteArray());  
-     }  
- 
-     private void putDataOnOutputStream(OutputStream os) {  
-         jxl.write.Label label;  
-         WritableWorkbook workbook;  
-         try {  
-             workbook = Workbook.createWorkbook(os);  
-             WritableSheet sheet = workbook.createSheet("Sheet1", 0);  
- 
-             label = new jxl.write.Label(0, 0, "struts2导出excel");  
-             sheet.addCell(label);  
- 
-             workbook.write();  
-             workbook.close();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
 这样例子就完成了：
直接访问：http://127.0.0.1:8080/excel/excel.action 就可以导出excel
**说明：**
1、例子中OutputStream转InputStream是一种简单的实现，但是需要内存比较多，可以参考：
[http://ostermiller.org/convert_java_outputstream_inputstream.html](http://ostermiller.org/convert_java_outputstream_inputstream.html)
2、参考实现：[http://merrygrass.iteye.com/blog/558274](http://merrygrass.iteye.com/blog/558274)
3、[java报表JXL和POI打印设置](http://www.blogjava.net/Vincent-chenxj/articles/242512.html)
代码在附件中，jar包自己下载后导入即可
**问题：**
Java代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- //横向打印
- sheet.getSettings().setOrientation(PageOrientation.LANDSCAPE);  
- //默认行高（似乎不起作用）
- sheet.getSettings().setDefaultRowHeight(510);  
- //设置行宽
- sheet.getSettings().setDefaultColumnWidth(10);  
横向设置和行宽起作用，**但行高不起作用**，望高手回答
---------------------------------------------------------------------------------
**默认行高问题解决：**
此方法针对excel2003，在创建sheet后，添加如下代码
Java代码  ![收藏代码](http://wasw100.iteye.com/images/icon_star.png)
- for(int i=0;i<65536;i++){  
-     sheet.setRowView(i, 360);  
- }  
 补充：jxl中20个高度对应excel的1个高度，jxl的1个宽度对应excel的7个宽度
 --EOF--
