# Google Brain 推荐算法，给了 Youtube 新生 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [精算狗](http://www.jobbole.com/members/chenyahui) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Casey Newton](https://www.theverge.com/2017/8/30/16222850/youtube-google-brain-algorithm-video-recommendation-personalized-feed)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
*YouTube 整合 Google Brain 推荐算法，视频播放量提升 20 倍。*
去年晚些时候，我在玩《羞辱2》（Dishonored 2）游戏时，照常在 YouTube 上搜索了如何通过游戏中棘手的部分。和往常一样，我找到了一个能回答我问题的视频。
![](http://wx3.sinaimg.cn/mw690/7cc829d3gy1fm098xpivkj21hc0u0dmd.jpg)
但是在我下一次访问 YouTube 时，网站给我推送了更多引人入胜的《羞辱》游戏视频，比如：
- 玩家在玩《羞辱》时能够[不被敌人察觉](https://www.youtube.com/watch?v=wKF5Rs1A8w8)的剪辑；
- 玩家用[极具创造性的方法](https://www.youtube.com/watch?v=tWSIHC2lvn4&t=597s)杀死每一个敌人的剪辑；
- 对游戏开发人员的采访；
- 聪明的[讽刺评价](https://www.youtube.com/watch?v=AivuXyh9PV4)。
我到 YouTube 去寻找我问题的答案，它则向我展示了一个宇宙。
随后，我发现自己一天就要访问 YouTube 好几次。大多数时候我都漫无目的，我已经习惯了网站主动提供我会喜欢的视频。一月份的时候，我迷上了一个叫 [Pinegrove](https://www.youtube.com/watch?v=mgSnptOsCuw&t=996s) 的民谣摇滚乐队，几周内 YouTube 就向我提供了几乎所有上传到服务器上的该乐队现场演出。当我今年春天有了一个新公寓时，我开始更频繁地下厨了。在我搜索了如何做意大利面包沙拉之后，YouTube 很快就向我推送了大量专业厨师：[Byron Talbott](https://www.youtube.com/user/ByronTalbott)、Serious Eat 的 [J. Kenji López-Alt](https://www.youtube.com/watch?v=66btvAWmp7g)、以及 [Tasty](https://www.youtube.com/channel/UCJFp8uSYCjXOMnkUyb3CQ3Q) 团队等。
YouTube一直都是非常实用。自从它于 2005 年成立，就一直是网络的中流砥柱。但是过去一年左右，反正对我来说，YouTube 似乎开始变得出奇的好。这个网站开始以怪异的准确性来预测我可能会感兴趣的视频——比以前精准太多了。那么什么改变了呢？
![](http://wx3.sinaimg.cn/mw690/7cc829d3gy1fm098yai29j20g40apq4j.jpg)
在 12 年的历程中，YouTube 已经从一个搜索驱动的网站转变成了一个本身就是目的地的网站。实现这一转变需要数百次的实验、少量的重新设计，以及在人工智能领域的一些重大飞跃。但是真正提升了 YouTube 的，是其在 feed 上的演化发展。
虽然现在很难回忆起来，但是 YouTube 最初不过是基础设施：它提供了一个把视频嵌入其他网页上的简便方法，这也是你最可能用到它的地方。随着 YouTube 的发展，它成了一个寻找档案电视剪辑、补深夜喜剧，以及观看最新热点视频的网站。除了维基百科以外，YouTube 很可能是网上最引人入胜的地方。你的同事在饮水机旁边提到了 [Harlem Shake](https://www.youtube.com/watch?v=8f7wj_RcqYk)，于是你花了余下的整个晚上，去 YouTube 上看了 Harlem Shake 的视频。
与此同时，Facebook 已经发明了我们这个时代的定义形式：News Feed。它是根据你的兴趣，个性化定制的无限更新流。Feed 占领了消费者网络，从 Tumblr 到 Twitter，到 Instagram，再到 LinkedIn。YouTube 早期实现个性化的方法要更有限，包括要求用户订阅频道。这个比喻来自于电视，并且产生了混杂的结果。虽然 2011 年的大量推送订阅取得了一些成功，但是 ComScore 上的数据显示，一个人观看 YouTube 的平均时间却没有变化。
频道不再像曾经那样支配着 YouTube。现在，在手机上打开 YouTube， 会发现它们隐藏在一个分开的标签页里。而应用打开的则是一个 feed 流，由根据用户兴趣定制的视频集组成。当然有来自你订阅频道的视频，但是还有来自你可能没有看过的频道的视频，这些视频跟你之前观看过的视频相关。
这就是为什么直接搜索了《羞辱》（Dishonored）视频之后，我开始看到游戏中的秘密行动和讽刺评价的推荐。YouTube 开发的工具使得它的推荐不仅是个性化的，而且极其准确，从而增加了整个网站的视频观看时间。
YouTube 推荐的技术领导 Jim McFadden 在 2011 年加入了公司，他说：“当人们知道他们是来找什么的时候，我们知道他们会上 YouTube。我们也希望在他们不一定知道想找什么的时候，为他们提供服务。”
在 2011 年 McFadden 加入 YouTube 几个月后，我第一次访问了这个公司。YouTube 当时和现在一样，[首要目标](http://www.sfgate.com/business/article/YouTube-s-effort-to-get-people-to-watch-longer-2352967.php)都是让用户花更多时间来观看视频。那时事情进行得不是特别顺利。McFadden 说：“YouTube.com 作为一个主页，并没有大量参与度。我们想，既然这样怎么才能把它转变成目标呢？”
Youtube 尝试了很多方法。给顶部的视频主（top creator）购买了专业摄影仪器。引进了 [leanback](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwjTzeaMnf3VAhUQzGMKHfNaA_AQFggoMAA&url=http%3A%2F%2Fwww.youtube.com%2Fplaylist%3Flist%3DPLLTK93ZhdOWb3X4M3xojmqTZKTGeRSXaQ&usg=AFQjCNHrRwK8QlbAD9YRMTacRrTKtN6Ipg)，这个特性允许用户在当前视频播放时排列一会要看的新视频。重新设计了 Youtube 主页，突出订阅频道而不是单独的视频。
每个用户观看的视频数量没有变化，但是一个改变让接下来的那个春天终于有了起色：YouTube 的算法推荐不再基于视频点击量，[而是基于](http://www.businessinsider.com/youtube-watch-time-vs-views-2015-7)视频观看时长。
几乎一夜之间，那些从误导人的标题和缩略图中获益的视频主就发现他们的浏览量暴跌。更高质量的视频不断涌现，这些视频与更长的观看时间紧密相连。接下来的三年，YouTube 的用户观看时长每年增长 50%。
我订阅了几个频道，并认为自己称得上是一个 YouTube 的常客。但是要想让它变成一个每天能被访问很多次的网站，YouTube 需要一些新的工具——这些工具在过去 18 个月内才成为现实。
这个月当我拜访 YouTube 办公室时，McFadden 揭示了 YouTube 突然的准确推荐的原因：Google Brain，它是母公司的人工智能部门，YouTube 从 2005 年开始使用它。Google Brain 不是 YouTube 使用人工智能（AI）的第一次尝试。YouTube 之前利用过 Google 建立的系统 Sibyl，将机器学习技术应用于推荐。但是 Google Brain 应用了一种叫非监督学习的技术：它的算法能够找到不同输入之间的关系，而软件工程师永远想不到这些关系。
“它的关键之一是，它能够概化（generalize）。然而之前，如果我看了一个喜剧演员的视频，我们的推荐很擅长说，这还有一个和它一样的视频。但是 Google Brain 模型能选出其他相似但并不完全一样的喜剧演员——甚至有更紧密的关系。它能够看到不那么明显的模式。”McFadden 说。
举个例子，Brain 算法开始向 YouTube 手机应用用户推送短视频， 向电视应用用户推送长视频。它推测，根据平台来推送不同时长的视频，能够增加视频观看时长；这个想法是正确的。YouTube 在 2016 年发布了 190 个类似的改动，并且会在今年逐步发布另外 300 个。Todd Beaupre 是 YouTube 挖掘团队的产品组经理，他说：“事实是，大量小的改进随着时间逐步积累。对每一个改进，你尝试了 10 件事，并发布一个。”
Brain 算法比 YouTube 之前运算得更快。过去几年，它需要花几天时间来将用户行为与未来推荐整合起来。该公司说，这使得公司很难确定热门主题。“如果我们想要让用户回来，弄清现在正在发生什么，那么我们已经从某种程度上解决了这个问题。现在的延迟是用分钟或者小时计算的，而不再是几天了。”Beaupre 说。
整合 Brain 产生了巨大的影响：超过 70% 的 YouTube 视频观看时长都是由算法推荐产生的。每天，YouTube 向用户推荐 200 万个不同的、76 种语言的视频。人们在 YouTube 主页上观看视频的总时长比三年前增长了 20 倍。
这大概和我自己的行为对上了。几年前，我开始经常在午餐时间访问 YouTube 主页，这样吃饭的时候可以看看视频。但是它的推荐足够好，让我开始更经常地给自己 YouTube 休息时间。本周我失控了，在 PlayStation 4 上登陆了 YouTube，这样我就能在我所拥有的最大屏幕设备上看推荐视频了。
这就是真正个性化 feed 的强大之处。但是我对 YouTube 的感觉，与其他组成电子生活的事物很不同；这让我感到吃惊。Facebook 的 feed 是基于好友的动态，以及你点过赞的主页的动态。它能告诉你谁订婚了或者生孩子了。但是我觉得，这些关于好友里程碑式事件的动态内容没那么有趣。Twitter 中则有关注人的推文，以及他们转推的内容。作为一个记者，我几乎被要求住在 Twitter 里，尽管最近主页的时间线里只有无穷无尽的、慌张的尖叫。
每种 feed 都有它的长处，尽管 2017 年让它们消减了。在 Twitter 上，无论你关注了谁，政治都主导了所有讨论。Facebook 对事件和组这类特征的一时热情导致它的 feed 周周都以不和谐的方法转变，让我觉得我和好友们不再那么相联。（都是图片的 Instagram 仍然像个绿洲，这个应用仍然在快速增长也不是什么奇迹。）
Facebook、Twitter 和 Instagram，他们的 feed 都经常要求你在他们的平台上表演；这似乎很显著。YouTube 明显是由表演驱动的，即使只有小部分的用户上传过视频，它也不曾对用户施加压力，要求他们上传视频。人们可以被动地享受 YouTube，就像它想方设法地想要取代的电视频道。在这个疯狂的时代，在没有被问到我对当天新闻的反应时，有些东西也能保持镇静。
YouTube 强调与你可能喜欢的视频相关联的视频，这意味着它的 feed 似乎总要比它同类型的网站在视野范围上更广、更好奇。YouTube 越是在远的地方寻找内容，它越像是想要逃离其他 feed。在黑暗的一年，我会接受 YouTube 提供的所有东西来逃避现实。
2013 年，Alexis Madrigal 在 Atlantic 上写道，假定我们知道 feed 已经达到了顶峰。他认为，未来是属于有限的经历的：时事通讯邮件、Medium 专栏、10 集的 Netflix 剧集。毕竟，无穷无尽的内容流让人感到疲惫。“当媒体主义的秩序废止时，自由不会冲进真空吸尘器里，但是会产生一个有着它自己逻辑的新兴秩序。”Madrigal 写道。“我们发现，流引进了它自己种类的强烈欲望和约束。更快！更多！更快！更多！更快！更多！”
4 年来，YouTube 的方法表明 feed 只会变得更加重要。抵制一个与不断提高的个性化方法相对应的、不断增长的视频库是困难的。YouTube 现在对用户进行了调查，调查他们有多喜欢推荐给他们的视频；随着时间流逝，结果会使 YouTube 更聪明——会导致更多的视频被「消费」。
Beaupre 向我把这个过程描述成穿过峡谷。“这里有与你喜欢过的视频紧密联系的视频，也有热点和流行的视频。但是在这之间，是魔法区。”如果 YouTube 的对手无法找到穿过峡谷的路，他们可能会发现很难与 YouTube 抗衡。
