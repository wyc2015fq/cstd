# Linux保卫战已经打响 - 袁萌专栏 - CSDN博客
2011年03月21日 06:21:00[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1518标签：[linux																[android																[linux内核																[英特尔																[meego																[webos](https://so.csdn.net/so/search/s.do?q=webos&t=blog)](https://so.csdn.net/so/search/s.do?q=meego&t=blog)](https://so.csdn.net/so/search/s.do?q=英特尔&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
	
	
	
	p { margin-bottom: 0.21cm; }
    近日，谷歌肆意篡改Linux炮制Android的恶行被揭露出来之后，躲在一旁不吭声，伺机反扑过来。这是一场Linux保卫战，我们不能袖手旁观，装作“看不懂”。为什么？
      3月20日，我在《谷歌Android篡改Linux内核的真相》一文中，已大体说明了问题的性质，在此就不再重复了。3月18日，Matthew
Broersma在欧洲《eWeek》上发表署名文章，题为“Experts:
Android May Violate Linux Rules”，更进一步说明了问题的严重性。该文作者说，“**Google’sapproach to the Linux kernel with Android may be leaving it open tolegal attack”****，同时，作者指出问题的焦点集中在谷歌自己****“****发明****”****的所谓****“****Bionic****库****”，即“****betweenthe Linux kernel and user-facingprograms”****之间的接口****库****，并且用****Bionic****库****（****BSD****）****来取代****Linux****原有的****Glibc****库****（****LGPL****）。因为，在谷歌看来，****Glibc****库必须遵守****LGPL****规则，确实妨碍它的****Android“****群狼战略”，因此，谷歌自己发明****Bionic****库来故意规避之。****让谷歌没有想到的是****，如此以来，不仅把自己放在火上烤，而且也把数十万不知情的****Android****开发者拖下了水。**
**    谷歌用自造的****Bionic****库替代****Linux****的原有****Glibc****库迟早要招来官司，而其竞争对手英特尔的****MeeGo****和****惠普的****WebOS****，虽然后两者也都是移动操作系统，但是，它们仍然都使用****Linux****原有的****Glibc****库（根据该文作者所引用的资料）****，由此可见，在这方面****，****英特尔与惠普都是老实人。**
**那么****，谷歌****Android****的前景将会如何呢？有关专家（即****Mneller****）说：“****Ifone of the many Linux kernel copyright holders decides to sue Google,it could result in a disaster****（不幸）****forthe Android developerecosystem****”****，意思是说，只要有一个****Linux****内核开发者（程序版权所有人）站出来控告谷歌的如此劣行，结果将是整个****Android****开发者“生态系统”的不幸****（****disaster****）****，类似日本的****9****级特****大****地震****。**
**    可以公平地说，现今****，****在中国境内没有几个****Linux****内核（指最新版本）的个人开发者，由此****，****谷歌以为自己就可以随意蒙骗我们中国人了。是的****，我知道谷歌在中国有不少****Android”****粉丝”，实际上，谷歌的****Android“****粉丝”对于谷歌用****Bionic****库替换****Glibc****库的真实用心（及其商业目的）并不完全清楚。现在，真相终于大白，不少****Android“****粉丝”一定会慢慢醒来。如你不信，你可以自己把****Bionic****库与****Glibc****库两者拿出来做一****番****“对比”研究，结果就会出来了****。**
**    说明****：一般而言，外国专家说的话（或论断），我们中国人的耳朵听不进去。****......****昨天，我打电话通知深圳方面（单位名称暂时保密）替我找几个人（从深圳大学找），准备实地验证一下谷歌****Android****（即****Bionic****库****）****的合法性，为我国****Linux****的“正版化”做一点儿实际贡献（即****Linux****保卫战已经打响）。**
**    注：此项目不需要向国家申请多少课题经费，也许几十块钱就够了。**
