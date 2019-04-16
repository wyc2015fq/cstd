# QA智能问答（QQ匹配） - PeterBishop - CSDN博客





2019年01月28日 15:32:51[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：152








# QA智能问答（QQ匹配）

## GitHub:[https://github.com/PeterBishop0/-](https://github.com/PeterBishop0/-)

参考链接：[https://www.cnblogs.com/rucwxb/p/7975504.html](https://www.cnblogs.com/rucwxb/p/7975504.html)

[https://www.jianshu.com/p/6e1ef63615f8](https://www.jianshu.com/p/6e1ef63615f8)[https://zhuanlan.zhihu.com/p/29119239](https://zhuanlan.zhihu.com/p/29119239)

[https://www.cnblogs.com/combfish/p/6474298.html](https://www.cnblogs.com/combfish/p/6474298.html)

[http://www.52nlp.cn/qa%E9%97%AE%E7%AD%94%E7%B3%BB%E7%BB%9F%E4%B8%AD%E7%9A%84%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0?tdsourcetag=s_pctim_aiomsg](http://www.52nlp.cn/qa%E9%97%AE%E7%AD%94%E7%B3%BB%E7%BB%9F%E4%B8%AD%E7%9A%84%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E6%8A%80%E6%9C%AF%E5%AE%9E%E7%8E%B0?tdsourcetag=s_pctim_aiomsg)

[https://blog.csdn.net/accumulate_zhang/article/details/78703601](https://blog.csdn.net/accumulate_zhang/article/details/78703601)

### 原理

参看《Applying Deep Learning To Answer Selection: A Study And An Open Task》，文中比较了好几种网络结构，选择了效果相对较好的其中一个来实现，网络描述如下：

![](http://www.52nlp.cn/wp-content/uploads/2016/06/t1-1-300x142.png)

Q&A共用一个网络，网络中包括HL，CNN，P+T和Cosine_Similarity，HL是一个g(W*X+b)的非线性变换，CNN就不说了，P是max_pooling，T是激活函数Tanh，最后的Cosine_Similarity表示将Q&A输出的语义表示向量进行相似度计算。根据论文中的思路，我用tensorflow实现了一个cnn,q是用户的问题，qp是标准问题，qn是错误的标准问题（负向答案），R是relu，详细描述下从输入到输出的矩阵变换过程：
- 
**q,qp,qn：[batch_size, sequence_len]**，所有分词切编码（编码看具体代码注释）完成的句子需要截断或padding到一个固定长度（因为后面的CNN一般是处理固定长度的矩阵），例如句子包含3个词，我们选择固定长度sequence_len为100，则需要将这个句子padding成{三个词}0...0(100个字)，其中的0就是添加的专门用于padding的无意义的符号。训练时都是做mini-batch的，所以这里是一个batch_size行的矩阵，每行是一个句子。输入到embedding层训练得到对应的vector，输出qe,qpe,qne

- 
**qe,qpe,qne：[batch_size, sequence_len, embedding_size,1]**。句子中的每个词都需要转换成对应的词向量，字向量的维度大小是embedding_size，这样q就从一个2维的矩阵变成了4维的qe(加一维进行卷积)，输入到卷积运算中

- 
**CNN+P+R输出**：[batch_size, num_filters_total]。

CNN的filter_size分别是[1，2，3，5]，

W(权重矩阵)：[filter_size,embedding_size,1,num_filters],

b(偏差矩阵)：[num_filters],

strides=[1, 1, 1, 1]，R就是在输出结果上加上Relu激活函数，得到h

h:[batch_size,(sequence_len-filter_size)/stride[1]+1,(embedding_size-embedding_size)/stride[2]+1,num_filters]

在经历过len(filter_size)轮之后，reshape为[batch_size, num_filters_total]

num_filters_total=len(filter_size)*num_filters

最后dropout输出。



- 
**Cosine_Similarity：[batch_size]**。最后的一层并不是通常的分类或者回归的方法，而是采用了计算两个向量（Q&A）夹角的方法，下面是网络损失函数。![](http://www.52nlp.cn/wp-content/uploads/2016/06/t2-300x28.png)，m是需要设定的参数margin，VQ、VA+、VA-分别是问题、正向答案、负向答案对应的语义表示向量。损失函数的意义就是：让正向答案和问题之间的向量cosine值要大于负向答案和问题的向量cosine值，大多少，就是margin这个参数来定义的。cosine值越大，两个向量越相近，所以通俗的说这个Loss就是要让正向的答案和问题愈来愈相似，让负向的答案和问题越来越不相似。

以上就是一些实现的思路，具体代码（注释详细）、数据集见[Git](https://github.com/PeterBishop0/-)，有问题留言






