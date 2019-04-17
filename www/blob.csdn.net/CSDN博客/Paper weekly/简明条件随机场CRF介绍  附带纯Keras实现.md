# 简明条件随机场CRF介绍 | 附带纯Keras实现 - Paper weekly - CSDN博客





2018年05月22日 11:59:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：603














作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




笔者去年曾写过文章《果壳中的条件随机场（CRF In A Nutshell）》[1]，以一种比较粗糙的方式介绍了一下条件随机场（CRF）模型。然而那篇文章显然有很多不足的地方，比如介绍不够清晰，也不够完整，还没有实现，在这里我们重提这个模型，将相关内容补充完成。 




**本文是对 CRF 基本原理的一个简明的介绍**。当然，“简明”是相对而言中，要想真的弄清楚 CRF，免不了要提及一些公式，如果只关心调用的读者，可以直接移到文末。




# 图示




按照之前的思路，我们依旧来对比一下普通的逐帧 softmax 和 CRF 的异同。 




**逐帧softmax**




CRF 主要用于序列标注问题，可以简单理解为是**给序列中的每一帧都进行分类**，既然是分类，很自然想到将这个序列用 CNN 或者 RNN 进行编码后，接一个全连接层用 softmax 激活，如下图所示：






**▲**逐帧softmax并没有直接考虑输出的上下文关联




**条件随机场**




然而，当我们设计标签时，比如用 s、b、m、e 的 4 个标签来做字标注法的分词，目标输出序列本身会带有一些上下文关联，比如 s 后面就不能接 m 和 e，等等。逐标签 softmax 并没有考虑这种输出层面的上下文关联，所以它意味着把这些关联放到了编码层面，希望模型能自己学到这些内容，但有时候会“强模型所难”。 




而 CRF 则更直接一点，它**将输出层面的关联分离了出来**，这使得模型在学习上更为“从容”：






**▲**CRF在输出端显式地考虑了上下文关联

# 数学




当然，如果仅仅是引入输出的关联，还不仅仅是 CRF 的全部，CRF 的真正精巧的地方，是它以路径为单位，考虑的是路径的概率。 




**模型概要**




假如一个输入有 n 帧，每一帧的标签有 k 中可能性，那么理论上就有k^n中不同的输入。我们可以将它用如下的网络图进行简单的可视化。在下图中，每个点代表一个标签的可能性，点之间的连线表示标签之间的关联，而每一种标注结果，都对应着图上的一条完整的路径。






**▲**4tag分词模型中输出网络图




而在序列标注任务中，我们的正确答案是一般是唯一的。比如“今天天气不错”，如果对应的分词结果是“今天/天气/不/错”，那么目标输出序列就是 bebess，除此之外别的路径都不符合要求。




换言之，在序列标注任务中，我们的研究的基本单位应该是路径，我们要做的事情，是从 k^n 条路径选出正确的一条，那就意味着，如果将它视为一个分类问题，那么将是 k^n 类中选一类的分类问题。




这就是逐帧 softmax 和 CRF 的根本不同了：**前者将序列标注看成是 n 个 k 分类问题，后者将序列标注看成是 1 个 k^n 分类问题**。




具体来讲，在 CRF 的序列标注问题中，我们要计算的是条件概率：









为了得到这个概率的估计，CRF 做了两个假设：




**假设一：该分布是指数族分布。**




这个假设意味着存在函数 f(y1,…,yn;x)，使得：









其中 Z(x) 是归一化因子，因为这个是条件分布，所以归一化因子跟 x 有关。这个 f 函数可以视为一个打分函数，打分函数取指数并归一化后就得到概率分布。 




**假设二：输出之间的关联仅发生在相邻位置，并且关联是指数加性的。**




这个假设意味着 f(y1,…,yn;x) 可以更进一步简化为：









这也就是说，现在我们只需要对每一个标签和每一个相邻标签对分别打分，然后将所有打分结果求和得到总分。




**线性链CRF**




尽管已经做了大量简化，但一般来说，(3) 式所表示的概率模型还是过于复杂，难以求解。于是考虑到当前深度学习模型中，RNN 或者层叠 CNN 等模型已经能够比较充分捕捉各个 y 与输出 x 的联系，因此，我们不妨考虑函数 g 跟 x 无关，那么：









这时候 g 实际上就是一个有限的、待训练的参数矩阵而已，而单标签的打分函数 h(yi;x) 我们可以通过 RNN 或者 CNN 来建模。因此，该模型是可以建立的，其中概率分布变为：









这就是线性链 CRF 的概念。




**归一化因子**




为了训练 CRF 模型，我们用最大似然方法，也就是用：









作为损失函数，可以算出它等于：









其中第一项是原来概率式的**分子**的对数，它目标的序列的打分，虽然它看上去挺迂回的，但是并不难计算。真正的难度在于**分母**的对数 logZ(x) 这一项。




归一化因子，在物理上也叫配分函数，在这里它需要我们对所有可能的路径的打分进行指数求和，而我们前面已经说到，这样的路径数是指数量级的（k^n），因此直接来算几乎是不可能的。




