# Keras之小众需求：自定义优化器 - Paper weekly - CSDN博客





2018年09月11日 12:11:38[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：860









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




今天我们来看一个小众需求：**自定义优化器**。




细想之下，不管用什么框架，自定义优化器这个需求可谓真的是小众中的小众。一般而言，对于大多数任务我们都可以无脑地直接上 Adam，而调参炼丹高手一般会用 SGD 来调出更好的效果，换言之不管是高手新手，都很少会有自定义优化器的需求。




那这篇文章还有什么价值呢？有些场景下会有一点点作用。比如**通过学习 Keras 中的优化器写法，你可以对梯度下降等算法有进一步的认识，你还可以顺带看到 Keras 的源码是多么简洁优雅。**




此外，有时候我们可以通过自定义优化器来实现自己的一些功能，比如给一些简单的模型（例如 Word2Vec）重写优化器（直接写死梯度，而不是用自动求导），可以使得算法更快；自定义优化器还可以实现诸如“软 batch”的功能。




# Keras优化器




我们首先来看 Keras 中自带优化器的代码，位于：




https://github.com/keras-team/keras/blob/master/keras/optimizers.py




简单起见，我们可以先挑 SGD 来看。当然，Keras 中的 SGD 算法已经把 momentum、nesterov、decay 等整合进去了，这使用起来方便，但不利于学习。所以我稍微简化了一下，给出一个纯粹的 SGD 算法的例子：




```
from keras.legacy import interfaces
from keras.optimizers import Optimizer
from keras import backend as K


class SGD(Optimizer):
    """Keras中简单自定义SGD优化器
    """

    def __init__(self, lr=0.01, **kwargs):
        super(SGD, self).__init__(**kwargs)
        with K.name_scope(self.__class__.__name__):
            self.iterations = K.variable(0, dtype='int64', name='iterations')
            self.lr = K.variable(lr, name='lr')

    @interfaces.legacy_get_updates_support
    def get_updates(self, loss, params):
        """主要的参数更新算法
        """
        grads = self.get_gradients(loss, params) # 获取梯度
        self.updates = [K.update_add(self.iterations, 1)] # 定义赋值算子集合
        self.weights = [self.iterations] # 优化器带来的权重，在保存模型时会被保存
        for p, g in zip(params, grads):
            # 梯度下降
            new_p = p - self.lr * g
            # 如果有约束，对参数加上约束
            if getattr(p, 'constraint', None) is not None:
                new_p = p.constraint(new_p)
            # 添加赋值
            self.updates.append(K.update(p, new_p))

        return self.updates

    def get_config(self):
        config = {'lr': float(K.get_value(self.lr))}
        base_config = super(SGD, self).get_config()
        return dict(list(base_config.items()) + list(config.items()))
```




应该不是解释了吧？有没有特别简单的感觉？定义一个优化器也不是特别高大上的事情。




# 实现“软batch” 




现在来实现一个稍微复杂一点的功能，就是所谓的“软 batch”，不过我不大清楚是不是就叫这个名字，姑且先这样叫着吧。大概的场景是：假如模型比较庞大，自己的显卡最多也就能跑 batch size=16，但我又想起到 batch size=64 的效果，那可以怎么办呢？




一种可以考虑的方案是，每次算 batch size=16，然后把梯度缓存起来，4 个 batch 后才更新参数。也就是说，每个小 batch 都算梯度，但每 4 个 batch 才更新一次参数。 




```
class MySGD(Optimizer):
    """Keras中简单自定义SGD优化器
    每隔一定的batch才更新一次参数
    """
    def __init__(self, lr=0.01, steps_per_update=1, **kwargs):
        super(MySGD, self).__init__(**kwargs)
        with K.name_scope(self.__class__.__name__):
            self.iterations = K.variable(0, dtype='int64', name='iterations')
            self.lr = K.variable(lr, name='lr')
            self.steps_per_update = steps_per_update # 多少batch才更新一次

    @interfaces.legacy_get_updates_support
    def get_updates(self, loss, params):
        """主要的参数更新算法
        """
        shapes = [K.int_shape(p) for p in params]
        sum_grads = [K.zeros(shape) for shape in shapes] # 平均梯度，用来梯度下降
        grads = self.get_gradients(loss, params) # 当前batch梯度
        self.updates = [K.update_add(self.iterations, 1)] # 定义赋值算子集合
        self.weights = [self.iterations] + sum_grads # 优化器带来的权重，在保存模型时会被保存
        for p, g, sg in zip(params, grads, sum_grads):
            # 梯度下降
            new_p = p - self.lr * sg / float(self.steps_per_update)
            # 如果有约束，对参数加上约束
            if getattr(p, 'constraint', None) is not None:
                new_p = p.constraint(new_p)
            cond = K.equal(self.iterations % self.steps_per_update, 0)
            # 满足条件才更新参数
            self.updates.append(K.switch(cond, K.update(p, new_p), p))
            # 满足条件就要重新累积，不满足条件直接累积
            self.updates.append(K.switch(cond, K.update(sg, g), K.update(sg, sg+g)))
        return self.updates

    def get_config(self):
        config = {'lr': float(K.get_value(self.lr)),
                  'steps_per_update': self.steps_per_update}
        base_config = super(MySGD, self).get_config()
        return dict(list(base_config.items()) + list(config.items()))
```




应该也很容易理解吧。如果带有动量的情况，写起来复杂一点，但也是一样的。重点就是引入多一个变量来储存累积梯度，然后引入 cond 来控制是否更新，原来优化器要做的事情，都要在 cond 为 True 的情况下才做（梯度改为累积起来的梯度）。对比原始的 SGD，改动并不大。




# “侵入式”优化器




上面实现优化器的方案是标准的，也就是按 Keras 的设计规范来做的，所以做起来很轻松。然而我曾经想要实现的一个优化器，却不能用这种方式来实现，经过阅读源码，得到了一种“侵入式”的写法，这种写法类似“外挂”的形式，可以实现我需要的功能，但不是标准的写法，在此也跟大家分享一下。




原始需求来源于之前的文章[从动力学角度看优化算法SGD：一些小启示](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490231&idx=1&sn=cfe4c949f1e11466994ce03a0ac76b76&chksm=96e9c537a19e4c21a5ab2e320792bca26334fb9f6d90e03ed6edccff7bd301935bbac19b4938&scene=21#wechat_redirect)，里边指出梯度下降优化器可以看成是微分方程组的欧拉解法，进一步可以联想到，微分方程组有很多比欧拉解法更高级的解法呀，能不能用到深度学习中？比如稍微高级一点的有**“Heun 方法 [1]”**：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75QYt6qDrSw87J4e2E9ib23tEkPtF2HiaK33hGVcNz8XMCAGI1DKrRick7w/640)




其中 p 是参数（向量），g 是梯度，pi 表示 p 的第 i 次迭代时的结果。这个算法需要走两步，大概意思就是普通的梯度下降先走一步（探路），然后根据探路的结果取平均，得到更精准的步伐，等价地可以改写为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgluwxnqCYh4Y3HGkiaktiac75cvvSEuDAdJ0oJZQHLZXsjvz4uTm57LZfbsPQMes0mQNHvScsOSBJoQ/640)




这样就清楚显示出后面这一步实际上是对梯度下降的微调。 




但是实现这类算法却有个难题，要计算两次梯度，一次对参数 g(pi)，另一次对参数 p̃i+1。而前面的优化器定义中 get_updates 这个方法却只能执行一步（对应到 tf 框架中，就是执行一步 sess.run，熟悉 tf 的朋友知道单单执行一步 sess.run 很难实现这个需求），因此实现不了这种算法。




经过研究 Keras 模型的训练源码，我发现可以这样写：




```
class HeunOptimizer:
    """自定义Keras的侵入式优化器
    """

    def __init__(self, lr):
        self.lr = lr

    def __call__(self, model):
        """需要传入模型，直接修改模型的训练函数，而不按常规流程使用优化器，所以称为“侵入式”
        其实下面的大部分代码，都是直接抄自keras的源码：
        https://github.com/keras-team/keras/blob/master/keras/engine/training.py#L491
        也就是keras中的_make_train_function函数。
        """
        params = model._collected_trainable_weights
        loss = model.total_loss

        inputs = (model._feed_inputs +
                  model._feed_targets +
                  model._feed_sample_weights)
        inputs += [K.learning_phase()]

        with K.name_scope('training'):
            with K.name_scope('heun_optimizer'):
                old_grads = [[K.zeros(K.int_shape(p)) for p in params]]
                update_functions = []
                for i,step in enumerate([self.step1, self.step2]):
                    updates = (model.updates +
                               step(loss, params, old_grads) +
                               model.metrics_updates)
                    # 给每一步定义一个K.function
                    updates = K.function(inputs,
                                         [model.total_loss] + model.metrics_tensors,
                                         updates=updates,
                                         name='train_function_%s'%i,
                                         **model._function_kwargs)
                    update_functions.append(updates)

                def F(ins):
                    # 将多个K.function封装为一个单独的函数
                    # 一个K.function就是一次sess.run
                    for f in update_functions:
                        _ = f(ins)
                    return _

                # 最后只需要将model的train_function属性改为对应的函数
                model.train_function = F

    def step1(self, loss, params, old_grads):
        ops = []
        grads = K.gradients(loss, params)
        for p,g,og in zip(params, grads, old_grads[0]):
            ops.append(K.update(og, g))
            ops.append(K.update(p, p - self.lr * g))
        return ops

    def step2(self, loss, params, old_grads):
        ops = []
        grads = K.gradients(loss, params)
        for p,g,og in zip(params, grads, old_grads[0]):
            ops.append(K.update(p, p - 0.5 * self.lr * (g - og)))
        return ops
```




用法是：




```
opt = HeunOptimizer(0.1)
opt(model)

model.fit(x_train, y_train, epochs=100, batch_size=32)
```




其中关键思想在代码中已经注释了，主要是 Keras 的优化器最终都会被包装为一个 train_function，所以我们只需要参照 Keras 的源码设计好 train_function，并在其中插入我们自己的操作。在这个过程中，需要留意到 K.function 所定义的操作相当于一次 sess.run 就行了。




**注：类似地还可以实现 RK23、RK45 等算法。**遗憾的是，这种优化器缺很容易过拟合，也就是很容易将训练集的 loss 降到很低，但是验证集的 loss 和准确率都很差。




# 优雅的Keras




本文讲了一个非常非常小众的需求：自定义优化器，介绍了一般情况下 Keras 优化器的写法，以及一种“侵入式”的写法。如果真有这么个特殊需求，可以参考使用。




通过 Keras 中优化器的分析研究，我们进一步可以观察到 Keras 整体代码实在是非常简洁优雅，难以挑剔。




# 参考文献




[1]. https://en.wikipedia.org/wiki/Heun%27s_method




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)

- 
[基于CNN的阅读理解式问答模型：DGCNN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)

- 
[再谈最小熵原理：飞象过河之句模版和语言结构](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489564&idx=1&sn=a5d2191dd978e7917f06f66022ce8358&chksm=96e9c79ca19e4e8ac874f0fa5808b99b7323149d42a78fade690c6d60c0337ecabe043f616d1&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)












**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。









