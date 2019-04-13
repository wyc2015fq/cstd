
# CRF汇总 - forever1dreamsxx--NLP - CSDN博客


2012年11月22日 17:08:29[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：3458


来自：[某某某的专栏](/todaylxp)
CSDN博客
原文地址：
[http://blog.csdn.net/](http://blog.csdn.net/todaylxp/article/details/4204038)[某某某的专栏](/todaylxp)[todaylxp/article/details/4204038](http://blog.csdn.net/todaylxp/article/details/4204038)

分词过程
CRF分词原理
1. CRF把分词过程看做字的词位分类问题，通常定义字的词位信息如下：
词首，常用B表示
词中，常用M表示
词尾，常用E表示
单子词，常用S表示
2. CRF分词的过程就是对词位标注后，将B和E之间的字，以及S单字构成分词
分词的类
CRFSegParser/CRFSegProcessor
CRFSegParser :分词器
CRFSegProcessor:分词器线程/可能运用多线程训练(测试)
分词过程
CRFSegProcessor::process(std::vector<TokenImpl*>&in,std::vector<TokenImpl*> &out)
std::vector<TokenImpl*>&in原子切分后的字符串流
td::vector<TokenImpl*> &out结果序列
{
1.位置标注过程
CRFPP::Tagger *_tagger;(派生类TaggerImpl)
_tagger->parse()// 位置标注过程
2.合并
例如标注为"B"的原子和相邻标注为"E"的原子合并为词，"S"单独构词
}
boolTaggerImpl::parse()
{
…
feature_index_->buildFeatures(this)
buildLattice();
if(nbest_||vlevel_>= 1)
forwardbackward();
viterbi();//维特比算法
if(nbest_)initNbest();
….
}
boolFeatureIndex::buildFeatures(TaggerImpl*tagger)
{
string_bufferos;
std::vector<int>feature;
tagger->set_feature_id(feature_cache_.size());
// size_t nTaggerSize = tagger->size();该句子的Token流
for(size_tcur= 0;cur<tagger->size(); ++cur) // 原子
{
///
for(std::vector<char*>::const_iteratorit=unigram_templs_.begin();
it!=unigram_templs_.end(); ++it) //特征
{
apply_rule(&os, *it,cur, *tagger) // 特征和原子组合
ADD;//for debug //如果特里树查找成功，则添加返回ID值
}
feature_cache_.add(feature);
feature.clear();
}
…
}

voidTaggerImpl::buildLattice()
{
// Lattice格子
feature_index_->rebuildFeatures(this);
//生成节点和路径
1.node节点，每个原子和特征构成(例如"字"和"B")
Node**为N*M的二维矩阵,i为字，j为可能位置标注
N为原子个数,M为位置枚举数
填充node_，path_行为token，列为属性特征
Path中lpath为前驱，rpath为后趋
for(size_ti= 0;i<x_.size(); ++i)
{
for(size_tj= 0;j<ysize_; ++j)
{
feature_index_->calcCost(node_[i][j]);//节点代价函数计算
conststd::vector<Path*> &lpath=node_[i][j]->lpath;
for(const_Path_iteratorit=lpath.begin();it!=lpath.end(); ++it)
feature_index_->calcCost(*it);
}
}
}














特征模板
\# Unigram
U00:%x[-2,0]
U01:%x[-1,0]
U02:%x[0,0]
U03:%x[1,0]
U04:%x[2,0]
U05:%x[-1,0]/%x[0,0]
U06:%x[0,0]/%x[1,0]
U07:%x[-1,0]/%x[1,0]
U08:%x[0,1]
U09:%x[-1,1]/%x[0,1]
\# Bigram
B

位置信息
B //起始位置
B1 // 暂不知
B2 // 暂不知
M // 中间位置
E //结束位置
S //单独成词










crf++为什么浪费内存：都是stl惹的祸
注意:STL线程不安全
利用休假读了下crf++代码，特别是有关存储训练数据的内存结构部分。很久以前，我就通过匆匆扫过一遍的映像，认为crf++这部分的浪费非常严重。近期的阅读证实了这一观点。我还是用去年发布的0.44版。
一般来说，系统训练需要缓存两部分的内容，一部分是关于训练数据的信息，主要包括特征的表达式以及相应的频率，另外一部分用来LBFGS训练的矩阵缓存，包括lambda权值向量。后面一部分内容是不可或缺的。如果要节省内存耗费，可以考虑把第一部分内容在记录到磁盘后，仅仅留下特征的代号和对应的频率信息在内存中。
问题是，我发现， crf++没有这么做。所有的信息都保留在内存中，直到训练结束的时候一口气写入model文件。当然，crf++也不是什么都没有做。crf++在开始训练前，函数
bool TaggerImpl::shrink()
{
CHECK_FALSE(feature_index_->buildFeatures(*this))
<< feature_index_->what();
std::vector <std::vector <const char *> >(x_).swap(x_);
std::vector <std::vector <Node *> >(node_).swap(node_);
std::vector <unsigned short int>(answer_).swap(answer_);
std::vector <unsigned short int>(result_).swap(result_);
return true;
}
中，
第一句对特征进行编码，后边4句企图释放掉已经无用的训练数据缓存。taku使用了标准的swap方式来释放stl管理的内容。不幸的是，这些内存其实根本就没有得到释放。注释掉这4句后，内存的使用也不会增长。简单来说，这组内存没有得到释放。
由于对stl不熟悉---实际上，我还不会用allocator---我使用标准c替代了相关部分。
首先，我用一个简单的自己管理的char *代替了std::vector <const char *> > TaggerImpl::x_,结果期望中的内存强制释放马上获得了10%的内存减少。
然后，我把model存盘分为两次，第一次在最开始的时候存下特征的表达式及其id，第二次在训练结束的时候存下lambda向量。在第一次存盘结束后，就释放掉特征表达式的缓存。这个缓存原始的crf++使用std::map <std::string, std::pair<int, unsigned int> > EncoderFeatureIndex::dic_;来管理的。我自己用标准c++ without stl重写了一个map。存盘结束后的内存释放又获得了10%的内存减少。
现在，主要的问题是，虽然只是影响训练数据装载的速度，但是我自己写的map在插入数据的时候比stl的要慢10倍。难道我要去偷看一遍stl? 困挠中。。。

CRF几个关键步骤：
目标函数：
/sum_X p(X){log Z - /sum_{i, Y}p~(y|x)/lambda_i f_i}
对lambda_i求偏导后为
/sum_X { - f_i + sum_Y p(y|x)f_i}
其中p(y|x)f_i为
/sum_t p(y_t|x)/sum_j f_j(y_t, x_{tj})
1、读入template,根据template生成feature，建立sequence, clique, node, feature之间映射关系，以方便更新/lambda
2、计算每个sequence中每个clique的potential，为forward-backward做准备
3、计算/sum_{i,Y}p~(y|x)/lambda_i f_i，即计算/sum_i /lambda count(f_i(x, y_key))
4、用forward-backward计算/alpha, /beta, z
5、计算gradient
6、扔lbfgs
pocket crf:
sequence存node的一个指针，并且存该X长度
clique存feature指针，feature个数，以及node的数组指针，node个数，以及y
node存clique指针，个数，以及y
这样就可以先把feature实际内容先写到硬盘上了
为了实现高阶CRF，大牛用了一个
vector<vector<vector<int> > > templet_group;
记录一个clique的某条边上的特征会影响到的path编号……
然后对于每个x每个clique就有
for(k=0;k<cli.feature_num;k++)
{
for(ii=0;ii<cli_para_num;ii++)
{
for(jj=0;jj<group[ii].size();jj++)
cur_path[group[ii][jj]]+=lambda[cli.fvector[k]+ii];
}
}
大牛果然很BT！
crf++
大体与pocket crf相似（说pocket crf与crf++大体相似是不是更好点@@）
由于要穷举所有可能的特征与y的组合，在buildLattice之前会针对当前x生成feature
另外beta不是马尔可夫的后向
在node里存lpath和rpath，便于计算边际概率时枚举
在path里存lnode和rnode，计算M时就很方便，但是这样就只能做一阶的了
很多细节还没看清楚，taku kudo写程序不写注释……而且会用一些很寒的宏@@

mallet:
很强大的包……什么都有……crf似乎是mccallum和Fernando Pereira合写的
没有穷举特征，但是做了特征选择
里面能看到些有意思的东西
比如getherconstraints这样的方法名称
不过当我看到左一个继承右一个继承，构造方法里还super，就在想这个效率……

[未完]
写HCRF设想：
class sequence
{
lnode *nodes;
hnode *nodes;
int node_num;
}
class node
{
clique **cliques;
unsigned short clique_num;
int y; int z; //或者是y，或者是z
}
class clique
{
int *fvector;
node **nodes;
clique **clique //记录在相同x下其他clique的指针，这个虽然极限情况下长度是N*N的，但是实际中应该没有那么BT
unsigned short feature_num;
unsigned short int node_num;
unsigned short int clique_num;
}
/alpha(i,j)=/sum_y q(j-1, j)*/alpha(i, j-1) //第一种情况
+/sum_z/sum_y q(i-1, i)q(j-1, j)*/alpha(i-1, j-1)  //第二种情况
/beta(i, j)=/sum_y q(j, j+1)* /beta(i, j+1) //第一种情况
+/sum_z/sum_y q(i, i+1)q(j, j+1)*/alpha(i+1, j+1) //第二种情况
发现我对边际概率计算有疑惑
我想估计的是p(/vec y, /vec z| /vec x)
其实我模型里还有个对齐，那么实际我想估计的是p(/vec y, /vec z, /vec /alpha|/vec x)咯？
那么如果我想估计clique(z_i, z_{i+1})的边际，是不是应该去计算/sum_y/sum_/alpha p(/vec y, /vec z, /vec /alpha|/vec x)？这样似乎代价太大了……近似下用/sum_y
 {i...j} p(/vec y, /vec z, /vec/alpha|/vec x)可以么？

不想穷举所有可能的组合了。一个clique记录一个x下相应的y,z的一个出现过的组合，并且记录下所有其他组合的clique的指针
这样计算path时只要找到同x下其他y,z就可以了……
**CRF工具包使用简介**
阅读(55) 评论(0) 发表时间：2009年02月23日 21:05
本文地址：http://qzone.qq.com/blog/55757749-1235394359

|最近我刚开始使用CRF工具包，里边的具体细节也没有研究明白，但论坛上有人询问工具包的使用，我就将它贴出来一下。希望和大家一起讨论讨论。
|另外需要说明的一点是，下面的内容是从crf工具包网页内容翻译的，可能理解上有偏差，翻译也有问题。所以还请大家指正。英文原文请大家参考网页。
|－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
|CRFs工具包
|工具包地址：http://chasen.org/~taku/software/CRF++/\#features，工具包包括Linux环境下的源代码和Windows下的可执行程序。用C++编写。
|使用方法：
|(1)安装
|编译器要求：C++编译器(gcc 3.0或更高)
|命令：
|% ./configure
|% make
|% su
|\# make install
|注：只有拥有root帐号的用户才能成功安装。
|(2)使用
|(2.1)训练和测试文件的格式
|训练和测试文件必须包含多个tokens，每个token包含多个列。token的定义可根据具体的任务，如词、词性等。每个token必须写在一行，且各列之间用空格或制表格间隔。一个token的序列可构成一个sentence，sentence之间用一个空行间隔。
|最后一列是CRF用于训练的正确的标注形式。
|例如：
|He        PRP  B-NP
|reckons   VBZ  B-VP
|the       DT   B-NP
|current   JJ   I-NP
|account   NN   I-NP
|deficit   NN   I-NP
|will      MD   B-VP
|narrow    VB   I-VP
|to        TO   B-PP
|only      RB   B-NP
|\#         \#    I-NP
|1.8       CD   I-NP
|billion   CD   I-NP
|in        IN   B-PP
|September NNP  B-NP
|.         .    O
|He        PRP  B-NP
|reckons   VBZ  B-VP
|..
|上面的例子每个token包含3列，分别为词本身、词性和Chunk标记。
|注意：如果每一个token的列数不一致，系统将不能正确运行。
|(2.2)准备特征模板
|使用该CRFs工具的用户必须自己确定特征模板。
|?基本模板和宏
|模板文件中的每一行代表一个template。每一个template中，专门的宏%x[row,col]用于确定输入数据中的一个token。row用于确定与当前的token的相对行数。col用于确定绝对行数。
|如已知下面的输入数据：
|Input: Data
|He        PRP  B-NP
|reckons   VBZ  B-VP
|the       DT   B-NP << 当前的token
|current   JJ   I-NP
|account   NN   I-NP
|特征模板形式为：
|templateexpanded feature
|%x[0,0]the
|%x[0,1]DT
|%x[-1,0]rokens
|%x[-2,1]PRP
|%x[0,0]/%x[0,1]the/DT
|ABC%x[0,1]123ABCthe123
|?模板类型
|有两种模板类型，它们可由template的第一个字符确定。
|第一种是Unigram template：第一个字符是U
|这是用于描述unigram feature的模板。当你给出一个模板"U01:%x[0,1]"，CRF会自动的生成一个特征函数集合(func1 ... funcN)，如：
|func1 = if (output = B-NP and http://bbs.langtech.org.cn/images/smilies/default/biggrin.gif" border=0>T") return 1 else return 0
|func2 = if (output = I-NP and http://bbs.langtech.org.cn/images/smilies/default/biggrin.gif" border=0>T") return 1 else return 0
|func3 = if (output = O and http://bbs.langtech.org.cn/images/smilies/default/biggrin.gif" border=0>T") return 1  else return 0
|....
|funcXX = if (output = B-NP and feature="U01:NN") return 1  else return 0
|funcXY = if (output = O and feature="U01:NN") return 1  else return 0
|...
|一个模型生成的特征函数的个数总数为L*N，其中L是输出的类别数，N是根据给定的template扩展出的unique string的数目。
|第二种类型Bigram template:第一个字符是B
|这个模板用于描述bigram features。使用这个模板，系统将自动产生当前输出token与前一个输出token(bigram)的组合。产生的可区分的特征的总数是L*L*N，其中L是输出类别数，N是这个模板产生的unique features数。当类别数很大的时候，这种类型会产生许多可区分的特征，这将会导致训练和测试的效率都很低下。
|?使用标识符区分相对位置
|如果用户需要区分token的相对位置时，可以使用标识符。
|比如在下面的例子中，宏"%x[-2,1]"和"%x[1,1]"都代表“DT”，但是它们又是不同的“DT“。
|The       DT  B-NP
|pen       NN  I-NP
|is        VB  B-VP << CURRENT TOKEN
|a         DT  B-NP
|为了区分它们，可以在模型中加入一个唯一的标识符(U01: 或 U02，即：
|U01:%x[-2,1]
|U02:%x[1,1]
|在这样的条件下，两种模型将被认为是不同的，因为他们将被扩展为”U01:DT“和”U02:DT”。只要你喜欢，你可以使用任何标识符，但是使用数字序号区分更很有用，因为它们只需简单的与特征数相对应。
|例子：下面是用于CoNLL2000中shared task和Base-NP chunking task的模板实例。只使用了bigram template B。这意味着前一个output token和current token将组合成bigram features。每个以\#起始的一行或者是空行都会被当作注释去掉。
|\# Unigram
|U00:%x[-2,0]
|U01:%x[-1,0]
|U02:%x[0,0]
|U03:%x[1,0]
|U04:%x[2,0]
|U05:%x[-1,0]/%x[0,0]
|U06:%x[0,0]/%x[1,0]
|U10:%x[-2,1]
|U11:%x[-1,1]
|U12:%x[0,1]q
|U13:%x[1,1]
|U14:%x[2,1]
|U15:%x[-2,1]/%x[-1,1]
|U16:%x[-1,1]/%x[0,1]
|U17:%x[0,1]/%x[1,1]
|U18:%x[1,1]/%x[2,1]
|U20:%x[-2,1]/%x[-1,1]/%x[0,1]
|U21:%x[-1,1]/%x[0,1]/%x[1,1]
|U22:%x[0,1]/%x[1,1]/%x[2,1]
|\# Bigram
|B
|(3)训练（编码）
|使用crf_learn 命令：
|% crf_learn template_file train_file model_file
|其中，template_file和train_file需由使用者事先准备好。crf_learn将生成训练后的模型并存放在model_file中。
|一般的，crf_learn将在STDOUT上输出下面的信息。还会输出其他的和LBFGS迭代相关的信息。
|% crf_learn template_file train_file model_file
|CRF++: Yet Another CRF Tool Kit
|Copyright (C) 2005 Taku Kudo, All rights reserved.
|reading training data:
|Done! 0.32 s
|Number of sentences:          77
|Number of features:           32856
|Freq:                         1
|eta:                          0.0001
|C(sigma^2):                   10
|iter=0 terr=0.7494725738 serr=1 obj=2082.968899 diff=1
|iter=1 terr=0.1671940928 serr=0.8831168831 obj=1406.329356 diff=0.3248438053
|iter=2 terr=0.1503164557 serr=0.8831168831 obj=626.9159973 diff=0.5542182244
|其中：
|iter：迭代次数
|terr：和tags相关的错误率(错误的tag数/所有的tag数)
|serr：与sentence相关的错误率（错误的sentence数/所有的sentence数）
|obj：当前对象的值。当这个值收敛到一个确定的值是，CRF模型将停止迭代
|diff：与上一个对象值之间的相对差
|有两个主要的参数用于控制训练条件：
|-c float：使用这个选项，你可以改变CRF的hyper-parameter。当取一个极大的C值，CRF将可能对训练数据产生过拟合（overfitting）现象。这个参数将会调节overfitting和underfitting之间的平衡。结果将会对参数带来有意义的影响。使用者可以通过使用held-out data或者更多的通用模型的选择方法如十字交叉验证法(cross validation)获得最有的值。
|-f NUM：这个参数用于设置特征的cut-off阈值。CRF++训练时只使用出现次数不少于NUM次数的特征进行训练。默认值为1。当使用CRF++训练大规模数据时，单一特征的数量将达到数百万，此时选择这个参数很有用。
|这里有一个使用这两个参数的例子：
|% crf_learn -f 3 -c 1.5 template_file train_file model_file
|(4)测试（解码）
|使用crf_test 命令：
|% crf_test -m model_file test_files ...
|其中，model_file是crf_learn创建的。在测试过程中，使用者不需要指定template file，因为，mode file已经有了template的信息。test_file是你想要标注序列标记的测试语料。这个文件的书写格式应该与训练文件一致。
|下面是一个crf_test输出的例子：
|% crf_test -m model test.data
|Rockwell        NNP     B       B
|International   NNP     I       I
|Corp.   NNP     I       I
|'s      POS     B       B
|Tulsa   NNP     I       I
|unit    NN      I       I
|..
|其中，最后一列是模型估计的tag。如果第三列是标准的tag，则可以通过简单的比较第三列和第四列之间的差别计算出准确率。
|详细的层次（verbose level）
|-v选项：将设置verbose level。默认值为0。通过增加层次，你可以从CRF++获得额外的信息。
|层次1：
|你可以对每个tag使用边缘概率(marginal probabilities)（这是一种对输出tag的confidence measure），对输出使用条件概率(conditional probably)（针对整个输出的confidence measure）。
|例如：
|% crf_test -v1 -m model test.data| head
|\# 0.478113
|Rockwell        NNP     B       B/0.992465
|International   NNP     I       I/0.979089
|Corp.   NNP     I       I/0.954883
|'s      POS     B       B/0.986396
|Tulsa   NNP     I       I/0.991966
|...
|其中，第一行的"\# 0.478113"即是输出的条件概率，而且每一个输出tag各自含有一个概率，表示形式如“B/0.992465”。
|层次2：
|你可以对所有的其他候选求边缘概率。
|例如：
|% crf_test -v2 -m model test.data
|\# 0.478113
|Rockwell        NNP     B       B/0.992465      B/0.992465      I/0.00144946    O/0.00608594
|International   NNP     I       I/0.979089      B/0.0105273     I/0.979089      O/0.0103833
|Corp.   NNP     I       I/0.954883      B/0.00477976    I/0.954883      O/0.040337
|'s      POS     B       B/0.986396      B/0.986396      I/0.00655976    O/0.00704426
|Tulsa   NNP     I       I/0.991966      B/0.00787494    I/0.991966      O/0.00015949
|unit    NN      I       I/0.996169      B/0.00283111    I/0.996169      O/0.000999975
|..
|N-best outputs
|-n选项：使用这个选项将获得N-best结果，它根据CRF计算出的条件概率排序得到。当选择了N-best结果，CRF++会自动添加一行，形式为“\# N prob”，其中N是排序后的输出结果，从0开始。prob表示output的条件概率。
|需要注意的是，如果CRF++不能找到足够的N条路径是，它将放弃列举N-best结果。这种情况在所给的句子很短的情况下常出现。
|CRF++使用前向Viterbi和后向A*搜索相结合的方法。这种结合适应了n-best结果的需求。
|下面是一个N-best结果的例子：
|% crf_test -n 20 -m model test.data
|\# 0 0.478113
|Rockwell        NNP     B       B
|International   NNP     I       I
|Corp.   NNP     I       I
|'s      POS     B       B
|...
|\# 1 0.194335
|Rockwell        NNP     B       B
|International   NNP     I       I
|


下面几个方面可能能够进一步提高CRF方法系统的性能：
1、移除Unigram矩阵写的操作（对CRF方法毫无意义），启用去年10月份开发的一遍扫描Unigram就全部完成的算法。
2、将计算score的部分用sse指令优化
3、将内码重新改为UCS2编码
CRF分词,v2 完成
没什么技术难度，主要是改code。将原有的N-gram的代码剥离，改内码为UCS-2(UTF-16)。
速度方面：
在MSR_Test文件上，达到了710kb/s的成绩（PM-1.2）、920kb/s(AMD x2 5600+)；
在一个30M的文本上（从起点下的也不知道讲什么的垃圾东西，反正是中文:-0)，速度达到了峰值1.5M/s(AMD x2 5600+)。
由于AMD的PR Ranking的命名方法，可以认为该CPU的单核计算接近P-IV 3.0G，考虑到我是采用UTF-8作为数据交换码，如使用GB码数据量应减少到2/3，峰值性能仍然接近1M/s，达到（可能超过了，因为CRF方法是内置OOV识别的）I3s系统速度上的性能指标；超过了海量科技声称的~700Kb/s。由字构词方法的性能（速度）已经达到了可以投入实际应用的水平。
N-gram方法彻底在分词领域退出历史舞台的日子已经不远了。
==================================
此外，关于CRF工具包的内存占用的问题：
我用6+6的方法训练PKU 98[1-6]的全部语料库+我修改过的CRF++工具包，生成.Feature文件占用内存~5G，实际Feature等辅助文件~800M；实际训练占用内存2G；使用官方CRF++，占用内存6.2G，在我的6G MEM的AMD系统上出现内存颠簸，无法高效的进行训练。而实际上，目前版本的CRF++如果单单是6Tag-set，则内存占用仍可减小1/2，训练速度可以提升2~3倍（这是分词系统开发的实际数据）。在网上流传的CRF方法计算时空复杂度高的说法是不确切的。虽然CRF比最大熵要训练上慢很多，但是通过技术手段，其内存消耗和速度并没有想像的大，只不过是CRF++这个软件包没有好好优化过而已。

MSR语料取10000个语句
iter=507 terr=0.00112 serr=0.02101 act=1759956 obj=20359.97829 diff=0.00002
Done!1786.48 s















doubleTaggerImpl::gradient(double *expected) // CRF过程
double *expected大小为词典集合大小
{
buildLattice();
forwardbackward();
viterbi();
}
node_为M*N为矩阵
M是训练数据中Token的数量
N是训练数据的标记集合数量
unsignedintx;
unsignedshortinty;
doublealpha;
doublebeta;
doublecost;
doublebestCost;
Node*prev; //前驱节点
int*fvector; //特征向量
std::vector<Path*>lpath; //前驱
std::vector<Path*>rpath;//后趋









CRFPP::Encoder encoder;
encoder.learn
参数解释:
constchar*templfile,//模板文件
constchar*trainfile,//训练数据
constchar*modelfile,//输出模型
booltextmodelfile,//结果是否文本输出
size_tmaxitr,//迭代次数
size_tfreq,
doubleeta,//迭代误差
doubleC,
unsignedshortthread_num,//线程数
unsignedshortshrinking_size,
intalgorithm)

// 读取训练数据
// 以空行分离句子
while(ifs)
{
…
TaggerImpl*_x=newTaggerImpl(); //一个训练语句
_x->open(&feature_index); //指向特征集/标记集
_x->set_thread_id(line%thread_num);//划分训练线程
if(!_x->read(&ifs) ||//Read train.data
!_x->shrink())//
{
// shrink过程
for(size_tcur= 0;cur<tagger->size(); ++cur)//遍历语句的每一个Token
{
///
for(std::vector<char*>::const_iteratorit=unigram_templs_.begin();
it!=unigram_templs_.end(); ++it) //每一个标记
{
(apply_rule(&os, *it,cur, *tagger))//Token+Tag组合
// 将Token+Tag组合添加入trie，如已添加则频率累加
// 类似词典
ADD;//for debug
}
feature_cache_.add(feature);
feature.clear();
}
}
…
}

L-BFGS算法

**对CRF++软件包的具体修改如下：**
**1、**crl_learn 增加了一个新算法参数，mmap。使用该参数后，会在磁盘上生成两个文件
model.features, model.train
**2、**crl_learn 增加了一个操作参数， -M
注意是大写。此时crl_learn会从磁盘上读取train的数据，计算alpha，在函数crf_train中
**主要的改进：**
**1、**读取数据与实际训练分开，内存降低到原来的1/2
**2、**采用内存映射文件，理论上原来占1.2G的训练任务可以降低到100M一下（分区计算）
**3、**加入节点池，系统记录train中用到的最长的tagger，直接分配node_**
**TODO：**
**1、**增加的新的crf_train函数，没有保存alpha值的功能（当然也没有直接保存到model文件的功能。实际上，我们已经完成了保存alpha值的功能，公开的版本没有加入，可以视为我们给使用这个版本CRF++的学生的一道编程作业。），这个需要你自己加，不然白算了:-）
**2、**在计算过程中最小化当前窗口，可以显著的降低内存消耗（MMAP)，但是，如果将train数据进行分区，还可以将内存占用降低到原来的1/10
**3**目前的代码仅在win32+vs2003上测试通过，Linux上的情况不保证正常工作



Done!128.61 s
Number of sentences: 86909
Number of features:  6419756
Number of thread(s): 1
Freq:                1
eta:                 0.00010
C:                   1.00000


















原因在于，我们引入了复杂的标注集来标注字在词中的位置。说来话长，理论上来说，切分问题是一个二值分类问题，切还是不切（哈姆雷特：To be, or not to be: that is the question，：））。但是第一个关于字标注的工作，
/bibitem{Xue:2003a}
Nianwen Xue. 2003. Chinese Word Segmentation as Character Tagging. {/em Computational Linguistics and Chinese Language Processing}, Vol. 8(1): 29-48.
并不是将其作为二值标注问题来处理的，而是作为4值标注问题。具体来说，就是将字在一个词中间的位置分为4类：词首B，词尾E，词中M，单字词S。作为二值标注问题，可能最开始由Peng Fuchun在coling-04引入。为什么会出现这样一种“退步”？原因在于计算负载上，Xue用的是最大熵(MaxEnt)，而Peng用的CRFs。显然的，CRF的计算负载一般要比MaxEnt高一个数量级，而且，CRF上用4个标注的学习代价比2个要高一倍不止。实际上，后续的CRF模型，只要是用于分词的，能够不用2个词位标注的学习非常非常罕见。
但是，但是，我们工作的要点就是要引入更多的词位标注来进行更为精确的词位学习。具体来说，我们进一步引入了2个额外的词位，B2,B3，分别代表多字词中第二个和第三个字的非词尾的位置。这样子，我们在CRFs中相当于前所未有的用到了6个词位标注。与此同时，我们大大降低了特征模版的数量。
下面对比三个特征模版集的不同，分别针对2，4，6标注集。我们分别用T,
 X, Z表示Tsai et al. 2006，Xue:2003/Low:2005, Zhao:2006用到的针对这三个标注集的特征模版：
---------------------------------------------------------------------------------
C-2, C-1, C0, C1, C2, C-2C-1, C-1C0, C0C1, C1C2, C-1C1, C-2C0, C-3C-1
T:   x    x    x   x        x       x     x             x     x      x
X:   x    x    x   x   x    x       x     x      x      x
Z:        x    x   x                x     x             x
---------------------------------------------------------------------------------
可以看到，Tsai, Xue/Low都要用到10个特征模版，而Zhao只需要6个。从特征模版来看，Tsai的模版涉及到5个字的上下文窗口：C-3到C1,
 Xue/Low涉及到5个字的上下文窗口：C-2到C2，Zhao涉及到三个字的上下文窗口：C-1到C1。这似乎显示Zhao的模版学习的上下文窗口宽度不足。但是，如果我们观察，不重复的使用词位标注集标注一个尽可能长的词
2-tag,两字词，BE
4-tag,三字词，BME
6-tag,五字词，BB2B3ME
也就是说，6-tag通过标注集而不是模版集足以保证学习5字上下文窗口。
如果把我们的目标归结为寻求一种性能最好的n-gram特征和标注集搭配。我们将会发现6-tag+6-ngram会是最佳的。为此，我在同样的CRF模型上用Tsai的2-tag标注和他们的n-gram特征模版（实际上，我发现在所有用于2标注集的特征模版中，他们的这组是性能最好的），Xue/Low的4-tag标注和他们的n-gram特征模版，进行了性能评估。下面是在Bakeoff-3数据上的F-score结果比较
AS    CityU    CTB    MSRA
2-tag+Tsai   .946   .962    .924   .946
4-tag+Xue    .952   .967    .930   .955
6-tag+Zhao   .954   .969    .932   .961
那么是什么阻止我们使用更多的标注集呢，可能是训练计算的时间太长。
下面是训练时间（单位是分）的比价，所有的数据都在一致的计算环境中获得
AS    CityU    CTB    MSRA
2-tag+Tsai   112     52      16     35
4-tag+Xue    206     79      28     73
6-tag+Zhao   402    146      47     117
可以看到，6-tag比4-tag基本上要多花一倍的训练时间，比2-tag要多花2-3倍的训练时间。
再看训练内存使用(单位Giga bytes)
AS    CityU    CTB    MSRA
2-tag+Tsai   5.4     2.4     0.9    1.8
4-tag+Xue    6.6     2.8     1.1    2.2
6-tag+Zhao   6.4     2.7     1.0    2.1
注意到，6-tag用的内存甚至比4-tag系统还要少！
所以，去买一台快点的计算机跑分词吧，因为我们不需要一个复杂的系统来做分词。简单的6-tag+6-ngram就够了。
最后，我说一下字符类型特征和开放测试的问题。
所谓字符类型特征，就是标记字符是正常的汉字，还是拉丁字母，阿拉伯数字，还是标点符号的特征。Bakeoff-3明确禁止在封闭测试中使用这一特征。不幸的是，很多人，包括我，没有注意到网页上的这一条款。
下面是bakeoff-3在分词封闭测试中至少获得前三名的参赛者及其成绩（F-score）列表，总共有6个参赛者。
------------------------------------------
Participant   AS    CityU   CTB    MSRA
(Site ID)
------------------------------------------
Zhu(1)       0.944  0.968   0.927  0.956
Carpenter(9) 0.943  0.961   0.907  0.957
Tsai(15)     0.957  0.972   /      0.955
Zhao(20)     0.958  0.971   0.933  /
Zhang(26)    0.949  0.965   0.926  0.957
Wang(32)     0.953  0.970   0.930  0.963
------------------------------------------
Best closed  0.958  0.972   0.933  0.963
------------------------------------------
其中，Zhu, Tsai, Zhao用了字符类型特征，Zhu和Zhao是直接从预先指定的字符类型列表中构造这一特征。Tsai用了一个别出心裁的方法，通过几条预定义的目标函数进行聚类来获得字符类型列表。当然，他们重新“发现”了其实事先就知道的不同的字符类型。也许，他们的这种做法才是最没有犯规的。但是，这种做法的实用价值是值得商榷的，是否有为了评估而评估之嫌？
关于字符类型特征的作用，我做过事后的经验性评估。我的结论是：字符类型特征在某型情形的确是发挥作用的，也就是对于性能的提升的确有帮助，但是，这种性能增进强烈的依赖于两个方面，第一个是所学习的文本的全半角是否统一，如果某些字母，数字，标点是半角，某些是全角。那么，字符类型特征会有较大的改进作用。反之，如果训练和测试语料中的全半角是基本一致的，也就是基本都是半角或者都是全角，那么，该特征将不起什么作用。第二个因素是训练语料的规模，不管全半角是否一致，该特征在大规模训练语料下对于性能改进微乎其微，相反，对于小规模的训练语料的性能改进较为可观。简而言之，字符类型特征不会导致性能的实质性增长。从这个角度来说，规定在封闭测试中禁止或者使用这一特征都讲得通。
这里顺便说一下，Wang(32)在MSRA上封闭测试的结果，他们使用了语言模型概率和MaxEnt输出联合解码的方法。他们的方法依赖于一个分配这两种输出的权重参数。不幸的是，根据他们的报告，在微软语料上的这一参数调整来自Bakeoff-2005的微软语料。而Bakeoff-2006微软训练语料只是2005的相应语料的一个子集（正是我建议黄老师取一半的2005语料提交为2006语料的），这样算不算引入了外部语言资源的信息？也许这个可以解释他们的这个成绩为什么比第二名高那么多吧
关于开放测试
目前为止，我只谈了封闭测试。为什么我不愿意将开放测试作为另外一个重点。原因就是在开放测试中的方法论其实相当的贫乏和单调，尽管Bakeoff的开放测试允许一切额外的语言资料和外部知识。
两种显而易见的资源已经被往届的Bakeoff参赛者集成到分词学习中了。一个是词典，一个是额外的切分语料。这一工作依然由Low and Ng在Bakeoff-2005完成。Bakeoff-3(2006)有两个值得一提的工作，一个是引入了第三种开放资源，词性标注(POS
 tagging)结果。这一工作在，
/bibitem{Wang and Shi:2006}
Mengqiu Wang and Yanxin Shi, Using Part-of-Speech Reranking to Improve Chinese Word Segmentation, SIGHAN-5.
另外一个试集成方法的改进。这个依然是我们的工作，我们建议直接引入其他的切分器的输出作为特征。我们称之为辅助切分器方法。这样，词典，附加语料都可以统一为辅助切分器，前者可以用最大匹配辅助切分器，后者可以用在附加语料上训练出来的切分器。这样做的优势，除了方法统一外，还有助于应用到CRF学习上，因为额外语料方法会大大增加训练时间。这对CRF训练来说是一个灾难。额外的，我们还证明，辅助的命名实体识别输出也是对分词性能改进有帮助的。
分词开放测试所追求的高性能的实质是什么？我理解为，我们需要更多的异质的切分语料。本质上来说，机器学习工具在记忆所训练的语料，它只能在和训练语料中出现过的相似度很高的句子上完成正确切分。由于机器学习技术的进步实在有限，其推广能力并不出众。所以，只要能集成进去如果多的额外语料，不管用什么方法。能够让机器学习工具记住曾有这样的句子，曾经有这样的切分，那么我们就能获得足够高的性能。
关于词典，我要说，额外的词典或许是起正面作用的。我一直认为，不在于是否使用词典，而在于使用什么样的词典。或者说，我们要用那些“好”的词典。词典并非越大越好，也不是所有的词典能够改进分词性能。有很多这样的负面的例子，很遗憾，我们不能发表这些结果，因为所有的出版出来的学术文章都只能是正面的，积极的结果（或许也是作者选择或投审稿人所好的结果）。关于陷阱，误区的文章实在太难发了。
实际上也很简单的，至少在解码/斜率计算上面。由于某种技术失误，path对象实现为了copy的模式，导致memmove调用在占用了大约50%的CPU时间。由于CRF++要支持多种Tag-set，因此Node的incoming&outcoming再用了vector。实际上，我们日常的训练往往一套tag-set要跑很多遍，这个地方可以改用数组。
我不知道其他的应用(Pos,NER)训练时间如何，至少CRF++目前的速度在分词上足够了。基本上6~7个小时出结果。
而实践上，由于实现者的不同，同样的算法效率差2~4倍都是很正常的。当年的DOOM大大们在i386上曾经提升速度达到了30倍，admire.

用于中文分词评测的语料有:
|CKIP
|台湾中央研究院(721551 词次,训练集)
|
|CityU
|香港城市大学(1092687 词次,训练集)
|
|CTB
|美国科罗拉多大学(642246 词次,训练集)
|
|NCC
|中国教育部国家语委(917255 词次,训练集)
|
|SXU
|山西大学(528238|词次,训练集|)
|
用于中文命名实体识别评测的语料有:
|CityU
|香港城市大学(1772202 字,训练集)
|
|MSRA
|微软亚洲研究院(1089050|字,训练集|)
|
|PKU
|北京大学(1833177 字,训练集)
|
用于中文词性标注评测的语料有:
|CKIP
|台湾中央研究院(721551 词次,训练集)
|
|CityU
|香港城市大学(1092687 词次,训练集)
|
|CTB
|美国科罗拉多大学(642246 词次,训练集)
|
|NCC
|中国教育部国家语委(535023 词次,训练集)
|
|PKU
|北京大学(1116754 词次,训练集)
|


目前crf在machine learning中仍旧活跃。对于crf的研究可以分为以下几个方面：(欢迎大家补充^^)
1)改变结构,使得crf可以适应更复杂的任务
比如：Dynamic crf
http://www.cs.umass.edu/~mccallum/papers/dcrf-nips03.pdf
skip chain crf
http://www.cs.umass.edu/~mccallum/papers/tr-04-49.pdf
2)加快crf训练速度
piecewise linear crf
http://www.machinelearning.org/proceedings/icml2007/papers/549.pdf
EG algorithm for crf training
http://people.csail.mit.edu/gamir/pubs/olcrf.pdf
3) feature selection
http://www.cs.umass.edu/~mccallum/papers/ifcrf-uai2003.pdf
4) semi supervised crf
http://www.metabolomics.ca/News/publications/Jiao_et_al.pdf
http://pages.cs.wisc.edu/~jerryzhu/pub/kcrf-revised.pdf
5) other:
semi crf
http://www.cs.cmu.edu/~wcohen/postscript/semiCRF.pdf
sparse crf
http://www.cs.umass.edu/~mccallum/papers/sparse-fb.pdf
命名实体识别是自然语言处理的重要内容,它的研究和实现具有重要的理论意义和实用价值。但目前这方面的研究多集中在现代文献领域,古籍中命名实体的识别是古典文献信息化进程中的关键所在,是一个值得探索研究的问题。本文以《三国演义》为例对基于条件随机域(Conditional
 Random Fields,CRF)的古籍地名自动识别做了一些探索,本文的结构和主要内容安排如下:第一章、绪论首先说明了本文研究的目的、意义。再围绕本文中心任务介绍了相关的基础理论和研究背景,主要是自然语言理解的含义和思想,还简述了古代文献电子信息化的研究现状和前景。第二章、命名实体的识别及其方法先引出命名实体识别这个问题,然后介绍了命名实体识别的意义和方法,重点在基于统计的方法。第三章、CRF模型CRF模型是本文中研究古籍地名自动识别的主要方法,本章就是从理论上对CRF模型的介绍,重点讨论了该模型应用于命名实体识别时的情况。
海量分词
下面是海量的在bakeoff 2005上的得分（iExtraCalcFlag==8，面向搜索的切分）：
PKU的测试语料
F score  =0.886461
Precision=0.885664
Recall   =0.887259
从其接口说明文件来看，可能使用的也是CRF类似的产生式模型再叠加其他模型。不过与CRF目前看到的结果不同，海量的结果很少生造出来中文不存在的词，其切分错误主要出现在
1、将原本测试语料库中的两个词合成了一个词
2、将原本测试语料库中的一个词切成了两个词。