事实上，**归一化因子难算，几乎是所有概率图模型的公共难题**。幸运的是，在 CRF 模型中，由于我们只考虑了临近标签的联系（马尔可夫假设），因此我们可以递归地算出归一化因子，这使得原来是指数级的计算量降低为线性级别。




具体来说，我们将计算到时刻 t 的归一化因子记为 Zt，并将它分为 k 个部分：









其中分别是截止到当前时刻 t 中、以标签 1,…,k 为终点的所有路径的得分指数和。那么，我们可以递归地计算：









它可以简单写为矩阵形式：









其中，而 G 是对 g(yi,yj) 各个元素取指数后的矩阵，即；而是编码模型（RNN、CNN等）对位置 t+1 的各个标签的打分的指数，即，也是一个向量。式 (10) 中，ZtG 这一步是矩阵乘法，得到一个向量，而 ⊗ 是两个向量的逐位对应相乘。





**▲**归一化因子的递归计算图示。从t到t+1时刻的计算，包括转移概率和j+1节点本身的概率




如果不熟悉的读者，可能一下子比较难接受 (10) 式。读者可以把 n=1,n=2,n=3 时的归一化因子写出来，试着找它们的递归关系，慢慢地就可以理解 (10) 式了。




**动态规划**




写出损失函数 −logP(y1,…,yn|x) 后，就可以完成模型的训练了，因为目前的深度学习框架都已经带有自动求导的功能，只要我们能写出可导的 loss，就可以帮我们完成优化过程了。 




那么剩下的最后一步，就是模型训练完成后，如何根据输入找出最优路径来。跟前面一样，这也是一个从 k^n 条路径中选最优的问题，而同样地，因为马尔可夫假设的存在，它可以转化为一个动态规划问题，用 viterbi 算法解决，计算量正比于 n。 




动态规划在本博客已经出现了多次了，**它的递归思想就是：一条最优路径切成两段，那么每一段都是一条（局部）最优路径**。在本博客右端的搜索框键入“动态规划”，就可以得到很多相关介绍了，所以不再重复了。




# 实现




经过调试，基于 Keras 框架下，笔者得到了一个线性链 CRF 的简明实现，**这也许是最简短的 CRF 实现了**。这里分享最终的实现并介绍实现要点。




**实现要点**




前面我们已经说明了，实现 CRF 的困难之处是 −logP(y1,…,yn|x) 的计算，而本质困难是归一化因子部分 Z(x) 的计算，得益于马尔科夫假设，我们得到了递归的 (9) 式或 (10) 式，它们应该已经是一般情况下计算 Z(x) 的计算了。 




那么怎么在深度学习框架中实现这种递归计算呢？要注意，从计算图的视角看，这是通过递归的方法定义一个图，而且这个图的长度还不固定。这对于 PyTorch这样的动态图框架应该是不为难的，但是对于TensorFlow或者基于 TensorFlow 的 Keras 就很难操作了（它们是静态图框架）。 




不过，并非没有可能，**我们可以用封装好的 RNN 函数来计算**。我们知道，RNN 本质上就是在递归计算：









新版本的 TensorFlow 和 Keras 都已经允许我们自定义 RNN 细胞，这就意味着函数 f 可以自行定义，而后端自动帮我们完成递归计算。**于是我们只需要设计一个 RNN，使得我们要计算的 Z 对应于 RNN 的隐藏向量**。




这就是 CRF 实现中最精致的部分了。




至于剩下的，是一些细节性的，包括：




1. 为了防止溢出，我们通常要取对数，但由于归一化因子是指数求和，所以实际上是这样的格式，它的计算技巧是：









TensorFlow 和 Keras 中都已经封装好了对应的 logsumexp 函数了，直接调用即可；




2. 对于分子（也就是目标序列的得分）的计算技巧，在代码中已经做了注释，主要是通过用“目标序列”点乘“预测序列”来实现取出目标得分；




3. 关于变长输入的 padding 部分如何进行 mask？我觉得在这方面 Keras 做得并不是很好。




为了简单实现这种 mask，我的做法是引入多一个标签，比如原来是 s、b、m、e 四个标签做分词，然后引入第五个标签，比如 x，将 padding 部分的标签都设为 x，然后可以直接在 CRF 损失计算时忽略第五个标签的存在，具体实现请看代码。



**代码速览**




纯 Keras 实现的 CRF 层，欢迎使用。




