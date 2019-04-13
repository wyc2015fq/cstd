
# 在Python中理解和实现文本分类的综合指南 - leofionn的博客 - CSDN博客


2018年06月13日 19:24:56[leofionn](https://me.csdn.net/qq_36142114)阅读数：660


# 在Python中理解和实现文本分类的综合指南
原文链接：https://www.analyticsvidhya.com/blog/2018/04/a-comprehensive-guide-to-understand-and-implement-text-classification-in-python/

## 介绍
在不同商业问题中广泛使用的自然语言处理任务之一是“文本分类”。文本分类的目标是自动将文本文档分类为一个或多个定义的类别。文本分类的一些例子是：
了解社交媒体的观众情绪，
检测垃圾邮件和非垃圾邮件，
自动标记客户查询和
将新闻文章分类为定义的主题。


## 目录
在本文中，我将解释关于文本分类和逐步过程以在python中实现它。
![](https://s3-ap-south-1.amazonaws.com/av-blog-media/wp-content/uploads/2018/04/Untitled-Diagram.png)
文本分类是监督机器学习任务的一个例子，因为包含文本文档及其标签的带标签的数据集用于训练分类器。端到端文本分类管道由三个主要组件组成：
1.数据集准备：第一步是数据集准备步骤，其中包括加载数据集和执行基本预处理的过程。然后将数据集分成火车和验证集。
2.特征工程：下一步是特征工程，其中原始数据集被转换成可用于机器学习模型的平坦特征。这一步还包括从现有数据创建新功能的过程。
3.模型训练：最后一步是模型构建步骤，其中机器学习模型在标记数据集上进行训练。
4.提高文本分类器的性能：在本文中，我们还将看看提高文本分类器性能的不同方法。
注意：本文不深入讲述NLP任务。如果你想修改基础知识并回到这里，你可以随时阅读这篇文章。

## 准备好你的机器
让我们一步步实现基本组件，以便在python中创建文本分类框架。首先，导入所有必需的库。
您需要必要的库来运行此代码 - 您可以将它们安装在各自的官方链接中
熊猫
Scikit学习
XGBoost
TextBlob
Keras

＃用于数据集准备，特征工程，模型培训的库
从sklearn进口model_selection，预处理，linear_model，naive_bayes，度量，SVM从sklearn.feature_extraction.text进口TfidfVectorizer，CountVectorizer从sklearn进口分解，合奏进口大熊猫，xgboost，numpy的，textblob，串从keras.preprocessing导入文本，顺序从keras进口层，模型，优化



## 1.数据集准备
对于本文的目的，我是亚马逊评论的使用数据集，可以在此链接下载。数据集由3.6M文本评论及其标签组成，我们将仅使用一小部分数据。要准备数据集，请将下载的数据加载到包含两列（文本和标签）的熊猫数据框中。（来源）

＃加载数据集data=open（'data / corpus'）。读（）标签，文本=[]，[]为我，线在枚举（数据。分裂（“\ n”））：含量=线。split（）标签。追加（content[0]）文本。追加（内容[1]）＃使用文本和标签创建数据框trainDF=pandas。DataFrame（）trainDF['text']=文本trainDF['label']=标签


接下来，我们将把数据集分成训练集和验证集，以便我们可以训练和测试分类器。此外，我们将编码我们的目标列，以便它可以用于机器学习模型。

＃将数据集分成训练和验证数据集train_x，valid_x，train_y，valid_y=model_selection。train_test_split（trainDF['text']，trainDF['label']）＃标签编码目标变量编码器=预处理。LabelEncoder（）train_y=编码器。fit_transform（train_y）valid_y=编码器。fit_transform（valid_y）



## 2.特色工程
下一步是特征工程步骤。在这一步中，将原始文本数据转换为特征向量，并使用现有数据集创建新特征。为了从我们的数据集中获得相关特征，我们将实施以下不同的想法。
2.1计数矢量作为特征
2.2 TF-IDF矢量作为特征
单词级别
N-gram水平
人物级别
2.3词嵌入作为特征
2.4基于文本/ NLP的特征
2.5主题模型作为特征
让我们仔细看看这些想法的实现。


### 2.1计数矢量作为特征
计数向量是数据集的矩阵符号，其中每行表示来自语料库的文档，每列表示来自语料库的术语，并且每个单元格表示特定文档中特定术语的频率计数。

＃创建一个countvectorvector对象count_vect=CountVectorizer（analyzer='word'，token_pattern=r'\ w {1，}'）count_vect。fit（trainDF['text']）＃使用计数向量化程序对象xtrain_count=count_vect转换训练和验证数据。transform（train_x）xvalid_count=count_vect。transform（valid_x）



### 2.2 TF-IDF矢量作为特征
TF-IDF分数表示文档和整个语料库中术语的相对重要性。TF-IDF分数由两部分组成：第一部分计算标准化术语频率（TF），第二部分是逆文档频率（IDF），计算方式为文集中文档数量除以编号的对数具体条款出现的文件。
TF（t）=（期限t在文件中出现的次数）/（文件中的期限总数）
IDF（t）= log_e（文件总数/包含期限t的文件数量）
TF-IDF矢量可以在不同级别的输入令牌（单词，字符，n元组）中生成，
一个。单词级别TF-IDF：表示不同文档中每个术语的tf-idf分数的矩阵
b。N-gram等级TF-IDF：N-grams是N个术语的组合。这个矩阵表示N-grams的tf-idf分数
c。字符级别TF-IDF：代表语料库中字符级别n-gram的tf-idf分数的矩阵

＃wordlevel tf-idftfidf_vect=TfidfVectorizer（analyzer='word'，token_pattern=r'\ w {1，}'，max_features=5000）tfidf_vect。fit（trainDF['text']）xtrain_tfidf=tfidf_vect。transform（train_x）xvalid_tfidf=tfidf_vect。transform（valid_x）＃的ngram水平TF-IDFtfidf_vect_ngram=TfidfVectorizer（分析器='单词'，token_pattern=- [R'\瓦特{1，}'，ngram_range=（2，3），max_features=5000）tfidf_vect_ngram。fit（trainDF['text']）xtrain_tfidf_ngram=tfidf_vect_ngram。transform（train_x）xvalid_tfidf_ngram=tfidf_vect_ngram。transform（valid_x）＃字符级别的TF-IDFtfidf_vect_ngram_chars=TfidfVectorizer（分析器='字符'，token_pattern=- [R'\瓦特{1，}'，ngram_range=（2，3），max_features=5000）tfidf_vect_ngram_chars。fit（trainDF['text']）xtrain_tfidf_ngram_chars=tfidf_vect_ngram_chars。transform（train_x）xvalid_tfidf_ngram_chars=tfidf_vect_ngram_chars。transform（valid_x）



### 2.3文字嵌入
单词嵌入是使用密集矢量表示来表示单词和文档的一种形式。向量空间中单词的位置是从文本中学习的，并且基于单词在使用时围绕单词的位置。可以使用输入语料库本身对词嵌入进行训练，也可以使用预先训练的词嵌入（例如Glove，FastText和Word2Vec）来生成词嵌入。他们中的任何一个都可以下载并用作传输学习。人们可以在这里阅读更多关于文字嵌入的知识。
snnipet展示了如何在模型中使用预先训练的词嵌入。有四个基本步骤：
加载预训练词嵌入
创建一个标记器对象
将文本文档转换为令牌序列并填充它们
创建令牌及其各自嵌入的映射
您可以从这里下载预先训练的词嵌入

＃加载预训练的字嵌入矢量embeddings_index={}为我，线在枚举（开放（'数据/维基新闻-300D-1M.vec'））：值=线。split（）embeddings_index[values[0]]=numpy。asarray（values[1：]，dtype='float32'）＃创建一个tokenizertoken=text。Tokenizer（）令牌。fit_on_texts（trainDF['text']）word_index=标记。word_index＃将文本转换为令牌序列并填充它们以确保相等长度的向量train_seq_x=序列。pad_sequences（令牌。texts_to_sequences（train_x），MAXLEN=70）valid_seq_x=序列。pad_sequences（令牌。texts_to_sequences（valid_x），MAXLEN=70）＃创建令牌嵌入映射embedding_matrix=numpy。零（（LEN（word_index）+1，300））为字，我在word_index。items（）：embedding_vector=embeddings_index。得到（字）如果embedding_vector是不无：embedding_matrix[我]=embedding_vector



### 2.4基于文本/ NLP的功能
还可以创建一些额外的基于文本的功能，这有时对改进文本分类模型有帮助。一些例子是：
文档的字数 - 文档中的单词总数
文档字符数 - 文档中的字符总数
文档的平均字密度 - 文档中使用的单词的平均长度
完整论文中的Puncutation Count - 文档中标点符号的总数
完整论文中的大写字母计数 - 文档中大写字母的总数
在完整论文中标题字数 - 文件中正确的案例（标题）字的总数
词性标签的频率分布：名词Count
动词数
形容词计数
副词计数
代数计数
这些功能是高度实验性的，只能根据问题陈述使用。

trainDF['char_count']=trainDF['text']。apply（len）trainDF['word_count']=trainDF['text']。申请（拉姆达X：LEN（X。分裂（）））trainDF['word_density']=trainDF['char_count']/（trainDF['WORD_COUNT']+1）trainDF['punctuation_count']=trainDF['text']。申请（拉姆达X：LEN（“”。联接（_的_在X，如果_中的字符串。标点符号）））trainDF['title_word_count']=trainDF['文本']。apply（lambdax：len（[WRD的WRD在X。split（）如果wrd。istitle（）]））trainDF['upper_case_word_count']=trainDF['text']。申请（拉姆达X：LEN（[WRD为WRD在X。分裂（）如果WRD。isupper）（）]）


pos_family={'noun'：['NN'，'NNS'，'NNP'，'NNPS']，'pron'：['PRP'，'PRP $'，'WP'，'WP $']，''动词'：['VB'，'VBD'，'VBG'，'VBN'，'VBP'，'VBZ']，'adj'：['JJ'，'JJR'，'JJS']，'adv'：['RB'，'RBR'，'RBS'，'WRB']}＃函数检查并获取给定句子中单词的词性标记计数defcheck_pos_tag（x，flag）：cnt=0try：wiki=textblob。TextBlob（X）为TUP的维基。标签：ppo=list（tup）[1]如果ppo在pos_family[flag]中：cnt+ =1除外：pass返回cnttrainDF['noun_count']=trainDF['text']。apply（lambdax：check_pos_tag（x，'noun'））trainDF['verb_count']=trainDF['text']。apply（lambdax：check_pos_tag（x，'verb'））trainDF['adj_count']=trainDF['text']。apply（lambdax：check_pos_tag（x，'adj'））trainDF['adv_count']=trainDF['text']。apply（lambdax：check_pos_tag（x，'adv'））trainDF['pron_count']=trainDF['text']。应用（lambdax：check_pos_tag（x，'pron'））



### 2.5主题模型作为功能
主题建模是一种从包含集合中最佳信息的文档集合中识别单词组（称为主题）的技术。我已经使用潜在Dirichlet分配来生成主题建模特征。LDA是一个从固定数量的话题开始的迭代模型。每个主题都被表示为一个词的分布，然后每个文档被表示为主题分布。虽然令牌本身没有意义，但由主题提供的单词的概率分布提供了文档中包含的不同想法的意义。人们可以在这里阅读更多关于话题模型
让我们看看它的实现：

＃训练一个LDA模型lda_model=分解。LatentDirichletAllocation（n_components=20，learning_method='online'，max_iter=20）X_topics=lda_model。fit_transform（xtrain_count）topic_word=lda_model。components_vocab=count_vect。get_feature_names（）＃查看主题模型n_top_words=10topic_summaries=[]fori，topic_distinenumerate（topic_word）：topic_words=numpy。数组（vocab）[numpy。argsort（topic_dist）] [：-（n_top_words+1）：-1]topic_summaries。append（''。join（topic_words））



## 3.模型建立
文本分类框架的最后一步是使用上一步创建的特征来训练分类器。机器学习模型有很多不同的选择，可以用来训练最终模型。为此，我们将实施以下不同的分类器：
朴素贝叶斯分类器
线性分类器
支持向量机
套袋模型
提升模型
浅神经网络
深度神经网络卷积神经网络（CNN）
长期短期模型（LSTM）
门控复发单元（GRU）
双向RNN
递归卷积神经网络（RCNN）
深度神经网络的其他变体
让我们实现这些模型并理解他们的细节。以下函数是可用于训练模型的效用函数。它接受训练数据的分类器，feature_vector，训练数据的标签和有效数据的特征向量作为输入。使用这些输入，训练模型并计算准确性分数。

deftrain_model（分类器，feature_vector_train，label，feature_vector_valid，is_neural_net=False）：＃将训练数据集合到分类器分类器中。fit（feature_vector_train，标签）＃预测验证数据集预测=分类器上的标签。预测（feature_vector_valid）如果is_neural_net：predictions=预测。argmax（axis=  -1）返回指标。accuracy_score（预测，valid_y）



### 3.1朴素贝叶斯
使用具有不同特征的sklearn实现实现朴素贝叶斯模型
朴素贝叶斯是一种基于贝叶斯定理的分类技术，在预测变量中具有独立性假设。朴素贝叶斯分类器假定类中特定特征的存在与此处任何其他特征的存在无关。

＃朴素贝叶斯上计数矢量精度=train_model（naive_bayes。MultinomialNB（），xtrain_count，train_y，xvalid_count）打印“NB，计数载体：”，精度＃朴素贝叶斯对词义的TF IDF矢量精度=train_model（naive_bayes。MultinomialNB（），xtrain_tfidf，train_y，xvalid_tfidf）打印“NB，WordLevel TF-IDF：”，准确度＃朴素贝叶斯上的ngram等级TF IDF载体精度=train_model（naive_bayes。MultinomialNB（），xtrain_tfidf_ngram，train_y，xvalid_tfidf_ngram）打印“NB，n-gram中的载体：”，精度＃朴素贝叶斯角色等级TF IDF载体精度=train_model（naive_bayes。MultinomialNB（），xtrain_tfidf_ngram_chars，train_y，xvalid_tfidf_ngram_chars）打印“NB，CharLevel载体：”，精度


NB，计数矢量：0.7004NB，WordLevel TF-IDF：0.7024NB，N-Gram载体：0.5344NB，CharLevel Vectors：0.6872



### 3.2线性分类器
实现线性分类器（Logistic回归）
Logistic回归通过使用logistic / sigmoid函数估计概率来度量分类因变量与一个或多个自变量之间的关系。在这里可以阅读更多有关逻辑回归的内容

＃线性分类上计数矢量精度=train_model（linear_model。逻辑回归（），xtrain_count，train_y，xvalid_count）打印“LR，计数载体：”，精度＃线性分类上词级TF IDF载体精度=train_model（linear_model。逻辑回归（），xtrain_tfidf，train_y，xvalid_tfidf）打印“LR，WordLevel TF-IDF”，精度＃线性分类上的ngram等级TF IDF载体精度=train_model（linear_model。逻辑回归（），xtrain_tfidf_ngram，train_y，xvalid_tfidf_ngram）打印“LR，n-gram中的载体：”，精度＃线性分类角色等级TF IDF载体精度=train_model（linear_model。逻辑回归（），xtrain_tfidf_ngram_chars，train_y，xvalid_tfidf_ngram_chars）打印“LR，CharLevel载体：”，精度


LR，计数矢量：0.7048LR，WordLevel TF-IDF：0.7056LR，N-Gram矢量：0.4896LR，CharLevel Vectors：0.7012



### 3.3实现SVM模型
支持向量机（SVM）是一种有监督的机器学习算法，可用于分类或回归挑战。该模型提取了分离两个类的最佳超平面/线。人们可以在这里阅读更多

上NGRAM等级TF IDF载体＃SVM精度=train_model（SVM。SVC（），xtrain_tfidf_ngram，train_y，xvalid_tfidf_ngram）打印“SVM，n-gram中的载体：”，精度


SVM，N-Gram矢量：0.5296



### 3.4套袋模型
实现随机森林模型
随机森林模型是一种集合模型，特别是装袋模型。它们是基于树型模型系列的一部分。人们可以在这里阅读关于套袋和随机森林的更多信息

上计数载体＃RF精度=train_model（合奏。RandomForestClassifier（），xtrain_count，train_y，xvalid_count）打印“RF，计数载体：”，精度上词级TF IDF载体＃RF精度=train_model（合奏。RandomForestClassifier（），xtrain_tfidf，train_y，xvalid_tfidf）打印“RF，WordLevel TF-IDF”，精度


RF，计数向量：0.6972RF，WordLevel TF-IDF：0.6988



### 3.5增强模型
实施Xtereme梯度增强模型
增强模型是基于树的模型的另一种集成模型的一部分。Boosting是一种主要用于减少偏见的机器学习集成元算法，也是监督式学习中的变异，以及一系列将弱学习者转换为强大学习者的机器学习算法。一个弱学习者被定义为一个分类器，它只与真实分类稍有关联（它可以比随机猜测更好地标注示例）。在这里阅读更多关于这些模型

＃Extereme梯度推进上计数矢量精度=train_model（xgboost。XGBClassifier（），xtrain_count。tocsc（），train_y，xvalid_count。tocsc（））打印“XGB，计数载体：”，精度＃Extereme梯度推进上词级TF IDF载体精度=train_model（xgboost。XGBClassifier（），xtrain_tfidf。tocsc（），train_y，xvalid_tfidf。tocsc（））打印“XGB，WordLevel TF-IDF”，精度＃Extereme梯度推进角色等级TF IDF载体精度=train_model（xgboost。XGBClassifier（），xtrain_tfidf_ngram_chars。tocsc（），train_y，xvalid_tfidf_ngram_chars。tocsc（））打印“XGB，CharLevel载体：”，精度


/usr/local/lib/python2.7/dist-packages/sklearn/preprocessing/label.py:151：DeprecationWarning：空数组的真值是不明确的。返回False，但将来会导致错误。使用`array.size> 0`来检查数组是否为空。如果差异：/usr/local/lib/python2.7/dist-packages/sklearn/preprocessing/label.py:151：DeprecationWarning：空数组的真值是不明确的。返回False，但将来会导致错误。使用`array.size> 0`来检查数组是否为空。如果差异：

Xgb，计数矢量：0.6324Xgb，WordLevel TF-IDF：0.6364Xgb，CharLevel Vectors：0.6548

/usr/local/lib/python2.7/dist-packages/sklearn/preprocessing/label.py:151：DeprecationWarning：空数组的真值是不明确的。返回False，但将来会导致错误。使用`array.size> 0`来检查数组是否为空。如果差异：



### 3.6浅神经网络
神经网络是一种数学模型，其设计与生物神经元和神经系统类似。这些模型用于识别标记数据中存在的复杂模式和关系。浅层神经网络主要包含三种类型的层 - 输入层，隐藏层和输出层。在这里阅读更多关于神经网络
![](https://s3-ap-south-1.amazonaws.com/av-blog-media/wp-content/uploads/2018/04/OH3gI-1.png)

DEFcreate_model_architecture（input_size）：＃创建输入层input_layer=层。输入（（input_size，），sparse=True）\# create hidden layerhidden_layer=layers.Dense(100,activation="relu")(input_layer)\# create output layeroutput_layer=layers.Dense(1,activation="sigmoid")(hidden_layer)classifier=models.Model(inputs=input_layer,outputs=output_layer)classifier.compile(optimizer=optimizers.Adam(),loss='binary_crossentropy')returnclassifier分类=create_model_architecture（xtrain_tfidf_ngram。塑造[1]）的精度=train_model（分类器，xtrain_tfidf_ngram，train_y，xvalid_tfidf_ngram，is_neural_net=真）打印“NN，NGRAM等级TF IDF载体”，准确性


纪元1/17500/7500 [==============================]  -  1s 67us /步 - 损失：0.6909NN，Ngram等级TF IDF矢量0.5296



### 3.7深度神经网络
深度神经网络是更复杂的神经网络，其中隐藏层执行比简单S形或Relu激活更复杂的操作。不同类型的深度学习模型可以应用于文本分类问题。
![](https://s3-ap-south-1.amazonaws.com/av-blog-media/wp-content/uploads/2018/04/OH3gI.png)
#### 3.7.1卷积神经网络
在卷积神经网络中，输入层上的卷积被用来计算输出。这导致了本地连接，输入的每个区域都连接到输出中的神经元。每个图层应用不同的过滤器并组合其结果。
![](https://s3-ap-south-1.amazonaws.com/av-blog-media/wp-content/uploads/2018/04/cnnimage.png)
在这里阅读更多有关卷积神经网络的内容

defcreate_cnn（）：＃添加一个Input Layerinput_layer=layers。输入（（70，））＃添加嵌入图层embedding_layer=图层的单词。嵌入（LEN（word_index）+1，300，权重=[embedding_matrix]，可训练=假）（input_layer）embedding_layer=层。SpatialDropout1D（0.3）（embedding_layer）＃添加卷积图层conv_layer=图层。Convolution1D（100，3，活化=“RELU”）（embedding_layer）＃添加池pooling_layer=图层。GlobalMaxPool1D（）（conv_layer）＃添加输出层output_layer1=layers。Dense（50，activation=“relu”）（pooling_layer）output_layer1=图层。压差（0.25）（output_layer1）output_layer2=图层。密集（1，activation=“sigmoid”）（output_layer1）＃编译模型模型=模型。模型（输入=input_layer，输出=output_layer2）模型。编译（优化=优化。亚当（），损耗='binary_crossentropy'）返回模型classifier=create_cnn（）accuracy=train_model（classifier，train_seq_x，train_y，valid_seq_x，is_neural_net=True）print“CNN，Word Embeddings”，准确度


纪元1/17500/7500 [==============================]  -  12s 2ms /步 - 损耗：0.5847CNN，文字嵌入0.5296



#### 3.7.2递归神经网络 - LSTM
与前馈神经网络中激活输出仅在一个方向上传播不同，神经元的激活输出在递归神经网络中在两个方向上（从输入到输出和从输出到输入）传播。这在神经网络架构中产生循环，其充当神经元的“记忆状态”。这种状态使神经元能够记住迄今为止学到的东西。
RNN中的记忆状态优于传统的神经网络，但称为消失梯度的问题与它们有关。在这个问题中，当学习大量图层时，网络很难学习和调整早期图层的参数。为了解决这个问题，已经开发了称为LSTM（长期短期记忆）模型的新型RNN。
![](https://s3-ap-south-1.amazonaws.com/av-blog-media/wp-content/uploads/2018/04/bptt-768x313.png)
阅读更多关于LSTMs的信息

defcreate_rnn_lstm（）：
＃添加一个Input Layerinput_layer=layers。输入（（70，））＃添加嵌入图层embedding_layer=图层的单词。嵌入（LEN（word_index）+1，300，权重=[embedding_matrix]，可训练=假）（input_layer）embedding_layer=层。SpatialDropout1D（0.3）（embedding_layer）＃添加LSTM Layerlstm_layer=图层。LSTM（100）（embedding_layer）＃添加输出层output_layer1=layers。Dense（50，activation=“relu”）（lstm_layer）output_layer1=图层。压差（0.25）（output_layer1）output_layer2=图层。密集（1，activation=“sigmoid”）（output_layer1）＃编译模型模型=模型。模型（输入=input_layer，输出=output_layer2）模型。编译（优化=优化。亚当（），损耗='binary_crossentropy'）返回模型分类=create_rnn_lstm（）精度=train_model（分类器，train_seq_x，train_y，valid_seq_x，is_neural_net=真）打印“RNN-LSTM，字曲面嵌入”，准确性


纪元1/17500/7500 [==============================]  -  22s 3ms / step  -  loss：0.6899RNN-LSTM，字嵌入0.5124



#### 3.7.3循环神经网络 - GRU
门控循环单元是另一种形式的递归神经网络。让我们在网络中添加一层GRU，而不是LSTM。

defcreate_rnn_gru（）：
＃添加一个Input Layerinput_layer=layers。输入（（70，））＃添加嵌入图层embedding_layer=图层的单词。嵌入（LEN（word_index）+1，300，权重=[embedding_matrix]，可训练=假）（input_layer）embedding_layer=层。SpatialDropout1D（0.3）（embedding_layer）＃添加GRU图层lstm_layer=图层。GRU（100）（embedding_layer）＃添加输出层output_layer1=layers。Dense（50，activation=“relu”）（lstm_layer）output_layer1=图层。压差（0.25）（output_layer1）output_layer2=图层。密集（1，activation=“sigmoid”）（output_layer1）＃编译模型模型=模型。模型（输入=input_layer，输出=output_layer2）模型。编译（优化=优化。亚当（），损耗='binary_crossentropy'）返回模型classifier=create_rnn_gru（）accuracy=train_model（classifier，train_seq_x，train_y，valid_seq_x，is_neural_net=True）print“RNN-GRU，Word Embeddings”，准确度


纪元1/17500/7500 [==============================]  -  19s 3ms / step  -  loss：0.6898RNN-GRU，文字嵌入0.5124



#### 3.7.4双向RNN
RNN图层也可以包裹在双向图层中。让我们将GRU层包裹在双向层中。

defcreate_bidirectional_rnn（）：
＃添加一个Input Layerinput_layer=layers。输入（（70，））＃添加嵌入图层embedding_layer=图层的单词。嵌入（LEN（word_index）+1，300，权重=[embedding_matrix]，可训练=假）（input_layer）embedding_layer=层。SpatialDropout1D（0.3）（embedding_layer）＃添加LSTM Layerlstm_layer=图层。双向（层。GRU（100））（embedding_layer）＃添加输出层output_layer1=layers。Dense（50，activation=“relu”）（lstm_layer）output_layer1=图层。压差（0.25）（output_layer1）output_layer2=图层。密集（1，activation=“sigmoid”）（output_layer1）＃编译模型模型=模型。模型（输入=input_layer，输出=output_layer2）模型。编译（优化=优化。亚当（），损耗='binary_crossentropy'）返回模型分类器=create_bidirectional_rnn（）准确性=train_model（分类器，train_seq_x，train_y，valid_seq_x，is_neural_net=True）print“RNN-双向字嵌入”，准确度


纪元1/17500/7500 [==============================]  -  32s 4ms / step  -  loss：0.6889RNN-双向字嵌入0.5124



#### 3.7.5递归卷积神经网络
一旦基本体系结构尝试完毕，就可以尝试这些层的不同变体，例如循环卷积神经网络。另一种变体可以是：
分层注意网络
注意力序列模型的顺序
双向递归卷积神经网络
有更多层数的CNN和RNN

defcreate_rcnn（）：
＃添加一个Input Layerinput_layer=layers。输入（（70，））＃添加嵌入图层embedding_layer=图层的单词。嵌入（LEN（word_index）+1，300，权重=[embedding_matrix]，可训练=假）（input_layer）embedding_layer=层。SpatialDropout1D（0.3）（embedding_layer）＃添加经常性图层rnn_layer=图层。双向（层。GRU（50，return_sequences=真））（embedding_layer）＃添加卷积图层conv_layer=图层。Convolution1D（100，3，活化=“RELU”）（embedding_layer）＃添加池pooling_layer=图层。GlobalMaxPool1D（）（conv_layer）＃添加输出层output_layer1=layers。Dense（50，activation=“relu”）（pooling_layer）output_layer1=图层。压差（0.25）（output_layer1）output_layer2=图层。密集（1，activation=“sigmoid”）（output_layer1）＃编译模型模型=模型。模型（输入=input_layer，输出=output_layer2）模型。编译（优化=优化。亚当（），损耗='binary_crossentropy'）返回模型分类=create_rcnn（）精度=train_model（分类器，train_seq_x，train_y，valid_seq_x，is_neural_net=真）打印“CNN，字曲面嵌入”，准确性


纪元1/17500/7500 [==============================]  -  11s 1ms /步 - 损耗：0.6902CNN，文字嵌入0.5124



## 改进文本分类模型
虽然上述框架可以应用于多个文本分类问题，但是为了达到更高的准确率，可以在总体框架中进行一些改进。例如，下面是一些改进文本分类模型和该框架性能的技巧：

1. 清洗文本：文本清洗有助于减少文本数据中出现的噪声，包括停用词、标点符号、后缀变化等。这篇文章有助于理解如何实现文本分类：

> https://www.analyticsvidhya.com/blog/2014/11/text-data-cleaning-steps-python/

2. 组合文本特征向量的文本/NLP特征：特征工程阶段，我们把生成的文本特征向量组合在一起，可能会提高文本分类器的准确率。
模型中的超参数调优：参数调优是很重要的一步，很多参数通过合适的调优可以获得最佳拟合模型，例如树的深层、叶子节点数、网络参数等。
3.建模中的超参数调整：调整参数是一个重要的步骤，可以对一些参数（如树长，叶子，网络参数等）进行微调以获得最佳拟合模型。
4.整体模型：堆叠不同的模型并混合它们的输出可以帮助进一步改进结果。



## 结束笔记
在本文中，我们讨论了如何准备一个文本数据集，如清理/创建训练和验证数据集，执行不同类型的特征工程，如Count Vector / TF-IDF / Word Embedding / Topic Modeling和基本文本特征，并最终训练Naive Bayes / Logistic回归/ SVM / MLP / LSTM和GRU等多种分类器。最后，讨论了改进文本分类器性能的不同方法。




