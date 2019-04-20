# 负责任的说，Java仍然免费 - Spark高级玩法 - CSDN博客
2018年11月29日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：176
最近有关Java收费的谣言不断，本不想对此发表评论，毕竟一些负责任的媒体早已经澄清了多次，我自己的立场也不中立，而且，仔细阅读那些文章，你会发现往往第一句话就是自相矛盾的呀。
不过，还是有一些企业的朋友不断咨询，甚至包括一位曾经工作于Java组的前同事，以后使用Java是否真的存在法律风险吗？Java未来还会发展吗？看来谣言带来的换乱超出了想象。
有鉴于此，明确一点，**Java****仍然免费**！并且针对大家的主要疑问，谈谈个人的观点：
第一，**“我公司想要使用JDK 11等新版本，但不想付费，怎么办？” **
企业用户仍然有可靠途径获取**免费**的JDK，既可以使用Oracle等发布的GPL协议的OpenJDK，也可以使用AdoptionOpenJDK社区等发布的OpenJDK产品，甚至是有能力的用户自己从源码构建！
第二，**“我不想升级JDK，目前的JDK 8还可以继续使用吗？”**
当然**可以**！这就是正常的软件EOL，OracleJDK 8只是在支持了近5年后，停止了免费更新，注意是“**更新**”。继续使用JDK8的用户，和目前停留在JDK 6、7的用户没有什么不同。为方便理解，大家可以对比现在还使用Windows 7或XP的客户，仅是无法获取安全更新，但并不会丧失使用的权力，这取决于用户自己的判断和选择。
第三，**“如果还想要得到JDK 8的更新怎么办？”**
这个就要看你使用的是什么JDK了，咨询下相应厂商的策略吧。大多数**企业**用户应该是在使用Oracle JDK，那么就需要订阅Oracle JDK支持服务才可以更新了，当然**个人用户**还可以继续免费更新到2020年12月。其他如Azul、Redhat、IBM等厂商的用户也是需要确认相关路线图，对于JDK 8的用户，这是唯一可能需要掏钱的情况。
第四，**“Java是GPL授权，我是不是要将Java开发的应用也开源？”**
**不**，这纯属无稽之谈，Java开发的应用与OpenJDK的GPL授权没有关系！
不止一个朋友表示了对Java GPL v2 +CPE协议授权的担心，源头似乎是最近某开源项目的宣传卖点，这个只能是佩服其想象力和价值观了，连GPL都能成为攻击点，真是掌握了核心竞争力了～
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/CT1oRwOQ009fdCIXosI8O7aGN61Wqianh1DXgjtdegpwYPGI1mFc2EeQzJIcicRBqI1ucxe23rpuLVLiaZv6sBG6g/640)
OpenJDK的GPL授权，只会约束基于OpenJDK源码**生产并发布****JDK****产品**的厂家，而**不是**Java开发的**应用**，绝大部分用户根本就不可能是JDK生产商，所以这个担心完全没有必要，而且，Java不是今天才开源的，使用GPL授权的开源项目比比皆是，如Linux，这也是业界最普遍、最受欢迎的授权选择之一。
好了，不在这种问题上纠缠了，态度严肃的IT决策者可能一直就觉得这是个笑话，也许大家会好奇，**为什么Java最近引发了如此的困惑呢？**
个人认为，这是历史原因和OpenJDK社区正在发生的变化叠加的影响，而且国内的部分自媒体也只在乎是否博眼球：
第一，**JDK ****！****= Oracle JDK**
长久以来，由于Oracle提供了近乎“免费”的JDK，让大多数Java用户，尤其是个人和中小企业，产生了这样一种错觉“JDK = Oracle JDK”，尽管，IBM、Azul、Redhat等都有自己的JDK产品公开发布，企业用户也大有人在。这种深度和自然的依赖，导致JDK 8免费支持期的结束令人产生了巨大的不适应。
第二，JDK 8等太成功了，后续的JDK得到采纳还需要一个过程。OpenJDK一个约定成俗的做法是，经过多年免费支持以后，该Oracle JDK版本进入EOL（End of Life），然后让出领导权，由社区接手并主导后续更新事务。
但是，JDK 8经过了近5年的发展，仍然很受欢迎，用户没有准备好，也没有足够动力去升级JDK，更不用提好多还在用JDK 6、7的用户了，这也许是所有基础软件面临的困扰吧。
第三，OpenJDK的权力中心正在向社区转移！Java正在逐步采用更加快速的迭代周期（半年一个大版本），更加开放的产品特性（Oracle开源了ZGC、AppCDS、JFR等）和更积极的社区协作（例如，ProjectSkara项目将把OpenJDK代码库等切换到github）等。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/CT1oRwOQ009fdCIXosI8O7aGN61Wqianh5ELSf3E7rzDMHWyBhicS1lRAbnSzJoJRgfXkEuoFIiaw1xJcrXB2ECGg/640)
虽然长远来看很可能是利好，但不可避免地带来了困惑，因为开发者需要对Java生态的长久信心，企业IT决策者需要可预计的Java路线图，这些恰恰是往往来源于核心厂商的长期主导，“自由职业者其实最喜欢的就是五险一金！”。
好了，最后再次明确，Java仍然免费，OpenJDK正在变得并且更加开放和活跃，Java/JVM拥有清晰的发展路线图，时间会说明一切。套用官宣用语，Java开发者和IT决策者可以吃下“定心丸”了:-）
参考：
关于这个问题Java Champion早就发表了公开信，但在Google Docs里，你懂的，看这个吧
https://www.infoq.com/news/2018/09/java-support-options-sept18
https://www.oracle.com/technetwork/java/eol-135779.html
https://adoptopenjdk.net/
文中图片来自于Google Image，版权属于原作者。
本文转载自「技术琐话」。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎转发分享～
