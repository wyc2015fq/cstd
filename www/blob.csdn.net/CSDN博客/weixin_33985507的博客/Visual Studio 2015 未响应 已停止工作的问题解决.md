# Visual Studio 2015 未响应/已停止工作的问题解决 - weixin_33985507的博客 - CSDN博客
2016年10月19日 01:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
在我把之前项目从10版本升级到15版本的时候，一打开转换的项目过几分钟立马卡死，出现未响应/已停止工作的问题，我试过了很多方法：
1、升级操作系统，8.1升级10，没用！
2、重装VS，没用！
3、卸载插件，纯净启动，没用！
4、重置用户数据启动VS，没用！
5、安全模式启动VS，没用！
6、删除项目中的suo文件、.vs文件夹，没用！
最后，我发现了技巧，我重新迁出一份最新版本的代码到本地，然后打开转换，一点问题的没有，成功问题解决！
可能是原来那份代码有很多编译的obj和bin文件等，导致读取失败！
这里也收集了在解决问题时的方案：
[https://connect.microsoft.com/VisualStudio/feedback/details/1832568/visual-studio-2015-not-responding-after-opening-existing-project](https://connect.microsoft.com/VisualStudio/feedback/details/1832568/visual-studio-2015-not-responding-after-opening-existing-project)
[http://www.it610.com/article/4362135.htm](http://www.it610.com/article/4362135.htm)
[http://www.ask3.cn/a/jiaocheng/bckf/2014/1008/105838.html](http://www.ask3.cn/a/jiaocheng/bckf/2014/1008/105838.html)
