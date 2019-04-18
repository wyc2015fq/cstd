# Word/Excel/PDF文件转换成HTML整理 - z69183787的专栏 - CSDN博客
2013年12月06日 14:37:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：922
项目开发过程中，需求涉及到了各种文档转换为HTML或者网页易显示格式，现在将实现方式整理如下： 
**一、使用Jacob转换Word,Excel为HTML**
“JACOB一个Java-COM中间件.通过这个组件你可以在Java应用程序中调用COM组件和Win32 libraries。”
首先下载Jacob包，JDK1.5以上需要使用Jacob1.9版本（JDK1.6尚未测试），与先前的Jacob1.7差别不大
1、将压缩包解压后，Jacob.jar添加到Libraries中；
2、将Jacob.dll放至“WINDOWS\SYSTEM32”下面。
需要注意的是： 
【使用IDE启动Web服务器时，系统读取不到Jacob.dll，例如用MyEclipse启动Tomcat，就需要将dll文件copy到MyEclipse安装目录的“jre\bin”下面。 
一般系统没有加载到Jacob.dll文件时，报错信息为：“java.lang.UnsatisfiedLinkError: no jacob in java.library.path”】 
新建类：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicclass JacobUtil  
- {  
- publicstaticfinalint WORD_HTML = 8;  
- 
- publicstaticfinalint WORD_TXT = 7;  
- 
- publicstaticfinalint EXCEL_HTML = 44;  
- 
- /**
-      * WORD转HTML
-      * @param docfile WORD文件全路径
-      * @param htmlfile 转换后HTML存放路径
-      */
- publicstaticvoid wordToHtml(String docfile, String htmlfile)  
-     {  
-         ActiveXComponent app = new ActiveXComponent("Word.Application"); // 启动word
- try
-         {  
-             app.setProperty("Visible", new Variant(false));  
-             Dispatch docs = app.getProperty("Documents").toDispatch();  
-             Dispatch doc = Dispatch.invoke(  
-                     docs,  
- "Open",  
-                     Dispatch.Method,  
- new Object[] { docfile, new Variant(false),  
- new Variant(true) }, newint[1]).toDispatch();  
-             Dispatch.invoke(doc, "SaveAs", Dispatch.Method, new Object[] {  
-                     htmlfile, new Variant(WORD_HTML) }, newint[1]);  
-             Variant f = new Variant(false);  
-             Dispatch.call(doc, "Close", f);  
-         }  
- catch (Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
- finally
-         {  
-             app.invoke("Quit", new Variant[] {});  
-         }  
-     }  
- 
- /**
-      * EXCEL转HTML
-      * @param xlsfile EXCEL文件全路径
-      * @param htmlfile 转换后HTML存放路径
-      */
- publicstaticvoid excelToHtml(String xlsfile, String htmlfile)  
-     {  
-         ActiveXComponent app = new ActiveXComponent("Excel.Application"); // 启动word
- try
-         {  
-             app.setProperty("Visible", new Variant(false));  
-             Dispatch excels = app.getProperty("Workbooks").toDispatch();  
-             Dispatch excel = Dispatch.invoke(  
-                     excels,  
- "Open",  
-                     Dispatch.Method,  
- new Object[] { xlsfile, new Variant(false),  
- new Variant(true) }, newint[1]).toDispatch();  
-             Dispatch.invoke(excel, "SaveAs", Dispatch.Method, new Object[] {  
-                     htmlfile, new Variant(EXCEL_HTML) }, newint[1]);  
-             Variant f = new Variant(false);  
-             Dispatch.call(excel, "Close", f);  
-         }  
- catch (Exception e)  
-         {  
-             e.printStackTrace();  
-         }  
- finally
-         {  
-             app.invoke("Quit", new Variant[] {});  
-         }  
-     }  
- 
- }  
当时我在找转换控件时，发现网易也转载了一偏关于Jacob使用帮助，但其中出现了比较严重的错误：String htmlfile = "C:\\AA";
只指定到了文件夹一级，正确写法是String htmlfile = "C:\\AA\\xxx.html";
到此WORD/EXCEL转换HTML就已经差不多了，相信大家应该很清楚了:)
**二、使用XPDF将PDF转换为HTML**
1、下载xpdf最新版本，地址：[http://www.foolabs.com/xpdf/download.html](http://www.foolabs.com/xpdf/download.html)
我下载的是[xpdf-3.02pl2-win32.zip](ftp://ftp.foolabs.com/pub/xpdf/xpdf-3.02pl2-win32.zip)
2、下载中文支持包
我下载的是[xpdf-chinese-simplified.tar.gz](ftp://ftp.foolabs.com/pub/xpdf/xpdf-chinese-simplified.tar.gz)
3、下载pdftohtml支持包
地址：[http://sourceforge.net/projects/pdftohtml/](http://sourceforge.net/projects/pdftohtml/)
我下载的是：[pdftohtml-0.39-win32.tar.gz](http://downloads.sourceforge.net/pdftohtml/pdftohtml-0.39-win32.tar.gz?modtime=1154595549&big_mirror=0)
4、解压调试
1) 先将xpdf-3.02pl2-win32.zip解压，解压后的内容可根据需要进行删减，如果只需要转换为txt格式，其他的exe文件可以删除，只保留pdftotext.exe，以此类推；
2) 然后将xpdf-chinese-simplified.tar.gz解压到刚才xpdf-3.02pl2-win32.zip的解压目录；
3) 将pdftohtml-0.39-win32.tar.gz解压，pdftohtml.exe解压到xpdf-3.02pl2-win32.zip的解压目录；
4) 目录结构：
 +---[X:\xpdf]
           |-------各种转换用到的exe文件
           |
           |-------xpdfrc
           |
           +------[X:\xpdf\xpdf-chinese-simplified]
                                      |
                                      |
                                      +-------很多转换时需要用到的字符文件
xpdfrc：此文件是用来声明转换字符集对应路径的文件
5) 修改xpdfrc文件(文件原名为sample-xpdfrc)
修改文件内容为：
Txt代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- #----- begin Chinese Simplified support package  
- cidToUnicode    Adobe-GB1       xpdf-chinese-simplified\Adobe-GB1.cidToUnicode  
- unicodeMap      ISO-2022-CN     xpdf-chinese-simplified\ISO-2022-CN.unicodeMap  
- unicodeMap      EUC-CN          xpdf-chinese-simplified\EUC-CN.unicodeMap  
- unicodeMap  GBK    xpdf-chinese-simplified\GBK.unicodeMap  
- cMapDir         Adobe-GB1       xpdf-chinese-simplified\CMap  
- toUnicodeDir                    xpdf-chinese-simplified\CMap  
- fontDir  C:\WINDOWS\Fonts  
- displayCIDFontTT Adobe-GB1 C:\WINDOWS\Fonts\simhei.ttf  
- #----- end Chinese Simplified support package  
6) 创建bat文件pdftohtml.bat（放置的路径不能包含空格）
内容为：
Txt代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @echo off  
- set folderPath=%1
- set filePath=%2
- cd /d %folderPath%  
- pdftohtml -enc GBK %filePath%  
- exit  
7) 创建类
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicclass ConvertPdf  
- {  
- privatestatic String INPUT_PATH;  
- privatestatic String PROJECT_PATH;  
- 
- publicstaticvoid convertToHtml(String file, String project)  
-     {  
-         INPUT_PATH = file;  
-         PROJECT_PATH = project;  
- if(checkContentType()==0)  
-         {  
-             toHtml();  
-         }  
-     }  
- 
- privatestaticint checkContentType()  
-     {  
-         String type = INPUT_PATH.substring(INPUT_PATH.lastIndexOf(".") + 1, INPUT_PATH.length())  
-                 .toLowerCase();  
- if (type.equals("pdf"))  
- return0;  
- else
- return9;  
-     }  
- 
- privatestaticvoid toHtml()  
-     {  
- if(new File(INPUT_PATH).isFile())  
-         {  
- try
-             {  
-                 String cmd = "cmd /c start X:\\pdftohtml.bat \"" + PROJECT_PATH + "\" \"" + INPUT_PATH + "\"";  
-                 Runtime.getRuntime().exec(cmd);  
-             }  
- catch (IOException e)  
-             {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- }  
String cmd = "....";此处代码是调用创建的bat文件进行转换 
8) 测试转换
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args)  
- {  
-     ConvertPdf.convertToHtml("C:\\test.pdf", "X:\\xpdf");  
- }  
以上就是整理的内容，后续还会添加视频转换为FLV格式，如果需要的话:)
说的不是很详细，碰到问题的朋友可以自己努力解决一下:)
