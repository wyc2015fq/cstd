# 《OpenGL 3 & DirectX 11: The War Is Over》：绘图 API 终战之日？ - 逍遥剑客 - CSDN博客
2008年11月23日 14:21:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：8311
![OpenGL Logo](http://blog.monkeypotion.net/wp-content/uploads/2008/10/opengl-logo.jpg)
（图片来源：www.cnblogs.com）
原文出处：[**OpenGL 3 & DirectX 11: The War Is Over**](http://www.tomshardware.com/reviews/opengl-directx,2019.html)
现今在 [DirectX](http://zh.wikipedia.org/w/index.php?title=DirectX&variant=zh-hant) 与 [OpenGL](http://zh.wikipedia.org/w/index.php?title=OpenGL&variant=zh-hant) 皆已相当普及的年代里，我们似乎也逐渐淡忘了两者从前的那段江湖恩怨。在着名硬体网站 [**Tom’s Hardware**](http://www.tomshardware.com/) 的这篇专栏文章里，作者带领我们重新回顾这场「绘图 API 战争」中的起伏转折，以及 OpenGL 3 与 DirectX 11 的未来展望，是篇值得仔细阅读的好文章。
「DirectX 最杀！很强很邪恶的微软帝国万岁！」
「想跨平台吗？OpenGL 才是唯一的光明道路！」
「我是初学者，应该要选择 DirectX 或 OpenGL？」
「我想做出一款超棒超好玩的游戏，要用 DirectX 还是 OpenGL 比较好？」
身为游戏程式设计者，如果你有定期浏览国外各大程式论坛的习惯，应该会对以上这些问题感到非常亲切、熟悉，而且可能还会有些反感吧？在游戏程式设计相关领域的讨论区里，最常见的万年讨论串非**「DirectX 与 OpenGL 的比较」**莫属。DirectX 与 OpenGL 双方各有死忠的支持者阵营，只要在网路上里提出类似以上叙述的问题，往往会引来双方拥护者激烈的对抗与辩论；如果又不小心触动了某些资深程式设计者的「逆鳞」，就更加会战到天荒地老无以復加。
在过去的短短几年之内，我们亲身经歷了消费性 3D 显示卡市场的爆炸性成长。从以前只有硬派玩家会花大钱购买的「3D 加速卡」，到现在几乎已成为标准配备的「3D 显示卡」，电脑绘图硬体不仅成功地打入了一般消费者的市场中，其性能也获得革命性的突破，目前 GPU 的电晶体数量甚至已经超越了 CPU。时至今日，**DirectX 与 OpenGL 在电脑绘图与游戏业界中的地位，究竟是处于分庭抗礼的情势，或者已经分出了胜负？**且让我细说从头。
这场由 Microsoft（简称为 MS）与 [Silicon Graphics](http://en.wikipedia.org/wiki/Silicon_Graphics)（简称为 SGI）公司相互交锋的绘图 API 战争，已经持续了十年之久。当年 DirectX 初试啼声之时，MS 挟其庞大的企业资源以及 Windows 作业系统的市佔率，正摩拳擦掌准备倾全力推行自家设计的绘图 API 标准。而相较之下，虽然 SGI 的资源比较弱势，但是行之有年的 OpenGL，早已佔据了电脑绘图界的王者地位而无可动摇；同时，**OpenGL 也得到最强而有力的同盟军 John Carmack 的支持背书**。由于当时使用 OpenGL 开发的 [Quake Engine](http://en.wikipedia.org/wiki/Quake_engine) 所达到的惊人绘图效果，因此使得所有绘图显示卡的制造商，都必须提供完整的驱动程式支援 OpenGL 标准，才能够符合游戏开发者与游戏玩家对于显示卡硬体的期望。
SGI 的 OpenGL 是当时专业绘图市场中的龙头老大，佔有极大的优势，而与此同时 MS 只能够一切从零出发。要开发出既好用且功能强大的绘图 API，学习曲线可说是相当的严峻；在早期的 DirectX 版本中，很多程式设计者都无法适应那种与 OpenGL 全然不同的复杂程式概念，因此许多人都对于 DirectX 不屑一顾，并不看好它的未来发展。但是 MS 并没有轻言放弃，随着每次释出的全新版本，DirectX 正一步步逐渐跟上 OpenGL 的脚步。
在双方开战初期，OpenGL 拥有非常显着的有利优势，然而**整场战役的转捩点，发生于 2001 年 MS 释出的 DirectX 8 之中**。这是首次 DirectX 的 API 不再仅止于拷贝 SGI 的规格标准；MS 在这次 DirectX 的全新版本中，引入了对于整个电脑绘图界来说极为重要的创新与变革：Vertex Shader 以及 Pixel Shader。顶点着色器与像素着色器的诞生，为绘图程式开发者们开拓了一条前所未见而且闪闪动人的星光大道。
相较之下，当时 SGI 的主要收益来源是非常昂贵的 3D 绘图工作站，他们没能预期到 3D 绘图显示卡市场的惊人需求爆发，而 ATI 与 Nvidia 这两间新兴的显示卡厂商，又竟然能以相当低廉的价格，将绘图显示卡打入游戏玩家的市场。另一方面，OpenGL 规格标准的发展，也受到各软硬体厂商之间的利益冲突因素牵连而迟迟无法达成共识。而在 MS 这方，却仅仅与 ATI 以及 Nvidia 两间公司合作制订 DirectX 的 API 规格，并且拥有最终的关键裁量权，因此能够相当顺利且迅速地持续发展。
在彼消此长的情况下，**当 DirectX 9 推出后，更是成功地获得了一场决定性的胜利**。于是，有许多软体与游戏的开发者，决定开始使用 DirectX 或者同时提供两者的支援；只有 John Carmack 以及有跨平台需求的开发者，仍然对于 OpenGL 忠心耿耿，但是他们的阵营已经比从前衰弱许多。当然，OpenGL 阵营手上仍然握有逆转命运的契机。于是在二年前，OpenGL ARB 组织终于将 OpenGL 的开发权交付给了 [Khronos](http://www.khronos.org/) 集团，将一切的希望都寄託在他们的身上。经过了二年的漫长等待后，在今年八月的 SIGGRAPH 研讨会中，Khronos 终于发表了万众瞩目的 OpenGL 3，所有 OpenGL 阵营的支持者莫不期待能够藉此扳回一成。然而，事情并没有如原先计画般的顺利。
将时序拉回 2002 年，此时 OpenGL 正逐渐失去电脑绘图界的领先地位。**MS 的 DirectX 9 提出了全新的着色器 (Shader) 绘图功能以及高阶着色语言 (HLSL)，而 OpenGL 阵营却拿不出可以相比拟的功能。**在 Shader 绘图架构问世之后，显示卡硬体很难再依循着传统的绘图管线架构生产制造，于是为了弥补现有 OpenGL 不足之处，各家显示卡厂商开始各自扩充原有的 OpenGL 规格，自订出一套自己专属的延伸绘图 API。
正当 OpenGL 阵营陷入一片混乱之时，[3DLabs](http://www.3dlabs.com/) 这间公司瞭解到 OpenGL 亟需迅速而彻底的变革，才能够跟得上显示卡硬体一日千里的发展脚步，于是当仁不让地提出一项拥有许多重大改革项目的 OpenGL 重整计画。首先，他们为 OpenGL 加入了高阶着色语言 GLSL，接着为了使 OpenGL 得到良好的效能，必须将 API 进行全面性的整理修改；在 OpenGL 2.0 Pure 的核心规格里，他们计画删除那些过时以及多余的功能特徵，只留下最符合现今硬体主流架构的功能，使开发者能够慢慢地由老旧的 OpenGL 1.x 版本，转移到全新的 OpenGL 2.0 版本。
然而很遗憾地，经过 OpenGL ARB 组织的冗长讨论后，这项周全的改善计画被回绝了。**在最终释出的 OpenGL 2.0 里，仅仅加入了对于 GLSL 的支援**，而 3DLabs 所提案的其他功能全都随风而逝，导致 OpenGL 2.0 的版本仍然远落后于 DirectX 所提供的功能。好不容易到了 2005 年时，OpenGL 终于赶上了 DirectX 在三年前所释出的 API 功能。此时各家显示卡厂商与软体开发者，都同意事态不能够再继续这样发展下去，否则 OpenGL 将会逐渐失去地位而被人遗忘。最后，OpenGL ARB 终于在 2006 年时，将接力棒交到了 Khronos 集团手上。
Khronos 以过去管理 OpenGL ES 的高效率与负责态度着称，在接手 OpenGL 的开发后，他们很快地建立起对外的沟通管道，并且对于 OpenGL 的未来发展与多方厂商进行周详的讨论，最后提出了两项 **OpenGL 发展计画的里程碑：Longs Peak 与 Mount Evans**。首先，在第一个开发里程碑 Longs Peak 中，他们打算删除那些已经过时的 API，使 OpenGL 能够集中在一组比较先进的功能组之中，并且提供与 Shader Model 2 相同等级的功能；而第二个里程碑 Mount Evans，则期望能够加入全新的 API，并且提供与 Shader Model 4 相同等级的功能。
时程很紧迫，需要完成的项目非常多，刚开始的开发状况仍称得上是一切顺利，没想到自 2007 年年末开始，Khronos 就不再公布任何关于 OpenGL 新版的开发进度消息，突然间由开诚布公的沟通，一瞬间转变为完全封闭的态度。直到今年八月 SIGGRAPH 的研讨会里，OpenGL 3 总算是千唿万唤始出来。但是在惊喜之后，则是失望、不满以及愤怒的情绪，如同洩洪般在网路讨论区里漫天盖地而来。
> 
But while some people were expecting a pleasant surprise, Khronos had a serious disillusionment in store for fans of OpenGL. 
这些强烈的负面回应，不只是因为 OpenGL 3 比原先预定的时程延后了将近一年的时间才发佈，同时也因为大多数在 Longs Peak 中所承诺的新功能也完全地被捨弃了。检视最终公布的成果，**OpenGL 3 看起来简直就像是 OpenGL 2.2 版本一样**，只不过是个「增进性的更新」(incremental update) 而已，API 并没有真正地产生改变。OpenGL 3 所提供的新功能，也与 DirectX 10 非常相似。
根据 Carmack 的说法，**OpenGL 3 标准未能达到预期成果的主要原因，在于某些 CAD 软体的开发者并不满意 Longs Peak 中制订的规格**。这些软体厂商，害怕相容性的问题会使得他们的应用程式某些比较老旧的功能失效。深入参与研发程序的 Nvidia 公司 Lichtenbelt 也说：「我们在应该移除哪些功能的议题上，遭遇到意见不一致的情势，主要是因为不同的市场需求所致。**我们发现我们无法做出一个满足所有人需求的 API。**」
回过头来看看另一方的 DirectX 阵营。在 2006 年发佈的 DirectX 10 里，MS 对 DirectX 整体做出了史上最彻头彻尾的修改。近年来，传统的 API 绘图架构已经快要跟不上显示卡硬体的发展脚步，因此 DirectX 10 的远大目标，就是要为未来的硬体架构提供一项稳固的根基建设。然而，DirectX 10 并没有如预期般获得游戏玩家与开发者的喜爱及关注。
对于游戏玩家来说，即使使用了 DirectX 10 可能也感受不到明显的改变；更糟的是，新的 API 只为 Vista 以上的系统撰写，正好为敌视 MS 的使用者找了一项非常充足的开战理由。而对于游戏开发者来说，在 Windows XP 仍然佔据绝大多数消费者市场的情况下，DirectX 10 与 Vista 作业系统绑在一起，不仅转换成本极高而且也发挥不了显着的市场作用，因此多数的游戏专案，仍然选择坚守着传统的 DirectX 9 标准。
![DirectX Logo](http://blog.monkeypotion.net/wp-content/uploads/2008/10/directx-logo.jpg)
（图片来源：www.socgame.com.tw）
在最近刚揭露初步讯息的 DirectX 11 中，公布了不少值得游戏开发者引颈期盼的全新功能。DirectX 11 奠基于 10 之上，也可以称为是一次增进性的版本更新；在 DirectX 11 正式问市以后，许多开发者应该会选择略过 DirectX 10，直接使用 DirectX 11 开发最新的游戏。而最棒的是，**DirectX 11 不仅能够向下相容于 DirectX 10 的显示卡，同时也能够在 Windows 7 与 Vista 上执行！**
在 DirectX 11 的许多新功能中，我个人认为最重要的莫过于**多执行绪绘图功能的支援**。再者，在 GPU 的专用开发语言上，相较于 Nvidia 大力推行的 CUDA，DirectX 11 的 Compute Shader 优势在于能够同时支援 ATI 以及 Nvidia 的显示卡，甚至是未来的 Intel Larrabee 显示硬体，而且也与 DirectX 的功能具备有更佳的整合能力。另外像是 Tessellation 阶段的导入、Texture Compression 效果的改善，以及物件导向化的 Shader Model 5 等等，也都是对于电脑绘图与游戏开发领域非常具有吸引力的功能特点。
许多人对于 OpenGL 失望之处，不仅是 API 本身的能力，同时也包含被处理的过程。在 OpenGL 3 里，仅仅勉强跟上了 DirectX 10 的脚步，而在几乎同一时间里，MS 已经公布了次世代 DirectX 11 版本的细节。虽然相较于 DirectX 10，最新的 DirectX 11 并没有什么革命性的创新之处，但是自 DirectX 10 推出以来，MS 已歷经了二年的困难处境，所以现在 MS 得以在稳健的基础上，回收当时花费庞大心力重建 API 的功夫。
原文标题所下的**「The War Is Over」**，已经很明确地表达了作者对于 DirectX 与 OpenGL 之战的心得感想。但是在文章最后，即便未来的可能性相当难以预期，作者仍希望在后续 OpenGL 3 的更新内容里，能够证明他所做出的结论是错误的。就我自己的感想来说，OpenGL 仍然是跨平台绘图应用程式的唯一选择，我也不希望 OpenGL 从此衰落而一蹶不振，演变成 DirectX 独占鳌头的局面。唯有正向的竞争压力，才能够加速促进绘图 API 以及绘图显示硬体的发展。
在瞭解了 DirectX 与 OpenGL 两强相争的歷史渊源以及来龙去脉之后，或许有些读者还是会想问：「到底应该要选择学习 DirectX 或者 OpenGL？」对于想进入游戏程式设计领域的初学者来说，**我的建议是：两者都需要学习。**DirectX 与 OpenGL 具有不同的设计理念与实作技巧，也拥有各自独具的优势与弱点；在游戏业界中，两方都有广泛的使用族群，并没有特别偏向某一方，所以两者皆不可偏废。
如果非要从两者中挑一个入门，那么我会建议**由 OpenGL 开始学习电脑绘图理论与实作技术**。因为学习 DirectX 的初学者，往往很容易被视窗程式的建立以及 COM 元件的架构所迷惑，反而会模煳了学习电脑绘图程式设计的焦点。若是使用 OpenGL，便可以利用 GLUT 或其他的辅助函式库，大幅简化与平台相关的视窗程式建立细节，使学习者能够专注在电脑绘图理论与程式实作技术的领域中。当然如果你在进入电脑绘图领域之前，已经相当熟悉 Windows 视窗程式设计，那么选择 DirectX 做为出发点也同样是没问题的。
在学会了 DirectX 与 OpenGL 之后，就可以把它们当成「个人工具箱」里的两把利器：当我想要学习最新的 Shader 程式设计，将显示卡性能发挥到极限时，我会拿出 DirectX 这把屠龙刀；而当我有个程式概念想要快速完成雏形试做时，我就会挥舞 OpenGL 倚天剑。不管是 DirectX 还是 OpenGL，随你自由来去，岂不乐乎？
