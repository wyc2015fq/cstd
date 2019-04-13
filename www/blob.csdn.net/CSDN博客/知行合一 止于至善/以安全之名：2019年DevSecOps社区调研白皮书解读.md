
# 以安全之名：2019年DevSecOps社区调研白皮书解读 - 知行合一 止于至善 - CSDN博客

2019年03月10日 20:01:28[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：485标签：[DevOps																](https://so.csdn.net/so/search/s.do?q=DevOps&t=blog)[2019																](https://so.csdn.net/so/search/s.do?q=2019&t=blog)[DevSecOps																](https://so.csdn.net/so/search/s.do?q=DevSecOps&t=blog)[安全																](https://so.csdn.net/so/search/s.do?q=安全&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=DevSecOps&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=2019&t=blog)个人分类：[DevOps																](https://blog.csdn.net/liumiaocn/article/category/6328277)
[
																								](https://so.csdn.net/so/search/s.do?q=2019&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=DevOps&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=DevOps&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
2019年3月5号，Sonatype团队发布了一年一度的DevSecOps社区调研白皮书。这是DevSecOps的第六次年度的社区调查，在本年度共计包含对5，558份IT从业者的调研问卷的结果分析，是从安全角度来看DevOps实践的权威材料，而Sonatype也会在本年度3月28号展开详细的说明，让我们先睹为快。
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 调研机构
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)这份报告由Sonatype团队携手下列伙伴完成：
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)CloudBees
Signal Sciences
Twistlock
Carnegie Mellon’s Software Engineering Institute
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 数据来源
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)本次白皮书的调研数据主要信息如下：
[
](https://img-blog.csdnimg.cn/20190307160718183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)数量： 共计5，558份调研数据
受访者所在组织规模：67%的受访者所在组织的开发人员超过25人
受访者身份：普通员工（Individual Contributor）占到65.74%, 经理25.24%，而执行级别的高级管理者占到9.01%
受访者职责：DevOps/开发者/架构/IT经理占到整体的70%左右，详细构成如下图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307162305169.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
受访者公司所在行业：金融与科技是在实施DevOps是对安全最为关心的行业，两者加起来占据半壁江山。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307162638430.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
解读：
在实践DevOps时，行业不同，对于安全的重视程度也不同。从对于安全事件非常关注的金融和科技行业的从业人员占到整体受访者的半数，这并不是偶然。当然，重视归重视，口头上的重视和实际的执行状况是需要进行对比的。我们将继续分析这份报告。
DevOps成熟度
在这份报告中，对于受访者所在组织的成熟度级别也通过一系列的问题进行了确认，对于成熟度高和低的组织的实践进行对比和分析。根据2019年的反馈揭示了DevOps团队已经对安全的集成进行了自动化的探索，这类的“左移”实践也在开发/运维/安全团队的协作方面得到了很好的效果。而关于本次受访者的成熟的，调查显示，
接近一半（48%）的属于“改进中”的水平，较好（27%）和较差各占一半（25%）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308150540127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
接近一半（47%）的受访者每周进行多次生产环境部署
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308150705521.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在开发和部署方面，DevOps在金融/科技/通信行业得到了更多实践
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308151019789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
解读：
DevSecOps正像报告中所指出的那样，它能帮助我们保持竞争力，而且能够从最开始就进行安全地开发和部署。这种积极主动的方式降低了安全问题的冲击，而且使得一起井然有序，而不至于在出现问题时到处救火。
实践的动机
安全很重要，大部分人仍然只是说说而已
安全很重要，但是仍然有接近一半的开发者承认他们没有时间去对应此类问题。这个比率连续三年保持不可思议的稳定性，再次验证安全整体来说，仍然只是口头层面的重视。只有那些成熟度较好的团队，不仅从工具和流程，以及安全相关的培训都进行了开展。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308154222119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在整个软件开发生命周期SDLC中，客户的要求成为推动的一个重要动机
只有1/4左右的受访者认为安全是对于高质量的交付同等重要
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308154256890.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
解读：
在2017年DevOps和安全相关的一些话题的整理时，从很多企业安全相关的数据就得出了一个结论：安全是一个被所有人口头上非常重视，但是往往在实际的实践中选择性无视的一个话题。而2019年的调查，再次佐证了这一结论。忽视安全的重要性，将会对DevOps的推行产生灾难性的后果。不管速度多快，如果速度不是建立在安全和可用的基础之上的话，失败将是早已注定。DevOps实践需要在全环节进行安全要素的引入。
软件生命周期SDLC和工具
安全策略：在软件生命周期“左移”
工具：安全相关的工具选择众多
自动化：成熟度高的组织在安全的自动化集成方面做地更好
成熟度更高的DevOps组织在安全自动化集成方面达到了3.5倍的差距
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308155509694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
成熟度高的DevOps组织在应用安全工具的使用方面全面领先
从Web应用防火墙/容器和应用安全/开源管控/静态应用分析/动态应用分析等方面，成熟度高的组织全面领先。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308155754753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在自动化和手工的选择方面，成熟度高的组织在使用自动化而不是手工流程方面有高达7倍的优势
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308160117724.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
容器和云
哪里有DevOps，哪里就有云和容器
随着DevOps成熟度的提升，容器和云的高效加速了开发和运维的协作，而云和容器方面也有自己的安全问题需要对应，而在这个领域也一直有着持续的投入。
安全和合规性检查
成熟度不同的组织，在安全和合规性检查的对应，差别也非常明显
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308161043781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
51% vs 16% 的容器安全解决方案
关于是否能够提供容器的安全相关的解决方案，差别也非常明显
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308161200398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
对于开源的管控
在DevOps实践中速度的竞赛也推动了开源软件的浪潮，目前，已经有超过85%的现代应用开发建立在开源组建基础之上的。这极大地提高了软件对于开源组建的依赖性，但并不是所有的组件都是安全的，从OpenSSL的Heartbleed到Proodle和BAsh与Struts2，由于开源组件越来越多地被使用，整体相关的安全漏洞还在上升的趋势。
应用中的开源组件清单
关于是否有一张列表用来记录和管理在应用开发中所使用到的开源组件这件简单的事情上。根据受访者的反馈，状况如下所示：
53% vs %21
53%的成熟度较高的组织保持着对所使用的清单，而成熟度低的组织只有21%的比例
即使是成熟度较高的组织，对于应用组件缺乏应用的控制这一角度，也达到了47%
开源管控策略
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310093602471.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
关于开源管控的策略，有相关的管控策略并能执行的，在成熟度高的组织中达到了62%，而成熟度低的组织中这一比例只有25%。
[
](https://img-blog.csdnimg.cn/20190310093602471.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)过去的5年中与开源软件关联的漏洞上升了71%的比率
[
](https://img-blog.csdnimg.cn/20190310093602471.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310093946466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在2014年的调查显示有14%的受访者遇到了和开源组件相关的相关的漏洞（以OpenSSL的Heartbleed为代表的漏洞），而这一比例在2019的调查中达到了24%，五年上升了71%。
[
](https://img-blog.csdnimg.cn/20190310093946466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)成熟度高的组织在开源组件的安全方面付出了更多的关注
[
](https://img-blog.csdnimg.cn/20190310093946466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310094310629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在被问及是否在过去的一年中经历过的漏洞是由于开源组件或者依赖的脆弱性所导致的这一问题是，成熟度较高的组织的受访者的这一比例是27%，而成熟度较低的组织为21%。
[
](https://img-blog.csdnimg.cn/20190310094310629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)解读：
正如报告中指出的那样，DevOps实践有助于当下更加灵活和快速地交付高质量的产品。而安全作为产品研发中关键的一环是不可或缺的。只有当安全无缝地进程进了软件开发生命周期之后，才是对DevOps实践的真正拥抱和迎接。
合规审计和安全加密
合规即代码
很多DevOps实践推动实现“合规即代码”，通过将合规策略进行固化使得对于开发和运维的审计跟踪能够被自动化集成到每日的工作之中。在以代码方式进行实现和集成之前，策略和工作流程的变化的审批和记录应该先进行标准化和规范化。
密件安全
代码或者密钥直接在仓库中进行管理，关键数据未被加密，关键业务的数据库和账户信息未得到很好的保护，甚至直接在Github上进行暴露。而这样的数据安全漏洞往往会造成灾难性的后果，这方面还有很多改进可做。
审计追踪
关于何人何时做了何事的追踪，是否进行审计跟踪这方面的调查，在成熟度高的组织中，开发这可以通过工具监控和审计所有有软件开发生命周期相关的环境变化，而成熟度较低的组织更多地一考虑手工过程，整体缺乏可追踪性。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310192509662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
准生产环境和生产环境是否进行部署内容的世代管理
随着持续集成和部署的实践深入，多次部署内容如果不进行合理的管控，会导致回滚出现问题或者磁盘空间不足，在生产环境和准生产环境部署的工件是否进行管控可以看出，仍然有达到28%的受访者未做任何处理
![在这里插入图片描述](https://img-blog.csdnimg.cn/201903101938331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
应用程序级别的密件加密
关于应用程序级别的密件加密，成熟度较高的组织只有1/4未作处理，而成熟度较低的组织高达54%的比例未做处理。
最大的挑战
快速且可控
在DevOps流水线中集成应用和操作安全实践相关的成文的固化安全策略时，DevSecOps的实践变得更加容易，而在今年的调查中，对于更好的管控安全相关的问题，最大的挑战被认为发现安全问题在整个流程中太晚，导致很多返工和速度的拖慢。
挑战排名
关于安全相关的挑战，在今年的调查中，发现问题的时机太晚成为各种成熟度一致认为的最大挑战。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310194843888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
安全策略或者团队的融入是否会拖慢软件开发团队
关于安全策略或者安全团队的融入是否会拖慢软件开发团队的进度这个问题上，受访者所在组织的大小直接影响结论的作出。少于100开发者的小团队整体上认为这不构成主要问题，60%左右的团队成员认为不会拖慢。而开发人员多余5000的大企业中，认为不会拖慢的人员比例仅占47%。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310195235111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
漏洞和对应
影响现状
1/4左右的企业在过去的一年中受到过安全漏洞的影响，这一问题相较于五年前，有了大幅的上升。
对策和计划
根据调查结果显示，成熟度较高的组织对于网络安全反应相关的对策和计划也明显高于成熟度较低的组织。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190310195621761.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
总结
安全生产，警钟长鸣。但是说到底，还是要落到企业的战略上，连续三年都认为安全问题非常重要，连续三年有一半的受访者都表示没有时间去对应安全问题，注定这一年的DevSecOps仍然在一定程度上流于形式。在安全方面，口是心非的根本原因不解决，安全在实际的实践中就很难得到真正的落地。

