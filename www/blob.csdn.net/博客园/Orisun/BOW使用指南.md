# BOW使用指南 - Orisun - 博客园







# [BOW使用指南](https://www.cnblogs.com/zhangchaoyang/articles/2193557.html)





先规范一下发间：bow的韵音同low而不是cow。

 bow包含三个项目：rainbow用于文本分类；arrow用于文本检索；crossbow用于文本聚类。这三个程序是独立的。

**Rainbow**

使用rainbow前首先要建立原始文档的一个model----包含了原始文档的一些统计信息，使用rainbow命令时通过-d选项来指定model的路径。

rainbow -d ~/model --index ~/20_newsgroups/*

以上命令是为 20_newsgroups所有分类创建model，生成~/model文件。

--index目录可以分别写：rainbow -d ~/model --index ~/20_newsgroups/talk.politics.guns ~/20_newsgroups/talk.politics.mideast ~/20_newsgroups/talk.politics.misc

--index可简写为-i

rainbow不支持一个文档拥有多个类标签。

各个文档属于哪个类都已经包含在了model中。

 rainbow -d model --print-doc-names    打印出model中包含的所有文件的文件名（包括完整路径）。

默认情况下rainbow在建立model把字母都转换成了小写，并去除了停用词。

当然用rainbow建立model时还有很多选项可以指定，比如--skip-html可以路过“<"和“>"之间的所有字符；--skip-headers (or -h)选项跳过新闻组或邮件的headers before beginning tokenization.

为原始文档建立好索引后就可以来进行分类了。

 rainbow -d ~/model --test-set=0.4 --test=3

表示输出3次试验的结果，60%的文档作为训练集，剩下40%作为测试集。

输出类似于：

 /home/mccallum/20_newsgroups/talk.politics.misc/178939 talk.politics.misc talk.politics.misc:0.98 talk.politics.mideast:0.015 talk.politics.guns:0.005

指出了一个文档属于各个类的概率。

bow路径下还有一个perl脚本文件--rainbow-stats，它的输入是以上分类命令的输出，它的输出是平均精度、标准差和混淆矩阵。

rainbow -d ~/model --test-set=0.4 --test=2 | rainbow-stats

进行2次trail，输出形如：

Trial 1

Correct: 1077 out of 1200 (89.75 percent accuracy)

 - Confusion details, row is actual, column is predicted

               classname   0   1   2  :total

 0    talk.politics.guns 378   2  20  :400  94.50%

 1 talk.politics.mideast   7 374  19  :400  93.50%

 2    talk.politics.misc  57  18 325  :400  81.25%

Percent_Accuracy  average 90.38 stderr 0.44

--test-set选项也可以为整数，如--test-set=30，将试图从集合中随机选取30个文档作为测试集，并尽可能保证我30个文档平均分布于各个类中。

--test-set=200pc表示每个分类中随机选取200个文档放入测试集。

你甚至可以具体指定哪个文件放在测试集中--test-set=file1 fil2     各个文件用空格分开。

同理可以指定train-set：rainbow -d ~/model --test-set=file1 --train-set=file2 --test=1

一个文件不能同时出现在--teat-set和--train-set中。

默认情况下不在--test-set中的文档都在--train-set中，我们也可以反过来指定：

rainbow -d ~/model --train-set=1pc --test-set=remaining --test=1

如果测试集不在model中，也可以另外指定：rainbow -d ~/model --test-files ~/more-talk.politics/*

特征项选择

--prune-vocab-by-infogain=N  （-T）计算每个词的average mutual information，选取互信息最高的N个词作为特征词。默认情况下N=0，即选择所有的词作为特征词。

--prune-vocab-by-doc-count=N  （-D）一个词的文档频率（即在多少个文档中出现）大于N时才把它作为特征词。

--prune-vocab-by-occur-count=N  （-O）一个词出现不少于N次时选为特征词。

比如rainbow -d ~/model --prune-vocab-by-infogain=50 --test=1

你还可以查处哪50个词选为了特征词，通过：rainbow -d ~/model -I 50

选择采用的分类方法：rainbow -d ~/model --method=tfidf --test=1

rainbow支持的分类方法有：naivebayes, knn, tfidf, prind（probabilistic indexing），默认情况下采用的是naivebayes。

采用naivebayes分类时可以指定的选项有：

--smoothing-method=METHODSet the method for smoothing word probabilities to avoid zeros; METHOD may be one of: goodturing, laplace, mestimate, wittenbell. The default is laplace, which is a uniform Dirichlet prior with alpha=2.

--event-model=EVENTNAMESet what objects will be considered the `events' of the probabilistic model. EVENTNAME can be one of: word (i.e. multinomial, unigram), document (i.e. multi-variate Bernoulli, bit vector), or document-then-word (i.e. document-length-normalized multinomial). For more details on these methods, see A Comparison of Event Models for Naive Bayes Text Classification. The default is word.

--uniform-class-priorsWhen classifying and calculating mutual information, use equal prior probabilities on classes, instead of using the distribution determined from the training data.

机器诊断

你可以查看model中的各种信息。

rainbow -d ~/model -I 10           查看前10个特征词的互信息（或文档频率、出现的次数）

rainbow -d ~/model --train-set=~/docs1 -I 10   查看文档1中前10个特征词的互信息（或文档频率、出现的次数）

rainbow -d ~/model -T 10 --print-word-probabilities=talk.politics.mideast         查看类别talk.politics.mideast中

互信息最高的前10个特征词的probability。输出形如：

god                             0.05026782

   people                          0.64977338

   government                      0.24062629

   car                             0.03502266

   game                            0.00412031

   team                            0.01030078

   bike                            0.00041203

   dod                             0.00041203

   hockey                          0.00123609

   windows                         0.00782859

概率之和为1.

rainbow -d ~/model --print-word-counts=team     查看单词team在各个类中出现的次数及所占比重。输出形如：

     2 /    125039  (  0.00002) alt.atheism

        6 /    119511  (  0.00005) comp.graphics

        5 /     91147  (  0.00005) comp.os.ms-windows.misc

        1 /     71002  (  0.00001) comp.sys.mac.hardware

       12 /    131120  (  0.00009) comp.windows.x

       15 /     62130  (  0.00024) misc.forsale

        2 /     83942  (  0.00002) rec.autos

       10 /     78685  (  0.00013) rec.motorcycles

      543 /     88623  (  0.00613) rec.sport.baseball

      970 /    115109  (  0.00843) rec.sport.hockey

        9 /    136655  (  0.00007) sci.crypt

        1 /     81206  (  0.00001) sci.electronics

        8 /    125235  (  0.00006) sci.med

       71 /    128754  (  0.00055) sci.space

        2 /    141389  (  0.00001) soc.religion.christian

       13 /    135054  (  0.00010) talk.politics.guns

       24 /    208367  (  0.00012) talk.politics.mideast

       14 /    164266  (  0.00009) talk.politics.misc

        9 /    130013  (  0.00007) talk.religion.misc

Note: the probability of the word team is not equal to the probability of team from the --print-word-probabilities command above, because we did not reduce vocabulary size to 10 in this example.

rainbow -d ~/model --train-set=3pc --print-doc-names=train

通过--print-doc-names=train你可以查看测试集中有哪些文档

通达--print-matrix还可以输出word-document矩阵。

Print entries for all words in the vocabulary, or just print the words that actually occur in the document.

aall

ssparse, (default)

Print word counts as integers or as binary presence/absence indicators.

bbinary

iinteger, (default)

How to indicate the word itself.

ninteger word index

wword string

ccombination of integer word index and word string, (default)

eempty, don't print anything to indicate the identity of the word

比如rainbow -d ~/model -T 100 --print-matrix=siw | head -n 10的输出是下面的形式：

~/20_newsgroups/alt.atheism/53366 alt.atheism  god 2  jesus 1  nasa 2  people 2  

   ~/20_newsgroups/alt.atheism/53367 alt.atheism  jesus 2  jewish 1  christian 1  

   ~/20_newsgroups/alt.atheism/51247 alt.atheism  god 4  evidence 2  

   ~/20_newsgroups/alt.atheism/51248 alt.atheism  

   ~/20_newsgroups/alt.atheism/51249 alt.atheism  nasa 1  country 2  files 1  law 3  system 1  government 1  

   ~/20_newsgroups/alt.atheism/51250 alt.atheism  god 3  people 2  evidence 1  law 1  system 1  public 5  rights 1  fact 1  religious 1  

   ~/20_newsgroups/alt.atheism/51251 alt.atheism  

   ~/20_newsgroups/alt.atheism/51252 alt.atheism  people 4  evidence 2  system 2  religion 1  

   ~/20_newsgroups/alt.atheism/51253 alt.atheism  god 19  christian 1  evidence 1  faith 5  car 2  space 1  game 1  

   ~/20_newsgroups/alt.atheism/51254 alt.atheism  people 1  jewish 3  game 1  bible 7  

而rainbow -d ~/model -T 10 --print-matrix=abe | head -n 10的输出是下面的形式：

   ~/20_newsgroups/alt.atheism/53366 alt.atheism  1  1  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/53367 alt.atheism  0  0  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51247 alt.atheism  1  0  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51248 alt.atheism  0  0  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51249 alt.atheism  0  0  1  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51250 alt.atheism  1  1  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51251 alt.atheism  0  0  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51252 alt.atheism  0  1  0  0  0  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51253 alt.atheism  1  0  0  1  1  0  0  0  0  0  

   ~/20_newsgroups/alt.atheism/51254 alt.atheism  0  1  0  0  1  0  0  0  0  0

通用选项

--verbosity=LEVEL（LEVEL的取值为，0，1，2，3，4，5）控制打印进度信息，数字越小打印的信息越少，默认为2。

例如rainbow -v 0 -d ~/model -I 10

凡事牵涉到随机选取的命令都可以指定种子：

rainbow -d ~/model -t 1 --test-set=0.3 --random-seed=2

rainbow -d ~/model --random-seed=1 --train-set=4pc --print-doc-names=train

**Arrow**

Arrow是一个单独的程序，它采用TFIDF来对文档进行检索。

为文档建立索引：arrow --index 20_newsgroups/talk.politics.*

建立索引后生成的文件放在哪儿了我还不知道。

检索：orisun@zcypc:~/master$ arrow --query

Loading data files...

Type your query text now.  End with a Control-D.

america,HITCOUNT 176

20_newsgroups/talk.politics.mideast/76225  0.104700  america 

20_newsgroups/talk.politics.guns/53295  0.094274  america 

20_newsgroups/talk.politics.misc/178545  0.089230  america 

20_newsgroups/talk.politics.misc/179112  0.088966  america 

20_newsgroups/talk.politics.guns/55265  0.087667  america 

20_newsgroups/talk.politics.misc/178465  0.086633  america 

20_newsgroups/talk.politics.mideast/77184  0.084780  america 

20_newsgroups/talk.politics.guns/54318  0.084553  america 

20_newsgroups/talk.politics.misc/178790  0.081991  america 

20_newsgroups/talk.politics.mideast/76286  0.081669  america 

.

orisun@zcypc:~/master$ arrow --query

Loading data files...

Type your query text now.  End with a Control-D.

china,HITCOUNT 30

20_newsgroups/talk.politics.mideast/76476  0.176953  china 

20_newsgroups/talk.politics.mideast/76536  0.152673  china 

20_newsgroups/talk.politics.guns/53341  0.151225  china 

20_newsgroups/talk.politics.mideast/75405  0.151225  china 

20_newsgroups/talk.politics.misc/178554  0.127059  china 

20_newsgroups/talk.politics.misc/176864  0.116911  china 

20_newsgroups/talk.politics.misc/178488  0.105641  china 

20_newsgroups/talk.politics.misc/178326  0.104150  china 

20_newsgroups/talk.politics.misc/176852  0.092056  china 

20_newsgroups/talk.politics.mideast/76264  0.090448  china 

.

orisun@zcypc:~/master$ arrow --query

Loading data files...

Type your query text now.  End with a Control-D.

AMERICA 

CHINA

,HITCOUNT 194

20_newsgroups/talk.politics.mideast/76476  0.150675  china 

20_newsgroups/talk.politics.mideast/76536  0.130001  china 

20_newsgroups/talk.politics.guns/53341  0.128768  china 

20_newsgroups/talk.politics.mideast/75405  0.128768  china 

20_newsgroups/talk.politics.misc/178554  0.108191  china 

20_newsgroups/talk.politics.misc/176864  0.099550  china 

20_newsgroups/talk.politics.guns/54857  0.091291  china america 

20_newsgroups/talk.politics.misc/178488  0.089953  china 

20_newsgroups/talk.politics.misc/178326  0.088683  china 

20_newsgroups/talk.politics.misc/176852  0.078386  china 

.

可以看到当查询多个词（AMERICA 和CHINA）时，查询词之间采用的是“或”的逻辑关系，并且自动把大写全部转换为小写。



**Crossbow**



目前还没有文档。












