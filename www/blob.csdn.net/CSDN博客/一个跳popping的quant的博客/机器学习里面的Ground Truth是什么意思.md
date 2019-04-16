# 机器学习里面的Ground Truth是什么意思 - 一个跳popping的quant的博客 - CSDN博客





2018年05月28日 18:18:04[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：15818








在看英文文献的时候，经常会看到Ground Truth这个词汇，翻译的意思是地面实况，放到机器学习里面，再抽象点可以把它理解为真值、真实的有效值或者是**标准的答案**。

维基百科对Ground Truth在机器学习领域的解释是：

**在机器学习中，“ground truth”一词指的是训练集对监督学习技术的分类的准确性。这在统计模型中被用来证明或否定研究假设。“ground truth”这个术语指的是为这个测试收集适当的目标（可证明的）数据的过程。**



今天在看《Outlier Analysis》时，有句话: **However, it is generally much harder to reduce bias in outlier ensembles because of the absence of ground truth.**

放到对异常点的检测方面来理解这个ground truth就好理解了。在对异常点进行检测的时候，通过一些ensemble methods可以在一定程度上提高准确性，从而减少bias，但是由于对于异常点的定义本身就是一个问题，所以在对这些数据进行label的过程中，保证labeled data是正确的异常点也是个问题。

再举个例子，在图像识别中，一张图片是猫还是狗这个没有什么争议性，但是如果在时间序列中让你指出什么样的数据是 normal，什么样的数据是 abnormal，100个人可能会有100种回答，因为 normal 和 abnormal 之间没有什么明确的界限，所以在研究时间序列中的 outlier analysis 时，Ground Truth 也是一个不可避免的问题。



