# EasyDSS RTMP流媒体服务器开发直播同时HLS录像功能实现时移回放 - xcyl的口袋 - CSDN博客
2017年07月03日 00:25:47[xcyl](https://me.csdn.net/cai6811376)阅读数：1377
EasyDSS商用流媒体服务器解决方案是由EasyDarwin开源团队原班人马开发的一套集流媒体点播、转码与管理、直播、录像、检索、时移回看于一体的一套完整的商用流媒体服务器解决方案，支持RTMP推流，RTMP/HLS分发。 
[EasyDarwin如何支持点播和RTMP/HLS直播？EasyDSS！](http://blog.csdn.net/xiejiashu/article/details/73253903)
今天在这里我来谈一下EasyDSS在支持RTMP/HLS直播的同时，是如何实现录像功能和录像回放的。
## 首先
EasyDSS支持RTMP/HLS的直播流的分发。这里对于如何录像，就有了一个天然的优势，就是HLS。我们团队考虑再三，觉得将录像存储为HLS为上上之策。一来是可以直接使用EasyDSS稳定高效的HLS切片模块，再者，使用HLS做客户端录像回放是多么惬意的一件事。
## 实现
- 我们将HLS切片配置一个record开关 
在开关关闭的时候，我们支持的是传统的HLS直播，不做持久化切片保存，在切片的过程中遵循删除最早的策略。仅提供基本的HLS直播，生成直播的m3u8文件； 
record开关打开的时候，我们在提供基本的HLS直播的同时，生成直播的m3u8文件，并且会同步写入一个record的点播m3u8文件，并将切片文件进行持久化保存，不做删除。
- 录像查询 
当record开关关闭时，我们将切片通过RTMP推流的application和name进行分目录创建 
![这里写图片描述](https://img-blog.csdn.net/20170703001627793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当record开关为打开时，我们将以推流的起始时间创建下一级文件夹，并且将对应的record.m3u8和切片存在这个文件夹里 
![这里写图片描述](https://img-blog.csdn.net/20170703002305990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FpNjgxMTM3Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 录像查询时根据查询的application和那么就能索引到对应的存储的record.m3u8，进行HLS点播
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：[587254841](https://jq.qq.com/?_wv=1027&k=4ASE72a)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
