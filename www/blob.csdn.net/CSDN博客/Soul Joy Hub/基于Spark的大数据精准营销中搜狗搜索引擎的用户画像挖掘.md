# 基于Spark的大数据精准营销中搜狗搜索引擎的用户画像挖掘 - Soul Joy Hub - CSDN博客

2016年12月19日 16:17:06[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：15943
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



转载请注明：转载 from  [http://blog.csdn.net/u011239443/article/details/53735609](http://blog.csdn.net/u011239443/article/details/53735609)

from [CCF举办的“大数据精准营销中搜狗用户画像挖掘”竞赛](http://www.wid.org.cn/data/science/player/competition/detail/description/239)

# 1.  选题背景与意义

## 1.1 用户画像与精准营销

![这里写图片描述](https://img-blog.csdn.net/20161223124402974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

“用户画像”是近几年诞生的名词。很多营销项目或很多广告主，在打算投放广告前，都要求媒体提供其用户画像。在以前，大多媒体会针对自身用户做一个分类，但是有了大数据后，企业及消费者行为带来一系列改变与重塑，通过用户画像可以更加拟人化的描述用户特点。 

用户画像，即用户信息标签化，就是企业通过收集与分析消费者社会属性、生活习惯、消费行为等主要信息的数据之后，完美地抽象出一个用户的商业全貌，可以看作是企业应用大数据技术的基本方式。用户画像为企业提供了足够的信息基础，能够帮助企业快速找到精准用户群体以及用户需求等更为广泛的反馈信息。 

消费方式的改变促使用户迫切希望尽快获取自己想要了解的信息，所以说，基于用户画像上的精准营销不管对企业还是对用户来说，都是有需求的，这会给双方交易带来极大便捷，也为双方平等沟通搭建了一个畅通平台。

## 1.2 搜索引擎下用户画像的挑战

![这里写图片描述](https://img-blog.csdn.net/20161223124422240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在搜索引擎下，由于搜索引擎本身使用方式的特殊性、用户的流动性、查询的实时性等，带来了与企业传统的对用户信息进行收集与分析有着巨大的不同、更加艰巨的挑战。 

例如，我们实时获取到的是用户的查询语句，而由于用户的流动性，并不能直接获取到如年龄、性别、学历等用户的标签信息。这么一来，也就无法根据用户属性对用户进行分群处理，而后再通过推荐系统进行产品上的优化

## 1.3 本文内容概要

本文内容概要如下：
- 第1章：简介用户画像与搜索引擎下用户画像的精准营销的挑战。
- 第2章：说明实验集群、数据与课题研究目标。
- 第3章：介绍使用分词工具对用户的搜索词列进行分词，以及相关的优化方案。
- 第4章：介绍在分词的基础上，对文本进行特征的抽取与转换，以及相关的优化方案。
- 第5章：介绍在原始特征向量上，进行聚类与降维。
- 第6章：介绍实验中试验过各分类模型
- 第7章：介绍模型参数调优
- 第8章：总结本课题研究中不足与展望后续的优化方案
- 第9章：参考文献

# 2. 课题实验准备

## 2.1 Spark集群
|节点|备注|
|----|----|
|cdh01|8核，32G内存，角色：Spark Master，HDFS NameNode，Spark Worker，HDFS DataNode|
|cdh02|8核，12G内存，角色：Spark Worker，HDFS DataNode|
|cdh03|8核，12G内存，角色：Spark Worker，HDFS DataNode|
|cdh04|8核，12G内存，角色：Spark Worker，HDFS DataNode|

## 2.2 数据集
|数据文件|备注|
|----|----|
|Train.csv|带标注的训练集|
|Test.csv|测试集|

## 2.3 数据介绍

本数据来源于[搜狗搜索数据](http://pan.baidu.com/share/link?shareid=3263357388&uk=2449705108)，ID经过加密，训练集中人口属性数据存在部分未知的情况（需要解决方案能够考虑数据缺失对算法性能的影响）。数据所有字段如下表所示：
|字段|说明|
|----|----|
|ID|加密后的ID|
|age|0：未知年龄; 1：0-18岁; 2：19-23岁; 3：24-30岁; 4：31-40岁; 5：41-50岁; 6： 51-999岁|
|Gender|0：未知1：男性2：女性|
|Education|0：未知学历; 1：博士; 2：硕士; 3：大学生; 4：高中; 5：初中; 6：小学|
|Query List|搜索词列表|

## 2.4 数据示例

对于train.csv中的数据记录：

> 
00627779E16E7C09B975B2CE13C088CB     4     2     0     钢琴曲欣赏100首     一个月的宝宝眼睫毛那么是黄色     宝宝右眼有眼屎    小儿抽搐怎么办    剖腹产后刀口上有线头    属羊和属鸡的配吗

## 2.5 课题任务描述

根据提供的用户历史一个月的查询词与用户的人口属性标签（包括性别、年龄、学历）做为训练数据，通过机器学习、数据挖掘技术构建分类算法来对新增用户的人口属性进行判定。

# 3. 查询词分词

## 3.1 NLPIR

![这里写图片描述](https://img-blog.csdn.net/20161223124453303?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

NLPIR汉语分词系统(又名ICTCLAS2013),主要功能包括中文分词；词性标注；命名实体识别；用户词典功能；支持GBK编码、UTF8编码、BIG5编码。新增微博分词、新词发现与关键词提取；张华平博士先后倾力打造十余年，内核升级10次。 

全球用户突破20万，先后获得了2010年钱伟长中文信息处理科学技术奖一等奖，2003年国际SIGHAN分词大赛综合第一名，2002年国内973评测综合第一名。 

我们传入每个用户的搜索词列，表经过NLPIR分词工具得到的分词。之后，我们做个进一步的优化策略：

### 3.1.1 去停用词

我们根据分词后词语所带的词性，对一些特征代表性不够强的词语进行过滤：

```java
for (int i = 0; i < sbtmp.length(); ++i) {
        char cc = sbtmp.charAt(i);
        if (cc == ' ') {
            sbtmp.deleteCharAt(i);
            --i;
        } else if (cc == '/') {

            // 去词条件
            Boolean isdel =
                    // 1. 去标点
                    (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'w')
                            // 2. 疑问词
                            || (i + 2 < sbtmp.length() && sbtmp.charAt(i + 1) == 'r'
                                    && sbtmp.charAt(i + 2) == 'y')
                            // 3. 数字
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'm')
                            // 4. 连词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'c')
                            // 5. 副词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'd')
                            // 6. 叹词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'e')
                            // 7. 拟声词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'o')
                            // 8. 介词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'p')
                            // 9. 量词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'q')
                            // 10. 助词
                            || (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) == 'u')
                            // 11. 纯动词
                            || (i + 2 < sbtmp.length() && sbtmp.charAt(i + 1) == 'v'
                                    && sbtmp.charAt(i + 2) == ' ');

            // 去词
            if (sbtmp.charAt(i + 1) != 'n' && sbtmp.charAt(i + 1) != 'i' && sbtmp.charAt(i + 1) != 'j'
                    && sbtmp.charAt(i + 1) != 'h'
                    && !(i + 2 < sbtmp.length() && sbtmp.charAt(i + 2) == 'n')) {
                while (i + 1 < sbtmp.length() && sbtmp.charAt(i + 1) != ' ') {
                    sbtmp.deleteCharAt(i + 1);
                }
                while (i >= 0 && sbtmp.charAt(i) != ',') {
                    sbtmp.deleteCharAt(i);
                    --i;
                }
            }
            // 若无需去词，把‘/’转为‘，’，并去除随后的词性标志
            else {
                sbtmp.setCharAt(i, ',');
                while (sbtmp.charAt(i + 1) != ' ') {
                    sbtmp.deleteCharAt(i + 1);
                }
            }

        }
    }
    for (int i = 1; i < sbtmp.length() - 1; ++i) {
        if (sbtmp.charAt(i) == ',' && (sbtmp.charAt(i - 1) == ',' || sbtmp.charAt(i + 1) == ',')) {
            sbtmp.deleteCharAt(i);
            --i;
        }
        // 去中间单个字
        else if (sbtmp.charAt(i - 1) == ',' && sbtmp.charAt(i + 1) == ',') {
            sbtmp.deleteCharAt(i);
            sbtmp.deleteCharAt(i);
            --i;
        }
        // 去首个单个字
        else if (sbtmp.charAt(i) == ',' && i == 1) {
            sbtmp.deleteCharAt(i - 1);
            sbtmp.deleteCharAt(i - 1);
            --i;
        }
    }
```

### 3.1.2 提取关键词

分词并不能很好的将常用的短语提取出来，如词语“用户画像”，使用分词工具更倾向于将其分成“用户”和“画像”，而失去了词语本身的含义。NLPIR还提供了提取一段话的关键词的功能，我们可以使用它：

```java
int numofIm = 1000;
String nativeByte = CLibrary.Instance.NLPIR_GetKeyWords(sInput, numofIm, false);
```

经过分词后，平均每位用户搜索词列所得到的词量在600个左右，这里我们设置提取1000个关键词，但实际上一个用户的关键词提取的数量在60～200左右。由于关键词的很强的特征性，并且提取出的数量又少，若后续我们直接使用如词语的词频作为用户的特征属性进行分类的话，很可能各个用户特征属性有巨大的差异，即用户之间拥有的相同关键词过少。

### 3.1.3 混合提取

在用户搜索词列分词基础上，在增加N次对其进行M个关键词提取的结果。

## 3.2 “结巴”分词

jieba，即“结巴”中文分词，一个优秀的开源的分词工具，一直致力于做最好的 Python 中文分词组件。我们直接使用它对用户搜索词列进行1000个关键词的提取，所能提取到的关键词比NLPIR数量有所提高。显然，关键词提取的数量增加，每个关键词的代表性就有所减弱。但在后续的分类实验中证明了，使用该分词方案，对比上节的各个分词方案，在模型相同的情况下，会有2%~5%的准确率的提升。 

关键词抽取可基于以下两种算法，后续实验实践证明基于 TF-IDF 算法的关键词的抽取，在该数据集和我们后续所选择的模型中会得到更好的效果。

### 3.2.1 基于 TF-IDF 算法的关键词抽取

```python
import jieba.analyse
jieba.analyse.extract_tags(sentence, topK=20, withWeight=False, allowPOS=())
```

> - sentence 为待提取的文本
- topK 为返回几个 TF/IDF 权重最大的关键词，默认值为 20
- withWeight 为是否一并返回关键词权重值，默认值为 False
- allowPOS 仅包括指定词性的词，默认值为空，即不筛选
- jieba.analyse.TFIDF(idf_path=None) 新建 TFIDF 实例，idf_path 为 IDF 频率文件

代码示例 （关键词提取）

```python
import sys
sys.path.append('../')

import jieba
import jieba.analyse
from optparse import OptionParser

USAGE = "usage:    python extract_tags.py [file name] -k [top k]"

parser = OptionParser(USAGE)
parser.add_option("-k", dest="topK")
opt, args = parser.parse_args()

if len(args) < 1:
    print(USAGE)
    sys.exit(1)

file_name = args[0]

if opt.topK is None:
    topK = 10
else:
    topK = int(opt.topK)

content = open(file_name, 'rb').read()

tags = jieba.analyse.extract_tags(content, topK=topK)

print(",".join(tags))
```

### 3.2.2 基于 TextRank 算法的关键词抽取

> - jieba.analyse.textrank(sentence, topK=20, withWeight=False, allowPOS=(‘ns’, ‘n’, ‘vn’, ‘v’)) 直接使用，接口相同，注意默认过滤词性。
- jieba.analyse.TextRank() 新建自定义 TextRank 实例
- 基本思想[1]: 
- 将待抽取关键词的文本进行分词
- 以固定窗口大小(默认为5，通过span属性调整)，词之间的共现关系，构建图
- 计算图中节点的PageRank，注意是无向带权图

# 4. 特征抽取与转换

## 4.1 TF-IDF
- TF（Term Frequency） 词频
- DF （Document Frequency） 词语出现的文档数目
- N  总共的文档数目
- IDF （Invert Document Frequency） 逆文档频率：$\large  IDF_k = log_2\frac{N}{DF_k}$
- IDF （Invert Document Frequency） 逆文档频率：$\large IDF_k = log_2(\frac{N}{DF_k} + 0.05)$
- $\large Value_k = TF_k * IDF_k$

IDF反映了一个特征词在整个文档集合中的情况，出现的愈多IDF值越低，这个词区分不同文档的能力越差。 

示例代码：

```
import org.apache.spark.ml.feature.{HashingTF, IDF, Tokenizer}

val sentenceData = spark.createDataFrame(Seq(
  (0, "Hi I heard about Spark"),
  (0, "I wish Java could use case classes"),
  (1, "Logistic regression models are neat")
)).toDF("label", "sentence")

val tokenizer = new Tokenizer().setInputCol("sentence").setOutputCol("words")
val wordsData = tokenizer.transform(sentenceData)
val hashingTF = new HashingTF().setInputCol("words").setOutputCol("rawFeatures").setNumFeatures(20)
val featurizedData = hashingTF.transform(wordsData)

val idf = new IDF().setInputCol("rawFeatures").setOutputCol("features")
val idfModel = idf.fit(featurizedData)
val rescaledData = idfModel.transform(featurizedData)
rescaledData.select("features", "label").take(3).foreach(println)
/*输出结果为：
[(20,[0,5,9,17],[0.6931471805599453,0.6931471805599453,0.28768207245178085,1.3862943611198906]),0]
[(20,[2,7,9,13,15],[0.6931471805599453,0.6931471805599453,0.8630462173553426,0.28768207245178085,0.28768207245178085]),0]
[(20,[4,6,13,15,18],[0.6931471805599453,0.6931471805599453,0.28768207245178085,0.28768207245178085,0.6931471805599453]),1]
*/
```

值得一提的是，Spark所提供的TF并不是数组，而是一个使用 MurmurHash 3 函数的哈希表。其默认向量维度为 2^18=262,144。我们运行上节示例代码可以发现，我们将哈希表大小设置成了20，第二条sentence：”I wish Java could use case classes”有7个不同的单词，经过hash函数却被映射成了只有5个属性为非零值，即有2个位置放了2个不同的单词。这具有很大的随机性，将两个无关词义的词语，甚至词义相反的词语，如“男”与“女”，映射到哈希表的同一位置，作为相同的用户属性来处理。

## 4.2 CountVectorizer

为了解决上节所提到的HashingTF哈希函数映射后导致词语重叠问题，我们使用了Spark的CountVectorizer。我们会先想CountVectorizer传入一个互斥的字符串数组，文本经过CountVectorizer转换后，会对该数组中所有的词语进行与属性的一一对应。 

我们对互斥的字符串数组进行的优化，过滤掉了词频为1的词语，将CountVectorizer的维度减少到原来的50%，极大的降低了后续训练模型时所需的内存，而且除去的数据噪音，增加了预测的准确度：

```
val diffTrain = Triandata.map { line =>
      val temp = line.split("\t")
      if (temp.length == 5) temp(4) else ""
    }
val diffTest = Testdata.map { line =>
      val temp = line.split("\t")
      if (temp.length == 5) temp(1) else ""
    }
val diffAll = diffTrain.union(diffTest).flatMap(_.split(",")).map((_, 1)).reduceByKey(_ + _).collect.filter(line => line._1 != "" && line._2 > 14).map(line => line._1)
```
`val cvm = new CountVectorizerModel(diffAll).setInputCol(tokenizer.getOutputCol).setOutputCol("features")  `
## 4.3 StopWordsRemover

![这里写图片描述](https://img-blog.csdn.net/20161223124547351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在上一章中，我们提到了分词时，根据分词结果所带的词性，对其进行去停用词。而后，我们发现使用”结巴”分词进行TF-IDF算法对用户搜索词列进行1000个关键词的提取对于后续的分类模型效果会更好。但是，我们在“结巴”关键词提取的结果却发现了类似于“什么”“即使”等不具有代表性的词语。于是我们1119个停用词，使用Spark的StopWordsRemover，对分词结果进行去停用词：
`val Stopdata = sc.textFile("hdfs://cdh01:8020//user/data/sogou2/stop",128).collect()``val remover = new StopWordsRemover().setInputCol("words").setOutputCol("filtered").setStopWords(Stopdata)`
## 4.4 权值规范化

![这里写图片描述](https://img-blog.csdn.net/20161223124609149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设想两个不同的用户A和用户B，用户A的搜索词列中只有1句查询语句，分词后得到了3个词语W和总共10个词。而用户B的搜索词列中有10句查询语句，分词后得到了10个词语W和总共100个词。很显然，B中W的TF远高于A中的W的TF，但我们知道词语W在A中比在B中更具有代表性。 

为了解决上述问题，我们使用了最大-最小规范化：

> 
将所有特征向量线性变换到用户指定最大-最小值之间。但注意在计算时还是一个一个特征向量分开计算的。通常将最大，最小值设置为1和0，这样就归一化到[0,1]。Spark中可以对min和max进行设置，默认就是[0,1]。 
$ \large Rescaled(e_i) = \frac{e_i - E_{min}}{E_{max} - E_{min}} * (max - min) + min$
$ \large E_{min},E_{max}$ 是某个特征向量所有元素的最大最小值 

  max,min是用户可以重新自定义的范围，默认为【0,1】，由所有特征共享

在后续，当我们对特征矩阵进行聚类后，得到的特征值可能为负值，可是很多分类器模型需要特征值为非负值。使用以上方法也可以解决这个问题。

## 4.5 同义词替换

![这里写图片描述](https://img-blog.csdn.net/20161223124629274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设想当一个用户的搜索词列的分词结果中出现了一些意思相近的词语，如“恋爱”与“爱情”、“菠萝”与“凤梨”。而我们的模型将其辨别为不同的特征属性，这无疑大量的增加了特征向量的维度和平分了同一意思的词语具有的代表性。 

为了解决上述问题，我们搜集了近4万条同义词词典，将意思相近的词语由1个词语来替换掉。该优化帮助原本的特征向量减少了3万以上的维度，降低了后续训练模型时所需的内存，而且凝聚了属性的代表性，增加了预测的准确度：

```
val sqlContext = new org.apache.spark.sql.SQLContext(sc)
    import sqlContext.implicits._
    val train = sc.textFile("hdfs://cdh01:8020//user/data/sogou2/JBtrain", 400)
    val test = sc.textFile("hdfs://cdh01:8020//user/data/sogou2/JBtest", 400)
    val same = sc.textFile("hdfs://cdh01:8020//user/data/sogou2/same", 400)
    same.filter { x => !x.contains('=') }.count()
    val sameWord = same.map { line =>
      val valuekey = line.split('=')
      (valuekey(1), valuekey(0))
    }.collect()
    val broadcastVar = sc.broadcast(sameWord)
    val diffTrain = train.map { line =>
      val broad = broadcastVar.value
      val regex = """^\d+$""".r
      val temp = line.split("\t")
      val wordArray = temp(4).split(",")
      var str = ""
      for (word <- wordArray) {
        val keyVal = broad.filter(line => line._1.equals(word))
        if (keyVal.length > 0) {
          val oneKeyVal = keyVal(0)
          str = str + "#" + oneKeyVal._2
        } else if (regex.findFirstMatchIn(word) == None) {
          str = str + "#" + word
        }
      }
      (temp(0), temp(1), temp(2), temp(3), str)
    }
    diffTrain.toDF().coalesce(1).write.csv("hdfs://cdh01:8020//user/data/sogou2/ReplaceJBtrain")

    val diffTest = test.map { line =>
      val broad = broadcastVar.value
      val regex = """^\d+$""".r
      val temp = line.split("\t")
      val wordArray = temp(1).split(",")
      var str = ""
      for (word <- wordArray) {
        val keyVal = broad.filter(line => line._1.equals(word))
        if (keyVal.length > 0) {
          val oneKeyVal = keyVal(0)
          str = str + "#" + oneKeyVal._2
        } else if (regex.findFirstMatchIn(word) == None) {
          str = str + "#" + word
        }
      }
      (temp(0), str)
    }
    diffTest.toDF().coalesce(1).write.csv("hdfs://cdh01:8020//user/data/sogou2/ReplaceJBtest")
```

## 4.6 形近词替换

![这里写图片描述](https://img-blog.csdn.net/20161223124644665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设想当一个用户的搜索词列的分词结果中出现了一些形近的词语，如“iPhone5”与“iPhone6”、“杭州”与“杭州站”。该问题和上节所提出的问题类似，为了解决这个问题，我们先来介绍“编辑距离算法”。 

编辑距离算法，是指两个字串之间，由一个转成另一个所需的最少编辑操作次数。许可的编辑操作包括将一个字符替换成另一个字符，插入一个字符，删除一个字符。例如，计算X字符串 与 Y字符串 的 编辑距离。dp[i][j] 为 X串的前i个字符 和 Y串的前j个字符 的编辑距离：

```cpp
if(X [i - 1] == Y[j - 1])       
    dp[i][j] = dp[i - 1][j - 1];   //最后字符相同    
else
{ 
    int t1 = dp[i - 1][j];                             //删除X第i个字符    
    t1 = t1 < dp[i][j - 1] ? t1 : dp[i][j - 1];        //删除Y第j个字符    
    t1 = t1 < dp[i - 1][j - 1] ? t1 : dp[i - 1][j - 1];//最后字符改相同
    dp[i][j] = t1 + 1;
}
```

这里我们所使用的优化方案为： 
![这里写图片描述](https://img-blog.csdn.net/20161223124902542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 对整个训练集和测试集的搜索词列做分词后的词频统计表
- 对每个用户的搜索词列分词后的各个词与词频统计表各词（排除前者自身）进行编辑距离计算。
- 得到词频统计表中编辑距离与该词编辑距离最小词，在这些词中在选择一个词频最高的词将该词替代。

## 4.7 额外增加数据量

![这里写图片描述](https://img-blog.csdn.net/20161223124704235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在大数据时代背景下，只要数据量足够的大，反而我们所选用的不同的算法模型对最终的预测准确率的影响会变小，获取更多数据会使模型更完善更准确。我们这里用不同方案所得到的分词结果，人为的增加训练集的数据。如将10万条记录的训练集进行NLPIR分词得到结果，与进行”结巴”提取关键词得到的结果拼接，就将训练集记录人为的翻倍了。后续的分类实验中证明了，使用该方案，在模型相同的情况下，相比原来会有1%左右的准确率的提升。

# 5.聚类与降维

2009年结束的Nexfix竞赛表明，很多参数团队用到的高等矩阵因子分解对模型提高预测准确略非常有帮助。模型使用矩阵因子分解方法从特征矩阵中抽取一组潜在的属性，并通过这些属性来描述用户。20世纪80年代后期，利用潜在的”语义”属性的思想被成功的应用于信息检索领域。Deerwesteret al. 在1990年提出使用奇异值分解(SVD)方法发现文档中的潜在的属性。[2]而本课题在实验中会使用到LDA方法。

## 5.1 LDA

隐含狄利克雷分配（LDA，Latent Dirichlet Allocation）是一种主题模型(Topic Model，即从所收集的文档中推测主题)。 甚至可以说LDA模型现在已经成为了主题建模中的一个标准，是实践中最成功的主题模型之一。那么何谓“主题”呢？，就是诸如一篇文章、一段话、一个句子所表达的中心思想。不过从统计模型的角度来说， 我们是用一个特定的词频分布来刻画主题的，并认为一篇文章、一段话、一个句子是从一个概率模型中生成的。也就是说 在主题模型中，主题表现为一系列相关的单词，是这些单词的条件概率。形象来说，主题就是一个桶，里面装了出现概率较高的单词(参见下面的图)，这些单词与这个主题有很强的相关性。 
![这里写图片描述](https://img-blog.csdn.net/20161220150122201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

LDA可以用来识别大规模文档集或语料库中潜藏的主题信息。它采用了词袋的方法，这种方法将每一篇文档视为一个词频向量，从而将文本信息转化为了易于建模的数字信息。但是词袋方法没有考虑词与词之间的顺序，这简化了问题的复杂性，同时也为模型的改进提供了契机。每一篇文档代表了一些主题所构成的一个概率分布，而每一个主题又代表了很多单词所构成的一个概率分布。 

LDA可以被认为是如下的一个聚类过程：
- 各个主题（Topics）对应于各类的“质心”，每一篇文档被视为数据集中的一个样本。
- 主题和文档都被认为存在一个向量空间中，这个向量空间中的每个特征向量都是词频（词袋模型）
- 与采用传统聚类方法中采用距离公式来衡量不同的是，LDA使用一个基于统计模型的方程，而这个统计模型揭示出这些文档都是怎么产生的。

### 5.1.1 模型训练

Spark API 参数介绍：
- K：主题数量（或者说聚簇中心数量）
- maxIterations：EM算法的最大迭代次数，设置足够大的迭代次数非常重要，前期的迭代返回一些无用的（极其相似的）话题，但是继续迭代多次后结果明显改善。我们注意到这对EM算法尤其有效。，至少需要设置20次的迭代，50-100次是更合理的设置，取决于数据集。
- docConcentration（Dirichlet分布的参数α)：文档在主题上分布的先验参数（超参数α)。当前必须大于1，值越大，推断出的分布越平滑。默认为-1，自动设置。
- topicConcentration（Dirichlet分布的参数β)：主题在单词上的先验分布参数。当前必须大于1，值越大，推断出的分布越平滑。默认为-1，自动设置。
- checkpointInterval：检查点间隔。maxIterations很大的时候，检查点可以帮助减少shuffle文件大小并且可以帮助故障恢复。

```
val lda=new LDA()
                .setK(20)
                .setOptimizer("online")
                .setCheckpointInterval(10)
                .setMaxIter(100)
    val model=lda.fit(dataset_lpa)
```

### 5.1.2 模型评价

生成的model不仅存储了推断的主题，还包括模型的评价方法。模型的评价指标：logLikelihood，logPerplexity。logLikelihood越大越好，logPerplexity越小越好

```
val ll = model.logLikelihood(dataset_lpa)
    val lp = model.logPerplexity(dataset_lpa)
```

用评价方法，在online 方法下，对setMaxIter进行调参：

```
for(i<-Array(5,10,20,40,60,120,200,500)){
    val lda=new LDA()
                .setK(3)
                .setTopicConcentration(3)
                .setDocConcentration(3)
                .setOptimizer("online")
                .setCheckpointInterval(10)
                .setMaxIter(i)
    val model=lda.fit(dataset_lpa) 

    val ll = model.logLikelihood(dataset_lpa) 
    val lp = model.logPerplexity(dataset_lpa)

    println(s"$i $ll")
    println(s"$i $lp")
 }
```

可以看到，logPerplexity在减小，LogLikelihood在增加，最大迭代次数需要设置50次以上，才能收敛： 
![这里写图片描述](https://img-blog.csdn.net/20161220151759943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161220151814880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 5.1.3 对语料的主题进行聚类

```
val topicsProb=model.transform(dataset_lpa)
    topicsProb.select("label", "topicDistribution")show(false)
    /** 
        +-----+--------------------------------------------------------------+
        |label|topicDistribution                                             |
        +-----+--------------------------------------------------------------+
        |0.0  |[0.523730754859981,0.006564444943344147,0.46970480019667477]  |
        |1.0  |[0.7825074858166653,0.011001204994496623,0.206491309188838]   |
        |2.0  |[0.2085069748527087,0.005698459472719417,0.785794565674572]   |
        ...

    */
```

label是文档序号，文档中各主题的权重，我们可以将该DataFrame带入后续的分类器中，进行训练。

### 5.1.4 其他聚类与降维

Spark在基于RDD的MLlib中还提供了SVD、PCA的降维方法，而基于DataFrame的聚类方法还包括k-means、Bisecting k-means和Gaussian Mixture，其中Gaussian Mixture提供的API类似与LDA，可以直接为我们返回文档中各主题的权重，以便于后续的分类。但是由于LDA在主题聚类上的典型性，我们的课题实验只试验了LDA的方案。

# 6. 分类方案

## 6.1 Cosine相似度

假设现在我们有一个测试集特征向量A和一个训练集的特征向量B：

> 
A：[1, 2, 2, 1, 1, 1, 0] 

  B：[1, 2, 2, 1, 1, 2, 1]

到这里，问题就变成了如何计算这两个向量的相似程度。我们可以把它们想象成空间中的两条线段，都是从原点（[0, 0, …]）出发，指向不同的方向。两条线段之间形成一个夹角，如果夹角为0度，意味着方向相同、线段重合；如果夹角为90度，意味着形成直角，方向完全不相似；如果夹角为180度，意味着方向正好相反。因此，我们可以通过夹角的大小，来判断向量的相似程度。夹角越小，就代表越相似。

![这里写图片描述](https://img-blog.csdn.net/20161221140946486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以二维空间为例，上图的a和b是两个向量，我们要计算它们的夹角θ。余弦定理告诉我们，可以用下面的公式求得： 
$ \large  cos\varTheta = \frac{x_1x_2 + y_1y_2}{\sqrt{x_1^2 + y_1^2} * \sqrt{x_2^2 + y_2^2}}$

拓展到n维向量，假定A和B是两个n维向量，A是 [A1, A2, …, An] ，B是 [B1, B2, …, Bn] ，则A与B的夹角θ的余弦等于： 
$ \large  cos\varTheta = \frac{\varSigma_{i = 1}^n(A_i * B_i)}{\sqrt{\varSigma_{i = 1}^n(A_i)^2} * \sqrt{\varSigma_{i = 1}^n(B_i)^2}} = \frac{A \cdot B}{|A| * |B|}$

使用这个公式，我们就可以得到，特征向量A与特征向量B的夹角的余弦： 
$\large  cos\varTheta = \frac{1 * 1 + 2 * 2 + 2* 2 + 1 * 1 + 1 * 1 + 1 * 2 + 0 * 1}{\sqrt{1^2 + 2^2 + 2^2 + 1^2 + 1^2 + 1^2 +  0^2} * \sqrt{1^2 + 2^2 + 2^2 + 1^2 + 1^2+ 2^2  + 1^2}} = \frac {13} {\sqrt{12} * \sqrt{16}} = 0.938$

余弦值越接近1，就表明夹角越接近0度，也就是两个向量越相似，这就叫”余弦相似度” 

我们这个方案，计算出一条测试集的特征向量与训练集各个特征向量的余弦相似度，将该条测试集的类别标记为与其余弦相似度最大的训练集特征向量所对应的类别。

## 6.2 One-vs-Rest

One-vs-Rest将只能用于二分问题的分类(如Logistic回归、SVM)方法扩展到多类。这种方法基本思想为：

> 
训练时依次把某个类别的样本归为一类,其他剩余的样本归为另一类，这样k个类别的样本就构造出了k个binary分类器。分类时将未知样本分类为具有最大分类函数值的那类。 

  假如我有四类要划分（也就是4个Label），他们是A、B、C、D。于是在抽取训练集的时候，分别抽取 

  （1）A所对应的向量作为正集，B，C，D所对应的向量作为负集 

  （2）B所对应的向量作为正集，A，C，D所对应的向量作为负集; 

  （3）C所对应的向量作为正集，A，B，D所对应的向量作为负集； 

  （4）D所对应的向量作为正集，A，B，C所对应的向量作为负集； 

  使用这四个训练集分别进行训练，然后的得到四个训练结果文件。在测试的时候，把对应的测试向量分别利用这四个训练结果文件进行测试。最后每个测试都有一个结果f1(x),f2(x),f3(x),f4(x)。于是最终的结果便是这四个值中最大的一个作为分类结果。

代码实例：

```
//定义一个binary分类器，如:LogisticRegression 
LogisticRegression lr=new LogisticRegression()
                .setMaxIter(10)
                .setRegParam(0.3)
                .setElasticNetParam(0.2)                
                .setThreshold(0.5);
//建立一对多多分类器model                
OneVsRestModel model=new OneVsRest()
                .setClassifier(lr)//将binary分类器用这种办法加入
                .fit(training);
//利用多分类器model预测
Dataset<Row>predictions=model.transform(test);
```

很遗憾的是，目前Spark基于DataFrame的MLlib binary分类器中并没有实现SVM，而基于RDD的MLlib有实现SVM，却没有实现One-vs-Rest。

## 6.3 朴素贝叶斯

朴素贝叶斯分类是一种思想比较简单的分类算法，朴素贝叶斯的思想基础是这样的：对于给出的待分类项，求解在此项出现的条件下各个类别出现的概率，哪个最大，就认为此待分类项属于哪个类别。 

朴素贝叶斯分类的正式定义如下：

> 
1、设为一个待分类测试特征向量$\Large x = {a_1,a_2,...,a_m}$，而每个$ \Large a_i$为属性$ \Large x_i$的值。

2、有类别集合$ \Large C = {y_1,y_2,...,y_n}$。

3、计算 $ \Large P(y_1|x),P(y_2|x)...P(y_n|x)$。

4、将该待分类的用户类别标记为$ \Large y_k$。

$\Large P(y_k|x) = max(P(y_1|x),P(y_2|x)...P(y_n|x))   ,   k\in(1..n)$

那么现在的关键就是如何计算第3步中的各个条件概率。我们假设各个特征属性是条件独立的，则根据贝叶斯定理有如下推导：

$ \Large P(y_i|x) = \frac{P(x|y_i)P(y_i)}{P(x)} $

因为分母对于所有类别为常数，因为我们只要将分子最大化皆可。又因为各特征属性是条件独立的，所以有：

$ \Large P(x|y_i)P(y_i) = P(a_1|y_i)P(a_2|y_i)...P(a_m|p_i)P(y_i)$

而等式右边的各项都可以有训练集求出。以此类推，便可以计算出计算第3步。 

 Spark Mllib实现朴素贝叶斯分类器，Smoothing为平滑系数：
`val lr = new NaiveBayes().setSmoothing(0.1).setFeaturesCol("features")`
## 6.4 前馈神经网络

Spark MLlib中实现了MultilayerPerceptronClassifier（MLPC），这是一个基于前馈神经网络的分类器，它是一种在输入层与输出层之间含有一层或多层隐含结点的具有正向传播机制的神经网络模型。 

![这里写图片描述](https://img-blog.csdn.net/20161221223528510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

中间的节点使用sigmoid （logistic）函数，输出层的节点使用softmax函数。输出层的节点的数目表示分类器有几类。MLPC学习过程中使用BP算法，优化问题抽象成logistic loss function并使用L-BFGS进行优化。 

下面我们来介绍下MultilayerPerceptronClassifier所设计到的参数：
`val lr = new MultilayerPerceptronClassifier().setMaxIter(51).setLayers(Array[Int](vector_len, 6, 5, classfiy_num)).setSeed(1234L).setFeaturesCol("features").setLabelCol("label").setPredictionCol("prediction")`- layers：各层的节点数，第1层的个数必须为特征向量的维度数，最后1层的个数必须为类别数，中间为各隐藏层的节点数，可以任意设置。对于隐藏成节点数的优化，我们做了一下的方案：由于我们的年龄、性别、学历的类别数分别是6、2、6。假设我们在预测年龄时，我们可以将隐藏的第1层节点数设为6 * 2 * 6 = 72,即根据先将其分为3个label都互斥的类别,。然后，将隐藏的第2层节点数设为6 * 2  = 12,即根据先将其分为年龄和性别3个label都互斥的类别。最后，再分到我们想预测的年龄的6个类别上。
- maxIter：最大迭代次数。进行参数调优时，主要是在优化这个参数。
- Seed：随机种子
- tol：（代码中未设置）允许误差。一般取0.001~0.00001，当迭代结果的误差小于该值时，结束迭代计算，给出结果。
- 
solver：（代码中未设置）优化器。有两种算法可供选择： l-bfgs和gd。默认为 l-bfgs算法，因为gd算法比l-bfgs算法需要多得多的迭代次数，即使在提高学习步长和提高允许误差tol的情况下，还是慢很多：

> 
.stepSize=0.03,tol=0.0001 

  l-bfgs:上很快能收敛，大约20次，训练速度也更快 

  maxIter = 5 accuracy = 0.35 training time = 267ms 

  maxIter = 10 accuracy = 0.74 training time = 429ms 

  maxIter = 20 accuracy = 0.91 training time = 657ms 

  maxIter = 50 accuracy = 0.92 training time = 941ms 

  maxIter = 100 accuracy = 0.92 training time = 914ms 

  maxIter = 500 accuracy = 0.92 training time = 1052ms

![这里写图片描述](https://img-blog.csdn.net/20161221225121612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

gd算法：基本上要比l-bfgs算法慢10以上 

  stepsize=0.2,tol=0.001 

  maxIter = 100 accuracy = 0.55 training time = 4209ms 

  maxIter = 500 accuracy = 0.92 training time = 11216ms 

  maxIter = 1000 accuracy = 0.92 training time = 14540ms 

  maxIter = 2000 accuracy = 0.92 training time = 14708ms 

  maxIter = 5000 accuracy = 0.92 training time = 14669ms 

- 
stepSize：学习步长。一般来说学习步长越大，权重变化越大，收敛越快；但训练速率过大，会引起系统的振荡。太高的学习步长,可以减少网络训练的时间,但是容易导致网络的不稳定与训练误差的增加，会引起系统的振荡。 

## 6.5 预分类迭代预测

![这里写图片描述](https://img-blog.csdn.net/20161223125032018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

数据表明，当我们在进行对年龄进行分类时，其实同一年龄段的男性和女性查询语句是存在较大的差异的。我们可以借鉴6.4 前馈神经网络中对layers（各层的节点数）优化的类似的思想： 

1. 将测试集进行性别男女的预测分类，预测成男性的分为测试集test1，预测成女性的分为测试集test2。 

2. 将训练集根据性别划分为train1（男性）和train2（女性）。 

3. 分别用train1训练模型来预测test1的年龄，train2训练模型来预测test2年龄。 

上诉思想可以实现一种迭代，即继续对年龄的预测结果进行划分来预测学历，再对学历的预测结果进行划分来预测性别，在进行上诉的第2、第3步，如此反复继续。 

这个方案由于将数据集划分了，每次降低了数据量，这会对准确率有负面的影响，但最终总体预测准确率提高了0.3%左右。

## 6.6 Boosting

Boosting分类器属于集成学习模型，它基本思想是把成百上千个分类准确率较低的树模型组合起来，成为一个准确率很高的模型。这个模型会不断地迭代，每次迭代就生成一颗新的树。

### 6.6.1  xgboost

xgboost 的全称是eXtreme Gradient Boosting。它是Gradient Boosting Machine的一个c++实现，作者为华盛顿大学研究机器学习专家陈天奇 。他在研究中深感自己受制于现有库的计算速度和精度，因此开始着手搭建xgboost项目。xgboost最大的特点在于，它能够自动利用CPU的多线程进行并行，同时在算法上加以改进提高了精度。在Kaggle的希格斯子信号识别竞赛中，它因为出众的效率与较高的预测准确度在比赛论坛中引起了参赛选手的 广泛关注 ，在1700多支队伍的激烈竞争中占有一席之地。  

 xgboost拥有自身的jvm项目包，可以和Spark集成。很遗憾的是，由于时间关系，我们并没有成功尝试这个方案，而是在已有分类的结果上实现了简单的加权投票的策略，最终总体预测准确率提高了0.4%左右。

# 7. 参数调优

## 7.1 交叉验证法

Spark Mllib 中实现的是留一法交叉验证法。留一法交叉验证法的思想是：将原来的训练集有N个数据集，将每一个数据集作为测试集，其它N-1个数据集作为训练集。这样得到N个分类器，N个测试结果。用这N个结果的平均值来衡量模型的性能：

```
val pipeline = new Pipeline()
  .setStages(Array(tokenizer, hashingTF, lr))

val paramGrid = new ParamGridBuilder()
  .addGrid(hashingTF.numFeatures, Array(10, 100, 1000))
  .addGrid(lr.regParam, Array(0.1, 0.01))
  .build()

val cv = new CrossValidator()
  .setEstimator(pipeline)
  .setEvaluator(new BinaryClassificationEvaluator)
  .setEstimatorParamMaps(paramGrid)
  .setNumFolds(2)  

val cvModel = cv.fit(training)

......

cvModel.transform(test)
```

参数介绍：
- Estimator：即所要进行评估的模型
- Evaluator：对模型评估器，可以是二分类的BinaryClassificationEvaluator 或是 多分类的  MulticlassClassificationEvaluator
- EstimatorParamMaps：模型参数表。可以由ParamGridBuilder调用addGrid方法对模型的某个参数设置一组需要验证的值，再调用build()返回。
- NumFolds：即所要划分的数据集的数量N

## 7.2 划分训练集验证法

划分训练集验证法的思想比较简单，我们将训练集按 m：1 - m 的比例划分成两个部分，第1部分作为新的训练集，第2部分作为验证集：

```
val trainValidationSplit = new TrainValidationSplit()
  .setEstimator(lr)
  .setEvaluator(new RegressionEvaluator)
  .setEstimatorParamMaps(paramGrid)
  .setTrainRatio(0.8)
```

我们可以从上述代码看到参数基本上和交叉验证法相同，其中TrainRatio就是我们说的m。 

划分训练集验证法优点在于所需要的时间较短。当我们在对前馈神经网络参数调优时，因为耗时过长而无法选用交叉验证法，而划分训练集验证法则是一种很好的替代方案。 

很遗憾的是，Spark Mllib所实现的交叉验证法和划分训练集验证法都没有返回验证所选得的一组最优参数的API，而是将其视为一种模型直接对原始训练集进行训练，最后返回预测结果。而且，划分训练集验证法只对训练集划分一次进行预测，这具有很大的偶然性。由于以上的原因，我们动手自己实现了划分训练集验证法，并每次验证进行了三次的随机划分和训练，以其平均值作为验证的结果，最后按准确率对参数组降序排序。：

```
val evaluations =
      for (
        smooth <- 1 to 100
      ) yield {
        val lr = new NaiveBayes().setSmoothing(smooth.toDouble / 100.0).setFeaturesCol("features")
        val pipeline = new Pipeline().setStages(Array(lr))
        var sumA = 0.0
        for (cnt <- 1 to 3) {
          val Array(trainData, testData) = dataIDF.randomSplit(Array(0.7, 0.3))
          trainData.cache()
          testData.cache()
          val model = pipeline.fit(trainData)
          val predictions = model.transform(testData)
          val evaluator = new MulticlassClassificationEvaluator().setLabelCol("label").setPredictionCol("prediction").setMetricName("accuracy")
          val accuracy = evaluator.evaluate(predictions)
          sumA = sumA + accuracy
        }
        val allAccuracy = sumA / 3.0
        println(((smooth), allAccuracy))
        ((smooth), allAccuracy)
      }
    evaluations.sortBy(_._2).reverse.foreach(println)
```

使用上述代码对贝叶斯分类器关于各个label预测的参数（平滑度）调优，总体平均准确率相比原来有6%左右的提升。

# 8. 回顾与展望

## 8.1 总结

经过我们对各个分词、特征提取与转换、聚类、分类以及参数调优的方案的尝试，最终我们选择了：
- 分词：NLPIR分词、结巴TF-IDF与结巴TextRank叠加，将训练集数据量变为原来的3倍。
- 特征提取与转换：选择了HashingTF，维度设置为10万。
- 聚类：弃用。
- 分类：使用前馈神经网络，对每个Label预测，设置参数都为：

```
MultilayerPerceptronClassifier().setMaxIter(32).setLayers(Array[Int](vector_len, 6, 5, classfiy_num))
```
- 参数调优：将HashingTF维度降低为1万，使用划分训练集验证法进行调优。

该方案所能达到的平均准确率为69.652%，再对其他方案与该方案进行简单的加权投票，平均准确率可达70%左右。

## 8.2 不足与展望

从上节可以发现，很遗憾的我们在最终的方案中做了很多原本优化上的妥协：
- 分词：从最终分词结果的数据上表明，分词结果还存在非常大可以改进的地方，还存在许多代表性极弱的词语。
- 特征提取与转换：由于模型的选择，试验证明使用IDF和权值规范化，并不能得到预期的效果。反而，只使用CountVectorizer会得到较好的效果。而在选用前馈神经网络分类器后，Spark机器无法支撑CountVectorizer如此大的维度（按结巴TF-IDF分词结果，使用CountVectorizer会得到维度为60多万的特征向量，而去词频为1的词语后得到的特征向量维度为30多万），因而我们退而求其次选用了HashingTF。
- 聚类：实验中的Spark集群使用LDA，所能承受特征维度在30万以下，影响了LDA的效果。而训练一次（迭代100次）所花费的时间，我们实在无法承受，再而我们对参数的设置（如主题个数、迭代次数）并没太大的经验。在多次试验，得到后续的分类效果并不良好的情况下，我们放弃了这个方案。
- 分类与参数调优：当我们弃用朴素贝叶斯分类器，转而试用并未进行参数调优的前馈神经网络分类器后，惊喜的发现后者比前者参数调优过的平均预测准确率还要高出4%左右。但很遗憾的是，就算我们将特征向量维度降低至10万，对前馈神经网络分类器的参数调优所耗费的时间还是远远的超出我们所能承受的。于是，我们尝试将维度降低至1万，调优得到参数组后，再带回到10万维度的特征向量；尝试减少数据量进行参数调优。而结果证明，这两种方案的调优都不能与使用原始的特征向量调优效果一致。
- xgboost：由于时间关系，我们并没有成功尝试这个方案，而是在已有分类的结果上实现了简单的加权投票的策略。

由于我们的经验不足，再加上实验室的项目任务、在校课程的学习任务以及后期转而对Spark Core源码的研究，使得原本可以做的优化并没有精力去很好的实现。希望在后续能将其完善的更好。

## 8.3 感谢

感谢实验室所提供的服务器集群环境，感谢同组同学在一起交流讨论中激发出灵感。非常感谢这次课题实验给我带来的学习机会，让我从头到尾自主的完成了一次数据处理、分析的过程，也深深的感受到了Spark的魅力和大数据处理的重要性，也坚定了我从事Spark大数据处理与分析研究的决心。

# 9. 参考文献

【1】Mihalcea R, Tarau P. TextRank: Bringing order into texts[C]. Association for Computational Linguistics, 2004. 

【2】Dietmar J, Markus Z. Recommender systems: An introduction[M]. Cambridge University Press, 2010: 35．

![这里写图片描述](https://img-blog.csdn.net/20170612225709881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

