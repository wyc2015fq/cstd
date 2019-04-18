# FEC（Forward Error Correction）前向纠错 UDP\RTP 中使用用于改善无线等网络丢包等问题--转... - weixin_33985507的博客 - CSDN博客
2017年05月16日 21:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
FEC（Forward Error Correction）前向纠错 UDP\RTP 中使用用于改善无线等网络丢包等问题
[算法](http://lib.csdn.net/base/datastructure)暂不介绍。
思路：FEC ENCODE 增加冗余包，当无线等网络丢包之后，接收端使用冗余包可将丢失的包DECODE出来。
举例：10个包，编码后会增加2个包，共12个包发送到接收端，接收端丢失第5和第9包，仅靠剩下的10个包就可以解出第5和第9包。
结果就是，接收端接收到了完整的10个包，代价仅仅是增加了冗余和cpu编解码的消耗。
参考：
1.
# RTP抗丢包传输方案
[点击打开链接](http://www.mediapro.cc/?page_id=14)
2. 
# [使用FEC改善UDP（RTP）音视频传输效果](http://blog.csdn.net/mediapro/article/details/50393685)：
[点击打开链接](http://blog.csdn.net/mediapro/article/details/50393685)
