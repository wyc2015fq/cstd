# 专栏 | 香侬科技独家对话斯坦福大学计算机学院教授Dan Jurafsky - Paper weekly - CSDN博客





2018年07月23日 13:31:42[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1246









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**专栏介绍：**《香侬说》为香侬科技打造的一款**以机器学习与自然语言处理为专题的访谈节目**。由斯坦福大学，麻省理工学院, 卡耐基梅隆大学，剑桥大学等知名大学计算机系博士生组成的“香侬智囊”撰写问题，采访顶尖科研机构（斯坦福大学，麻省理工学院，卡耐基梅隆大学，谷歌，DeepMind，微软研究院，OpenAI 等）中人工智能与自然语言处理领域的学术大牛, 以及在博士期间就做出开创性工作而直接进入顶级名校任教职的学术新星，分享他们广为人知的工作背后的灵感以及对相关领域大方向的把控。




**本期采访嘉宾是斯坦福大学计算机学院终身教授Dan Jurafsky。**随后香侬科技计划陆续推出 Eduard Hovy (卡耐基梅隆大学), Anna Korhonen (剑桥大学), Andrew Ng (斯坦福大学), Ilya Sukskever (OpenAI)，William Yang Wang (加州大学圣芭芭拉分校), Jason Weston (Facebook人工智能研究院), Steve Young (剑桥大学) 等人的访谈，敬请期待。




**斯坦福大学计算机学院教授 Dan Jurafsky 是自然语言处理领域泰斗，他所著的《语音与语言处理》一书，被翻译成 60 多种语言，是全世界自然语言处理领域最经典的教科书**。Dan Jurafsky 曾在 ACL 2006，EMNLP 2013，WWW 2013 获最佳论文奖，在 2002 年获得麦克阿瑟天才奖（美国跨领域最高奖项），2017 年获得美国科学院 Cozzarelli 奖，2015 年获得古尔德奖。




Dan Jurafsky 教授在 Google Scholar 上引用量超过 3 万，h-index 达 75。他的主要研究方向有自然语言理解，对话系统，人与机器语言处理之间的关系等，并一直尝试运用自然语言处理方法来解决社会学和行为学问题。同时，他还对食物语言学以及中文有着极大的兴趣，他所著的科普图书《食物的语言—从语言学家角度读菜单》被翻译成多国语言，荣获 2015 国际畅销书榜首，并获 2015 年 James Beard Award 提名。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bQnWjhrq7lWmDTrcNJLHjdeBjiaS7WKgJj9XcsibickWy4V47YvzJU7ae3xCXNHpCfBuJG6ReGkkhX54Pm7Btzfug/640)

**▲**图1. 斯坦福大学计算机学院终身教授Dan Jurafsky早在80年代就与中国结下了不解之缘。图为他1985年在北京大学进修中文时的留影（第二排右二即是青年时代的Dan Jurafsky）。

© Jurafsky教授个人主页：https://web.stanford.edu/~jurafsky/




**香侬科技：**您现正在编辑《语音和语言处理》的第三版，这本书是自然语言处理（Natural Language Processing, NLP）领域使用最广泛的教科书，编辑的过程中，**您对过去几年自然语言处理领域的变化总体上有何体会？**最令人兴奋的事是什么？最令人失望的事又是什么（如果有的话）？




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bQnWjhrq7lWmDTrcNJLHjdeBjiaS7WKgJeJupGnElqS4erF9hHsTCWZibiafOmn7E5bX7Libq7qxwPjwQHvUNSjAsw/640)

** ▲**图2. Dan Jurafsky与James Martin所著的《语音和语言处理》一书，被翻译成60多种语言，是全世界自然语言处理领域最经典的教科书。




**Jurafsky：**能在这个时代身处这个领域是一件令人激动的事！当然，我会为深度学习感到特别的兴奋，而我觉得最值得期待的是自然语言生成方面将发生的巨大改变，这是一个很有潜力的领域，却在自然语言处理中被边缘化了太久的时间。




另外，嵌入，特别是基于上下文的嵌入（embeddings/contextualized embeddings）的使用也令人兴奋不已，它让我们得以构造模型来捕捉词义在不同时间、空间，语境中的动态变化。另外一件事是人们对 NLP 领域的社会性有了日渐提高的觉知：人们既意识到模型存在一定的偏见，也意识到这些模型可以用来模拟和理解人与人之间的互动，进而将这个世界变得更好。




**香侬科技：**许多 NLP 研究人员都有很强的语言学背景，甚至本身就来自该领域。然而，**随着深度学习的方法在 NLP 中变得越来越主导，有人说（这甚至可以算得上一种趋势）语言学知识不再是进行 NLP 研究的必要条件：只要训练一个双向长短时记忆循环神经网络（bidirectional LSTM RNN）就足够了。您能评价下这一说法吗？**另外，您如何评价 Frederick Jelinek 教授的名言“每次我解雇一个语言学家时，我的语音识别器的性能都会提高”？




