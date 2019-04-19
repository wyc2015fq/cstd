# H.264 Profile、Level、Encoder三张简图 - maopig的专栏 - CSDN博客
2013年06月02日 14:01:17[maopig](https://me.csdn.net/maopig)阅读数：2521
个人分类：[x264 h264																[DM368](https://blog.csdn.net/maopig/article/category/919673)](https://blog.csdn.net/maopig/article/category/847676)
H.264有四种画质级别,分别是BP、EP、MP、HP：
　　1、BP-Baseline Profile：基本画质。支持I/P 帧，只支持无交错（Progressive）和CAVLC；
　　2、EP-Extended profile：进阶画质。支持I/P/B/SP/SI 帧，只支持无交错（Progressive）和CAVLC；
　　3、MP-Main profile：主流画质。提供I/P/B 帧，支持无交错（Progressive）和交错（Interlaced），
　　　 也支持CAVLC 和CABAC 的支持；
　　4、HP-High profile：高级画质。在main Profile 的基础上增加了8x8内部预测、自定义量化、 无损视频编码和更多的YUV 格式；
想要说明H.264 HP与H.264 MP的区别就要讲到H.264的技术发展了。JVT于2003年完成H.264基本部分标准制定工作，包含Baseline profile、Extended profile和Main profile，分别包括不同的编码工具。之后JVT又完成了H.264 FRExt（即：Fidelity Range Extensions）扩展部分（Amendment）的制定工作，包括High
 profile（HP）、High 10 profile（Hi10P）、High 4:2:2 profile（Hi422P）、High 4:4:4 profile（Hi444P）4个profile。
　　H.264 Baseline profile、Extended profile和Main profile都是针对8位样本数据、4:2:0格式的视频序列，FRExt将其扩展到8～12位样本数据，视频格式可以为4:2:0、4:2:2、4:4:4，设立了High profile（HP）、High 10 profile（Hi10P）、High 4:2:2 profile（Hi422P）、High 4:4:4
 profile（Hi444P） 4个profile，这4个profile都以Main profile为基础。
在相同配置情况下，High profile（HP）可以比Main profile（MP）节省10%的码流量，比MPEG-2 MP节省60%的码流量，具有更好的编码性能。根据应用领域的不同，Baseline profile多应用于实时通信领域，Main profile多应用于流媒体领域，High profile则多应用于广电和存储领域。
H.264 Baseline Profile对应MPEG-4 SP 
H.264 Main Profile对应MPEG-4 ASP 
H.264 Extended Profile对应MPEG-4 ARTS or FGS 
H.264 Baseline Profile对应MPEG-4 Studio。
## H.264 Profile、Level、Encoder三张简图
【整理者】61ic.com
【提供者】QuestionMark         
【详细说明】H.264 Profile、Level、Encoder三张简图
取自 wikipedia                                                                                                                                                                                                                                      
h.264 profile
![](http://www.61ic.com/code/attachments/month_1003/20100309_d233c0489c46697c323bt7nwz7f4Vv7D.jpg)
h.264 level
![](http://www.61ic.com/code/attachments/month_1003/20100309_b50b9fe60841fbf4572daJBgHsqARRa8.jpg)
h.264 software encoder
![](http://www.61ic.com/code/attachments/month_1003/20100309_eaf6c4bfcb9eec9331f7mrmHRMOZ9ZEJ.jpg)
本文出自 “[流媒体技术工程组](http://xcshen.blog.51cto.com/)” 博客，请务必保留此出处[http://xcshen.blog.51cto.com/2835389/552669](http://xcshen.blog.51cto.com/2835389/552669)
