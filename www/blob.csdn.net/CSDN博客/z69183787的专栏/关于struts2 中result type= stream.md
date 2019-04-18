# 关于struts2 中result type="stream" - z69183787的专栏 - CSDN博客
2016年05月21日 14:07:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4118
struts2中 result="stream"，可以实现文件的下载功能。
这里有两篇比较清楚和基础的文章：[http://kin111.blog.51cto.com/738881/167475](http://kin111.blog.51cto.com/738881/167475)
[http://developer.51cto.com/art/200906/130122.htm](http://developer.51cto.com/art/200906/130122.htm)
我这里的应用是POI在内存中生成excel文件供客户端下载。struts.xml中的配置如下
**[html]**[view
 plain](http://blog.csdn.net/zhutulang/article/details/7106386?reload#)[copy](http://blog.csdn.net/zhutulang/article/details/7106386?reload#)
- <!-- 进销存信息查询统计 -->
- <actionname="jxcQuery_*"class="com.chengzhong.action.JxcChaXunAction"method="{1}">
- <resultname="success">jxcchaxun.jsp</result>
- <!--进销存信息导出为excel -->
- <resultname="toexcelOK"type="stream">
- <paramname="contentType">application/vnd.ms-excel</param>
- <paramname="contentDisposition">attachment;filename="${fileName}"</param>
- <paramname="inputName">excelIS</param>
- </result>
- <resultname="error">../404.htm</result>
- </action>
1.type="stream"           把一般内容输出到流
2.参数contentType的地方指定为application/vnd.ms-excel
3.设置为 attachment 将会告诉浏览器下载该文件，filename 指定下载文件保有存时的文件名，若未指定将会是以浏览的页面名作为文件名，如以 download.action 作为文件名。这里使用的是动态文件名，${fileName}。它将通过 Action 的 getFileName() 获得文件名。也就是说Action里面要有一个getFileName ()的方法。
    比如下面这个：       
**[java]**[view
 plain](http://blog.csdn.net/zhutulang/article/details/7106386?reload#)[copy](http://blog.csdn.net/zhutulang/article/details/7106386?reload#)
- public String getFileName() {          
-                   String fileName = "进销存信息统计表.xls"
- try {          
- //中文文件名需要转码为 ISO8859-1，否则乱码        
- returnnew String(fileName.getBytes(), "ISO8859-1");          
-          } catch (UnsupportedEncodingException e) {          
- return"jxc.xls";          
-          }          
-      }          
或者Action 里面有一个fileName的属性，提供它的set 和get 方法。在程序合适的位置给fileName赋值。这两者本质上应该是一样的。
我刚开始没有用注释处的方法转码，用中文做文件名的时候，它就象前面说的用浏览的页面名作为文件名了。
4.<param name="inputName">excelIS</param>
     参数inputName指定输入流的名称。和前面的文件名一样，Action里或者提供一个getExcelIS()的方法，或者提供excelIS属性。
5.对流的操作：
ByteArrayOutputStream baos = new ByteArrayOutputStream();
  try {
   /************************************************************/ 
    DataImportAndExportTool.InfoToExcel(Info, sheetName).write(baos);
    byte[] ba = baos.toByteArray(); 
    baos.close(); 
 excelIS = new ByteArrayInputStream(ba);       
   /*******************************************************************/
   //DataImportAndExportTool.InfoToExcel(Info, sheetName).write(fOut);     //这样excel表保存在了服务器上
  } catch (Exception e) {
   e.printStackTrace();
  }
   return "toexcelOK";、
DataImportAndExportTool.InfoToExcel(Info, sheetName)是我生成excel的方法，info是一个list，sheetName 是工作薄名，这句执行后生成了一个workbook。
ByteArrayOutputStream类是在创建它的实例时，程序内部创建一个byte型别数组的缓冲区，然后利用ByteArrayOutputStream和ByteArrayInputStream的实例向数组中写入或读出byte型数据。在网络传输中我们往往要传输很多变量，我们可以利用ByteArrayOutputStream把所有的变量收集到一起，然后一次性把数据发送出去。
ByteArrayOutputStream用途很多。其中缓冲是一个。对象的深浅复制也会用到
注意：以下是一些参数说明
**contentType**
内容类型，和互联网MIME标准中的规定类型一致，例如text/plain代表纯文本，text/xml表示XML，image/gif代表GIF图片，image/jpeg代表JPG图片
**inputName**
下载文件的来源流，对应着action类中某个类型为Inputstream的属性名，例如取值为*inputStream*的属性需要编写getInputStream()方法
**contentDisposition**
文件下载的处理方式，包括内联(inline)和附件(attachment)两种方式，而附件方式会弹出文件保存对话框，否则浏览器会尝试直接显示文件。取值为：
*attachment;filename="struts2.txt"*，表示文件下载的时候保存的名字应为*struts2.txt*。如果直接写*filename="struts2.txt"*，那么默认情况是代表*inline*，浏览器会尝试自动打开它，等价于这样的写法：*inline; filename="struts2.txt"*
**bufferSize**
下载缓冲区的大小
。在这里面，**contentType**属性和**contentDisposition**分别对应着HTTP响应中的头*Content-Type*和*Content-disposition*头。
