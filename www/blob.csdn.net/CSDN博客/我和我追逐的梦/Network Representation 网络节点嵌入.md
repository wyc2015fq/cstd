
# Network Representation 网络节点嵌入 - 我和我追逐的梦~~~ - CSDN博客


置顶2016年08月10日 21:21:07[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：13847标签：[NRL																](https://so.csdn.net/so/search/s.do?q=NRL&t=blog)[网络表征																](https://so.csdn.net/so/search/s.do?q=网络表征&t=blog)[网络嵌入																](https://so.csdn.net/so/search/s.do?q=网络嵌入&t=blog)[网络节点向量																](https://so.csdn.net/so/search/s.do?q=网络节点向量&t=blog)[Embedding																](https://so.csdn.net/so/search/s.do?q=Embedding&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=网络节点向量&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=网络嵌入&t=blog)个人分类：[数据挖掘与机器学习																](https://blog.csdn.net/heyongluoyao8/article/category/2220409)
[
																								](https://so.csdn.net/so/search/s.do?q=网络嵌入&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=网络表征&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=网络表征&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=NRL&t=blog)

### Network Representation
最近看了几篇网络节点embedding的论文，思想很新颖，很有趣，这里分享给大家。
Network Representation可以翻译为网络(节点)表征、网络(节点)向量、网络(节点)嵌入等…
Aim to learn distributed vector representation for each vertex in a network.
目的是将学习图的每个节点的向量，即embedding，类似word2vec.
一旦将节点表示成向量了之后，很多问题都变得简单了，如分类、聚类、半监督学习、标签传播、图分割等等，关键就看效果了。
[DeepWalk: Online Learning of Social Representations](http://www.perozzi.net/projects/deepwalk/)
在图上随机游走产生长度为2w+1的路径，对每个点随机\gamma个随机游走序列
每一条随机游走路径便是相当于一个序列(相当于一句话,idea参考word2vec)，这样序列中的点就有上下文，定义一个时间窗口w，并进行马尔可夫假设，最后使用word2vec中的Skip-Gram训练每一个节点的向量
假设一个路径序列为S=\{v_1,...,v_{|S|}\},对于v_i \in S,其上下文为C=\{v_{i-w},v_{i-w+1},...,v_{i+w-1},v_{i+w}\}, 那么DeepWalk的优化目标为:
f=\frac{1}{|S|}\sum^{|S|}_{i=1}\sum_{-w \le j \le w, j \ne 0}log p(v_{i+j}|v_i)
其中,
p(v_{j}|v_i)=\frac{exp(c^T_{v_j}r_{v_i})}{\sum_{v \in C}exp(c^T_{v}r_{v_i})}
r_{v_i}是点v_i的向量表征,c_{v_j}是点v_i上下文中点{v_j}的向量表征
DeepWalk使目标f最大化，使用Skip-Gram与Hierarchical Softmax进行训练得到每个点的vector
DeepWalk等价于MF(matrix factorization,矩阵分解)
[源码](https://github.com/phanein/deepwalk)
NRL: Network Representation Learning with Rich Text Information
DeepWalk <=> MF
对于图G(V,E)
设M是图的邻接矩阵
设D是随机游走序列集，里面每一个元素都是(v,c)对,V是点集,V_C上下文点集,一般而言|V|=|V_C|
对每一个点v \in V, 将其embedding成一个k维向量r_v \in \Re^{k}, k \ll |V|
对每一个上下文中的点v \in V_C, 也将其embeddind成一个k维向量c_v \in \Re^{k}, k \ll |V|
设W=\Re^{k \times |V|}, H=\Re^{k \times |V_c|}, 那么需要证明M=W^TH
对于每一个(v,c)对,定义:
N(v,c)=count((v,c) \in D)
N(v)=\sum_{c^{'} \in V_C} N(v,c^{'})
N(c)=\sum_{v^{'} \in V} N(v^{'},c)
Levy and Goldberg已经证明在使用负采样(Negative Sampling)Skip-Gram中，词上下文(word-context)矩阵M如果足够大，那么M中的每一个元素：
M_{ij}=log\frac{N(v_i,c_j) \cdot |D|}{N(v_i) \cdot N(c_j)} - logn
其中,n是每一个词的上下文负采样对的个数。
同理，对于softmax的Skip-Gram:
M_{ij}=log\frac{N(v_i,c_j)}{N(v_i)}
定义转移矩阵A, A_{ij}=\frac{1}{d_i} if e(i,j) \in E, else:A_{ij}=0
定义行向量e_i=\{0,1\}^{|V|},e_{ii}=1, else: 0
因此,e_iA的第j个元素表示点i转移到点j的概率,e_iA^t的第j个元素表示点i经过t步转移到点j的概率
因此,[e_i(A+A^2+A^3+...+A^t)]_j是点v_j出现在点v_i窗口为t下文(右边)序列中的概率
因此:
\frac{N(v_i,v_j)}{N(v_i)}=\frac{[e_i(A+A^2+A^3+...+A^t)]_j}{t}
详细证明可以参见原文。
本文方法
Low-rank MF
M \in \Re^{b \times d}, \Omega=\{(i,j)|i \in [0,b-1],j \in [0,d-1]\}, W \in \Re^{k \times b}, H \in \Re^{k \times d}
minimize:
min_{W,H} \sum_{(i,j) \in \Omega}(M_{ij}-(W^TH)_{ij})^2+\frac{\lambda}{2}(||W||_F^2+||H||_F^2)
||\cdot||_F是Frobenius norm
Low-rank MF即对一个低秩的M进行分解
假设我们有额外的两个特征矩阵X \in \Re^{f_x \times b}, Y \in \Re^{f_y \times d},他们每一列表示一个对象有f_x, f_y维特征,那么minimize:
min_{W,H} \sum_{(i,j) \in \Omega}(M_{ij}-(X^TW^THY)_{ij})^2+\frac{\lambda}{2}(||W||_F^2+||H||_F^2)
具体参见:[Inductive matrix completion for predicting gene-disease associations](http://bigdata.ices.utexas.edu/project/gene-disease/)
Text-Associated DeepWalk(TADW)
Text feature matrix
T \in \Re^{f_t \times |V|}
Speed and accuracy tradeoff
M=\frac{(A+A^2)}{2}
minimize
W \in \Re^{k \times |V|}, H \in \Re^{k \times f_t}
min_{W,H} \sum_{(i,j) \in \Omega}(M_{ij}-(W^THT)_{ij})^2+\frac{\lambda}{2}(||W||_F^2+||H||_F^2)
矩阵分解得到W,H. 即Low-rank MF中X为单位向量,Y=T
TADW会收敛局部最优

wiki实验
数据
graph.txt
每一行两列，分别为两个点id，表示它们之间有边连接，无向图
tfidf.txt
每一行三列，第一列是doc的id，第二列是词的id，第三列是该词的TFIDF值
group.txt
每一行两列，第一列是doc的id，第二列是该doc所属的label的id
超参数
W与H矩阵的维度k=80
正则化参数lambda=0.2
text Feature维度f_t=200
train ratid of svm:0.1
label数目15
步骤
设count(d)=n=|V|,count(word)=m
使用tfidf.txt构建稀疏矩阵TFIDF,对TFIDF进行SVD:
TFIDF \in \Re^{n \times m}
TFIDF=U*S*V
U \in \Re^{n \times f_t}, S \in \Re^{f_t \times f_t},V \in \Re^{f_t \times m}
T=U*S \in \Re^{|V| \times f_t}
这样便得到了图中每个节点的Text feature矩阵，并对T列向量进行单位向量化
构造转移矩阵A的稀疏矩阵
A_{ij}=\frac{1}{d_i} if e_{ij} \in E, else: 0
构造M矩阵
M=\frac{(A+A^2)}{2}
train_mf
对M矩阵进行分解得到W与H
直接调用[“Inductive matrix completion for predicting gene-disease associations”的矩阵分解](http://bigdata.ices.utexas.edu/project/gene-disease/), X为单位向量,Y=T
SVM分类
每篇文档的向量是W^T的行向量与T*H^T的行向量拼接而成,使用[LibLinear中的linearsvm](http://www.csie.ntu.edu.tw/~cjlin/liblinear/.)

LINE: Large-scale information network embedding
[源码](https://github.com/tangjianpku/LINE)
待补充
[LSHM: Learning latent representations of nodes for classifying in heterogeneous social networks](https://www.researchgate.net/publication/260433586_Learning_latent_representations_of_nodes_for_classifying_in_heterogeneous_social_networksø)
异构网络(heterogeneous) VS 同构网络(Homogeneous)
如果网络中的节点的类型都一样，即每个节点的标签集合是一样的，那么该网络是同构网络，如facebook上的好友网络，微博上的粉丝网络等。
否则，便是异构网络，如作者-论文引用网络，作者的标签是研究领域，论文的标签是其所发表的会议或者期刊。又如用户－物品网络等。Flickr数据集：用户、照片、评论、tag、朋友关系、合作关系等。DBLP数据集：作者、论文、地点、引用关系、合作关系等。
假设
不同类型的节点会互相影响
半监督学习算法
算法优势
将点的表征映射到低维隐空间中，该映射考虑了节点标签之间的依赖、标签之间的关系、以及点的特征
能够对不同类型的节点进行分类
前提
图G(V,E)有是一个无向图(带权值或者不带权值)
图G(V,E)有\tau=1,2,...,T种类型的节点,N个节点,v_i的类型为t_i \in \tau, 节点类型为t的标签集为Y^t
w_{ij} \in E为节点i,j之间的边
已有标签节点集合为(x_1,...,x_l),个数为l, l<N,标签集合为C^{t_i}, i \in [1,l],N-l未标注的节点
如果x_i的标签为y_i
同构网络学习模型(T=1)
(\hat{y_1},...,\hat{y_N})=argmin_{\tilde{y_1},...,\tilde{y_N}}\sum_{i=1}^{l}\Delta(\tilde{y_i},y_i)+\lambda\sum_{i,j}w_{i,j}||\tilde{y_i}-\tilde{y_j}||^2
可以使用随机梯度下降、坐标下降法、随机游走等算法来计算模型参数。
异构网络
不同类型的节点不具有传播性
将异构网络分拆多个同够网络会导致不同类型节点之间的依赖信息丢失
异构网络处理方法
将网络中的每个节点使用一个隐空间\Re^Z中的向量来表示,每个节点的向量为z
要保证相连的节点的向量相似度高(smoothness，平滑性)
不同类型的节点使用不同的分类器进行分类，也就是有T个分类器(class separability，类别分离性)
目标函数
smoothness
\sum_{i,j/w_{i,j} \ne 0}w_{i,j}||z_i-z_j||^2
class separability
每种类型的节点都有一个分类器，设为f_{\theta}^{t}
那么损失函数使用hinge-loss：
\sum_{i=1}^{l}\Delta(f_{\theta}^{t_i}(z_i),y_i)=\sum_{k=1}max\{0,1-y^{k}_if_{\theta}^{t_i,k}(z_i))\}
其中,y_i^k为节点i的真实标签向量，即如果节点i的标签为j，那么y_i^j=1,其它分量为-1.f_{\theta}^{t_i,k}(z_i))为其分类器预测向量
优化目标,minimize:
L(z,\theta)=\sum_{i=1}^{l}\Delta(f^{t_i}_{\theta}(z_i),y_i)+\lambda \sum_{i,j/w_{i,j} \ne 0}w_{i,j}||z_i-z_j||^2
参数: 每个节点的向量z_i,分类器的参数\theta
随机梯度下降算法
Input(G,I,\theta,\lambda,\epsilon)//I为最大迭代次数
随机初始化每个节点的向量z
i:=0
while(i<I):
随机选择w_{i,j} \gt 0的一对(x_i,x_j)
ifi \le l: // 节点i是带标签的点
\theta \gets \theta + \epsilon\nabla_{\theta}\Delta(f_{\theta}^{t_i}(z_i),y_i)
z_i \gets z_i + \epsilon\nabla_{z_i}\Delta(f_{\theta}^{t_i}(z_i),y_i)
end if
ifj \le l: // 节点j是带标签的点
\theta \gets \theta + \epsilon\nabla_{\theta}\Delta(f_{\theta}^{t_j}(z_j),y_j)
z_i \gets z_j + \epsilon\nabla_{z_j}\Delta(f_{\theta}^{t_j}(z_j),y_j)
end if
z_i \gets z_i + \epsilon\lambda\nabla_{z_i}||z_i-z_j||^2
z_j \gets z_j + \epsilon\lambda\nabla_{z_j}||z_i-z_j||^2
i:=i+1
end while
Output(z_i,y_i)


