# struts2 文件上传与下载原理 - z69183787的专栏 - CSDN博客
2013年12月16日 10:25:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1428
**文件上传的原理：**
　　表单元素的enctype属性指定的是表单数据的编码方式，该属性有3个值：
　　1) application/x-www-form-urlencoded：这是默认编码方式，它只处理表单域里的value属性值，采用这种编码方式的表单会将表单域的
值处理成URL编码方式。
　　2) multipart/form-data：这种编码方式的表单会以二进制流的方式来处理表单数据，这种编码方式会把文件域指定文件的内容也封装到
请求参数里。
　　3) text/plain：这种方式主要适用于直接通过表单发送邮件的方式。
　　文件上传是web应用经常用到的一个知识。原理是，通过为表单元素设置enctype=”multipart/form-data”属性，让表单提交的数据以二
进制编码的方式提交，在接收此请求的Servlet中用二进制流来获取内容，就可以取得上传文件的内容，从而实现文件的上传。
　　在Java领域中，有两个常用的文件上传项目：一个是Apache组织Jakarta的Common-FileUpload组件
([url]http://commons.apache.org/fileupload/[/url])，另一个是Oreilly组织的COS框架([url]http://www.servlets.com/cos/[/url])。利用这两个框架都能很方便
的实现文件的上传。
**==================================Struts2上传文件**
增加commons-fileupload-1.2.jar和commons-io-1.3.1.jar到lib
**=====jsp=====**
form 的 enctype 设置为 multipart/form-data
**==============UploadAction==============**private String username;
private String password;
private File file; // 对应文件域
private String fileFileName; // 前面的File属性的名字 + FileName（固定的）
private String fileContent; // 前面的File属性的名字 + Content
// setter... getter...
String execute() throws Exception {
InputStream is = new FileInputStream( file );
String root = ServletActionContext.getRequest().getRealPath("/upload");
File destFile = new File(root,this.getFileFileName());
OutputStream os = new FileOutputStream( destFile );
byte[] buffer = new byte[400];
int length = 0;
while( (length = is.read(buffer)) > 0 ) {
os.write(buffer,0,length);
}
is.close();
os.close();
return SUCCESS;
}
**=================中文问题=================**
不过我全部用UTF-8并未遇到中文问题
struts2-core包
struts-default.xml ----拦截器的设置
org.apache.struts2.default.properties ----全局属性的设置
33行 strusts.i18n.encoding=UTF-8 默认UTF-8
可以在struts.xml下进行设置
<struts>
设置字符集
**<constant name="struts.i18n.encoding" value="gbk"/>**
设置上传文件缓存
**<constant name="struts.multipart.saveDir" value="c:/"/>**
</struts>
其他属性
**struts.multipart.parser=jakarta** struts2采用那种方式上传
pell
cos
**struts.multipart.maxSize=2097152** 默认上传文件最大的请求大小2M
**struts.action.extension=action** 整个url的后缀名
**================上传多个文件================**
有两种方式：
**1.数组**
File[] file 文件
String[] fileFileName 文件名
String[] fileContentType 文件类型
**2.集合**
List<File> file 
List<String> fileFileName
List<String> fileContentType
**--------action中：--------**
String execute() {
for(int i = 0; i < file.size(); i++) {
InputStream is = new FileInputStream(file.get(i));
String root = ServletActionContext.getRequest().getRealPath("/upload");
File destFile = new File(root,this.getFileFileName().get(i)); 
... 
}
return SUCCESS;
}
**------jsp中：------**
多个file时，file的名字要一致，都要叫file，它会自动set到跟file名对应的List中去
<s:file name="file" />
<s:file name="file" />
<s:file name="file" />
**========================上传任意个文件========================**
<td id="more">
<input type="button" value="添加" />
</td>
**------JS:------**
funcation addMore() {
var td = document.getElementById("more");
//生成一个换行符
var br = document.createElement("br");
//创建一个input组件
var input = document.createElement("input");
var button = document.createElement("input");
//指定类型 为 file 为文件上传
input.type = "file";
//指定组件的名字
input.name = "file";
button.type = "button";
button.value = "删除";
//为删除按钮注册一个事件 
button.onclick = function() {
//alert("删除按钮");
//删除一行
td.removeChild(br);
td.removeChild(input);
td.removeChild(button);
}
//将创建的组件加到<td>中
td.appendChild(br);
td.appendChild(input);
td.appendChild(button);
}
**=======================限制上传类型=======================**org.apache.struts2.interceptor.FileUploadInterceptor类
Long **maximumSize**：最大上传大小---每一个文件的大小，不是总和
String **allowedTypes**：允许的类型
**-------------struts.xml-------------**
<struts>
<action ...>
<result name="input">/upload.jsp</result>
<result .../>
加入一个上传文件的拦截器并设置其属性
<interceptor-ref name="**fileUpload**">
**<param name="maximumSize">409600</param>** 单个上传文件最大不能超过400K
**<param name="allowedTypes">...</param>** mime类型，多个用逗号分开
</interceptor-ref>
** 加入默认的拦截器
<interceptor-ref name="**defaultStack**" />
</action> 
</struts>
注：后缀可以到tomcat/conf/web.xml中找<mime-type>中的字符串
**--------------upload.jsp--------------**
添加<s:fielderror />
**----------------------更改显示的错误信息----------------------**
org.apache.struts2中 找到struts-messages.properties
-----------------------
**上传文件类型不匹配**
struts.messages.error.content.type.not.allowed=Content-Type not allowed: {0} "{1}" {2}
-----------------------
**上传文件大小超出规定**struts.messages.error.file.too.large=File too large: {0} "{1}" {2}
-----------------------
**上传文件出错**struts.messages.error.uploading=Error uploading: {0}
创建一个全局的属性文件 /src/**messages**.properties
**struts.messages.error.content.type.not.allowed**=不支持上传该类型的文件
**struts.messages.error.file.too.large**=上传文件过大，请重试
**struts.messages.error.uploading**=上传文件时发生错误
**---------国际化---------**
<constant name="**struts.custom.i18n.resources**" value="**messages**"/>
messages_en_US.properties
messages_zh_CN.properties
**==============================下载==============================**
处理下载的类：org.apache.struts2.dispatcher.**StreamResult**
== 属性 ==
String **contentType** = "text/plain"; 
String **contentLength**;
String **contentDisposition** = "inline"; 
String **inputName** = "inputStream"; 
InputStream **inputStream**; 
int **bufferSize** = 1024;
== 说明 ==
**contentType**
内容类型，和互联网MIME标准中的规定类型一致，例如text/plain代表纯文本，text/xml表示XML，image/gif代表GIF图片，image/jpeg代表JPG图片
用来做动态文件下载的，事先并不知道未来的文件类型是什么，那么我们可以把它的值设置成为：application/octet-stream;charset=ISO8859-1 ，注意一定要加入charset，否则某些时候会导致下载的文件出错
**inputName**
下载文件的来源流，对应着action类中某个类型为Inputstream的属性名，例如取值为inputStream的属性需要编写getInputStream()方法
**contentDisposition**
文件下载的处理方式，包括**内联**(**inline**)和**附件**(**attachment**)两种方式，而附件方式会弹出文件保存对话框，否则浏览器会尝试直接显示文件。取值为：attachment;filename="struts2.txt"，表示文件下载的时候保存的名字应为struts2.txt。如果直接写filename="struts2.txt"，那么默认情况是代表inline，浏览器会尝试自动打开它，等价于这样的写法：inline;
 filename="struts2.txt"
**bufferSize**
下载缓冲区的大小
**#** contentType属性和contentDisposition分别对应着HTTP响应中的头Content-Type和Content-disposition头。
如：
HTTP头内容：
HTTP/1.1 200 OK 
Server: Apache-Coyote/1.1 
Content-disposition: attachment;filename="struts2.txt" 
Content-Type: text/plain 
Transfer-Encoding: chunked 
Date: Sun, 02 Mar 2008 02:58:25 GMT
**----------action----------**
Class DownloadAction extends ActionSupport {
private String path;
// setter... getter...
//必须返回一个输入流，该流是让用户下载的
public **InputStream** getDownloadFile() {
//从某个文件获得流 --这里是获得项目root下upload下的文件
//也可以 new FileInputStream("c:/test.text");
return ServletActionContext.getServletContext().getResourceAsStream("/upload/struts2.ppt");
}
public String execute() throws Exception {
return SUCCESS;
}
}
**-----------struts.xml-----------**
<action name="download" class="org.scorpio.jh.struts2.upload.action.DownloadAction">
**<!-- 依赖注入文件路径 -->**<param name="path">/download/xhtml.txt</param>
**<!-- 设置结果类型为 流 -->**<result name="success" type="***stream***">
**<!-- 设置内容类型 -->**<param name="***contentType***">text/plain</param>
**<!-- 设置下载文件的名字 attachment：作为附件，filename=：指定下载文件名-->**<param name="***contentDisposition***">attachment;filename="xhtml.txt"</param>
**<!-- 设置下载文件的输入流对应的方法 downloadFile对应DownloadAction中的getDownloadFile()-->**<param name="***inputName***">downloadFile</param>
**<!-- 指定下载文件的缓冲大小 -->**
<param name="***bufferSize***">4096</param>
</result>
</action>
**==========================解决下载文件名中文问题==========================**
1.在下载action获取文件名的方法中先进行转码然后再返回
path = new String( path.getBytes(), "ISO-8859-1" );
2.xml配置文件动态的获取path的值
<param name="contentDisposition">attachment;filename="**${path}**"</param>
${path} 用于动态的获取所配置的action中path成员的值，相当于调用getPath()方法
3. /*解决中文乱码问题，设置后产生一个新的String对象此对象以改变了编码*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         String newpath=URLEncoder.encode(path,"utf-8");
**-------action-------**private String path;
public String getPath() {
try { //转换成西欧字符集
**path = new String( path.getBytes(), "ISO-8859-1" );**} catch (UnsupportedEncodingException e) {
e.printStackTrace();
} 
return path;
}
public void setPath(String path) {
this.path = path;
}
**---------------struts.xml---------------**
<action name="download" class="org.scorpio.jh.struts2.upload.action.DownloadAction">
<param name="path">/download/wmlscript实例.txt</param>
<result name="success" type="stream">
<param name="contentType">text/plain</param>
<!-- 动态的获取 DownloadAction的path属性 -->
<param name="contentDisposition">attachment;filename="${path}"</param>
<param name="inputName">downloadFile</param>
<param name="bufferSize">4096</param>
</result>
</action>
**=================安全隐患=================**
访问者如果精通Struts 2的话，它可能使用这样的带有表单参数的地址来访问：
[[url]http://localhost:8080/struts2hello/download3.action?inputPath=/WEB-INF/web.xml[/url]](http://localhost:8080/struts2hello/download3.action?inputPath=/WEB-INF/web.xml)，这样的结果就是下载后的文件内容是您系统里面的web.xml的文件的源代码，甚至还可以用这种方式来下载任何其它JSP文件的源码。这对系统安全是个很大的威胁。作为一种变通的方法，读者最好是从数据库中进行路径配置，然后把Action类中的设置inputPath的方法统统去掉，简言之就是删除这个方法定义：
public void setPath(String path) {
this.path = path;
}
而实际情况则应该成为 download.action?fileid=1 类似于这样的形式来进行。或者呢，读者可以在execute()方法中进行路径检查，如果发现有访问不属于download下面文件的代码，就一律拒绝，不给他们返回文件内容。例如，我们可以把刚才类中的execute()方法加以改进，成为这样：
public String execute() throws Exception {
// 文件下载目录路径
String downloadDir = ServletActionContext.getServletContext().getRealPath("/download");
// 文件下载路径
String downloadFile = ServletActionContext.getServletContext().getRealPath(inputPath);
java.io.File file = new java.io.File(downloadFile);
downloadFile = file.getCanonicalPath();// 真实文件路径,去掉里面的..等信息
// 发现企图下载不在 /download 下的文件, 就显示空内容
if(!downloadFile.startsWith(downloadDir)) {
return null;
}
return SUCCESS;
}
这时候如果访问者再企图下载web.xml的内容，它只能得到一个空白页，现在访问者只能下载位于/download目录下的文件
其他的一些资料：
Struts 2中实现文件上传
[[url]http://www.blogjava.net/max/archive/2007/03/21/105124.html[/url]](http://www.blogjava.net/max/archive/2007/03/21/105124.html)
Struts 2中实现文件下载（修正中文问题）
[[url]http://www.blogjava.net/beansoft/archive/2008/03/03/183468.html[/url]](http://www.blogjava.net/beansoft/archive/2008/03/03/183468.html%5B/url%5D)
**附:contentType类型.**
'ez' => 'application/andrew-inset', 
'hqx' => 'application/mac-binhex40', 
'cpt' => 'application/mac-compactpro', 
'doc' => 'application/msword', 
'bin' => 'application/octet-stream', 
'dms' => 'application/octet-stream', 
'lha' => 'application/octet-stream', 
'lzh' => 'application/octet-stream', 
'exe' => 'application/octet-stream', 
'class' => 'application/octet-stream', 
'so' => 'application/octet-stream', 
'dll' => 'application/octet-stream', 
'oda' => 'application/oda', 
'pdf' => 'application/pdf', 
'ai' => 'application/postscript', 
'eps' => 'application/postscript', 
'ps' => 'application/postscript', 
'smi' => 'application/smil', 
'smil' => 'application/smil', 
'mif' => 'application/vnd.mif', 
'xls' => 'application/vnd.ms-excel', 
'ppt' => 'application/vnd.ms-powerpoint', 
'wbxml' => 'application/vnd.wap.wbxml', 
'wmlc' => 'application/vnd.wap.wmlc', 
'wmlsc' => 'application/vnd.wap.wmlscriptc', 
'bcpio' => 'application/x-bcpio', 
'vcd' => 'application/x-cdlink', 
'pgn' => 'application/x-chess-pgn', 
'cpio' => 'application/x-cpio', 
'csh' => 'application/x-csh', 
'dcr' => 'application/x-director', 
'dir' => 'application/x-director', 
'dxr' => 'application/x-director', 
'dvi' => 'application/x-dvi', 
'spl' => 'application/x-futuresplash', 
'gtar' => 'application/x-gtar', 
'hdf' => 'application/x-hdf', 
'js' => 'application/x-javas
cript', 
'skp' => 'application/x-koan', 
'skd' => 'application/x-koan', 
'skt' => 'application/x-koan', 
'skm' => 'application/x-koan', 
'latex' => 'application/x-latex', 
'nc' => 'application/x-netcdf', 
'cdf' => 'application/x-netcdf', 
'sh' => 'application/x-sh', 
'shar' => 'application/x-shar', 
'swf' => 'application/x-shockwave-flash', 
'sit' => 'application/x-stuffit', 
'sv4cpio' => 'application/x-sv4cpio', 
'sv4crc' => 'application/x-sv4crc', 
'tar' => 'application/x-tar', 
'tcl' => 'application/x-tcl', 
'tex' => 'application/x-tex', 
'texinfo' => 'application/x-texinfo', 
'texi' => 'application/x-texinfo', 
't' => 'application/x-troff', 
'tr' => 'application/x-troff', 
'roff' => 'application/x-troff', 
'man' => 'application/x-troff-man', 
'me' => 'application/x-troff-me', 
'ms' => 'application/x-troff-ms', 
'ustar' => 'application/x-ustar', 
'src' => 'application/x-wais-source', 
'xhtml' => 'application/xhtml+xml', 
'xht' => 'application/xhtml+xml', 
'zip' => 'application/zip', 
'au' => 'audio/basic', 
'snd' => 'audio/basic', 
'mid' => 'audio/midi', 
'midi' => 'audio/midi', 
'kar' => 'audio/midi', 
'mpga' => 'audio/mpeg', 
'mp2' => 'audio/mpeg', 
'mp3' => 'audio/mpeg', 
'aif' => 'audio/x-aiff', 
'aiff' => 'audio/x-aiff', 
'aifc' => 'audio/x-aiff', 
'm3u' => 'audio/x-mpegurl', 
'ram' => 'audio/x-pn-realaudio', 
'rm' => 'audio/x-pn-realaudio', 
'rpm' => 'audio/x-pn-realaudio-plugin', 
'ra' => 'audio/x-realaudio', 
'wav' => 'audio/x-wav', 
'pdb' => 'chemical/x-pdb', 
'xyz' => 'chemical/x-xyz', 
'bmp' => 'image/bmp', 
'gif' => 'image/gif', 
'ief' => 'image/ief', 
'jpeg' => 'image/jpeg', 
'jpg' => 'image/jpeg', 
'jpe' => 'image/jpeg', 
'png' => 'image/png', 
'tiff' => 'image/tiff', 
'tif' => 'image/tiff', 
'djvu' => 'image/vnd.djvu', 
'djv' => 'image/vnd.djvu', 
'wbmp' => 'image/vnd.wap.wbmp', 
'ras' => 'image/x-cmu-raster', 
'pnm' => 'image/x-portable-anymap', 
'pbm' => 'image/x-portable-bitmap', 
'pgm' => 'image/x-portable-graymap', 
'ppm' => 'image/x-portable-pixmap', 
'rgb' => 'image/x-rgb', 
'xbm' => 'image/x-xbitmap', 
'xpm' => 'image/x-xpixmap', 
'xwd' => 'image/x-xwindowdump', 
'igs' => 'model/iges', 
'iges' => 'model/iges', 
'msh' => 'model/mesh', 
'mesh' => 'model/mesh', 
'silo' => 'model/mesh', 
'wrl' => 'model/vrml', 
'vrml' => 'model/vrml', 
'css' => 'text/css', 
'html' => 'text/html', 
'htm' => 'text/html', 
'asc' => 'text/plain', 
'txt' => 'text/plain', 
'rtx' => 'text/richtext', 
'rtf' => 'text/rtf', 
'sgml' => 'text/sgml', 
'sgm' => 'text/sgml', 
'tsv' => 'text/tab-separated-values', 
'wml' => 'text/vnd.wap.wml', 
'wmls' => 'text/vnd.wap.wmlscript', 
'etx' => 'text/x-setext', 
'xsl' => 'text/xml', 
'xml' => 'text/xml', 
'mpeg' => 'video/mpeg', 
'mpg' => 'video/mpeg', 
'mpe' => 'video/mpeg', 
'qt' => 'video/quicktime', 
'mov' => 'video/quicktime', 
'mxu' => 'video/vnd.mpegurl', 
'avi' => 'video/x-msvideo', 
'movie' => 'video/x-sgi-movie', 
'ice' => 'x-conference/x-cooltalk'
