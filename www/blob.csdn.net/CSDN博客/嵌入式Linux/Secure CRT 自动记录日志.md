
# Secure CRT 自动记录日志 - 嵌入式Linux - CSDN博客

2018年11月05日 16:41:46[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：64


配置自动log操作如下：
1.options ---> Global Options
![](https://pic1.zhimg.com/80/v2-fed2d9c3e694870bbe98504236197da0_hd.jpg)
2、General->Default Session->Edit Default Settings
![](https://pic3.zhimg.com/80/v2-be06bb52093349e6641dc9e61e6c95d6_hd.jpg)
3、Terminal->Log File
![](https://pic4.zhimg.com/80/v2-e692228c6e9f20c62e0147ab4eba8257_hd.jpg)
设置如图上所示
点击 日志 ，在选项框中 Log file name中填入路径和命名参数：
> E:\Log\%Y_%M_%D_%H.log
在Upon connect 输入
> [%Y%M%D_%h:%m:%s]
在On each line 输入
> [%h:%m:%s]
日志的格式为：年_月_日_主机名.log的日志，然后勾上Start log upon connect 和 append to file。
参数含义如下：
> %H 主机名（连接主机的IP地址）

> %Y 年份

> %M 月份

> %D 日

> %h 小时

> %m 分钟

> %s 秒
在Custom log data 中的On each line会话框中填入：
> %Y%M%D_%h:%m:%s
则CRT会在日志内容中的每行加入时间戳，具体内容按需要填入参数。
4、点击ok
**操作视频如下**
[https://zhuanlan.zhihu.com/p/48604386](https://zhuanlan.zhihu.com/p/48604386)

