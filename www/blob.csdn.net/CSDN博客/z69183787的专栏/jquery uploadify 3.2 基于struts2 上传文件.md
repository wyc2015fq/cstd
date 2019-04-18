# jquery uploadify 3.2 基于struts2 上传文件 - z69183787的专栏 - CSDN博客
2013年11月15日 20:24:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1457
个人分类：[Struts2																[Javascript-Jquery插件																[文件处理-上传](https://blog.csdn.net/z69183787/article/category/2229597)](https://blog.csdn.net/z69183787/article/category/2175469)](https://blog.csdn.net/z69183787/article/category/2175433)
项目截图：
![](http://static.oschina.net/uploads/space/2013/0331/181832_e4tp_992608.png)
index.jsp 代码--》 down的例子做的修改（不知道哪里改错了的）
|`01`|`<``link``rel``=``"stylesheet"``type``=``"text/css"``href``=``"css/uploadify.css"``>`|
|`02`|` ``<``script``type``=``"text/javascript"``src``=``"jquery/jquery.uploadify.min.js"``></``script``>`|
|`03`|` ``<``script``type``=``"text/javascript"``src``=``"jquery/jquery-1.7.js"``></``script``>`|
|`04`|` ``<``script``type``=``"text/javascript"``src``=``"jquery/swfobject2.2.js"``></``script``>`|
|`05`|` ``<``script``>`|
|`06`|```$(document).ready(function() {```|
|`07`|`    ``$("#uploadify").uploadify({`|
|`08`|`           ``//开启调试`|
|`09`|`           ````'debug' : false,```|
|`10`|`           ``//是否自动上传`|
|`11`|`           ``'auto':false,`|
|`12`|`           ``//超时时间`|
|`13`|`           ``'successTimeout':99999,`|
|`14`|`           ``//附带值`|
|`15`|`           ``'formData':{`|
|`16`|`               ``'userid':'用户id',`|
|`17`|`               ``'username':'用户名',`|
|`18`|`               ``'rnd':'加密密文'`|
|`19`|`           ``},`|
|`20`|`           ````'swf': "jquery/uploadify.swf",```|
|`21`|`           ``'queueID':'fileQueue',`|
|`22`|`           ``'fileObjName':'"uploadify"',`|
|`23`|`           ``'uploader':'uploadAction',`|
|`24`|`           ``'buttonImage':'jquery/uploadify-cancel.png',`|
|`25`|`           ``'width':'100',`|
|`26`|`           ``'height':'32',`|
|`27`|`           ``'fileTypeDesc':'支持的格式：',`|
|`28`|`           ``'fileSizeLimit':'10000MB',`|
|`29`|`           ````'buttonText' : '选择文件',//浏览按钮```|
|`30`|`           ````'queueSizeLimit' : 25,```|
|`31`|`           ``//每次更新上载的文件的进展`|
|`32`|`           ````'onUploadProgress' : function(file, bytesUploaded, bytesTotal, totalBytesUploaded, totalBytesTotal) {```|
|`33`|`                ``//有时候上传进度什么想自己个性化控制，可以利用这个方法`|
|`34`|`                ``//使用方法见官方说明`|
|`35`|`           ``},`|
|`36`|`           ``//选择上传文件后调用`|
|`37`|`           ````'onSelect' : function(file) {```|
|`38`|`           ``},`|
|`39`|`           ``//返回一个错误，选择文件的时候触发`|
|`40`|`           ````'onSelectError':function(file, errorCode, errorMsg){```|
|`41`|`               ````switch(errorCode) {```|
|`42`|`                   ````case -100:```|
|`43`|`                       ``alert("上传的文件数量已经超出系统限制的"+$('#uploadify').uploadify('settings','queueSizeLimit')+"个文件！");`|
|`44`|`                       ``break;`|
|`45`|`                   ````case -110:```|
|`46`|`                       ````alert("文件 ["+file.name+"] 大小超出系统限制的"+$('#uploadify').uploadify('settings','fileSizeLimit')+"大小！");```|
|`47`|`                       ``break;`|
|`48`|`                   ````case -120:```|
|`49`|`                       ````alert("文件 ["+file.name+"] 大小异常！");```|
|`50`|`                       ``break;`|
|`51`|`                   ````case -130:```|
|`52`|`                       ````alert("文件 ["+file.name+"] 类型不正确！");```|
|`53`|`                       ``break;`|
|`54`|`               ``}`|
|`55`|`           ``},`|
|`56`|`           ``//检测FLASH失败调用`|
|`57`|`           ``'onFallback':function(){`|
|`58`|`               ``alert("您未安装FLASH控件，无法上传图片！请安装FLASH控件后再试。");`|
|`59`|`           ``},`|
|`60`|`           ``//上传到服务器，服务器返回相应信息到data里`|
|`61`|`           ````'onUploadSuccess':function(file, data, response){```|
|`62`|`            ``//$('#uploadify').uploadify('upload');`|
|`63`|`            ``//alert(data);`|
|`64`|`           ``}`|
|`65`|`       ``});`|
|`66`|`    ``});`|
|`67`||
|`68`||
|`69`|` ``</``script``>`|
|`70`||
|`71`||
|`72`|`  ``</``head``>`|
|`73`|`  `|
|`74`|`  ``<``body``>`|
|`75`|` ``<``div``id``=``"fileQueue"``style``=``"width:300px"``></``div``>`|
|`76`|`    ``<``input``type``=``"file"``name``=``"uploadify"``id``=``"uploadify"``multiple``=``"true"``/>`|
|`77`|`    ``<``p``>`|
|`78`|`        ``<``a``href``=``"javascript:$('#uploadify').uploadify('upload')"``>开始上传</``a``> `|
|`79`|`        ``<``a``href``=``"javascript:$('#uploadify').uploadify('cancel')"``>取消所有上传</``a``>`|
|`80`|`    ``</``p``>`|
|`81`|`  ``</``body``>`|
处理action代码：
|`01`|`@SuppressWarnings``(``"serial"``)`|
|`02`|`public``class``UploadAction ``extends````ActionSupport {```|
|`03`||
|`04`|`private````File uploadify;```|
|`05`||
|`06`||
|`07`|`private````String uploadifyFileName;```|
|`08`||
|`09`||
|`10`|`@SuppressWarnings``(``"deprecation"``)`|
|`11`|`public````String uploadFile()````throws````Exception {```|
|`12`||
|`13`||
|`14`|```String extName =````""``;````// 扩展名```|
|`15`||
|`16`||
|`17`|```String newFileName =````""``;````// 新文件名```|
|`18`||
|`19`||
|`20`|```String nowTime =````new``SimpleDateFormat(``"yyyymmddHHmmss"``)`|
|`21`|`.format(``new``Date());````// 当前时间```|
|`22`||
|`23`||
|`24`|```String savePath = ServletActionContext.getRequest().getRealPath(````""``);`|
|`25`||
|`26`||
|`27`|```savePath = savePath +````"/uploads/"``;`|
|`28`|`System.out.println(savePath);`|
|`29`|```HttpServletResponse response = ServletActionContext.getResponse();```|
|`30`|`response.setCharacterEncoding(``"utf-8"``);`|
|`31`||
|`32`||
|`33`|```// 获取扩展名```|
|`34`|`if``(uploadifyFileName.lastIndexOf(``"."````) >=````0````) {```|
|`35`|```extName = uploadifyFileName.substring(uploadifyFileName```|
|`36`|`.lastIndexOf(``"."``));`|
|`37`|`}`|
|`38`|```newFileName = nowTime + extName;```|
|`39`||
|`40`||
|`41`|`uploadify.renameTo(``new````File(savePath + newFileName));```|
|`42`||
|`43`||
|`44`|```response.getWriter().print(uploadifyFileName +````"上传成功"``);`|
|`45`||
|`46`||
|`47`|`return``null``; ````// 这里不需要页面转向，所以返回空就可以了```|
|`48`||
|`49`||
|`50`|`}`|
|`51`||
|`52`||
|`53`|`public````File getUploadify() {```|
|`54`|`return``uploadify;`|
|`55`|`}`|
|`56`||
|`57`||
|`58`|`public``void````setUploadify(File uploadify) {```|
|`59`|`this````.uploadify = uploadify;```|
|`60`|`}`|
|`61`||
|`62`||
|`63`|`public````String getUploadifyFileName() {```|
|`64`|`return``uploadifyFileName;`|
|`65`|`}`|
|`66`||
|`67`||
|`68`|`public``void````setUploadifyFileName(String uploadifyFileName) {```|
|`69`|`this````.uploadifyFileName = uploadifyFileName;```|
|`70`|`}`|
|`71`||
|`72`||
|`73`|`}`|

