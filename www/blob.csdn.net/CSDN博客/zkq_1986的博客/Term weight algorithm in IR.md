# Term weight algorithm in IR - zkq_1986的博客 - CSDN博客





2016年08月12日 11:36:50[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：486








## 1 TF-IDF

## 2 BM25

f是TD-IDF中的TF，|D|是文档D的长度，avgdl是语料库全部文档的平均长度。k1和b是参数。usually chosen, in absence of an advanced optimization, as $k_1 \in [1.2, 2.0]$   and b = 0.75 。 
![这里写图片描述](https://img-blog.csdn.net/20160815200257046)
![这里写图片描述](https://img-blog.csdn.net/20160815200245971)
### b的相关性

令： y=1-b+b*x， x表示|D|/avgdl, x与y的关系如上图。 

b越大，文档长度对相关性得分的影响越大，反之越小。b越大时，当文档长度大于平均长度，那么相关性得分越小；反之越大。 

这可以理解为，当文档较长时，包含qi的机会越大，因此，同等fi的情况下，长文档与qi的相关性应该比短文档与qi的相关性弱。 
![这里写图片描述](https://img-blog.csdn.net/20160815200229221)
### K的相关性

令： y=(tf*(k+1))./(tf+k)， k与y的关系如下图。 
![这里写图片描述](https://img-blog.csdn.net/20160815200532378)

从图表明, k对相似度的影响不大。
## 3 DFR(divergence form randomness)

### Basic Randomness Models

The DFR models are based on this simple idea: “The more the **divergence** of the within-document term-frequency from its frequency within the collection, the more the information carried by the word t in the document d”. In other words the term-weight is inversely related to the probability of term-frequency within the document d obtained by a model M of randomness:



$\displaystyle \textrm{weight} (t\vert d) \propto -\log \textrm{Prob}_{M} \left (t\in d\vert\textrm{Collection}\right)$

  (8) 

where the subscript M stands for the type of model of randomness employed to compute the probability.  The basic models are derived in the following table.|Basic DFR Models| |
|----|----|
|D|Divergence approximation of the binomial|
|[P](javadoc/org/terrier/matching/models/basicmodel/P.html)|Approximation of the binomial|
|[BE](javadoc/org/terrier/matching/models/basicmodel/B.html)|Bose-Einstein distribution|
|G|Geometric approximation of the Bose-Einstein|
|[I(n)](javadoc/org/terrier/matching/models/basicmodel/In.html)|Inverse Document Frequency model|
|[I(F)](javadoc/org/terrier/matching/models/basicmodel/IF.html)|Inverse Term Frequency model|
|[I(ne)](javadoc/org/terrier/matching/models/basicmodel/In_exp.html)|Inverse Expected Document Frequency model|

If the model M is the binomial distribution, then the basic model is P and computes the value:



$\displaystyle -\log \textrm{Prob}_{P}(t\in d\vert \textrm{Collection})=-\log \left(\begin {array}{c} TF\ tf \end{array}\right )p^{tf}q^{TF -tf}$

 where:
- TF is the term-frequency of the term t in the Collection 
- tf is the term-frequency of the term t in the document d 
- N is the number of documents in the Collection 
- p is 1/N and q=1-p

Similarly, if the model M is the geometric distribution, then the basic model is G and computes the value: 


$-log Prob_G(t\in d|Collection)=-log((\frac{1}{1+\lambda})(\frac{\lambda}{1+\lambda}))$

where λ = F/N.







