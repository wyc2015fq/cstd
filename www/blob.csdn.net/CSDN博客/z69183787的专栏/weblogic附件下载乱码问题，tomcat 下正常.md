# weblogic附件下载乱码问题，tomcat 下正常 - z69183787的专栏 - CSDN博客
2014年07月24日 20:30:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4525
在weblogic和tomcat下如何解决Word、Excel等打开乱码问题.
在weblogic应用程序下对于有时上传的文件，（如word、excel等）需要打开或下载到本地时，在weblogic容器默认情况下可能是以记事本打开从而导致乱码现象。为解决此问题可以在你的项目（project）下的web.xml文件中增加如下代码即可解决直接打开而出现乱码问题.
为了让能在IE浏览器中自动打开或弹出对话框的设置：需要在WEB.XML中进行如下的设置：在WEB.XML中添加<mime-mapping>
<extension>: 文件的扩展名
<mime-type>: 除了该类型文件的可执行文件,同WINDOW注册表中的
/HKEY_CLASSES_ROOT下该类文件的Content Type 的值一样.
如能在IE中自动打开DOC，XLS，PDF文件需在web.xml文件最后增加如下配置:
<mime-mapping> 
<extension>doc</extension>
<mime-type>application/msword</mime-type> 
</mime-mapping> 
<mime-mapping> 
<extension>xls</extension> 
<mime-type>application/msexcel</mime-type> 
</mime-mapping> 
<mime-mapping> 
<extension>pdf</extension> 
<mime-type>application/pdf</mime-type> 
</mime-mapping>
<mime-mapping> 
<extension>zip</extension> 
<mime-type>application/zip</mime-type> 
</mime-mapping>
<mime-mapping> 
<extension>rar</extension> 
<mime-type>application/rar</mime-type> 
</mime-mapping>
<mime-mapping> 
<extension>txt</extension> 
<mime-type>application/txt</mime-type> 
</mime-mapping>
<mime-mapping> 
<extension>chm</extension> 
<mime-type>application/mshelp</mime-type>
</mime-mapping> 
<mime-mapping>
<extension>mp3</extension>
<mime-type>audio/x-mpeg</mime-type>
</mime-mapping>
web.xml在WEB-INF目录下面.
至此即可解决出现上述的问题了.注意：有时候可能要清除一下IE缓存，才能解决上述问题。
另外页面中只需要增加:
response.setContentType("application/vnd.ms-Excel; charset=gb2312");
response.setHeader("Content-disposition","attachment;filename=BatchWork_1.xls");//Excel
这两行代码即可实现页面导出成Excel或Word(将ms-Excel修改成ms-Word即可)文档.
注: 以上两行代码是要加上新的页面上也就是导出需要重新做一个页面,在这个新的页面中加上这两句话即可.如果需要将filename= BatchWork_1.xls文件以中文名命名的话,可以这样写filename=" new String("查询.xls".getBytes("GBK"),"ISO8859-1")
