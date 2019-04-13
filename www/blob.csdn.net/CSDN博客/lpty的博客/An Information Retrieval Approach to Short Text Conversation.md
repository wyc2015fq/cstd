
# An Information Retrieval Approach to Short Text Conversation - lpty的博客 - CSDN博客

2019年02月01日 17:10:34[lpty](https://me.csdn.net/sinat_33741547)阅读数：89标签：[QA																](https://so.csdn.net/so/search/s.do?q=QA&t=blog)[Retrieval																](https://so.csdn.net/so/search/s.do?q=Retrieval&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=QA&t=blog)个人分类：[论文阅读																](https://blog.csdn.net/sinat_33741547/article/category/7727913)



# 引言
Hang Li 等发在2014年arxiv上的文章，原文地址：[https://arxiv.org/pdf/1408.6988.pdf](https://arxiv.org/pdf/1408.6988.pdf)
基于新浪微博的数据集做的短文本单轮QA，从论文名可以看到，这是基于检索的方式做的，作者称之为short text conversation (STC)。大概流程是先做Retrieval召回candidate pairs，之后对待定数据做人工标注，再对candidate pairs进行特征提取，最终用LTR的方式进行排序。
# 数据集
数据集是从新浪微博爬取的微博及其下的评论构成(p, r)这样的pairs，选区的是一些中国搞NLP的高级知识分子的微博，相对来说posts的质量较高。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201162612158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Sampling Strategy
确定10个在sina微博上活跃的NLP大牛，然后爬他们的followee，得到3200多个NLPer/MLer作为种子。
之后基于上述种子爬了两个月，抓取他们的微博及相关评论。统计出来数据的topic主要为：Research、General Arts and Science、IT Technology、Life等
### Processing, Filtering, and Data Cleaning
接下来对数据进行清洗，主要有以下几个策略：
1、去除post小于10个字符，及response小于5个字符的，还有一些万金油式的" Wow"或者“Nice”之类的语句。
2、只保留每个post前100个response，因为在时序上，前面的response会和post联系更紧密些。
3、去除一些可能是广告的response。
4、去除标点符号和表情，同时分词。
### Labeled Post-Comment Pairs
对上述的pairs，用三个基础的检索模型进行召回，每个召回10条，所以总共有<=30个pairs，之后对这个candidate pairs进行人工标注，策略如下：
1、Semantic Relevance
在语义上，post和response必须是相关的，如：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201164252563.png)
2、Logic Consistency
在逻辑上，必须无误的，比如：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201164314383.png)
3、Speech Act Agreement
同时，response和post主题之间的切合度也很重要，如：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019020116441616.png)
根据上述3种策略，对pairs进行标注，最终得到的数据集如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201164544246.png)
需要注意的是，label pairs里面是不包含原本的response的，因为时靠检索寻找到的待定pairs。
[
](https://img-blog.csdnimg.cn/20190201164544246.png)
# 模型
[
](https://img-blog.csdnimg.cn/20190201164544246.png)上面说道，整体模型分为三个部分，Retrieval，Match，Rank如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201164854673.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Retrieval
检索部分即使上面构造candidate pairs的方法，作者用了以下方式：
##### Query-Response Similarity
Query和Response之间的consine相似度，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019020116511937.png)
##### Query-Post Similarity
Query和Post之间的consine相似度，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201165201202.png)
##### Query-Response Matching in Latent Space
将Query和Response的表示降维，再计算相似度，作者提到这样的方法能够得到更多的语义信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201165510223.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201165516701.png)
### Match
召回待定的pairs后，再根据多种方式抽取其特征，其中前三种为上述Retrieval得到的相似度
##### Translation-based Language Model
为了解决lexical gap，即是两个句子之间相似的词很少但是语义比较接近的情况，提出的基于翻译模型的特征，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201165850593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
具体的推导及训练方式在原论文中有详细描述。
[
](https://img-blog.csdnimg.cn/20190201165850593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
##### Deep Matching Model
[
](https://img-blog.csdnimg.cn/20190201165850593.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)上面提到的方法都是线性模型，这里提出了Deep Match Model用来提取非线性特征：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201170058894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
##### Topic-Word Model
Topic-Word Model提到了，可能句子之间相似的词很少，但是在有先验知识的情况下，是更加符合post的response，如：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
所以这里提出了Topic-Word Model用来获取主题相关的信息。
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
##### Other Simple Matching Features
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)除上面之外，还有Longest Common String (LCS)、Co-occurrence features等简单的特征。
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Rank
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)根据上面得到的特征，使用Rank SVM进行排序，得到最后的结果。
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 实验
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)作者依据上述方式，进行实验，评价指标为Mean Average Precision (MAP)和Precision@1 (P@1)。
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Results of Basic Linear Matching Models
[
](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201170716740.png)
### Results of Combining all the Features
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
除此之外，还有一些其他case study及分析等，这里就不列举了。
[
](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 结论
[
](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)提出了一套完整的基于retrieval的QA的结构，后续很多工作都基于这个结构展开，同时数据处理的流程也很值得学习。
[
](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 引用
[
](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)1、An Information Retrieval Approach to Short Text Conversation
[
            ](https://img-blog.csdnimg.cn/20190201170744234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)

[
  ](https://img-blog.csdnimg.cn/20190201170311514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)