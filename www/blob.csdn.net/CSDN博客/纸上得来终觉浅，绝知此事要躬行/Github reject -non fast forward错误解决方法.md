# Github reject -non fast forward错误解决方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月21日 11:01:29[boonya](https://me.csdn.net/boonya)阅读数：833








如题所示之问题：

出现情况分析：**项目Git只设置了本地路径而对远程路径无效，需要设置fetch路径。打开Eclipse GIt 视图，**找到Remotes

![](https://img-blog.csdn.net/20161121105539896)

new remote ....

![](https://img-blog.csdn.net/20161121105723475)

输入名称等，ok执行fetch操作，之后到项目中pull之后就可以执行Commit and Push.




