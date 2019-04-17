# 【机器学习算法笔记】5. 自组织映射SOM - tostq的专栏 - CSDN博客





2017年03月29日 10:14:38[tostq](https://me.csdn.net/tostq)阅读数：2390
所属专栏：[机器学习算法笔记](https://blog.csdn.net/column/details/15045.html)









# 【机器学习算法笔记】5. 自组织映射SOM

自组织映射是一类非监督学习算法 

自组织原则：
- 1、自增强：如果两个神经元是同时激活的，则突触强度会选择性地增强；如果是异步激活的，突触强度会减弱
- 2、竞争原则：可用资源的局限使得最强健增长的突触是以其他神经元作为代价的
- 3、协作：在神经元级别中，对突触权值的修改趋于互相合作。
- 4、结构化信息：在一个输入信号中存在的潜在次序和结构代表了冗余信息，其通过自组织系统以知识的形式获得

## 5.1 Hebb神经元

**Hebb算法核心思想**是，当两个神经元同时处于激发状态时两者间的连接权会被加强，否则被减弱。 

根据假设：当突触信号x同后突触信号y一致时，突触权值会增强： 
![这里写图片描述](https://img-blog.csdn.net/20170329100909787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了防止权重无限增大，需要归一化： 
![这里写图片描述](https://img-blog.csdn.net/20170329100921729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Hebb神经元的结构： 
![这里写图片描述](https://img-blog.csdn.net/20170329100940995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Hebb神经元能形成关于输入分布第一个主分量的滤波器。
## 5.2 自组织映射SOM

自组织映射源于竞争学习，利用到了Hebb神经元结构。典型的SOM结构： 
![这里写图片描述](https://img-blog.csdn.net/20170329100959272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 5.2.1 竞争学习算法步骤：

1、输入向量x，同各神经元权重都归一化 

2、每次输入都有一个获胜神经元（输入向量与神经元权重向量的内积最大） 

3、获胜神经元输出1，其余输出0，只有获胜神经元才能调整权重 
![这里写图片描述](https://img-blog.csdn.net/20170329101023678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**结果：**使得获胜神经元调整权重Wj，使其进一步向当前输入向量X靠近，当下次出现与X相像的输入模式时，上次的神经元更容易获胜。在反复的竞争学习中，竞争层的各神经细胞对应的权值逐渐被调整为输入样本空间的聚类中心。 

SOM只有两层：输入层和竞争层，但竞争层神经元的排列有多种形式：一维线阵、二维平面、三维栅格等等。

### 5.2.2 自组织映射网（SOFM）

SOM分为训练阶段和工作阶段： 

1、训练阶段，权向量被训练为输入样本空间的聚类中心。 

2、在工作阶段，当输入向量与某个竞争层的内星权值相似时，自然会被分到对应的聚类上去。 

因此SOFM可用作模式分类器。注意当输入模式在训练集中从未出现过时，SOFM网只能将它归入最接近的模式分类中去。
**1. 权值初始化问题：**

初始权向量也不能完全均匀地随机分布，而应该与输入样本的大致区域充分重合。 

一种简单易行的方法是从训练集中随机抽取m个输入样本作为初始权值。 

另一种方法是先计算全体输入向量的质心，再在质心的基础上叠加小随机值作为初始权向量。
**2. 权值调整方法：**

在胜者为王基础上改进的，即优胜领域内的神经元都可以调整权值。理论上应该是离胜者越近，学习率的越大，但是为简化计算，实际中优胜领域内一般取相同的学习率。优胜领域开始定的很大，随着训练次数的增加，最终应该收缩到0。

**3. 优胜领域的设计：**

优胜领域的设计原则是不断缩小，通常凭借经验，下面给出两种计算方法： 
![这里写图片描述](https://img-blog.csdn.net/20170329101211827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

C1是与竞争层神经元个数m有关的正常数，B1是大于1的常数，tm为预选定的最大训练次数。
**4.学习率的设计：**

η(t)在训练开始时取很大，之后快速下降，这样有利于快速捕捉到输入向量的大致结构。然后η(t)又在较小的值上缓降至趋于0的值，这样可以精细地调整权值，使之符合输入空间的样本分布结构。比如可用下式 
![这里写图片描述](https://img-blog.csdn.net/20170329101233945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

或者就让η(t)随时间线性下降至0 
![这里写图片描述](https://img-blog.csdn.net/20170329101248875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**5. 输出层的设计：**

在未知样本类别数目的情况下，输出层细胞数目设计偏多偏少都不好。在这种情况下，宁可先设计较多的输出神经元，以便地映射样本的拓扑结构，如果分类过细再酌情减少输出神经元。输出神经元过多带来的“死神经元”问题（在训练过程中某神经元从未获胜过且远离其他获胜神经元，因而权值从未调整过）一般可通过重新初始化权值得到解决。 

输出神经元的排列结构一般要反应实际问题的物理意义。例如对于旅行商问题，二维平面比较直观；对于一般分类问题，一维线阵意义明确且结构简章；对于机械手臂控制问题，用三维栅格更能反应其空间轨迹特征。
### 5.2.3 SOFM的作用
- 1、保序映射。即将输入空间的样本模式类有序地映射在输出层上。
- 2、数据压缩。即将高维空间样本在保持拓扑结构不变的情况下映射到低维空间。SOFM在这方面有明显优势。无论输入空间样本有多少维，都可以在SOFM输出层的某个区域得到响应。
- 3、特征提取。高维空间的向量经过特征提取后可以在低维特征空间得到更清晰的表达，因此映射不仅是单纯的数据压缩，更是一种规律的发现。











