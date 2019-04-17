# 首个会翻跟斗的MIT四足机器人，将造10台借予其他院校 - 知乎
# 



> MIT（麻省理工学院）今年 3 月初推出的全球首个「会翻跟斗」的四足机器人 Mini Cheetah 只有 9 公斤重量，四肢可自由弯曲并以每秒 2.45 米的速度前进。它可以横向移动、跳跃、自动从摔倒中爬起……目前，MIT 的研究人员正准备让机器人像猫一样在被抛出后也能够安全落地。

他们还有个更慷慨的计划：造 10 台这样的机器人帮助其他研究机构在其之上验证自己的 AI 算法。
![](https://pic3.zhimg.com/v2-f9614cd5fe80cdb6f47ab9a44b73fe0e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='819'></svg>)Mini Cheetah 位于关节上的马达看起来非常科幻，这让它的灵活性可以与体操运动员相媲美
MIT 展示了 Mini Cheetah（迷你猎豹）的强大机动能力：机器人可以在不平坦的地面上小跑，速度可以达到人类正常行走速度的两倍。四足结构使其不会被轻易推倒，即使摔倒了，它也可以像会武功一样迅速起身恢复原状。据 MIT 研究人员介绍，它的设计「坚不可摧」，在后空翻漏油之后也可以恢复。
[MIT机器人Mini Cheetah_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/s0848mpby02.html)
为了预防四肢或马达损坏，Mini Cheetah 的设计考虑到了模块化：每个机器人的腿都由三个相同的低成本电动马达驱动，这些马达（非定制，很便宜）有一个单级 6:1 变速箱，它们的最大关节速度是每秒 40 弧度，速度非常快。研究人员使用现成的部件进行设计，每个点击都可以轻松进行替换。把这些部件整合起来就像在堆乐高积木一样。

MIT 研究人员将在今年五月的国际机器人与自动化大会上展示这一最新研究成果，他们目前正在制造更多的四足机器，目标是 10 台——他们希望每台机器人都可以借给其他实验室。

MIT 希望能够将 Mini Cheetah 租借给其他研究组织，从而帮助他们在高机动性机器人上验证全新算法。

上周，IEEE 采访了 MIT 机械工程学教授，仿生机器人实验室主任 Sangbae Kim，请教了他一些关于 Cheetah 设计和未来规划的问题。

**IEEE：为什么要做一个迷你版的 Cheetah？**

**Sangbae Kim：**这是为了加快研究进程。和搞机器人研究的人聊聊天你就会知道做这个工作有多么痛苦：机器人非常脆弱、危险，它们没有足够的扭矩，也很难建模。总之问题一大堆。如果你有一个 Cheetah 3，有多少时间你是可以运行它的呢？可能是百分之一甚至更少，因为运行机器人非常困难。这个工作有危险性，或者如果哪个地方出了问题，一修就是一个月。有了 Mini Cheetah，你就可以每天用它工作 5 个小时。我们做了一个三个小时的 demo，没出现什么问题。基本上我们已经达到了一个可靠的水平，可以将它交给其他团队，那些饱受硬件摧残的团队。

Mini Cheetah 的大小刚刚好。这个大小不至于太危险，也不至于太脆弱。这个机器人我们已经用了 12 个月，没有更换任何一个机械部件。这主要归功于我们的驱动器技术。我们要设计一个能抵抗撞击，能跳能着地的机器人。它的速度已经达到了每秒 2.5 米，对于它的体型来说，这个速度已经很快了。但理论上来说，它的速度可以达到每秒 4 米——接近 Cheetah 3 的最高速度。

Sangbae Kim 表示，他们希望举办「一场机器狗穿越障碍的比赛，每个团队用不同的算法控制一只 Mini Cheetah，看一看哪种策略更有效。」

**IEEE：液压马达也被用于大部分高性能动力机器人中，为什么电力驱动器是 Cheetah 和 Mini Cheetah 这种机器人的最佳选择？**

**Sangbae Kim：**关于这一点，我之前和 March Raibert（波士顿动力创始人）讨论过，我们两人的看法有点分歧。他认为这里面有规模问题，但我不这么认为。电力驱动器的伸缩性很好，说实话我对 Mini Cheetah 的表现感到非常惊讶，因为它里面的电力驱动器大大缩小。秘密在于设计的细节：如果你观察 Mini Cheetah 的肩部（驱动器），会发现它比 Cheetah 3 的肩部大。虽然不是特别明显，但 Mini Cheetah 的肩部占体长的比例更大一些，因此性能也更强。

电力马达大型化时很容易，可用于一匹马大小的机器人，从而让它跑的像马一样快。功率密度方面也没问题。但对人形机器人而言，问题就变得有趣了，因为人形机器人需要许多自由度。在 12 级自由度时，没有问题，我认为电力马达能够达到。但如果是 24 级的自由度，你需要在腕关节、踝关节脖子等都有多级别自由度……这么多的自由度混合在一个关节连接处，电力马达就不行了。这也是为什么液压开始引人关注，因为它是一个强大的能源，而驱动器又相对太小，要比同样扭矩的电力马达小很多。当然，液压还有其他成本要考虑。所以，在一定自由度内，我认为电力驱动器在各个方面会较好。但一旦自由度更大，液压会更有优势。

**IEEE：目前已经有 Mini Cheetah 了，未来会继续研究 Cheetah 3 吗？**

**Sangbae Kim：**我们的模拟工作已经进行了 95%，对控制器足够自信时就会在机器人上进行测试。目前我们的模拟测试进行了大约 50%，其中的 40% 都是在 Mini Cheetah 上进行的，在 Cheetah 3 上只进行了 10%。即使我们的算法在模拟中并不是超级鲁棒的，Mini Cheetah 也可以通过测试。它需要大约 2 分钟启动配置，如果出了问题也不会坏掉。

对于机器人研究来说，不易受损是一件很重要的事，因为这意味着你可以在它身上尝试更多技术，或放手让你的学生们去研究它，而不是事先列出一式四份的禁令，给机器人穿上防护盔甲，随时准备拿出支票薄，或者站在灭火器旁边。坚固耐用的机器人意味着你可以减少模拟中的工作量，这会更有趣（并且可能更有效率）。

目前的计划是做出 10 台 Mini Cheetah，五到六台供 MIT 的其他实验室使用，另有四到五台会借给其他研究有足机器人的团队。最后，根据 Sangbae 的说法，他们希望举办「一场机器狗穿越障碍的比赛，每个团队用不同的算法控制一只 Mini Cheetah，看一看哪种策略更有效。」这个比赛最初可能会是人类控制机器人，但最终 Mini Cheetah 将会完全自动。一旦目标实现，我们就可以在不久的将来看到消费级版本了。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
参考链接：
- [http://news.mit.edu/2019/mit-mini-cheetah-first-four-legged-robot-to-backflip-0304](https://link.zhihu.com/?target=http%3A//news.mit.edu/2019/mit-mini-cheetah-first-four-legged-robot-to-backflip-0304)
- [https://spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research](https://link.zhihu.com/?target=https%3A//spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research)[Mini cheetah is the first four-legged robot to do a backflip ​news.mit.edu![图标](https://pic3.zhimg.com/v2-769c653fba786e2f842ee12713690852_180x120.jpg)](https://link.zhihu.com/?target=http%3A//news.mit.edu/2019/mit-mini-cheetah-first-four-legged-robot-to-backflip-0304)[https://spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research](https://link.zhihu.com/?target=https%3A//spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research)[How MIT's Mini Cheetah Can Help Accelerate Robotics Research​spectrum.ieee.org![图标](https://pic4.zhimg.com/v2-723bab02c440e28d8fe8a2b421829bc7_180x120.jpg)](https://link.zhihu.com/?target=https%3A//spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research)[https://spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research](https://link.zhihu.com/?target=https%3A//spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research)[Mini cheetah is the first four-legged robot to do a backflip ​news.mit.edu![图标](https://pic3.zhimg.com/v2-769c653fba786e2f842ee12713690852_180x120.jpg)](https://link.zhihu.com/?target=http%3A//news.mit.edu/2019/mit-mini-cheetah-first-four-legged-robot-to-backflip-0304)[https://spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research](https://link.zhihu.com/?target=https%3A//spectrum.ieee.org/automaton/robotics/robotics-hardware/mit-mini-cheetah-accelerate-research)


