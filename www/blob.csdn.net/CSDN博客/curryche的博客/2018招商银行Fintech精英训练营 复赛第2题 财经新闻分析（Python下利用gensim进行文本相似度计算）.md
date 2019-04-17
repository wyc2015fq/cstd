# 2018招商银行Fintech精英训练营 复赛第2题 财经新闻分析（Python下利用gensim进行文本相似度计算） - curryche的博客 - CSDN博客





2018年06月14日 09:14:11[curryche](https://me.csdn.net/whwan11)阅读数：545标签：[招商银行																[文本相似度																[Python																[Fintech																[复赛](https://so.csdn.net/so/search/s.do?q=复赛&t=blog)




今年招商银行Fintech精英训练营复赛一共有4道常规题目和1道附加题，选手必须从4道常规题中选择一道完成，附加题可做可不做。 

我选择的是第2题“财经新闻分析”，题目的具体内容如下（找不到原题链接，以下是当时保存的文档，内容完全一致）：

## 原题目

赛题二  财经新闻分析 

评价指标 

该赛题是让参赛者为每一条测试集数据寻找其最相似的TOP 20条新闻（不含测试新闻本身），我们会根据参赛者提交的结果和实际的数据进行对比，采用mAP值作为评价指标，评分公式如下： 


$MAP=\frac{1}{D}\sum\limits_{d=1}^{D}\frac{1}{|Y_d|}\sum\limits_{t\in Y_d}f_t$


$f_t=\begin{cases}\frac{|\{t^{'}\in Y_d\}|r_{t^{'}}\le r_t|}{r_t}&, if\ t\in Z_d \\0&, otherwise\end{cases}$
其中$D$表示测试集中新闻的总数量，$Y_d$表示新闻$d$的$n$条真实相似新闻集合，无序，$Y_d=\{Y_{d1},Y_{d2},Y_{d3},……,Y_{dn}\}$；$Z_d$表示选手提交$m$条(赛题中m=20）相似新闻的有序集合$Z_d=\{Z_{d1},Z_{d2},Z_{d3},……,Z_{dm}\}$；$Z_d$中各元素的Rank值分别$1,2,3,……,m$,记为$r_i$。对于集合$K$，$|K|$表示K中元素的个数,即$|Y_d |=n$。 

【评分示例】 

对某新闻A取TOP10个相似文档，假定A的实际相似新闻集合为$\{a,b,c,d,e,f,g\}$,选手1提交的有序集合为$\{a,g,o,f,d,m,t,b,c,l\}$，选手2提交的有序集合为$\{o,m,t,l,a,g,f,d,b,c\}$，则选手1在新闻A的评分计算如下：  


$f_a=\frac{1}{1},f_b=\frac{5}{8},f_c=\frac{6}{9},f_d=\frac{4}{5},f_e=0,f_f=\frac{3}{4},f_g=\frac{2}{2}$



$AP= \frac{1}{7} (\frac{1}{1}+\frac{5}{8}+\frac{6}{9}+\frac{4}{5}+0+\frac{3}{4}+\frac{2}{2})=0.69166$

选手2在新闻A的评分计算如下： 


$f_a=\frac{1}{5},f_b=\frac{5}{9},f_c=\frac{6}{10},f_d=\frac{4}{8},f_e=0,f_f=\frac{3}{7},f_g=\frac{2}{6}$



$AP=\frac{1}{7} (\frac{1}{5}+\frac{5}{9}+\frac{6}{10}+\frac{4}{8}+0+\frac{3}{7}+\frac{2}{6})=0.37392$

对所有测试新闻的AP加权平均即可得到最终的答案评分mAP。 

【特别提醒】 

提交格式为“UTF-8 无BOM”格式编码的txt文件，去重之后50$\times$20=1000行，不含重复数据。 

提交的相似新闻集合中不包含测试新闻本身。
## 1.数据集

题目提供了训练集合测试集，[数据集下载地址](https://download.csdn.net/download/whwan11/10472137)

训练集包含48万多条新闻标题，格式为“id-title”，保存在“train_data.csv”文件中，如图所示：  

![这里写图片描述](https://img-blog.csdn.net/20180611162527860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

测试集包含50条新闻标题，格式与训练集一致，保存在“test_data.csv”文件中。

## 2.基于TF-IDF模型的文本相似度分析

### 2.1环境

64位Windows7系统 

6G内存 

64位Python3.5 

主要Python工具包：gensim，jieba
### 2.2加载数据

```bash
df_train=pd.read_csv('train_data.csv',encoding='utf-8')
df_train_use=df_train[0:500]  #取500个训练数据用于验证程序
df_test=pd.read_csv('test_data.csv',encoding='gbk')
```

确保数据集与代码文件在同一路径中，如果不在同一路径，在加载数据集时需要加上文件路径。

### 2.3分词

分词就是把一个句子分成若干个词，使用jieba模块进行分词：

```
print('cut words\n')
all_doc_list_train= []
for i in df_train_use.index:      #对训练集分词
    doc_list = [word for word in jieba.cut(df_train_use.ix[i,'title'])]
    #doc_list =df_train_use.ix[i,'title']
    #doc_list=[x.word for x in psg.cut(df_train_use.ix[i,'title']) if x.flag not in stop_flag] #获取词性为名词的词
    all_doc_list_train.append(doc_list)    

all_doc_list_test = []
for i in df_test.index:        #对测试集分词
    doc_list = [word for word in jieba.cut(df_test.ix[i,'title'])]
    #doc_list=[x.word for x in psg.cut(df_test.ix[i,'title']) if x.flag not in stop_flag] #获取词性为名词的词
    all_doc_list_test.append(doc_list)
```

### 2.4去停用词和词频为1的词

分词后，标题的句子含有一些常用词，比如“的”、“地”、“和”，或者标点符号，一般称为“停用词”，这些停用词对句子的相似度分析作用不大，因此要把停用词从分词结果中剔除。

```
#去停用词
all_doc_list_train=[[token for token in text if token not in stopword] for text in all_doc_list_train] 
all_doc_list_test=[[token for token in text if token not in stopword] for text in all_doc_list_test]
```

停用词表可以自定义，也可以使用网上整理好的停用词表。 

此外，遍历训练集分词结果，把只出现一次的词去掉。

```
frequency = defaultdict(int) #构建一个字典对象  
#遍历分词后的结果集，计算每个词出现的频率  
for text in all_doc_list_train:  
    for token in text:  
        frequency[token]+=1
all_doc_list_train=[[token for token in text if frequency[token]>1] for text in all_doc_list_train] #删去词频为1的词
all_doc_list_test=[[token for token in text if frequency[token]>1] for text in all_doc_list_test]
```

### 2.5建立TF-IDF模型

TF-IDF(Term Frequency-Inverse Document Frequency, 词频-逆文件频率)

> 
TF-IDF（term frequency–inverse document frequency）是一种用于信息检索与数据挖掘的常用加权技术。TF意思是词频(Term Frequency)，IDF意思是逆文本频率指数(Inverse Document Frequency)。 

  TF-IDF是一种统计方法，用以评估一字词对于一个文件集或一个语料库中的其中一份文件的重要程度。字词的重要性随着它在文件中出现的次数成正比增加，但同时会随着它在语料库中出现的频率成反比下降。


TFIDF的主要思想是：如果某个词或短语在一篇文章中出现的频率TF高，并且在其他文章中很少出现，则认为此词或者短语具有很好的类别区分能力，适合用来分类。 

TFIDF实际上是：TF * IDF，TF词频(Term Frequency)，IDF逆向文件频率(Inverse Document Frequency)。TF表示词条在文档d中出现的频率。IDF的主要思想是：如果包含词条t的文档越少，也就是n越小，IDF越大，则说明词条t具有很好的类别区分能力。 

如果某一类文档C中包含词条t的文档数为m，而其它类包含t的文档总数为k，显然所有包含t的文档数n=m+k，当m大的时候，n也大，按照IDF公式得到的IDF的值会小，就说明该词条t类别区分能力不强。但是实际上，如果一个词条在一个类的文档中频繁出现，则说明该词条能够很好代表这个类的文本的特征，这样的词条应该给它们赋予较高的权重，并选来作为该类文本的特征词以区别与其它类文档。这就是IDF的不足之处。 

 在一份给定的文件里，词频（term frequency，TF）指的是某一个给定的词语在该文件中出现的频率。这个数字是对词数(term count)的归一化，以防止它偏向长的文件。（同一个词语在长文件里可能会比短文件有更高的词数，而不管该词语重要与否。）对于在某一特定文件$j$的词语$t_i$来说，它的重要性可表示为： 


$TF_{j}(t_i)=\frac{n_{t_i,j}}{\sum\limits_kn_{t_k,j}}$
上式中分子是该词$t_i$在文件$j$中的出现次数，而分母则是在文件$j$中所有词的出现次数之和。 

 逆向文件频率（inverse document frequency，IDF）是一个词语普遍重要性的度量。某一特定词语的IDF，可以由总文件数目除以包含该词语之文件的数目，再将得到的商取对数得到： 


$IDF(t_i)=log\frac{|D|}{|\{d\in D:t_i\in d\}|}$
$|D|$：语料库中的文件总数。 
$|\{d\in D:t_i\in d\}|$：包含词语$t_i$的文件数目，如果该词语不在语料库中，就会导致分母为零，因此一般情况下使用$1+|\{d\in D:t\in d\}|$作为分母。 

然后再计算TF与IDF的乘积： 


$TFIDF_{j}(t_i)=TF_{j}(t_i)\times IDF(t_i)$
某一特定文件内的高词语频率，以及该词语在整个文件集合中的低文件频率，可以产生出高权重的TF-IDF。因此，TF-IDF倾向于过滤掉常见的词语，保留重要的词语。 

TF-IDF模型的建立和训练部分代码如下：

```
dictionary = corpora.Dictionary(all_doc_list_train) #根据训练集分词结果创建字典

doc_train_vec=[dictionary.doc2bow(doc) for doc in all_doc_list_train]  #将分词结果转为词袋模型
doc_test_vec = [dictionary.doc2bow(doc) for doc in all_doc_list_test]

tfidf_model = models.TfidfModel(doc_train_vec)   #tfidf模型训练
```

### 2.6相似度计算

```
index = similarities.SparseMatrixSimilarity(tfidf_model[doc_train_vec], num_features=len(dictionary.keys()))

sims=[]
for i in doc_test_vec:   #计算相似度
    sim = index[tfidf_model[i]]
    sims.append(sim)
```

相似度排序，保留前K个相似度对应的训练集标题ID

```
#保留前K个相似度对应的标题
K=200
sort_sims=[]
index=0

for i in sims:
    j=sorted(enumerate(i), key=lambda item: -item[1])
    count=0
    current_source_id=df_test.ix[index,'id']
    current_items=[]
    selete_title=[]

    for items in j:
        if (items[0]+1)==current_source_id:
            continue
        if all_doc_list_train[items[0]] in selete_title:
            continue       
        current_items.append(items)
        selete_title.append(all_doc_list_train[items[0]])
        #last_title=all_doc_list_train[items[0]]
        count+=1
        if count==K:
            break
    sort_sims.append(current_items)
    index+=1
```

## 3.TFIDF+直接词频向量余弦相似度

2中计算相似度时是通过TF-IDF模型将文档转为词袋向量，然后通过计算词袋向量 

之间的余弦相似度来计算文档之间的相似度的，这样算出来的结果提交的时候评分是0.098左右，为了提高评分，本人想到了一种提高的方法。 

具体的做法是先通过TF-IDF模型找到与测试集中某条标题最相似的Top K条新闻标题，然后在这K条标题里再次选出Top 20条标题作为最后结果。但是在K子集里计算相似度时是直接将标题转为词频向量，计算向量之间的余弦相似度。没用到TF-IDF模型。 

这样做的评分是0.119左右，有少许的提高。
```
#在保留的K个训练集再次筛选
test_index=0
target_title=[]
target_id=[]
similarity=[]# 保存前20个相似度
sub_K=20

tfidf_key= analyse.extract_tags

for i in sort_sims:
    print (test_index)
    indexs=[]
    for j in i:     #提取候选子集对应的index
        indexs.append(j[0])

    df_sub_train=pd.DataFrame(df_train_use.ix[indexs])   #提取候选子集的内容
    df_sub_train.index=range(0,K)

    #分词
    all_doc_list_subtrain=[]
    for i2 in df_sub_train.index:
        doc_list_subtrain=[word for word in jieba.cut(df_sub_train.ix[i2,'title'])]              
        all_doc_list_subtrain.append(doc_list_subtrain)

    doc_test=[word for word in jieba.cut(df_test.ix[test_index,'title'])] 
    key_test=tfidf_key(df_test.ix[test_index,'title'])

    #去停用词
    all_doc_list_subtrain=[[token for token in text if token not in stopword2] for text in all_doc_list_subtrain] 
    doc_test=[token for token in doc_test if token not in stopword2]  

    sub_frequency = defaultdict(int) #构建一个字典对象  
    #遍历分词后的结果集，计算每个词出现的频率  
    for text in all_doc_list_subtrain:  
        for token in text:  
            sub_frequency[token]+=1            
    for token in doc_test:  
        sub_frequency[token]+=1 
    #all_doc_list_subtrain=[[token for token in text if sub_frequency[token]>1] for text in all_doc_list_subtrain] #去掉词频为1的词
    #doc_test=[token for token in doc_test if sub_frequency[token]>1] 

    sub_dictionary = corpora.Dictionary(all_doc_list_subtrain+[doc_test]) #根据训练集分词结果创建字典

    doc_sub_train_vec=[sub_dictionary.doc2bow(doc) for doc in all_doc_list_subtrain]
    doc_test_vec =[sub_dictionary.doc2bow(doc_test)]

    numpy_train = gensim.matutils.corpus2dense(doc_sub_train_vec,num_terms=len(sub_dictionary.keys()),dtype=np.float64).T   #构建词频向量
    numpy_test = gensim.matutils.corpus2dense(doc_test_vec,num_terms=len(sub_dictionary.keys()),dtype=np.float64).T   

    #规范化向量
    #numpy_train_norm=preprocessing.normalize(numpy_train, norm='l2')
    #numpy_test_norm=preprocessing.normalize(numpy_test, norm='l2')

    sub_sims=[]

    for i3 in numpy_train:
            sim=1-pdist([i3,numpy_test[0]],'cosine')
            sub_sims.append(sim[0])

    sort_sub_sims=sorted(enumerate(sub_sims), key=lambda item: -item[1])

    #保留最相似的20个标题
    get_sub_sims=[]
    last_title=[]
    sub_count=0
    sub_selete_title=[]
    for items in sort_sub_sims:
        if all_doc_list_subtrain[items[0]] in sub_selete_title:
            continue

        get_sub_sims.append(items)
        sub_selete_title.append(all_doc_list_subtrain[items[0]])

        sub_count+=1
        if sub_count==20:
            break    

    for i1 in get_sub_sims:
        target_id.append(df_sub_train.ix[i1[0],'id'])      
        target_title.append(df_sub_train.ix[i1[0],'title'])
        similarity.append(i1[1])

    test_index+=1
```

## 4.总结

第二题的题目虽然看起来不难，但是做了才发现要将评分提高是很困难的，感觉就是在用不同方法尽可能拟合标准答案，到最后最高分好像也才0.13多一点，体验就是让人抓狂，科科~](https://so.csdn.net/so/search/s.do?q=Fintech&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)](https://so.csdn.net/so/search/s.do?q=文本相似度&t=blog)](https://so.csdn.net/so/search/s.do?q=招商银行&t=blog)




