# NSA 是如何破解大量加密信息的？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [zer0Black](http://www.jobbole.com/members/lxtalx) 翻译。未经许可，禁止转载！
英文出处：[Freedom to Tinker](https://freedom-to-tinker.com/blog/haldermanheninger/how-is-nsa-breaking-so-much-crypto/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
近几年有谣言说 NSA 可以解密已加密网络中的大部分数据。在 2012 年，James Bamford 发布了一篇 [文章](http://www.wired.com/2012/03/ff_nsadatacenter/)，引用了匿名的 NSA 前成员的说法，他证实“NSA已经取得了计算力的突破性进展，他们有能力破解当前已公开的加密算法”。斯诺登公布的文档也暗指像一些额外的线索和文档显示，NSA已经建立了大量的基础设备来拦截和解密 VPN 流量，它至少能解密一些 NSA 想要知道的 HTTPS 和 SSH 连接。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1f671kiqgjqj20fs0ajdiy.jpg)
然而，对于这些超前的工作是如何运作的，技术小组是如何判断后门或逻辑缺陷的等问题，这些文档都没有说明。2015 年 10 月 13 日，在 ACM CCS 会议的安全研究分会场，我们和 12 个同行发布了一篇[技术谜题揭秘报告](https://weakdh.org/imperfect-forward-secrecy.pdf)。
Diffie-Hellman 秘钥交换算法，是我们推荐使用的防御监听的算法。它是现代密码学的基石，VPN、HTTPS 网站、邮件和许多其他的协议都用在 Diffie-Hellman。我们的报告显示，通过数理理论的交汇和糟糕的协议实现，许多现实世界里的 Diffie-Hellman 用户在面对国家级攻击时都是脆弱的。这个事实稍微有点讽刺。
如果读者里有技术狂人，就会发现一些问题：如果客户端和服务端要使用 Diffie-Hellman，它们首先需要协定一个特殊形式的大素数。为什么不能每个人都用同样的素数？似乎不用问什么原因。实际上，许多应用倾向于使用标准的或者硬编码的素数。但算法从数学家到实现者之间，会丢失一个非常重要的细节：攻击者可以执行一个单点大规模计算来破解大素数，使用该大素数的个人连接很容易丢失。
你要问计算量有多大？大概是整个二战时期破解英格玛的计算规模（相较于那时候的计算量而言）。甚至连估算难度都很棘手，因为这个算法涉及到的内容太复杂，但我们有论文给出了一些保守的估算。对于最常用的 Diffie-Hellman（1024位），花了几亿美元来建造专门的破译机器，这能够每年破解一个 Diffie-Hellman 素数。
对情报机构而言，这值得么？一旦少量的大素数被滥用，那他们可解密的连接，将会很可观。破译一个通用的 1024 位大素数，将让NSA 能解密全球三分之二的 VPN 和四分之一的 SSH 服务器。破解两个1024 位大素数，将能够被动监听上百万个 HTTPS 网站中的20%。总而言之，在大规模计算上的一次投资，使它能够监听数以兆计的加密连接。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f671kk4hgaj20jy0dbabi.jpg)
NSA 能够负担这些投资。在斯诺登泄露的部分文件里，有一份 2013 年的[黑色预算](https://www.eff.org/files/2013/11/25/2013-08-30_cryptome_congressional_budget_2013.pdf)的申请，文件显示 NSA 已经将“发展突破性的密码分析能力以打击敌对方密码系统和利用网络流量”提上了日程。它显示了 NSA 的预算大约一年有100 亿美元，其中超过 10 亿被用于计算网络技术开发和几个子项目中。
基于我们已有的证据来说，我们无法确切证明 NSA 已经完成了。然而，我们提出的 Diffie-Hellman 攻击方式，相较于其他可能性，更能解释在当前已知的技术水平下如何获得大规模破解的能力。例如，斯诺登发布的文档显示了 NSA 的 [VPN解密设施](http://www.spiegel.de/media/media-35526.pdf) 通过拦截加密连接，并使用超级计算机计算已知数据来得到密钥。这套系统的设计不遗余力的收集必要的特殊数据，为攻击 Diffie-Hellman 做准备。但它不适合 AES 或其他对称加密算法。这份文档清晰是显示了 NSA 使用其他类似软硬件“移植”的技术来破解特殊目标的加密算法，但这些并没有解释 NSA 大规模被动监听 VPN 信道的能力。
一旦 Diffie-Hellman 这种不牢靠的使用方式在标准和实现中被滥用，这个问题将会影响到许多年后，甚至给现有的安全推荐和我们新的调查带来影响。与此同时，其他强大的政府也有可能实现类似的攻击，假设他们还没那么做的话。
我们的调查阐明了 NSA 两项任务间的矛盾，收集情报和保卫美国网络安全。假如我们的猜想是正确的，情报机构已经掌控了弱 Diffie-Hellman，帮忙修正这个问题仅是小小的一步而已。在防守方，NSA 推荐大家应该向椭圆曲线密码学过渡，椭圆曲线密码学没有已知的漏洞，但这一推荐在没有明确推论或证明的情况下，就被大多数人忽略了。这个问题太复杂，因为安全通过采取 NSA 推荐，表面上价值不高。看看这这个显而易见的影响：[后门密码标准](https://en.wikipedia.org/wiki/Dual_EC_DRBG)。
这种状况让人人的安全都处于危险中。这种规模的漏洞是不加区别地影响着大家的安全，包括美国公民和公司，但我们希望，能有一种更清晰的技术，来了解 ZF 监控背后的密码机制，为大家能有更好的安全。
更多详情，请见我们的研究论文：《[Imperfect Forward Secrecy: How Diffie-Hellman Fails in Practice](https://weakdh.org/imperfect-forward-secrecy.pdf)》（更新：我们这篇论文获得了CCS 2015 最佳论文奖！）
*[J. Alex Halderman](https://jhalderm.com/)*是密歇根大学计算机科学与工程的助理教授，密歇根计算机安全和社会中心的主管。
*[Nadia Heninger](https://www.cis.upenn.edu/~nadiah/)*是宾夕法尼亚大学计算机与信息科学系助理教授。
