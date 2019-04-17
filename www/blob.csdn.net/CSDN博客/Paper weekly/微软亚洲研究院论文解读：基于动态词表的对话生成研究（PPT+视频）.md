# 微软亚洲研究院论文解读：基于动态词表的对话生成研究（PPT+视频） - Paper weekly - CSDN博客





2018年01月03日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：3481









 本文为 12 月 27 日，北京航空航天大学博士生、微软亚洲研究院实习生——**吴俣**在第 21 期 PhD Talk 中的直播分享实录。

本次 Talk 的主题是基于动态词表的对话生成研究。首先，吴俣博士带大家回顾了近几年来聊天机器人领域的发展，并仔细对比检索式聊天机器人和生成式聊天机器人的优点和缺点。




随后，他还以第一作者的身份，解读了北京航空航天大学和微软亚洲研究院于 AAAI 2018 发表的工作 Neural Response Generation with Dynamic Vocabularies。 




这篇论文致力于在对话生成时构建动态词典，在使 decoding 速度加快的同时，还能去除不相关词汇的干扰。在不影响效果的前提下，本文模型将在线生成速度提升了 40%。 







△ Talk 实录回放




# 浅析对话系统




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNHAwQS6ibbDBtbMsB2IoIldGu9z3NibW0DKo3w4oGYMYUI2BCuib5bDZug](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNHAwQS6ibbDBtbMsB2IoIldGu9z3NibW0DKo3w4oGYMYUI2BCuib5bDZug/)




**对话系统主要分为两类，一类是任务型，另一类是非任务型。**任务型对话系统主要应用于企业客服、订票、天气查询等场景，非任务型驱动对话系统则是指以微软小冰为代表的聊天机器人形式。 




之所以强调这一点，是因为今年我在 ACL 发表了一篇论文，有同学发邮件问我为什么参考了论文和源代码，还是无法让聊天机器人帮忙订披萨。我只能说，目前聊天机器人实在种类繁多，有的机器人只负责闲聊，有的机器人可以帮你完成某些特定任务。 




本次 Talk 会更侧重于介绍闲聊机器人，也就是非任务驱动型对话系统。首先我想给大家推荐一篇关于聊天机器人的综述文章 — **A Survey on Dialogue Systems: Recent Advances and New Frontiers**。




这篇文章来自京东数据科学团队，是一篇较为全面的对话系统综述，其中引用了 121 篇相关论文，并对论文进行了归类。不仅非常适合初学者，也能让大家对聊天机器人领域有一个更为全面的认识。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNOSfpHXRV2RbEbs8akOOozTmHrBnEF4AD9KQicy4cEvkxNPD2o0bTp2g](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNOSfpHXRV2RbEbs8akOOozTmHrBnEF4AD9KQicy4cEvkxNPD2o0bTp2g/)




**面向任务的对话系统主要分为知识库构造、自然语言理解、状态跟踪和策略选择。**针对知识库构造，假设我们的使用场景为酒店预订，那首先我们需要构建一些和酒店相关的知识，比如酒店房型、报价以及酒店位置。




具备了这些基础知识之后，接下来就需要展开对话，通过自然语言理解去分辨问题类型（酒店类型、房间类型等）。确认好相关类型后，我们需要借助 policy 模块，让系统切换到下一个需要向用户确认的信息。更直观地说，我们需要循循善诱引导用户将右表信息填写完整。




# 聊天机器人类型




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN4FEVRbmjOSBkowZibAdTdicEWH7I3uwQQFeicUCCyythNCXo65wyEOk6Q](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN4FEVRbmjOSBkowZibAdTdicEWH7I3uwQQFeicUCCyythNCXo65wyEOk6Q/)




普遍来说，聊天机器人主要分为两类，我认为准确来说应该分为三类。




比较早期的研究基本属于第一类：**基于模板的聊天机器人，它会定义一些规则，对你的话语进行分析得到某些实体，然后再将这些实体和已经定义好的规则去进行组合，从而给出回复。**这类回复往往都是基于模板的，比如说填空。




