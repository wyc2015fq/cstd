# 编译nginx windows遇到”error C3646: “st_size”: 未知重写说明符“的解决办法 - xcyl的口袋 - CSDN博客
2017年02月14日 14:41:31[xcyl](https://me.csdn.net/cai6811376)阅读数：445
编译windows版的nginx，若引用了自定义的C++编写的模块，会遇到下列错误
![这里写图片描述](https://img-blog.csdn.net/20170214143910037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这时，我们需要在引用C++头文件之前使用如下代码段
```
#if (defined _WIN32 && defined _OFF_T_DEFINED)
typedef off_t _off_t;
#endif
```
此问题解决
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[288214068](http://jq.qq.com/?_wv=1027&k=2Dlyhr7) / [496258327](http://jq.qq.com/?_wv=1027&k=2Hyz2ea)
Copyright © www.EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
