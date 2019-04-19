# Caffe中学习率策略应如何选择 - 数据之美的博客 - CSDN博客
2017年04月12日 19:05:08[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：980
今天，在训练网络时想换一种学习策略试试，因此重新研究了一下Caffe中提供的各种学习率策略，在这里和大家聊聊我使用时的一些经验教训。
我们先来看看和学习率策略有关的参数，以下的内容来自caffe.proto中：
**[cpp]**[view
 plain](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)[copy](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2042631/fork)
- // The learning rate decay policy. The currently implemented learning rate
- // policies are as follows:
- //    - fixed: always return base_lr.
- //    - step: return base_lr * gamma ^ (floor(iter / step))
- //    - exp: return base_lr * gamma ^ iter
- //    - inv: return base_lr * (1 + gamma * iter) ^ (- power)
- //    - multistep: similar to step but it allows non uniform steps defined by
- //      stepvalue
- //    - poly: the effective learning rate follows a polynomial decay, to be
- //      zero by the max_iter. return base_lr (1 - iter/max_iter) ^ (power)
- //    - sigmoid: the effective learning rate follows a sigmod decay
- //      return base_lr ( 1/(1 + exp(-gamma * (iter - stepsize))))
- //
- // where base_lr, max_iter, gamma, step, stepvalue and power are defined
- // in the solver parameter protocol buffer, and iter is the current iteration.
-   optional string lr_policy = 8;  
-   optional float gamma = 9; // The parameter to compute the learning rate.
-   optional float power = 10; // The parameter to compute the learning rate.
**[cpp]**[view
 plain](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)[copy](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2042631/fork)
- optional float base_lr = 5; // The base learning rate
**[cpp]**[view
 plain](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)[copy](http://blog.csdn.net/sunshine_in_moon/article/details/53563611#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2042631/fork)
- // the stepsize for learning rate policy "step"
-   optional int32 stepsize = 13;  
- // the stepsize for learning rate policy "multistep"
-   repeated int32 stepvalue = 34;  
其实，上面已经写得很清楚了，尤其是学习率是如何计算的也已经给出了详细的说明。我想和大家分享我的一些心得。
就我现在看到的使用最多的学习策略有fixed，step，inv，multistep。下面一个个介绍。
1、fixed 固定策略
故名思议就是学习率始终是一个固定值，如果你是一个新手或者你正准备训练一个全新的网络，我建议您使用这种策略。因为现在你对与数据的分布和网络参数还都一无所知，fixed策略方便调节，我们可以根据训练情况包括loss和accuracy的值随时调节我们的学习率。比如，我喜欢开始时将学习率设置一个较大的值（0.1-0.01之间），但是不能使loss爆掉呦。而后通过观察loss和accuracy的值，比如两者下降缓慢或不在下降，出现震荡的情况，就将学习率调小。也就是逐次减小的策略。
2、step 均匀分步策略
这个策略要结合参数stepsize使用，当循环次数达到stepsize的整数倍时lr=base_lr*gamma^(floor(iter/stepsize)).在这里给大家说一个我最近使用这种策略所犯的错误。我从250000次开始继续训练我的网络，我将base_lr设置成0.001，stepsize设置成100000，gamma设置成0.1，我的本意是想让训练在300000次的时候学习率降到0.0001。结果呢？相信大家已经知道答案了学习率变成了1e-6.因此特别提醒你，如果你是在训练中途将学习策略改变成step，请仔细计算stepsize的值。
3、multistep 多分步或不均匀分步
这种学习策略也是我们最近才看到有人使用，这种学习策略和step策略很相似。这种学习策略需要配合参数stepvalue使用，stepvalue可以在文件中设置多个，如stepvalue=10000，stepvalue=20000，......,当迭代次数达到我们依次指定的stepvalue的值时，学习率就会根据公式重新计算。这种学习率策略我虽然尝试的并不多，但是我发现他有一个很好的用处，就是我们在刚开始训练网络时学习率一般设置较高，这样loss和accuracy下降很快，一般前200000次两者下降较快，后面可能就需要我们使用较小的学习率了。step策略由于过于平均，而loss和accuracy的下降率在整个训练过程中又是一个不平均的过程，因此有时不是很合适。fixed手工调节起来又很麻烦，这时multistep可能就会派上用场了。
4、inv （英语太差不知是哪个单词的缩写）
其实，从公式中我们就可以看到，这种学习策略的优势就在于它使得学习率在每一次迭代时都减小，但每次减小又都是一个非常小的数，这样就省去的自己手动调节的麻烦。这种策略使用的也很普遍。
以上四种学习策略就是我见到的大家经常使用的策略。本来想把一些典型的参数也给大家贴出来，想了想还是算了，这留给大家了。给大家一个建议多看看Caffe中提供的solve.prototxt里大神们是怎样选择参数的，这对我们很有帮助。