**Jurafsky：**我坚定地相信，想要为一个知识领域做出贡献，充分了解这个领域是有帮助的，所以我认为 NLP 研究人员深刻地理解语言的功能，以及熟悉各类语言现象，比如：指代，组合性，变异，语法结构，隐含意义，情感，语言风格，对话互动等仍是至关重要的。




但理解语言和语言现象并不意味着盲目地套用不恰当的语言学模型。Jelinek 教授的那句话（他曾告诉我他的原话其实更加婉转：“每个语言学家离开团队时模型的识别率都会上升'”）实际上是指语音识别中的发音建模。事实证明（并且现在仍然正确）在拥有足够的数据时，机器学习能够比人工定义语音规则更好地解决语音多样性的问题。 




所以我认为这个领域未来仍将是机器学习与语言结构、知识的不断融合，而每个研究人员将在不同时间不同情况下决定如何分配这两个重要组成部分的权重。




**香侬科技：****从历史的角度来看，重大的突破通常首先在语音处理中发生，然后传播到自然语言处理领域**。例如：在 20 世纪 90 年代早期，来自语音领域的 Peter Brown 和 Robert Mercer 将统计机器学习模型引入 NLP 领域，从而彻底改变了该领域；而深度学习方法是首先在 2012 年被微软研究院科学家邓力等人应用在语音领域并取得突破性进展，而在 NLP 中大规模使用深度学习要到 2013-2014 年。回顾这些，您能解释为什么会发生这种情况吗，还是说它只是巧合？




**Jurafsky：**正如你所说，统计模型确实是从语音领域传播到 NLP，深度学习也是从语音和视觉领域传播到 NLP。我认为这根本不是巧合，一般来说创新来自于在不同领域工作的人一起工作时的结合。对诺贝尔奖获得者的研究表明，他们往往是“搭桥者”—将不同领域的方法联系在一起。因此，我对年轻学者的建议是多利用跨学科的联系，与相关但不同领域的人交谈。这就是重大突破诞生的方式。




**香侬科技：**您在博士后阶段做了 3 年的语音处理研究。您能描述一下这些年的研究是如何影响了您在 NLP 领域的研究生涯吗？




**Jurafsky：**它的影响是非常巨大的。我的博士后是在 1992-1995 年，正是机器学习、概率理论（probability theory）、图模型（graphical models）、神经网络（neural networks）以及早期版本的嵌入（embeddings）同时进入 NLP 的时期。我很幸运能够在加州大学伯克利分校国际计算机科学研究所（ICSI - UC Berkeley）的一个语音识别和神经网络实验室攻读博士后，并与 Nelson Morgan 和 Jerry Feldman 合作。那个实验室对我有着重要的意义，我的导师们对 NLP 领域的“大熔炉”观点对我产生了非常大的影响：你必须重视文本、语音、对话以及认知科学，给予它们和工程学一样多的思考。 




我们当时不知道什么会成为最主流的模型，是机器学习这个大领域，还是具体的图模型或神经网络。当时，因为没有足够多的 GPU，训练神经网络要慢得多，所以实验室必须搭建自己的向量处理器，而一个有着 4000 个单元的隐藏层的语音识别网络在当时是非常巨大的神经网络，要花极久的时间来训练。如果你当时让我预测，我不会预想到深度学习二十年之后会是今天这样的局面。有趣的是，我和 Martin 写的《语音和语言处理》教科书的第一版仅仅介绍了神经网络作为语音识别算法；在第二版，我们删除了神经网络，转而使用高斯模型，而在第三版中，我们又把神经网络加回来了！




**香侬科技：**在过去，您和您的学生使用 NLP 技术研究了许多社会科学中的重要问题（例如，Garg et al. PNAS 2018; Voigt et al. PNAS 2017, Winner of Cozzarelli Prize）。**您对于想要进行更多这样跨学科研究的 NLP 研究人员有哪些建议呢？**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/bQnWjhrq7lWmDTrcNJLHjdeBjiaS7WKgJ5zkaeibF40GZCNUWtJpB5icGgiargNRwCsiaFS3Y9ebuBMyibMjhELngClw/640)

**▲**图3. Voigt et al. PNAS 2017中Dan Jurafsky的实验室与斯坦福大学心理系合作，利用自然语言处理方法，自动评估警察对不同种族的人说话时的尊重程度。

© Voigt et al. PNAS 2017




**Jurafsky：**我们应该跟社会科学家多交流！我认为，如果你要研究与人类有关的任何东西，与社会科学专家合作这一点非常重要！社会科学专家不仅有着更多关于人和社会关系的思考，而且与计算机科学家相比，他们往往在统计和因果推理方面更有经验。再次强调，是跨学科引发了创新！




