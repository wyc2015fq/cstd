# RISC-V架构能否让国产IC真正“自主研发”？ - 人工智能学家 - CSDN博客
2018年04月20日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：10161
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkC8mdKwbRXdoSNkkR2Ot9LNwYUj95icf4hibALs3Bohft1zOpnkOnlWmg/640?wx_fmt=png)
来源：EET电子工程专辑 
最近中美贸易战激战正酣，中兴被美国禁运的消息传来，一时间国内舆论大哗，说了多年的芯片“自主研发”，这么多的国产芯片上市公司，似乎到了关键时刻就被人卡脖子了。
已经有很多媒体谈到了国产芯片的自主制造，但似乎没有多少人谈到芯片架构的问题。芯片指令集架构是沟通软硬件运算之间的桥梁，以前曾经有过许多的架构，但随着时间的推移，类似于MIPS等架构已经逐渐消失了。
2016年7月18日，日本软银宣布以243亿英镑（约合320亿美元）收购英国芯片CPU IP设计公司ARM。ARM在中国有100多位合作伙伴,去年基于ARM生产的芯片产品有170亿颗，而全球人口也只有70亿。有业内人士预测在物联网时代，基于ARM架构芯片将达到万亿级别。目前国产芯片中商业发展最好的华为海思、紫光展锐均采用ARM架构来进行IC设计。
目前主流的架构包括Intel的X86、ARM架构，在日常生活中见到最多的是ARM，基本上所有的芯片公司都在采用ARM的架构。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkGgnHNb2G2g7XJbWebNUsQz3TppEGdqDiaefLJ5Vzo714G0svOgIuMaw/640?wx_fmt=jpeg)
可以说，谈到物联网应用已经不能绕开ARM，但是在ARM基本上要一统天下，甚至侵入Intel的桌面PC和服务器业务的时候，免费开源的RISC-V诞生了。
“RISC-V可能真正能成为国产的自主的指令集架构。”RISC-V推广人，武汉聚芯微电子架构师胡振波认为。他表示，RISC-V作为免费的架构，将会和ARM产生竞争。在手机等传统ARM的垄断领域会保持强势存在，在一些新兴的边缘领域，比如IoT、AI、边缘计算领域，RISC-V将具有爆发空间。
**什么是RISC-V？**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkXiapwdENiaXbHzMjgY8O2QibCYYqdayh2xCkM6AXtkFiaj7cronwQXcdEw/640?wx_fmt=jpeg)
2017年，两位现代计算机架构先驱John Hennessy与David Patterson分别获得了2017年ACM图灵奖，他们本身是RISC-V技术的发起人和推动者。这两位泰斗也双双进驻谷歌。
由于不满ARM等处理器架构的复杂性和相关知识产权的限制，在这两位大师的推动下，伯克利大学决定发明一种全新的指令集架构，可以被任何学术机构或商业组织自由使用。这两位大师编排的教材，在全球范围内，以美国为首在大学教材方面都在采用RISC-V为教材。在全球范围内，RISC-V也被很多国家定义成国家标准指令集，比如印度。在业界也引起了很多关注，比如三星明确表示会使用RISC-V用在相关的产品中。2016年，RiSC-V成立了基金会，初创企业包括Google、西部数据、台湾晶心、联发科、杭州中天、华为等。
**RISC-V的意义是什么？**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkjyAG7BGQU625XVdKo6bDBaDVmGKkC0QHjyUG1kibZ0VVd6RngtesSeA/640?wx_fmt=jpeg)
前面提到过，芯片指令集架构是沟通软硬件运算之间的桥梁。RISC-V作为全新的指令集是一个颠覆性的架构，这个架构的目标和以往所有的商业架构都不一样。可以被任何学术机构和商业组织使用。如果我们要用到ARM的架构是要付费的，RISC-V任何人都可以免费使用。
这个颠覆性的理念在整个处理器的历史上没有出现过的。正式因为其颠覆性，RISC-V在2016年被评为最佳技术。
**RISC-V的技术特点是什么？**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkfzQVpKVa90M64ictUrCeCWkVlk0Ty6x7H4ldNfPyoe4eZ1N3EibIkIGQ/640?wx_fmt=jpeg)
RISC-V的技术特点是三点：模块化、极简、可扩展。首先它是一个模块化的指令集，配合一些扩展华的指令集来组成。嵌入式领域的应用和服务器领域的应用不一样，这第一个特点是模块化。
第二个特点是极简，因为一些传统的X86和ARM架构的篇幅，传统商业架构非常繁多，并且彼此之间不兼容。因为RISC-V是后发的架构，拥有技术上的后发优势，技术上非常精简。
另外非常易于实现，相比传统的商用操作系统。最后一点是可扩展，传统的ARM指令集是不允许做扩展，但是RISC-V是方便做可扩展的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6Sk6lZFaghx7eF6sBuqcEWXib1ncL9gvaZicEdk8ibDtbetkaiaicDibPh3eV9g/640?wx_fmt=jpeg)
由于RISC-V的三大技术特点，因此吸引到非常多的大公司都在纷纷的给它站台，RISC-V基金会每年会进行两到三场RISC-V的研讨会，每一次都会有大公司和知名大学来主办主场。
**只有RISC-V才能实现国产自主的指令集架构**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkP30YEJTFqckdEw99jDX1JiahEvS5SVdf9eCTibKOT9uZiaeAwib8tsjA6g/640?wx_fmt=jpeg)
目前国产处理器所采用的指令集架构现状。
国产操作系统可以用开源的Linux，可是自主开发芯片指令集架构这玩意就比较难了。因此要谈“自主研发”似乎绕不开芯片架构。国外的指令集架构本质上是一种授权付费的，很难实现国产自主。如果国家自己定义一套指令集架构没有太大意义，因为处理器架构必须是全球范围的一个通用架构，必须获得生态支持。“RISC-V可能真正能成为国产的自主的指令集架构。”胡振波表示，现在有了RISC-V可以很好的解决这一问题，由于这是由开放的基金会组织，并不会收费。中国公司也可以加入这个基金会，另外目前全世界正在迅速发展，正在产生全新的生态。
胡振波表示，目前半导体和嵌入式领域从业者都很关注RISC-V。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkB89gKIuS7sk3o4Ic9ny51ZuNcickMGq0fLVDlfhvibrExxYicHfDhXoHg/640?wx_fmt=jpeg)
再来看看RISC-V版本的盘点，RISC-V只是一个指令集架构，定义了一个标准，你可以按照这个标准来做处理器。因为这是一个开放的指令集架构，因此你个人也可以做。所以过去几十年冒出了很多不同的版本。也有一些商用的IP公司推出了一些商用版本，基本上以国外为主。中国大陆目前为止，只有蜂鸟E200是由中国大陆开发的，目前这个表里只列出了主流的版本。
**RISC-V的应用场景有哪些？**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6SkZTZBic7pw742iawyhaiamfwlEcItdraEhRK7I2H9AAUMlPrvubgE2wktQ/640?wx_fmt=jpeg)
胡振波表示，在产业界和芯片界，主流的大公司都在用RISC-V架构来做产品。西部数据和AMD都明确的说在用RISC-V做芯片，CEVA在用RISC-V做控制芯片核，谷歌也在用RISC-V做一些新的芯片项目。还有美光。除了这些大公司，还有很多业界大公司已经非常普遍的在用RISC-V来做内核。围绕商业技术涌现了很多的专用公司，美国对于新技术的接受程度是很快的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6Skoh4icITJRqSibV0EATFicXA35XdBoy5DInf6ajEuOsV3dLWAkN7kJs2QQ/640?wx_fmt=jpeg)
中国大陆其实目前还没有明确的商业公司来做这个事情，以上列举了比较知名的公司。非常有代表性的几款芯片，包括美国的Sifive，采用Linux处理器核，另一款是Greenwaves IoT应用处理器，这是欧洲公司推出的非常低功耗的IoT处理器，也是采用RISC-V的处理器。
**RISC-V技术对于IoT的意义**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6Skxy7kGsqYXia0E9jWHjaiaGu3TY47x7GCuZOiaX1XmnOyKkOETetP3Sc7w/640?wx_fmt=jpeg)
那么RISC-V技术对于IoT将带来什么样的意义呢？
首先是持续的碎片化，Iota是一个碎片化的生态，这个生态里如果能快速定义出差异化的产品。面对这个技术同样的要求，低功耗、低成本、安全性以及边缘计算、增强计算也是明确的新的要求。
由于越来越以应用为中心的趋势，也就是说不是在向以前以芯片公司为中心。以前是MTK、高通主导应用，而IoT是碎片化的以应用为中心，替代了传统的芯片公司为中心的模式。另外，传统的ARM的IP售价还是很贵的。并且每个版本的发布时间也是很长，同时也让创新门槛变高，只有大公司才能买到IP，很难快速响应边缘化的计算需求。
RISC-V的优点很好的解决这些问题，一个是RISC-V是全世界通用的标准指令集，生态长期向好。因为全球的大学都在以RISC-V授课，随着5年、10年的维度来看，对RISC-V的掌握情况会成为基本技能。尤其它的架构是免费开放，降低了创新门槛，有技术能力的个人和公司能够使用这个架构来开发一些有意思的应用。由于具备低成本的优势，也降低了创新的代价。
基于以上原因，所以能够很快速的响应碎片化，越来越多的新创公司越来越多使用RISC-V内核，希望进入技术创新领域。越来越多的国内公司也在使用RISC-V内核。胡振波认为，RISC-V能够降低边缘计算的成本和门槛，带来快速的创新产品。
**基于RISC-V自主研发的蜂鸟E200处理器**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6Sk4XnXC39KDxvh9LOsY0iaEichMLXSBnjFuK01rWZFKeiaUf3bCVTz3OrXA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQgOHhjYgBlq7gTNpMu6Sk9ibATWHgSwoeXxSLnAwD0KCicEpavQoxdruia1Vm0SbdHgRHDneyTKvww/640?wx_fmt=jpeg)
最后，胡振波介绍了自己研发的首颗采用RISC-V架构的MCU蜂鸟E200。据介绍，这款全球最小的32位RISC-V核心可以实现超低功耗，对标的是ARM的M0+的MCU内核，安全可控，无需向国外公司支付版税。胡振波表示蜂鸟E200还有205、205FD等一系列产品线。
“这就是开放的指令集的魅力，接下来会冒出更多无法想象的指令集版本。”胡振波最后表示。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