除了聊天机器人，这种基于模板的文本形成方式还可以应用于很多其他领域，比如自动写稿机器人。




目前比较热门的聊天机器人应该是另外两类，一类是检索型，另一类则是生成型。**检索型聊天机器人，主要是指从事先定义好的索引中进行搜索。**这需要我们先从互联网上获取一些对话 pairs，然后基于这些数据构造一个搜索引擎，再根据文本相似度进行查找。




**生成型聊天机器人目前是研究界的一个热点。**和检索型聊天机器人不同的是，它可以生成一种全新的回复，因此相对更为灵活。但它也有自身的缺点，就像图中的婴儿那样，它有时候会出现语法错误，或者生成一些没营养的回复。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNEu96OWwLzlkiaztibyQiaBrWZbErMfJ28IOaCsfVPPA6Fzicox3NJ891pA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNEu96OWwLzlkiaztibyQiaBrWZbErMfJ28IOaCsfVPPA6Fzicox3NJ891pA/)




**检索型聊天机器人首先需要构建一些文本和回复的 pairs，然后再训练匹配模型，上线之后先做检索再做匹配。**相似度算法有很多种选择，现在一般都采用深度学习，如果是做系统的话，肯定需要融合很多相似度的特征。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNHAN6ZBbOth050qBA9iaY74IiahuiaB1QdOmKDTMaPvian2FlLSx7j4GHXw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNHAN6ZBbOth050qBA9iaY74IiahuiaB1QdOmKDTMaPvian2FlLSx7j4GHXw/)




**生成模型大多都是基于 Seq2Seq 框架进行修改**，所谓万变不离其宗，不管大家怎么做，都会是以这个框架为基础。文本生成也是如此，在 RNN 语言模型和 Seq2Seq 出来之后，几乎所有生成模型都是基于这个框架。即使把 RNN 换成 CNN 或 Attention is All You Need，也仍然离不开这个框架。




# 检索型 vs. 生成型




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNWb0Suhddia7qey2VZZMvhq8og6icfIhqhzhllUvNYmwicUfROB9l9EFvA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNWb0Suhddia7qey2VZZMvhq8og6icfIhqhzhllUvNYmwicUfROB9l9EFvA/)




**检索型聊天机器人的最大优点在于它的回复多样且流畅，其次，这个系统对编程者的入门门槛较低。**即使你对深度学习和自然语言处理不甚了解，但只要你善于写代码，并且能从网上抓取一定量的数据，就可以搭建一个检索型聊天机器人。




另外，对于研究人员来说，检索型聊天机器人比较易于评测，借助 MAP、MRR、NDCG 等传统信息检索方法即可完成。 




**检索型聊天机器人的缺点在于它过于依赖数据质量**。如果你抓取的数据质量欠佳，那就很有可能前功尽弃。




就工业界来说，要评估某个检索型聊天机器人，首先我们会看其背后的排序算法，其次不能忽略的则是数据质量和规模，最后再看其回复数据是否足够有趣，以上几个因素共同决定着检索型聊天机器人的整体质量。 




**生成模型的最大优势在于有一套通用 code，可以忽略语言直接开跑。**只要在某种语言上跑得较为顺利，就可以将其应用到所有语言上。




**很多人认为 safe responses 是生成式聊天机器人的一个缺点，但其实从某种角度上来说，这也是它的一个优点。**相比检索型聊天机器人，它生成的回复质量较为稳定。




**生成模型的另一个优点是，它非常容易实现多轮对话，并且能够偏向于某种情感。**假设我希望生成一句高兴的话，那么用生成模型会比检索模型更容易实现。 




对于早期的研究者来说，**生成模型的最大缺点在于它的回复较为单一**。其次，**由于缺乏自动评评测手段，研究人员难以对生成式聊天机器人进行评估**。一个模型的好坏，更多需要靠人进行标注。此外，对工业界而言，生成式聊天机器人的门槛会相对较高。





