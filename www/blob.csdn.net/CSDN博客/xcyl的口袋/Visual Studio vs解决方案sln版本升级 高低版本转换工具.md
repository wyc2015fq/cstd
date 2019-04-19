# Visual Studio vs解决方案sln版本升级/高低版本转换工具 - xcyl的口袋 - CSDN博客
2016年09月01日 11:11:57[xcyl](https://me.csdn.net/cai6811376)阅读数：5984
EasyDarwin各开源项目目前只提供Visual Studio 2008的解决方案文件，我们使用[Visual Studio 版本互转工具](http://www.wuleba.com/25894.html)给用户提供更多的Visual Studio版本选择。使用方法如下：
- 
打开转换工具
![这里写图片描述](https://img-blog.csdn.net/20160901105937504)
- 
点击浏览，选择需要转换的解决方案文件
![这里写图片描述](https://img-blog.csdn.net/20160901110055568)
- 
在下方列表中选择需要转换的版本然后双击
![这里写图片描述](https://img-blog.csdn.net/20160901110456573)
- 
关于
![这里写图片描述](https://img-blog.csdn.net/20160901110417166)
- 
# 注意
> 
如果选择把一个高级版本“降级”为低级版本，则需要手动进行一些修改，否则项目出错。例如：把2013转成2010之后，还需要自己手动删除项目内.NET Framework 4.5相关的引用，把那些引用删改之后，项目才可以进行编译编译。（因为2013默认使用.NET Framework 4.5，而2010默认使用.NET Framework 4.0，所以需要进行适当修改）。
**此工具转换C++工程无法保证完美转换，需要配合项目进行一定的配置修改。**
工具及更多相关工具下载
VS版本互转工具：[https://github.com/EasyDarwin/Tools/](https://github.com/EasyDarwin/Tools/)
更多开发工具下载：[https://github.com/EasyDarwin/Tools](https://github.com/EasyDarwin/Tools)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
