# EasyDSS RTMP流媒体服务器运行遇到getpwnam("xxxxx")错误的解决办法 - xcyl的口袋 - CSDN博客
2017年06月27日 10:07:46[xcyl](https://me.csdn.net/cai6811376)阅读数：666
## EasyDSS RTMP流媒体服务器是什么？
[EasyDarwin如何支持点播和RTMP/HLS直播？EasyDSS！](http://blog.csdn.net/xiejiashu/article/details/73253903)
## getpwnam(“xxxxx”)
近期有EasyDSS流媒体服务器用户反应运行easydss for linux会遇到 
`easydss: [emerg] getpwnam("nobody") failed`
错误，导致easydss无法运行。
## 原因
我们使用getpwnam这个函数来获取nobody账户的账户信息，并且我们默认任务这个nobody属于nobody用户组。 
这里主要分为两种情况。
- 在部分Linux发行版中默认无nobody用户组（如：ubuntu 14.04.1 LTS）。这里就需要添加nobody用户组，并且把nobody用户加入nobody用户组。 
命令为： 
`groupadd nobody`
`usermod -a -G nobody nobody`
查看用户命令为`cat /etc/passwd`，查看用户组为`cat /etc/group`
- 我们easydss for linux使用ubuntu 14.04 LTS编译，并且“试图”使用`-static`解决所有linux发行版的运行兼容问题，结果，这里就是此问题引起。
> 
if you link your program statically to GNU libc and use name services (getpwnam() etc.), you end up linked dynamically against libc’s NSS (name service switch).
[https://unix.stackexchange.com/questions/227910/will-my-linux-binary-work-on-all-distros](https://unix.stackexchange.com/questions/227910/will-my-linux-binary-work-on-all-distros)
## 解决方案
- 运行easydss for linux时，遇到getpwnam错误时，检查nobody用户和nobody组是否存在，不存在则添加。
- 较小众或者版本较低的linux发行版，请联系EasyDarwin团队进行支持。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[587254841](https://jq.qq.com/?_wv=1027&k=4ASE72a)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
