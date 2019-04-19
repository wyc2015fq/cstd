# EasyCamera开源项目介绍 - xcyl的口袋 - CSDN博客
2016年06月02日 14:24:08[xcyl](https://me.csdn.net/cai6811376)阅读数：1829
# EasyCamera开源项目介绍
**EasyCamera** 并不是做摄像机硬件方案，我们是在硬件方案的基础上，通过摄像机方案商提供的SDK在摄像机内部与摄像机主服务进行交互，包括实时音视频、云台控制、联动报警等功能，我们在摄像机内部植入EasyCamera程序，一边通过SDK与摄像机主进程交互(获取音视频数据、控制云台、联动报警等交互)，一边与EasyDarwin云平台(EasyCMS、EasyDarwin、EasyRMS等云服务器)对接，上传数据、接受指令控制，形成一套云视频摄像机方案.
EasyCamera服务支持跨平台Windows/Linux，支持ARM摄像机(目前已经有国内多家摄像机方案支持接入，包括雄迈、海芯威视、普顺达等)，支持Android/iOS移动设备(开发中)，对接EasyDarwin开源流媒体平台，我们定制的摄像机采海思HI3518C/HI3518E方案，支持RTSP、Onvif、Web管理、配套SDK工具，作为开发和演示硬件工具，我们提供了全套完备的程序和文档，既可以用于流媒体学习，又可以用于方案移植参考，更可以直接用于项目中，购买参考设备可以在：[https://easydarwin.taobao.com/](https://easydarwin.taobao.com/)，用户也可以将我们参考摄像机的SDK部分替换成自己摄像机的硬件SDK，移植非常方便；
## EasyCamera包括
- **SDK** 摄像机版本及SDK调用示例
- **SRC** EasyCamera开源程序
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
