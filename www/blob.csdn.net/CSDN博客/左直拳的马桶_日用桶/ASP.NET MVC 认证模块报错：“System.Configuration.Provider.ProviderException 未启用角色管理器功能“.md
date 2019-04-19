# ASP.NET MVC 认证模块报错：“System.Configuration.Provider.ProviderException: 未启用角色管理器功能“ - 左直拳的马桶_日用桶 - CSDN博客
2014年01月03日 15:49:29[左直拳](https://me.csdn.net/leftfist)阅读数：4719
新建MVC4项目的时候
![](https://img-blog.csdn.net/20140103152527671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选 Internet 应用程序的话，出来的示例项目就自带了默认的登录认证等功能。如果选空或者基本，就没有。
如果没有，现在又想加进去，怎么办呢？
抄啊。将示例项目的代码原原本本地操过去，包括app_start啦，Models啦，controller啦，Filters啦，global.asax啦，web.config啦，结果一点注册，报错：
异常详细信息: System.Configuration.Provider.ProviderException: 未启用角色管理器功能。
![](https://img-blog.csdn.net/20140103153822265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这究竟是为何呢！想来想去，只有bin这个目录没有照搬了。
经过一番对照，示例项目多了两个DLL:
WebMatrix.Data.dll
WebMatrix.WebData.dll
但是，报错那个项目也明明引用了这个类集，只不过BIN里没有出现而已。经网上查资料，将此两个类集的属性设为复制本地，问题解决：
1、鼠标右键点类集
![](https://img-blog.csdn.net/20140103154829718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、设为复制本地
![](https://img-blog.csdn.net/20140103154913140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
