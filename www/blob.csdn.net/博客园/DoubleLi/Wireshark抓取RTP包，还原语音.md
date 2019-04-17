# Wireshark抓取RTP包，还原语音 - DoubleLi - 博客园






最近在做基于SIP的VoIP通信研究，使用Wireshark软件可以对网络流量进行抓包。

VoIP使用RTP协议对语音数据进行传输，语音载荷都封装在RTP包里面。要对传输中的语音进行截获和还原，需要通过Wireshark对RTP包进行分析和解码。该过程如下：

1.打开截获的pcapng文件，点击Telephony-->RTP-->Show All Streams.

![](http://img.blog.csdn.net/20151230164413980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2.根据发送地址和目的地址，选择要分析的语音流，点击图中的Analyze进行分析处理。Packets数太少可能会播放不出声音。

![](http://img.blog.csdn.net/20151230164557549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3.点击player。

![](http://img.blog.csdn.net/20151230164826354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4.对语音数据进行解码。点击Decode

![](http://img.blog.csdn.net/20151230165124355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5.得到语音波形。

![](http://img.blog.csdn.net/20151230165210706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

6.选择语音文件后，点击Play就可以听到声音了

![](http://img.blog.csdn.net/20151230165234810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



注意：目前我所[测试](http://lib.csdn.net/base/softwaretest)的能正确解码的是G.711编码，也即是PCM编码G.722和GSM等，没有正确解码成功。个人认为是因为Wireshark中缺乏了响应的解码库，导致了解码失败。

感谢QQ群：SIP VoIP 测试交流 323827101 中的枫林晨曦 给我的建议和启发。欢迎大家一起交流讨论。









