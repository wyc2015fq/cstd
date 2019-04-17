# pyLDA系列︱考量时间因素的动态主题模型（Dynamic Topic Models） - 素质云笔记/Recorder... - CSDN博客





2018年02月26日 15:29:20[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2726








笔者很早就对LDA模型着迷，最近在学习gensim库发现了LDA比较有意义且项目较为完整的Tutorials,于是乎就有本系列,本系列包含三款：Latent Dirichlet Allocation、Author-Topic Model、Dynamic Topic Models
|pyLDA系列模型|解析|功能| | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|[ATM模型（Author-Topic Model）](http://blog.csdn.net/sinat_26917383/article/details/79339727)|加入监督的’作者’,每个作者对不同主题的偏好;弊端：*chained topics*, *intruded words*, *random topics*, and *unbalanced topics* (see [Mimno and co-authors 2011](https://people.cs.umass.edu/~wallach/publications/mimno11optimizing.pdf))|作者主题偏好、词语主题偏好、相似作者推荐、可视化[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)主题模型文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性案例与数据主要来源，jupyter notebook可见[gensim的官方github](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/ldaseqmodel.ipynb)详细解释可见：[Dynamic Topic Modeling in Python](https://radimrehurek.com/gensim/models/ldaseqmodel.html).# 1、理论介绍### 论文出处：David Blei does a good job explaining the theory behind this in this [Google talk](https://www.youtube.com/watch?v=7BMsuyBPx90). If you prefer to directly read the [paper on DTM by Blei and Lafferty](http://repository.cmu.edu/cgi/viewcontent.cgi?article=2036&context=compsci)### 参考博客：[This](http://rare-technologies.com/understanding-and-coding-dynamic-topic-models/)### 相关作用：Dynamic Topic Models (DTM) leverages the knowledge of different documents belonging to a different time-slice in an attempt to map how the words in a topic change over time. 同时，**DTM每个时期都有同样K个主题数**，性能是C++版本的DTM模型的5-7倍- （1）You want to find semantically similar documents; one from the very beginning of your time-line, and one in the very end. 从一个时间点到另外时间点中，相似文章有哪些。基于时间的主题，里面的关键词却会发生相应变化。 传统的相似技术不可能做到这样的效果，相同的主题基本内容不变，但是关键词会随着时间而发生变化，也就是所谓的：Time corrected Document Similarity 具有时间校对功能的文档相似性- （2）第二个性能：观察主题中，关键词随时间如何变化，随着时间变化，一开始主题中的词语比较发散式，之后会变得越来越成熟。相关理论可见： ![这里写图片描述](https://img-blog.csdn.net/20180226152830141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- （3）常规性能 the points of interest would be in what the topics are and how the documents are made up of these topics.|函数或模型|作用||----|----||print_topics|不同时期的5个主题的情况||print_topic_times|每个主题的3个时期，主题重要词分别是什么||doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量||model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]||print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205|.# 2 Dynamic Topic Models模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}||time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。|.# 3、Dynamic Topic Models函数解析## 3.1 主函数解析`LdaSeqModel(corpus=None, time_slice=None, id2word=None, alphas=0.01, num_topics=10, initialize='gensim', sstats=None, lda_model=None, obs_variance=0.5, chain_variance=0.005, passes=10, random_state=None, lda_inference_max_iter=25, em_min_iter=6, em_max_iter=20, chunksize=100)`常规参数可参考：[pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)不太一样的参数：- time_slice（最重要）,time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。- chain_variance:话题演变的快慢是由参数variance影响着,其实LDA中Beta参数的高斯参数，chain_variance的默认是0.05，提高该值可以让演变加速- initialize:两种训练DTM模型的方式，第一种直接用语料，第二种用已经训练好的LDA中的个别统计参数矩阵给入作训练。**最简训练模式：**`%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`## 3.2 两种训练模式- 第一种就是上面提到的最简模式，也就是`initialize='gensim'`：`ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`- 第二种模式`initialize='own'`： 已经有了训练好的LDA模型，可以把一些参数解析出来，然后给入模型，此时就需要调整. You can use your own sstats of an LDA model previously trained as well by specifying ‘own’ and passing a np matrix through sstats. If you wish to just pass a previously used LDA model, pass it through lda_model Shape of sstats is (vocab_len, num_topics) .# 4、辅助参数及功能点## 4.1  辅助函数一：print_topics`print_topics(time=0, top_terms=20)`不同时期的5个主题的概况，其中time是指时期阶段，官方案例中训练有三个时期，就是三个月，那么time可选:[0,1,2]，返回的内容格式为：(word, word_probability)```pythonfrom gensim.models import ldaseqmodelfrom gensim.corpora import Dictionary, bleicorpusimport numpyfrom gensim.matutils import hellinger%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)ldaseq.print_topics(time=1)>>> [[(u'blair', 0.0062483544617615841),  (u'labour', 0.0059223974769828398)],  [(u'film', 0.0050860317914258307),  (u'minister', 0.0044210871797581213)],  [(u'government', 0.0039312390246381002),  (u'election', 0.0038787664682510613)],  [(u'prime', 0.0038773564950156151),  (u'party', 0.0036428824115890975)],  [(u'brown', 0.0034964052703373195),  (u'howard', 0.0032628247571913969)]```返回的内容是，每个时期的5个主题，案例中为时期记号为’0’的时期中，5个主题内关键词分别是什么。## 4.2 辅助函数二：print_topic_times`print_topic_times(topic, top_terms=20)`不同主题三个时期的情况```pythonldaseq.print_topic_times(topic=0)>>> [[(u'blair', 0.0061528696567048772),  (u'labour', 0.0054905202853533239)],  [(u'film', 0.0051444037762632807),  (u'minister', 0.0043556939573101399)],  [(u'government', 0.0038839073759585367),  (u'election', 0.0037979240057325865)]```返回的内容是：每个主题的3个时期，主题重要词分别是啥。## 4.3 辅助函数三：doc_topics`doc_topics(doc_number)`不同文档主题偏好（常规）```pythondoc = ldaseq.doc_topics(558) # check the 558th document in the corpuses topic distributionprint (doc)>>> [  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01]```五个主题中，第558篇文章对1，4号主题更为偏好。 当然可以通过解析corpus来进行核实：```pythonwords = [dictionary[word_id] for word_id, count in corpus[558]]print (words)>>> [u'set', u'time,"', u'chairman', u'decision', u'news', u'director', u'former', u'vowed', u'"it', u'results', u'club', u'third', u'home', u'paul', u'saturday.', u'south', u'conference']```## 4.4 新文档预测如果有一个新文档过来，怎么进行预测呢？```doc_football_1 = ['economy', 'bank', 'mobile', 'phone', 'markets', 'buy', 'football', 'united', 'giggs']doc_football_1 = dictionary.doc2bow(doc_football_1)doc_football_1 = ldaseq[doc_football_1]print (doc_football_1)>>> [ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]```步骤就是先把`doc_football_1` 文档分词，然后进行`dictionary.doc2bow`矢量化，然后就可以用`ldaseq`模型进行预测，可见结果,与doc_topics  类似，返回的是该新文档，与四号主题比较贴合。## 4.5 跨时间+主题属性的文档相似性（核心功能）dtms主题建模更方便的用途之一是我们可以比较不同时间范围内的文档，并查看它们在主题方面的相似程度。当这些时间段中的单词不一定重叠时，这是非常有用的。```doc_football_2 = ['arsenal', 'fourth', 'wenger', 'oil', 'middle', 'east', 'sanction', 'fluctuation']doc_football_2 = dictionary.doc2bow(doc_football_2)doc_football_2 = ldaseq[doc_football_2]>>> array([ 0.00141844,  0.00141844,  0.00141844,  0.00141844,  0.99432624])hellinger(doc_football_1, doc_football_2)>>> 0.0062680905375190245```步骤跟之前的预测差不多，先把文档`dictionary.doc2bow`矢量化，然后变成文档主题偏好向量(1*5)，然后根据两个文档的主题偏好向量用hellinger距离进行求相似。## 4.6 可视化模型DTMvis```pythonfrom gensim.models.wrappers.dtmmodel import DtmModelfrom gensim.corpora import Dictionary, bleicorpusimport pyLDAvis# dtm_path = "/Users/bhargavvader/Downloads/dtm_release/dtm/main"# dtm_model = DtmModel(dtm_path, corpus, time_slice, num_topics=5, id2word=dictionary, initialize_lda=True)# dtm_model.save('dtm_news')# if we've saved before simply load the model# ldaseq_chain.save('dtm_news')dtm_model = DtmModel.load('dtm_news')doc_topic, topic_term, doc_lengths, term_frequency, vocab = dtm_model.dtm_vis(time=0, corpus=corpus)vis_wrapper = pyLDAvis.prepare(topic_term_dists=topic_term, doc_topic_dists=doc_topic, doc_lengths=doc_lengths, vocab=vocab, term_frequency=term_frequency)pyLDAvis.display(vis_wrapper)```![这里写图片描述](https://img-blog.csdn.net/20180226152847702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70).# 5、话题一致性评价指标```pythonfrom gensim.models.coherencemodel import CoherenceModelimport pickle# we just have to specify the time-slice we want to find coherence for.topics_wrapper = dtm_model.dtm_coherence(time=0)  # 标准模型topics_dtm = ldaseq.dtm_coherence(time=2)   # 本次模型# running u_mass coherence on our modelscm_wrapper = CoherenceModel(topics=topics_wrapper, corpus=corpus, dictionary=dictionary, coherence='u_mass')cm_DTM = CoherenceModel(topics=topics_dtm, corpus=corpus, dictionary=dictionary, coherence='u_mass')print ("U_mass topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())# to use 'c_v' we need texts, which we have saved to disk.texts = pickle.load(open('Corpus/texts', 'rb'))cm_wrapper = CoherenceModel(topics=topics_wrapper, texts=texts, dictionary=dictionary, coherence='c_v')cm_DTM = CoherenceModel(topics=topics_dtm, texts=texts, dictionary=dictionary, coherence='c_v')print ("C_v topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())```该部分解释较少，详情可见： We also, however, have the option of passing our own model or suff stats values. Our final DTM results are heavily influenced by what we pass over here. We already know what a “Good” or “Bad” LDA model is (if not, read about it [here](http://nbviewer.jupyter.org/github/dsquareindia/gensim/blob/280375fe14adea67ce6384ba7eabf362b05e6029/docs/notebooks/topic_coherence_tutorial.ipynb)). **公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)|主题模型|文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性案例与数据主要来源，jupyter notebook可见[gensim的官方github](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/ldaseqmodel.ipynb)详细解释可见：[Dynamic Topic Modeling in Python](https://radimrehurek.com/gensim/models/ldaseqmodel.html).# 1、理论介绍### 论文出处：David Blei does a good job explaining the theory behind this in this [Google talk](https://www.youtube.com/watch?v=7BMsuyBPx90). If you prefer to directly read the [paper on DTM by Blei and Lafferty](http://repository.cmu.edu/cgi/viewcontent.cgi?article=2036&context=compsci)### 参考博客：[This](http://rare-technologies.com/understanding-and-coding-dynamic-topic-models/)### 相关作用：Dynamic Topic Models (DTM) leverages the knowledge of different documents belonging to a different time-slice in an attempt to map how the words in a topic change over time. 同时，**DTM每个时期都有同样K个主题数**，性能是C++版本的DTM模型的5-7倍- （1）You want to find semantically similar documents; one from the very beginning of your time-line, and one in the very end. 从一个时间点到另外时间点中，相似文章有哪些。基于时间的主题，里面的关键词却会发生相应变化。 传统的相似技术不可能做到这样的效果，相同的主题基本内容不变，但是关键词会随着时间而发生变化，也就是所谓的：Time corrected Document Similarity 具有时间校对功能的文档相似性- （2）第二个性能：观察主题中，关键词随时间如何变化，随着时间变化，一开始主题中的词语比较发散式，之后会变得越来越成熟。相关理论可见： ![这里写图片描述](https://img-blog.csdn.net/20180226152830141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- （3）常规性能 the points of interest would be in what the topics are and how the documents are made up of these topics.|函数或模型|作用||----|----||print_topics|不同时期的5个主题的情况||print_topic_times|每个主题的3个时期，主题重要词分别是什么||doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量||model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]||print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205|.# 2 Dynamic Topic Models模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}||time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。|.# 3、Dynamic Topic Models函数解析## 3.1 主函数解析`LdaSeqModel(corpus=None, time_slice=None, id2word=None, alphas=0.01, num_topics=10, initialize='gensim', sstats=None, lda_model=None, obs_variance=0.5, chain_variance=0.005, passes=10, random_state=None, lda_inference_max_iter=25, em_min_iter=6, em_max_iter=20, chunksize=100)`常规参数可参考：[pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)不太一样的参数：- time_slice（最重要）,time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。- chain_variance:话题演变的快慢是由参数variance影响着,其实LDA中Beta参数的高斯参数，chain_variance的默认是0.05，提高该值可以让演变加速- initialize:两种训练DTM模型的方式，第一种直接用语料，第二种用已经训练好的LDA中的个别统计参数矩阵给入作训练。**最简训练模式：**`%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`## 3.2 两种训练模式- 第一种就是上面提到的最简模式，也就是`initialize='gensim'`：`ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`- 第二种模式`initialize='own'`： 已经有了训练好的LDA模型，可以把一些参数解析出来，然后给入模型，此时就需要调整. You can use your own sstats of an LDA model previously trained as well by specifying ‘own’ and passing a np matrix through sstats. If you wish to just pass a previously used LDA model, pass it through lda_model Shape of sstats is (vocab_len, num_topics) .# 4、辅助参数及功能点## 4.1  辅助函数一：print_topics`print_topics(time=0, top_terms=20)`不同时期的5个主题的概况，其中time是指时期阶段，官方案例中训练有三个时期，就是三个月，那么time可选:[0,1,2]，返回的内容格式为：(word, word_probability)```pythonfrom gensim.models import ldaseqmodelfrom gensim.corpora import Dictionary, bleicorpusimport numpyfrom gensim.matutils import hellinger%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)ldaseq.print_topics(time=1)>>> [[(u'blair', 0.0062483544617615841),  (u'labour', 0.0059223974769828398)],  [(u'film', 0.0050860317914258307),  (u'minister', 0.0044210871797581213)],  [(u'government', 0.0039312390246381002),  (u'election', 0.0038787664682510613)],  [(u'prime', 0.0038773564950156151),  (u'party', 0.0036428824115890975)],  [(u'brown', 0.0034964052703373195),  (u'howard', 0.0032628247571913969)]```返回的内容是，每个时期的5个主题，案例中为时期记号为’0’的时期中，5个主题内关键词分别是什么。## 4.2 辅助函数二：print_topic_times`print_topic_times(topic, top_terms=20)`不同主题三个时期的情况```pythonldaseq.print_topic_times(topic=0)>>> [[(u'blair', 0.0061528696567048772),  (u'labour', 0.0054905202853533239)],  [(u'film', 0.0051444037762632807),  (u'minister', 0.0043556939573101399)],  [(u'government', 0.0038839073759585367),  (u'election', 0.0037979240057325865)]```返回的内容是：每个主题的3个时期，主题重要词分别是啥。## 4.3 辅助函数三：doc_topics`doc_topics(doc_number)`不同文档主题偏好（常规）```pythondoc = ldaseq.doc_topics(558) # check the 558th document in the corpuses topic distributionprint (doc)>>> [  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01]```五个主题中，第558篇文章对1，4号主题更为偏好。 当然可以通过解析corpus来进行核实：```pythonwords = [dictionary[word_id] for word_id, count in corpus[558]]print (words)>>> [u'set', u'time,"', u'chairman', u'decision', u'news', u'director', u'former', u'vowed', u'"it', u'results', u'club', u'third', u'home', u'paul', u'saturday.', u'south', u'conference']```## 4.4 新文档预测如果有一个新文档过来，怎么进行预测呢？```doc_football_1 = ['economy', 'bank', 'mobile', 'phone', 'markets', 'buy', 'football', 'united', 'giggs']doc_football_1 = dictionary.doc2bow(doc_football_1)doc_football_1 = ldaseq[doc_football_1]print (doc_football_1)>>> [ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]```步骤就是先把`doc_football_1` 文档分词，然后进行`dictionary.doc2bow`矢量化，然后就可以用`ldaseq`模型进行预测，可见结果,与doc_topics  类似，返回的是该新文档，与四号主题比较贴合。## 4.5 跨时间+主题属性的文档相似性（核心功能）dtms主题建模更方便的用途之一是我们可以比较不同时间范围内的文档，并查看它们在主题方面的相似程度。当这些时间段中的单词不一定重叠时，这是非常有用的。```doc_football_2 = ['arsenal', 'fourth', 'wenger', 'oil', 'middle', 'east', 'sanction', 'fluctuation']doc_football_2 = dictionary.doc2bow(doc_football_2)doc_football_2 = ldaseq[doc_football_2]>>> array([ 0.00141844,  0.00141844,  0.00141844,  0.00141844,  0.99432624])hellinger(doc_football_1, doc_football_2)>>> 0.0062680905375190245```步骤跟之前的预测差不多，先把文档`dictionary.doc2bow`矢量化，然后变成文档主题偏好向量(1*5)，然后根据两个文档的主题偏好向量用hellinger距离进行求相似。## 4.6 可视化模型DTMvis```pythonfrom gensim.models.wrappers.dtmmodel import DtmModelfrom gensim.corpora import Dictionary, bleicorpusimport pyLDAvis# dtm_path = "/Users/bhargavvader/Downloads/dtm_release/dtm/main"# dtm_model = DtmModel(dtm_path, corpus, time_slice, num_topics=5, id2word=dictionary, initialize_lda=True)# dtm_model.save('dtm_news')# if we've saved before simply load the model# ldaseq_chain.save('dtm_news')dtm_model = DtmModel.load('dtm_news')doc_topic, topic_term, doc_lengths, term_frequency, vocab = dtm_model.dtm_vis(time=0, corpus=corpus)vis_wrapper = pyLDAvis.prepare(topic_term_dists=topic_term, doc_topic_dists=doc_topic, doc_lengths=doc_lengths, vocab=vocab, term_frequency=term_frequency)pyLDAvis.display(vis_wrapper)```![这里写图片描述](https://img-blog.csdn.net/20180226152847702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70).# 5、话题一致性评价指标```pythonfrom gensim.models.coherencemodel import CoherenceModelimport pickle# we just have to specify the time-slice we want to find coherence for.topics_wrapper = dtm_model.dtm_coherence(time=0)  # 标准模型topics_dtm = ldaseq.dtm_coherence(time=2)   # 本次模型# running u_mass coherence on our modelscm_wrapper = CoherenceModel(topics=topics_wrapper, corpus=corpus, dictionary=dictionary, coherence='u_mass')cm_DTM = CoherenceModel(topics=topics_dtm, corpus=corpus, dictionary=dictionary, coherence='u_mass')print ("U_mass topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())# to use 'c_v' we need texts, which we have saved to disk.texts = pickle.load(open('Corpus/texts', 'rb'))cm_wrapper = CoherenceModel(topics=topics_wrapper, texts=texts, dictionary=dictionary, coherence='c_v')cm_DTM = CoherenceModel(topics=topics_dtm, texts=texts, dictionary=dictionary, coherence='c_v')print ("C_v topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())```该部分解释较少，详情可见： We also, however, have the option of passing our own model or suff stats values. Our final DTM results are heavily influenced by what we pass over here. We already know what a “Good” or “Bad” LDA model is (if not, read about it [here](http://nbviewer.jupyter.org/github/dsquareindia/gensim/blob/280375fe14adea67ce6384ba7eabf362b05e6029/docs/notebooks/topic_coherence_tutorial.ipynb)). **公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性|print_topics|不同时期的5个主题的情况|print_topic_times|每个主题的3个时期，主题重要词分别是什么|doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量|model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]|print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]|dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。|
|[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)|主题模型|文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性案例与数据主要来源，jupyter notebook可见[gensim的官方github](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/ldaseqmodel.ipynb)详细解释可见：[Dynamic Topic Modeling in Python](https://radimrehurek.com/gensim/models/ldaseqmodel.html).# 1、理论介绍### 论文出处：David Blei does a good job explaining the theory behind this in this [Google talk](https://www.youtube.com/watch?v=7BMsuyBPx90). If you prefer to directly read the [paper on DTM by Blei and Lafferty](http://repository.cmu.edu/cgi/viewcontent.cgi?article=2036&context=compsci)### 参考博客：[This](http://rare-technologies.com/understanding-and-coding-dynamic-topic-models/)### 相关作用：Dynamic Topic Models (DTM) leverages the knowledge of different documents belonging to a different time-slice in an attempt to map how the words in a topic change over time. 同时，**DTM每个时期都有同样K个主题数**，性能是C++版本的DTM模型的5-7倍- （1）You want to find semantically similar documents; one from the very beginning of your time-line, and one in the very end. 从一个时间点到另外时间点中，相似文章有哪些。基于时间的主题，里面的关键词却会发生相应变化。 传统的相似技术不可能做到这样的效果，相同的主题基本内容不变，但是关键词会随着时间而发生变化，也就是所谓的：Time corrected Document Similarity 具有时间校对功能的文档相似性- （2）第二个性能：观察主题中，关键词随时间如何变化，随着时间变化，一开始主题中的词语比较发散式，之后会变得越来越成熟。相关理论可见： ![这里写图片描述](https://img-blog.csdn.net/20180226152830141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- （3）常规性能 the points of interest would be in what the topics are and how the documents are made up of these topics.|函数或模型|作用||----|----||print_topics|不同时期的5个主题的情况||print_topic_times|每个主题的3个时期，主题重要词分别是什么||doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量||model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]||print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205|.# 2 Dynamic Topic Models模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}||time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。|.# 3、Dynamic Topic Models函数解析## 3.1 主函数解析`LdaSeqModel(corpus=None, time_slice=None, id2word=None, alphas=0.01, num_topics=10, initialize='gensim', sstats=None, lda_model=None, obs_variance=0.5, chain_variance=0.005, passes=10, random_state=None, lda_inference_max_iter=25, em_min_iter=6, em_max_iter=20, chunksize=100)`常规参数可参考：[pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)不太一样的参数：- time_slice（最重要）,time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。- chain_variance:话题演变的快慢是由参数variance影响着,其实LDA中Beta参数的高斯参数，chain_variance的默认是0.05，提高该值可以让演变加速- initialize:两种训练DTM模型的方式，第一种直接用语料，第二种用已经训练好的LDA中的个别统计参数矩阵给入作训练。**最简训练模式：**`%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`## 3.2 两种训练模式- 第一种就是上面提到的最简模式，也就是`initialize='gensim'`：`ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`- 第二种模式`initialize='own'`： 已经有了训练好的LDA模型，可以把一些参数解析出来，然后给入模型，此时就需要调整. You can use your own sstats of an LDA model previously trained as well by specifying ‘own’ and passing a np matrix through sstats. If you wish to just pass a previously used LDA model, pass it through lda_model Shape of sstats is (vocab_len, num_topics) .# 4、辅助参数及功能点## 4.1  辅助函数一：print_topics`print_topics(time=0, top_terms=20)`不同时期的5个主题的概况，其中time是指时期阶段，官方案例中训练有三个时期，就是三个月，那么time可选:[0,1,2]，返回的内容格式为：(word, word_probability)```pythonfrom gensim.models import ldaseqmodelfrom gensim.corpora import Dictionary, bleicorpusimport numpyfrom gensim.matutils import hellinger%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)ldaseq.print_topics(time=1)>>> [[(u'blair', 0.0062483544617615841),  (u'labour', 0.0059223974769828398)],  [(u'film', 0.0050860317914258307),  (u'minister', 0.0044210871797581213)],  [(u'government', 0.0039312390246381002),  (u'election', 0.0038787664682510613)],  [(u'prime', 0.0038773564950156151),  (u'party', 0.0036428824115890975)],  [(u'brown', 0.0034964052703373195),  (u'howard', 0.0032628247571913969)]```返回的内容是，每个时期的5个主题，案例中为时期记号为’0’的时期中，5个主题内关键词分别是什么。## 4.2 辅助函数二：print_topic_times`print_topic_times(topic, top_terms=20)`不同主题三个时期的情况```pythonldaseq.print_topic_times(topic=0)>>> [[(u'blair', 0.0061528696567048772),  (u'labour', 0.0054905202853533239)],  [(u'film', 0.0051444037762632807),  (u'minister', 0.0043556939573101399)],  [(u'government', 0.0038839073759585367),  (u'election', 0.0037979240057325865)]```返回的内容是：每个主题的3个时期，主题重要词分别是啥。## 4.3 辅助函数三：doc_topics`doc_topics(doc_number)`不同文档主题偏好（常规）```pythondoc = ldaseq.doc_topics(558) # check the 558th document in the corpuses topic distributionprint (doc)>>> [  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01]```五个主题中，第558篇文章对1，4号主题更为偏好。 当然可以通过解析corpus来进行核实：```pythonwords = [dictionary[word_id] for word_id, count in corpus[558]]print (words)>>> [u'set', u'time,"', u'chairman', u'decision', u'news', u'director', u'former', u'vowed', u'"it', u'results', u'club', u'third', u'home', u'paul', u'saturday.', u'south', u'conference']```## 4.4 新文档预测如果有一个新文档过来，怎么进行预测呢？```doc_football_1 = ['economy', 'bank', 'mobile', 'phone', 'markets', 'buy', 'football', 'united', 'giggs']doc_football_1 = dictionary.doc2bow(doc_football_1)doc_football_1 = ldaseq[doc_football_1]print (doc_football_1)>>> [ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]```步骤就是先把`doc_football_1` 文档分词，然后进行`dictionary.doc2bow`矢量化，然后就可以用`ldaseq`模型进行预测，可见结果,与doc_topics  类似，返回的是该新文档，与四号主题比较贴合。## 4.5 跨时间+主题属性的文档相似性（核心功能）dtms主题建模更方便的用途之一是我们可以比较不同时间范围内的文档，并查看它们在主题方面的相似程度。当这些时间段中的单词不一定重叠时，这是非常有用的。```doc_football_2 = ['arsenal', 'fourth', 'wenger', 'oil', 'middle', 'east', 'sanction', 'fluctuation']doc_football_2 = dictionary.doc2bow(doc_football_2)doc_football_2 = ldaseq[doc_football_2]>>> array([ 0.00141844,  0.00141844,  0.00141844,  0.00141844,  0.99432624])hellinger(doc_football_1, doc_football_2)>>> 0.0062680905375190245```步骤跟之前的预测差不多，先把文档`dictionary.doc2bow`矢量化，然后变成文档主题偏好向量(1*5)，然后根据两个文档的主题偏好向量用hellinger距离进行求相似。## 4.6 可视化模型DTMvis```pythonfrom gensim.models.wrappers.dtmmodel import DtmModelfrom gensim.corpora import Dictionary, bleicorpusimport pyLDAvis# dtm_path = "/Users/bhargavvader/Downloads/dtm_release/dtm/main"# dtm_model = DtmModel(dtm_path, corpus, time_slice, num_topics=5, id2word=dictionary, initialize_lda=True)# dtm_model.save('dtm_news')# if we've saved before simply load the model# ldaseq_chain.save('dtm_news')dtm_model = DtmModel.load('dtm_news')doc_topic, topic_term, doc_lengths, term_frequency, vocab = dtm_model.dtm_vis(time=0, corpus=corpus)vis_wrapper = pyLDAvis.prepare(topic_term_dists=topic_term, doc_topic_dists=doc_topic, doc_lengths=doc_lengths, vocab=vocab, term_frequency=term_frequency)pyLDAvis.display(vis_wrapper)```![这里写图片描述](https://img-blog.csdn.net/20180226152847702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70).# 5、话题一致性评价指标```pythonfrom gensim.models.coherencemodel import CoherenceModelimport pickle# we just have to specify the time-slice we want to find coherence for.topics_wrapper = dtm_model.dtm_coherence(time=0)  # 标准模型topics_dtm = ldaseq.dtm_coherence(time=2)   # 本次模型# running u_mass coherence on our modelscm_wrapper = CoherenceModel(topics=topics_wrapper, corpus=corpus, dictionary=dictionary, coherence='u_mass')cm_DTM = CoherenceModel(topics=topics_dtm, corpus=corpus, dictionary=dictionary, coherence='u_mass')print ("U_mass topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())# to use 'c_v' we need texts, which we have saved to disk.texts = pickle.load(open('Corpus/texts', 'rb'))cm_wrapper = CoherenceModel(topics=topics_wrapper, texts=texts, dictionary=dictionary, coherence='c_v')cm_DTM = CoherenceModel(topics=topics_dtm, texts=texts, dictionary=dictionary, coherence='c_v')print ("C_v topic coherence")print ("Wrapper coherence is ", cm_wrapper.get_coherence())print ("DTM Python coherence is", cm_DTM.get_coherence())```该部分解释较少，详情可见： We also, however, have the option of passing our own model or suff stats values. Our final DTM results are heavily influenced by what we pass over here. We already know what a “Good” or “Bad” LDA model is (if not, read about it [here](http://nbviewer.jupyter.org/github/dsquareindia/gensim/blob/280375fe14adea67ce6384ba7eabf362b05e6029/docs/notebooks/topic_coherence_tutorial.ipynb)). **公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性|print_topics|不同时期的5个主题的情况|print_topic_times|每个主题的3个时期，主题重要词分别是什么|doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量|model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]|print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]|dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。| | | |
|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性| | | | | | | | | | | | | | | | | | | | | | | | | | | | |
| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|print_topics|不同时期的5个主题的情况| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|print_topic_times|每个主题的3个时期，主题重要词分别是什么| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|doc_topics|不同文档主题偏好（常规），跟LDA中`get_document_topics` 一致，返回内容如下：[  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01] 下文称为主题偏好向量| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|model[]新文档预测|ldaseq[dictionary.doc2bow( [‘economy’, ‘bank’, ‘mobile’, ‘phone’, ‘markets’, ‘buy’, ‘football’, ‘united’, ‘giggs’])]，返回内容：[ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|print_topics,跨时间+主题属性的文档相似性|hellinger(doc_football_1, doc_football_2)，返回的是：0.95828252517231205| | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]| | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]| | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}| | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|time-slices|时间记录，按顺序来排列|time_slice = [438, 430, 456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。| | | | | | | | | | | | | | | | | | | | | | | | | | | | |

.

# 3、Dynamic Topic Models函数解析

## 3.1 主函数解析
`LdaSeqModel(corpus=None, time_slice=None, id2word=None, alphas=0.01, num_topics=10, initialize='gensim', sstats=None, lda_model=None, obs_variance=0.5, chain_variance=0.005, passes=10, random_state=None, lda_inference_max_iter=25, em_min_iter=6, em_max_iter=20, chunksize=100)`
常规参数可参考：[pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)

不太一样的参数：
- time_slice（最重要）,time_slice = [438, 430, 

456]代表着，三个月的时间里，第一个有438篇文章，第二个月有430篇文章，第三个月有456篇文章。当然可以以年月日以及任意计量时间都可以。
- chain_variance:话题演变的快慢是由参数variance影响着,其实LDA中Beta参数的高斯参数，chain_variance的默认是0.05，提高该值可以让演变加速
- initialize:两种训练DTM模型的方式，第一种直接用语料，第二种用已经训练好的LDA中的个别统计参数矩阵给入作训练。

**最简训练模式：**
`%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`
## 3.2 两种训练模式
- 第一种就是上面提到的最简模式，也就是`initialize='gensim'`：
`ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)`- 第二种模式`initialize='own'`： 已经有了训练好的LDA模型，可以把一些参数解析出来，然后给入模型，此时就需要调整. 

You can use your own sstats of an LDA model previously trained as 

well by specifying ‘own’ and passing a np matrix through sstats. If 

you wish to just pass a previously used LDA model, pass it through 

lda_model Shape of sstats is (vocab_len, num_topics) 

.
# 4、辅助参数及功能点

## 4.1  辅助函数一：print_topics
`print_topics(time=0, top_terms=20)`
不同时期的5个主题的概况，其中time是指时期阶段，官方案例中训练有三个时期，就是三个月，那么time可选:[0,1,2]，返回的内容格式为：(word, word_probability)

```python
from gensim.models import ldaseqmodel
from gensim.corpora import Dictionary, bleicorpus
import numpy
from gensim.matutils import hellinger

%time ldaseq = ldaseqmodel.LdaSeqModel(corpus=corpus, id2word=dictionary, time_slice=time_slice, num_topics=5)

ldaseq.print_topics(time=1)
>>> [[(u'blair', 0.0062483544617615841),
  (u'labour', 0.0059223974769828398)],
  [(u'film', 0.0050860317914258307),
  (u'minister', 0.0044210871797581213)],
  [(u'government', 0.0039312390246381002),
  (u'election', 0.0038787664682510613)],
  [(u'prime', 0.0038773564950156151),
  (u'party', 0.0036428824115890975)],
  [(u'brown', 0.0034964052703373195),
  (u'howard', 0.0032628247571913969)]
```

返回的内容是，每个时期的5个主题，案例中为时期记号为’0’的时期中，5个主题内关键词分别是什么。

## 4.2 辅助函数二：print_topic_times
`print_topic_times(topic, top_terms=20)`
不同主题三个时期的情况

```python
ldaseq.print_topic_times(topic=0)
>>> [[(u'blair', 0.0061528696567048772),
  (u'labour', 0.0054905202853533239)],
  [(u'film', 0.0051444037762632807),
  (u'minister', 0.0043556939573101399)],
  [(u'government', 0.0038839073759585367),
  (u'election', 0.0037979240057325865)]
```

返回的内容是：每个主题的3个时期，主题重要词分别是啥。

## 4.3 辅助函数三：doc_topics
`doc_topics(doc_number)`
不同文档主题偏好（常规）

```python
doc = ldaseq.doc_topics(558) # check the 558th document in the corpuses topic distribution
print (doc)
>>> [  5.46298825e-05   2.18468637e-01   5.46298825e-05   5.46298825e-05    7.81367474e-01]
```

五个主题中，第558篇文章对1，4号主题更为偏好。 

当然可以通过解析corpus来进行核实：

```python
words = [dictionary[word_id] for word_id, count in corpus[558]]
print (words)
>>> [u'set', u'time,"', u'chairman', u'decision', u'news', u'director', u'former', u'vowed', u'"it', u'results', u'club', u'third', u'home', u'paul', u'saturday.', u'south', u'conference']
```

## 4.4 新文档预测

如果有一个新文档过来，怎么进行预测呢？

```
doc_football_1 = ['economy', 'bank', 'mobile', 'phone', 'markets', 'buy', 'football', 'united', 'giggs']
doc_football_1 = dictionary.doc2bow(doc_football_1)
doc_football_1 = ldaseq[doc_football_1]
print (doc_football_1)
>>> [ 0.00110497  0.00110497  0.00110497  0.00110497  0.99558011]
```

步骤就是先把`doc_football_1` 文档分词，然后进行`dictionary.doc2bow`矢量化，然后就可以用`ldaseq`模型进行预测，可见结果,与doc_topics  类似，返回的是该新文档，与四号主题比较贴合。

## 4.5 跨时间+主题属性的文档相似性（核心功能）

dtms主题建模更方便的用途之一是我们可以比较不同时间范围内的文档，并查看它们在主题方面的相似程度。当这些时间段中的单词不一定重叠时，这是非常有用的。

```
doc_football_2 = ['arsenal', 'fourth', 'wenger', 'oil', 'middle', 'east', 'sanction', 'fluctuation']
doc_football_2 = dictionary.doc2bow(doc_football_2)
doc_football_2 = ldaseq[doc_football_2]
>>> array([ 0.00141844,  0.00141844,  0.00141844,  0.00141844,  0.99432624])

hellinger(doc_football_1, doc_football_2)
>>> 0.0062680905375190245
```

步骤跟之前的预测差不多，先把文档`dictionary.doc2bow`矢量化，然后变成文档主题偏好向量(1*5)，然后根据两个文档的主题偏好向量用hellinger距离进行求相似。

## 4.6 可视化模型DTMvis

```python
from gensim.models.wrappers.dtmmodel import DtmModel
from gensim.corpora import Dictionary, bleicorpus
import pyLDAvis

# dtm_path = "/Users/bhargavvader/Downloads/dtm_release/dtm/main"
# dtm_model = DtmModel(dtm_path, corpus, time_slice, num_topics=5, id2word=dictionary, initialize_lda=True)
# dtm_model.save('dtm_news')

# if we've saved before simply load the model
# ldaseq_chain.save('dtm_news')
dtm_model = DtmModel.load('dtm_news')

doc_topic, topic_term, doc_lengths, term_frequency, vocab = dtm_model.dtm_vis(time=0, corpus=corpus)
vis_wrapper = pyLDAvis.prepare(topic_term_dists=topic_term, doc_topic_dists=doc_topic, doc_lengths=doc_lengths, vocab=vocab, term_frequency=term_frequency)
pyLDAvis.display(vis_wrapper)
```

![这里写图片描述](https://img-blog.csdn.net/20180226152847702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

.
# 5、话题一致性评价指标

```python
from gensim.models.coherencemodel import CoherenceModel
import pickle

# we just have to specify the time-slice we want to find coherence for.
topics_wrapper = dtm_model.dtm_coherence(time=0)  # 标准模型
topics_dtm = ldaseq.dtm_coherence(time=2)   # 本次模型

# running u_mass coherence on our models
cm_wrapper = CoherenceModel(topics=topics_wrapper, corpus=corpus, dictionary=dictionary, coherence='u_mass')
cm_DTM = CoherenceModel(topics=topics_dtm, corpus=corpus, dictionary=dictionary, coherence='u_mass')

print ("U_mass topic coherence")
print ("Wrapper coherence is ", cm_wrapper.get_coherence())
print ("DTM Python coherence is", cm_DTM.get_coherence())

# to use 'c_v' we need texts, which we have saved to disk.
texts = pickle.load(open('Corpus/texts', 'rb'))
cm_wrapper = CoherenceModel(topics=topics_wrapper, texts=texts, dictionary=dictionary, coherence='c_v')
cm_DTM = CoherenceModel(topics=topics_dtm, texts=texts, dictionary=dictionary, coherence='c_v')

print ("C_v topic coherence")
print ("Wrapper coherence is ", cm_wrapper.get_coherence())
print ("DTM Python coherence is", cm_DTM.get_coherence())
```

该部分解释较少，详情可见： 

We also, however, have the option of passing our own model or suff stats values. Our final DTM results are heavily influenced by what we pass over here. We already know what a “Good” or “Bad” LDA model is (if not, read about it [here](http://nbviewer.jupyter.org/github/dsquareindia/gensim/blob/280375fe14adea67ce6384ba7eabf362b05e6029/docs/notebooks/topic_coherence_tutorial.ipynb)). 


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






