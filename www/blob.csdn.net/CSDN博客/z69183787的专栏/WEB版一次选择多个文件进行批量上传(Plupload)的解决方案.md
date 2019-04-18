# WEB版一次选择多个文件进行批量上传(Plupload)的解决方案 - z69183787的专栏 - CSDN博客
2015年06月30日 18:02:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4476
# 说明：Plupload支持多种浏览器，多种上传方式！
      一般的WEB方式文件上传只能使用FileUpload控件进行一个文件一个文件的进行上传，就算是批量上传，也要把文件一个一个的添加到页面，无法如 windows程序一样，一次选择多个文件进行批量上传。这样在某些应用上就显得很不人性化，有时候客户希望能够一次选择很多文件，然后让系统把选择的文 件全部上传。本人在2010年时使用swfupload为核心进行文件的批量上传的解决方案。见文章：[WEB版一次选择多个文件进行批量上传(swfupload)的解决方案](http://www.cnblogs.com/chillsrc/archive/2010/02/21/1670594.html)。
     由于上一个swfupload示例对于IE9的兼容性的不太好，以及随着HTML5与IE9、IE10的逐渐普及，加上鉴于swfupload已经很久没有进行过更新等等原因，要对批量文件上传的方案要进行更新，使用新的解决方案。于是在经过一阵寻找之后，决定使用plupload做为核心重新写了一个批量文件上传解决方案。
    Plupload 是一个Web浏览器上的界面友好的文件上传组件，可显示上传进度、图像自动缩略和上传分块，同时上传多个文件。Plupload 的上传文件的引擎使用Flash，Silverlight，HTML5，Gears，BrowserPlus或正常的FileUpload。
     Plupload组件是由TinyMCE开发者开发的Web文件上传组件,是一个高度可用的上传处理组件方便您添加到您的内容管理系统或类似的系统。Plupload目前分为核心API和一个jQuery上传队列组件，这使您可以使用现在组件或自己编写自己的自定义实现。Plupload使用GPLv2许可。
 关于Plupload的功能说明：
|Feature|Flash|Gears|HTML 5|Silverlight|BrowserPlus|HTML 4|
|----|----|----|----|----|----|----|
|Chunking|√|√|√1|√|√|X|
|Drag/Drop|X|√|√2|X|√|X|
|PNG Resize|√|√|√3|√|√|X|
|JPEG Resize|√|√|√3|√|√|X|
|Type filtering|√|√|√4|√|√|X|
|Stream upload|√|√|√|√|X|X|
|Multipart upload|√|√|√5|√|√|√|
|File size restriction|√|√|√|√|√|X|
|Upload progress|√|√|√|√|√|X|
|Custom headers|√|√|√|√|X|X|
关于上表中的一些注意点说明：
1.Chunking is properly supported in Chrome. On most part in Firefox 4+.
   分块功能支持的浏览器有：chrome,firefox 4以上版本
2.Drag/drop support of files is currently only available in Firefox and WebKit. Safari on Windows has some strange problems and requires workaround.
文件拖放功能目录只适用于Firefox和WebKit。Safari在Windows上有一些奇怪的问题,需要解决。
3.Image resizing is only possible on Firefox 3.5+ (with fixed quality) and Chrome. Safari/Opera doesn't support direct data access to the selected files.
图像缩放目前只是支持Firefox 3.5 +(固定质量)和Chrome两个浏览器。Safari和Opera不支持直接访问选定的文件。
4.File type filtering is currently not supported by all browsers. But we fill the HTML 5 accept attribute so once the support is there it will work.
文件类型过滤是目前所有的浏览器都不支持。但是我们设置了HTML 5接受属性所以一旦浏览器支持，它就会工作。
5. Multipart upload is only supported in Gecko and WebKit.
多部分上传仅支持在Gecko和WebKit。
      关于Plupload的一些说明：
      1) Plupload使用jQuery的组件做为选择文件和上传文件的队列组件。
      2) Plupload使用Flash，Silverlight，HTML5，Gears，BrowserPlus、FileUpload上传文件技术引擎。
      3) Plupload允许自定义使用Plupload核心API来进行选择文件与上传文件。
      4) JavaScript用来激活文件选择对话框。此文件选择对话框是可以设置允许用户选择一个单独的文件或者是多个文件。 选择的的文件类型也是可以被限制的，因此用户只能选择指定的适当的文件，例如jgp;gif。
      5)  Plupload允许对上传过程中的一些事件进行自定义，写上自己的处理方式。
      6)  选定的文件的上传和它所在页面、表单是独立的。每个文件都是单独上传的，这就保证了服务端脚本能够在一个时间点更容易地处理单个文件。具体信息可以访问Plupload官方网站：http://www.plupload.com/
