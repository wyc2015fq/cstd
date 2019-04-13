
# 使用gensim和sklearn搭建一个文本分类器（二）：代码和注释 - Multiangle's Notepad - CSDN博客


2016年09月12日 17:49:00[multiangle](https://me.csdn.net/u014595019)阅读数：7616标签：[gensim																](https://so.csdn.net/so/search/s.do?q=gensim&t=blog)[sklearn																](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)[nlp																](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)[自然语言处理																](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)个人分类：[python																](https://blog.csdn.net/u014595019/article/category/5636921)[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[自然语言处理																](https://blog.csdn.net/u014595019/article/category/6183383)[
							](https://blog.csdn.net/u014595019/article/category/3051069)
[
																					](https://blog.csdn.net/u014595019/article/category/5636921)所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)[
							](https://blog.csdn.net/u014595019/article/category/5636921)
[
				](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=gensim&t=blog)
在之前的[ 使用gensim和sklearn搭建一个文本分类器（一）：流程概述](http://blog.csdn.net/u014595019/article/details/52433754)中，叙述了一个使用lsi来对文本进行向量化，再使用线性核svm进行分类的文本分类器。在这篇文章中，沿着之前的思路，提供了该文本分类器的具体实现。该分类器对之前的流程进行了适当的整合，现在有五个主要流程：
生成词典
生成tfidf向量
生成lsi向量
分类器参数训练
对新文本进行分类。
前4个步骤可以看做是分类器的训练过程，而第五个阶段，则是使用训练得到的参数对新文本进行分类。
所有步骤都写在main函数中，并使用标记隔开以保证可读性。**由于这5个阶段相对独立，因此每一阶段会将阶段成果存入磁盘，这样后一阶段会直接从磁盘读取之前的成果，不再重复执行上一阶段的代码。如果想重新运行某个阶段，只要把对应的结果删除即可**
这里要注意，在运行程序前要先指定一些路径，其中最重要的要属path_doc_root和path_tmp了。前者存放为存放文本文件的目录，后者存放运行程序时生成的中间文件。对于前者，我这里的值是
```python
path_doc_root =
```
```python
'/media/multiangle/F/DataSet/THUCNews/THUCNewsTotal'
```
也就是说，所有的文本资料都应该按类放于该文件夹下，每个类都是一个文件夹， 每个类别文件夹下都存放了许多文档，一个文档是一个txt文件。如下图所示。我这里用的是THUNews的文件集
![这里写图片描述](https://img-blog.csdn.net/20160912192855155)[ ](https://img-blog.csdn.net/20160912192855155)
![这里写图片描述](https://img-blog.csdn.net/20160912193020547)
具体的代码如下所示
```python
from
```
```python
gensim
```
```python
import
```
```python
corpora, models
```
```python
from
```
```python
scipy.sparse
```
```python
import
```
```python
csr_matrix
```
```python
from
```
```python
sklearn.discriminant_analysis
```
```python
import
```
```python
LinearDiscriminantAnalysis
```
```python
from
```
```python
sklearn
```
```python
import
```
```python
svm
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
import
```
```python
os,re,time,logging
```
```python
import
```
```python
jieba
```
```python
import
```
```python
pickle
```
```python
as
```
```python
pkl
```
```python
# logging.basicConfig(level=logging.WARNING,
```
```python
#                     format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
```
```python
#                     datefmt='%a, %d %b %Y %H:%M:%S',
```
```python
#                     )
```
```python
class
```
```python
loadFolders
```
```python
(object)
```
```python
:
```
```python
# 迭代器
```
```python
def
```
```python
__init__
```
```python
(self,par_path)
```
```python
:
```
```python
self.par_path = par_path
```
```python
def
```
```python
__iter__
```
```python
(self)
```
```python
:
```
```python
for
```
```python
file
```
```python
in
```
```python
os.listdir(self.par_path):
            file_abspath = os.path.join(self.par_path, file)
```
```python
if
```
```python
os.path.isdir(file_abspath):
```
```python
# if file is a folder
```
```python
yield
```
```python
file_abspath
```
```python
class
```
```python
loadFiles
```
```python
(object)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self,par_path)
```
```python
:
```
```python
self.par_path = par_path
```
```python
def
```
```python
__iter__
```
```python
(self)
```
```python
:
```
```python
folders = loadFolders(self.par_path)
```
```python
for
```
```python
folder
```
```python
in
```
```python
folders:
```
```python
# level directory
```
```python
catg = folder.split(os.sep)[-
```
```python
1
```
```python
]
```
```python
for
```
```python
file
```
```python
in
```
```python
os.listdir(folder):
```
```python
# secondary directory
```
```python
file_path = os.path.join(folder,file)
```
```python
if
```
```python
os.path.isfile(file_path):
                    this_file = open(file_path,
```
```python
'rb'
```
```python
)
                    content = this_file.read().decode(
```
```python
'utf8'
```
```python
)
```
```python
yield
```
```python
catg,content
                    this_file.close()
```
```python
def
```
```python
convert_doc_to_wordlist
```
```python
(str_doc,cut_all)
```
```python
:
```
```python
sent_list = str_doc.split(
```
```python
'\n'
```
```python
)
    sent_list = map(rm_char, sent_list)
```
```python
# 去掉一些字符，例如\u3000
```
```python
word_2dlist = [rm_tokens(jieba.cut(part,cut_all=cut_all))
```
```python
for
```
```python
part
```
```python
in
```
```python
sent_list]
```
```python
# 分词
```
```python
word_list = sum(word_2dlist,[])
```
```python
return
```
```python
word_list
```
```python
def
```
```python
rm_tokens
```
```python
(words)
```
```python
:
```
```python
# 去掉一些停用次和数字
```
```python
words_list = list(words)
    stop_words = get_stop_words()
```
```python
for
```
```python
i
```
```python
in
```
```python
range(words_list.__len__())[::-
```
```python
1
```
```python
]:
```
```python
if
```
```python
words_list[i]
```
```python
in
```
```python
stop_words:
```
```python
# 去除停用词
```
```python
words_list.pop(i)
```
```python
elif
```
```python
words_list[i].isdigit():
            words_list.pop(i)
```
```python
return
```
```python
words_list
```
```python
def
```
```python
get_stop_words
```
```python
(path=
```
```python
'/home/multiangle/coding/python/PyNLP/static/stop_words.txt'
```
```python
)
```
```python
:
```
```python
file = open(path,
```
```python
'rb'
```
```python
).read().decode(
```
```python
'utf8'
```
```python
).split(
```
```python
'\n'
```
```python
)
```
```python
return
```
```python
set(file)
```
```python
def
```
```python
rm_char
```
```python
(text)
```
```python
:
```
```python
text = re.sub(
```
```python
'\u3000'
```
```python
,
```
```python
''
```
```python
,text)
```
```python
return
```
```python
text
```
```python
def
```
```python
svm_classify
```
```python
(train_set,train_tag,test_set,test_tag)
```
```python
:
```
```python
clf = svm.LinearSVC()
    clf_res = clf.fit(train_set,train_tag)
    train_pred  = clf_res.predict(train_set)
    test_pred = clf_res.predict(test_set)
    train_err_num, train_err_ratio = checkPred(train_tag, train_pred)
    test_err_num, test_err_ratio  = checkPred(test_tag, test_pred)
    print(
```
```python
'=== 分类训练完毕，分类结果如下 ==='
```
```python
)
    print(
```
```python
'训练集误差: {e}'
```
```python
.format(e=train_err_ratio))
    print(
```
```python
'检验集误差: {e}'
```
```python
.format(e=test_err_ratio))
```
```python
return
```
```python
clf_res
```
```python
def
```
```python
checkPred
```
```python
(data_tag, data_pred)
```
```python
:
```
```python
if
```
```python
data_tag.__len__() != data_pred.__len__():
```
```python
raise
```
```python
RuntimeError(
```
```python
'The length of data tag and data pred should be the same'
```
```python
)
    err_count =
```
```python
0
```
```python
for
```
```python
i
```
```python
in
```
```python
range(data_tag.__len__()):
```
```python
if
```
```python
data_tag[i]!=data_pred[i]:
            err_count +=
```
```python
1
```
```python
err_ratio = err_count / data_tag.__len__()
```
```python
return
```
```python
[err_count, err_ratio]
```
```python
if
```
```python
__name__==
```
```python
'__main__'
```
```python
:
    path_doc_root =
```
```python
'/media/multiangle/F/DataSet/THUCNews/THUCNewsTotal'
```
```python
# 根目录 即存放按类分类好的问本纪
```
```python
path_tmp =
```
```python
'/media/multiangle/F/DataSet/THUCNews/tmp'
```
```python
# 存放中间结果的位置
```
```python
path_dictionary     = os.path.join(path_tmp,
```
```python
'THUNews.dict'
```
```python
)
    path_tmp_tfidf      = os.path.join(path_tmp,
```
```python
'tfidf_corpus'
```
```python
)
    path_tmp_lsi        = os.path.join(path_tmp,
```
```python
'lsi_corpus'
```
```python
)
    path_tmp_lsimodel   = os.path.join(path_tmp,
```
```python
'lsi_model.pkl'
```
```python
)
    path_tmp_predictor  = os.path.join(path_tmp,
```
```python
'predictor.pkl'
```
```python
)
    n =
```
```python
10
```
```python
# n 表示抽样率， n抽1
```
```python
dictionary =
```
```python
None
```
```python
corpus_tfidf =
```
```python
None
```
```python
corpus_lsi =
```
```python
None
```
```python
lsi_model =
```
```python
None
```
```python
predictor =
```
```python
None
```
```python
if
```
```python
not
```
```python
os.path.exists(path_tmp):
        os.makedirs(path_tmp)
```
```python
# # ===================================================================
```
```python
# # # # 第一阶段，  遍历文档，生成词典,并去掉频率较少的项
```
```python
#       如果指定的位置没有词典，则重新生成一个。如果有，则跳过该阶段
```
```python
if
```
```python
not
```
```python
os.path.exists(path_dictionary):
        print(
```
```python
'=== 未检测到有词典存在，开始遍历生成词典 ==='
```
```python
)
        dictionary = corpora.Dictionary()
        files = loadFiles(path_doc_root)
```
```python
for
```
```python
i,msg
```
```python
in
```
```python
enumerate(files):
```
```python
if
```
```python
i%n==
```
```python
0
```
```python
:
                catg    = msg[
```
```python
0
```
```python
]
                file    = msg[
```
```python
1
```
```python
]
                file = convert_doc_to_wordlist(file,cut_all=
```
```python
False
```
```python
)
                dictionary.add_documents([file])
```
```python
if
```
```python
int(i/n)%
```
```python
1000
```
```python
==
```
```python
0
```
```python
:
                    print(
```
```python
'{t} *** {i} \t docs has been dealed'
```
```python
.format(i=i,t=time.strftime(
```
```python
'%Y-%m-%d %H:%M:%S'
```
```python
,time.localtime())))
```
```python
# 去掉词典中出现次数过少的
```
```python
small_freq_ids = [tokenid
```
```python
for
```
```python
tokenid, docfreq
```
```python
in
```
```python
dictionary.dfs.items()
```
```python
if
```
```python
docfreq <
```
```python
5
```
```python
]
        dictionary.filter_tokens(small_freq_ids)
        dictionary.compactify()
        dictionary.save(path_dictionary)
        print(
```
```python
'=== 词典已经生成 ==='
```
```python
)
```
```python
else
```
```python
:
        print(
```
```python
'=== 检测到词典已经存在，跳过该阶段 ==='
```
```python
)
```
```python
# # ===================================================================
```
```python
# # # # 第二阶段，  开始将文档转化成tfidf
```
```python
if
```
```python
not
```
```python
os.path.exists(path_tmp_tfidf):
        print(
```
```python
'=== 未检测到有tfidf文件夹存在，开始生成tfidf向量 ==='
```
```python
)
```
```python
# 如果指定的位置没有tfidf文档，则生成一个。如果有，则跳过该阶段
```
```python
if
```
```python
not
```
```python
dictionary:
```
```python
# 如果跳过了第一阶段，则从指定位置读取词典
```
```python
dictionary = corpora.Dictionary.load(path_dictionary)
        os.makedirs(path_tmp_tfidf)
        files = loadFiles(path_doc_root)
        tfidf_model = models.TfidfModel(dictionary=dictionary)
        corpus_tfidf = {}
```
```python
for
```
```python
i, msg
```
```python
in
```
```python
enumerate(files):
```
```python
if
```
```python
i%n==
```
```python
0
```
```python
:
                catg    = msg[
```
```python
0
```
```python
]
                file    = msg[
```
```python
1
```
```python
]
                word_list = convert_doc_to_wordlist(file,cut_all=
```
```python
False
```
```python
)
                file_bow = dictionary.doc2bow(word_list)
                file_tfidf = tfidf_model[file_bow]
                tmp = corpus_tfidf.get(catg,[])
                tmp.append(file_tfidf)
```
```python
if
```
```python
tmp.__len__()==
```
```python
1
```
```python
:
                    corpus_tfidf[catg] = tmp
```
```python
if
```
```python
i%
```
```python
10000
```
```python
==
```
```python
0
```
```python
:
                print(
```
```python
'{i} files is dealed'
```
```python
.format(i=i))
```
```python
# 将tfidf中间结果储存起来
```
```python
catgs = list(corpus_tfidf.keys())
```
```python
for
```
```python
catg
```
```python
in
```
```python
catgs:
            corpora.MmCorpus.serialize(
```
```python
'{f}{s}{c}.mm'
```
```python
.format(f=path_tmp_tfidf,s=os.sep,c=catg),
                                       corpus_tfidf.get(catg),
                                       id2word = dictionary
                                       )
            print(
```
```python
'catg {c} has been transformed into tfidf vector'
```
```python
.format(c=catg))
        print(
```
```python
'=== tfidf向量已经生成 ==='
```
```python
)
```
```python
else
```
```python
:
        print(
```
```python
'=== 检测到tfidf向量已经生成，跳过该阶段 ==='
```
```python
)
```
```python
# # ===================================================================
```
```python
# # # # 第三阶段，  开始将tfidf转化成lsi
```
```python
if
```
```python
not
```
```python
os.path.exists(path_tmp_lsi):
        print(
```
```python
'=== 未检测到有lsi文件夹存在，开始生成lsi向量 ==='
```
```python
)
```
```python
if
```
```python
not
```
```python
dictionary:
            dictionary = corpora.Dictionary.load(path_dictionary)
```
```python
if
```
```python
not
```
```python
corpus_tfidf:
```
```python
# 如果跳过了第二阶段，则从指定位置读取tfidf文档
```
```python
print(
```
```python
'--- 未检测到tfidf文档，开始从磁盘中读取 ---'
```
```python
)
```
```python
# 从对应文件夹中读取所有类别
```
```python
files = os.listdir(path_tmp_tfidf)
            catg_list = []
```
```python
for
```
```python
file
```
```python
in
```
```python
files:
                t = file.split(
```
```python
'.'
```
```python
)[
```
```python
0
```
```python
]
```
```python
if
```
```python
t
```
```python
not
```
```python
in
```
```python
catg_list:
                    catg_list.append(t)
```
```python
# 从磁盘中读取corpus
```
```python
corpus_tfidf = {}
```
```python
for
```
```python
catg
```
```python
in
```
```python
catg_list:
                path =
```
```python
'{f}{s}{c}.mm'
```
```python
.format(f=path_tmp_tfidf,s=os.sep,c=catg)
                corpus = corpora.MmCorpus(path)
                corpus_tfidf[catg] = corpus
            print(
```
```python
'--- tfidf文档读取完毕，开始转化成lsi向量 ---'
```
```python
)
```
```python
# 生成lsi model
```
```python
os.makedirs(path_tmp_lsi)
        corpus_tfidf_total = []
        catgs = list(corpus_tfidf.keys())
```
```python
for
```
```python
catg
```
```python
in
```
```python
catgs:
            tmp = corpus_tfidf.get(catg)
            corpus_tfidf_total += tmp
        lsi_model = models.LsiModel(corpus = corpus_tfidf_total, id2word=dictionary, num_topics=
```
```python
50
```
```python
)
```
```python
# 将lsi模型存储到磁盘上
```
```python
lsi_file = open(path_tmp_lsimodel,
```
```python
'wb'
```
```python
)
        pkl.dump(lsi_model, lsi_file)
        lsi_file.close()
```
```python
del
```
```python
corpus_tfidf_total
```
```python
# lsi model已经生成，释放变量空间
```
```python
print(
```
```python
'--- lsi模型已经生成 ---'
```
```python
)
```
```python
# 生成corpus of lsi, 并逐步去掉 corpus of tfidf
```
```python
corpus_lsi = {}
```
```python
for
```
```python
catg
```
```python
in
```
```python
catgs:
            corpu = [lsi_model[doc]
```
```python
for
```
```python
doc
```
```python
in
```
```python
corpus_tfidf.get(catg)]
            corpus_lsi[catg] = corpu
            corpus_tfidf.pop(catg)
            corpora.MmCorpus.serialize(
```
```python
'{f}{s}{c}.mm'
```
```python
.format(f=path_tmp_lsi,s=os.sep,c=catg),
                                       corpu,
                                       id2word=dictionary)
        print(
```
```python
'=== lsi向量已经生成 ==='
```
```python
)
```
```python
else
```
```python
:
        print(
```
```python
'=== 检测到lsi向量已经生成，跳过该阶段 ==='
```
```python
)
```
```python
# # ===================================================================
```
```python
# # # # 第四阶段，  分类
```
```python
if
```
```python
not
```
```python
os.path.exists(path_tmp_predictor):
        print(
```
```python
'=== 未检测到判断器存在，开始进行分类过程 ==='
```
```python
)
```
```python
if
```
```python
not
```
```python
corpus_lsi:
```
```python
# 如果跳过了第三阶段
```
```python
print(
```
```python
'--- 未检测到lsi文档，开始从磁盘中读取 ---'
```
```python
)
            files = os.listdir(path_tmp_lsi)
            catg_list = []
```
```python
for
```
```python
file
```
```python
in
```
```python
files:
                t = file.split(
```
```python
'.'
```
```python
)[
```
```python
0
```
```python
]
```
```python
if
```
```python
t
```
```python
not
```
```python
in
```
```python
catg_list:
                    catg_list.append(t)
```
```python
# 从磁盘中读取corpus
```
```python
corpus_lsi = {}
```
```python
for
```
```python
catg
```
```python
in
```
```python
catg_list:
                path =
```
```python
'{f}{s}{c}.mm'
```
```python
.format(f=path_tmp_lsi,s=os.sep,c=catg)
                corpus = corpora.MmCorpus(path)
                corpus_lsi[catg] = corpus
            print(
```
```python
'--- lsi文档读取完毕，开始进行分类 ---'
```
```python
)
        tag_list = []
        doc_num_list = []
        corpus_lsi_total = []
        catg_list = []
        files = os.listdir(path_tmp_lsi)
```
```python
for
```
```python
file
```
```python
in
```
```python
files:
            t = file.split(
```
```python
'.'
```
```python
)[
```
```python
0
```
```python
]
```
```python
if
```
```python
t
```
```python
not
```
```python
in
```
```python
catg_list:
                catg_list.append(t)
```
```python
for
```
```python
count,catg
```
```python
in
```
```python
enumerate(catg_list):
            tmp = corpus_lsi[catg]
            tag_list += [count]*tmp.__len__()
            doc_num_list.append(tmp.__len__())
            corpus_lsi_total += tmp
            corpus_lsi.pop(catg)
```
```python
# 将gensim中的mm表示转化成numpy矩阵表示
```
```python
data = []
        rows = []
        cols = []
        line_count =
```
```python
0
```
```python
for
```
```python
line
```
```python
in
```
```python
corpus_lsi_total:
```
```python
for
```
```python
elem
```
```python
in
```
```python
line:
                rows.append(line_count)
                cols.append(elem[
```
```python
0
```
```python
])
                data.append(elem[
```
```python
1
```
```python
])
            line_count +=
```
```python
1
```
```python
lsi_matrix = csr_matrix((data,(rows,cols))).toarray()
```
```python
# 生成训练集和测试集
```
```python
rarray=np.random.random(size=line_count)
        train_set = []
        train_tag = []
        test_set = []
        test_tag = []
```
```python
for
```
```python
i
```
```python
in
```
```python
range(line_count):
```
```python
if
```
```python
rarray[i]<
```
```python
0.8
```
```python
:
                train_set.append(lsi_matrix[i,:])
                train_tag.append(tag_list[i])
```
```python
else
```
```python
:
                test_set.append(lsi_matrix[i,:])
                test_tag.append(tag_list[i])
```
```python
# 生成分类器
```
```python
predictor = svm_classify(train_set,train_tag,test_set,test_tag)
        x = open(path_tmp_predictor,
```
```python
'wb'
```
```python
)
        pkl.dump(predictor, x)
        x.close()
```
```python
else
```
```python
:
        print(
```
```python
'=== 检测到分类器已经生成，跳过该阶段 ==='
```
```python
)
```
```python
# # ===================================================================
```
```python
# # # # 第五阶段，  对新文本进行判断
```
```python
if
```
```python
not
```
```python
dictionary:
        dictionary = corpora.Dictionary.load(path_dictionary)
```
```python
if
```
```python
not
```
```python
lsi_model:
        lsi_file = open(path_tmp_lsimodel,
```
```python
'rb'
```
```python
)
        lsi_model = pkl.load(lsi_file)
        lsi_file.close()
```
```python
if
```
```python
not
```
```python
predictor:
        x = open(path_tmp_predictor,
```
```python
'rb'
```
```python
)
        predictor = pkl.load(x)
        x.close()
    files = os.listdir(path_tmp_lsi)
    catg_list = []
```
```python
for
```
```python
file
```
```python
in
```
```python
files:
        t = file.split(
```
```python
'.'
```
```python
)[
```
```python
0
```
```python
]
```
```python
if
```
```python
t
```
```python
not
```
```python
in
```
```python
catg_list:
            catg_list.append(t)
    demo_doc =
```
```python
"""
这次大选让两党的精英都摸不着头脑。以媒体专家的传统观点来看，要选总统首先要避免失言，避免说出一些“offensive”的话。希拉里，罗姆尼，都是按这个方法操作的。罗姆尼上次的47%言论是在一个私人场合被偷录下来的，不是他有意公开发表的。今年希拉里更是从来没有召开过新闻发布会。
川普这种肆无忌惮的发言方式，在传统观点看来等于自杀。
"""
```
```python
print(
```
```python
"原文本内容为："
```
```python
)
    print(demo_doc)
    demo_doc = list(jieba.cut(demo_doc,cut_all=
```
```python
False
```
```python
))
    demo_bow = dictionary.doc2bow(demo_doc)
    tfidf_model = models.TfidfModel(dictionary=dictionary)
    demo_tfidf = tfidf_model[demo_bow]
    demo_lsi = lsi_model[demo_tfidf]
    data = []
    cols = []
    rows = []
```
```python
for
```
```python
item
```
```python
in
```
```python
demo_lsi:
        data.append(item[
```
```python
1
```
```python
])
        cols.append(item[
```
```python
0
```
```python
])
        rows.append(
```
```python
0
```
```python
)
    demo_matrix = csr_matrix((data,(rows,cols))).toarray()
    x = predictor.predict(demo_matrix)
    print(
```
```python
'分类结果为：{x}'
```
```python
.format(x=catg_list[x[
```
```python
0
```
```python
]]))
```
[
						](https://img-blog.csdn.net/20160912192855155)
[
	](https://img-blog.csdn.net/20160912192855155)
