# EasyCamera摄像机配置 - xcyl的口袋 - CSDN博客
2016年06月02日 14:43:10[xcyl](https://me.csdn.net/cai6811376)阅读数：2418
# EasyCamera摄像机配置
EasyCamera目前支持的摄像机有GM8126、HI3518C/HI3518E两种方案的摄像机。下面我以HI3518C方案为例介绍摄像机的配置。
### 1、连接网络
将摄像机通过有线的方式连接到摄像机，路由器需要开启DHCP功能，给摄像机分配到IP地址（如果路由器没有开启DHCP功能，摄像机连接网线后，摄像机的默认IP就是*192.168.1.88*）；
### 2、查找摄像机
打开/EasyCamera-master/SDK/NetLib/bin/HiCamSearcher.exe，搜索摄像机： 
![查找摄像机](https://img-blog.csdn.net/20160602143559443)
### 3、进入摄像机的Web管理
通过获取的摄像机IP地址登陆摄像机的Web管理界面，默认用户名：admin，默认密码：admin；若想在Web页面预览摄像机画面，可点击 **安装软件（初次使用）** 安装插件（*插件仅支持IE浏览器，若为高版本IE浏览器请开启兼容模式*）；插件安装完成点击 **电脑观看** 进入管理界面； 
![摄像机登陆首页](https://img-blog.csdn.net/20160602143936275)
### 4、区分摄像机硬件方案
进入设备信息页面，找到“软件版本”或者“固件版本”项，如果版本号以V5打头，那么摄像机是智源GM8126方案，如果版本号以V6打头，那么摄像机是海思HI3518C方案,如果版本号以V7打头，那么摄像机是海思HI3518E方案。 
![这里写图片描述](https://img-blog.csdn.net/20160602144732601)
### 5、摄像机开启Telnet服务
进入系统维护页面，在系统升级项中点击浏览找到所提供的升级包（GM8126方案选择/EasyCamera-master/SDK/GM8126/**telnet_8126.pkg**，HI3518C/HI3518E方案选择/EasyCamera-master/SDK/HI3518/**telnet_3518.pkg**），点击确定，等待系统重启。  
![这里写图片描述](https://img-blog.csdn.net/20160602145013134)
### 6、通过Telnet访问摄像机
摄像机开启telnet服务后即可通过telnet 终端进行访问。GM8126方案用户名为：root，密码为空、HI3518C方案用户名为：admin，密码为：HX2105，HI3518E方案用户名为：admin，密码为：2601hx。如下图所示。**摄像机自带的程序与配置位于/mnt/mtd/，请勿删除此目录下任何内容！！！**
![这里写图片描述](https://img-blog.csdn.net/20160602145248338)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
