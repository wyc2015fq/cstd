# 基于LVD、贝叶斯模型算法实现的电商行业商品评论与情感分析案例 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:58:22[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：400











### 文章目录
- [一、 项目需求](#__1)
- [1.1 语言处理相关库（snownlp，jieba，gensim）](#11_snownlpjiebagensim_13)
- [1.2  LDA主题模型](#12__LDA_25)
- [1.3 相关模型的比较](#13__50)
- [二、代码：](#_57)
- [2.1 导入数据—评论提取—文本去重](#21__59)
- [2.2 模型构造—情感分析—分词—去除停用](#22__130)
- [2.3 LDA主题分析](#23_LDA_167)




### 一、 项目需求

现在大家进行网购，在购物之前呢，肯定会看下相关商品的评论，看下好评和差评，然后再综合衡量，最后才会决定是否购买相关的商品。对一个指定商品，生产商，卖家，买家认同该商品的哪些优点/不认同那些。
- 评论分析：
A、电商评论的重要性 ------      消费者先看评论，然后决定是否下单

B、电商评论太多时遇到的问题--------销量多，评论数大

C、提取有价值的评论----去除灌水评论（例如：好评，东西很好；商品很赞等）- 项目的输出：
商品好评–/差评–若干个topic中，其关键词以及权重
##### 1.1 语言处理相关库（snownlp，jieba，gensim）

安装与机器学习，自然语言处理相关库,在`Anaconda Prompt` 下输入：

```python
```python
pip install snownlp              # 处理自然语言库/情感分析
pip install jieba                # 中文分词
pip install gensim               # 机器学习算法/自然语言相关的功能 

gensim.corpora.dictionary.Dictionary的函数 doc2bow(document)
document-->词组成的列表list，这个list中有些词是重复的，也缺少每个词的词频。
需要先用 doc2bow 函数将数据转换成恰当的格式。
```
```

![这里写图片描述](https://img-blog.csdn.net/20180816180511123?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.2  LDA主题模型

比方说，如果一篇文章是在讲狗的，那“狗”和“骨头”等词出现的频率会高些；讲猫的，那“猫”和“鱼”等词出现的频率会高些。在这里，”狗”就是一个topic，”猫”也是一个topic。
**词袋模型（bag of words）只考虑频数，不考虑词序，混乱的**
- **LDA (Latent Dirichlet allocation)主题模型**
1 已知条件： 某些文档的都是由那些词构成的

2 假定条件： LDA 模型假设每一篇文章都在讲述若干个主题topic，可以人为的设定k 值

3 求解：    通过LDA 模型，可以求出K个主题中，每个主题到底长成什么样子。- 能做：  求解出K个主题长什么样子
- 不能做：不能人为设定主题，不能人为设定什么是财经，体育，科技。

LDA是无监督学习，模型训练后，每个主题是什么就是什么

**LDA 思想**
- LDA是一个贝叶斯模型（Bayers model）。之前的朴素贝叶斯只是其中一个。只不过LDA更复杂些
- 在贝叶斯模型中，根据可见 —推出—> 不可见，

例如： 打喷嚏：–>(夏天)可能是呛着了

–>(冬天)可能是感冒了- LDA模型优点

1 无监督：无需劳心劳力的去对训练数据做标注

2 预处理简单：分词，去停用词

3 参数少，一般只需要设定参数K （K表示一拱多少个主题）
##### 1.3 相关模型的比较

除了LDA技术，SVD(奇异值分解)技术也可以应用于主题模型分析。
- 问题: LSI与LSA有什么区别？
技术是相同的，都是采用SVD降维，而且都是处理自然语言中的term-document矩阵。

一般LSI专指索引和搜索领域。
### 二、代码：

获取数据集博客下载地址：[https://download.csdn.net/download/wsp_1138886114/10606715](https://download.csdn.net/download/wsp_1138886114/10606715)

##### 2.1 导入数据—评论提取—文本去重

```python
```python
#导入数据-评论提取
import pandas as pd

inputfile = '.\\data\\huizong.csv'              #汇总文件
outputfile = '.\\data\\meidi_jd.txt'	        #评论提取保存路径
data = pd.read_csv(inputfile,encoding = 'utf-8')
data = data[[u'评论']][data[u'品牌'] == u'美的']
data.to_csv(outputfile,index = False,header = False,encoding = 'utf-8')

#文本去重
inputfile = '.\\data\\meidi_jd.txt'             #评论文件
outputfile = '.\\data\\meidi_jd_process_1.txt'  #评论处理后保存路径
data = pd.read_csv(inputfile, encoding = 'utf-8', header = None)
data=data.dropna()     
l1 = len(data)
data = pd.DataFrame(data[0].unique())
l2 = len(data)
data.to_csv(outputfile, index = False, header = False, encoding = 'utf-8')
print(u'删除了%s条评论。' %(l1 - l2))

#机械压缩去词
def cutword(strs,reverse=False):
        s1=[]
        s2=[]
        s=[]
        if reverse :
            strs=strs[::-1]
        s1.append(strs[0])
        for i in strs[1:]:
            if i==s1[0] :
                if len(s2)==0:
                    s2.append(i)
                else :
                    if s1==s2:
                        s2=[]
                        s2.append(i)
                    else:
                        s=s+s1+s2
                        s1=[]
                        s2=[]
                        s1.append(i)
            else :
                if s1==s2 and len(s1)>=2 and len(s2)>=2:
                    s=s+s1
                    s1=[]
                    s2=[]
                    s1.append(i)
                else:
                    if len(s2)==0:
                        s1.append(i)
                    else :
                        s2.append(i)
        if s1==s2:
            s=s+s1
        else:
            s=s+s1+s2
        if reverse :
            #print ''.join(s[::-1])
            return ''.join(s[::-1])
        else:
            #print ''.join(s)
            return ''.join(s)
			
data2 = data.iloc < [:,0].apply(cutword)
data2 = data2.apply(cutword,reverse = True)

#短句过滤
data3 = data2[data2.apply(len) >= 4]
```
```

##### 2.2 模型构造—情感分析—分词—去除停用

```python
```python
#情感分析
from snownlp import SnowNLP

coms = []
coms = data3.apply(lambda x:SnowNLP(x).sentiments)

data1 = data3[coms >= 0.5]              # 大于0.5为积极的情绪词
data2 = data3[coms < 0.5]

#分词
import jieba
mycut = lambda s: ''.join(jieba.cut(s))	# 自定义简单分词函数，分词用空格隔开
data1 = data1.apply(mycut)              # 通过广播进行分词，加快速度
data2 = data2.apply(mycut)

#outputfile1 = 'd:/data/example08/meidi_jd_pos_cut.txt'
#outputfile2 = 'd:/data/example08/meidi_jd_neg_cut.txt'
#data1.to_csv(outputfile1,index = False,header = False,encoding = 'utf-8')	#保存结果
#data2.to_csv(outputfile2,index = False,header = False,encoding = 'utf-8')	#保存结果

#去除停用（与评论无关的词）
stoplist = '.\\data\\stoplist.txt'
stop = pd.read_csv(stoplist,encoding = 'utf-8',
                   header = None,sep = 'tipdm')	    #sep设置一个不存在的分割词 tipdm

stop = [' ',''] + list(stop[0])	                    #Pandas 自动过滤空格符，这里是手动添加

pos = pd.DataFrame(data1[:5000])
neg = pd.DataFrame(data1[:5000])

neg[1] = neg[0].apply(lambda s: s.split(' '))	                #定义一个分割词，用apply广播
neg[2] = neg[1].apply(lambda x:[i for i in x if i not in stop]) #逐词判断是否为停用词
pos[1] = neg[0].apply(lambda s: s.split(' '))
pos[2] = neg[1].apply(lambda x [i for i in x if i not in stop])
```
```

##### 2.3 LDA主题分析

```python
```python
from gensim import corpora,models

#负面主题分析
neg_dict = corpora.Dictionary(neg[2])	            #建立词典
neg_corpus = [neg_dict.doc2bow(i) for i in neg[2]]  #建立语料库
neg_lda = models.LdaModel(neg_corpus,num_topics = 3,id2word = neg_dict)	#LDA模型训练
for i in range(3):
	print('topic' + str(i))
	print(neg_lda.print_topic(i))                   #输出每个主题
	
#正面主题分析
pos_dict = corpora.Dictionary(pos[2])	            #建立词典
pos_corpus = [pos_dict.doc2bow(i) for i in pos[2]]	#建立语料库
pos_lda = models.LdaModel(pos_corpus,num_topics = 3,id2word = pos_dict)	#LDA模型训练
for i in range(3):
	print('topic' + str(i))
	print(pos_lda.print_topic(i))	                #输出每个主题
```
```





