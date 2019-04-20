# spark源码导读一 - Spark高级玩法 - CSDN博客
2018年11月05日 00:03:14[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：110

经常有球友问，我分享了源码视频，那么该如何下手？
实际上，正常情况下，单纯爱好去看源码的情况不多，阅读源码无非就一下几种情况：
1，业务需求，现有框架满足不了项目需要，需要对源码的一部分进行修改或者叫做二次开发，需要阅读部分源码。那么这个针对性就很强了，修改不符合需求的部分。
2，寻求bug解决思路。比如，最近用flink，jsontablesource这种方式，一旦出现json格式非标，flink就会挂掉，由于flink开启checkpoint和savepoint，实现仅一次处理，那么就会导致flink拉不起来。哈哈一下，这里浪尖提了一个issue，然后这边定制改了一下。这种阅读针对性很强，就是为了解决问题。
3，追求真相，寻求调优。这种也是很有针对性去看的，比如spark的shuffle调优，网上文章一大堆，但是真的去按照他们文章推荐去调优了发现大部分不奏效。实际上，spark更新换代快，文章很容易老旧，这种情况下，就需要官网和源码，只有了解了真相才能尽可能做到最优。那么可能会有疑问就是，更新换代这么快，啥时候是阅读结束，只能告诉你，阅读源码是一个长期工程。但是完整的阅读完一个版本源码，后面就简单了，不信可以试试。
又有人问，浪院长，这个源码阅读有什么先后顺序吗？
答，从上面表述来看，可以认为没有什么先后顺序。
但是，假如你就想着完整的读一下比如spark的源码，那么这时候做法就有章可循了。
就是，按照自己在使用中对spark的运行原理的理解，去阅读源码核实自己的理解。
只有这样，带着疑问去读，你才有可能坚持下去而且学到很多，每一次揭秘都是成就感满满。
文章标题，说了是spark源码导读第一步，那么熟悉源码的你可以在没任何参考的情况下画出spark 的rpc的结构图吗？
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFXkoYDRG5goU7lzzmia9cR7PRVCXd16NNpG1XBGJuIrkF5MF0gcy2gTG8L8wYUjcP9siaAcMdwhYHAw/640?wx_fmt=jpeg)RPC架构图
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFXkoYDRG5goU7lzzmia9cR7PGKyR6ia6cskMia2mfePlpxBUpNAr67EqqB93mNZMEJGG9IE6RW9ORgGw/640?wx_fmt=jpeg)公众号
