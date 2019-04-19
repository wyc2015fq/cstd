# myeclipse下jsp页面汉字不能保存问题 - 左直拳的马桶_日用桶 - CSDN博客
2015年06月04日 16:09:02[左直拳](https://me.csdn.net/leftfist)阅读数：2285
在JSP页面中写了些汉字，结果想保存时，却提示编码不对。真扯！说什么“ISO-9000”之类的东东。
我记得以前可以在eclipse的菜单“编辑”中指定当前文件的编码，但不知道是不是我现在用的是myeclipse的缘故，这个菜单项死活找不到。后来在网上寻寻觅觅，解决之道为： 
 windows-preferences-content type-…,将编码改为utf-8即可。
![这里写图片描述](https://img-blog.csdn.net/20150604160713043)
但UTF-8 分两种，带签名和无签名（？），如果选UTF-8还有乱码的话，不妨在这两种编码中切换下：
eclipse好像这方面功能较弱，UTF-8只显示一种。可以用其他编辑工具打开源代码，进行转换、保存。如微软的visual studio，nodepad++。
![这里写图片描述](https://img-blog.csdn.net/20160531135940305)
