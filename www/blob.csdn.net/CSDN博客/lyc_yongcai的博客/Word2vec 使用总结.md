
# Word2vec 使用总结 - lyc_yongcai的博客 - CSDN博客


2017年06月15日 10:32:20[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：1415


word2vec是google 推出的做词嵌入（word embedding）的开源工具。 简单的说，它在给定的语料库上训练一个模型，然后会输出所有出现在语料库上的单词的向量表示，这个向量称为"word embedding"。基于这个向量表示，可以计算词与词之间的关系，例如相似性(同义词等)，语义关联性（中国 - 北京 = 英国 - 伦敦）等。NLP中传统的词表示方法是 one-hot representation， 即把每个单词表示成dim维的稀疏向量，dim等于词汇量的大小。这个稀疏向量只有一个维度（该单词的index）上是1，其余全是0。这个表示方法使得单词之间是孤立的。
 word embedding则是把单词的表示降维到n维的稠密向量，n<<dim。
本文是对word2vec工具使用过程的整理与总结，方便大家尽快上手。本文分别对英文和中文两种语料处理为例。
1、word2vec 源码
Google Code 提供的 word2vec 可以通过 SVN 下载,
地址为:http://word2vec.googlecode.com/svn/trunk/
2、环境
编译和运行都需要在 linux 系统中进行; 我的系统是Ubuntu16.04。
3、编译
(1)cd 进入 word2vec 的代码目录;
(2)直接输入: make,然后回车即开始编译,完成后目录中多出几个没有后缀的文件, 即为编译后的程序;
4、下载训练数据（英文语料）
text8:下载地址:http://mattmahoney.net/dc/text8.zip
直接解压到当前文件夹,text8 数据文件与程序在同一目录。
5、训练
训练 wordvector 一般要指定向量的维度,可以使用默认值:200
编辑 demo-word.sh
原文件中的脚本如下:
make
if [ ! -e text8 ]; then
wget http://mattmahoney.net/dc/text8.zip -O text8.gz
gzip -d text8.gz -f
fi
time ./word2vec -train text8 -output vectors.txt -cbow 0 -size 200 -window 5 -negative 0 -hs
1 -sample 1e-3 -threads 12 -binary 0 -iter 20

保存后,就可以在命令行下运行了:>sh demo-word.sh
等待训练完成，就可以的到训练好的词向量 vectors.txt 。

5、参数介绍
-train text8  表示在指定的语料库 text8 上训练模型
-output vectors.txt表示训练得到词向量的结果
-cbow 0   表示使用skip-gram模型，设置 1 表示使用 CBOW 模型
-size 200   表示训练词向量的维度
-window 5   表示训练窗口大小为5，即考虑一个单词的前5个单词和后5个单词
-negative 0 -hs 1  表示是使用negative sample 还是使用HS 算法，在此使用的 HS 算法
-sample 1e-3   采用的阀值
-threads 12   线程数
-binary 0  表示输出为文本格式，设置为 1 输出保存为二进制形式
-iter20   表示迭代次数

6、训练中文语料
我用的是商品点评的语料，处理了一份中文wiki语料库，已经完成格式化、繁体转简体和分词的过程，下载页面： http://pan.baidu.com/s/1jHZCvvo       文件名为：wiki_chinese_preprocessed.simplied.txt
对于中文语料，第一步需要分词。现成的工具很多，我使用的 jieba 分词。感觉效果还挺不错的。
分完词后，把语料库整理成Word2vec的输入格式。这个格式很简单，单词之间用空格隔开就行了。 word2vec 把一个单词的前面和后面的k个单词作为context训练， 其中会自动把换行符替换成 </s> ，也就是句子分隔符。
编辑 demo-word.sh
time ./word2vec -trainwiki_chinese_preprocessed.simplied.txt-output wiki_vectors.txt -cbow 0 -size 200 -window 5 -negative 0 -hs 1 -sample 1e-3 -threads 12 -binary
 0 -iter 20
保存后,就可以在命令行下运行了:>sh demo-word.sh
漫长的训练已经开始了。。。。1G的语料大概需要两个小时能训练完吧！！！
。。。。
。。。。
训练完成后的到  wiki_vectors.txt  词向量文件



