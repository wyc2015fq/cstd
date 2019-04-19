# 程序员神器 StackOverflow 10 岁了，它长大后想成为什么？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [一汀](http://www.jobbole.com/members/wx1905494155) 翻译。未经许可，禁止转载！
英文出处：[Jeff Atwood](https://blog.codinghorror.com/what-does-stack-overflow-want-to-be-when-it-grows-up/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【伯乐在线导读】：今年 9 月，程序员必备神器之一的 Stack Overflow 正式成立 10 周年了。截至目前为止，SO 用户量高达 930 万，也许你经常在 SO 上找解决方案。但你可能还不真正了解 SO。来看看 SO 创始人 Jeff Atwood 是怎么说的。
现实生活中常常有人问我，我的工作是什么，对此我有一个 15 秒的回答：
> 
我们创建了一个类似维基百科的网站，程序员们可以在上面发表或者解答问题。网站的名字叫 [Stack Overflow](https://stackoverflow.com/)。
截至 2018 年 9 月，Joel Spolsky 与我共同创建的 Stack Overflow 已经走过了 10 个年头。从 2012 年开始我就在做其他的工作了，但是能让人们在我过世之后还能想起我的东西，那肯定还是我的老伙伴 Stack Overflow。
这里我好像应该滔滔不绝地说 Stack Overflow 有多么优秀，而我作为创始人是有多么伟大。
![](http://www.poorlydrawnlines.com/wp-content/uploads/2011/09/bragging.jpg)
但I这些我都不在乎。
我真正在乎的是，Stack Overflow 对程序员们是不是有帮助。对此，让我们看看如今最牛叉的开发者之一，我的偶像 John Carmack 是怎么评价的。
> 
![Stack Overflow为提升开发者的效率事业差不多贡献了好几亿美元](https://d2mxuefqeaa7sj.cloudfront.net/s_C8591C5627C21D2C099CE3885D4246D7CFCC30D73E98C53D54E39F0983BC11D8_1541195322340_image.png) 在为提升开发者的效率方面，Stack Overflow 可能贡献了好几十亿美元
说实话，2013 年 9 月 17 日是很美好的一天。我读到这条推的时候吓了一跳，不光是因为我经常用 Carl Sagan 的方式读 Billions 这个词。我在 Twitter 每隔几天就会读到一些残酷无尽的人间疾苦，以及人们在网络上的互相叫骂。与此相反，那一天是我感觉到的只有喜悦。这也提醒了我，我该查查 Twitter，看看如今谁还对网络抱有不同的理解。
Stack Overflow 有着如此多用户，也帮助了一个时代的开发者，对此我感到既荣幸又谦卑。但是，实现这一成就的并不是我。
- 是你们，对 Stack Overflow 贡献了经过深入研究后想到的提问；
- 是你们，对 Stack Overflow 贡献出了简洁而清晰的答案；
- 是你们，编辑了 Stack Overflow 的提问或答案，并使其变得更好；
世界各地开发者们所贡献的那些大大小小的提问与回答，把 Stack Overflow 变成了一个在开发领域能与维基百科抗衡的创意共享知识库。这实在是…非常的不可思议。
![](https://blog.codinghorror.com/content/images/2018/10/stack-overflow-homepage-oct-2018.png)
不过成功的故事都很无聊。这个世界上有很多人，本身运气好，但还时不时的告诉别人是自己的努力以及喝活力汽水换来了成功。我觉得失败的故事更有教育意义，在建立业务与规划未来时，我把自己想象成深渊专家，并开始一场比赛。这是我自己做事的习惯。
![abyss-oc](https://blog.codinghorror.com/content/images/2018/10/abyss-oc.jpg)
*当你在凝视深渊的时候，深渊也正在凝视着你 – Friedrich Nietzsche*
由此，我现在要与耀眼的深渊对视，预测一下Stack Overflow未来十年会遇到的挑战。这之前，我要先澄清以下事实。
**1、从 2012 年 2 月开始，我就没有再为 Stack Overflow 做事了，也没有对其运营有过任何建议。**你问我对如何运作Stack Overflow 竟然能没有建议？额，那你可能不认识我。你问我难道我不会时不时给员工发邮件告诉他们我的想法？我也许会吧，但是我为数不多的归档邮件可以证明，这个事情很少发生。
**2、Stack 有着优秀的员工**，他们中的大多数（包括我离开之前的 Stack Overflow 社区成员）都能对我们的使命给出更好的，不像我那样胡思乱想的阐述。我会用生命信任他们吗？不会。但是我会用 Joel 的生命信任他们！
**3、Stack Overflow并不属于我或者 Joel，或者其他一些优秀的开发者。**Stack Overflow 的运作靠的是世界各地日复一日做开发的人们，就像你或我一样。我觉得 Stack Overflow 就像个家长，它的目标是让孩子们最终能离开家长身边，成为可以独当一面的大人。
4、作为 Stack Overflow 的创始人，我在社区成立的最初四年里，花了非常多的时间参与制定规则与规范。你现在阅读的是我所[强观点，弱坚持](https://blog.codinghorror.com/strong-opinions-weakly-held/)。这只是我的一些想法，我也希望自己的预测是准确的，但是这并不意味着我可以预测未来，或者我有资格去预测未来。不过我并不会以自己是否具有资格而不去做一些事情。
## Stack Overflow首先是一个 wiki
Stack Overflow不仅是个论坛，它跟维基百科有很多相似之处。我们衡量问题和答案是否有意义的方式，不是看那些问题和答案对特定几个人的帮助，而是看随着时间的推移，这些问题和答案能不能帮助到越来越多的人。我从 [2008 年 Stack Overflow 上线](https://blog.codinghorror.com/stack-overflow-none-of-us-is-as-dumb-as-all-of-us/)后就在强调这个关系。来看看下面谁的地位最高。
![](https://blog.codinghorror.com/content/images/2018/10/stack-overflow-venn-diagram.png)
为了强调这一核心价值，Stack Overflow添加了一个简洁的功能。那就是在用户资料里会显示，你所贡献的问题与回答帮助到了多少人。
![](https://blog.codinghorror.com/content/images/2018/10/stackoverflow-people-reached-profile-stat-1.png)
这些问答内容到底服务于谁？回答问题为何有如此严格的审核过程？对于 Stack Overflow 最常见的抱怨通常来自于对前面这两个问题的误解。
> 
![我希望更多的人能明白，Stack Overflow并不是一个“回答我的问题”的地方，它是“让我们合作建立一个对未来的开发者们有益的地方”。也许Stack Overflow应该更加努力去帮助用户理解这件事。](https://d2mxuefqeaa7sj.cloudfront.net/s_C8591C5627C21D2C099CE3885D4246D7CFCC30D73E98C53D54E39F0983BC11D8_1541370256957_image.png) 我希望更多的人能明白，Stack Overflow并不是一个“回答我的问题”的地方，它是“让我们合作建立一个对未来的开发者们有益的地方”。也许 Stack Overflow 应该更加努力去帮助用户理解这件事。
如今很多用户，甚至泡在 Hacker News 上的技术圈网友，都不知道 Stack Overflow 上有个功能，那就是每一个问题都是可以修改的，即使是没有登录的匿名用户也可以修改。对此我深表惊讶。这个功能不奇怪，对吧，因为 Stack Overflow 就是一种维基百科，这也是维基百科的运行模式，任何人都可以修改任何内容。不信的话，现在就去试试吧，找一个你认为可以提高的问题或者回答，点击“改善这个回答（improve this answer）”或者“改善这个问题（improve this question）”，然后写下你的改良版。
![](https://blog.codinghorror.com/content/images/2018/10/stack-overflow-edit-question.png)
Stack Overflow 有很多功能（甚至也包括我自己在 2012 年之前的一些所作所为）都容易导致用户误解其核心价值。理论上，“如今每一个开发者都听过，用过以及了解 Stack Overflow”，但我觉得这个假设不准确。毕竟每时每刻都有新的开发者诞生。说得更复杂一点，Stack Overflow 的使用模式有三种，从大到小，以倒金字塔的形式排列如下：
**1、我在需要的时候去搜索答案**
用户直接上网搜索，搜索引擎会直接显示出 Stack Overflow 中的高票答案。出现在搜索引擎第一页，这也是Stack Overflow的主要目标。如Stack Overflow正常运行，98%的开发者在他们的整个职业生涯中，不需要主动提出或者回答问题。只要通过网络搜索就可以找Stack Overflow上到他们需要的结果。这是个好事，非常好的事。
**2、我遇到很困难的问题时会参与Stack Overflow的讨论，因为单纯的搜索找不到我想要的答案**
只在遇到难以解决的困难时，参与Stack Overflow的讨论，这很合理。然而，我觉得这个阶层的用户最容易感觉到Stack Overflow不是那么容易使用，因为这类用户可能对Stack Overflow很熟悉，但是并不清楚发布问题的流程。并且在他们急切想寻找答案的时候，他们没有时间或心思去应对Stack Overflow对于问题背景、格式、描述以及引用要求。
**3、为了自己的职业发展，我主动参与Stack Overflow的问题讨论**
这个阶层的用户很有经验，他们贡献了很多答案，也了解什么样的问题是好问题，是他们感兴趣、愿意回答的问题。他们不经常提问，因为他们知道如何去全面搜索他们想要的答案。但是他们一旦提问，那一定是个示范性的好问题。
（理论上这里还有个第四阶层用户，他们无私的贡献了很多提问与回答，目的只是为了推动软件开发行业的发展，造福于新一代的开发者们。但是我们没空提这些大神，你们只会让我们显得更加平凡，所以我们就此打住吧）
第一阶层的用户在社区里开心地逛了好几年，却在变为第二阶层用户时，一下子有了不开心的用户体验。对此我一点儿也不惊讶。我认为解决这个问题最主要的方式，就是改变并提高提问页面的用户体验。另外值得注意的一点是，用户在提出了某个问题后，可能收到关于问题信息不足的负面反馈，但是他们也许并不知道，你的问题应该是“有益于其他用户而并不只是你自己”。
Stack Overflow采用了维基百科的模式，也使其自身受到了很多限制。即使用户在提问前就知道这些，很多时候到底什么是“有用的信息”也很难判断。同理，很多时候我们也不确定到底什么样的话题，人群或者地点需要一份维基百科。[Henrietta Lacks](https://en.wikipedia.org/wiki/Henrietta_Lacks) 有自己的维基百科页面，这毫无争议，但是他住在奥哈马市的表兄 Dave，那个提出了一个关于 PHP 5.6 的奇怪问题的人，是不是该被写入维基百科呢？
## 随着时间的推移，重复内容像地雷一样遍地都是
这事我很早就预料到了。老实说，我有点庆幸自己在 2012 年离开了 Stack Overflow，这样我就不用去处理这个难以置信的技术性难题：重复性内容。在我听到的关于 Stack Overflow 的所有抱怨里，重复内容是我觉得最有共鸣的。
如果你接受Stack Overflow是个类似于维基百科系统这一前提，那同理你显然不能接受，在维基百科中，对于意大利有五个不同的词条。Stack Overflow不允许对于同一个技术问题有重复的提问。我们确实有很多避免重复问题的功能，比如输入问题时的同步搜索，以及提交问题前，你会看到一个很明显的，鼓励用户先去搜索相关问题的搜索框。
![](https://blog.codinghorror.com/content/images/2018/10/stack-overflow-how-to-ask.jpg)
… 如何查找并判断重复内容是个非常有难度的问题，即使是Google这样的公司，有着名副其实世界最聪明的工程师团队，专攻了20年也没有解决这个问题。
当你在一个不允许重复问题的网站中提问时，系统去重的难度取决于总问题数量，处理一百万的问题总量的去重与一千万甚至一亿相比，是非常不同的。系统处理问题去重的难度，会从不太难处理变为最终的完全无法处理。比如你提出了一个与艺术类相关的问题，那么系统需要根据你的提问内容，在不胜枚举的已有问题中进行筛选，以确保没有看起来相似的提问。
![](https://blog.codinghorror.com/content/images/2018/10/stackoverflow-asking-duplicate-question.png)
等会儿，还有个更难的问题！
- [相似问题中有一点内容变化也是可以的](https://stackoverflow.blog/2010/11/16/dr-strangedupe-or-how-i-learned-to-stop-worrying-and-love-duplication/)，因为十个不同的人在提出同一个问题时，完全可能使用毫不相关的词语来形容这个问题。我知道这听起来很疯狂，但是相信我：人类极其擅长做这样的事。我们希望保留这些重复的问题，并且让他们都指向同一个主问题，以便于用户更好的搜索他们需要的内容，即使这些用户使用了那些平常不太会被用到的词语去描述问题。
- 如何判断你提出的问题是不是重复，这是个不小的挑战。多少词语的重叠才能决定一个问题是不是与另一个重复？谁来决定？不同人有不同理论。这是个以人类语言为标准的解析，然后人类吧……不可预知。这个系统无法做到让所有人满意，去重的缺陷会一直存在于系统之中。
我对于越来越严重的重复问题并没有一个好的解决方案。但是我想指出，早期在 [Stack Exchange](https://stackexchange.com/sites#) 有很多先例，它们把网站分为“初级”和“高级”区域，不同区域的规则不同。我们在别的地方也能找到类似的例子，比如 Math 和 MathOverflow，English 和 English Learners， Unix 和 Ubuntu，也许是时候搞一个以初级用户为主的 Stack Overflow了，在那里我们可以允许多一些重复，少一些规则。
## Stack Overflow是个可以同行评审的竞争性系统
Stack Overflow确实是个相当明确的竞争性系统，它的一大标志就是[“总会有更好的解决办法”](http://wiki.c2.com/?ThereIsMoreThanOneWayToDoIt)。根据我的多年观察，激励开发者最有效的方式就是…巧妙地暗示出别人的解决方案也许比你的更好。
> 
![- 你好Randall。医生说你能听到我说话，虽然你看起来像植物人。我是来告诉你，别着急慢慢康复。因为Ross接替了你的工作，并且做的非常好。他甚至找到了你代码里的瓶颈，还说他改过的代码变快了两倍。 - 这不可能！！！！！我现在就回办公室！](https://blog.codinghorror.com/content/images/2018/10/geek-hero-motivating-programmers.png) – 你好Randall。医生说你能听到我说话，虽然你看起来像植物人。我是来告诉你，别着急慢慢康复。因为Ross接替了你的工作，并且做的非常好。他甚至找到了你代码里的瓶颈，还说他改过的代码变快了两倍。
– 这不可能！！！！！我现在就回办公室！
Stack Overflow的竞争性质体现在了它的公开声望系统上，就是[用户名旁边那个拥有神奇力量的数字](https://blog.codinghorror.com/for-a-bit-of-colored-ribbon/)。所有的声望值都来源于其他用户，而不是所谓的系统。
![](https://blog.codinghorror.com/content/images/2018/10/stack-overflow-top-rep-by-year.png)
每当你提出问题或者提交回答时，你的问题或回答都可以被其他用户指指点点，他们可以编辑、标记、关闭、打开、顶、踩或者收起。这样做的目的是让 Stack Overflow 成为一个同行评审和友好竞争的系统，就像在公司里，你的代码被你从没见过的另一个部门的人来评审。有人以友好的方式去质疑你所提问题的提论，也是完全合理的，比如，[你真的想用这个正则表达式去匹配 HTML 吗](https://stackoverflow.com/questions/1732348/regex-match-open-tags-except-xhtml-self-contained-tags)？
我完全清楚这种竞争性质的同行评审系统，并不适合每一个人。Stack Overflow 采用维基百科的模式，导致它存在不能接受重复内容这样的限制。那么根据你的情况与背景，同行评审时，你收到的评价可能会让你觉得不舒服。
我听部分用户反应，在 Stack Overflow 提问的过程中会感觉到焦虑。对我来说，在 Stack Overflow上提问，应该感受到一种 ”我要展示出我最好的一面“ 的正常焦虑：
- 在你的同事面前演讲的焦虑
- 考试要取得好成绩的焦虑
- 开始新工作，与你尊敬的优秀同事们一起工作的焦虑
- 第一天去学校报到，即将见到新同学的焦虑
至于那种完全不会感到焦虑的地方，我唯一能想到的就是，从事了很久的工作，已经不再关注与工作本身，因此也没有那种担心有一天就会丢了工作的焦虑。这样怎么会好呢？所以说我不喜欢零焦虑的系统。
也许你不喜欢竞争。那么能不能有个少量竞争模式的问答系统呢？一个没有投支持或者反对票功能的系统，这样无论发表什么内容都不会感觉焦虑。这就像是一个全是你的支持者的网络，大家都相信你，希望你成功。这当然也是可以的。我认为应该有类似这样的网站，用户可以根据自己的需求与目标来选择适合自己的体验。那么 Stack 应该建立一个这样模式的社区吗？这样的社区已经有了吗？这是个开放题。也请随意在留言区发表你的看法。
## Stack Overflow是为了日常开发者而设计的
Stack Overflow的目标用户到底是谁，这也是经常容易被混淆的一点。这个回答很直观，而且从从未改变过：
![](https://blog.codinghorror.com/content/images/2018/10/stackoverflow-for-business-description.png)
一个为专业和热情的程序员而存在的问答平台。这是指：
> 
当前正在从事程序开发职业的人，或者如果愿意就能立即胜任程序开发工作的人。
如果你觉得好奇，这个定义的一部分是公开的商业决策。为了盈利，你的用户群体必须要有一部分拿着开发者薪水的人，或者在找开发者的工作的人。整个 Stack Overflow 社区也许有着知识共享的标签，但是它并不是个非营利组织。我们的出发点是可持续经营，这也是为什么我们在 Stack Overflow 上线一年之后，就成立 [Stack Overflow Careers 招聘平台](https://blog.codinghorror.com/stack-overflow-careers-amplifying-your-awesome/)的原因，回顾一下，成立的确实有点过早了。为了实现比 2009 好很多的集成化用户体验，招聘平台被归入了 Stack Overflow，放在了 [stackoverflow.com/jobs](https://stackoverflow.com/jobs)下面。
用户的选择定位并不是说要排斥非开发者，但是 Stack Overflow 确实是一个有着严格同行评审，对已经在从事相关行业的人来说非常优秀的功能，但同时也是对于学生或者初学者来说很不友好的功能。这也是为什么，我每次在推特上，看到有人推荐学生去 Stack Overflow 找答案时，我会小心翼翼的建议不要这样。对于开发领域的新手或者学生来说，他们需要的，与 Stack Overflow 所提供的是完全相反的。他们需要的是：
- 一对一的指导
- 实时屏幕共享协作
- 实时语音
- 理论背景知识课程
- 初学者练习
- 一个练习与实验的场所
这些都是对初学者来说，很好很合理的事情，但是 Stack Overflow 一个也不做。你可以通过 Stack Overflow 来从头学习如何编程吗？理论上你可以通过任何软件做任何事情，你甚至可以通过 Reddit 与人进行日常交流，如果你是受虐狂的话。但是答案还是肯定的，理论上你可以通过 Stack Overflow 学习如何编程，如果你是喜欢竞争模式（声望、被关闭、被踩）的神童，也完全能接受要去帮助别人而不只是自己学习知识这一观点。但是我强烈不推荐这样做。对初学者来说，除了 Stack Overflow 外还有[很多更好更合适的平台](https://blog.codinghorror.com/heres-the-programming-game-you-never-asked-for/)。那么 Stack Overflow 能不能成为一个适合新手和学生的平台呢？我不清楚，我也不能决定。
这些就是我要说的。我们可以不再与深渊对视。
我希望我的观点对 Stack Overflow 不会有什么负面影响。总的来说，我认为目前的 Stack Overflow 很强大。但是，无论是 [2008 年](https://stackoverflow.blog/2008/11/25/stack-overflow-is-you/)还是2018 年，我怎么想有关系吗？
> 
Stack Overflow 是你们的。 Stack Overflow 将信念赌在了这一点：信任你的同行。Stack Overflow 的成长离不开那些积极参与社区讨论的开发者们。是你们让我相信开发者社区是最好的学习与成长的地方。是你们让我收获了如此多的对于 Stack Overflow 的赞誉。这不是我的功劳，而是你们的。 很久之前我在 Code Horror 上就知道了合作的力量是多么强大。目前我们的社区已经达到我今生难以企及的高度。我唯一能要求的，或者是我们能要求的，就是大家互相帮助互相成长。 如果有人认可了你的付出，那么你值得为此感到骄傲。
开发者社区的力量能够创造以及终结 Stack Overflow。Stack Overflow 长大了会成为什么？它的未来将由我们共同创造。
![](https://blog.codinghorror.com/content/images/2018/10/stackoverflow-none-of-us-is-as-dumb-as-all-of-us.jpg)
PS：Stack Overflow 十周年快乐！
