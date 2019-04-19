# java实现附件预览(openoffice+swftools+flexpaper) - 零度的博客专栏 - CSDN博客
2016年07月05日 10:09:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：3706
### 1、概述
####     1.1 主要原理：
         1.通过第三方工具openoffice，将word、excel、ppt、txt等文件转换为pdf文件
         2.通过swfTools将pdf文件转换成swf格式的文件
         3.通过FlexPaper文档组件在页面上进行展示
####     1.2 安装包下载
1.openoffice是Apache下的一个开放免费的文字处理软件
下载地址：[Apache
 oppenoffice  官网下载 版本-3.4.1](http://www.openoffice.org/zh-cn/download/)
2.SWFTools是一组用来处理Flash的swf文件的工具包，我们使用它将pdf文件转成swf文件!
下载地址：[SWFTools官网下载
 swftools-2013-04-09-1007.exe](http://www.swftools.org/download.html)
3.FlexPaper是一个开源轻量级的在浏览器上显示各种文档的组件
下载地址：[FlexPaper官网下载
 版本1.5.1](http://flexpaper.devaldi.com/download/)
4.JODConverter一个Java的OpenDocument 文件转换器，该工具包会用到调用openoffice，实现将word、excel、ppt、txt等文件转换为pdf文件。在此我们只用到它的jar包。
下载地址：[JODCConverter下载](http://sourceforge.net/projects/jodconverter/files/)
####    1.3 安装文件
1.将所下载的文件（JODConverter除外）进行安装，盘符可随自己设定！需要注意的是在openoffice安装完成后，当我们使用它时，需将它的服务打开。在次我们需要以命令的方式打开:
打开dos窗口，进入openoffice安装盘符，输入以下代码来启动服务：
soffice -headless -accept="socket,host=127.0.0.1,port=8100;urp;" -nofirststartwizard
注意最后一个命令前边的‘—’,可不要写错！服务起不来，项目可是继续不下去的哦.
官网启动服务截图如下：
![](https://img-blog.csdn.net/20130509175206145)
本地截图：
![](https://img-blog.csdn.net/20130509175251332)
### 2、开发过程
####         2.1 新建项目
        将flexpaper文件中的js文件夹(包含了flexpaper_flash_debug.js，flexpaper_flash.js,jquery.js,这三个js文件主要是预览swf文件的插件)拷贝至网站根目录;将FlexPaperViewer.swf拷贝至网站根目录下(该文件主要是用在网页中播放swf文件的播放器)，目录结构如下图：
![](https://img-blog.csdn.net/20130509175618717)
注：需创建upload文件夹
#### 2.2 创建fileUpload.jsp
```
<%@ page language="java" contentType="text/html; charset=UTF-8"  pageEncoding="UTF-8"%>  
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">  
<html>  
<head>  
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">  
<title>文档在线预览系统</title>  
<style>  
    body {margin-top:100px;background:#fff;font-family: Verdana, Tahoma;}  
    a {color:#CE4614;}  
    #msg-box {color: #CE4614; font-size:0.9em;text-align:center;}  
    #msg-box .logo {border-bottom:5px solid #ECE5D9;margin-bottom:20px;padding-bottom:10px;}  
    #msg-box .title {font-size:1.4em;font-weight:bold;margin:0 0 30px 0;}  
    #msg-box .nav {margin-top:20px;}  
</style>  
  
</head>  
<body>  
<div id="msg-box">  
    <form name="form1"  method="post" enctype="multipart/form-data" action="docUploadConvertAction.jsp">  
        <div class="title">  
            请上传要处理的文件，过程可能需要几分钟，请稍候片刻。  
        </div>  
        <p>  
            <input name="file1" type="file">  
        </p>  
        <p>  
            <input type="submit" name="Submit" value="上传">  
        </p>  
   </form >  
</div>  
</body>  
</html>
```
####         2.3 创建转换页docUploadConvertAction.jsp
```
01.<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>  
02.  
03.<%@page import="java.io.*"%>  
04.<%@page import="java.util.Enumeration"%>  
05.<%@page import="com.oreilly.servlet.MultipartRequest"%>  
06.<%@page import="com.oreilly.servlet.multipart.DefaultFileRenamePolicy"%>  
07.<%@page import="com.cectsims.util.DocConverter"%>  
08.<%  
09.//文件上传采用cos组件上传，可更换为commons-fileupload上传，文件上传后，保存在upload文件夹  
10.//获取文件上传路径  
11.String saveDirectory =application.getRealPath("/")+"upload";  
12.//打印上传路径信息  
13.System.out.println(saveDirectory);  
14.//每个文件最大50m  
15.int maxPostSize = 50 * 1024 * 1024 ;  
16.//采用cos缺省的命名策略，重名后加1,2,3...如果不加dfp重名将覆盖  
17.DefaultFileRenamePolicy dfp = new DefaultFileRenamePolicy();  
18.//response的编码为"UTF-8",同时采用缺省的文件名冲突解决策略,实现上传,如果不加dfp重名将覆盖  
19.MultipartRequest multi = new MultipartRequest(request, saveDirectory, maxPostSize,"UTF-8",dfp);  
20.//MultipartRequest multi = new MultipartRequest(request, saveDirectory, maxPostSize,"UTF-8");  
21.//输出反馈信息  
22. Enumeration files = multi.getFileNames();  
23.     while (files.hasMoreElements()) {  
24.        System.err.println("ccc");  
25.       String name = (String)files.nextElement();  
26.       File f = multi.getFile(name);  
27.       if(f!=null){  
28.         String fileName = multi.getFilesystemName(name);  
29.         //获取上传文件的扩展名  
30.         String extName=fileName.substring(fileName.lastIndexOf(".")+1);  
31.         //文件全路径  
32.         String lastFileName= saveDirectory+"\\" + fileName;  
33.         //获取需要转换的文件名,将路径名中的'\'替换为'/'  
34.         String converfilename = saveDirectory.replaceAll("\\\\", "/")+"/"+fileName;  
35.         System.out.println(converfilename);  
36.         //调用转换类DocConverter,并将需要转换的文件传递给该类的构造方法  
37.         DocConverter d = new DocConverter(converfilename);  
38.         //调用conver方法开始转换，先执行doc2pdf()将office文件转换为pdf;再执行pdf2swf()将pdf转换为swf;  
39.         d.conver();  
40.         //调用getswfPath()方法，打印转换后的swf文件路径  
41.         System.out.println(d.getswfPath());  
42.         //生成swf相对路径，以便传递给flexpaper播放器  
43.         String swfpath = "upload"+d.getswfPath().substring(d.getswfPath().lastIndexOf("/"));  
44.         System.out.println(swfpath);  
45.         //将相对路径放入sessio中保存  
46.         session.setAttribute("swfpath", swfpath);  
47.         out.println("上传的文件:"+lastFileName);  
48.         out.println("文件类型"+extName);  
49.         out.println("<hr>");  
50.       }  
51.     }  
52.  
53.%>  
54.<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">  
55.<html>  
56.<head>  
57.<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">  
58.<title>Insert title here</title>  
59.<style>  
60.    body {margin-top:100px;background:#fff;font-family: Verdana, Tahoma;}  
61.    a {color:#CE4614;}  
62.    #msg-box {color: #CE4614; font-size:0.9em;text-align:center;}  
63.    #msg-box .logo {border-bottom:5px solid #ECE5D9;margin-bottom:20px;padding-bottom:10px;}  
64.    #msg-box .title {font-size:1.4em;font-weight:bold;margin:0 0 30px 0;}  
65.    #msg-box .nav {margin-top:20px;}  
66.</style>  
67.</head>  
68.<body>  
69.    <div>  
70.        <form name="viewForm" id="form_swf" action="documentView.jsp" method="POST">  
71.            <input type='submit' value='预览' class='BUTTON SUBMIT'/>  
72.        </form>  
73.    </div>  
74.</body>  
75.</html>
```
####         2.4 创建查看页documentView.jsp
```
01.<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>  
02.<%  
03.    String swfFilePath=session.getAttribute("swfpath").toString();  
04.%>  
05.<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">  
06.<html>  
07.<head>  
08.<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">  
09.<script type="text/javascript" src="js/jquery.js"></script>  
10.<script type="text/javascript" src="js/flexpaper_flash.js"></script>  
11.<script type="text/javascript" src="js/flexpaper_flash_debug.js"></script>  
12.<style type="text/css" media="screen">   
13.            html, body  { height:100%; }  
14.            body { margin:0; padding:0; overflow:auto; }     
15.            #flashContent { display:none; }  
16.        </style>   
17.  
18.<title>文档在线预览系统</title>  
19.</head>  
20.<body>   
21.        <div style="position:absolute;left:50px;top:10px;">  
22.            <a id="viewerPlaceHolder" style="width:820px;height:650px;display:block"></a>  
23.              
24.            <script type="text/javascript">   
25.                var fp = new FlexPaperViewer(     
26.                         'FlexPaperViewer',  
27.                         'viewerPlaceHolder', { config : {  
28.                         SwfFile : escape('<%=swfFilePath%>'),  
29.                         Scale : 0.6,   
30.                         ZoomTransition : 'easeOut',  
31.                         ZoomTime : 0.5,  
32.                         ZoomInterval : 0.2,  
33.                         FitPageOnLoad : true,  
34.                         FitWidthOnLoad : false,  
35.                         FullScreenAsMaxWindow : false,  
36.                         ProgressiveLoading : false,  
37.                         MinZoomSize : 0.2,  
38.                         MaxZoomSize : 5,  
39.                         SearchMatchAll : false,  
40.                         InitViewMode : 'SinglePage',  
41.                           
42.                         ViewModeToolsVisible : true,  
43.                         ZoomToolsVisible : true,  
44.                         NavToolsVisible : true,  
45.                         CursorToolsVisible : true,  
46.                         SearchToolsVisible : true,  
47.                          
48.                         localeChain: 'en_US'  
49.                         }});  
50.            </script>              
51.        </div>  
52.</body>  
53.</html>
```
####         2.5 创建转换类DocConverter.java
```
01.package com.cectsims.util;  
02.import java.io.BufferedInputStream;  
03.import java.io.File;  
04.import java.io.IOException;  
05.import java.io.InputStream;  
06.  
07.import com.artofsolving.jodconverter.DocumentConverter;  
08.import com.artofsolving.jodconverter.openoffice.connection.OpenOfficeConnection;  
09.import com.artofsolving.jodconverter.openoffice.connection.SocketOpenOfficeConnection;  
10.import com.artofsolving.jodconverter.openoffice.converter.OpenOfficeDocumentConverter;  
11.  
12./** 
13. * doc docx格式转换 
14. */  
15.public class DocConverter {  
16.    private static final int environment = 1;// 环境 1：windows 2:linux  
17.    private String fileString;// (只涉及pdf2swf路径问题)  
18.    private String outputPath = "";// 输入路径 ，如果不设置就输出在默认的位置  
19.    private String fileName;  
20.    private File pdfFile;  
21.    private File swfFile;  
22.    private File docFile;  
23.      
24.    public DocConverter(String fileString) {  
25.        ini(fileString);  
26.    }  
27.  
28.    /** 
29.     * 重新设置file 
30.     *  
31.     * @param fileString 
32.     */  
33.    public void setFile(String fileString) {  
34.        ini(fileString);  
35.    }  
36.  
37.    /** 
38.     * 初始化 
39.     *  
40.     * @param fileString 
41.     */  
42.    private void ini(String fileString) {  
43.        this.fileString = fileString;  
44.        fileName = fileString.substring(0, fileString.lastIndexOf("."));  
45.        docFile = new File(fileString);  
46.        pdfFile = new File(fileName + ".pdf");  
47.        swfFile = new File(fileName + ".swf");  
48.    }  
49.      
50.    /** 
51.     * 转为PDF 
52.     *  
53.     * @param file 
54.     */  
55.    private void doc2pdf() throws Exception {  
56.        if (docFile.exists()) {  
57.            if (!pdfFile.exists()) {  
58.                OpenOfficeConnection connection = new SocketOpenOfficeConnection(8100);  
59.                try {  
60.                    connection.connect();  
61.                    DocumentConverter converter = new OpenOfficeDocumentConverter(connection);  
62.                    converter.convert(docFile, pdfFile);  
63.                    // close the connection  
64.                    connection.disconnect();  
65.                    System.out.println("****pdf转换成功，PDF输出：" + pdfFile.getPath()+ "****");  
66.                } catch (java.net.ConnectException e) {  
67.                    e.printStackTrace();  
68.                    System.out.println("****swf转换器异常，openoffice服务未启动！****");  
69.                    throw e;  
70.                } catch (com.artofsolving.jodconverter.openoffice.connection.OpenOfficeException e) {  
71.                    e.printStackTrace();  
72.                    System.out.println("****swf转换器异常，读取转换文件失败****");  
73.                    throw e;  
74.                } catch (Exception e) {  
75.                    e.printStackTrace();  
76.                    throw e;  
77.                }  
78.            } else {  
79.                System.out.println("****已经转换为pdf，不需要再进行转化****");  
80.            }  
81.        } else {  
82.            System.out.println("****swf转换器异常，需要转换的文档不存在，无法转换****");  
83.        }  
84.    }  
85.      
86.    /** 
87.     * 转换成 swf 
88.     */  
89.    @SuppressWarnings("unused")  
90.    private void pdf2swf() throws Exception {  
91.        Runtime r = Runtime.getRuntime();  
92.        if (!swfFile.exists()) {  
93.            if (pdfFile.exists()) {  
94.                if (environment == 1) {// windows环境处理  
95.                    try {  
96.                        Process p = r.exec("D:/Program Files/SWFTools/pdf2swf.exe "+ pdfFile.getPath() + " -o "+ swfFile.getPath() + " -T 9");  
97.                        System.out.print(loadStream(p.getInputStream()));  
98.                        System.err.print(loadStream(p.getErrorStream()));  
99.                        System.out.print(loadStream(p.getInputStream()));  
100.                        System.err.println("****swf转换成功，文件输出："  
101.                                + swfFile.getPath() + "****");  
102.                        if (pdfFile.exists()) {  
103.                            pdfFile.delete();  
104.                        }  
105.  
106.                    } catch (IOException e) {  
107.                        e.printStackTrace();  
108.                        throw e;  
109.                    }  
110.                } else if (environment == 2) {// linux环境处理  
111.                    try {  
112.                        Process p = r.exec("pdf2swf " + pdfFile.getPath()  
113.                                + " -o " + swfFile.getPath() + " -T 9");  
114.                        System.out.print(loadStream(p.getInputStream()));  
115.                        System.err.print(loadStream(p.getErrorStream()));  
116.                        System.err.println("****swf转换成功，文件输出："  
117.                                + swfFile.getPath() + "****");  
118.                        if (pdfFile.exists()) {  
119.                            pdfFile.delete();  
120.                        }  
121.                    } catch (Exception e) {  
122.                        e.printStackTrace();  
123.                        throw e;  
124.                    }  
125.                }  
126.            } else {  
127.                System.out.println("****pdf不存在,无法转换****");  
128.            }  
129.        } else {  
130.            System.out.println("****swf已经存在不需要转换****");  
131.        }  
132.    }  
133.  
134.    static String loadStream(InputStream in) throws IOException {  
135.  
136.        int ptr = 0;  
137.        in = new BufferedInputStream(in);  
138.        StringBuffer buffer = new StringBuffer();  
139.  
140.        while ((ptr = in.read()) != -1) {  
141.            buffer.append((char) ptr);  
142.        }  
143.  
144.        return buffer.toString();  
145.    }  
146.    /** 
147.     * 转换主方法 
148.     */  
149.    @SuppressWarnings("unused")  
150.    public boolean conver() {  
151.  
152.        if (swfFile.exists()) {  
153.            System.out.println("****swf转换器开始工作，该文件已经转换为swf****");  
154.            return true;  
155.        }  
156.  
157.        if (environment == 1) {  
158.            System.out.println("****swf转换器开始工作，当前设置运行环境windows****");  
159.        } else {  
160.            System.out.println("****swf转换器开始工作，当前设置运行环境linux****");  
161.        }  
162.        try {  
163.            doc2pdf();  
164.            pdf2swf();  
165.        } catch (Exception e) {  
166.            e.printStackTrace();  
167.            return false;  
168.        }  
169.  
170.        if (swfFile.exists()) {  
171.            return true;  
172.        } else {  
173.            return false;  
174.        }  
175.    }  
176.  
177.    /** 
178.     * 返回文件路径 
179.     *  
180.     * @param s 
181.     */  
182.    public String getswfPath() {  
183.        if (swfFile.exists()) {  
184.            String tempString = swfFile.getPath();  
185.            tempString = tempString.replaceAll("\\\\", "/");  
186.            return tempString;  
187.        } else {  
188.            return "";  
189.        }  
190.  
191.    }  
192.    /** 
193.     * 设置输出路径 
194.     */  
195.    public void setOutputPath(String outputPath) {  
196.        this.outputPath = outputPath;  
197.        if (!outputPath.equals("")) {  
198.            String realName = fileName.substring(fileName.lastIndexOf("/"),  
199.                    fileName.lastIndexOf("."));  
200.            if (outputPath.charAt(outputPath.length()) == '/') {  
201.                swfFile = new File(outputPath + realName + ".swf");  
202.            } else {  
203.                swfFile = new File(outputPath + realName + ".swf");  
204.            }  
205.        }  
206.    }  
207.  
208.}
```
####         2.6 部署发布
        启动tomcat，部署当前web应用
        地址栏输入[http://localhost:8080/ctcesims/documentUpload.jsp](http://localhost:8080/ctcesims/documentUpload.jsp) 如下图：
![](https://img-blog.csdn.net/20130513093459485)
单击选择文件，选择您要上传的文档，然后单击上传，处理完成后，打印如下信息，如下图所示：
![](https://img-blog.csdn.net/20130513093615107)
单击预览按钮，就会生成预览界面，如下图：
![](https://img-blog.csdn.net/20130513093702262)
**4.常见问题**
       若出现swf无法预览，请访问:[http://www.macromedia.com/support/documentation/en/flashplayer/help/settings_manager04a.html#119065](http://www.macromedia.com/support/documentation/en/flashplayer/help/settings_manager04a.html#119065)
将生成swf的文件夹设置为信任文件位置。
        以下给出 flexpaper 2.1.9 的不同之处：初始化方式改变，若文件目录与项目目录不在一起，可将附件目录在服务器中设置为虚拟目录
```
1.<%@ page language="java" contentType="text/html; charset=UTF-8"  
2.    pageEncoding="UTF-8"%>  
3.<%  
4.    //String swfFilePath=session.getAttribute("swfpath").toString();  
5.%>  
6.<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">  
7.<html>  
8.<head>  
9.<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">  
10.<script type="text/javascript" src="js/jquery.min.js"></script>  
11.<script type="text/javascript" src="js/flexpaper.js"></script>  
12.<script type="text/javascript" src="js/flexpaper_handlers.js"></script>  
13.<style type="text/css" media="screen">   
14.            html, body  { height:100%; }  
15.            body { margin:0; padding:0; overflow:auto; }     
16.            #flashContent { display:none; }  
17.        </style>   
18.  
19.<title>文档在线预览系统</title>  
20.</head>  
21.<body>   
22.        <div style="position:absolute;left:50px;top:10px;">  
23.           <div id="documentViewer" class="flexpaper_viewer" style="width:770px;height:500px"></div>  
24.   
25.            <script type="text/javascript">   
26.  
27.            var startDocument = "Paper";  
28.  
29.            $('#documentViewer').FlexPaperViewer(  
30.                    { config : {  
31.  
32.                        SWFFile : 'upload/ddd3.swf',  
33.  
34.                        Scale : 0.6,  
35.                        ZoomTransition : 'easeOut',  
36.                        ZoomTime : 0.5,  
37.                        ZoomInterval : 0.2,  
38.                        FitPageOnLoad : true,  
39.                        FitWidthOnLoad : false,  
40.                        FullScreenAsMaxWindow : false,  
41.                        ProgressiveLoading : false,  
42.                        MinZoomSize : 0.2,  
43.                        MaxZoomSize : 5,  
44.                        SearchMatchAll : false,  
45.                        InitViewMode : 'Portrait',  
46.                        RenderingOrder : 'flash',  
47.                        StartAtPage : '',  
48.  
49.                        ViewModeToolsVisible : true,  
50.                        ZoomToolsVisible : true,  
51.                        NavToolsVisible : true,  
52.                        CursorToolsVisible : true,  
53.                        SearchToolsVisible : true,  
54.                        WMode : 'window',  
55.                        localeChain: 'en_US'  
56.                    }}  
57.            );  
58.         
59.            </script>              
60.        </div>  
61.</body>  
62.</html>
```
```
<span style="font-family:Arial;font-size:12px;"><%@ page language="java" contentType="text/html; charset=UTF-8"
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
</html></span>
```
        最后，若需要去除打印功能及logo，可重新编译 flexpaper 的 flash文件，网上也有下载。
### 3、[FlexPaper参数](http://blog.csdn.net/z69183787/article/details/18659913)
Flexpaper可能用到如下参数
||**SwfFile***(String)*|需要使用Flexpaper打开的文档|
|----|----|----|
||**Scale***(Number)*|初始化缩放比例，参数值应该是大于零的整数|
||**ZoomTransition***(String)*|Flexpaper中缩放样式，它使用和Tweener一样的样式，默认参数值为easeOut.其他可选值包括: easenone, easeout, linear, easeoutquad|
||**ZoomTime***(Number)*|从一个缩放比例变为另外一个缩放比例需要花费的时间，该参数值应该为0或更大。|
||**ZoomInterval***(Number)*|缩放比例之间间隔，默认值为0.1，该值为正数。|
||**FitPageOnLoad***(Boolean)*|初始化得时候自适应页面，与使用工具栏上的适应页面按钮同样的效果。|
||**FitWidthOnLoad***(Boolean)*|初始化的时候自适应页面宽度，与工具栏上的适应宽度按钮同样的效果。|
||**localeChain***(String)*|设置地区（语言），目前支持以下语言。 en_US (English) fr_FR (French) zh_CN (Chinese, Simple) es_ES (Spanish) pt_BR (Brazilian Portugese) ru_RU (Russian) fi_FN (Finnish) de_DE (German) nl_NL (Netherlands) tr_TR (Turkish) se_SE (Swedish) pt_PT (Portugese) el_EL (Greek) da_DN (Danish) cz_CS (Czech) it_IT (Italian) pl_PL (Polish) pv_FN (Finnish) hu_HU (Hungarian)|
||**FullScreenAsMaxWindow***(Boolean)*|当设置为true的时候，单击全屏按钮会打开一个flexpaper最大化的新窗口而不是全屏，当由于flash播放器因为安全而禁止全屏，而使用flexpaper作为独立的flash播放器的时候设置为true是个优先选择。|
||**ProgressiveLoading***(Boolean)*|当设置为true的时候，展示文档时不会加载完整个文档，而是逐步加载，但是需要将文档转化为9以上的flash版本（使用pdf2swf的时候使用-T 9 标签）。|
||**MaxZoomSize***(Number)*|设置最大的缩放比例。|
||**MinZoomSize***(Number)*|最小的缩放比例。|
||**SearchMatchAll***(Boolean)*|设置为true的时候，单击搜索所有符合条件的地方高亮显示。|
||**InitViewMode***(String)*|设置启动模式如"Portrait" or "TwoPage".|
||**ViewModeToolsVisible***(Boolean)*|工具栏上是否显示样式选择框。|
||**ZoomToolsVisible***(Boolean)*|工具栏上是否显示缩放工具。|
||**NavToolsVisible***(Boolean)*|工具栏上是否显示导航工具。|
||**CursorToolsVisible***(Boolean)*|工具栏上是否显示光标工具。|
||**SearchToolsVisible***(Boolean)*|工具栏上是否显示搜索。|