# 怎样提高生成的多样性




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNiapJSPzUqFFzyAwMic520GEVQ2lPdOibicPNDrUxq0Nsic441HLuuGiaiaGibQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNiapJSPzUqFFzyAwMic520GEVQ2lPdOibicPNDrUxq0Nsic441HLuuGiaiaGibQ/)




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN9VZDuXY2iahcS1Nc6c0zWyboaNiaBmxHIwoTH7GqoZPEjZjNicnAdkESA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN9VZDuXY2iahcS1Nc6c0zWyboaNiaBmxHIwoTH7GqoZPEjZjNicnAdkESA/)




**第一种方法是将模型做得更复杂**，比如上图这篇论文使用了 latent variable 来解决 boring responses 这个问题。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNwYia6ZmrY9l1FtJZ1AKSZJyT4G6OqH7BvQLvJb8M1nxiawfHiczn059MA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNwYia6ZmrY9l1FtJZ1AKSZJyT4G6OqH7BvQLvJb8M1nxiawfHiczn059MA/)




上图中的论文，则是**在生成时将概率 bias 到一些特定的主题词**。假设某个词是主题词，我们就在生成过程中相应提高它被选中的概率。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNsnVicqHSTXUkoneBvZVsEc3qq5xJQykiadicEZNQ1Nweyl0uQ8wGk4MDg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNsnVicqHSTXUkoneBvZVsEc3qq5xJQykiadicEZNQ1Nweyl0uQ8wGk4MDg/)




**第二个流派是采用重排序方法**，目前最简单有效的方法是先用生成模型生成大量回复，再用分类器对回复进行排序，排名越靠前效果越好。只要生成的回复数量够多，该方法就一定可行。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNDxHJGPic0VP3wSxRMkdx3qoXSLAHBpA683eWdnG61uacGBW6gGsv9iaA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNDxHJGPic0VP3wSxRMkdx3qoXSLAHBpA683eWdnG61uacGBW6gGsv9iaA/)




**第三种方法是基于增强学习的**。增强学习有两种不同方法，一种基于策略，另一种基于价值。




基于策略的代表作来自李纪为，其基本思路是：假设已有生成模型 G，给定一个 input 并生成 20 个回复，利用排序公式 P(S|T) + λP(T|S) 对回复进行评分作为 reward。Reward 值越大，梯度更新则相应越大。 




**我们再来看看 GAN 的相关方法**。李纪为对 SeqGAN 的模型加以改进，将其用在了回复生成上。




其基本思路是，每生成一个词的同时，用搜索的方法去搜索其最后生成的完整句子，然后用 discriminator D 对其进行评分，分值越高，意味着词的 reward 也越高。之后的思路则跟 SeqGAN 一模一样。




# 本文思路




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNlWt38w3J7P1ViclIW72JqKNqAEsOwpYZt6qqdSAUBa8vS5kUuEndr1A](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNlWt38w3J7P1ViclIW72JqKNqAEsOwpYZt6qqdSAUBa8vS5kUuEndr1A/)




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNCbW0DMEOIAxOyZyHib3osjv6Ava4fIYpZP4ZhyOhLrsAgBj6j9I3BNQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNCbW0DMEOIAxOyZyHib3osjv6Ava4fIYpZP4ZhyOhLrsAgBj6j9I3BNQ/)




**我们做这篇论文的初衷，是为了提出一种不借助繁重工具或算法的回复生成方法。**因为无论是复杂模型、后排序、增强学习还是 GAN，都属于一种用时间换精度的方法。 




我们希望在避免产生大量时间开销的同时，还能提高回复生成的质量。**提升效率的关键在于 Seq2Seq 的最后一层 — 投影层**，这个投影往往会是一个大型矩阵。




我们认为其实没有必要形成这个大型矩阵，因为有用词汇仅有很小一部分，而这一小部分词汇就足够生成一句非常流畅、高度相关的话。比如对“的地得”这类功能词和与 input 相关度较高的词做一个并集，就可以仅用一个小规模字典生成极为流畅的有效回复。 




详细来说，我们会给每一个 input 构建一个动态词典。这么做的目的是为了减少在线 decoding 时间，同时对不相关词进行剔除。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNgKxaic5QXUCaD3nYPDwQZDTOibiaic4BkX8KGK1kQwsWJZUk9ZUYMHhz7A](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNgKxaic5QXUCaD3nYPDwQZDTOibiaic4BkX8KGK1kQwsWJZUk9ZUYMHhz7A/)




