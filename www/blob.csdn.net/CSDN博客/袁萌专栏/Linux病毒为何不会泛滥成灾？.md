# Linux病毒为何不会泛滥成灾？ - 袁萌专栏 - CSDN博客
2011年02月28日 14:14:00[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1638标签：[linux																[linux内核																[网络应用																[webos																[制造																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=制造&t=blog)](https://so.csdn.net/so/search/s.do?q=webos&t=blog)](https://so.csdn.net/so/search/s.do?q=网络应用&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
	
	
	
	p { margin-bottom: 0.21cm; }
     大家知道，Linux不是没有病毒，而是Linux病毒不易四处传播，感染他人。此话，根据何在？
        2月28日，看了《第一财经日报》一篇报道，题为“揭秘病毒产业链运作：操纵流量一年获益数亿”，把我吓了一跳。制造病毒，形成病毒产业链，如此就会发大财，......真是匪夷所思也！那么，我们该怎么办呢？在这里，我想对大家说的是，Linux病毒就不会这么猖獗，表现温柔多了。此话当真？
    记得，在5～6年前，我就Linux病毒的传播机制问题写过一篇短文。现在，我把当时所参照的原文又找了出来，以便与大家一块研究。原文题目很有趣味，是科普性文章，叫做“The
short life and hard times of a Linux
virus”（“一个Linux病毒的短暂寿命与艰难岁月”）。此文，最早写于2000年6月10日，修改于2005年7月30日，具有一定的参考意义。
该文最核心的论据（之一）是，一个Linux二进制病毒要想感染某些可执行程序，那么，这些可执行程序必须具有被这个用户所激活的病毒“写入”的可能性。但是，一般而言，“the
programs are owned by root and the user is running from a
non-privileged account”，意思是说，用户没有这样高的权限。由此，我们不难理解，Windows病毒为何那么猖獗了。
    该文的结束语是：这不意味着，Linux病毒永远不会成为一种流行病（epidemic），而是说，“a
successful Linux virus must be well-crafted and innovative to succeed
in the inhospitable Linux
ecosystem”，意思是说，一个成功的Linux病毒必须是手法高超且具有非凡创意的软件作品，才能在这个“不好客”的Linux生态系统中获得最终的繁衍不断（succeed）。
    从历史上来看，1996年秋，第一次发现Linux病毒Staog，后来，在1997年1月31日，又发现一种类似木马的Linux病毒Bliss，结果发现，两者均与Linux内核存在缺陷有关，缺陷改正之后，就不易发生类似现象了。直到现在，十几年过去了，没有实际发生任何Linux病毒的大流行，更谈不上“泛滥成灾“。应该说，Linux病毒非常温柔，只是一个“话题”罢了。
    近年来，病毒厂商发了大财，不是因为他们有“能耐”，而是他们运气好。现在，苹果的iPad，谷歌的Android，惠普的webOS都不怕病毒，只是有些应用程序表现鬼鬼祟祟（比如：“吸费”），类似木马，只要从正规网络应用商店取得应用程序，一般不会发生这种问题。当今，病毒制造者能够发大财，是草根百姓的不幸，是业务主管们（特别是“高管”）的失职。我们无可奈何，乌乎！

