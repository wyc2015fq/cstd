# ReSIProcate环境搭建 - DoubleLi - 博客园






1首先下载resiprocate-1.6

2取消resiprocate-1.6目录的只读属性

3然后使用Visual Studio 2008打开resiprocate-1.6下的reSIProcate_9_0.sln解决方案

4 右击reSIProcate_9_0解决方案下的resiprocate，reprosetup,(最好把re开头项目都生成下)项目，选择重新生成，等编译完成后，会在resiprocate-1.6\repro\WinSetup\Debug下生成可执行文件。拷贝resiprocate-1.6\contrib\popt\win32下的libiconv-2.dll、libintl-2.dll、popt1.dll三个dll文件到resiprocate-1.6\repro\WinSetup\Debug。即将libiconv-2.dll、libintl-2.dll、popt1.dll三个dll文件与可执行文件放到同一个目录下，否则执行时会出错。

5到目录下，安装执行文件。

打开浏览器,输入地址：[http://127.0.0.1:5080](http://127.0.0.1:5080/)。可以看到登录页面：

6点击登录链接，输入用户名和密码(均为admin)。出现设置页面。

 设置域名：如果是在局域网，域名直接设置

为IP地址，如127.0.0.1。点击Add按钮添加域名。

下面是运行成功图：

：![ReSIProcate环境搭建](http://s12.sinaimg.cn/middle/92da10514ba9e2d056e3b&690)



![ReSIProcate环境搭建](http://s16.sinaimg.cn/middle/92da10514ba9e18aabdbf&690)












