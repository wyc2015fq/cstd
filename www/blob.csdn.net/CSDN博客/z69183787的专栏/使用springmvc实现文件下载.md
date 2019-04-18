# 使用springmvc实现文件下载 - z69183787的专栏 - CSDN博客
2018年06月30日 20:57:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：264
https://blog.csdn.net/actionzh/article/details/77103754
文件下载比较简单，直接在页面给出了一个url，该url的属性包括要下载文件的文件名以及路径，就可以实现文件下载了。但是如果该文件的文件名为中文文件名，在某些早起的浏览器上就会导致下载失败；如果使用最新的Firefox、Chrome、Opera、Safari则都可以正常下载文件名为中文的文件了。
SpringMVC提供了一个ResponseEntity类型，使用它可以很方便地定义返回的HttpHeaders和HttpStatus。以下文件下载代码：
@RequestMapping(value ="/DownloadFile")
public ResponseEntity<byte[]> DownloadFile(HttpServletRequestreq, HttpServletResponseresp)throws ServletException, IOException {
//  接受的是UTF-8
req.setCharacterEncoding("utf-8");
//获取项目根目录
String path="D:\\tomcat\\apache-tomcat-7.0.52\\apache-tomcat-7.0.52\\wtpwebapps\\AjaxUpload\\反馈报告.pdf";
//获取文件名
String filename="反馈报告.pdf";
File file = null;
HttpHeaders headers =null;
try {
System.out.println(filename);//myfiles
file =new File(path);
//请求头
headers =new HttpHeaders();
StringfileName1 =new String(filename.getBytes("UTF-8"),"iso-8859-1");//解决文件名乱码
//通知浏览器以attachment（下载方式）打开图片
headers.setContentDispositionFormData("attachment",fileName1);
//application/octet-stream二进制流数据（最常见的文件下载）。
headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
} catch (Exceptione) {
thrownew RuntimeException(e);
}
returnnew ResponseEntity<byte[]>(FileUtils.readFileToByteArray(file),headers, HttpStatus.OK);
DownloadFil处理方法接收页面传递的文件名filename和路径后，使用Apache Commons FileUpload组件的FileUtils读取项目的文件，并将其构建成ResponseEntity对象返回客户端下载。使用ResponseEntity对象，可以很方便的定义返回的HttpHeaders和HttpStatus。上面代码中的MediaType，代表的是Internet Media Type，即互联网媒体类型，也叫做MIME类型。在Http协议消息头中，使用Content-Type来表示具体请求中的媒体类型信息。HttpStatus类型代表的是Http协议中的状态（这里有一个问题就是，这个http状态网上很多文章都是用的`HttpStatus.CREATED``，如果你用的火狐或谷歌浏览器那么一点问题都没有，然而使用IE浏览器是无法下载的，这个问题弄了好久才得以解决，下面是我的测试）。`
`1.  ``IE``浏览器使用HttpStatus.CREATED：`
![](https://img-blog.csdn.net/20170811221521092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aW9uemg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.*IE*`浏览器`使用*HttpStatus.OK：*
*![](https://img-blog.csdn.net/20170811221535922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aW9uemg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*
由此可见文件下载的Http协议中的状态最好使用`HttpStatus.OK。`
