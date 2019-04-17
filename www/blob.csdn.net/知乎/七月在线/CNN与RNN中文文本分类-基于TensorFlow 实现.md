# CNN与RNN中文文本分类-基于TensorFlow 实现 - 知乎
# 



> 摘要：本文是基于TensorFlow在中文数据集上的简化实现，使用了字符级CNN和RNN对中文文本进行分类，达到了较好的效果。




CNN做句子分类的论文可以参看:

Convolutional Neural Networks for Sentence Classification




还可以去读dennybritz大牛的博客：

Implementing a CNN for Text Classification in TensorFlow




以及字符级CNN的论文：

Character-level Convolutional Networks for Text Classification




如今，TensorFlow大版本已经升级到了1.3，对很多的网络层实现了更高层次的封装和实现，甚至还整合了如Keras这样优秀的一些高层次框架，使得其易用性大大提升。相比早起的底层代码，如今的实现更加简洁和优雅。




本文是基于TensorFlow在中文数据集上的简化实现，使用了字符级CNN和RNN对中文文本进行分类，达到了较好的效果。




**数据集**




本文采用了清华NLP组提供的THUCNews新闻文本分类数据集的一个子集（原始的数据集大约74万篇文档，训练起来需要花较长的时间）。数据集请自行到THUCTC：**一个高效的中文文本分类工具包下载**，请遵循数据提供方的开源协议。（下载地址：[http://thuctc.thunlp.org/](https://link.zhihu.com/?target=http%3A//thuctc.thunlp.org/)）




本次训练使用了其中的10个分类，每个分类6500条数据。




类别如下：

体育, 财经, 房产, 家居, 教育, 科技, 时尚, 时政, 游戏, 娱乐




数据集划分如下：
- 训练集: 5000*10
- 验证集: 500*10
- 测试集: 1000*10




从原数据集生成子集的过程请参看helper下的两个脚本。其中，copy_data.sh用于从每个分类拷贝6500个文件，cnews_group.py用于将多个文件整合到一个文件中。




执行该文件后，得到三个数据文件：
- cnews.train.txt: 训练集(50000条)
- cnews.val.txt: 验证集(5000条)
- cnews.test.txt: 测试集(10000条)

**预处理**

data/cnews_loader.py为数据的预处理文件。



- read_file(): 读取文件数据;
- build_vocab(): 构建词汇表，使用字符级的表示，这一函数会将词汇表存储下来，避免每一次重复处理;
- read_vocab(): 读取上一步存储的词汇表，转换为 {词：id} 表示;
- read_category(): 将分类目录固定，转换为 {类别: id}表示;
- to_words(): 将一条由id表示的数据重新转换为文字;
- preocess_file(): 将数据集从文字转换为固定长度的id序列表示;
- batch_iter(): 为神经网络的训练准备经过shuffle的批次的数据。




经过数据预处理，数据的格式如下：
![](https://pic3.zhimg.com/v2-ed32cddf759a552795a6bf323d521bfe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='403' height='149'></svg>)



**CNN卷积神经网络**




**配置项**




CNN可配置的参数如下所示，在cnn_model.py中。
![](https://pic1.zhimg.com/v2-57610cbee5aad402ab5cdfe9948ad87c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='921' height='494'></svg>)



**CNN模型**




具体参看cnn_model.py的实现。




大致结构如下：



![](https://pic3.zhimg.com/v2-b61cae160749bbfeb4df0ab8924f7376_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1072' height='690'></svg>)



**训练与验证**




运行 python run_cnn.py train，可以开始训练。




若之前进行过训练，请把tensorboard/textcnn删除，避免TensorBoard多次训练结果重叠。



![](https://pic4.zhimg.com/v2-9cdc25bb4d0f1785b966823b783a5e5b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='933' height='548'></svg>)



在验证集上的最佳效果为94.12%，且只经过了3轮迭代就已经停止。




准确率和误差如图所示：



![](https://pic4.zhimg.com/v2-b309300010201334180d65945cfab11b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='327'></svg>)



**测试**




运行 python run_cnn.py test 在测试集上进行测试。



![](https://pic3.zhimg.com/v2-ab705c8e46209162764636e203a96786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='427'></svg>)![](https://pic4.zhimg.com/v2-c8f4414f298bc843bf3c0b2ce62dc263_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='929' height='272'></svg>)



在测试集上的准确率达到了96.04%，且各类的precision, recall和f1-score都超过了0.9。




从混淆矩阵也可以看出分类效果非常优秀。




**RNN循环神经网络**




**配置项**




RNN可配置的参数如下所示，在rnn_model.py中。



![](https://pic3.zhimg.com/v2-b54d2ba1c3dd112c4605e0428d0278ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='929' height='455'></svg>)



**RNN模型**




具体参看 rnn_model.py 的实现。




大致结构如下：



![](https://pic2.zhimg.com/v2-eab98014972a662f5902a44d7befb97d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1046' height='670'></svg>)



**训练与验证**




这部分的代码与 run_cnn.py极为相似，只需要将模型和部分目录稍微修改。




运行 python run_rnn.py train，可以开始训练。




若之前进行过训练，请把tensorboard/textrnn删除，避免TensorBoard多次训练结果重叠。
![](https://pic2.zhimg.com/v2-d51123c5ac0dcf324e27888e52685fe5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='536'></svg>)![](https://pic2.zhimg.com/v2-bd702aa35bb3b81b13b814879f705561_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='929' height='429'></svg>)



在验证集上的最佳效果为91.42%，经过了8轮迭代停止，速度相比CNN慢很多。




准确率和误差如图所示：



![](https://pic4.zhimg.com/v2-d550c4bce7eaad7c6be26478a2b0e6d3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='339'></svg>)



**测试**




运行 python run_rnn.py test 在测试集上进行测试。



![](https://pic4.zhimg.com/v2-86ab0c48a4a5fa24a11f668d2f00a107_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='938' height='574'></svg>)



在测试集上的准确率达到了94.22%，且各类的precision, recall和f1-score，除了家居这一类别，都超过了0.9。




从混淆矩阵可以看出分类效果非常优秀。




对比两个模型，可见RNN除了在家居分类的表现不是很理想，其他几个类别较CNN差别不大。




还可以通过进一步的调节参数，来达到更好的效果。




为方便预测，repo 中 predict.py 提供了 CNN 模型的预测方法。

> 来源 | Github
作者 | Gaussic
原文链接：[https://github.com/gaussic/text-classification-cnn-rnn](https://link.zhihu.com/?target=https%3A//github.com/gaussic/text-classification-cnn-rnn)




**分享一哈：七月在线（11.10-11.12）三天大回馈**

今晚24点结束 还没参加的亲们抓紧时间喽

机械键盘等奖品等你

更有精品课程继续1元秒 爆款课程0元送

 主会场传送门   ↓   ↓   ↓   ↓   ↓   ↓   ↓  
[七月在线双11三天大回馈（11.10~11.12）：免费送2018VIP、精品课程1元秒，3天3夜大奖直抽iPhone X​www.julyedu.com![图标](https://pic2.zhimg.com/v2-132875ca2ffc565664ea31699a8234ad_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/active/sale)





