# 我的Java后端书架 - 程序员江湖 - CSDN博客





2017年04月02日 16:34:20[黄小斜](https://me.csdn.net/a724888)阅读数：872
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









﻿﻿**这位大侠，这是我的公众号：程序员江湖。**
**分享程序员面试与技术的那些事。 干货满满，关注就送。**
![这里写图片描述](https://img-blog.csdn.net/20180827211353568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 我的Java后端书架 (2016年暖冬4.0版)



                                              原文出处： [江南白衣](http://calvin1978.blogcn.com/articles/javabookshelf.html)

书架主要针对Java后端开发。

3.0版把一些后来买的、看的书添补进来，又或删掉或降级一些后来没有再翻开过的书。

更偏爱那些能用简短流畅的话，把少壮不努力的程序员所需的基础补回来的薄书，而有些教课书可能很著名，但干涩枯燥，喋喋不休的把你带回到大学课堂上昏昏欲睡，不录。

### 1. 操作系统与网络的书

[《Linux内核设计与实现 第3版》](http://book.douban.com/subject/6097773/)

Robert Love用最薄的篇幅，顺畅的文字将Linux内核主要的算法讲清楚了，[《深入理解Linux内核》](http://book.douban.com/subject/2287506/)，[《深入Linux内核架构》](http://book.douban.com/subject/4843567/)之类厚厚的全是代码，不是专门的内核程序员看这本足够了。

[《Linux系统编程 第2版》](http://book.douban.com/subject/25828773/)

继续是Robert Love，比起APUE也是以薄见长，专门针对重要的系统调用讲解。

[《性能之巅》](http://book.douban.com/subject/26586598/)

操作系统的性能调优、监控、工具和方法论，看这本就够了，已经足够厚，可能是书单里最厚的一本。

[《TCP/IP详解 卷1:协议》](http://book.douban.com/subject/1088054/)

这么多年过去了，TCP的书好像主要还是只有这一本，有点旧了，看了也还是半懂不懂的。后人在2011年写了[第二版](http://book.douban.com/subject/10742272/)，机械工业正在翻译。

[《WireShark网络分析就这么简单》](http://book.douban.com/subject/26268767/)和 [《WireShark网络分析的艺术》](http://book.douban.com/subject/26710788/) new!

多少人，是看了这两本轻松又实战的书，才真正理解TCP的细节。

![](https://img1.doubanio.com/lpic/s6387169.jpg)![](https://img3.doubanio.com/lpic/s27303690.jpg)![](https://img1.doubanio.com/lpic/s28283438.jpg)![](https://img3.doubanio.com/lpic/s1543906.jpg)![](https://img3.doubanio.com/lpic/s28453336.jpg)

PS：[《UNIX环境高级编程》](http://book.douban.com/subject/25900403/)和[《UNIX网络编程》](http://book.douban.com/subject/1500149/)，APUE和UNP更多作为一本超厚工具书存在。[《Unix 编程艺术》](http://book.douban.com/subject/1467587/)，扯的都是闲篇，厚厚的一本其实略读一下就行。 [《现代操作系统 第3版》](http://book.douban.com/subject/3852290/)如果看LKD未尽兴，可以回头看看这本基础概念，感觉比那本枯燥的[《操作系统概念》](http://book.douban.com/subject/4289836/)(恐龙书)读起来舒服。

[《TCP/IP指南》](http://tcpipguide.com/free/t_toc.htm) 前面wireshark书作者的推荐，网上有英文免费版，然后有中文版的[卷1](http://book.douban.com/subject/3062468/)和[卷2](http://book.douban.com/subject/3082451/)，但可能那么多章节那么厚你只关心TCP和HTTP两部分。[《HTTP权威指南》](http://book.douban.com/subject/10746113/)，同样是自己从厚厚的目录里挑选感兴趣的章节来看。另外，那些日本韩国人写的《图解XXX》感觉都不喜欢，真的不行。

### 2. 算法的书

[《数据结构与算法分析-Java语言描述 第3版》](http://book.douban.com/subject/26745780/)

够薄，数据结构与算法分析的点基本都涵盖了，而且喜欢它的示例代码是Java写的，新出了第3版。

[《算法 第4版》](http://book.douban.com/subject/10432347/)

可与上一本对比着读，厚一些，也多些图，但知识点没上面的全，也是Java的。

![](https://img3.doubanio.com/lpic/s28518223.jpg)![](https://img1.doubanio.com/lpic/s8938479.jpg)

PS： [《数学之美》](http://book.douban.com/subject/26163454/)、[《编程珠玑》](http://book.douban.com/subject/26302533/)，都是专栏文章，讲得并不系统，可以当兴趣读物来看。
[《算法设计与分析基础 第3版》](http://book.douban.com/subject/26337727/)数学系偏爱无比枯燥很多公式的[《算法导论》](http://book.douban.com/subject/20432061)， 计算机系喜欢这本实用主义的典型。

### 3. 架构设计的书

[《软件系统架构：使用视点和视角与利益相关者合作 第2版》](http://book.douban.com/subject/24530471/)

也是教科书，最难得的是，这本老书在十年后的去年升级了第二版，所以感觉鲜活了好多，也许是最鲜活的一本架构书。

[《恰如其分的软件架构 – 风险驱动的设计方法》](http://book.douban.com/subject/24872314/)

由于人类与生俱来的惰性，计算机原本科学的、精准的设计方式，有了敏捷的借口之后就很难再维持了。本书就是在这种背景下，提出由风险来决定设计的度。除了开始的风险驱动部分，其余部分就是规规矩矩标标准准的架构师教科书。

[《发布！软件的设计与部署 – Release It!: Design and Deploy Production-Ready Software 》](http://book.douban.com/subject/26304417/)

关于高可靠性的软件，学校里不会教，出来社会却要面对的那部分，英文的原标题更清晰。

[《大型网站技术架构:核心原理与案例分析》](http://book.douban.com/subject/25723064/)

淘宝出品，大型互联网站的科普入门书。

[《高扩展性网站的50条原则》](https://book.douban.com/subject/10756899/) new!

同是入门级读物，如果还有个高可用50条原则，那就齐了。

[《微服务设计》](http://book.douban.com/subject/26772677/) new!

那么多微服务的书，还是这本比较不像赚快钱的。

[《大数据日知录》](http://book.douban.com/subject/25984046/)

前几年参加各种技术会议，CAP，最终一致性，RWN，向量时钟，Paxos，一致性哈希，Gossip什么的能灌你一耳朵。而现在，你只要在家安安静静的看书就够了。不过这个领域发展太快，又一年过去了，期望它可以持续出新版。

![](https://img1.doubanio.com/lpic/s27190968.jpg)![](https://img1.doubanio.com/lpic/s26859279.jpg)![](https://img1.doubanio.com/lpic/s27986278.jpg)![](https://img3.doubanio.com/lpic/s27250675.jpg)

![](https://img3.doubanio.com/lpic/s9126724.jpg)![](https://img1.doubanio.com/lpic/s28626997.jpg)![](https://img3.doubanio.com/lpic/s27465082.jpg)

PS： 关于[设计模式](http://www.amazon.cn/gp/product/B001130JN8/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&tag=importnew-23&linkCode=as2&camp=536&creative=3200&creativeASIN=B001130JN8)，我以前曾经有过很多很多本，GOF23啦，企业应用架构模式啦，EIP啦， POSA 5卷本啦，反模式啦，JavaEE/SOA/Restful的模式啦。但现在觉得对新人来说，一本Java写的[《Head First 设计模式》](http://book.douban.com/subject/2243615/)，知道什么叫设计模式就够了。

[《程序员必读之软件架构》](http://book.douban.com/subject/26248182/)作者维护着[codingthearchitecture.com](http://codingthearchitecture.com/) 。不过中文书名叫“必读”有点过。

### 4. 语言的书

[《Java并发编程实战》](http://book.douban.com/subject/10484692/)

Java并发经典，人手一本不用多说了。

[《实战Java高并发程序设计》](http://book.douban.com/subject/26663605/) new!

国人新作，流畅易读，内容也比上面一本来得新。

[《深入理解 Java 虚拟机 第2版》](http://book.douban.com/subject/24722612/)

理解虚拟机并不是那么难，Java程序员来说，很多知识其实是必须的。另外还有几本类似主题的书，忽然一下子都出来了。

[《Java性能权威指南》](http://book.douban.com/subject/26740520/) new!

比起多年前那部调优圣经，讲得更加深入，也更加贴近现在的JDK。可以从里面挑些知识点来，做Java调优的面试题。

[《有效的单元测试》](https://book.douban.com/subject/26364867/) new!

不同于那些动辄BDD的高深书籍，专注于如何写“好”的，可维护的单元测试，拿来给团队看，能省很多口水。

[《七周七语言》](http://book.douban.com/subject/10555435/)

《七周七X》系列的开山之作，可能也是最好的一本。

![](https://img3.doubanio.com/lpic/s7663093.jpg)![](https://img3.doubanio.com/lpic/s28339364.jpg)![](https://img3.doubanio.com/lpic/s27458236.jpg)![](https://img1.doubanio.com/lpic/s28487617.jpg)

![](https://img3.doubanio.com/lpic/s28046831.jpg)![](https://img3.doubanio.com/lpic/s9046771.jpg)

PS：[《Effective Java》](http://book.douban.com/subject/3360807/)外界一致推崇，但有点太过誉了。另外[《Thinking in Java》](http://book.douban.com/subject/2130190/)有点旧了，而且作者思路随意，译者语言晦涩，新程序员还是建议同时再看两卷[《Java核心技术 － Core Java》](https://book.douban.com/subject/25762168/)。

### 5. 具体技术的书

[《Docker: 容器与容器云》](http://book.douban.com/subject/26593175/)

这本书叫Docker一本就够了，的确够了，在那些Docker操作指南书之上。不想着改Docker代码的看它就够了，别想着什么《Docker源码分析》。

[《Redis设计与实现分析》](http://book.douban.com/subject/25900156/)

用Redis的工程师桌面必备吧。

[《分布式服务框架：原理与实践》](http://book.douban.com/subject/26702824/) new!

如果要写一个SOA框架，要留意的方方面面。

[《Spark技术内幕》](http://book.douban.com/subject/26649141/)

深度与厚度之间，选了这本200页的薄书，一样有很多的原理与代码解释，但不会像有的书那样贴20行代码只写一行字。

[《Netty权威指南 第2版》](http://book.douban.com/subject/26373138/)

虽然网上的吐槽较多，但Netty 快速入门也只有这一本了。

![](https://img3.doubanio.com/lpic/s28606455.jpg)![](https://img1.doubanio.com/lpic/s27297117.jpg)![](https://img3.doubanio.com/lpic/s28378000.jpg)![](https://img3.doubanio.com/lpic/s28324850.jpg)![](https://img3.doubanio.com/lpic/s28055322.jpg)

### 6. 程序员的自我修养

PS. 最近没买什么新书，随便说点旧书:

[《程序员修炼之道-从小工到专家》](http://book.douban.com/subject/1152111/)，Pragmatic Programmer-注重实效的程序员开山之作，翻译的马达维文笔也和熊节一样好。

[《代码整洁之道》](http://book.douban.com/subject/4199741/)和 [《程序员的职业素养》](http://book.douban.com/subject/11614538/)，英文名是很相近的《Clean Code》和 《Clean Coder》，应该接替[《代码大全2》](http://book.douban.com/subject/1951158/)成为必看的系列，因为后者太厚了，而且也有不少过时的东西，要自己去过滤。

[《重构》](http://book.douban.com/subject/4262627/)很厚，但最有价值就是前面几章的洗脑篇，具体实作不如薄薄的[《重构手册》](http://book.douban.com/subject/1173730/)。

关于敏捷的书，最开始的那本[《解析极限编程–拥抱变化》](http://book.douban.com/subject/1790225/)就很好，再随便找本Scrum的流程看看就够了，[《敏捷开发的艺术》](http://book.douban.com/subject/4037534/)也不错。

[《布道之道 – Driving Technical Change:Why People on Your Team Don’t Act on Good Ideas,and How to Convince Them They Should》](http://book.douban.com/subject/6990284/)，经常在组织里推行新技术的同学可以看下，七种怀疑论者模式，脑海中一幅幅熟悉的面孔。

PS. 温伯格的书网上很推崇，《成为技术领导者》之类的，但我觉得年代太远，读起来其实没多大意思，一两个鸡汤观点还要自己从书里慢慢淘，有那功夫不如看点别的。

### 7. 没有覆盖到的内容

数据库如MySQL，我们DBA太专业，自己没机会搞。



