# 编译easydss遇到stray '\239' inprogram,stray '\187' inprogram,stray '\191' inprogram的解决办法 - xcyl的口袋 - CSDN博客
2017年11月30日 10:33:02[xcyl](https://me.csdn.net/cai6811376)阅读数：527
使用用户提供的交叉编译工具链编译easydss时遇到一个编译错误
```bash
stray '\239' inprogram
stray '\187' inprogram
stray '\191' inprogram
```
初看这个错误摸不着头脑，网上的解决方案也是一本正经的胡说八道
那我们来看看`239 187 191`是什么，也就是`EF BB BF`
![这里写图片描述](https://img-blog.csdn.net/20171130102733862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
原来是utf-8的BOM，那原因我大概了解了
这个源码的文件格式是带BOM的utf-8，此编译器版本较老不支持
那我们将源码文件做一下转换，转换成无BOM的utf-8
解决
BTW，visual studio创建的utf-8的源码文件都是添加BOM的，这个要注意
## 关于EasyDSS流媒体服务器
EasyDSS商用流媒体服务器解决方案是一套集流媒体点播、转码与管理、直播、录像、检索、时移回看于一体的一套完整的商用流媒体服务器解决方案，EasyDSS高性能RTMP流媒体服务器支持RTMP推流，同步输出HTTP、RTMP、HLS、HTTP-FLV，支持推流分发/拉流分发，支持秒开、GOP缓冲、录像、检索、回放、录像下载、网页管理等多种功能，是目前市面上最合理的一款商用流媒体服务器！
> 
详细说明：[http://www.easydss.com/](http://www.easydss.com/)
点击链接加入群【EasyDSS流媒体服务器】：[560148162](https://jq.qq.com/?_wv=1027&k=5oI6uUG)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
