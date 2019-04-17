# 【深度剖析HMM（附Python代码）】4.HMM代码测试及hmmlearn介绍 - tostq的专栏 - CSDN博客





2017年04月27日 14:35:54[tostq](https://me.csdn.net/tostq)阅读数：6428








相信经过上几节的说明，大家对于HMM应该有比较好的了解，也许大家已经自己试着运行代码了。

这一节主要介绍下另一个著名的HMM的Python库——hmmlearn，这个库提供了三个HMM模型（高斯HMM、离散HMM及高斯混合HMM），比我的代码速度更快，而且更有稳定，而且其还提供了相应的教程和API函数说明：[http://hmmlearn.readthedocs.io/en/latest/index.html](http://hmmlearn.readthedocs.io/en/latest/index.html)

而本文提供的代码更为简单，比较适合初学者学习参考，但不适合实际应用，实际应用可以使用hmmlearn。

为了考察本文代码的准确性，本节选择hmmlearn作为参考组，利用python测试工具unnitest，来考察代码的正确性。大体思路是先利用hmmlearn初始化一个具体HMM模型作为参照组，并生成一段序列，再将该序列作为本文HMM模型的训练样本，最后比较同参照组的参数差别。


# 1、python测试工具unnitest
测试工具unnitest非常容易使用，首先是建立一个继承自TestCase的测试类，然后通过覆盖setUp()完成相关初始化，最后通过覆盖tearDown()方法清除测试中产生的数据，为以后的TestCase留下一个干净的环境。我们需要在测试类中编写以test_开头的测试函数，unnitest会在测试中自动执行以test_开头的测试函数，unnitest的使用框架：







```python
import unittest  
  
class XXXXX(unittest.TestCase):  
  
    def setUp(self):  
	# 自行编写
	pass


if __name__ == '__main__':  
    unittest.main()
```


参考文献：
[https://docs.python.org/2/library/unittest.html](https://docs.python.org/2/library/unittest.html)



对离散HMM模型的测试代码



```python
# 计算平方误差
def s_error(A, B):
    return sqrt(np.sum((A-B)*(A-B)))/np.sum(B)


class DiscreteHMM_Test(unittest.TestCase):


    def setUp(self):
        # 建立两个HMM，隐藏状态个数为4，X可能分布为10类
        n_state =4
        n_feature = 10
        X_length = 1000
        n_batch = 100 # 批量数目
        self.n_batch = n_batch
        self.X_length = X_length
        self.test_hmm = hmm.DiscreteHMM(n_state, n_feature)
        self.comp_hmm = ContrastHMM(n_state, n_feature)
        self.X, self.Z = self.comp_hmm.module.sample(self.X_length*10)
        self.test_hmm.train(self.X, self.Z)


    def test_train_batch(self):
        X = []
        Z = []
        for b in range(self.n_batch):
            b_X, b_Z = self.comp_hmm.module.sample(self.X_length)
            X.append(b_X)
            Z.append(b_Z)


        batch_hmm = hmm.DiscreteHMM(self.test_hmm.n_state, self.test_hmm.x_num)
        batch_hmm.train_batch(X, Z)
        # 判断概率参数是否接近
        # 初始概率判定没有通过！！！
        self.assertAlmostEqual(s_error(batch_hmm.start_prob, self.comp_hmm.module.startprob_), 0, 1)
        self.assertAlmostEqual(s_error(batch_hmm.transmat_prob, self.comp_hmm.module.transmat_), 0, 1)
        self.assertAlmostEqual(s_error(batch_hmm.emission_prob, self.comp_hmm.module.emissionprob_), 0, 1)


    def test_train(self):
        # 判断概率参数是否接近
        # 单批量的初始概率一定是不准的
        # self.assertAlmostEqual(s_error(self.test_hmm.start_prob, self.comp_hmm.module.startprob_), 0, 1)
        self.assertAlmostEqual(s_error(self.test_hmm.transmat_prob, self.comp_hmm.module.transmat_), 0, 1)
        self.assertAlmostEqual(s_error(self.test_hmm.emission_prob, self.comp_hmm.module.emissionprob_), 0, 1)


    def test_X_prob(self):
        X,_ = self.comp_hmm.module.sample(self.X_length)
        prob_test = self.test_hmm.X_prob(X)
        prob_comp = self.comp_hmm.module.score(X)
        self.assertAlmostEqual(s_error(prob_test, prob_comp), 0, 1)


    def test_predict(self):
        X, _ = self.comp_hmm.module.sample(self.X_length)
        prob_next = self.test_hmm.predict(X,np.random.randint(0,self.test_hmm.x_num-1))
        self.assertEqual(prob_next.shape,(self.test_hmm.n_state,))


    def test_decode(self):
        X,_ = self.comp_hmm.module.sample(self.X_length)
        test_decode = self.test_hmm.decode(X)
        _, comp_decode = self.comp_hmm.module.decode(X)
        self.assertAlmostEqual(s_error(test_decode, comp_decode), 0, 1)


if __name__ == '__main__':
    unittest.main()
```



# 2、hmmlearn产生HMM模型
利用hmmlearn初始化一个高斯HMM模型




```python
class ContrastHMM():
    def __init__(self, n_state, n_feature):
        self.module = hmmlearn.hmm.GaussianHMM(n_components=n_state,covariance_type="full")
        # 初始概率
        self.module.startprob_ = np.random.random(n_state)
        self.module.startprob_ = self.module.startprob_ / np.sum(self.module.startprob_)
        # 转换概率
        self.module.transmat_ = np.random.random((n_state,n_state))
        self.module.transmat_ = self.module.transmat_ / np.repeat(np.sum(self.module.transmat_, 1),n_state).reshape((n_state,n_state))
        # 高斯发射概率
        self.module.means_ = np.random.random(size=(n_state,n_feature))*10
        self.module.covars_ = .5 * np.tile(np.identity(n_feature), (n_state, 1, 1))
```



利用hmmlearn初始化一个离散HMM模型




```python
class ContrastHMM():
    def __init__(self, n_state, n_feature):
        self.module = hmmlearn.hmm.MultinomialHMM(n_components=n_state)
        # 初始概率
        self.module.startprob_ = np.random.random(n_state)
        self.module.startprob_ = self.module.startprob_ / np.sum(self.module.startprob_)
        # print self.module.startprob_
        # 转换概率
        self.module.transmat_ = np.random.random((n_state,n_state))
        self.module.transmat_ = self.module.transmat_ / np.repeat(np.sum(self.module.transmat_, 1),n_state).reshape((n_state,n_state))
        # print self.module.transmat_
        # 发射概率
        self.module.emissionprob_ = np.random.random(size=(n_state,n_feature))
        self.module.emissionprob_ = self.module.emissionprob_ / np.repeat(np.sum(self.module.emissionprob_, 1),n_feature).reshape((n_state,n_feature))
        # print self.module.emissionprob_
```



# PS：




项目说明：[http://blog.csdn.net/tostq/article/details/70846702](http://blog.csdn.net/tostq/article/details/70846702)


代码下载：[https://github.com/tostq/Easy_HMM](https://github.com/tostq/Easy_HMM) （点星是对作者最好的支持！！！^_^）