一： 下面就是利用Plupload 组件，让客户一次选择多个文件，然后将选择的文件上传到服务器的批量文件解决方案。
让我们先来看看客户端的界面效果图。（多选文件，批量上传，上传进度显示）
1) 显示界面：
![](http://images.cnitblog.com/blog/10343/201301/30203434-2504d4d0a3b34fd086f08414c2ea458a.png)
图1
2) 进行多文件选择：
![](http://images.cnitblog.com/blog/10343/201301/30203518-480bc3548f0b4326b6752d7c775b1e27.png)
 图2
3) 上传进度显示
![](http://images.cnitblog.com/blog/10343/201301/30203547-3b5dcff06bbc481297d29749b7bb1ea0.png)
                                       图3
![](http://images.cnitblog.com/blog/10343/201301/30203655-7b5a89b347684e3a9c4b51eb37b46562.png)
                       图4
二：具体的代码与操作步骤：
第一步，要进行下面的过程，必须先准备好Plupload组件。
1) Plupload:大家可以访问Plupload官方网站：http://www.plupload.com/，在这个网站上可以下载到组件与demo。
第二步,创建应用的目录结构，我这个示例的目录结构如图：
1.主要目录结构
![](http://images.cnitblog.com/blog/10343/201301/30203726-7071fa46aabe496d90c0ede8ec9b2bd7.png)
2. 文件上传用到的js脚本文件。
![](http://images.cnitblog.com/blog/10343/201301/30203736-33b501ce98f640e5b246f75f042c6d2c.png)
3. jquery.ui.plupload目录中的文件，这个文件实际上就是界面中的显示信息。
![](http://images.cnitblog.com/blog/10343/201301/30203750-ed5469e688934d2cbd86f6b6d40cbfb6.png)
第三步，前台部分准备客户操作的WEB界面，如下[WebUploadFileTest2.aspx、uploadFiles.ashx]
1) 前台客户端代码，其中WebUploadFileTest2.aspx的代码如下，WebUploadFileTest2.aspx.cs文件中只使用默认的代码，不用添加任何代码。
WebUploadFileTest2.aspx
Html代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="WebUploadFileTest2.aspx.cs" Inherits="WebApplication1.WebUploadFileTest2" %>
 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    
   <title>多文件上传</title>
 
   <!-- Load Queue widget CSS and jQuery -->
<style type="text/css">@import url(Scripts/jquery.ui.plupload/css/jquery.ui.plupload.css);</style>
 
 
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jqueryui/1.8.23/jquery-ui.min.js"></script>
<link rel="stylesheet" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.23/themes/base/jquery-ui.css"/>
<!-- Third party script for BrowserPlus runtime (Google Gears included in Gears runtime now) -->
<script type="text/javascript" src="http://bp.yahooapis.com/2.4.21/browserplus-min.js"></script>
 
<!-- Load plupload and all it's runtimes and finally the jQuery UI queue widget -->
<script type="text/javascript" src="Scripts/plupload.full.js"></script>
<script type="text/javascript" src="Scripts/jquery.ui.plupload/jquery.ui.plupload.js"></script>
 
<script type="text/javascript">
    // Convert divs to queue widgets when the DOM is ready
    $(function () {
        $("#uploader").plupload({
            // General settings
            runtimes: 'gears,flash,silverlight,browserplus,html5', // 这里是说用什么技术引擎
            url: 'uploadFiles.ashx', // 服务端上传路径
            max_file_size: '10mb', // 文件上传最大限制。
            chunk_size: '1mb', // 上传分块每块的大小，这个值小于服务器最大上传限制的值即可。
            unique_names: true, // 上传的文件名是否唯一
 
            // Resize images on clientside if we can
            //// 是否生成缩略图（仅对图片文件有效）
            resize: { width: 320, height: 240, quality: 90 },
 
            // Specify what files to browse for
            ////  这个数组是选择器，就是上传文件时限制的上传文件类型
            filters: [
{ title: "Image files", extensions: "jpg,gif,png" },
{ title: "Zip files", extensions: "zip,rar,7z" }
],
 
            // Flash settings
            // plupload.flash.swf 的所在路径
            flash_swf_url: 'Scripts/plupload.flash.swf',
 
            // Silverlight settings
            // silverlight所在路径
            silverlight_xap_url: 'Scripts/plupload.silverlight.xap'
        });
 
        // Client side form validation
        $('form').submit(function (e) {
            var uploader = $('#uploader').plupload('getUploader');
 
            // Files in queue upload them first
            if (uploader.files.length > 0) {
                // When all files are uploaded submit form
                uploader.bind('StateChanged', function () {
                    if (uploader.files.length === (uploader.total.uploaded + uploader.total.failed)) {
                        $('form')[0].submit();
                    }
                });
 
                uploader.start();
            } else
                alert('You must at least upload one file.');
 
            return false;
        });
    });
</script>
 
</head>
<body>
    <form id="form1" runat="server">
    <div>
     <h2>一次选择多个文件进行上传</h2>
    </div>
     <div id="uploader" style="width: 600px">
        <p>You browser doesn't have Flash, Silverlight, Gears, BrowserPlus or HTML5 support.</p>
    </div>
    </form>
</body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
以上代码最后的显示结果如图:
![](http://images.cnitblog.com/blog/10343/201301/30203927-8b22fa72156643eca7d53a7471bc7d6c.png)
                                     图8.
2)后台服务器端代码：uploadFiles.ashx文件中使用默认的代码，不需要添加任何代码。uploadFiles.ashx.cs文件的代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    /// <summary>
    /// uploadFiles 的摘要说明
    /// </summary>
    public class uploadFiles : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
            context.Response.ContentType = "text/plain";
            context.Response.Write("Hello World");
            UploadFile(context);
        }
        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
        public void UploadFile(HttpContext context)
        {
            context.Response.CacheControl = "no-cache";
            string s_rpath = FileHelper.GetUploadPath();//@"E:\My Documents\Visual Studio 2008\WebSites\SWFUpload\demos\applicationdemo.net";
            string Datedir = DateTime.Now.ToString("yy-MM-dd");
            string updir = s_rpath + "\\" + Datedir;
            string extname = string.Empty;
            string fullname = string.Empty;
            string filename = string.Empty;
            if (context.Request.Files.Count > 0)
            {
                try
                {
                    for (int j = 0; j < context.Request.Files.Count; j++)
                    {
                        
                        HttpPostedFile uploadFile = context.Request.Files[j];
                        int offset =Convert.ToInt32( context.Request["chunk"]); //当前分块
                        int total =Convert.ToInt32(context.Request["chunks"]);//总的分块数量
                        string name = context.Request["name"];
                        //文件没有分块
                        if (total == 1)
                        {
                            if (uploadFile.ContentLength > 0)
                            {
                                if (!Directory.Exists(updir))
                                {
                                    Directory.CreateDirectory(updir);
                                }
                                 extname = Path.GetExtension(uploadFile.FileName);
                                 fullname = DateTime.Now.Year.ToString() + DateTime.Now.Month.ToString() + DateTime.Now.Day.ToString() + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Second.ToString();
                                 filename = uploadFile.FileName;
                                uploadFile.SaveAs(string.Format("{0}\\{1}", updir, filename));
                            }
                        }
                        else
                        {
                            //文件 分成多块上传
                            fullname = WriteTempFile(uploadFile, offset);
                            if (total-offset==1)
                            {
                                //如果是最后一个分块文件 ，则把文件从临时文件夹中移到上传文件 夹中
                                System.IO.FileInfo fi = new System.IO.FileInfo(fullname);
                                string oldFullName = string.Format("{0}\\{1}", updir, uploadFile.FileName);
                                FileInfo oldFi = new FileInfo(oldFullName);
                                if (oldFi.Exists)
                                {
                                    //文件名存在则删除旧文件 
                                    oldFi.Delete();
                                }
                                fi.MoveTo(oldFullName);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    context.Response.Write("Message" + ex.ToString());
                }
            }
        }
        /// <summary>
        /// 保存临时文件 
        /// </summary>
        /// <param name="uploadFile"></param>
        /// <param name="chunk"></param>
        /// <returns></returns>
        private string WriteTempFile(HttpPostedFile uploadFile, int chunk)
        {
            
            string tempDir = FileHelper.GetTempPath();
            if (!Directory.Exists(tempDir))
            {
                Directory.CreateDirectory(tempDir);
            }
            string fullName = string.Format("{0}\\{1}.part", tempDir, uploadFile.FileName);
            if (chunk==0)
            {
                //如果是第一个分块，则直接保存
                uploadFile.SaveAs(fullName);
            }
            else
            {
                //如果是其他分块文件 ，则原来的分块文件，读取流，然后文件最后写入相应的字节
                FileStream fs = new FileStream(fullName, FileMode.Append);
                if (uploadFile.ContentLength>0)
                {
                int       FileLen = uploadFile.ContentLength;
   byte[] input = new byte[FileLen];
   // Initialize the stream.
   System.IO.Stream MyStream = uploadFile.InputStream;
   // Read the file into the byte array.
   MyStream.Read(input, 0, FileLen);
   fs.Write(input,0,FileLen);
   fs.Close();
                }
            }
            
            return fullName;
        }
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
第四步：文件辅助类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/// <summary>
///FileHelper 的摘要说明
/// </summary>
public class FileHelper
{
    public FileHelper()
    {
        //
        //TODO: 在此处添加构造函数逻辑
        //
    }
    /// <summary>
    /// 获取上传目录
    /// </summary>
    /// <returns></returns>
     public static string GetUploadPath()
        {
          string path = HttpContext.Current.Server.MapPath("~/");
          string dirname = GetDirName();
          string uploadDir = path + "\\" + dirname;
          CreateDir(uploadDir);
          return uploadDir;
        }
    /// <summary>
    /// 获取临时目录
    /// </summary>
    /// <returns></returns>
     public static string GetTempPath()
     {
         string path = HttpContext.Current.Server.MapPath("~/");
         string dirname = GetTempDirName();
         string uploadDir = path + "\\" + dirname;
         CreateDir(uploadDir);
         return uploadDir;
     }
        private static string GetDirName()
        {
            return System.Configuration.ConfigurationManager.AppSettings["uploaddir"];
        }
        private static string GetTempDirName()
        {
            return System.Configuration.ConfigurationManager.AppSettings["tempdir"];
        }
    public static void CreateDir(string path)
        {
                if (!System.IO.Directory.Exists(path))
                {                    
                    System.IO.Directory.CreateDirectory(path);
                }
        }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
第五步，在进行上传之后，图片文件进行缩略图保存。如下图。
![](http://images.cnitblog.com/blog/10343/201301/30204316-ec49ee3db3a146dfbf0b39a1ec04261e.png)
                       图 9
注释掉以下语句：    // resize: { width: 320, height: 240, quality: 90 }, 之后上传之后的图片，plupload组件将不在进行缩略。如下图。
![](http://images.cnitblog.com/blog/10343/201301/30204403-bbc04b0ea7f74fad9c10c01349313b5f.png)
                               图10
第六步：以上都是英文界面，可以添加中文语言包。代码如下。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!-- Load Queue widget CSS and jQuery -->
<style type="text/css">@import url(Scripts/jquery.ui.plupload/css/jquery.ui.plupload.css);</style>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jqueryui/1.8.23/jquery-ui.min.js"></script>
<link rel="stylesheet" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.23/themes/base/jquery-ui.css"/>
<!-- Third party script for BrowserPlus runtime (Google Gears included in Gears runtime now) -->
<script type="text/javascript" src="http://bp.yahooapis.com/2.4.21/browserplus-min.js"></script>
<!-- Load plupload and all it's runtimes and finally the jQuery UI queue widget -->
<script type="text/javascript" src="Scripts/plupload.full.js"></script>
<script type="text/javascript" src="Scripts/i18n/zh-cn.js"></script> //这一句是重点，添加中文语言包文件 
<script type="text/javascript" src="Scripts/jquery.ui.plupload/jquery.ui.plupload.js"></script>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
添加中文语言包之后的界面显示如下图：
![](http://images.cnitblog.com/blog/10343/201301/30204606-d1693cda5e0449538a14ecfbaf4883f2.png)
上传错误信息：
![](http://images.cnitblog.com/blog/10343/201301/30204614-c07568684a924a0d9e8a1ffd51e69326.png)
![](http://images.cnitblog.com/blog/10343/201301/30204622-21dc1ccedbd0495aa499b05ec5d88210.png)
 demo文件 ：[批量文件上传示例](http://files.cnblogs.com/chillsrc/WebApplication1_vs2010_1.rar)
一共有两个文件 ：WebUploadFileTest2.aspx示例是上文中给出的代码示例文件 。
WebUploadFileTest.aspx是另一个示例文件 。
