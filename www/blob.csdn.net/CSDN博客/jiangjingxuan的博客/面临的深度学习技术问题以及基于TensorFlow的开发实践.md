# 面临的深度学习技术问题以及基于TensorFlow的开发实践 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 15:45:00[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2118








**一、用于Bot的深度学习技术及其面临的难题**




聊天机器人（chatbot），也被称为会话代理或对话系统，现已成为了一个热门话题。微软在聊天机器人上押上了重注，Facebook（M）、苹果（Siri）、谷歌、微信和 Slack 等公司也是如此。通过开发 Operator 或 x.ai 这样的消费者应用、 Chatfuel 这样的 bot 平台和 Howdy's Botkit
 这样的 bot 库，新一波创业者们正在尝试改变消费者与服务的交互方式。微软最近发布了他们自己的 bot 开发者框架。




许多公司希望开发出能进行无法与人类相区分的自然会话的 bot，并且许多公司声称要使用自然语言处理（NLP）和深度学习技术来使其成为可能。但是，在众多关于人工智能的大肆宣传中，有时很难将现实和幻想区分开。




在这个系列文章中，我想回顾一些被用来设计对话代理的深度学习技术，我会从解释我们现在所处的发展现状、什么是可能的和什么在短时间内几乎是不可能的开始。这篇文章作为导言，我们会在接下来的文章中谈论实现细节。




**模型分类**




基于检索式模型 vs 生成式模型




基于检索式模型（更简单）使用了预定义回复库和某种启发式方法来根据输入和语境做出合适的回复。这种启发式方法可以像基于规则的表达式匹配一样简单，也可以像机器学习分类器集一样复杂。这些系统不会产生任何新文本，他们只是从固定的集合中挑选一种回复而已。




生成式模型（更困难）不依赖于预定义回复库。他们从零开始生成新回复。生成式模型通常基于机器翻译技术，但区别于语言翻译，我们把一个输入「翻译」成一个输出「回复」。




