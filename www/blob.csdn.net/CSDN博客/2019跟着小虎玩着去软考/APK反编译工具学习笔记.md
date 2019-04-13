
# APK反编译工具学习笔记 - 2019跟着小虎玩着去软考 - CSDN博客

2011年06月15日 10:50:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2827标签：[反编译工具																](https://so.csdn.net/so/search/s.do?q=反编译工具&t=blog)[jar																](https://so.csdn.net/so/search/s.do?q=jar&t=blog)[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[cmd																](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=工具&t=blog)个人分类：[手机开发																](https://blog.csdn.net/littletigerat/article/category/649495)[Android																](https://blog.csdn.net/littletigerat/article/category/613910)[
							](https://blog.csdn.net/littletigerat/article/category/649495)
[
				](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=jar&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=反编译工具&t=blog)
**APK****反编译工具学习笔记**
## 一．
## dex2jar
## 工具的用途是什么？
用于将dex文件转换为jar文件
## 二．
## dex2jar
## 工具在哪儿下载？
http://code.google.com/p/dex2jar/
## 三．如何使用
## dex2jar
## 工具？
在cmd下进入dex2jar.bat所在路径，然后输入“dex2jar.bat XXX”，XXX指的是你要反编译的apk中的classes.dex文件所在路径及名称，比如：我的dex2jar.bat在
F:/ Android/Crack/dex2jar-0.0.7.7-SNAPSHOT路径下,
classes.dex在F:/Android/Learn/Test下，所以：你进入dex2jar.bat路径下后，输入dex2jar.bat D:/Android/Learn/Test/classes.dex，这样会生成一个jar文件。
## 四．
## jd-gui
## 工具的作用是什么？
将jar文件反向工程为java代码
## 五．
## jd-gui
## 工具在哪儿下载？
http://java.decompiler.free.fr/?q=jdgui
## 六．如何使用
## jd-gui
## 工具
1.将jd-gui-0.3.3.windows.rar下载后解压，点执行文件。
2.把刚才dex2jar生成的jar文件拖入jd_gui界面即可生成源代码

