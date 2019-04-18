# struts2文件下载及 inputStream的理解 - z69183787的专栏 - CSDN博客
2014年04月29日 10:28:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4923
struts.xml文件配置：
**[html]**[view
 plain](http://blog.csdn.net/wnczwl369/article/details/7483290#)[copy](http://blog.csdn.net/wnczwl369/article/details/7483290#)
- <spanstyle="font-size:16px;"><?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE struts PUBLIC  
-     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"  
-     "http://struts.apache.org/dtds/struts-2.0.dtd">
- 
- <struts>
- <constantname="struts.enable.DynamicMethodInvocation"value="false"/>
- <constantname="struts.devMode"value="true"/>
- 
- <packagename="app14a"extends="struts-default">
- <actionname="Menu">
- <result>/jsp/Menu.jsp</result>
- </action>
- <actionname="ViewCss"class="app14a.FileDownloadAction">
- <resultname="success"type="stream">
- <paramname="inputName">inputStream</param>
- <paramname="contentType">text/css</param>
- <paramname="contentDisposition">filename="main11.css"</param>
- <paramname="bufferSize">2048</param>
- </result>
- </action>
- <actionname="DownloadCss"class="app14a.FileDownloadAction">
- <resultname="success"type="stream">
- <paramname="inputName">inputStream</param>
- <paramname="contentType">application/octet-stream</param>
- <paramname="contentDisposition">filename="main111222.css"</param>
- <paramname="bufferSize">2048</param>
- </result>
- </action>
- </package>
- </struts></span>
**    action特殊的地方在于result的类型是一个流（stream），配置stream类型的结果时，因为无需指定实际的显示的物理资源，所以无需指定location属性，只需要指定inputName属性，该属性指向被下载文件的来源，对应着Action类中的某个属性，类型为InputStream,，struts2会寻找一个返回类型为InputStream的方法getInputStream()（方法名最好为getInputStream,有人说此方法名可以为别的可能需要与struts.xml中的inputName保存一致下面有解释，不过我实验的结果不是这样，须为getInputStream，可能是org.apache.struts2.dispatcher.ResultStream类限制造成,感兴趣的可以看看此类的源码，就会明白的），得到InputStream。**
**    另一种理解：action中定义一个返回InputStream的方法，该方法作为被下载文件的入口，且需要配置stream类型结果时指定inputName参数，inputName参数的值就是方法去掉get前缀、首字母小写的字符串。**
下面则列出了和下载有关的一些参数列表： 
参数说明 
contentType 
内容类型，和互联网MIME标准中的规定类型一致，例如text/plain代表纯文本，text/xml表示XML，image/gif代表GIF图片，image/jpeg代表JPG图片 
inputName 
下载文件的来源流，对应着action类中某个类型为Inputstream的属性名，例如取值为inputStream的属性需要编写getInputStream()方法 
contentDisposition 
文件下载的处理方式，包括内联(inline)和附件(attachment)两种方式，而附件方式会弹出文件保存对话框，否则浏览器会尝试直接显示文件。取值为： 
attachment;filename="struts2.txt"，表示文件下载的时候保存的名字应为struts2.txt。如果直接写filename="struts2.txt"，那么默认情况是代表inline，浏览器会尝试自动打开它，等价于这样的写法：inline; filename="struts2.txt" 
bufferSize 
下载缓冲区的大小