![](http://read.html5.qq.com/image?src=forum&q=5&r=0&imgflag=7&imageUrl=http://mmbiz.qpic.cn/mmbiz/KmXPKA19gWicBWqf7QloDfd0P3jJHtZq63OMKNc30bjzJ5USqiaSj3HKIZUsOfyxmtjbic4ib6Xibj3wkv2wLI0BVYA/640?wx_fmt=png)




两种方式都有明显的优势和劣势。由于采用人工制作的回复库，基于检索式方法不会犯语法错误。然而它们可能无法处理没见过的情况，因为它们没有合适的预定义回复。同样，这些模型不能重新提到上下文中的实体信息，如先前对话中提到过的名字。生成式模型更「聪明」。它们可以重新提及输入中的实体并带给你一种正和人类对话的感觉。然而，这些模型很难训练，很可能会犯语法错误（特别是长句），而且通常要求大量的训练数据。





基于检索式模型或生成式模型都可以应用深度学习技术，但是相关研究似乎正转向生成式方向。像序列到序列（Sequence to Sequence）这样的深度学习架构是唯一可以适用于产生文本的，并且研究者希望在这个领域取得快速进步。然而，我们仍处于构建工作良好的生成式模型的早期阶段。现在的生产系统更可能是基于检索式的。




长对话 vs 短对话




对话越长，就越难使它自动化。一方面，短文本对话（更简单）的目标是单独回复一个简单的输入。例如，你可能收到一个用户的特定问题并回复合适的答案。而长对话（更困难）要求你经历多个转折并需要记录说过什么话。客户支持类对话通常是包含多个问题的长对话流。




开域(open domain) vs 闭域(closeddomain)




在开域（更困难）环境中，用户可以进行任何对话。不需要明确定义的目标或意图。像Twitter 和 Reddit 这种社交媒体网站上的对话通常是开域的——它们可以是任何主题。话题的无限数量和用于产生合理回复的一定量的知识使开域成为了一个艰难的问题。




在闭域（更简单）设定中，因为系统试图达成一个非常明确的目标，可能输入和输出的空间会有所限制。例如客户技术支持或购物助手就属于闭域的范畴。这些系统不需要能谈论政治，它们只需要尽可能高效地完成它们特定的任务。当然，用户仍然可以进行任何他们想要的对话，但是这样的系统不需要能处理所有情况，并且用户也不期望它能处理。




**普遍难题**




在构建大部分属于活跃研究领域的会话代理方面存在着许多明显和不明显的难题。




1. 整合语境




为了生成明智的回复，系统可能需要整合语言语境（linguistic context）和物理语境（physical context）。在长对话中，人们记录已经被说过的话和已经交换过的信息。这是结合语言语境的例子。最普遍的方法是将对话嵌入一个向量中，但在长对话上进行这样的操作是很有挑战性的。「使用生成式分层神经网络模型构建端到端对话系统」和「神经网络对话模型的注意与意图」两个实验中都选择了这个研究方向。此外还可能需要整合其它类型的语境数据，例如日期/时间、位置或用户信息。




2. 一致人格




当生成回复时，对于语义相同的输入，代理应该生成相同的回答。例如，你想在「你多大了？」和「你的年龄是多少？」上得到同样的回答。这听起来很简单，但是将固定的知识或者「人格」整合进模型是非常困难的研究难题。许多系统学习如何生成语义合理的回复，但是它们没有被训练如何生成语义上一致的回复。这一般是因为它们是基于多个不同用户的数据训练的。「基于个人的神经对话模型」这样的模型是明确的对人格建模的方向上的第一步。




![](http://read.html5.qq.com/image?src=forum&q=5&r=0&imgflag=7&imageUrl=http://mmbiz.qpic.cn/mmbiz/KmXPKA19gWicBWqf7QloDfd0P3jJHtZq64eiaGvmkNJKbOf9Q6yz5OOxjO27plLB5ibEdQtXFqo9XU603hQ5IrBibw/640?wx_fmt=png)




3. 模型评估





评估一个对话代理的理想方式是衡量它是否完成了它的任务，例如，在给定对话中解决客户支持问题。但是获取这样的标签成本高昂，因为它们要求人类的判断和评估。某些时候并不存在明确定义的目标，比如开域模型中的情况。通常像 BLEU 这样被用于机器翻译且是基于文本匹配的标准并不能胜任，因为智能的回复可以包括完全不同的单词或短语。实际上，在
*How NOT To Evaluate Your Dialogue System: An Empirical Study of UnsupervisedEvaluation Metrics for Dialogue Response Generation*中，研究者发现没有一个通用的度量能真正与人类判断一一对应。




4. 意图和多样性




生成式系统的普遍问题是它们往往能生成像「太好了！」或「我不知道」这样的能适用于许多输入情况的普遍回复。谷歌的智能回复（Smart Reply ）早期版本常常用「我爱你」回复一切。一定程度上这是系统根据数据和实际训练目标/算法训练的结果。然而，人类通常使用针对输入的回复并带有意图。因为生成系统（特别是开域系统）是不被训练成有特定意图的，所以它们缺乏这种多样性。




**实际工作情况**




纵观现在所有最前沿的研究，我们发展到哪里了？这些系统的实际工作情况如何？让我们再看看我们的分类法。一个基于检索式开域系统显然是不可能实现的，因为你不能人工制作出足够的回复来覆盖所有情况。生成式开域系统几乎是人工通用智能（AGI: Artificial General Intelligence），因为它需要处理所有可能的场景。我们离
 AGI 还非常遥远（但是这个领域有许多研究正在进行）。



这就让我们的问题进入了生成式和基于检索式方法都适用的受限的领域。对话越长，语境就越重要，问题就变得越困难。




在最近的采访中，现任百度首席科学家吴恩达说得很好：




> 
当今深度学习的价值在你可以获得许多数据的狭窄领域内。有一件事它做不到：进行有意义的对话。存在一些演示，并且如果你仔细挑选这些对话，看起来就像它正在进行有意义的对话，但是如果你亲自尝试，它就会快速偏离轨道。





许多公司从外包他们的对话业务给人类工作者开始，并承诺一旦他们收集到了足够的数据，他们就会使其「自动化」。只有当他们在一个相当狭窄的领域中这样操作时，这才有可能发生——比如呼叫 Uber 的聊天界面。任何稍微多点开域的事（像销售邮件）就超出了我们现在的能力范围。然而，我们也可以利用这些系统建议和改正回复来辅助人类工作者。这就更符合实际了。




生产系统的语法错误成本很高并会赶走用户。所以，大多数系统可能最好还是使用不会有语法错误和不礼貌回答的基于检索式方法。如果公司能想办法得到大量的数据，那么生成式模型就将是可行的——但是，必须需要其它技术的辅助来防止它们像微软的 Tay 一样脱轨。




**总结和推荐阅读**




背靠科技巨头的资源或创业公司的热情的聊天机器人们正在努力挤进我们生活的方方面面。在一般的话题上，它们还远不能实现如人类水平的沟通和语境理解能力；但在购物助手等狭窄领域的的应用中，训练有素的聊天机器人完全可以取代人类来协助和提升消费者的购物体验。如果你对相关研究感兴趣，那么，下列论文可以让你开个好头：



- 
Neural Responding Machine for Short-Text Conversation (2015-03)

- 
A Neural Conversational Model (2015-06)

- 
A Neural Network Approach to Context-Sensitive Generation of Conversational Responses (2015-06)

- 
The Ubuntu Dialogue Corpus: A Large Dataset for Research in Unstructured Multi-Turn Dialogue Systems (2015-06)

- 
Building End-To-End Dialogue Systems Using Generative Hierarchical Neural Network Models (2015-07)

- 
A Diversity-Promoting Objective Function for Neural Conversation Models (2015-10)

- 
Attention with Intention for a Neural Network Conversation Model (2015-10)

- 
Improved Deep Learning Baselines for Ubuntu Corpus Dialogs (2015-10)

- 
A Survey of Available Corpora for Building Data-Driven Dialogue Systems (2015-12)

- 
Incorporating Copying Mechanism in Sequence-to-Sequence Learning (2016-03)

- 
A Persona-Based Neural Conversation Model (2016-03)

- 
How NOT To Evaluate Your Dialogue System: An Empirical Study of Unsupervised     Evaluation Metrics for Dialogue Response Generation (2016-03)




**二、如何在TensorFlow上部署一个基于检索的Bot模型**




我们将在这篇文章中探讨基于检索的 Bot 的实现。基于检索的模型使用了一个预定义响应库（repository of pre-defined responses），这一点不同于生成模型——它们会生成它们从未见过的响应。将一个语境（截至于该点的对话）和一个潜在的响应输入检索模型后，模型会输出这个响应的得分。为了找到好的响应，我们可以计算多个响应的得分，然后选择其中得分最高的响应。




但是如果你能构建生成模型，那为什么又要建立一个检索模型呢？生成模型看上去更灵活因为它们不需要这种预定义的响应库，对吗？




问题是，实践中的检索模型表现得并不好。至少现在还不够好。因为它们在响应方式上的自由度太大了。生成模型往往会产生语法错误，而且会生成不相关的、通用或者不一致的响应。它们还需要巨量的训练数据，而且难以优化。当下绝大部分的产品系统都是以检索为基础的，或者是基于检索和生成方法的结合。谷歌的 Smart Replay 是很好的例子。生成模型的研究领域很活跃，但我们还没达到实用的程度。今天如果你想构建一个聊天代理，最好还是选择基于检索的模型。




**Ubuntu 对话语料库**




本文中，我们使用了 Ubuntu 对话预料库（Ubuntu Dialog Corpus，UDC）(https://github.com/rkadlec/ubuntu-ranking-dataset-creator)，UDC 是最大的公共对话数据库之一。它以一个公共 IRC 网络上的 Ubuntu 频道为基础 。有篇论文详细描述了创造这个语料库的过程，所以在此不再赘述。但是，理解我们使用的数据的类型是非常重要的，这里先做一些探索。




这个训练数据由 1,000,000 个样本构成，其中 50% 是积极的 （标签为 1），50% 是消极的（标签为 0）。每个样本都由一个语境（context）（也就是截至于该点的对话）、一个话语（utterance）和一个语境的响应（response）构成。积极标签表示这个话语是对这个语境的真实响应，消极标签代表的则不是真实的响应——而是从语料库中随机抽取的。下面是一些样本数据。




![](http://read.html5.qq.com/image?src=forum&q=5&r=0&imgflag=7&imageUrl=http://mmbiz.qpic.cn/mmbiz/KmXPKA19gWibJV89c0PibXwkdM2DJqbn2r0kiahoFmUPr4zL883uIrOfuvPotHzfDe0uYibX1pXQQibMH5vVzYwZWkQ/0?wx_fmt=png)





注意，数据库生成脚本已经为我们做好了一堆预处理——使用 NLTK 工具给输出内容打上代号封存起来，然后把输出的内容按照异体形式进行归类（lemmatize）。这个脚本也代替了名称、位置、组织、URL 和带有特殊代号的系统路径等实体。这个预处理过程不是完全必要的，但是它可能能将整个性能表现提升几个百分点。每个语境平均有 86 个单词，而每个话语平均包含 17
 个单词。




这个数据集带有测试和验证集，它们的格式不同于训练数据的格式。测试/验证集中的每一个记录都由一个语境、一个真实话语（真实响应）和 9 个称为干扰项（ distractor）的不正确话语构成。这个模型的目标会将最高分分配给这个真实的话语，给错误的话语分配较低分数。




![](http://read.html5.qq.com/image?src=forum&q=5&r=0&imgflag=7&imageUrl=http://mmbiz.qpic.cn/mmbiz/KmXPKA19gWibJV89c0PibXwkdM2DJqbn2r4DSmlEHKevExmPTuiaFnJB351QGibPj6RoQON2Gx27WibOfMEjrmyu51g/0?wx_fmt=png)





评估模型有很多种方法。常用的标准是 recall@k。recall@k 意思是我们让这个模型从 10 个可能的响应中挑出 k 个最好的（1 个真实和 9 个干扰项）响应。如果这个正确的响应在这些选择出的响应中，我们就将该测试样本标记为正确。所以，一个更大的 k 意味着这个任务会更简单。如果我们设 k = 10， 那么我们 100% 需要重新调用，因为我们总共只有
 10 个可选的响应。如果我们设 k=1， 那么这个模型挑出正确的回应的机会只有 1 次。




在这里你也许疑问这 9 个干扰项是怎么选出来的。这个数据集中的 9 个干扰项是随机选出来的。但是，在真实世界，你也许有几百万个可能的响应，而且你不知道哪一个是正确的。你不可能能够评估一百万个潜在的响应，然后选出得分最高的一个——这个代价太大了。谷歌的 Smart Replay 使用聚类技术得出一组可能的响应来一个一个挑选。或者，如果你总共只有一百个潜在的响应，你也可以全部评估它们。




**基线**




在开始使用神奇的神经网络模型之前，我们先建立一些简单的基线模型（baseline model）来帮助理解我们预计会得到什么类型的性能。我们会使用下面的函数来估算我们的 recall@k 标准：




> 
***def evaluate_recall(y, y_test, k=1):***

*** num_examples = float(len(y))***

***    num_correct = 0***

*** for predictions, label in zip(y, y_test):***

***        if label in predictions[:k]:***

*** num_correct += 1***

***    return num_correct/num_examples***





其中，y 是我们按得分递减顺序列出的列表，y_test 是真实标签。例如，一个 [0,3,1,2,5,6,4,7,8,9] 构成的 y 中可能是 0 号话语得分最高、9号话语得分最低。记住每一个测试样本有 10 个话语，第一个（索引0）总是正确的那一个，因为这个话语列产生在另外 9 个干扰项之前。




直观来看，一个完全随机的预测得分应该是：recall@1 为 10%、recall@2 为20%，等等。我们来看看是不是这种情况。




> 
***# Random Predictor***

***def predict_random(context, utterances):***

***    return np.random.choice(len(utterances), 10, replace=False)***



***# Evaluate Random predictor***

***y_random = [predict_random(test_df.Context[x], test_df.iloc[x,1:].values) for x in range(len(test_df))]***

***y_test = np.zeros(len(y_random))***

***for n in [1, 2, 5, 10]:***

***    print("Recall @ ({}, 10): {:g}".format(n, evaluate_recall(y_random, y_test, n)))***



***Recall @ (1, 10): 0.0937632***

***Recall @ (2, 10): 0.194503***

***Recall @ (5, 10): 0.49297***

***Recall @ (10, 10): 1***





很棒，看起来有用。我们当然不仅仅想要一个随机预测。最初的那篇论文中讨论的另一个基线是 tf-idf 预测。 tf-idf 意思是「术语频率-逆文档频率（term frequency – inverse document frequency）」，它测量文档中的一个单词与整个语料相关的重要性。这里不过多深入细节（网上有许多关于 tf-idf 的教程），具有相似内容的文档将会相似的
 tf-idf 向量。直观上看，如果一个语境和一个响应有相似的单词，它们就更可能是正确的一对。至少比随机出来的一对更有可能。现有的许多数据库（如 scikit-learn）都内置了 tf-idf 函数，所以非常容易使用。我们建一个 tf-idf 看看它表现如何。




> 
***class TFIDFPredictor:***

*** def __init__(self):***

***        self.vectorizer = TfidfVectorizer()***



***    def train(self, data):***

*** self.vectorizer.fit(np.append(data.Context.values,data.Utterance.values))***




*** def predict(self, context, utterances):***

***        # Convert context and utterances into tfidf vector***

*** vector_context = self.vectorizer.transform([context])***

***        vector_doc = self.vectorizer.transform(utterances)***

*** # The dot product measures the similarity of the resulting vectors***

***        result = np.dot(vector_doc, vector_context.T).todense()***

*** result = np.asarray(result).flatten()***

***        # Sort by top results and return the indices in descending order***

*** return np.argsort(result, axis=0)[::-1]***




***# Evaluate TFIDF predictor***

***pred = TFIDFPredictor()***

***pred.train(train_df)***

***y = [pred.predict(test_df.Context[x], test_df.iloc[x,1:].values) for x in range(len(test_df))]***

***for n in [1, 2, 5, 10]:***

*** print("Recall @ ({}, 10): {:g}".format(n, evaluate_recall(y, y_test, n)))***




***Recall @ (1, 10): 0.495032***

***Recall @ (2, 10): 0.596882***

***Recall @ (5, 10): 0.766121***

***Recall @ (10, 10): 1***





我们能看到  tf-idf 模型表现十分出色，远比随机模型要好。不过还远不够完美，我们也没有那么好的假设。首先，一个正确的响应没必要和语境相似。第二， tf-idf 忽略了单词顺序，而这可能是重要的信号。有了神经网络模型我们能做得更好。




**双重编码器 LSTM**




本文中我们将要创建一个叫双重编码器  LSTM（Dual Encoder LSTM）网络的深度学习模型。这种网络只是我们能应用在这个问题上的网络类型之一，而且它不一定是最好的一个。你可以试试所有还没被试过的深度学习架构——这个领域的研究非常活跃。例如，常用在机器翻译中的 seq2seq 模型也可能可以很好地完成这个任务。我们选择这个双重编码器的原因是它已经被多次报道过在这个数据集上的表现非常好。这就意味着我们知道预计能得到什么，也能明确我们的实施是正确的。应用其他模型解决这个问题也会非常有趣。




我们将要创建的双重编码器  LSTM 是这样的（出自论文：The Ubuntu Dialogue Corpus: A Large Dataset for Research in Unstructured Multi-Turn Dialogue Systems）：

![](http://read.html5.qq.com/image?src=forum&q=5&r=0&imgflag=7&imageUrl=http://mmbiz.qpic.cn/mmbiz/KmXPKA19gWibJV89c0PibXwkdM2DJqbn2rOpDsj5YvUsLGIE4iaFO4YCpI7yQnGeMwPic9FG2t0z5LcCsByJcFsksQ/0?wx_fmt=png)
它大概做以下工作：




1. 会被单词分裂，而且每个单词都嵌入到一个向量中。词嵌入（word embeddings ）由斯坦福的 GloVe 向量初始化，然后在训练中进行精细的调整（边注：这是可选的，且未在图中展示。我发现用 GloVe 初始化词向量不会在模型性能上有很大区别。）




2. 嵌入的语境和响应被一个单词一个单词地馈送到同一个循环神经网络。这个循环神经网络生成一个不太严格的向量表征，该表征获取了语境及其响应（图中的 c 和 r ）的「意义」。我们可以选择这些向量的大小，而我们选择的尺寸是 256 维。




3. 我们用 c 乘以矩阵 M 来「预测」响应 r'。如果 c 是一个 256  维的向量，那么 M 就是一个 256×256 维的矩阵。结果是另一个 256 维的向量，我们可以将它解释为一个生成的响应。矩阵 M 是在训练中被学习到的。




4. 我们通过计算这个两个向量的点积测量预测响应 r' 与真实响应 r 的相似度。大点积表示这些向量是相似的，则该响应应该得到高分。之后我们应用一个 sigmoid 函数把这个分数转换成概率。注意第 3 步和第 4 步被融合在一个图中。




训练这个网络，我们还需要一个损失（成本）函数。我们使用的是分类问题中常用的二元交叉熵损失（ binary cross-entropy loss）函数。让我们将用于语境-响应对的真实标签称为 y，其要么是 1（实际响应），要么是 0（不正确响应）。让我们将上面第 4 步中的预测概率称为 y'. 那么，该交叉熵损失可通过 L= −y * ln(y') − (1
 − y) * ln(1−y) 计算得到。这个公式之后的直观知识很简单。如果 y=1，则只剩下 L = -ln(y')，会使预测处于远离 1 的不利位置；而如果 y=0，则只剩下  L= −ln(1−y)，会使预测处于远离  0 的不利位置.




在实施中，我们会结合使用 numpy 、pandas Tensorflow 和 TF Learn（ Tensorflow 高层便利函数（ high-level convenience functions ）的综合体）。




**数据预处理**




数据库最初以 CSV 的格式出现。我们能直接使用 CSV ，但更好的方法是将我们的数据转换成 Tensorflow 的优先的 Example 格式 。（边注：也有 tf.SequenceExample）但是它看上去还并不被 tf.learn 支持。）这种格式的主要好处是它允许我们直接从输入文件中加载张量（tensor），并且让 Tensorflow 处理输入所有的混排（shuffling）、分批（batching）和列队（queuing）。




每一个 Example 都包含以下域（field）：



- 
context：一个表示语境文本的单词标识序列，比如 [231, 2190, 737, 0, 912]

- 
context_len: 语境长度，比如以上例子为 5

- 
utterance：一个表示话语（响应）的单词标识序列

- 
utterance_len:话语的长度

- 
label： 只在训练数据中。0 或 1。

- 
distractor_[N]: 只在测试/验证数据中。N 的范围从 0 到 8。一个表示干扰项话语的单词标识序列。

- 
distractor_[N]_len :只在测试/验证数据中。N 的范围从 0 到 8。话语的长度。





由 Python 脚本 prepare_data.py完成这个预处理，它会生成 3 个文件： train.tfrecords、validation.tfrecords 和 test.tfrecords.你可以自己运行这个脚本，或者在 https://drive.google.com/open?id=0B_bZck-ksdkpVEtVc1R6Y01HMWM
 下载数据。




**创建一个输入函数**




为了使用 Tensorflow 中对训练和评估的内置支持，我们需要创建一个输入函数——该函数能返回输入数据的批次。事实上，因为我们的训练和测试数据有不同的格式，我们需要为它们提供不同输入函数。输入函数应该返回一批特征和标签（如果可以的话）。用代码表示为：




> 
***def input_fn():***

*** # TODO Load and preprocess data here***

***  return batched_features, labels***





因为在训练和评估过程中我们需要不同的输入函数，而且因为我们厌恶代码重复，所以我们创造了一个名叫 create_input_fn 包装函数，其能创建合适模式的输入函数。它也需要一些其它参数。我们使用的定义如下：




> 
***def create_input_fn(mode, input_files, batch_size, num_epochs=None):***

*** def input_fn():***

***    # TODO Load and preprocess data here***

*** return batched_features, labels***

***  return input_fn***





完整代码可见于文件 udc_inputs.py。在一个较高的层面上，该函数实现了以下功能：



- 
在我们的 Example 文件中创造了一个描述域（field）的特征定义

- 
使用 tf.TFRecordReader 从 input_files 中读取记录

- 
根据特征定义解析记录

- 
提取训练标签

- 
批处理多个样本和训练标签

- 
返回批处理过的样本的训练标签





**定义评估标准**




我们已经提到了我们想使用 recall@k 标准来评估我们的模型。幸运的是，TensorFlow 已经具备了许多我们可以使用的评估标准，其中就包括 recall@k。为了使用这些标准，我们需要创造一个能够将标准名（metric name）映射到做出预测的函数和作为参数的标签的词典。




> 
***def create_evaluation_metrics():***

*** eval_metrics = {}***

***  for k in [1, 2, 5, 10]:***

*** eval_metrics["recall_at_%d" % k] = functools.partial(***

***        tf.contrib.metrics.streaming_sparse_recall_at_k,***

*** k=k)***

***  return eval_metrics***





上面，我们使用了 functools.partial 将带有三个参数的函数转换成只带有 2 个参数的函数。不要对 streaming_sparse_recall_at_k 的名字感到困惑。其中的「streaming（流）」只是意味着该标准是在多批次处理上的积累，而「sparse（稀疏）」则是指我们的标签的格式。




这就将我们带到了一个非常重要的点上：评估过程中我们的预测的格式究竟是什么？在训练过程中，我们预测了样本正确的概率。但在评估过程中，我们的目标是对表达和 9 个干扰项进行评分，然后选出最好的一个——我们并不是简单地预测正确或不正确。这意味着：在评估过程中，每个样本都会得到一个包含 10 个分数的向量，如[0.34, 0.11, 0.22, 0.45, 0.01,
 0.02, 0.03, 0.08, 0.33, 0.11]，其中这些分数分别对应于真实回应和 9 个干扰项。其中对每一个表达的评分都是独立的，所以这些概率之和不需要等于 1。 因为真实回应总是数组中的元素 0，所以每个样本的标签是 0。上面的样本可以根据 recall@1 而被不正确地分类，因为第 3 个干扰项获得了 0.45 的概率，而真实回应仅有 0.34。 但其会被 recall@2 评分为正确。




**样板文件训练代码**




在编写实际的神经网络代码之前，我喜欢先编写训练以及评估模型的样板文件代码。这是因为，一旦你遵守正确的接口，就非常容易置换出使用的网络的类型。假设我们有一个模型函数 model_fn，把我们的批量特征、标记和模式（训练或者评估），然后返回预测。接下来，我们就能像下面这样编写训练模型的通用代码：




> 
***estimator = tf.contrib.learn.Estimator(***

***model_fn=model_fn,***

***model_dir=MODEL_DIR,***

***config=tf.contrib.learn.RunConfig())***




***input_fn_train = udc_inputs.create_input_fn(***

***mode=tf.contrib.learn.ModeKeys.TRAIN,***

***input_files=[TRAIN_FILE],***

***batch_size=hparams.batch_size)***




***input_fn_eval = udc_inputs.create_input_fn(***

***mode=tf.contrib.learn.ModeKeys.EVAL,***

***input_files=[VALIDATION_FILE],***

***batch_size=hparams.eval_batch_size,***

***num_epochs=1)***



***eval_metrics = udc_metrics.create_evaluation_metrics()***




***# We need to subclass theis manually for now. The next TF version will***

***# have support ValidationMonitors with metrics built-in.***

***# It's already on the master branch.***

***class EvaluationMonitor(tf.contrib.learn.monitors.EveryN):***

***def every_n_step_end(self, step, outputs):***

*** self._estimator.evaluate(***

***    input_fn=input_fn_eval,***

*** metrics=eval_metrics,***

***    steps=None)***




***eval_monitor = EvaluationMonitor(every_n_steps=FLAGS.eval_every)***

***estimator.fit(input_fn=input_fn_train, steps=None, monitors=[eval_monitor])***





在这里，我们为 model_fn 创造了一个评估器，两个训练以及评估数据的输入函数，以及我们的评估参数资料。我们也定义了一个监控器，能在训练过程中评估模型的每一个 FLAGS.eval_every 步骤。最终，我们训练了这个模型。训练的运行是无限期的，但 Tensorflow 自动保存了检查点文件到 MODEL_DIR，所以你能随时停止训练。一个更精致的技术是使用早停止，这意味着你可以在一个验证集参数停止改进（即，开始过拟合）时自动停止训练。你可以在
 udc_train.py（https://github.com/dennybritz/chatbot-retrieval/blob/master/udc_train.py）看到完整代码。




我想提及的两件事是 FLAGS 的使用。这是一种将命令行参数给予到编程的方式（类似于 Python 的 argparse）。 hparams 是我们在 hparams.py 创造的一个自定义对象，持有模型中我们能调整的超参数、nobs。这一 hparams 对象在我们具现化的时候会被交给模型。




**建立模型**




如今，我们已经建立了输入、解析、评估和训练的样本文件代码，是时候为双重 LSTM 神经网络编写代码了。因为我们有不同格式的训练和评估数据，我写了一个 create_model_fn 包装器，它可以为我们带来正确格式的数据。这个包装器采用  amodel_impl 参数（argument），这是一个真正可以做预测的函数。在我们的案例中，我们上面描述的是 双重编码器
 LSTM，但我们能轻松的将其替换为其他神经网络。让我们看一下它是什么样的：




> 
***def dual_encoder_model(***

*** hparams,***

***    mode,***

*** context,***

***    context_len,***

*** utterance,***

***    utterance_len,***

*** targets):***




*** # Initialize embedidngs randomly or with pre-trained vectors if available***

***  embeddings_W = get_embeddings(hparams)***

***  # Embed the context and the utterance***

*** context_embedded = tf.nn.embedding_lookup(***

***      embeddings_W, context, name="embed_context")***

*** utterance_embedded = tf.nn.embedding_lookup(***

***      embeddings_W, utterance, name="embed_utterance")***




*** # Build the RNN***

***  with tf.variable_scope("rnn") as vs:***

*** # We use an LSTM Cell***

***    cell = tf.nn.rnn_cell.LSTMCell(***

*** hparams.rnn_dim,***

***        forget_bias=2.0,***

*** use_peepholes=True,***

***        state_is_tuple=True)***



***    # Run the utterance and context through the RNN***

*** rnn_outputs, rnn_states = tf.nn.dynamic_rnn(***

***        cell,***

*** tf.concat(0, [context_embedded, utterance_embedded]),***

***        sequence_length=tf.concat(0, [context_len, utterance_len]),***

*** dtype=tf.float32)***

***    encoding_context, encoding_utterance = tf.split(0, 2, rnn_states.h)***




***  with tf.variable_scope("prediction") as vs:***

*** M = tf.get_variable("M",***

***      shape=[hparams.rnn_dim, hparams.rnn_dim],***

*** initializer=tf.truncated_normal_initializer())***




*** # "Predict" a response: c * M***

***    generated_response = tf.matmul(encoding_context, M)***

*** generated_response = tf.expand_dims(generated_response, 2)***

***    encoding_utterance = tf.expand_dims(encoding_utterance, 2)***




***    # Dot product between generated response and actual response***

*** # (c * M) * r***

***    logits = tf.batch_matmul(generated_response, encoding_utterance, True)***

*** logits = tf.squeeze(logits, [2])***




*** # Apply sigmoid to convert logits to probabilities***

***    probs = tf.sigmoid(logits)***




***    # Calculate the binary cross-entropy loss***

*** losses = tf.nn.sigmoid_cross_entropy_with_logits(logits, tf.to_float(targets))***




*** # Mean loss across the batch of examples***

***  mean_loss = tf.reduce_mean(losses, name="mean_loss")***

*** return probs, mean_loss***





全部的代码在这里 dual_encoder.py：https://github.com/dennybritz/chatbot-retrieval/blob/master/models/dual_encoder.py。在这种情况下，现在我们可以具现化我们早就定义好的模型功能，这是  dual_encoder.py 里面的主程序。




> 
***model_fn = udc_model.create_model_fn(***

*** hparams=hparams,***

***  model_impl=dual_encoder_model)***





是这样，我们现在可以运行 python udc_train.py，然后开始训练我们的神经网络，偶尔评估验证数据上的查全（你可以选择想要多长时间使用--eval_every switch 进行一次评估。）为了得到一份我们定义的使用 tf.flags 和 hparams 关于所有可用的命令行标记的完整列表，你可以运行 python udc_train.py
 --help。




> 
***INFO:tensorflow:training step 20200, loss = 0.36895 (0.330 sec/batch).***

***INFO:tensorflow:Step 20201: mean_loss:0 = 0.385877***

***INFO:tensorflow:training step 20300, loss = 0.25251 (0.338 sec/batch).***

***INFO:tensorflow:Step 20301: mean_loss:0 = 0.405653***

***...***

***INFO:tensorflow:Results after 270 steps (0.248 sec/batch): recall_at_1 = 0.507581018519, recall_at_2 = 0.689699074074, recall_at_5 = 0.913020833333, recall_at_10 = 1.0, loss = 0.5383***

***...***





**模型评估**




在训练完模型之后，你可以在测试设定上使用 python udc_test.py —model_dir=$MODEL_DIR_FROM_TRAINING 对其进行评估，例如，python udc_test.py —model_dir=~/github/chatbot-retrieval/runs/1467389151。这将会在测试设定上运行 recall@k
 评估参数，而非在验证设定上运行。注意，在训练的过程中，你必须调用带有同样参数的udc_test.py。所以，如果你使用 --embedding_size=128 训练，你需要调用同样的测试脚本。




大约训练了 20,000 步之后（在快速 GPU 上运行了大约 1 个小时），我们的模型在测试设定上获得了如下结果：




> 
***recall_at_1 = 0.507581018519***

***recall_at_2 = 0.689699074074***

***recall_at_5 = 0.913020833333***





尽管 recall@1 的结果接近我们的 TFIDF 模型， recall@2 和 recall@5 的结果更加的好，表明我们的神经网络会对正确的答案分配更高的分数。最初的论文报告中，三者的得分分别是 0.55、0.72、0.92，但我无法复制出同样高的分数。可能其他的数据预处理，或者超参数优化很大的限制了得分。




**做预测**




你可以修改以及运行 udc_predict.py (https://github.com/dennybritz/chatbot-retrieval/blob/master/udc_predict.py) 获得未知数据的概率得分。例如 python udc_predict.py --model_dir=./runs/1467576365/ 输出：




> 
***Context: Example context***

***Response 1: 0.44806***

***Response 2: 0.481638***





可以想象，输入 100 个对话环境中可能的回答，然后挑选出得分最高的一个。




**结论**




在此博文中，我们已经实施了一个基于检索的神经网络模型，它能够为给定对话环境下的潜在回答打分。然而，里面还有很大的提升空间。想象一下，在此任务上，其他的神经网络的表现可能要比双 LSTM 编码器好。在超参数优化，以及预处理步骤的改进上都有很大的提升空间。






