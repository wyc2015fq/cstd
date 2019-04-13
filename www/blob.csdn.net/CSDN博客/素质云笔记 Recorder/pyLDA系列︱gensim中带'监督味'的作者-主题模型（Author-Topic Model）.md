
# pyLDA系列︱gensim中带'监督味'的作者-主题模型（Author-Topic Model） - 素质云笔记-Recorder... - CSDN博客

2018年02月20日 15:37:39[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5489


笔者很早就对LDA模型着迷，最近在学习gensim库发现了LDA比较有意义且项目较为完整的Tutorials,于是乎就有本系列,本系列包含三款：Latent Dirichlet Allocation、Author-Topic Model、Dynamic Topic Models
|pyLDA系列模型|解析|功能|
|---|---|---|
|[ATM模型（Author-Topic Model）](http://blog.csdn.net/sinat_26917383/article/details/79339727)|加入监督的’作者’,每个作者对不同主题的偏好;弊端：|chained topics|,|intruded words|,|random topics|, and|unbalanced topics|(see|[Mimno and co-authors 2011](https://people.cs.umass.edu/~wallach/publications/mimno11optimizing.pdf)|)|作者主题偏好、
|词语主题偏好、
|相似作者推荐、
|可视化
|
|[LDA模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)|主题模型|文章主题偏好、
|单词的主题偏好、
|主题内容展示、
|主题内容矩阵|
|[DTM模型（Dynamic Topic Models）](http://mp.blog.csdn.net/mdeditor/79377761)|加入时间因素，不同主题随着时间变动|时间-主题词条矩阵、
|主题-时间词条矩阵、文档主题偏好、新文档预测、跨时间+主题属性的文档相似性|
|案例与数据主要来源，jupyter notebook可见|[gensim的官方github](https://radimrehurek.com/gensim/models/atmodel.html)
|详细参数解释：|[Author-topic models](https://radimrehurek.com/gensim/models/atmodel.html)
|.
|1、理论介绍
|[Author Topic Model解析](http://blog.csdn.net/chinaliping/article/details/9299953)
|2、Author-Topic Model数据准备
|2.1 元数据
|作者|文章|
|---|---|
|张三|文章1|
|李四|文章2,文章3,文章4|
|王五|文章5|
|…|…|
|2.2 模型需要材料
|材料|解释|示例|
|---|---|---|
|corpus|用过gensim 都懂|[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2)]]|
|dictionary|用过gensim 都懂,dictionary = Dictionary(docs)|docs的格式,每篇文章都变成如下样式,然后整入List之中：[‘probabilistic’, ‘characterization’, ‘of’, ‘neural’, ‘model’, ‘computation’, ‘richard’,’david_rumelhart’, ‘-PRON-_grateful’, ‘helpful_discussion’, ‘early_version’, ‘like_thank’, ‘brown_university’]|
|id2word|每个词语ID的映射表，dictionary构成，id2word = dictionary.id2token|{0: ’ 0’, 1: ’ American nstitute of Physics 1988  ‘, 2: ’ Fig’, 3: ’ The’, 4: ‘1 1’, 5: ‘2 2’, 6: ‘2 3’, 7: ‘CA 91125  ‘, 8: ‘CONCLUSIONS  ‘}|
|author2doc|作者到每个文档ID的映射表,dict,人名+代表作的ID,字典形式|{‘RobertA.Jacobs’: [252, 429], ‘G.J.Goodhill’: [1007], ‘MichaelHumphreys’: [323]}|
|doc2author|从每个文档的作者映射表,author2doc 倒转|
|2.3 案例中spacy的使用
|下面的案例是官网的案例，所以英文中使用spacy进行分词和清洗，使用的时候需要额外加载一些model，因为网络问题，一个30MB+的内容老是timeout，尼玛！
|参考spacy官方Link:|[https://spacy.io/usage/models\#download-pip](https://spacy.io/usage/models#download-pip)
|第一种方式：
|python -m spacy download en|\# 如果没有翻墙会很慢|使用的话就是：
|import spacy
nlp = spacy.|load|(|'en'|)|第二种方式：
|1.先到该|[网站](https://github.com/explosion/spacy-models/releases)|下载tar包
|就是这个|[download link](https://github.com/explosion/spacy-models/releases/download/en_core_web_md-2.0.0/en_core_web_md-2.0.0.tar.gz)
|2.解压
|!pip install /Users/you/en_core_web_md-2.0.0.tar.gz
|3.使用
|import|en_core_web_sm
nlp = en_core_web_sm.load()
nlp(|u'This is a sentence.'|)|.
|3、官网案例demo
|3.1 主函数介绍
|3.1.1 AuthorTopicModel主函数:
|class gensim.models.atmodel.AuthorTopicModel(corpus=|None|, num_topics=|100|, id2word=|None|, author2doc=|None|, doc2author=|None|, chunksize=|2000|, passes=|1|, iterations=|50|, decay=|0.5|, offset=|1.0|, alpha=|'symmetric'|, eta=|'symmetric'|, update_every=|1|, eval_every=|10|, gamma_threshold=|0.001|, serialized=|False|, serialization_path=|None|, minimum_probability=|0.01|, random_state=|None|)|与LDA相同的一些参数：
|corpus、passes、iterations、chunksize、eval_every、alpha/eta、random_state，详细解析可见：|[pyLDA系列︱gensim中的主题模型（Latent Dirichlet Allocation）](http://blog.csdn.net/sinat_26917383/article/details/79357700)
|不同的参数：
|id2word,文档ID到词语的映射表，id2word,id2word=dictionary.id2token,由字典生成
|author2doc,作者到每个文档ID的映射表，author2doc
|doc2author,author2doc的反向表征,从每个文档的作者映射表
|最简训练模式：
|model|=|AuthorTopicModel(corpus, num_topics=10, author2doc=author2doc, id2word=id2word)|最简增量训练模式：
|\# update主函数|update(corpus=|None|, author2doc=|None|, doc2author=|None|, chunksize=|None|, decay=|None|, offset=|None|, passes=|None|, update_every=|None|, eval_every=|None|, iterations=|None|, gamma_threshold=|None|, chunks_as_numpy=|False|)|\# 最简方式|model.update(other_corpus, other_author2doc)|延伸一：利用random_state进行随机化设置
|%%|time|model_list = []|for|i|in|range(|5|):
    model = AuthorTopicModel(corpus=corpus, num_topics=|10|, id2word=dictionary.id2token, \
                    author2doc=author2doc, chunksize=|2000|, passes=|100|, gamma_threshold=|1e-10|, \
                    eval_every=|0|, iterations=|1|, random_state=i)
    top_topics = model.top_topics(corpus)
    tc =|sum|([t[|1|]|for|t|in|top_topics])
    model_list.append((model, tc))|通过设置该参数random_state,不同的随机种子,并选择具有最高主题一致性的模型。
|top_topics 代表的指标为主题一致性。
|.
|延伸二：参数serialized、serialization_path
|serialized打开之后，可以把该模型中的corpus语料，以MmCorpus格式保存到serialization_path设置的指定目录之中。
|%time model_ser = AuthorTopicModel(corpus=corpus,id2word=dictionary.id2token,                               author2doc=author2doc,  serialized=|True|,serialization_path=|'/tmp/model_serialization.mm'|)|\# 序列化模型存放|serialized=True|之后，运行之后会自动把corpus保存。常规的corpus序列化方式：|corpora.MmCorpus.serialize('/tmp/model_serialization.mm', corpus)
|下次在想使用的时候，可以这么反序列：
|new_corpus = corpora.MmCorpus(|'/tmp/model_serialization.mm'|)|3.1.2 辅助函数
|主题展示函数六个：
|model.show_topic(topicid, topn=10) 输入主题号，得到每个主题哪些重要词+重要词概率
|model.show_topics(num_topics=10, num_words=10, log=False, formatted=True) 每个主题下，重要词等式
|model.get_topic_terms(topicid, topn=10) 输入主题号，返回重要词以及重要词概率
|model.get_topics() 返回主题数|字数的矩阵，10主题|7674个单词
|（|不计入|）get_document_topics ，get_document_topics(word_id, minimum_probability=None),该参数适合LDA，并不适用在ATM模型之中。Method “get_document_topics” is not valid for the author-topic model.
|五：model.print_topic(1, topn=10)
|六：print_topics(num_topics=20, num_words=10)
|\# first
show_topic(topicid, topn=|10|)
>>> [(|'action|',|0.013790729946622874|),
 (|'control|',|0.013754026606322274|),
 (|'policy|',|0.010037394726575378|),
 (|'q|',|0.0087439205722043382|),
 (|'reinforcement|',|0.0087102831394097746|),
 (|'optimal|',|0.0074764680531377312|),
 (|'robot|',|0.0057665635437760083|),
 (|'controller|',|0.0053787501576589725|)]
\# second
model.show_topics(num_topics=|10|)
>>> [(|0|,
  '|0.014|*|"action"|+|0.014|*|"control"|+|0.010|*|"policy"|+|0.009|*|"q"|+|0.009|*|"reinforcement"|+|0.007|*|"optimal"|+|0.006|*|"robot"|+|0.005|*|"controller"|+|0.005|*|"dynamic"|+|0.005|*|"environment"|'),
 (|1|,
  '|0.020|*|"image"|+|0.008|*|"face"|+|0.007|*|"cluster"|+|0.006|*|"signal"|+|0.005|*|"source"|+|0.005|*|"matrix"|+|0.005|*|"filter"|+|0.005|*|"search"|+|0.004|*|"distance"|+|0.004|*|"o_o"|')]
\# third
model.get_topic_terms(|1|, topn=|10|)
>>> [(|774|,|0.019700538013351386|),
 (|3215|,|0.0075965808303036916|),
 (|3094|,|0.0067132528809042526|),
 (|514|,|0.0063925849599646822|),
 (|2739|,|0.0054527647598129206|),
 (|341|,|0.004987335769043616|),
 (|752|,|0.0046566448210636699|),
 (|1218|,|0.0046234352422933724|),
 (|186|,|0.0042132891022475458|),
 (|829|,|0.0041800479706789939|)]
\#forth
model.get_topics()
>>>|array|([[|9.57974777e-05|,|6.17130780e-07|,|6.34938224e-07|, ...,|6.17080048e-07|,|6.19691132e-07|,|6.17090716e-07|],
       [|9.81065671e-05|,|3.12945042e-05|,|2.80837858e-04|, ...,|7.86879291e-07|,|7.86479617e-07|,|7.86592758e-07|],
       [|4.57734625e-05|,|1.33555568e-05|,|2.55108081e-05|, ...,|5.31796854e-07|,|5.32000122e-07|,|5.31934336e-07|],
\# 五
model.print_topic(|1|, topn=|10|)
>>> '|0.025|*|"image"|+|0.010|*|"object"|+|0.008|*|"distance"|+|0.007|*|"recognition"|+|0.005|*|"pixel"|+|0.004|*|"cluster"|+|0.004|*|"class"|+|0.004|*|"transformation"|+|0.004|*|"constraint"|+|0.004|*|"map"|'
\# 六
model.print_topics(num_topics=|20|, num_words=|10|)
[(|0|,
  '|0.008|*|"gaussian"|+|0.007|*|"mixture"|+|0.006|*|"density"|+|0.006|*|"matrix"|+|0.006|*|"likelihood"|+|0.005|*|"noise"|+|0.005|*|"component"|+|0.005|*|"prior"|+|0.005|*|"estimate"|+|0.004|*|"log"|'),
 (|1|,
  '|0.025|*|"image"|+|0.010|*|"object"|+|0.008|*|"distance"|+|0.007|*|"recognition"|+|0.005|*|"pixel"|+|0.004|*|"cluster"|+|0.004|*|"class"|+|0.004|*|"transformation"|+|0.004|*|"constraint"|+|0.004|*|"map"|'),
 (|2|,
  '|0.011|*|"visual"|+|0.010|*|"cell"|+|0.009|*|"response"|+|0.008|*|"field"|+|0.008|*|"motion"|+|0.007|*|"stimulus"|+|0.007|*|"direction"|+|0.005|*|"orientation"|+|0.005|*|"eye"|+|0.005|*|"frequency"|')]|作者偏好主题函数两个：
|model[‘name’]
|model.get_author_topics(‘name’)
|model.id2author.values()  模型作者列表
|model.id2author.values()|\#作者列表|>>>|dict_values([|'A.A.Handzel'|,|'A.Afghan'|,|'A.B.Bonds'|,|'A.B.Kirillov'|,|'A.Blake'|,|'A.C.C.Coolen'|,|'A.C.Tsoi'|,|'A.Cichocki'|,|'A.D.Back'|,|'A.D.Rexlish'|,|'A.Dembo'|,|'A.Drees'|,|'A.During'|,|'A.E.Friedman'|,|'A.F.Murray'|,|'A.Ferguson'|,|'A.G.Barto'|,|'A.G.U.Perera'|]|model[|'GeoffreyE.Hinton'|]
model.get_author_topics(|'GeoffreyE.Hinton'|)|>>>|[(|6|,|0.76808063951144978|), (|7|,|0.23181972762044473|)]|两个函数返回的内容都一样，表示：该作者（GeoffreyE.Hinton）偏好第6、7号主题，主题分别是概率。
|单词的主题偏好:get_term_topics()
|返回词典中指定词汇最有可能对应的主题
|[(model.get_term_topics(i),dictionary.id2token[i])|for|i|in|range(|100|)]|>>>|[([],|'acknowledgements'|),
 ([],|'acknowledgements_-PRON-'|),
 ([],|'acquire'|),
 ([(|0|,|0.013787687427660612|)],|'action'|),
 ([],|'action_potential'|),
 ([],|'active'|)]|其中单词’action’，更偏好于0号主题（’Circuits’），可能性为0.013
|3.1.3 衡量指标
|两个ATM   模型的衡量指标|单词边界（perwordbound）、一致性指标（top_topics）|：
|\# 指标一：perwordbound|from gensim|.models|import atmodel
doc2author = atmodel|.construct|_doc2author(model|.corpus|, model|.author|2doc)|\# Compute the per-word bound.|\# Number of words in corpus.|corpus_words = sum(cnt for document|in|model|.corpus|for _, cnt|in|document)|\# Compute bound and divide by number of words.|perwordbound = model|.bound|(model|.corpus|, author2doc=model|.author|2doc, \
                           doc2author=model|.doc|2author) / corpus_words
print(perwordbound)|\# 指标二：话题一致性指标计算|%time top_topics = model|.top|_topics(model|.corpus|)
tc = sum([t[|1|] for t|in|top_topics])|用的是话题的一致性指标，用于模型选择以及不同主题内容评估。
|其中top_topics 返回的针对主题的，10个主题 * 2（每个主题重要词概率+一致性指标）：
|[([(|0.0081142522|,|'gaussian'|),   (|0.0029860872|,|'hidden'|)],
  -|0.83264680887371556|),
 ([(|0.010487712|,|'layer'|),   (|0.0023913214|,|'solution'|)],
  -|0.96372771081309494|)]|...|其中 tc代表计算了所有主题一致性指标之和，还可以计算平均：
|avg_topic_coherence = sum([t[|1|]|for|t|in|top_topics]) / num_topics|print|(|'Average topic coherence: %.4f.'|% avg_topic_coherence)|3.1.4 相似作者推荐
|官方案例中包括两种相似性距离：|cos距离、Hellinger距离
|第一种：自带、常规的cos距离
|from gensim.similarities import MatrixSimilarity|\# Generate a similarity object for the transformed corpus.|index = MatrixSimilarity(model[list(model.id2author.values())])|\# Get similarities to some author.|author_name =|'YannLeCun'|sims = index[model[author_name]]
sims 
>>> array([|0.20777275|,|0.8723157|,|0.|,|...|,|0.16174853|,|0.07938839|,|0.|], dtype=float32)|\#sims的列表是，'YannLeCun'作者跟其他每个作者，主题偏好向量的cos距离|其中|model[list(model.id2author.values())]|中，model.id2author是作者姓名的列表，model[姓名列表]代表每个作者-主题偏好列表向量，当然不定长，有的作者对某些主题没有一点关联，就会缺失。
|主题偏好向量为：
|[(4, 0.88636616132828339), (8, 0.083545096138703312)],
 [(4, 0.27129746825443646), (8, 0.71594003971848896)],
 [(0, 0.07188868711639794),  (1, 0.069390116586544176),
  (3, 0.035190795872695843),  (4, 0.011718365474455844),
  (5, 0.058831820905365088),  (6, 0.68542799691757561),
  (9, 0.041390087371285786)]|譬如|[(4, 0.88636616132828339), (8, 0.083545096138703312)]|代表某作者，偏好于4/8号主题。
|第二种：Hellinger距离
|官方自定义函数：
|from|gensim|import|matutils|import|pandas|as|pd|\# Make a list of all the author-topic distributions.|\# 作者-主题偏好向量|author_vecs = [model.get_author_topics(author)|for|author|in|model.id2author.values()]|def|similarity|(vec1, vec2)|:|'''Get similarity between two vectors'''|dist = matutils.hellinger(matutils.sparse2full(vec1, model.num_topics), \
                              matutils.sparse2full(vec2, model.num_topics))
    sim =|1.0|/ (|1.0|+ dist)|return|sim|def|get_sims|(vec)|:|'''Get similarity of vector to all authors.'''|sims = [similarity(vec, vec2)|for|vec2|in|author_vecs]|return|sims|def|get_table|(name, top_n=|10|, smallest_author=|1|)|:|'''
    Get table with similarities, author names, and author sizes.
    Return `top_n` authors as a dataframe.
    '''|\# Get similarities.|sims = get_sims(model.get_author_topics(name))|\# Arrange author names, similarities, and author sizes in a list of tuples.|table = []|for|elem|in|enumerate(sims):
        author_name = model.id2author[elem[|0|]]
        sim = elem[|1|]
        author_size = len(model.author2doc[author_name])|if|author_size >= smallest_author:
            table.append((author_name, sim, author_size))|\# Make dataframe and retrieve top authors.|df = pd.DataFrame(table, columns=[|'Author'|,|'Score'|,|'Size'|])
    df = df.sort_values(|'Score'|, ascending=|False|)[:top_n]|return|df|可以用|get_table('YannLeCun', top_n=10,smallest_author=3)|得到’YannLeCun’作者，与其他每一个作者的Hellinger距离。
|.
|3.2 官方案例:基于ATM模型的NIPS科技论文作者-主题分布及偏好探究
|3.2.1 数据下载
|!wget -|O|-|'http://www.cs.nyu.edu/~roweis/data/nips12raw_str602.tgz'|>|/tmp/nips|12raw_str602.tgz
>>>|100|%[===================>]|12.26|M|25.1|KB/s|in|11|m|34|s|3.2.2 数据整理与形成作者-文档映射表
|import tarfile
import os, re
\# 数据解压
filename = '/tmp/nips12raw_str602.tgz'
tar = tarfile.open(filename, 'r:gz')
for item in tar:
    tar.extract(item, path='/tmp')
\# Folder containing all NIPS papers.
data_dir = '/tmp/nipstxt/'  \#|Set|this path|to|the data|on|your machine.
\# Folders containin individual NIPS papers.
yrs = [|'00'|,|'01'|,|'02'|,|'03'|,|'04'|,|'05'|,|'06'|,|'07'|,|'08'|,|'09'|,|'10'|,|'11'|,|'12'|]
dirs = [|'nips'|+ yr|for|yr|in|yrs]
\# [|'nips00'|,|'nips01'|,|'nips02'|,|'nips03'|,|'nips04'|,|'nips05'|,|'nips06'|,|'nips07'|,|'nips08'|,|'nips09'|,|'nips10'|,|'nips11'|,|'nips12'|]
\#|Get|all|document texts|and|their|corresponding|IDs.
\# doc_ids,list+str,第几届NIPS+|'_'|+第几篇,|'07_713'|,用来准备author2doc的材料
docs = []
doc_ids = []|for|yr_dir|in|dirs:
    files = os.listdir(data_dir + yr_dir)  \# List|of|filenames.|for|filen|in|files:
        \#|Get|document ID.
        (idx1, idx2) = re.search(|'[0-9]+'|, filen).span()  \# Matches the indexes|of|the|start|end|end|of|the ID.
        doc_ids.append(yr_dir[|4|:] +|'_'|+ str(|int|(filen[idx1:idx2])))
        \#|Read|document text.
        \# Note: ignoring characters that cause encoding errors.|with|open|(data_dir + yr_dir +|'/'|+ filen, errors=|'ignore'|, encoding=|'utf-8'|)|as|fid:
            txt = fid.|read|()
        \#|Replace|any|whitespace (newline, tabs, etc.)|by|a single|space|.
        txt = re.sub(|'\s'|,|' '|, txt)
        docs.append(txt)
\# 制作author2doc,作者-文档映射表
filenames = [data_dir +|'idx/a'|+ yr +|'.txt'|for|yr|in|yrs]  \#|Using|the years defined|in|previous cell.
\#|Get|all|author|names|and|their|corresponding|document IDs.
author2doc = dict()
i =|0|for|yr|in|yrs:
    \# The files|"a00.txt"|and|so|on|contain the author-document mappings.
    filename = data_dir +|'idx/a'|+ yr +|'.txt'|for|line|in|open|(filename, errors=|'ignore'|, encoding=|'utf-8'|):
        \#|Each|line corresponds|to|one author.
        contents = re.split(|','|, line)
        author_name = (contents[|1|] + contents[|0|]).strip()
        \# Remove|any|whitespace|to|reduce redundant author|names|.
        author_name = re.sub(|'\s'|,|''|, author_name)
        \#|Get|document IDs|for|author.
        ids = [c.strip()|for|c|in|contents[|2|:]]|if|not|author2doc.|get|(author_name):
            \# This|is|a new author.
            author2doc[author_name] = []
            i +=|1|\#|Add|document IDs|to|author.
        author2doc[author_name].extend([yr +|'_'|+ id|for|id|in|ids])
\# Use an|integer|ID|in|author2doc, instead|of|the IDs provided|in|the NIPS dataset.
\# Mapping|from|ID|of|document|in|NIPS datast,|to|an|integer|ID.
doc_id_dict = dict(zip(doc_ids, range(len(doc_ids))))
\#|Replace|NIPS IDs|by|integer|IDs.|for|a, a_doc_ids|in|author2doc.items():|for|i, doc_id|in|enumerate(a_doc_ids):
        author2doc[a][i] = doc_id_dict[doc_id]|3.2.3 数据清洗与分词
|\# 数据处理
\# 分词,同时在后面加入2-gram,现在每篇文章变成了:word1,word2,word3,..,word1_word2,word2_word3...这样的形式
import en_core_web_sm
import spacy
nlp = en_core_web_sm.|load|()
nlp(u|'This is a sentence.'|)
%%|time|processed_docs = []|for|doc|in|nlp.pipe(docs, n_threads=|4|, batch_size=|100|):
    \# Process document|using|Spacy NLP pipeline.
    ents = doc.ents  \# Named entities.
    \# Keep|only|words (|no|numbers,|no|punctuation).
    \# Lemmatize tokens, remove punctuation|and|remove stopwords.
    doc = [token.lemma_|for|token|in|doc|if|token.is_alpha|and|not|token.is_stop]
    \# Remove common words|from|a stopword list.
    \#doc = [token|for|token|in|doc|if|token|not|in|STOPWORDS]
    \#|Add|named entities, but|only|if|they|are|a compound|of|more than word.
    doc.extend([str(entity)|for|entity|in|ents|if|len(entity) >|1|])
    processed_docs.append(doc)
\# Compute bigrams.  
\#|'neural_networks'|from|gensim.models import Phrases
\#|Add|bigrams|and|trigrams|to|docs (|only|ones that appear|20|times|or|more).
bigram = Phrases(docs, min_count=|20|)|for|idx|in|range(len(docs)):|for|token|in|bigram[docs[idx]]:|if|'_'|in|token:
            \# Token|is|a bigram,|add|to|document.
            docs[idx].append(token)|3.2.4 构建模型语料
|\# Create a dictionary representation|of|the documents,|and|filter|out|frequent|and|rare words.
from gensim.corpora import Dictionary
dictionary = Dictionary(docs)
\# Remove rare|and|common tokens.
\# Filter|out|words that occur too frequently|or|too rarely.
max_freq =|0.5|min_wordcount =|20|dictionary.filter_extremes(no_below=min_wordcount, no_above=max_freq)
_ = dictionary[|0|]  \# This sort|of|"initializes"|dictionary.id2token.
\# Bag-|of|-words representation|of|the documents.
corpus = [dictionary.doc2bow(doc)|for|doc|in|docs]
print(|'Number|of|authors: %d' % len(author2doc))
print(|'Number|of|unique tokens: %d' % len(dictionary))
print(|'Number|of|documents: %d' % len(corpus))|3.2.5 模型训练与选择
|\# 模型训练|from|gensim.models import AuthorTopicModel
%|time|model = AuthorTopicModel(corpus=corpus, num_topics=|10|, id2word=dictionary.id2token, \
                author2doc=author2doc, chunksize=|2000|, passes=|1|, eval_every=|0|, \
                iterations=|1|, random_state=|1|)|\# 模型选择|\#如果你觉得模型没有收敛，那么可以采用增量训练,`model.update(corpus, author2doc)`。|\#通过设置该参数random_state,不同的随机种子,并选择具有最高主题一致性的模型。|%%|time|model_list = []|for|i|in|range(|5|):
    model = AuthorTopicModel(corpus=corpus, num_topics=|10|, id2word=dictionary.id2token, \
                    author2doc=author2doc, chunksize=|2000|, passes=|100|, gamma_threshold=|1e-10|, \
                    eval_every=|0|, iterations=|1|, random_state=i)
    top_topics = model.top_topics(corpus)
    tc =|sum|([t[|1|]|for|t|in|top_topics])
    model_list.append((model, tc))|\# 模型评估:主题一致性|model, tc =|max|(model_list, key=lambda x: x[|1|])
print(|'Topic coherence: %.3e'|%tc)|其中需要了解一下模型的保存与载入：
|\# 模型保存|model.save(|'/mnt/gensim/lda/model.atmodel'|)
model = AuthorTopicModel.load(|'/mnt/gensim/lda/model.atmodel'|)|3.2.6 模型评估指标:per-word bound与话题一致性
|\# 每词单词边界指标|from gensim|.models|import atmodel
doc2author = atmodel|.construct|_doc2author(model|.corpus|, model|.author|2doc)|\# Compute the per-word bound.|\# Number of words in corpus.|corpus_words = sum(cnt for document|in|model|.corpus|for _, cnt|in|document)|\# Compute bound and divide by number of words.|perwordbound = model|.bound|(model|.corpus|, author2doc=model|.author|2doc, \
                           doc2author=model|.doc|2author) / corpus_words
print(perwordbound)|\# 话题一致性指标计算|%time top_topics = model|.top|_topics(model|.corpus|)|3.3 模型使用与展示
|3.3.1  模型基本使用
|\# 主题内容展示|model.show_topic(|0|)|\# 自定义每个主题主要内容|topic_labels = [|'Circuits'|,|'Neuroscience'|,|'Numerical optimization'|,|'Object recognition'|, \|'Math/general'|,|'Robotics'|,|'Character recognition'|, \|'Reinforcement learning'|,|'Speech recognition'|,|'Bayesian modelling'|]|\# 查看每个主题下都有哪些词语|for|topic|in|model.show_topics(num_topics=|10|):
    print(|'Label: '|+ topic_labels[topic[|0|]])
    words =|''|for|word, prob|in|model.show_topic(topic[|0|]):
        words += word +|' '|print(|'Words: '|+ words)
    print()|'''
Label: Circuits
Words: action control policy q reinforcement optimal robot controller dynamic environment 
Label: Neuroscience
Words: image face cluster signal source matrix filter search distance o_o 
'''|\# 查看某作者偏好主题内容|model[|'YannLeCun'|]|from|pprint|import|pprint|def|show_author|(name)|:|print(|'\n%s'|% name)
    print(|'Docs:'|, model.author2doc[name])
    print(|'Topics:'|)
    pprint([(topic_labels[topic[|0|]], topic[|1|])|for|topic|in|model[name]])|\# 作者的主要文章有哪些，话题有那个|show_author(|'YannLeCun'|)|打印出来为：
|YannLeCun|Docs|:|[183, 355, 288, 515, 529, 544, 679, 662, 711, 720, 1512]|Topics|:|[('Reinforcement learning', 0.99977836093843353)]|其中大牛YannLeCun，更偏向于强化学习主题。
|3.3.2 作者-主题对应图t-SNE
|这里有一个参数:smallest_author,可以将一些长尾作者进行剔除
|%%|time|from|sklearn.manifold import TSNE
tsne = TSNE(n_components=|2|, random_state=|0|)
smallest_author =|0|\# Ignore authors with documents less than this.|authors = [model.author2id[|a|]|for|a|in|model.author2id.|keys|()|if|len|(model.author2doc[|a|]) >= smallest_author]
_ = tsne.fit_transform(model.state.gamma[authors, :])|\# Result stored in tsne.embedding_|\# Tell Bokeh to display plots inside the notebook.|from|bokeh.io import output_notebook|\#!pip3 install bokeh|output_notebook()|from|bokeh.models import HoverTool|from|bokeh.plotting import figure, show, ColumnDataSource
x = tsne.embedding_[:,|0|]
y = tsne.embedding_[:,|1|]
author_names = [model.id2author[|a|]|for|a|in|authors]|\# Radius of each point corresponds to the number of documents attributed to that author.|scale =|0.1|author_sizes = [|len|(model.author2doc[|a|])|for|a|in|author_names]
radii = [size * scale|for|size|in|author_sizes]
source = ColumnDataSource(
        data=dict(
            x=x,
            y=y,
            author_names=author_names,
            author_sizes=author_sizes,
            radii=radii,
        )
    )|\# Add author names and sizes to mouse-over info.|hover = HoverTool(
        tooltips=[
        (|"author"|,|"@author_names"|),
        (|"size"|,|"@author_sizes"|),
        ]
    )
p = figure(tools=[hover,|'crosshair,pan,wheel_zoom,box_zoom,reset,save,lasso_select'|])
p.scatter(|'x'|,|'y'|, radius=|'radii'|, source=source, fill_alpha=|0.6|, line_color=None)
show(p)![这里写图片描述](https://img-blog.csdn.net/20180220152104128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|[ ](https://img-blog.csdn.net/20180220152104128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|上图中的圆圈是单个作者，它们的大小表示归因于相应作者的文档数量。将鼠标悬停在圆圈上会告诉您作者的名称及其大小。一些圆圈的重叠，代表着不同作者话题内容的重叠。
|[

](https://img-blog.csdn.net/20180220152104128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|3.3.3 作者相似探究
|[
](https://img-blog.csdn.net/20180220152104128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|不同作者有不同的主题偏好，同样因为不同作者有不同偏好，那么跟上面t-SNE一样，一些作者就会有共同兴趣的部分，从而可以进行相似性推荐。
|其中使用的距离不是cos距离，而是 Hellinger distance。
|H|(|p|,|q|)|=|1|2|∑|K|(|p|i|−|q|i|)|2|,
|H(p, q) = \frac{1}{\sqrt{2}} \sqrt{\sum_{i=1}^K (\sqrt{p_i} - \sqrt{q_i})^2},
|where|p|p|and|q|q|are both topic distributions for two different authors. We define the similarity as
|S|(|p|,|q|)|=|1|.
|S(p, q) = \frac{1}{1 + H(p, q)}.
|主要函数为：
|\# Make a function that returns similarities based on the Hellinger distance.|from|gensim|import|matutils|import|pandas|as|pd|\# Make a list of all the author-topic distributions.|author_vecs = [model.get_author_topics(author)|for|author|in|model.id2author.values()]|def|similarity|(vec1, vec2)|:|'''Get similarity between two vectors'''|dist = matutils.hellinger(matutils.sparse2full(vec1, model.num_topics), \
                              matutils.sparse2full(vec2, model.num_topics))
    sim =|1.0|/ (|1.0|+ dist)|return|sim|def|get_sims|(vec)|:|'''Get similarity of vector to all authors.'''|sims = [similarity(vec, vec2)|for|vec2|in|author_vecs]|return|sims|def|get_table|(name, top_n=|10|, smallest_author=|1|)|:|'''
    Get table with similarities, author names, and author sizes.
    Return `top_n` authors as a dataframe.
    '''|\# Get similarities.|sims = get_sims(model.get_author_topics(name))|\# Arrange author names, similarities, and author sizes in a list of tuples.|table = []|for|elem|in|enumerate(sims):
        author_name = model.id2author[elem[|0|]]
        sim = elem[|1|]
        author_size = len(model.author2doc[author_name])|if|author_size >= smallest_author:
            table.append((author_name, sim, author_size))|\# Make dataframe and retrieve top authors.|df = pd.DataFrame(table, columns=[|'Author'|,|'Score'|,|'Size'|])
    df = df.sort_values(|'Score'|, ascending=|False|)[:top_n]|return|df|来看看效果：
|get_table(|'YannLeCun'|, smallest_author=|3|)![这里写图片描述](https://img-blog.csdn.net/20180220152539909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
|之前笔者关于主题模型的一些博客：
|[主题模型︱几款新主题模型——SentenceLDA、CopulaLDA、TWE简析与实现](http://blog.csdn.net/sinat_26917383/article/details/75193793)
|[NLP︱LDA主题模型的应用难题、使用心得及从多元统计角度剖析](http://blog.csdn.net/sinat_26917383/article/details/52233341)
|[LDA︱基于LDA的Topic Model变形+一些NLP开源项目](http://blog.csdn.net/sinat_26917383/article/details/52095013)
|[R语言︱LDA主题模型——最优主题数选取(topicmodels)+LDAvis可视化(lda+LDAvis)](http://blog.csdn.net/sinat_26917383/article/details/51547298)
|公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

