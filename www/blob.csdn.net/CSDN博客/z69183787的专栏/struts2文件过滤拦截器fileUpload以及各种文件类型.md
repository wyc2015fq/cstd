# struts2文件过滤拦截器fileUpload以及各种文件类型 - z69183787的专栏 - CSDN博客
2014年06月26日 21:33:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6263
分为两种方式吧，一种注解，一种配置struts.xml
①
基于方法
@Action(interceptorRefs={@InterceptorRef("validation"), @InterceptorRef("defaultStack")}
基于类
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- @InterceptorRefs({  
- @InterceptorRef (  
-         value="fileUpload",  
- params={"allowedTypes", "application/msword"}  
-     ),  
- @InterceptorRef (value="defaultStack")  
- })  
需要注意的是：必须加上defaultStack，而且defaultStack要放在最后
加上defaultStack的原因是 这个拦截器可以负责传参，参看 http://www.cnblogs.com/lanhj/p/3321890.html
defaultStack包含了fileUpload，但是defaultStack中的fileUpload是没有参数的，也就是支持任意类型的文件，
而xml的机制是：在重复出现同一个拦截器时，以第一个出现的为准（Annotation也会翻译成xml）
因此当defaultStack放在最前边时，默认的fileUpload会覆盖咱们的fileUpload
②
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- <package name="default"extends="struts-default">  
-     <action name="upload2"class="edu.wuli.action.UploadAction">  
-         <param name="savePath">/homework</param>  
-         <result name="input">/WEB-INF/content/wrong.jsp</result>  
-         <result type="redirect">homework!query?article.id=${article.id}</result>  
-         <interceptor-ref name="fileUpload">    
-             <param name="allowedTypes">    
-                 application/msword   
-             </param>    
-             <param name="maxinumSize">3</param>    
-         </interceptor-ref>    
-         <interceptor-ref name="defaultStack"/>    
-     </action>  
- </package>  
需要注意的是action的name别符合零配置，否则不走这段配置，而是直接走Action
怎么输出fileUpload的错误信息？
需要使用i18n的资源文件，然后用<s:fielderror/>可以输出错误信息。
例:
在xml中加入：
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- <constant name="struts.custom.i18n.resources" value="mess"/>  
然后在src目录下新建mess.properties,在里边加上：
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- struts.messages.error.content.type.not.allowed=upload file contenttype is invalidate  
- struts.messages.error.file.too.large=upload file is too large  
- struts.messages.error.uploading=upload file uploading error  
三行分别代表：格式错误，文件容量过大， 上传错误
如果想输出中文，就需要转成Unicode码：
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- struts.messages.error.content.type.not.allowed=\u6587\u4EF6\u7C7B\u578B\u9519\u8BEF  
- struts.messages.error.file.too.large=\u6587\u4EF6\u8FC7\u5927  
- struts.messages.error.uploading=\u4E0A\u4F20\u9519\u8BEF  
fileUpload文件容量设置（以字节为单位，以下限制1M）
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- <param name="maximumSize">1048576</param>   
）：
各种文件类型：
[查看文本](http://www.niwozhi.net/demo_c68_i37860.html#)[打印](http://www.niwozhi.net/demo_c68_i37860.html#)
- '.a'      : 'application/octet-stream',      
- 
- '.ai'     : 'application/postscript',      
- 
- '.aif'    : 'audio/x-aiff',      
- 
- '.aifc'   : 'audio/x-aiff',      
- 
- '.aiff'   : 'audio/x-aiff',      
- 
- '.au'     : 'audio/basic',      
- 
- '.avi'    : 'video/x-msvideo',      
- 
- '.bat'    : 'text/plain',      
- 
- '.bcpio' : 'application/x-bcpio',      
- 
- '.bin'    : 'application/octet-stream',      
- 
- '.bmp'    : 'image/x-ms-bmp',      
- 
- '.c'      : 'text/plain',     
- 
- '.cdf'    : 'application/x-cdf',      
- 
- '.cdf'    : 'application/x-netcdf',      
- 
- '.cpio'   : 'application/x-cpio',      
- 
- '.csh'    : 'application/x-csh',      
- 
- '.css'    : 'text/css',      
- 
- '.dll'    : 'application/octet-stream',      
- 
- '.doc'    : 'application/msword',      
- 
- '.dot'    : 'application/msword',      
- 
- '.dvi'    : 'application/x-dvi',      
- 
- '.eml'    : 'message/rfc822',      
- 
- '.eps'    : 'application/postscript',      
- 
- '.etx'    : 'text/x-setext',      
- 
- '.exe'    : 'application/octet-stream',      
- 
- '.gif'    : 'image/gif',      
- 
- '.gtar'   : 'application/x-gtar',      
- 
- '.h'      : 'text/plain',      
- 
- '.hdf'    : 'application/x-hdf',      
- 
- '.htm'    : 'text/html',      
- 
- '.html'   : 'text/html',      
- 
- '.ief'    : 'image/ief',      
- 
- '.jpe'    : 'image/jpeg',      
- 
- '.jpeg'   : 'image/jpeg',      
- 
- '.jpg'    : 'image/jpeg',      
- 
- '.js'     : 'application/x-javascript',      
- 
- '.ksh'    : 'text/plain',      
- 
- '.latex' : 'application/x-latex',      
- 
- '.m1v'    : 'video/mpeg',      
- 
- '.man'    : 'application/x-troff-man',      
- 
- '.me'     : 'application/x-troff-me',      
- 
- '.mht'    : 'message/rfc822',      
- 
- '.mhtml' : 'message/rfc822',      
- 
- '.mif'    : 'application/x-mif',      
- 
- '.mov'    : 'video/quicktime',      
- 
- '.movie' : 'video/x-sgi-movie',      
- 
- '.mp2'    : 'audio/mpeg',      
- 
- '.mp3'    : 'audio/mpeg',      
- 
- '.mpa'    : 'video/mpeg',      
- 
- '.mpe'    : 'video/mpeg',      
- 
- '.mpeg'   : 'video/mpeg',      
- 
- '.mpg'    : 'video/mpeg',      
- 
- '.ms'     : 'application/x-troff-ms',      
- 
- '.nc'     : 'application/x-netcdf',      
- 
- '.nws'    : 'message/rfc822',      
- 
- '.o'      : 'application/octet-stream',      
- 
- '.obj'    : 'application/octet-stream',      
- 
- '.oda'    : 'application/oda',      
- 
- '.p12'    : 'application/x-pkcs12',      
- 
- '.p7c'    : 'application/pkcs7-mime',      
- 
- '.pbm'    : 'image/x-portable-bitmap',      
- 
- '.pdf'    : 'application/pdf',      
- 
- '.pfx'    : 'application/x-pkcs12',      
- 
- '.pgm'    : 'image/x-portable-graymap',      
- 
- '.pl'     : 'text/plain',      
- 
- '.png'    : 'image/png',      
- 
- '.pnm'    : 'image/x-portable-anymap',      
- 
- '.pot'    : 'application/vnd.ms-powerpoint',      
- 
- '.ppa'    : 'application/vnd.ms-powerpoint',      
- 
- '.ppm'    : 'image/x-portable-pixmap',      
- 
- '.pps'    : 'application/vnd.ms-powerpoint',      
- 
- '.ppt'    : 'application/vnd.ms-powerpoint',      
- 
- '.ps'     : 'application/postscript',      
- 
- '.pwz'    : 'application/vnd.ms-powerpoint',      
- 
- '.py'     : 'text/x-python',      
- 
- '.pyc'    : 'application/x-python-code',      
- 
- '.pyo'    : 'application/x-python-code',      
- 
- '.qt'     : 'video/quicktime',      
- 
- '.ra'     : 'audio/x-pn-realaudio',      
- 
- '.ram'    : 'application/x-pn-realaudio',      
- 
- '.ras'    : 'image/x-cmu-raster',      
- 
- '.rdf'    : 'application/xml',      
- 
- '.rgb'    : 'image/x-rgb',      
- 
- '.roff'   : 'application/x-troff',      
- 
- '.rtx'    : 'text/richtext',      
- 
- '.sgm'    : 'text/x-sgml',      
- 
- '.sgml'   : 'text/x-sgml',      
- 
- '.sh'     : 'application/x-sh',      
- 
- '.shar'   : 'application/x-shar',      
- 
- '.snd'    : 'audio/basic',      
- 
- '.so'     : 'application/octet-stream',      
- 
- '.src'    : 'application/x-wais-source',      
- 
- '.sv4cpio': 'application/x-sv4cpio',      
- 
- '.sv4crc' : 'application/x-sv4crc',      
- 
- '.swf'    : 'application/x-shockwave-flash',      
- 
- '.t'      : 'application/x-troff',      
- 
- '.tar'    : 'application/x-tar',      
- 
- '.tcl'    : 'application/x-tcl',      
- 
- '.tex'    : 'application/x-tex',      
- 
- '.texi'   : 'application/x-texinfo',      
- 
- '.texinfo': 'application/x-texinfo',      
- 
- '.tif'    : 'image/tiff',      
- 
- '.tiff'   : 'image/tiff',      
- 
- '.tr'     : 'application/x-troff',      
- 
- '.tsv'    : 'text/tab-separated-values',      
- 
- '.txt'    : 'text/plain',      
- 
- '.ustar' : 'application/x-ustar',      
- 
- '.vcf'    : 'text/x-vcard',      
- 
- '.wav'    : 'audio/x-wav',      
- 
- '.wiz'    : 'application/msword',      
- 
- '.wsdl'   : 'application/xml',      
- 
- '.xbm'    : 'image/x-xbitmap',      
- 
- '.xlb'    : 'application/vnd.ms-excel',      
- 
- '.xls'    : 'application/excel',      
- 
- '.xls'    : 'application/vnd.ms-excel',      
- 
- '.xml'    : 'text/xml',      
- 
- '.xpdl'   : 'application/xml',      
- 
- '.xpm'    : 'image/x-xpixmap',      
- 
- '.xsl'    : 'application/xml',      
- 
- '.xwd'    : 'image/x-xwindowdump',      
- 
- '.zip'    : 'application/zip',
  