**香侬科技：**近年来，人们对机器学习的模型中的偏见有很多担忧。这个问题似乎在 NLP 领域格外突出，因为在自然环境（例如，twitter）中收集的数据不可避免地包含偏见（性别歧视，种族歧视等）。**盲目地用这些数据训练深度神经网络将导致有偏见的模型预测**。您怎么看待这一问题？




**Jurafsky：**是的，现在每天有数百万，甚至是数十亿人在使用 NLP 工具，如机器翻译，信息抽取，自动推荐等等，这是激动人心的进展。但是正像你所说，这些广泛的应用是有副作用的！NLP 的工作在道德层面上是影响社会的，越来越多的人，包括这个领域的年轻从业者以及我们科学和技术的消费者正在更多地关注这些影响。 




我很高兴我们终于开始正视这些问题！也许我们可以向那些长期以来必须面对这些道德困境和社会挑战的领域学习，比如：医药学，核物理学，生物学，社会科学等。你问我当需要权衡准确性与偏见时应该做些什么，我的答案是我们需要时时扪心自问：我们工作的终极目标是什么。我们现在意识到，这个终极目标绝不仅仅是为了提高准确性或速度，而是真正让世界变得更美好。这是一个模糊的答案，需要结合具体的算法或任务来实践，但是希望我们能够成功！




# 参考文献




[1]. Jurafsky D and Martin J H. Speech and Language Processing: An Introduction to Natural Language Processing, Computational Linguistics and Speech Recognition[M]. Second Edition. 2009, Prentice-Hall. 

[2]. Jurafsky, D. 2014. The Language of Food: A Linguist Reads the Menu[M]. Norton. 

[3]. Garg N et al. Word embeddings quantify 100 years of gender and ethnic stereotypes[J]. Proceedings of the National Academy of Sciences, 2018, 115(16): E3635-E3644. 

[4]. Voigt R et al. Language from police body camera footage shows racial disparities in officer respect[J]. Proceedings of the National Academy of Sciences, 2017, 114(25): 6521-6526.



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)



** 香侬招聘**