```python
# -*- coding:utf-8 -*-

from keras.layers import Layer
import keras.backend as K


class CRF(Layer):
    """纯Keras实现CRF层
    CRF层本质上是一个带训练参数的loss计算层，因此CRF层只用来训练模型，
    而预测则需要另外建立模型。
    """
    def __init__(self, ignore_last_label=False, **kwargs):
        """ignore_last_label：定义要不要忽略最后一个标签，起到mask的效果
        """
        self.ignore_last_label = 1 if ignore_last_label else 0
        super(CRF, self).__init__(**kwargs)
    def build(self, input_shape):
        self.num_labels = input_shape[-1] - self.ignore_last_label
        self.trans = self.add_weight(name='crf_trans',
                                     shape=(self.num_labels, self.num_labels),
                                     initializer='glorot_uniform',
                                     trainable=True)
    def log_norm_step(self, inputs, states):
        """递归计算归一化因子
        要点：1、递归计算；2、用logsumexp避免溢出。
        技巧：通过expand_dims来对齐张量。
        """
        states = K.expand_dims(states[0], 2) # (batch_size, output_dim, 1)
        trans = K.expand_dims(self.trans, 0) # (1, output_dim, output_dim)
        output = K.logsumexp(states+trans, 1) # (batch_size, output_dim)
        return output+inputs, [output+inputs]
    def path_score(self, inputs, labels):
        """计算目标路径的相对概率（还没有归一化）
        要点：逐标签得分，加上转移概率得分。
        技巧：用“预测”点乘“目标”的方法抽取出目标路径的得分。
        """
        point_score = K.sum(K.sum(inputs*labels, 2), 1, keepdims=True) # 逐标签得分
        labels1 = K.expand_dims(labels[:, :-1], 3)
        labels2 = K.expand_dims(labels[:, 1:], 2)
        labels = labels1 * labels2 # 两个错位labels，负责从转移矩阵中抽取目标转移得分
        trans = K.expand_dims(K.expand_dims(self.trans, 0), 0)
        trans_score = K.sum(K.sum(trans*labels, [2,3]), 1, keepdims=True)
        return point_score+trans_score # 两部分得分之和
    def call(self, inputs): # CRF本身不改变输出，它只是一个loss
        return inputs
    def loss(self, y_true, y_pred): # 目标y_pred需要是one hot形式
        mask = 1-y_true[:,1:,-1] if self.ignore_last_label else None
        y_true,y_pred = y_true[:,:,:self.num_labels],y_pred[:,:,:self.num_labels]
        init_states = [y_pred[:,0]] # 初始状态
        log_norm,_,_ = K.rnn(self.log_norm_step, y_pred[:,1:], init_states, mask=mask) # 计算Z向量（对数）
        log_norm = K.logsumexp(log_norm, 1, keepdims=True) # 计算Z（对数）
        path_score = self.path_score(y_pred, y_true) # 计算分子（对数）
        return log_norm - path_score # 即log(分子/分母)
    def accuracy(self, y_true, y_pred): # 训练过程中显示逐帧准确率的函数，排除了mask的影响
        mask = 1-y_true[:,:,-1] if self.ignore_last_label else None
        y_true,y_pred = y_true[:,:,:self.num_labels],y_pred[:,:,:self.num_labels]
        isequal = K.equal(K.argmax(y_true, 2), K.argmax(y_pred, 2))
        isequal = K.cast(isequal, 'float32')
        if mask == None:
            return K.mean(isequal)
        else:
            return K.sum(isequal*mask) / K.sum(mask)
```




除去注释和 accuracy 的代码，真正的 CRF 的代码量也就 30 行左右，可以说跟哪个框架比较都称得上是简明的 CRF 实现了。




用纯 Keras 实现一些复杂的模型，是一件颇有意思的事情。目前仅在 TensorFlow 后端测试通过，理论上兼容 Theano、CNTK 后端，但可能要自行微调。




**使用案例**




我的 Github 中还附带了一个使用 CNN+CRF 实现的中文分词的例子，用的是 Bakeoff 2005 语料，例子是一个完整的分词实现，包括 viterbi 算法、分词输出等。 




> 
**Github地址**：https://github.com/bojone/crf/ 





相关的内容还可以看我之前的文章：




> 
中文分词系列：基于双向LSTM的seq2seq字标注 [2] 

中文分词系列：基于全卷积网络的中文分词 [3]





# 结语




终于介绍完了，希望大家有所收获，也希望最后的实现能对大家有所帮助。




# 参考文献




[1]. 果壳中的条件随机场 (CRF In A Nutshell)

https://kexue.fm/archives/4695

[2]. 中文分词系列：基于双向LSTM的seq2seq字标注

https://kexue.fm/archives/3924

[3]. 中文分词系列：基于全卷积网络的中文分词

https://kexue.fm/archives/4195









**点击以下标题查看作者其他文章：**




- 
[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)

- 
[基于CNN的阅读理解式问答模型：DGCNN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)


- 
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)


- 
[何恺明大神的「Focal Loss」，如何更好地理解？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486817&idx=1&sn=40ecdf7f06abafe0f472bffcf740ac09&chksm=96e9d2e1a19e5bf735b658368c286f5da6a69fd0ddcb6103c2e015f65e088feac6d6d53bd279&scene=21#wechat_redirect)










[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488566&idx=2&sn=fd5d9ee7fb899f161ad134d61a080977&chksm=96e9cbb6a19e42a01aea90fb30c1f326e45160fa326715695abbc6adeb2ee056d04cff0cbf15&scene=21#wechat_redirect)

[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)

**▲**戳我查看招募详情




**#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****









**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。






▽ 点击 | 阅读原文| 进入作者博客




