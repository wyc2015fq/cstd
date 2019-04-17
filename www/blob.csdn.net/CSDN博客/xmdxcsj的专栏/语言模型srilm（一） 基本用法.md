# 语言模型srilm（一） 基本用法 - xmdxcsj的专栏 - CSDN博客





2015年12月18日 18:30:01[xmucas](https://me.csdn.net/xmdxcsj)阅读数：9946








## 一、基本训练

### 基本参数

```
##功能
#读取分词后的text文件或者count文件，然后用来输出最后汇总的count文件或者语言模型
##参数
#输入文本：
#  -read 读取count文件
#  -text 读取分词后的文本文件
#词典文件：
#  -vocab 限制text和count文件的单词，没有出现在词典的单词替换为<unk>；如果没有，所有的单词将会被自动加入词典
#  -limit-vocab 只限制count文件的单词（对text文件无效），没有出现在词典里面的count将会被丢弃
#  -write-vocab 输出词典
#语言模型：
#  -lm 输出语言模型
#  -write-binary-lm 输出二进制的语言模型
#  -sort 输出语言模型gram排序
```

有两种训练方法，分别如下：

```
#choice1: text->count->lm
#ngram-count -text $text -vocab ${vocab} -order 2 -sort -tolower -lm ${arpa}

#choice2: text->count count->lm
#ngram-count -text ${text} -order 2 -sort -tolower -write ${count}
```

## 二、语言模型打分

```
##功能
#用于评估语言模型的好坏，或者是计算特定句子的得分，用于语音识别的识别结果分析。
##参数
#计算得分：
#  -order 模型阶数，默认使用3阶
#  -lm 使用的语言模型
#  -use-server S 启动语言模型服务，S的形式为port@hostname
#  -ppl 后跟需要打分的句子（一行一句，已经分词），ppl表示所有单词，ppl1表示除了</s>以外的单词
#    -debug 0 只输出整体情况
#    -debug 1 具体到句子
#    -debug 2 具体每个词的概率
#产生句子：
#  -gen 产生句子的个数
#  -seed 产生句子用到的random seed
ngram -lm ${lm} -order 2 -ppl ${file} -debug 1 > ${ppl}
```

## 三、语言模型剪枝

```
##功能
#用于减小语言模型的大小，剪枝原理参考(http://blog.csdn.net/xmdxcsj/article/details/50321613)
##参数
#模型裁剪：
#  -prune threshold 删除一些ngram，满足删除以后模型的ppl增加值小于threshold，越大剪枝剪得越狠
#  -write-lm 新的语言模型
ngram -lm ${oldlm} -order 2 -prune ${thres} -write-lm ${newlm}
```

## 四、语言模型合并

```
##功能
#用于多个语言模型之间插值合并，以期望改善模型的效果
##参数
#模型插值：
#  -mix-lm 用于插值的第二个ngram模型，-lm是第一个ngram模型
#  -lambda 主模型（-lm对应模型）的插值比例，0~1，默认是0.5
#  -mix-lm2 用于插值的第三个模型
#  -mix-lambda2 用于插值的第二个模型（-mix-lm对应的模型）的比例，那么第二个模型的比例为1-lambda-mix-lambda2
#  -vocab 当两个模型的词典不一样的时候，使用该参数限制词典列表，没有效果
#  -limit-vocab 当两个模型的词典不一样的时候，使用该参数限制词典列表，没有效果
ngram -lm ${mainlm} -order 2 -mix-lm ${mixlm} -lambda 0.8 -write-lm ${mergelm}
```

在合并语言模型之前，可以使用脚本计算出最好的比例，参考srilm的compute-best-mix脚本

## 五、语言模型使用词典限制

有两种方法可以根据给定的词典对模型加以限制 

一种是在训练的时候使用-vocab限制 

另外一种是在训练完成以后，使用srilm的脚本，如下：
```
##功能
#对已有的语言模型，使用新的字典进行约束，产生新的语言模型
#1.n-grams的概率保持不变
#2.回退概率重新计算
#3.增加新的一元回退概率
##参数
#模型裁剪：
#  -vocab 词典单词的列表，不包括发音
#  -write-lm 新的语言模型
change-lm-vocab -vocab ${vocab} -lm ${oldlm} -write-lm ${newlm} -order 2
```




