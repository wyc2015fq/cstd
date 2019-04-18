# 训练过程--关于训练数据与标签的trick - wydbyxr的博客 - CSDN博客
2018年12月05日 13:49:55[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：52
# Label Shuffling平衡策略
  在去年冠军土堆的Class-Aware Sampling方法的启发下，海康威视提出了Label Shuffling的类别平衡策略。
  Label Shuffling方法的优点在于，只需要原始图像列表，所有操作都是在内存中在线完成，非常易于实现。hard-data-mining。
  DL 产生较为严重的误差原因大概是，lable 其实正负数对半，但模型的输出几乎都是正，即使是负数，该预测值的数值也非常少，并没有找到合适的解决方法。
## 具体步骤
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205140721655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  1）首先对原始图像列表按照标签顺序进行排序。
  2）然后计算每个类别的样本数，并且得到样本最多的那个类别的样本数，根据这个最多的样本数，对每一类随机产生一个随机排列的列表。
  3）然后用每个类别的列表中的数对各自类别的样本数求余，得到一个索引值，从该类的图像中提取图像，生成该类的图像随机列表；
  4）然后把所有类别的随机列表连在一起，做个Random Shuffling，得到最后的图像列表，用这个列表进行训练。
# 标签不足的情况
  这种情况下，使用的很多方法都在pretrain范畴，但不一定是SAE，DBN这种。
# 难分样本挖掘(hard example mining)
  将难分样本挖掘(hard example mining)机制嵌入到SGD算法中，使得在训练的过程中根据region proposal的损失自动选取合适的region proposal作为正负例训练。最终提高精度。
# 使用对抗样本进行训练
  神经网络很容易被对抗样本戏弄。一个金鱼图片本来可以分类正确。但是，如果我们加入中间图片的噪音模式，则分类器认为这是一张雏菊的图片。
## 解决
  显然，多扫视后投票和无监督预训练的策略都不能解决这个漏洞。
  使用高度正则化会有所帮助，但会影响判断不含噪声图像的准确性。
  Ian Goodfellow提出了训练这些对抗样本的理念。它们可以自动的生成并添加到训练集中。结果表明，除了对对抗样本有所帮助之外，这也提高了原始样本上的准确性。
