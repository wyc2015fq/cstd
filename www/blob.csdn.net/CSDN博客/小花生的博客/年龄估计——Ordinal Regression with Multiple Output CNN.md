
# 年龄估计——Ordinal Regression with Multiple Output CNN - 小花生的博客 - CSDN博客


2018年08月06日 20:12:44[Peanut_范](https://me.csdn.net/u013841196)阅读数：226


《Ordinal Regression with Multiple Output CNN for Age Estimation》
CVPR2016，Zhenxing Niu et al
**将传统的年龄回归问题转化为一系列二分类的子问题。**
**网络结构：**
![这里写图片描述](https://img-blog.csdn.net/20180806200429919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806200429919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
经过三个卷积层后，紧接着一个全连接层，然后输出k-1个通道的二分类任务。
把k个等级的顺序回归问题同，转换成k-1个二分类的子问题，对于每一个rank rk，二分类是去构建是否样本yi > rk，预测结果为{0，1}，满足为1，不满足为0。
![](https://img-blog.csdn.net/20180806200457232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180806200457232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
应用到年龄识别这个特定问题上就是：输出yi的年龄值原本为1-100岁，现在将这个问题转化为100个子问题，判断该样本图片预测年龄是否大于rank {1, 2, 3, …, 100}岁，若大于rk，则该子问题输出为1，否则为0。最后根据所有的100个子问题的输出结果计算预测年龄，
最后的预测年龄为将所有的预测结果求和再加1。
![](https://img-blog.csdn.net/20180806200524626?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180806200524626?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**损失函数：**
![](https://img-blog.csdn.net/20180806200549829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180806200549829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
采用cross-entropy函数作为损失函数，但对于多任务而言，每个任务都有权值系数，可以根据每个年龄数据量在总数据量中的比重。
[
](https://img-blog.csdn.net/20180806200549829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)同时，提出了亚洲人脸数据集，Asian Face Age Dataset (AFAD)
该数据集主要从人人网收集。包含了164432 的人脸图片，并且带有准确的年龄标签。其中，100752 为男性，63680为女性，年龄范围为15-40。
**各阶段男女比例的分布如下面曲线图所示：**
![这里写图片描述](https://img-blog.csdn.net/20180806200616550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180806200616550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考：[http://afad-dataset.github.io/](http://afad-dataset.github.io/)
[
](https://img-blog.csdn.net/20180806200616550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

###### 注：博众家之所长，集群英之荟萃。
[
						](https://img-blog.csdn.net/20180806200616550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180806200616550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