**本文其实是在 Seq2Seq 的基础上加了一个动态词表，每给一个 input，我们会生成两类词。**




**第一类词的生成完全基于规则，即静态词典。**静态词典主要包含一些功能词，功能词主要起润滑剂的作用，它们能让一句话的语法变得通畅。静态词典是基于词性构建的，主要包含代词和助词，名词和动词不包含在内。 




**第二类词是内容词，即动态词典。**动态词典是根据 input 猜测与其相关的词，即我们可以用哪些词对 input 进行回复。这个词典的构建不能再像静态词典那样基于词性，而是要借助分类器或词预测模型，预测哪些词和已给定的 input 相关。 




**有了这两个词之后，我们就可以给每个 input 构建一个词典。**这个词典的规模会很小，很小的概念是指原来的词典规模是 3 万，现在能缩减到 1000-2000 这个量级。




从矩阵乘法角度来看，如果能把一个矩阵从 N 乘以三万的维度，缩减到 N 乘以一千的维度，就可以大大提升矩阵乘法的运算速度。




# 词预测模型




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNyuFa4GibLzDEVKxjZia07pGvNA9OWVXTo3H76N7sgoXdeNBGvyib1rFWg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNyuFa4GibLzDEVKxjZia07pGvNA9OWVXTo3H76N7sgoXdeNBGvyib1rFWg/)



接下来我们来看如何做词预测，即如何对内容词（content words）进行预测。内容词的 input vector 是 encoder 生成的 vector。有了 vector 后，我们需要过一个回归模型（MLP），继而预测需要用到哪些词。这个预测的损失就是将最后出现在回复里的词作为正例（标签为 1），再随机采样一些负例作为 0 标签的词，从而进行预测。 




如何采样负例非常关键。剔除掉一句话中的功能词，大概会剩下 10-15 个正例的词。我们需要通过频率对负例进行采样，如果一个词经常出现，那么它被采样为负例的几率就会很大。 




通过对负例进行采样，我们在进行词预测时，可以更准确地预测出内容词是什么。反之，这个词预测模型会跟 Seq2Seq 生成回复模型出现同样的问题，即总是出现高频词。只有利用频率对负例进行采样，才能让高频词在构建词典时就被剔除。




# 时间复杂度




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNQRtfOWicKaw7UUvU2byRU855pwVUx6ibknoANlf0AkJZGicScnAHa7x6A](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNQRtfOWicKaw7UUvU2byRU855pwVUx6ibknoANlf0AkJZGicScnAHa7x6A/)




在介绍完词预测方法后，我们来看时间复杂度的计算，即以什么样的速度进行 decoding。




首先将 Seq2Seq 和本文的 decoding 方法进行对比，可以发现二者在 GRU 和 Attention 上花费的时间完全一致，但是**本文方法在 Projection 上花的时间会少很多**。




原因在于 Seq2Seq 的 vocabulary size 通常都很大，比如 3 万这种级别乘出来的数。而本文这个 T 可能只有几千，并且**我们无需为每个词建立一个表，而是为每一句话建立一个表**。因此，我们构建词典的时间开销要远小于从全局字典里进行词预测。




**当然，这种情况的前提是回复词数需大于 1**。当回复词数等于 1 时，即逐词预测，本文方法反而会比 Seq2Seq 变得更慢。也就是说，在词的数量越多的时候，词典规模越小，节省的时间也就越多。




经实验证明，**这种方法相比 Seq2Seq 能节省约 40% 的时间**。