香侬科技 (http://shannon.ai/) ，是一家深耕金融领域的人工智能公司，旨在利用机器学习和人工智能算法提取、整合、分析海量金融信息，让 AI 为金融各领域赋能。




公司在 2017 年 12 月创立，获红杉中国基金独家数千万元融资。创始人之一李纪为是斯坦福大学计算机专业历史上第一位仅用三年时间就获得博士的人。过去三年 Google ScholarCitation>1,800，h-index达21。公司硕士以上比例为 100%，博士占比超 30%，成员皆来自斯坦福、MIT、CMU、Princeton、北京大学、清华大学、人民大学、南开大学等国内外知名学府。




**全职或实习加入香侬研发团队，请[点击查看香侬招聘贴](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488179&idx=2&sn=6342c9c9f23522a12c4120ef4f0ff1d9&chksm=96e9cd33a19e442589137eacd2aa347042844b3991b36bfe9a9eb122096a0252f4fd785de46c&scene=21#wechat_redirect)**

**简历投递邮箱：**hr@shannonai.com

**Tips：****听说在邮件标题中注明****“****PaperWeekly****”****，能大大提升面试邀约率**




# 英文采访稿




**ShannonAI: **As you are now editing the third version of the text book "Speech and Language Processing", the most widely used NLP text book, what is your general feeling about the changes in the NLP field over the past few years? What’s the most exciting part? Or if there is any, what’s the most disappointing part?




**Jurafsky: **This is such an exciting time to be in the field! I'm of course especially excited by deep learning. What makes me most enthusiastic is the potential for big changes to natural language generation, which is a field that has so much potential but was really marginalized in NLP for so long. And also the ability of modern embeddings (and contextualized embeddings!) to help us model the dynamics of word meaning (across time, across geography, across sentences and texts). The second thing I'm really excited about is the rise of social awareness in the field; both the realization that our models have biases, and their potential to be used to model and understand human social interaction and improve our world.




**ShannonAI: **Many of the NLP researchers have strong or even come from the linguistic background. As deep learning methods become more and more dominating, there is a voice or even a trend that linguistic knowledge is no longer a necessity in doing NLP research. Blindly training a bi-directional LSTM suffices. Could you comment on this point? Furthermore, could you comment on Frederick Jelinek's famous quote “every time I fire a linguist, the performance of the speech recognizer improves”?




**Jurafsky: **I'm a big believer that to make contributions to a field, a domain of knowledge, it helps to know something about the domain. So I think it will continue to be crucial for NLP folks to have a deep understanding of how language works, and familiarity with linguistic phenomena, things like reference, compositionality, variation, grammatical structure, connotation and affect, style, conversational interaction. But understanding language and linguistic phenomena doesn't mean blindly copying inappropriate linguistic models. The Jelinek quote (which Fred told me he phrased more diplomatically as ``Anytime a linguist leaves the group the recognition rate goes up'') was actually about pronunciation modeling in speech recognition; it turned out then, and it's still true now, that given sufficient data, machine learning is simply a better solution for modeling phonetic variation than hand-written phonetic rule cascades. 




So I think the field will continue to be what it has been: a beautiful integration of machine learning and linguistic structure and knowledge, and each individual researcher will weight these two important components in different amounts at different times.




**ShannonAI: **From a historical perspective, major breakthroughs usually took place first in speech processing, and then spread to NLP. For example: in the early 1990s Peter Brown and Robert Mercer were from the speech field. They introduced statistical machine learning models to the NLP field which revolutionized the NLP field; Deep learning methods brought breakthroughs first in the speech domain by Deng et al. from Microsoft in 2012, whereas the usage of deep learning at scale in NLP was as late as 2013-2014. Inretrospect, do you have a theory why this happens, or if it is just a coincidence?




**Jurafsky: **Yes, it's true both that statistical models came to NLP from speech, and that deep learning came to NLP from speech and also from vision. I believe that this is not a coincidence at all, that in general innovations come from interstices, when people who work on different areas are thrown together. Studies of Nobel Prize winners have shown that they tend to be bridge-builders, people who connect methods from different fields. So my advice to young scholars is to draw on interdisciplinary ties, talk to your neighbors in different areas. That's the way in which breakthroughs happen!




**ShannonAI:**You did 3 years of research on speech processing when you were doing your postdoc. Could you describe how these years of research influenced your NLP research career? 




**Jurafsky:**It was so influential. My postdoc was in 1992-1995, which is exactly when machine learning, probability theory and graphical models, neural networks, and early versions of embeddings were all simultaneously entering NLP, and I was lucky enough to get a postdoc at ICSI Berkeley in a speech recognition and neural network lab, working with Nelson Morgan and Jerry Feldman. That lab had a huge impact on me, and I was very influenced by my advisors in taking a "big tent" view of the NLP field, the idea that you had to think about text, speech, dialogue, and think about cognitive modeling and science just as much as engineering. 




We didn't know at the time whether it would be machine learning in general, or specifically graphical models or in fact neural networks that would become the dominant paradigm. And of course neural networks were so much slower then---no commercially available GPUs---so the lab had to build its own special vector co-processor boards, and what we thought of as enormous speech recognition nets---with one 4000-unit hidden layer--- still took forever to train. If you had asked me to guess, I don't think I would have predicted the 2-decade delayed arrival of deep learning. Amusingly, the first edition of the Jurafsky and Martin textbook described speech recognition only using neural networks; for the second edition we took nets out and described discriminative training of Gaussians instead; and now in the third edition we're putting the neural networks back!




**ShannonAI:**In the past you and your students have done a lot of work using NLP techniques to address important issues in social sciences (e.g., Garg et al. PNAS 2018; Voigt et al. PNAS 2017, Winner of Cozzarelli Prize). What are some recommendations you would like to give to NLP researchers if they want to do more interdisciplinary research like these?



**Jurafsky:**Talk to social scientists! I think it's so important if you're going to study anything related to humans to collaborate with experts on humans!! Not only are social scientists trained much more to think about people and social relations, but, compared to computer scientists, they often get far more training in statistical and causal inference. And, once again, its interdisciplinarity that leads to innovation!




**ShannonAI:**In recent years, there has been a lot of concerns about the biases in machine learning. This issue seems to be particularly potent in the field of NLP, since data collected in naturalistic settings (e.g., twitter) inevitably contains biases. Training deep neural networks to blindly learn from those data would result in biased model predictions. How should we address this problem? (Alternatively, you could answer a related question:  how can we use NLP for social good?)




**Jurafsky:**Yeah, this problem is a side-effect of a really exciting development, which is that NLP tools are finally being used by millions (billions?) of people every day, for MT, IR, recommendations, and soon! So the ethical aspects of our work finally have real consequences, and the young practitioners in the field, as well as the consumers of our science and technology, are really stepping up to look at these consequences. 




I'm excited that we're finally addressing these issues! Maybe we can learn from the tough ethical and social challenges that other fields---medicine, nuclear physics, biology, social science--- have long had to learn to wrestle with. You asked what we should do when accuracy trades off with bias, and I think the answer is that we need to always be asking ourselves what is the true higher goal of our work, and what we're realizing now is that it's not just about optimizing for accuracy or for speed, but for truly making the world a better place. That's a vague answer, and has to be contextualized for any individual algorithm or task, but let's hope we succeed!





**关于PaperWeekly**




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 加入社区刷论文




