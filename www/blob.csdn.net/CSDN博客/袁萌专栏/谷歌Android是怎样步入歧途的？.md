# 谷歌Android是怎样步入歧途的？ - 袁萌专栏 - CSDN博客
2011年03月25日 06:24:00[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1213

	
	
	
	p { margin-bottom: 0.21cm; }
    我们看问题要追根溯源，不能无根无据地瞎说一通。谷歌Android到底从何而来，想干什么？很据何在？
    根据维基百科最新资料（截止今年3月24日），我们知道，在2003年10月，Andy
Rubin和Rich
Miner创办了Android公司，位于美国加州。公司的名称大概与创始人名字多少有点“关联”，但是，之所以用机器人“Android”来给公司命名，与该公司的发展目标有关。Andy
Rubin说："...smarter
mobile devices that are more aware of its owner's location and
preferences"，意思是说，聪明的移动设备（指机器人）能够更好地知晓其主人所在的位置与喜好。这个想法正好也是移动互联网的发展目标。2005年8月，谷歌一口吞下Android（被并购），准备进入移动互联网市场。
    来到谷歌，Andy
Rubin团队基于Linux内核（v2.6）开发了一款移动设备平台（Android原型）。于是，谷歌手中拿着这个“Android宝贝”，到处游说、招揽各大型互联网厂商，终于在2007年11月5日成立了“Open
Handset
Alliance”，阵容非常壮观，包括一大串知名厂商，比如：motorola、高通、......等等，目的是发展移动设备的开放标准，就在这同一天，谷歌的首款Android产品(平台级)发布了。
    可以说，谷歌市场推广做得相当成功，看似一帆风顺，但是，危机四伏。虽然说，Android内核是从Linux导出的，但是，谷歌却在Linux主干树（main
tree）的外部进行了“tweaked”(“凑合”)，在Android系统中没有本地X
Window系统，Android也不支持GNU库的全部标准，跟现有的GNU/Linux系统之间存在很大“间隙”。从实质上来看，谷歌是把Android搞成了一个Linux的“fork”（“分叉”），结果，在2010年1月，Linux内核管理者把谷歌不再继续维护的Android代码从Linux代码库中删除了。至此，Linux与谷歌的android系统划清了界限。
    实际上，谷歌Android开发团队与Linux开发团队之间的摩擦直到今年1月份仍然继续存在，比如，在去年12月，Linux内核维护者Greg
Kroah-Hartman说：“Google
was no longer trying to get their code changes included in mainstream
Linux”，而且，某些Android开发者暗示：“......was
getting fed up with（厌倦）
the
process“，因为他们自称是一个小团队，并且说：“....had
more urgent work to do on Android”。由此可见，两队开发人马终于分手了。
    由于谷歌一味蛮干，招惹来不少官司，惹了不少麻烦，终于走上了歧途。。对此，《自由软件基金会》（FSF）批评谷歌，写道："It's
sad to see that Google apparently shunned（避开）those
protections in order to make proprietary software development easier
on Android"，并且说：“Google
had not taken any clear position or action against software
patents”，意思是说，谷歌不走正道，没有采取清晰的立场与行动来抵御有关软件专利的侵权官司。这句话说得十分中肯。