# 模型训练




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNQGyicsI0qjxLfLjDicw0Erzh0d38a3XExv6PcWpoKh4rzic8TWTicqhZHQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNQGyicsI0qjxLfLjDicw0Erzh0d38a3XExv6PcWpoKh4rzic8TWTicqhZHQ/)




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNWINRnM5I3RonK2tLldzqAYwKN0icHh8PNbnrUoQ0YMOcDQJnf495FDg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNWINRnM5I3RonK2tLldzqAYwKN0icHh8PNbnrUoQ0YMOcDQJnf495FDg/)




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN8TeYbL4kmAqfia83LBSaJ4EzsCWNXzVrpBvlBjXCTcKg8I9QZeFkB3w](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN8TeYbL4kmAqfia83LBSaJ4EzsCWNXzVrpBvlBjXCTcKg8I9QZeFkB3w/)




如果只对动态词典进行训练，将导致训练和预测的时候出现一些 gap。即使在训练的时候就采用动态词表这种方法，也依然会面临一个问题，就是你不知道选词这个过程会对你做回复造成什么影响。




为了解决这个问题，**我们在训练时选择将动态词典作为一个隐变量来处理**。针对公式的详细推导，大家可以参考论文。 




由于是隐变量，假设动态词典 T 是完全变例，即一个词有选或者不选这两种可能。如果有 3 万个词，那么 T 就有 2 的三万次方这么大，因此这种方法是不可行的。那我们应该怎么做呢？




我们可以通过蒙特卡洛采样的方法，来估计词典会对回复产生什么影响。我把![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN6T6MDVrdz9MmRicOC44Ak8Y65XE4CmcuibhrtneNykf0ibr9HTb7LiaDRg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSN6T6MDVrdz9MmRicOC44Ak8Y65XE4CmcuibhrtneNykf0ibr9HTb7LiaDRg/)变成了真正采样，相当于采样 S 次，且对每次所出结果都进行一次计算，然后对结果进行平均。 




这样一来，我们就可以把词典构建和回复生成这两个损失串在一起，相当于放入一同一个公式里表示，而不是将词典和生成分开单独训练。利用逐次采样得出的结果，来评估动态词典在不同情况下，其相对应的回复生成的损失是什么。 




由于这个损失是通过采样得出，因此它会和 RL 一样存在 variance。因此我们加了一个 baseline BK 用于梯度更新，从而使训练更为稳定。




# 实验




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNibtGeJWKxY03KdnWgLeJevZJOsbaeaWGcTibu8jIF8wKbF0fjamXT6IQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNibtGeJWKxY03KdnWgLeJevZJOsbaeaWGcTibu8jIF8wKbF0fjamXT6IQ/)




本文实验所用数据来自我们之前的一篇文章，这些词可以覆盖约 99% 的词。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNTmgRIzDOZbzVlqOy0o1wUiabFsCLQ6QnUia1ESb1frIhiaLjpX0s7Deow](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNTmgRIzDOZbzVlqOy0o1wUiabFsCLQ6QnUia1ESb1frIhiaLjpX0s7Deow/)

△ 本文使用的开源 baseline





![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNIcUL6b5Wuk0kbDibJVWGjFKWXicF4qdf2cb6lW25cKmvnpnSnibGJ7Opw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNIcUL6b5Wuk0kbDibJVWGjFKWXicF4qdf2cb6lW25cKmvnpnSnibGJ7Opw/)




**目前研究界仍未找到一个很好的自动指标，能用于回复生成或对话评测。**




现有的方法可分为四类：




**第一类方法是计算 BLEU 值**，也就是直接计算 word overlap、ground truth 和你生成的回复。由于一句话可能存在多种回复，因此从某些方面来看，BLEU 可能不太适用于对话评测。




**第二类方法是计算 embedding 的距离**，这类方法分三种情况：直接相加求平均、先取绝对值再求平均和贪婪匹配。




**第三类方法是衡量多样性**，主要取决于 distinct-ngram 的数量和 entropy 值的大小。




**最后一种方法是图灵测试**，用 retrieval 的 discriminator 来评价回复生成。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNGLLeibMTqjas9uTDOeW2GWylA4tuMl7IOJSDUtMVss5tIjhwfkicnA2g](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNGLLeibMTqjas9uTDOeW2GWylA4tuMl7IOJSDUtMVss5tIjhwfkicnA2g/)




