# Python盗号原理-代码实现截屏键盘记录远程发送 - zhusongziye的博客 - CSDN博客





2018年04月25日 19:49:27[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：159








最简单的，也是技术手段相对较低的盗号方式当属钓鱼了（当然，社工更考验心理），除了钓鱼网站，就是发布某些带有诱惑性的工具，诱导消费者下载，运行后开启后门，或者启用钩子进行键盘记录。本篇文章我们就来讲一讲技术的实现。

![](https://img-blog.csdn.net/20180425194624816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 环境拓扑：

操作系统：windows7


python版本：2.7.14

主要库/模块：smtplib/pyHook/PIL/pythoncom等

# 设计思路：

> 
1.启用Hook，进行键盘的监听（当然鼠标坐标也可以监控，这不作为本节课的重点）


2.获取title标签/标题内容（比如说：新建文本文档）

3.设置敏感标题（这点很重要，我们只需要对特定敏感的标题进行记录，不然从一开始就记录键盘值那会很乱）

4.发现敏感标题（类似于QQ邮箱、微博、英雄联盟等网页标题）

5.发现标题后进行记录，当用户按回车或者Tab的时候进行截图发送


# 重点函数/方法：

> 
.find() 寻找关键字

time.strftime('%Y%m%d%H%M%S',time.localtime(time.time())) 时间点

ImageGrab.grab() 截图

hm = pyHook.HookManager() 创建hook


注：判断回车 空格 回删 Tab键 可以对照ascii码表来判断

![](https://img-blog.csdn.net/20180425194719925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（键盘ascii对照表）

好做完前期的思想铺垫后，我们开始上代码部分。


# 代码实现：

用到的库模块


![](https://img-blog.csdn.net/20180425194739371?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（用到的类库）

第一部分：发送邮件部分

![](https://img-blog.csdn.net/20180425194751224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（邮件发送代码）

注：截图功能在下面介绍，这只是流程


第二部：设置按键触发事件

![](https://img-blog.csdn.net/20180425194825429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（按键截图）

![](https://img-blog.csdn.net/20180425194835553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（启动函数）

完成上面的一切之后我们就可以来测试一下了，我们先打开网易邮箱测试下效果~


![](https://img-blog.csdn.net/20180425194845674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（启动）

这是启动效果，当然你可以把这些debug提示都去掉，鄙人是为了测试好用，在CMD下运行的所以会有黑框显示，这个不作为难点。

我们打开网易的163进行登录，下面的是后台debug显示，也就是我们程序的运行效果。

![](https://img-blog.csdn.net/20180425194858261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（后台显示）

我们再来看我们的邮箱收到了什么：

![](https://img-blog.csdn.net/20180425194908279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（完美）

好啦，这就是我们今天所将的内容了，么么哒伙计们，感谢支持！




源自：https://www.toutiao.com/a6547624596902773262/






