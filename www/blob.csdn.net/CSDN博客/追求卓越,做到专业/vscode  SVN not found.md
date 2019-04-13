
# vscode  SVN not found - 追求卓越,做到专业 - CSDN博客


2018年12月27日 20:11:00[Waldenz](https://me.csdn.net/enter89)阅读数：867标签：[vscode svn not found																](https://so.csdn.net/so/search/s.do?q=vscode svn not found&t=blog)[vscode svn.path																](https://so.csdn.net/so/search/s.do?q=vscode svn.path&t=blog)[svn.path																](https://so.csdn.net/so/search/s.do?q=svn.path&t=blog)[svn																](https://so.csdn.net/so/search/s.do?q=svn&t=blog)[vscode																](https://so.csdn.net/so/search/s.do?q=vscode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=svn&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=svn.path&t=blog)个人分类：[VS/VS Code																](https://blog.csdn.net/enter89/article/category/1185833)
[
																								](https://so.csdn.net/so/search/s.do?q=svn.path&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=vscode svn.path&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=vscode svn.path&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=vscode svn not found&t=blog)
svn客户端安装的是TortoiseSVN；
vscode 安装的为SVN的插件；
# 问题现象：
vscode打开文件夹后右下角提示如下报错：SVN not found. Install it or configure it using the "svn.path"
![](https://img-blog.csdnimg.cn/20181227181214629.png)
同时输出窗口提信息：Svn installation not found.
![](https://img-blog.csdnimg.cn/2018122718114624.png)
# 错误原因：
vscode找不到本地的svn.exe. 这个exe在默认安装TortoiseSVN时，默认是不勾选的，所以不会注意，其实在vscode中安装SVN插件时，该插件就已经提示了，需要在安装TortoiseSVN时，勾选上 Command Line Tools.
![](https://img-blog.csdnimg.cn/20181227195802729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
# 解决方法：
重新安装TortoiseSVN,并勾选上command line client tools选项。
![](https://img-blog.csdnimg.cn/20181227200128278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
有网友说需要安装VisualSVN，其实是没必要的，VisualSVN是SVN的服务器端，本地是可以不用的。



