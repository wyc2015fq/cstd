
# Pinterest给创业公司的启示：DevOps模式对业务的快速增长至关重要 - 雷锋网 - CSDN博客


2012年09月10日 11:31:00[leiphone](https://me.csdn.net/leiphone)阅读数：1908标签：[devops																](https://so.csdn.net/so/search/s.do?q=devops&t=blog)[创业																](https://so.csdn.net/so/search/s.do?q=创业&t=blog)[服务器																](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[facebook																](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=工作&t=blog)个人分类：[创业公司																](https://blog.csdn.net/leiphone/article/category/873394)
[
																								](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=创业&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=devops&t=blog)
![](http://www.leiphone.com/wp-content/uploads/2012/09/how-to-use-pinterest-to-market-your-business.jpg)Pinterest是一个号称”个人版猎酷工具”的美国视觉社交网站，网站创办于2011年，目前网站用户增长速度赶上了五年前的Facebook，是继[Facebook](http://www.leiphone.com/tag/facebook)、Twitter、Tumblr之后，又一个受世界瞩目的网站。根据营销服务公司Experian近期的研究数据，7月份Pinterest的在美国的每周浏览次数达到了2895万次，而去年一年加起来只有127万次浏览，总的浏览次数已经增长了5124%。单从数据来看，Pinterest公司对于如何实现业务快速增长还是颇有心得的。
Pinterest爆炸式增长速度背后的真正原因是什么？该公司的运营工程师Ryan Park为我们揭开了其中的谜底。在他看来，Devops模式在Pinterest爆炸式增长过程中扮演了非常重要的角色。
那么Ryan Park提到的[DevOps](http://baike.baidu.com/view/5705743.htm)指的是什么？传统的工作流程中，开发和运维之间存在很多的沟通错位而造成部署上的问题，由此，DevOps理念应运而生。DevOps是“开发”和“运营”的结合体，它是一组过程、方法与系统的统称，用于促进开发（应用程序/软件工程）、技术运营和质量保障（QA）部门之间的沟通、协作与整合。
 它的出现是由于软件行业日益清晰地认识到：为了按时交付软件产品和服务，开发和运营工作必须紧密合作。
![](http://www.leiphone.com/wp-content/uploads/2012/09/Devops.jpg)
DevOps模式最大的好处是可以更快速、更高效的交付所需的软件功能。更快的对软件进行更新并进行必要的修改，从而改善终端用户的使用体验。虽然DevOps模式很强大，但真正要开展起来并不容易。Park表示，要掌握诸如Puppet实验室的Puppet以及Opscode的Chef等配置工具，是个很艰难的过程，但是因为这种全新的业务模式很高效，依然有很多公司愿意去尝试它们。
那么公司转型到DevOps模式的过程中，怎样才能做到尽可能地减轻转型带来的痛苦并且创建持久可靠的业务流程呢？下面来听听Ryan Park为我们总结的经验之谈：
**1.采用自动化**
Ryan Park表示，你需要仔细对比现有的自动化工具，然后选择一种适合自己的进行使用。Pinterest使用的是Puppet配置管理工具，它可以将启动新服务器的流程自动化，最重要的是它可以自动配置正确的参数。
Park表示，“在以前，需要人工去配置很多集成服务的参数。参数跟踪、检查每台服务器做了哪些事情、增添新用户的数据，这些都需要人工去操作。人工操作不但让工作人员苦不堪言，而且可靠性不强，一般100台服务器只有91台服务器可以正常运行，而自动话工具则可以很好地解决这个问题。”
**2.创建单一可信来源**
虽然要做到这一点绝非轻而易举之事，但是通过使用Puppet或者Chef的控制面板，我们就能清楚地知道哪些系统是在本地的服务器上运行、哪些又是在亚马逊的EC2的云服务上运行。
Park表示：“养成搭建可信来源的习惯非常重要，系统需要知道所有服务器的名称，并了解哪些服务器正在进行分布式缓冲、哪些在运行其他的服务。但是，要将所有重要的信息集中起来，你还得输入必要的数据才行。
**3.尽量避免文化冲突。**
开发人员和运营人员的思维方式和工作方式内容大相径庭，主要是因为他们要达成的工作目标经常发生冲突。一方面开发人员希望尽快部署最酷的产品，而另一方面运营人员需要确保正在运行的系统能够正常的运行下去。如果能雇到懂运营的软件工程师或者懂开发的专业运营人员，将对公司的业务非常有帮助。当然前提是，两个团队都需要同公司的业务目标保持一致。
![](http://www.leiphone.com/wp-content/uploads/2012/09/WallOfConfusion.jpg)
**4.事先做好准备。**
对于那些已经拥有大型开发团队和运营团队的企业来说，在如何让双方更好地协作方面，总是会遇到不少的问题。而创业公司在创立之初，因为规模较少，就不存在这样的困扰，但是随着规模的壮大，如何合理制定工作流程、选择合适的技术就显得尤为重要了。Park表示，如果提前做了充分的准备，这种变化过渡就会更加顺畅。
**5.融入社区**
在各种各样的社区中，有很多专业的DevOps人士，所以要充分利用好这笔资源。订阅适当的邮件列表、参加聚会和供应商会议，可以扩大自己的交际圈并结实大量的专业人士，比如说本月底即将召开的Puppet大会。
Via[Gigaom](http://gigaom.com/cloud/pinterest-to-startups-devops-is-hard-but-do-it-anyway/)

**（****[Danice](http://www.leiphone.com/author/danice)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

