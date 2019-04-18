# Java使用Jacob转换Word为HTML - z69183787的专栏 - CSDN博客
2013年12月06日 14:48:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1635
**从今天开始，我也要养成记录开发中遇到的问题和解决方法的好习惯！![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)**
最近开发一个Android项目，需要用到查看Word和Pdf文档的功能，由于Android没有直接显示Word和PDF文档的组件，只有一个Webview能查看html网页，所以决定将文档于服务器端转换为html，之后不论是在线预览还是下载到移动终端都可以直接查看了。
最近在网上查阅相关资料，找到利用Jacob来转换Word为html，除了占用CPU性能多一些，好像还不错(.doc和.docx都可以转换的！)。废话不多说，切入正题，这篇文章就先介绍转换Word为html的过程，Pdf还在研究当中，如果有结果我也会发出来！
“JACOB一个Java-COM中间件.通过这个组件你可以在Java应用程序中调用COM组件和Win32 libraries。”
**Ps：Jacob只能用于windows系统，如果你的系统不是windows，建议使用Openoffice.org，这个是跨平台的，虽然我没用，但是应该不麻烦，就是需要先安装Openoffice这个软件，然后使用8100服务。至于Poi，说实话，我真不爱用，那个需要先解析word，然后自己覆写成html，工作量大不说，还得不偿失，因为很难保证转换的html内容的格式与原来word文档格式一致，并且.docx转换也很费劲。**
1、到官网下载Jacob，目前最新版是1.17，地址链接：[http://sourceforge.net/projects/jacob-project/](http://sourceforge.net/projects/jacob-project/)
2、将压缩包解压后，Jacob.jar添加到Libraries中(先复制到项目目录中，右键单击jar包选择Build Path—>Add to Build Path)；
3、将Jacob.dll放至当前项目所用到的“jre\bin”下面(比如我的Eclipse正在用的Jre路径是D:\Java\jdk1.7.0_17\jre\bin)。
**Ps：我就是按照上面的步骤配置的，一点问题没有，但是有些人可能还会报错，比如：java.lang.UnsatisfiedLinkError: no jacob in java.library.path，这是系统没有加载到jacob.dll，网上解决方法是将Jacob.dll放至“WINDOWS\SYSTEM32”下面（我没试过，因为我的直接没问题）。**
**Java代码：**
**[java]**[view
 plain](http://blog.csdn.net/liugenghao3048/article/details/8771673#)[copy](http://blog.csdn.net/liugenghao3048/article/details/8771673#)
- publicclass JacobUtil {  
- 
- // 8 代表word保存成html
- publicstaticfinalint WORD_HTML = 8;   
- 
- publicstaticvoid main(String[] args) {  
-         String docfile = "C:\\Users\\無名\\Desktop\\xxx.doc";  
-         String htmlfile = "C:\\Users\\無名\\Desktop\\xxx.html";  
-         JacobUtil.wordToHtml(docfile, htmlfile);  
-     }  
- 
- /**  
-      * WORD转HTML  
-      * @param docfile WORD文件全路径  
-      * @param htmlfile 转换后HTML存放路径  
-      */
- publicstaticvoid wordToHtml(String docfile, String htmlfile)     
-     {     
- // 启动word应用程序(Microsoft Office Word 2003)
-         ActiveXComponent app = new ActiveXComponent("Word.Application");  
-         System.out.println("*****正在转换...*****");  
- try
-         {     
- // 设置word应用程序不可见  
-             app.setProperty("Visible", new Variant(false));    
- // documents表示word程序的所有文档窗口，（word是多文档应用程序）
-             Dispatch docs = app.getProperty("Documents").toDispatch();    
- // 打开要转换的word文件
-             Dispatch doc = Dispatch.invoke(     
-                     docs,     
- "Open",     
-                     Dispatch.Method,     
- new Object[] { docfile, new Variant(false),   
- new Variant(true) }, newint[1]).toDispatch();     
- // 作为html格式保存到临时文件
-             Dispatch.invoke(doc, "SaveAs", Dispatch.Method, new Object[] {     
-                     htmlfile, new Variant(WORD_HTML) }, newint[1]);     
- // 关闭word文件
-             Dispatch.call(doc, "Close", new Variant(false));     
-         }     
- catch (Exception e)     
-         {     
-             e.printStackTrace();     
-         }     
- finally
-         {     
- //关闭word应用程序
-             app.invoke("Quit", new Variant[] {});     
-         }   
-         System.out.println("*****转换完毕********");  
-     }  
- }  

 好了，这是我第一次发表博客，希望对大家有帮助！
