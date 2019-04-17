# 【深度剖析HMM（附Python代码）】5. 用HMM解决三个实际问题 - tostq的专栏 - CSDN博客





2017年04月27日 17:57:30[tostq](https://me.csdn.net/tostq)阅读数：8770








经过上几节的学习，相信大家对HMM都有了比较深的了解，这一节我们通过几个实际例子（丢骰子问题、中文分词问题及股票预测问题）来进一步讲解HMM



# 1. 丢骰子问题



具体代码参见：Dice_01.py

**丢骰子问题描述：**


假设六面骰、四面骰和八面骰各三枚，每次丢一枚骰子，记录骰子朝上的数字。多次丢骰子后，得到了一串数字，问：

         A.求每次丢的骰子的种类（问题1）

         B.丢出该串数字的概率（问题2）

         C．下次丢骰子最有可能的数字（问题3）

**问题分析：**

已知条件：

A. 观测值：骰子朝上的数字x

B. 隐状态：抛掷的骰子种类z

C. 隐状态初始概率：

![](https://img-blog.csdn.net/20170427145238263?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


D. 隐状态转换概率：

![](https://img-blog.csdn.net/20170427145259172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


E.发射概率为：

![](https://img-blog.csdn.net/20170427145318344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


假设掷15次骰子的结果为1 6 3 5 2 7 3 5 2 4 3 6 1 5 4

问题A：通过解码问题完成（decode函数），其实我们一眼就能知道结果了

Dice_01.py运行结果：这里的0,1,2分别表示六面骰、四面骰和八面骰


![](https://img-blog.csdn.net/20170427145842816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


问题B：丢出该结果的概率（X_prob函数）

运行结果，已对概率做了自然对数变换

![](https://img-blog.csdn.net/20170427145901991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


问题C：predict函数解决

这里分别表示下一状态各骰子丢出各个数字的概率，可以看出最有可能的数字为1，2，3，4。

![](https://img-blog.csdn.net/20170427145911210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




# 2. 中文分词问题

具体代码参见：Wordseg_02.py


**中文分词问题描述：**

假设有一中文句子，而该中文句子的每个字在该句中都有BEMS共四种状态，其中B代表该字是某词语中的起始字，M代表是某词语中的中间字，E代表是某词语中的结束字，S则代表是单字成词。中文分词问题是根据某中文句子得到其每个字的状态。比如：

“我在家里吃饭”——》（分词）“我|在|家里|吃中饭”——》（状态）“SSBEBME”

**问题分析：**


已知一系列的中文句子（序列）X以及句中每个字的状态（隐状态）Z.

我们将中文的每个字表示为字典里的序号，而每个字在序列中都有4个状态，由此将中文分词问题转换为离散HMM模型。

但是相较于问题1，问题2中并不是单个长序列，而一系列短序列，为了训练一系列短序列，我们需要对原训练方法进行改进。

最简单的思路，是将所有的短序列合并成一个长序列，但是如此以来无法训练出初始状态参数。

因此我们建立一个批量的HMM训练方法，此时我们需要重新改写参数优化的最大似然式：

![](https://img-blog.csdn.net/20170427151035476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


可以看出上式同单序列的训练方法非常一致，只是需要将多个序列的进行叠加起来，不过要注意的是不同序列在叠加之前，需要进行归一化处理！相关Python代码如下：



```python
def train_batch(self, X, Z_seq=list()):
        # 针对于多个序列的训练问题，其实最简单的方法是将多个序列合并成一个序列，而唯一需要调整的是初始状态概率
        # 输入X类型：list(array)，数组链表的形式
        # 输入Z类型: list(array)，数组链表的形式，默认为空列表（即未知隐状态情况）
        self.trained = True
        X_num = len(X) # 序列个数
        self._init(self.expand_list(X)) # 发射概率的初始化

        # 状态序列预处理，将单个状态转换为1-to-k的形式
        # 判断是否已知隐藏状态
        if Z_seq!=list():
            Z = []
            for n in range(X_num):
                Z.append(np.zeros((len(X[n]),self.n_state)))
                for i in range(len(Z[n])):
                    Z[n][i][int(Z_seq[n][i])] = 1
        else:
            Z = []  # 初始化状态序列list
            for n in range(X_num):
                Z.append(list(np.ones((len(X[n]), self.n_state))))

        for e in range(self.n_iter):  # EM步骤迭代
            # 更新初始概率过程
            #  E步骤
            print "iter: ", e
            b_post_state = []  # 批量累积：状态的后验概率，类型list(array)
            b_post_adj_state = np.zeros((self.n_state, self.n_state)) # 批量累积：相邻状态的联合后验概率，数组
            b_start_prob = np.zeros(self.n_state) # 批量累积初始概率
            for n in range(X_num): # 对于每个序列的处理
                X_length = len(X[n])
                alpha, c = self.forward(X[n], Z[n])  # P(x,z)
                beta = self.backward(X[n], Z[n], c)  # P(x|z)

                post_state = alpha * beta / np.sum(alpha * beta) # 归一化！
                b_post_state.append(post_state)
                post_adj_state = np.zeros((self.n_state, self.n_state))  # 相邻状态的联合后验概率
                for i in range(X_length):
                    if i == 0: continue
                    if c[i]==0: continue
                    post_adj_state += (1 / c[i]) * np.outer(alpha[i - 1],
                                                            beta[i] * self.emit_prob(X[n][i])) * self.transmat_prob

                if np.sum(post_adj_state)!=0:
                    post_adj_state = post_adj_state/np.sum(post_adj_state)  # 归一化！
                b_post_adj_state += post_adj_state  # 批量累积：状态的后验概率
                b_start_prob += b_post_state[n][0] # 批量累积初始概率

            # M步骤，估计参数，最好不要让初始概率都为0出现，这会导致alpha也为0
            b_start_prob += 0.00001*np.ones(self.n_state)
            self.start_prob = b_start_prob / np.sum(b_start_prob)
            b_post_adj_state += 0.001
            for k in range(self.n_state):
                if np.sum(b_post_adj_state[k])==0: continue
                self.transmat_prob[k] = b_post_adj_state[k] / np.sum(b_post_adj_state[k])

            self.emit_prob_updated(self.expand_list(X), self.expand_list(b_post_state))
```



实验结果：

![](https://img-blog.csdn.net/20170427175149404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的0 、1、 2、 3表示状态B、M、E、S

比如"我要回家吃饭"的分词结果为"我要|回家|吃|饭"

比如"江主席发表重要讲话"的分词结果为"江主席|发表|重要|讲话"

这个程序运行有点慢，实际不需要使用完整的HMM模型也能快速完成中文分词工作，具体可以参考这个代码[https://github.com/fxsjy/finalseg](https://github.com/fxsjy/finalseg)，其代码也非常简单，利用了一个简化的HMM思路，完成了一个快速中文分词。





# 3. 股票预测



具体代码参见：Stock_03.py


股票数据是典型的序列数据，也是HMM的经典应用例子，假设我们将股票的每日价格变化和交易额视为观测值，而将股票的涨跌或者维持视为隐状态，由此我们可以清晰地建立一个HMM模型。
实验结果：
![](https://img-blog.csdn.net/20170427175555859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里有四个张图，分别表示四种股票状态所出现的日期






# PS：





项目说明：[http://blog.csdn.net/tostq/article/details/70846702](http://blog.csdn.net/tostq/article/details/70846702)

代码下载：[https://github.com/tostq/Easy_HMM](https://github.com/tostq/Easy_HMM) （点星是对作者最好的支持！！！^_^）




