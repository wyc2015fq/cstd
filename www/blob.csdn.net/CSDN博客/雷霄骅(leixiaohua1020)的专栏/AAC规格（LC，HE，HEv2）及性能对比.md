# AAC规格（LC，HE，HEv2）及性能对比 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月24日 13:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：37









现如今常见的AAC规格有三种：LC-AAC（最基本的），HE-AAC（AACPlus v1），HE-AAC v2（AACPlus v2）。他们三个的关系如下图所示：

![](https://img-blog.csdn.net/20130924125624640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

HE：“High Efficiency”（高效性）。HE-AAC v1（又称AACPlusV1，SBR)，用容器的方法实现了AAC（LC）和SBR技术。SBR其实代表的是Spectral Band Replication(频段复制)。简要叙述一下，音乐的主要频谱集中在低频段，高频段幅度很小，但很重要，决定了音质。如果对整个频段编码，若是为了 保护高频就会造成低频段编码过细以致文件巨大；若是保存了低频的主要成分而失去高频成分就会丧失音质。SBR把频谱切割开来，低频单独编码保存主要成分， 高频单独放大编码保存音质，“统筹兼顾”了，在减少文件大小的情况下还保存了音质，完美的化解这一矛盾。

HEv2：用容器的方法包含了HE-AAC v1和PS技术。PS指“parametric stereo”（参数立体声）。原来的立体声文件文件大小是一个声道的两倍。但是两个声道的声音存在某种相似性，根据香农信息熵编码定理，相关性应该被去 掉才能减小文件大小。所以PS技术存储了一个声道的全部信息，然后，花很少的字节用参数描述另一个声道和它不同的地方。

LC-AAC，HE-AAC，HE-AAC v2比特率和主观质量之间的关系。由图可见，在低码率的情况下，HE-AAC，HE-AAC v2编码后的音质要明显好于LC-AAC

![](https://img-blog.csdn.net/20130924130118796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用不同的测试音频素材获得的HE-AAC（160kbps），杜比数字（384kbps），WMA（192kbps）的主观质量之间的关系。由图可见HE-AAC在160kbps的码率下的音质已经超过了杜比数字在384kbps码率下的音质，也超过了WMA在193kbps码率下的音质。足以见得其性能的强悍。

![](https://img-blog.csdn.net/20130924130219687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

LC-AAC，HE-AAC，HE-AAC v2比特率和主观质量之间的关系。由图可见，在低码率的情况下，HE-AAC，HE-AAC v2编码后的音质要明显好于LC-AAC。而在码率较大后（128kbps），其主观质量逐渐相同。

![](https://img-blog.csdn.net/20130924130632078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

参考文档：[http://tech.ebu.ch/docs/techreview/trev_305-moser.pdf](http://tech.ebu.ch/docs/techreview/trev_305-moser.pdf)



AAC解码器faad2源代码（VC工程）下载地址：

[http://download.csdn.net/detail/leixiaohua1020/6374877](http://download.csdn.net/detail/leixiaohua1020/6374877)






