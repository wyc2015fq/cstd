# 在部署EasyDarwin流媒体云平台过程中redis运行出现#Unknown RDB format version: 7错误的解决方法 - xcyl的口袋 - CSDN博客
2017年06月15日 14:02:28[xcyl](https://me.csdn.net/cai6811376)阅读数：1868
在部署EasyDarwin云平台（EasyDarwin云平台介绍以及部署请看[EasyDarwin云平台部署实践](http://blog.csdn.net/jinlong0603/article/details/60886234)）时遇到redis出现`#Unknown RDB format version: 7`错误导致redis无法启动
![这里写图片描述](https://img-blog.csdn.net/20170615135324213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此错误的意义为当前redis版本无法处理version：7的rdb文件，原因是此rdb文件为其他版本redis生成，当前版本redis无法兼容
解决方法：将此rdb删除重启redis即可。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