表 1 中的前四行是 baseline，DVS2S 是将词预测和 Seq2Seq 的损失串在一起计算，S-DVS2S 则是对这两个 loss 分别进行计算。从结果可以看出，DVS2S 的效果均优于其他方法。 




表 2 是人工标注结果，数值 0 和 2 分别代表最差效果和最优效果，Kappa 则指三者的一致性。人工标注得到的 Kappa 值通常较低，也就是说，即使让真人来进行评测，也很难得到一致性的评价。




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNoWHttQmgwsqomltbIa0ibibu8uyE03SBdNSeCKoj3xSwX2aOyIcnqyrQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNoWHttQmgwsqomltbIa0ibibu8uyE03SBdNSeCKoj3xSwX2aOyIcnqyrQ/)

△ 速度对比：本文模型可节省40%的时间




![VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNBU69gUpYzYe7bGVqqD8DVRbC4WfzhGm21fVGvMabC9YQFYzDsHklxA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkORUkeMO1HCfkUbTVgJZSNBU69gUpYzYe7bGVqqD8DVRbC4WfzhGm21fVGvMabC9YQFYzDsHklxA/)

△ 案例效果对比




# 总结




首先，我们将静态词典换成了动态词典，用于聊天机器人中的回复生成。其次，我们提出了一种全新的方法，将词预测损失和回复生成的损失融入到同一个损失函数，以 joint 的方式对这个函数进行优化。最后，我们基于一个大规模数据集，对本文模型的效率和效果进行了验证。




****>>>> **获取完整PPT和视频实录**




关注PaperWeekly微信公众号，回复**20171227**获取下载链接。




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/?)




**点击以下标题查看往期实录：**




- 
[基于生成模型的事件流研究 + NIPS 2017 论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486772&idx=2&sn=e4aa1601872de283e9898e007ca8fd39&chksm=96e9d2b4a19e5ba2dc5f46b752a5b93d48efdcc3fa8ce5ad122a056dea049b3893b74a2585dc&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486772&idx=2&sn=e4aa1601872de283e9898e007ca8fd39&chksm=96e9d2b4a19e5ba2dc5f46b752a5b93d48efdcc3fa8ce5ad122a056dea049b3893b74a2585dc&scene=21#wechat_redirect)[亚马逊高级应用科学家熊元骏：行为理解研究进展](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486679&idx=1&sn=e142574dd940434db051c79c905cce15&chksm=96e9d357a19e5a41ecb48d318876ad7db1c36c0f761afc63368a8fcdcc11d33057902a47289f&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486772&idx=2&sn=e4aa1601872de283e9898e007ca8fd39&chksm=96e9d2b4a19e5ba2dc5f46b752a5b93d48efdcc3fa8ce5ad122a056dea049b3893b74a2585dc&scene=21#wechat_redirect)[清华大学冯珺：基于强化学习的关系抽取和文本分类](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486440&idx=2&sn=d43b6799f62337ec19f405bed1cd0138&chksm=96e9d468a19e5d7ea1cd673045eaa57dd8f5bee49074b6ef0f4fc64558e4626a0461867b216b&scene=21#wechat_redirect)

- 
[基于双语主题模型的跨语言层次分类体系匹配](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485671&idx=2&sn=9ae43be03e1df26a7cfb527f051d58c2&chksm=96e9d767a19e5e71171e29b285b1e7b5de07b99c061422e21c0cfd78a99eca4c1a9c603a4109&scene=21#wechat_redirect)


- 
[东南大学高桓：知识图谱表示学习](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485556&idx=1&sn=13a34ca78f311ef25a0aa624d961c2be&chksm=96e9d7f4a19e5ee201941e9111a41ca1f2e0abe35ea33c425f62c8d793d3dbbd86bb6be2715c&scene=21#wechat_redirect)


- 
[带多分类判别器的GAN模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485307&idx=2&sn=a72494118454b6b96b18a77f7d7490d3&chksm=96e9d8fba19e51ed901cbabf7e57f54547b512fa48aef29ae7b4b9b0b69a341098cbec18365e&scene=21#wechat_redirect)




**关于PaperWeekly**




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。





![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




