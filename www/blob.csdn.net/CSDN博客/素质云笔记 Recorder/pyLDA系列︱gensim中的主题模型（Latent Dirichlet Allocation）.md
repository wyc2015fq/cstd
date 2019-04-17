# pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation） - 素质云笔记/Recorder... - CSDN博客





2018年02月23日 23:17:02[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5737








笔者很早就对LDA模型着迷，最近在学习gensim库发现了LDA比较有意义且项目较为完整的Tutorials,于是乎就有本系列,本系列包含三款：Latent Dirichlet Allocation、Author-Topic Model、Dynamic Topic Models
|pyLDA系列模型|解析|功能| | | | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|[ATM模型（Author-Topic Model）](http://blog.csdn.net/sinat_26917383/article/details/79339727)|加入监督的’作者’,每个作者对不同主题的偏好;弊端：*chained topics*, *intruded words*, *random topics*, and *unbalanced topics* (see [Mimno and co-authors 2011](https://people.cs.umass.edu/~wallach/publications/mimno11optimizing.pdf))|作者主题偏好、词语主题偏好、相似作者推荐、可视化[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)主题模型文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性本篇为常规的LDA简单罗列：- Gentle introduction to the LDA model: [http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/](http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/)- Gensim’s LDA API documentation: [https://radimrehurek.com/gensim/models/ldamodel.html](https://radimrehurek.com/gensim/models/ldamodel.html)- Topic modelling in Gensim: [http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html](http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html).# 1 模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|.# 2 函数解析参考自[官网教程 models.ldamodel – Latent Dirichlet Allocation](https://radimrehurek.com/gensim/models/ldamodel.html)## 2.1 主函数解析`class gensim.models.ldamodel.LdaModel(corpus=None, num_topics=100, id2word=None, distributed=False, chunksize=2000, passes=1, update_every=1, alpha='symmetric', eta=None, decay=0.5, offset=1.0, eval_every=10, iterations=50, gamma_threshold=0.001, minimum_probability=0.01, random_state=None, ns_conf=None, minimum_phi_value=0.01, per_word_topics=False, callbacks=None, dtype=<type 'numpy.float32'>)`- corpus：用该参数传入的文档语料将会被用来训练模型，如果不指定该参数，则不进行任何训练，默认后续会调用 update() 方法对模型语料进行更新- num_topics：需要提取的潜在主题数- id2word：用于设置构建模型的词典，决定了词汇数量，id2word = dictionary.id2token- distributed：是否开启分布式计算- chunksize：文件块大小，等同深度学习训练的batch，一次性给入2000篇文章，一次性给入越多，性能越好，该指标会略微影响最终结果- **passes**：等同epoch，how often we train the model on the entire corpus.- **iterations**：it controls how often we repeat a particular loop over each    document.   跟passes相对，每篇文章的loop，其跟passes两个指标都很重要，若把这两个指标设置得足够高，会有好效果。- alpha：决定文档主题狄利克雷先验分布的超参数，默认取值为对称 1.0/num_topics 先验，可以自行设置，也支持以下两种取值：- （1）‘asymmetric’ ：固定的非对称 1.0/topicno 先验 - （2） ‘auto’：根据实际数据学习得到的非对称先验- eta：决定主题词汇狄利克雷先验分布的超参数，可以自行设置为对称的先验分布常量或者长度为词汇总数的向量作为非对称先验，此外也支持以下两种取值：- （1）‘auto’：根据实际数据学习得到的非对称先验 - （2）形如 num_topics x num_words 的矩阵：为每一个主题都引入一个词汇非对称先验分布- minimum_probability：用于限制返回一个文档主题的概率- 利用random_state进行随机化设置##### 延伸一：如何选择iterations 和 passes两个参数：I suggest the following way to choose iterations and passes. First, enable logging (as described in many Gensim tutorials), and set `eval_every = 1` in `LdaModel`. When training the model look for a line in the log that looks something like this:```2016-06-21 15:40:06,753 - gensim.models.ldamodel - DEBUG - 68/1566 documents converged within 400 iterations```If you set `passes = 20` you will see this line 20 times. Make sure that by the final passes, most of the documents have converged. So you want to choose both passes and iterations to be high enough for this to happen.## 2.2 辅助函数解析### 2.2.1 get_document_topics 文章主题偏好```主函数：get_document_topics(bow, minimum_probability=None,   minimum_phi_value=None, per_word_topics=False)```- minimum_probability：忽略概率小于该参数值的主题- per_word_topics：取值为 True 时，同时按照可能性的降序返回词汇对应的主题```model.get_document_topics(corpus[0])>>> [(1, 0.13500942), (3, 0.18280579), (4, 0.1801268), (7, 0.50190312)]```返回某篇文档（corpus编号为0的文档），该篇文章的每个主题分布大致情况，如果太小就没有。### 2.2.2 get_term_topics 单词的主题偏好`get_term_topics(word_id, minimum_probability=None)¶`get_term_topics 方法用于返回词典中指定词汇最有可能对应的主题，调用方式为：实例`.get_term_topics(word_id, minimum_probability=None)`，- word_id 即为指定词汇 id- minimum_probability 为返回主题的最小概率限定ATM模型也有该功能### 2.2.3 get_topic_terms以及get_topics 主题内容展示```model.get_topics()get_topic_terms(topicid, topn=10)```输入主题号，返回重要词以及重要词概率get_topic_terms 方法以（词汇 id，概率）的形式返回指定主题的重要词汇，调用方式为：get_topic_terms(topicid, topn=10)- topicid 即为主题 id- topn 为返回的词汇数。```# 函数一model.get_topic_terms(1, topn=10)>>> [(774, 0.019700538013351386), (3215, 0.0075965808303036916), (3094, 0.0067132528809042526), (514, 0.0063925849599646822), (2739, 0.0054527647598129206), (341, 0.004987335769043616), (752, 0.0046566448210636699), (1218, 0.0046234352422933724), (186, 0.0042132891022475458), (829, 0.0041800479706789939)]# 函数二：>>> array([[  9.57974777e-05,   6.17130780e-07,   6.34938224e-07, ...,          6.17080048e-07,   6.19691132e-07,   6.17090716e-07],       [  9.81065671e-05,   3.12945042e-05,   2.80837858e-04, ...,          7.86879291e-07,   7.86479617e-07,   7.86592758e-07],       [  4.57734625e-05,   1.33555568e-05,   2.55108081e-05, ...,          5.31796854e-07,   5.32000122e-07,   5.31934336e-07],```### 2.2.4 print_topic以及print_topics- model.print_topic(1, topn=10)- print_topics(num_topics=20, num_words=10)结果展示：```# 函数一model.print_topic(1, topn=10)>>> '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'# 函数二model.print_topics(num_topics=20, num_words=10)[(0,  '0.008*"gaussian" + 0.007*"mixture" + 0.006*"density" + 0.006*"matrix" + 0.006*"likelihood" + 0.005*"noise" + 0.005*"component" + 0.005*"prior" + 0.005*"estimate" + 0.004*"log"'), (1,  '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'), (2,  '0.011*"visual" + 0.010*"cell" + 0.009*"response" + 0.008*"field" + 0.008*"motion" + 0.007*"stimulus" + 0.007*"direction" + 0.005*"orientation" + 0.005*"eye" + 0.005*"frequency"')]```### 2.2.5 show_topic以及show_topics- model.show_topic(topicid, topn=10)输入主题号，得到每个主题哪些重要词+重要词概率- model.show_topics(num_topics=10, num_words=10, log=False, formatted=True)每个主题下，重要词等式```show_topic(topicid, topn=10)>>> [('action', 0.013790729946622874), ('control', 0.013754026606322274), ('policy', 0.010037394726575378), ('q', 0.0087439205722043382), ('reinforcement', 0.0087102831394097746), ('optimal', 0.0074764680531377312), ('robot', 0.0057665635437760083), ('controller', 0.0053787501576589725)]# secondmodel.show_topics(num_topics=10)>>> [(0,  '0.014*"action" + 0.014*"control" + 0.010*"policy" + 0.009*"q" + 0.009*"reinforcement" + 0.007*"optimal" + 0.006*"robot" + 0.005*"controller" + 0.005*"dynamic" + 0.005*"environment"'), (1,  '0.020*"image" + 0.008*"face" + 0.007*"cluster" + 0.006*"signal" + 0.005*"source" + 0.005*"matrix" + 0.005*"filter" + 0.005*"search" + 0.004*"distance" + 0.004*"o_o"')]```## 2.3 评价指标主题一致性指标```top_topics = model.top_topics(corpus) tc = sum([t[1] for t in top_topics])```其中top_topics 返回的针对主题的，10个主题 * 2（每个主题重要词概率+一致性指标）：```[([(0.0081142522, 'gaussian'),   (0.0029860872, 'hidden')],  -0.83264680887371556), ([(0.010487712, 'layer'),   (0.0023913214, 'solution')],  -0.96372771081309494)]  ...```其中 tc代表计算了所有主题一致性指标之和，还可以计算平均：```avg_topic_coherence = sum([t[1] for t in top_topics]) / num_topicsprint('Average topic coherence: %.4f.' % avg_topic_coherence)```### 参考：[Python 文本数据分析初学指南](https://www.gitbook.com/book/datartisan/begining-text-mining-with-python/details)**公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)|主题模型|文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性本篇为常规的LDA简单罗列：- Gentle introduction to the LDA model: [http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/](http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/)- Gensim’s LDA API documentation: [https://radimrehurek.com/gensim/models/ldamodel.html](https://radimrehurek.com/gensim/models/ldamodel.html)- Topic modelling in Gensim: [http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html](http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html).# 1 模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|.# 2 函数解析参考自[官网教程 models.ldamodel – Latent Dirichlet Allocation](https://radimrehurek.com/gensim/models/ldamodel.html)## 2.1 主函数解析`class gensim.models.ldamodel.LdaModel(corpus=None, num_topics=100, id2word=None, distributed=False, chunksize=2000, passes=1, update_every=1, alpha='symmetric', eta=None, decay=0.5, offset=1.0, eval_every=10, iterations=50, gamma_threshold=0.001, minimum_probability=0.01, random_state=None, ns_conf=None, minimum_phi_value=0.01, per_word_topics=False, callbacks=None, dtype=<type 'numpy.float32'>)`- corpus：用该参数传入的文档语料将会被用来训练模型，如果不指定该参数，则不进行任何训练，默认后续会调用 update() 方法对模型语料进行更新- num_topics：需要提取的潜在主题数- id2word：用于设置构建模型的词典，决定了词汇数量，id2word = dictionary.id2token- distributed：是否开启分布式计算- chunksize：文件块大小，等同深度学习训练的batch，一次性给入2000篇文章，一次性给入越多，性能越好，该指标会略微影响最终结果- **passes**：等同epoch，how often we train the model on the entire corpus.- **iterations**：it controls how often we repeat a particular loop over each    document.   跟passes相对，每篇文章的loop，其跟passes两个指标都很重要，若把这两个指标设置得足够高，会有好效果。- alpha：决定文档主题狄利克雷先验分布的超参数，默认取值为对称 1.0/num_topics 先验，可以自行设置，也支持以下两种取值：- （1）‘asymmetric’ ：固定的非对称 1.0/topicno 先验 - （2） ‘auto’：根据实际数据学习得到的非对称先验- eta：决定主题词汇狄利克雷先验分布的超参数，可以自行设置为对称的先验分布常量或者长度为词汇总数的向量作为非对称先验，此外也支持以下两种取值：- （1）‘auto’：根据实际数据学习得到的非对称先验 - （2）形如 num_topics x num_words 的矩阵：为每一个主题都引入一个词汇非对称先验分布- minimum_probability：用于限制返回一个文档主题的概率- 利用random_state进行随机化设置##### 延伸一：如何选择iterations 和 passes两个参数：I suggest the following way to choose iterations and passes. First, enable logging (as described in many Gensim tutorials), and set `eval_every = 1` in `LdaModel`. When training the model look for a line in the log that looks something like this:```2016-06-21 15:40:06,753 - gensim.models.ldamodel - DEBUG - 68/1566 documents converged within 400 iterations```If you set `passes = 20` you will see this line 20 times. Make sure that by the final passes, most of the documents have converged. So you want to choose both passes and iterations to be high enough for this to happen.## 2.2 辅助函数解析### 2.2.1 get_document_topics 文章主题偏好```主函数：get_document_topics(bow, minimum_probability=None,   minimum_phi_value=None, per_word_topics=False)```- minimum_probability：忽略概率小于该参数值的主题- per_word_topics：取值为 True 时，同时按照可能性的降序返回词汇对应的主题```model.get_document_topics(corpus[0])>>> [(1, 0.13500942), (3, 0.18280579), (4, 0.1801268), (7, 0.50190312)]```返回某篇文档（corpus编号为0的文档），该篇文章的每个主题分布大致情况，如果太小就没有。### 2.2.2 get_term_topics 单词的主题偏好`get_term_topics(word_id, minimum_probability=None)¶`get_term_topics 方法用于返回词典中指定词汇最有可能对应的主题，调用方式为：实例`.get_term_topics(word_id, minimum_probability=None)`，- word_id 即为指定词汇 id- minimum_probability 为返回主题的最小概率限定ATM模型也有该功能### 2.2.3 get_topic_terms以及get_topics 主题内容展示```model.get_topics()get_topic_terms(topicid, topn=10)```输入主题号，返回重要词以及重要词概率get_topic_terms 方法以（词汇 id，概率）的形式返回指定主题的重要词汇，调用方式为：get_topic_terms(topicid, topn=10)- topicid 即为主题 id- topn 为返回的词汇数。```# 函数一model.get_topic_terms(1, topn=10)>>> [(774, 0.019700538013351386), (3215, 0.0075965808303036916), (3094, 0.0067132528809042526), (514, 0.0063925849599646822), (2739, 0.0054527647598129206), (341, 0.004987335769043616), (752, 0.0046566448210636699), (1218, 0.0046234352422933724), (186, 0.0042132891022475458), (829, 0.0041800479706789939)]# 函数二：>>> array([[  9.57974777e-05,   6.17130780e-07,   6.34938224e-07, ...,          6.17080048e-07,   6.19691132e-07,   6.17090716e-07],       [  9.81065671e-05,   3.12945042e-05,   2.80837858e-04, ...,          7.86879291e-07,   7.86479617e-07,   7.86592758e-07],       [  4.57734625e-05,   1.33555568e-05,   2.55108081e-05, ...,          5.31796854e-07,   5.32000122e-07,   5.31934336e-07],```### 2.2.4 print_topic以及print_topics- model.print_topic(1, topn=10)- print_topics(num_topics=20, num_words=10)结果展示：```# 函数一model.print_topic(1, topn=10)>>> '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'# 函数二model.print_topics(num_topics=20, num_words=10)[(0,  '0.008*"gaussian" + 0.007*"mixture" + 0.006*"density" + 0.006*"matrix" + 0.006*"likelihood" + 0.005*"noise" + 0.005*"component" + 0.005*"prior" + 0.005*"estimate" + 0.004*"log"'), (1,  '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'), (2,  '0.011*"visual" + 0.010*"cell" + 0.009*"response" + 0.008*"field" + 0.008*"motion" + 0.007*"stimulus" + 0.007*"direction" + 0.005*"orientation" + 0.005*"eye" + 0.005*"frequency"')]```### 2.2.5 show_topic以及show_topics- model.show_topic(topicid, topn=10)输入主题号，得到每个主题哪些重要词+重要词概率- model.show_topics(num_topics=10, num_words=10, log=False, formatted=True)每个主题下，重要词等式```show_topic(topicid, topn=10)>>> [('action', 0.013790729946622874), ('control', 0.013754026606322274), ('policy', 0.010037394726575378), ('q', 0.0087439205722043382), ('reinforcement', 0.0087102831394097746), ('optimal', 0.0074764680531377312), ('robot', 0.0057665635437760083), ('controller', 0.0053787501576589725)]# secondmodel.show_topics(num_topics=10)>>> [(0,  '0.014*"action" + 0.014*"control" + 0.010*"policy" + 0.009*"q" + 0.009*"reinforcement" + 0.007*"optimal" + 0.006*"robot" + 0.005*"controller" + 0.005*"dynamic" + 0.005*"environment"'), (1,  '0.020*"image" + 0.008*"face" + 0.007*"cluster" + 0.006*"signal" + 0.005*"source" + 0.005*"matrix" + 0.005*"filter" + 0.005*"search" + 0.004*"distance" + 0.004*"o_o"')]```## 2.3 评价指标主题一致性指标```top_topics = model.top_topics(corpus) tc = sum([t[1] for t in top_topics])```其中top_topics 返回的针对主题的，10个主题 * 2（每个主题重要词概率+一致性指标）：```[([(0.0081142522, 'gaussian'),   (0.0029860872, 'hidden')],  -0.83264680887371556), ([(0.010487712, 'layer'),   (0.0023913214, 'solution')],  -0.96372771081309494)]  ...```其中 tc代表计算了所有主题一致性指标之和，还可以计算平均：```avg_topic_coherence = sum([t[1] for t in top_topics]) / num_topicsprint('Average topic coherence: %.4f.' % avg_topic_coherence)```### 参考：[Python 文本数据分析初学指南](https://www.gitbook.com/book/datartisan/begining-text-mining-with-python/details)**公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|
|[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)|主题模型|文章主题偏好、单词的主题偏好、主题内容展示、主题内容矩阵[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)加入时间因素，不同主题随着时间变动时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性本篇为常规的LDA简单罗列：- Gentle introduction to the LDA model: [http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/](http://blog.echen.me/2011/08/22/introduction-to-latent-dirichlet-allocation/)- Gensim’s LDA API documentation: [https://radimrehurek.com/gensim/models/ldamodel.html](https://radimrehurek.com/gensim/models/ldamodel.html)- Topic modelling in Gensim: [http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html](http://radimrehurek.com/topic_modeling_tutorial/2%20-%20Topic%20Modeling.html).# 1 模型需要材料|材料|解释|示例||----|----|----||corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]||id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|.# 2 函数解析参考自[官网教程 models.ldamodel – Latent Dirichlet Allocation](https://radimrehurek.com/gensim/models/ldamodel.html)## 2.1 主函数解析`class gensim.models.ldamodel.LdaModel(corpus=None, num_topics=100, id2word=None, distributed=False, chunksize=2000, passes=1, update_every=1, alpha='symmetric', eta=None, decay=0.5, offset=1.0, eval_every=10, iterations=50, gamma_threshold=0.001, minimum_probability=0.01, random_state=None, ns_conf=None, minimum_phi_value=0.01, per_word_topics=False, callbacks=None, dtype=<type 'numpy.float32'>)`- corpus：用该参数传入的文档语料将会被用来训练模型，如果不指定该参数，则不进行任何训练，默认后续会调用 update() 方法对模型语料进行更新- num_topics：需要提取的潜在主题数- id2word：用于设置构建模型的词典，决定了词汇数量，id2word = dictionary.id2token- distributed：是否开启分布式计算- chunksize：文件块大小，等同深度学习训练的batch，一次性给入2000篇文章，一次性给入越多，性能越好，该指标会略微影响最终结果- **passes**：等同epoch，how often we train the model on the entire corpus.- **iterations**：it controls how often we repeat a particular loop over each    document.   跟passes相对，每篇文章的loop，其跟passes两个指标都很重要，若把这两个指标设置得足够高，会有好效果。- alpha：决定文档主题狄利克雷先验分布的超参数，默认取值为对称 1.0/num_topics 先验，可以自行设置，也支持以下两种取值：- （1）‘asymmetric’ ：固定的非对称 1.0/topicno 先验 - （2） ‘auto’：根据实际数据学习得到的非对称先验- eta：决定主题词汇狄利克雷先验分布的超参数，可以自行设置为对称的先验分布常量或者长度为词汇总数的向量作为非对称先验，此外也支持以下两种取值：- （1）‘auto’：根据实际数据学习得到的非对称先验 - （2）形如 num_topics x num_words 的矩阵：为每一个主题都引入一个词汇非对称先验分布- minimum_probability：用于限制返回一个文档主题的概率- 利用random_state进行随机化设置##### 延伸一：如何选择iterations 和 passes两个参数：I suggest the following way to choose iterations and passes. First, enable logging (as described in many Gensim tutorials), and set `eval_every = 1` in `LdaModel`. When training the model look for a line in the log that looks something like this:```2016-06-21 15:40:06,753 - gensim.models.ldamodel - DEBUG - 68/1566 documents converged within 400 iterations```If you set `passes = 20` you will see this line 20 times. Make sure that by the final passes, most of the documents have converged. So you want to choose both passes and iterations to be high enough for this to happen.## 2.2 辅助函数解析### 2.2.1 get_document_topics 文章主题偏好```主函数：get_document_topics(bow, minimum_probability=None,   minimum_phi_value=None, per_word_topics=False)```- minimum_probability：忽略概率小于该参数值的主题- per_word_topics：取值为 True 时，同时按照可能性的降序返回词汇对应的主题```model.get_document_topics(corpus[0])>>> [(1, 0.13500942), (3, 0.18280579), (4, 0.1801268), (7, 0.50190312)]```返回某篇文档（corpus编号为0的文档），该篇文章的每个主题分布大致情况，如果太小就没有。### 2.2.2 get_term_topics 单词的主题偏好`get_term_topics(word_id, minimum_probability=None)¶`get_term_topics 方法用于返回词典中指定词汇最有可能对应的主题，调用方式为：实例`.get_term_topics(word_id, minimum_probability=None)`，- word_id 即为指定词汇 id- minimum_probability 为返回主题的最小概率限定ATM模型也有该功能### 2.2.3 get_topic_terms以及get_topics 主题内容展示```model.get_topics()get_topic_terms(topicid, topn=10)```输入主题号，返回重要词以及重要词概率get_topic_terms 方法以（词汇 id，概率）的形式返回指定主题的重要词汇，调用方式为：get_topic_terms(topicid, topn=10)- topicid 即为主题 id- topn 为返回的词汇数。```# 函数一model.get_topic_terms(1, topn=10)>>> [(774, 0.019700538013351386), (3215, 0.0075965808303036916), (3094, 0.0067132528809042526), (514, 0.0063925849599646822), (2739, 0.0054527647598129206), (341, 0.004987335769043616), (752, 0.0046566448210636699), (1218, 0.0046234352422933724), (186, 0.0042132891022475458), (829, 0.0041800479706789939)]# 函数二：>>> array([[  9.57974777e-05,   6.17130780e-07,   6.34938224e-07, ...,          6.17080048e-07,   6.19691132e-07,   6.17090716e-07],       [  9.81065671e-05,   3.12945042e-05,   2.80837858e-04, ...,          7.86879291e-07,   7.86479617e-07,   7.86592758e-07],       [  4.57734625e-05,   1.33555568e-05,   2.55108081e-05, ...,          5.31796854e-07,   5.32000122e-07,   5.31934336e-07],```### 2.2.4 print_topic以及print_topics- model.print_topic(1, topn=10)- print_topics(num_topics=20, num_words=10)结果展示：```# 函数一model.print_topic(1, topn=10)>>> '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'# 函数二model.print_topics(num_topics=20, num_words=10)[(0,  '0.008*"gaussian" + 0.007*"mixture" + 0.006*"density" + 0.006*"matrix" + 0.006*"likelihood" + 0.005*"noise" + 0.005*"component" + 0.005*"prior" + 0.005*"estimate" + 0.004*"log"'), (1,  '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'), (2,  '0.011*"visual" + 0.010*"cell" + 0.009*"response" + 0.008*"field" + 0.008*"motion" + 0.007*"stimulus" + 0.007*"direction" + 0.005*"orientation" + 0.005*"eye" + 0.005*"frequency"')]```### 2.2.5 show_topic以及show_topics- model.show_topic(topicid, topn=10)输入主题号，得到每个主题哪些重要词+重要词概率- model.show_topics(num_topics=10, num_words=10, log=False, formatted=True)每个主题下，重要词等式```show_topic(topicid, topn=10)>>> [('action', 0.013790729946622874), ('control', 0.013754026606322274), ('policy', 0.010037394726575378), ('q', 0.0087439205722043382), ('reinforcement', 0.0087102831394097746), ('optimal', 0.0074764680531377312), ('robot', 0.0057665635437760083), ('controller', 0.0053787501576589725)]# secondmodel.show_topics(num_topics=10)>>> [(0,  '0.014*"action" + 0.014*"control" + 0.010*"policy" + 0.009*"q" + 0.009*"reinforcement" + 0.007*"optimal" + 0.006*"robot" + 0.005*"controller" + 0.005*"dynamic" + 0.005*"environment"'), (1,  '0.020*"image" + 0.008*"face" + 0.007*"cluster" + 0.006*"signal" + 0.005*"source" + 0.005*"matrix" + 0.005*"filter" + 0.005*"search" + 0.004*"distance" + 0.004*"o_o"')]```## 2.3 评价指标主题一致性指标```top_topics = model.top_topics(corpus) tc = sum([t[1] for t in top_topics])```其中top_topics 返回的针对主题的，10个主题 * 2（每个主题重要词概率+一致性指标）：```[([(0.0081142522, 'gaussian'),   (0.0029860872, 'hidden')],  -0.83264680887371556), ([(0.010487712, 'layer'),   (0.0023913214, 'solution')],  -0.96372771081309494)]  ...```其中 tc代表计算了所有主题一致性指标之和，还可以计算平均：```avg_topic_coherence = sum([t[1] for t in top_topics]) / num_topicsprint('Average topic coherence: %.4f.' % avg_topic_coherence)```### 参考：[Python 文本数据分析初学指南](https://www.gitbook.com/book/datartisan/begining-text-mining-with-python/details)**公众号“素质云笔记”定期更新博客内容：**![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}| | | |
|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性| | | | | | | | | | | | |
| | | | | | | | | | | | | | | |
|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]| | | | | | | | | | | | |
|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}| | | | | | | | | | | | |

.

# 2 函数解析

参考自[官网教程 models.ldamodel – Latent Dirichlet Allocation](https://radimrehurek.com/gensim/models/ldamodel.html)

## 2.1 主函数解析
`class gensim.models.ldamodel.LdaModel(corpus=None, num_topics=100, id2word=None, distributed=False, chunksize=2000, passes=1, update_every=1, alpha='symmetric', eta=None, decay=0.5, offset=1.0, eval_every=10, iterations=50, gamma_threshold=0.001, minimum_probability=0.01, random_state=None, ns_conf=None, minimum_phi_value=0.01, per_word_topics=False, callbacks=None, dtype=<type 'numpy.float32'>)`- corpus：用该参数传入的文档语料将会被用来训练模型，如果不指定该参数，则不进行任何训练，默认后续会调用 update() 

方法对模型语料进行更新
- num_topics：需要提取的潜在主题数
- id2word：用于设置构建模型的词典，决定了词汇数量，id2word = dictionary.id2token
- distributed：是否开启分布式计算
- chunksize：文件块大小，等同深度学习训练的batch，一次性给入2000篇文章，一次性给入越多，性能越好，该指标会略微影响最终结果
- **passes**：等同epoch，how often we train the model on the entire corpus.
- **iterations**：it controls how often we repeat a particular loop over 

each    document.   跟passes相对，每篇文章的loop，其跟passes两个指标都很重要，若把这两个指标设置得足够高，会有好效果。
- alpha：决定文档主题狄利克雷先验分布的超参数，默认取值为对称 1.0/num_topics 先验，可以自行设置，也支持以下两种取值：
- （1）‘asymmetric’ ：固定的非对称 1.0/topicno 先验 
- （2） ‘auto’：根据实际数据学习得到的非对称先验
- eta：决定主题词汇狄利克雷先验分布的超参数，可以自行设置为对称的先验分布常量或者长度为词汇总数的向量作为非对称先验，此外也支持以下两种取值：
- （1）‘auto’：根据实际数据学习得到的非对称先验 
- （2）形如 num_topics x num_words 

的矩阵：为每一个主题都引入一个词汇非对称先验分布
- minimum_probability：用于限制返回一个文档主题的概率
- 利用random_state进行随机化设置

##### 延伸一：如何选择iterations 和 passes两个参数：

I suggest the following way to choose iterations and passes. First, enable logging (as described in many Gensim tutorials), and set `eval_every = 1` in `LdaModel`. When training the model look for a line in the log that looks something like this:

```
2016-06-21 15:40:06,753 - gensim.models.ldamodel - DEBUG - 68/1566 documents converged within 400 iterations
```

If you set `passes = 20` you will see this line 20 times. Make sure that by the final passes, most of the documents have converged. So you want to choose both passes and iterations to be high enough for this to happen.

## 2.2 辅助函数解析

### 2.2.1 get_document_topics 文章主题偏好

```
主函数：get_document_topics(bow, minimum_probability=None,
   minimum_phi_value=None, per_word_topics=False)
```
- minimum_probability：忽略概率小于该参数值的主题
- per_word_topics：取值为 True 时，同时按照可能性的降序返回词汇对应的主题

```
model.get_document_topics(corpus[0])
>>> [(1, 0.13500942), (3, 0.18280579), (4, 0.1801268), (7, 0.50190312)]
```

返回某篇文档（corpus编号为0的文档），该篇文章的每个主题分布大致情况，如果太小就没有。

### 2.2.2 get_term_topics 单词的主题偏好
`get_term_topics(word_id, minimum_probability=None)¶`
get_term_topics 方法用于返回词典中指定词汇最有可能对应的主题，调用方式为：实例`.get_term_topics(word_id, minimum_probability=None)`，
- word_id 即为指定词汇 id
- minimum_probability 为返回主题的最小概率限定

ATM模型也有该功能

### 2.2.3 get_topic_terms以及get_topics 主题内容展示

```
model.get_topics()
get_topic_terms(topicid, topn=10)
```

输入主题号，返回重要词以及重要词概率

get_topic_terms 方法以（词汇 id，概率）的形式返回指定主题的重要词汇，调用方式为：get_topic_terms(topicid, topn=10)
- topicid 即为主题 id
- topn 为返回的词汇数

。

```
# 函数一
model.get_topic_terms(1, topn=10)
>>> [(774, 0.019700538013351386),
 (3215, 0.0075965808303036916),
 (3094, 0.0067132528809042526),
 (514, 0.0063925849599646822),
 (2739, 0.0054527647598129206),
 (341, 0.004987335769043616),
 (752, 0.0046566448210636699),
 (1218, 0.0046234352422933724),
 (186, 0.0042132891022475458),
 (829, 0.0041800479706789939)]

# 函数二：
>>> array([[  9.57974777e-05,   6.17130780e-07,   6.34938224e-07, ...,
          6.17080048e-07,   6.19691132e-07,   6.17090716e-07],
       [  9.81065671e-05,   3.12945042e-05,   2.80837858e-04, ...,
          7.86879291e-07,   7.86479617e-07,   7.86592758e-07],
       [  4.57734625e-05,   1.33555568e-05,   2.55108081e-05, ...,
          5.31796854e-07,   5.32000122e-07,   5.31934336e-07],
```

### 2.2.4 print_topic以及print_topics
- model.print_topic(1, topn=10)
- print_topics(num_topics=20, num_words=10)

结果展示：

```
# 函数一
model.print_topic(1, topn=10)
>>> '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'

# 函数二
model.print_topics(num_topics=20, num_words=10)
[(0,
  '0.008*"gaussian" + 0.007*"mixture" + 0.006*"density" + 0.006*"matrix" + 0.006*"likelihood" + 0.005*"noise" + 0.005*"component" + 0.005*"prior" + 0.005*"estimate" + 0.004*"log"'),
 (1,
  '0.025*"image" + 0.010*"object" + 0.008*"distance" + 0.007*"recognition" + 0.005*"pixel" + 0.004*"cluster" + 0.004*"class" + 0.004*"transformation" + 0.004*"constraint" + 0.004*"map"'),
 (2,
  '0.011*"visual" + 0.010*"cell" + 0.009*"response" + 0.008*"field" + 0.008*"motion" + 0.007*"stimulus" + 0.007*"direction" + 0.005*"orientation" + 0.005*"eye" + 0.005*"frequency"')]
```

### 2.2.5 show_topic以及show_topics
- model.show_topic(topicid, topn=10)

输入主题号，得到每个主题哪些重要词+重要词概率
- model.show_topics(num_topics=10, num_words=10, log=False, 

formatted=True)

每个主题下，重要词等式

```
show_topic(topicid, topn=10)
>>> [('action', 0.013790729946622874),
 ('control', 0.013754026606322274),
 ('policy', 0.010037394726575378),
 ('q', 0.0087439205722043382),
 ('reinforcement', 0.0087102831394097746),
 ('optimal', 0.0074764680531377312),
 ('robot', 0.0057665635437760083),
 ('controller', 0.0053787501576589725)]

# second
model.show_topics(num_topics=10)
>>> [(0,
  '0.014*"action" + 0.014*"control" + 0.010*"policy" + 0.009*"q" + 0.009*"reinforcement" + 0.007*"optimal" + 0.006*"robot" + 0.005*"controller" + 0.005*"dynamic" + 0.005*"environment"'),
 (1,
  '0.020*"image" + 0.008*"face" + 0.007*"cluster" + 0.006*"signal" + 0.005*"source" + 0.005*"matrix" + 0.005*"filter" + 0.005*"search" + 0.004*"distance" + 0.004*"o_o"')]
```

## 2.3 评价指标

主题一致性指标

```
top_topics = model.top_topics(corpus)
 tc = sum([t[1] for t in top_topics])
```

其中top_topics 返回的针对主题的，10个主题 * 2（每个主题重要词概率+一致性指标）：

```
[([(0.0081142522, 'gaussian'),   (0.0029860872, 'hidden')],
  -0.83264680887371556),
 ([(0.010487712, 'layer'),   (0.0023913214, 'solution')],
  -0.96372771081309494)]
  ...
```

其中 tc代表计算了所有主题一致性指标之和，还可以计算平均：

```
avg_topic_coherence = sum([t[1] for t in top_topics]) / num_topics
print('Average topic coherence: %.4f.' % avg_topic_coherence)
```

### 参考：

[Python 文本数据分析初学指南](https://www.gitbook.com/book/datartisan/begining-text-mining-with-python/details)


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




