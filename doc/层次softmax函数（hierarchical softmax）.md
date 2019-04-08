## 层次softmax函数（hierarchical softmax）





## 一、h-softmax

在面对label众多的分类问题时，fastText设计了一种hierarchical softmax函数。使其具有以下**优势**：

- （1）适合大型数据+高效的训练速度：能够训练模型“在使用标准多核CPU的情况下10分钟内处理超过10亿个词汇”，特别是与深度模型对比，fastText能将训练时间由数天缩短到几秒钟。
- （2）支持多语言表达：利用其语言形态结构，fastText能够被设计用来支持包括英语、德语、西班牙语、法语以及捷克语等多种语言。

可以认为，FastText= （word2vec中）CBOW + h-softmax；其结构为：输入 - 隐层 - h-softmax

### 基本原理

- 根据标签（label）和频率建立霍夫曼树；（label出现的频率越高，Huffman树的路径越短）
- Huffman树中每一叶子结点代表一个label；

 

## 二、理论分析

### 层次之间的映射

将输入层中的词和词组构成特征向量，再将特征向量通过线性变换映射到隐藏层，隐藏层通过求解最大似然函数，然后根据每个类别的权重和模型参数构建Huffman树，将Huffman树作为输出。 
![这里写图片描述](https://img-blog.csdn.net/20170315150912764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWWlja19MaWFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 模型的训练

Huffman树中每一叶子结点代表一个label，在每一个非叶子节点处都需要作一次二分类，走左边的概率和走右边的概率，这里用逻辑回归的公式表示 
![img](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180413105803435-1055736024.png)

 

### 如何做到fast

![img](https://images2018.cnblogs.com/blog/1181483/201804/1181483-20180413110133810-774587320.png)

 

参考：[fastText原理及应用](https://blog.csdn.net/yick_liao/article/details/62222153)