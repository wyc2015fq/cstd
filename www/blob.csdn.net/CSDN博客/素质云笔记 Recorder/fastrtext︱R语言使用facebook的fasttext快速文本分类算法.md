# fastrtext︱R语言使用facebook的fasttext快速文本分类算法 - 素质云笔记/Recorder... - CSDN博客





2017年10月27日 18:49:43[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2034








> 
FastText是Facebook开发的一款快速文本分类器，提供简单而高效的文本分类和表征学习的方法，不过这个项目其实是有两部分组成的。理论介绍可见博客：[NLP︱高级词向量表达（二）——FastText（简述、学习笔记）](http://blog.csdn.net/sinat_26917383/article/details/54850933)

本轮新更新的fastrtext，同样继承了两个功能：**训练词向量 + 文本分类模型训练**


#### 来源：

[https://github.com/pommedeterresautee/fastrtext](https://github.com/pommedeterresautee/fastrtext)

#### 相关文档地址：

[https://pommedeterresautee.github.io/fastrtext/index.html](https://pommedeterresautee.github.io/fastrtext/index.html)

#### 相关博客：
- [重磅︱文本挖掘深度学习之word2vec的R语言实现](http://blog.csdn.net/sinat_26917383/article/details/51319312)
- [重磅︱R+NLP：text2vec包——New 文本分析生态系统 No.1（一,简介）](http://blog.csdn.net/sinat_26917383/article/details/53161863)

其中text2vec包中有词向量Glove的运算。

.

# 一、安装

## 1.安装

```
# From Cran
install.packages("fastrtext")

# From Github
# install.packages("devtools")
devtools::install_github("pommedeterresautee/fastrtext")
```

.

## 2.主函数介绍

```
The following arguments are mandatory:
  -input              training file path
  -output             output file path

  The following arguments are optional:
  -verbose            verbosity level [2]

  The following arguments for the dictionary are optional:
  -minCount           minimal number of word occurences [5]
  -minCountLabel      minimal number of label occurences [0]
  -wordNgrams         max length of word ngram [1]
  -bucket             number of buckets [2000000]
  -minn               min length of char ngram [3]
  -maxn               max length of char ngram [6]
  -t                  sampling threshold [0.0001]
  -label              labels prefix [__label__]

  The following arguments for training are optional:
  -lr                 learning rate [0.05]
  -lrUpdateRate       change the rate of updates for the learning rate [100]
  -dim                size of word vectors [100]
  -ws                 size of the context window [5]
  -epoch              number of epochs [5]
  -neg                number of negatives sampled [5]
  -loss               loss function {ns, hs, softmax} [ns]
  -thread             number of threads [12]
  -pretrainedVectors  pretrained word vectors for supervised learning []
  -saveOutput         whether output params should be saved [0]

  The following arguments for quantization are optional:
  -cutoff             number of words and ngrams to retain [0]
  -retrain            finetune embeddings if a cutoff is applied [0]
  -qnorm              quantizing the norm separately [0]
  -qout               quantizing the classifier [0]
  -dsub               size of each sub-vector [2]
```

也就是execute（）时候，可以输入的函数是啥。 

-dim，向量长度，默认100维； 

-wordNgrams，词类型，一般可以选择2，二元组 

-verbose，输出信息的详细程度，0-2，不同层次的详细程度（0代表啥也不显示）。 

-lr：学习速率[0.1] 

-lrUpdateRate：更改学习率的更新速率[100] 

-dim ：字向量大小[100] 

-ws：上下文窗口的大小[5] 

-epoch：循环数[5] 

-neg：抽样数量[5] 

-loss：损失函数 {ns，hs，softmax} [ns] 

-thread：线程数[12] 

-pretrainedVectors：用于监督学习的预培训字向量 

-saveOutput：输出参数是否应该保存[0] 

.
# 二、官方案例一 —— 文本分类模型训练

## 2.1 加载数据并训练

```
library(fastrtext)

data("train_sentences")
data("test_sentences")

# prepare data
tmp_file_model <- tempfile()

train_labels <- paste0("__label__", train_sentences[,"class.text"])
train_texts <- tolower(train_sentences[,"text"])
train_to_write <- paste(train_labels, train_texts)
train_tmp_file_txt <- tempfile()
writeLines(text = train_to_write, con = train_tmp_file_txt)

test_labels <- paste0("__label__", test_sentences[,"class.text"])
test_texts <- tolower(test_sentences[,"text"])
test_to_write <- paste(test_labels, test_texts)

# learn model
execute(commands = c("supervised", "-input", train_tmp_file_txt, "-output", tmp_file_model, "-dim", 20, "-lr", 1, "-epoch", 20, "-wordNgrams", 2, "-verbose", 1))
```

其中可以看到与之前熟知的机器学习相关模型不同，其模型运行是通过execute来得到，并保存。 

其中：

来看看输入数据长啥样子： 
![这里写图片描述](https://img-blog.csdn.net/20171027181621400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

数据是char格式的，之前__label__XXX 是该文本的标签，然后空格接上文本内容。
运行结果：

```
## 
Read 0M words
## Number of words:  5060
## Number of labels: 15
## 
Progress: 100.0%  words/sec/thread: 1457520  lr: 0.000000  loss: 0.300770  eta: 0h0m
```

.

## 2.2 验证集+运行模型

```
# load model
model <- load_model(tmp_file_model)
# prediction are returned as a list with words and probabilities
predictions <- predict(model, sentences = test_to_write)
```

load_model模型文件位置，test_to_write是验证文本，长这样（其实跟训练集长一样）： 
![这里写图片描述](https://img-blog.csdn.net/20171027182446641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
显示：

```
print(head(predictions, 5))
## [[1]]
## __label__OWNX 
##     0.9980469 
## 
## [[2]]
## __label__MISC 
##     0.9863281 
## 
## [[3]]
## __label__MISC 
##     0.9921875 
## 
## [[4]]
## __label__OWNX 
##     0.9082031 
## 
## [[5]]
## __label__AIMX 
##      0.984375
```

.

## 2.3 模型验证

计算准确率

```
# Compute accuracy
mean(sapply(predictions, names) == test_labels)
```

计算海明距离

```php
# because there is only one category by observation, hamming loss will be the same
get_hamming_loss(as.list(test_labels), predictions)
## [1] 0.8316667
```

.

## 2.4 一些小函数

查看监督模型的label有哪些，get_labels函数。 

如果已经训练好模型，放了一段时间，又不知道里面有哪些标签，可以这么找一下。

```
model <- load_model(model_test_path)
print(head(get_labels(model), 5))
#> [1] "__label__MISC" "__label__OWNX" "__label__AIMX" "__label__CONT"
#> [5] "__label__BASE"
```

查看模型的参数都用了啥get_parameters：

```
model <- load_model(model_test_path)
print(head(get_parameters(model), 5))
#> $learning_rate
#> [1] 0.05
#> 
#> $learning_rate_update
#> [1] 100
#> 
#> $dim
#> [1] 20
#> 
#> $context_window_size
#> [1] 5
#> 
#> $epoch
#> [1] 20
#>
```

.

# 三、官方案例二 —— 计算词向量

## 3.1 加载数据 + 训练

```
library(fastrtext)

    data("train_sentences")
    data("test_sentences")
    texts <- tolower(train_sentences[,"text"])
    tmp_file_txt <- tempfile()
    tmp_file_model <- tempfile()
    writeLines(text = texts, con = tmp_file_txt)
    execute(commands = c("skipgram", "-input", tmp_file_txt, "-output", tmp_file_model, "-verbose", 1))
```

commands 里面的参数是：”skipgram”，也就是计算词向量，跟word2vec一致。 

输入的文本内容，不用带标签信息：

![这里写图片描述](https://img-blog.csdn.net/20171027183455679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 3.2 词向量
` model <- load_model(tmp_file_model)`
加载词向量的文件，加载的是bin文件

```
# test word extraction
    dict <- get_dictionary(model)
    print(head(dict, 5))
## [1] "the"  "</s>" "of"   "to"   "and"
```

dict 就是词向量的字典，

```
# print vector
  print(get_word_vectors(model, c("time", "timing")))
```

显示一下，词向量的维度。 
![这里写图片描述](https://img-blog.csdn.net/20171027183612969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 3.3 计算词向量距离——get_word_distance

```
# test word distance
  get_word_distance(model, "time", "timing")
##            [,1]
## [1,] 0.02767485
```

.

## 3.4 找出最近邻词——get_nn

get_nn参数只有三个，最后数字代表选择前多少个近义词。

```
library(fastrtext)
model_test_path <- system.file("extdata", "model_unsupervised_test.bin", package = "fastrtext")
model <- load_model(model_test_path)
get_nn(model, "time", 10)
#>      times       size   indicate     access    success   allowing   feelings 
#>  0.6120564  0.5041215  0.4941387  0.4777856  0.4719051  0.4696053  0.4652924 
#>   dictator      amino accuracies 
#>  0.4595046  0.4582702  0.4535145
```

.

## 3.5 词的类比——get_analogies

```
library(fastrtext)
model_test_path <- system.file("extdata", "model_unsupervised_test.bin", package = "fastrtext")
model <- load_model(model_test_path)
get_analogies(model, "experience", "experiences", "result")
#>  results 
#> 0.726607
```

类比关系式： 

get_analogies(model, w1, w2, w3, k = 1) 

w1 - w2 + w3 

也即是： 

experience  - experiences + result
![这里写图片描述](https://img-blog.csdn.net/20171028111931930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








