# 【Graph Embedding】LINE：算法原理，实现和应用 - 浅梦的博客 - CSDN博客





2019年02月12日 10:26:03[浅梦s](https://me.csdn.net/u012151283)阅读数：525
个人分类：[图表示学习																[深度学习](https://blog.csdn.net/u012151283/article/category/6698461)](https://blog.csdn.net/u012151283/article/category/8660483)








本文首发于知乎专栏([https://zhuanlan.zhihu.com/p/56478167](https://zhuanlan.zhihu.com/p/56478167)

)，欢迎关注 ~

之前介绍过DeepWalk([DeepWalk：算法原理，实现和应用](https://zhuanlan.zhihu.com/p/56380812))，DeepWalk使用DFS随机游走在图中进行节点采样，使用word2vec在采样的序列学习图中节点的向量表示。

LINE也是一种基于邻域相似假设的方法，只不过与DeepWalk使用DFS构造邻域不同的是，LINE可以看作是一种使用BFS构造邻域的算法。此外，LINE还可以应用在带权图中(DeepWalk仅能用于无权图)。

之前还提到不同的graph embedding方法的一个主要区别是对**图中顶点之间的相似度的定义**不同，所以先看一下LINE对于相似度的定义。

# LINE 算法原理

## 一种新的相似度定义

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190211160744766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)

### first-order proximity

1阶相似度用于描述图中成对顶点之间的局部相似度，形式化描述为若$u$,$v$之间存在直连边，则边权$w_{uv}$即为两个顶点的相似度，若不存在直连边，则1阶相似度为0。

如上图，6和7之间存在直连边，且边权较大，则认为两者相似且1阶相似度较高，而5和6之间不存在直连边，则两者间1阶相似度为0。

### second-order proximity

仅有1阶相似度就够了吗？显然不够，如上图，虽然5和6之间不存在直连边，但是他们有很多相同的邻居顶点(1,2,3,4)，这其实也可以表明5和6是相似的，而2阶相似度就是用来描述这种关系的。

形式化定义为

令$p_u=(w_{u,1},...,w_{u,|V|})$表示顶点$u$与所有其他顶点间的1阶相似度，则$u$与$v$的2阶相似度可以通过$p_u$和$p_v$的相似度表示。若$u$与$v$之间不存在相同的邻居顶点，则2阶相似度为0。
## 优化目标

### 1st

对于每一条无向边$(i,j)$，定义顶点$v_i$和$v_j$之间的联合概率为：

$p_1(v_i,v_j)=\frac{1}{1+\exp(-\vec{u}^T_i \cdot \vec{u}_j)}$，$\vec{u_i}$为顶点$v_i$的低维向量表示。（可以看作一个内积模型，计算两个item之间的匹配程度）

同时定义经验分布$\hat{p_1}=\frac{w_{ij}}{W}$,$W=\sum_{(i,j)\in E} w_{ij}$

优化目标为最小化：$O_1=d(\hat{p}_1(\cdot,\cdot),p_1(\cdot,\cdot))$

$d(\cdot,\cdot)$是两个分布的距离，常用的衡量两个概率分布差异的指标为KL散度，使用KL散度并忽略常数项后有

$O_1=-\sum_{(i,j)\in E}w_{ij}\log{p_1(v_i,v_j)}$

**1st order 相似度只能用于无向图当中。**

### 2nd

这里对于每个顶点维护两个embedding向量，一个是该顶点本身的表示向量，一个是该点作为其他顶点的上下文顶点时的表示向量。

对于有向边$(i,j)$，定义给定顶点$v_i$条件下，产生上下文(邻居)顶点$v_j$的概率为
$p_2(v_j|v_i)=\frac{\exp(\vec{c}^T_j\cdot\vec{u}_i)}{\sum_{k=1}^{|V|}\exp(\vec{c}^T_k\cdot\vec{u}_i)}$，其中$|V|$为上下文顶点的个数。

优化目标为
$O_2=\sum_{i\in V}\lambda_i d(\hat{p}_2(\cdot|v_i),p_2(\cdot|v_i))$，其中$\lambda_i$为控制节点重要性的因子，可以通过顶点的度数或者PageRank等方法估计得到。

经验分布定义为：$\hat{p}_2(v_j|v_i)=\frac{w_{ij}}{d_i}$,$w_{ij}$是边$(i,j)$的边权，$d_i$是顶点$v_i$的出度，对于带权图，$d_i=\sum_{k\in N(i)}W_{ik}$

使用KL散度并设$\lambda_i = d_i$，忽略常数项，有
$O_2=-\sum_{(i,j)\in E} w_{ij}\log{p_2(v_j|v_i)}$

## 优化技巧

### Negative sampling

由于计算2阶相似度时，softmax函数的分母计算需要遍历所有顶点，这是非常低效的，论文采用了负采样优化的技巧，目标函数变为：

$\log{\sigma({\vec{c}^T_j\cdot\vec{u}_i})}+\sum_{i=1}^K{E_{v_n\sim P_n(v)}[-\log{\sigma({\vec{c}^T_n\cdot\vec{u}_i})}]}$，K是负边的个数。

论文使用$P_n(v)\propto d_v^{3/4}$，$d_v$是顶点$v$的出度

### Edge Sampling

注意到我们的目标函数在log之前还有一个权重系数$w_{ij}$，在使用梯度下降方法优化参数时，$w_{ij}$会直接乘在梯度上。如果图中的边权方差很大，则很难选择一个合适的学习率。若使用较大的学习率那么对于较大的边权可能会引起梯度爆炸，较小的学习率对于较小的边权则会导致梯度过小。

对于上述问题，如果所有边权相同，那么选择一个合适的学习率会变得容易。这里采用了将带权边拆分为等权边的一种方法，假如一个权重为$w$的边，则拆分后为$w$个权重为1的边。这样可以解决学习率选择的问题，但是由于边数的增长，存储的需求也会增加。

另一种方法则是从原始的带权边中进行采样，每条边被采样的概率正比于原始图中边的权重，这样既解决了学习率的问题，又没有带来过多的存储开销。

这里的采样算法使用的是**Alias**算法，Alias是一种$O(1)$时间复杂度的离散事件抽样算法。具体内容可以参考 [Alias Method:时间复杂度O(1)的离散采样方法](https://zhuanlan.zhihu.com/p/54867139)

## 其他问题

### 低度数顶点

对于一些顶点由于其邻接点非常少会导致embedding向量的学习不充分，论文提到可以利用邻居的邻居构造样本进行学习，这里也暴露出LINE方法仅考虑一阶和二阶相似性，对高阶信息的利用不足。

### 新加入顶点

对于新加入图的顶点$v_i$，若该顶点与图中顶点存在边相连，我们只需要固定模型的其他参数，优化如下两个目标之一即可：

$-\sum_{j\in N(i)} w_{ji}\log{p_1(v_j,v_i)}$或$-\sum_{j\in N(i)} w_{ji}\log{p_1(v_j|v_i)}$

若不存在边相连，则需要利用一些side info，留到后续工作研究。

# LINE核心代码

## 模型和损失函数定义

LINE使用梯度下降的方法进行优化，直接使用tensorflow进行实现，就可以不用人工写参数更新的逻辑了~

我的实现中把1阶和2节的方法融合到一起了，可以通过超参数`order`控制是分开优化还是联合优化，论文推荐分开优化。

首先输入就是两个顶点的编号，然后分别拿到各自对应的embedding向量，最后输出内积的结果。

真实`label`定义为1或者-1，通过模型输出的内积和`line_loss`就可以优化使用了负采样技巧的目标函数了~
```python
def line_loss(y_true, y_pred):
    return -K.mean(K.log(K.sigmoid(y_true*y_pred)))

def create_model(numNodes, embedding_size, order='second'):

    v_i = Input(shape=(1,))
    v_j = Input(shape=(1,))

    first_emb = Embedding(numNodes, embedding_size, name='first_emb')
    second_emb = Embedding(numNodes, embedding_size, name='second_emb')
    context_emb = Embedding(numNodes, embedding_size, name='context_emb')

    v_i_emb = first_emb(v_i)
    v_j_emb = first_emb(v_j)

    v_i_emb_second = second_emb(v_i)
    v_j_context_emb = context_emb(v_j)

    first = Lambda(lambda x: tf.reduce_sum(
        x[0]*x[1], axis=-1, keep_dims=False), name='first_order')([v_i_emb, v_j_emb])
    second = Lambda(lambda x: tf.reduce_sum(
        x[0]*x[1], axis=-1, keep_dims=False), name='second_order')([v_i_emb_second, v_j_context_emb])

    if order == 'first':
        output_list = [first]
    elif order == 'second':
        output_list = [second]
    else:
        output_list = [first, second]

    model = Model(inputs=[v_i, v_j], outputs=output_list)
```

## 顶点负采样和边采样

下面的函数功能是创建顶点负采样和边采样需要的采样表。中规中矩，主要就是做一些预处理，然后创建alias算法需要的两个表。

```python
def _gen_sampling_table(self):
    # create sampling table for vertex   
    power = 0.75    
    numNodes = self.node_size    
    node_degree = np.zeros(numNodes)  # out degree    
    node2idx = self.node2idx
    for edge in self.graph.edges():        
        node_degree[node2idx[edge[0]]]  += self.graph[edge[0]][edge[1]].get('weight', 1.0)
    total_sum = sum([math.pow(node_degree[i], power) for i in range(numNodes)])
    norm_prob = [float(math.pow(node_degree[j], power)) /  total_sum for j in range(numNodes)]
    self.node_accept, self.node_alias = create_alias_table(norm_prob)
    # create sampling table for edge    numEdges = self.graph.number_of_edges()    
    total_sum = sum([self.graph[edge[0]][edge[1]].get('weight', 1.0) for edge in self.graph.edges()])    
    norm_prob = [self.graph[edge[0]][edge[1]].get('weight', 1.0) * numEdges / total_sum for edge in self.graph.edges()] 
    self.edge_accept, self.edge_alias = create_alias_table(norm_prob)
```

# LINE应用

和之前一样，还是用LINE在wiki数据集上进行节点分类任务和可视化任务。 wiki数据集包含 2,405 个网页和17,981条网页之间的链接关系，以及每个网页的所属类别。

由于1阶相似度仅能应用于无向图中，所以本例中仅使用2阶相似度。

本例中的训练，评测和可视化的完整代码在下面的git仓库中，
[https://github.com/shenweichen/GraphEmbedding](https://github.com/shenweichen/GraphEmbedding)

```python
G = nx.read_edgelist('../data/wiki/Wiki_edgelist.txt',create_using=nx.DiGraph(),nodetype=None,data=[('weight',int)])

model = LINE(G,embedding_size=128,order='second')
model.train(batch_size=1024,epochs=50,verbose=2)
embeddings = model.get_embeddings()

evaluate_embeddings(embeddings)
plot_embeddings(embeddings)
```

## 分类任务结果
|micro-F1|macro-F1|
|----|----|
|0.6403|0.5286|

结果有一定随机性，可以多运行几次，或者稍微调整epoch个数。

## 可视化结果

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190211201940642.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)






