# 实现Onvif设备发现并与前端配合进行快速部署 - xcyl的口袋 - CSDN博客
2017年08月13日 23:44:58[xcyl](https://me.csdn.net/cai6811376)阅读数：823
我们都知道EasyNVR支持Onvif协议接入设备
并且之前也介绍了如何进行Onvif的设备发现，探测，信息获取等等
那我们是如何使用的呢，我们是如何使用Onvif协议为实施人员减轻配置工作的呢
## 看图
![这里写图片描述](https://img-blog.csdn.net/20170813233752448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
EasyNVR的通道配置页面，可以对通道进行多种配置
![这里写图片描述](https://img-blog.csdn.net/20170813233838019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
配置弹出窗口，可以选择直接填写参数，也可以使用我们的Onvif发现来辅助
![这里写图片描述](https://img-blog.csdn.net/20170813233953928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选择Onvif探测，会展开Onvif参数进行填写配置，这里可以直接填写IP，用户名密码进行参数获取
![这里写图片描述](https://img-blog.csdn.net/20170813234102259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
IP地址框中展示的是EasyNVR进行的设备发现所发现的设备，减去直接填写的烦恼，当然，广播搜索不到的需要填写具体设备信息
![这里写图片描述](https://img-blog.csdn.net/20170813234239096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
填好Onvif信息后，点击探测
![这里写图片描述](https://img-blog.csdn.net/20170813234316641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
会将获取到的设备信息填充进来，直接配置完成
**使用Onvif设备发现与探测可以为我们节省很多配置时间**
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
