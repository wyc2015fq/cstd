# 独立子空间分析(Independent subspace analysis,ISA) - wydbyxr的博客 - CSDN博客
2018年09月30日 10:59:48[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：275
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 独立子空间分析(Independent subspace analysis,ISA)
  ISA 一种在ICA（Independent Component Analysis)基础上发展出来的机器学习方法。是一种无监督特征学习方法，可从图像中学习出具有相位不变的特征。
  ISA将观察数据的向量分解成相互独立的子空间来表示。从结构上来看，ISA可以被看作是一个两层网络。
## 从神经学角度理解
  ICA提取的是线性特征，线性特征面临的一个问题就是缺乏不变性。从视神经科学的角度来讲，ICA学到的特征检测器可以被看作是简单细胞，而一个复杂细胞对不同相位可以给出相同的响应，这就是所谓的相位不变性。线性系统的响应依赖于输入信号相位和检测器相位的一致性。
  ISA通过对不同线性特征检测器的结果进行池化的方式实现了一个对相位不敏感的特征表达，其学习产生的特征检测器可以看作复杂细胞。
## 具体实现
  ISA是一种无监督学习方法，具有两层网络的生成模型，可以有效模拟人类视觉系统V1区简单细胞(simple cells)与复杂细胞(complex cells)感受野的层次化响应模式。
  1）模型第一层学习线性变换的权重W（类似FC）,
  2）第二层将同一子空间元素合并，执行固定的非线性变换V（L2-pooling），得到对相位变化响应不变的特征。
  图例：
![在这里插入图片描述](https://img-blog.csdn.net/20180930105918419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180930105937222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
