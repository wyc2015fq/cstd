# Sparkml库标签和索引之间转化 - Spark高级玩法 - CSDN博客
2017年12月20日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1043
**StringIndexer**
StringIndexer将一串字符串标签编码为一列标签索引。这些索引范围是[0, numLabels)按照标签频率排序，因此最频繁的标签获得索引0。如果用户选择保留它们，那么看不见的标签将被放在索引numLabels处。如果输入列是数字，我们将其转换为字符串值并将为其建索引。当下游管道组件（例如Estimator或 Transformer使用此字符串索引标签）时，必须将组件的输入列设置为此字符串索引列名称。在许多情况下，您可以使用设置输入列setInputCol。
例1， 假如我们有下面的DataFrame，带有id和category列：
|Id|category|
|----|----|
|0|a|
|1|b|
|2|c|
|3|a|
|4|a|
|5|c|
对着个Dataframe使用StringIndexer，输入列式category，categoryIndex作为输出列，得到如下值：|Id|Category|CategoryIndex|
|----|----|----|
|0|a|0.0|
|1|b|2.0|
|2|c|1.0|
|3|a|0.0|
|4|a|0.0|
|5|c|1.0|
字符a，索引值是0，原因是a出现的频率最高，接着就是c：1，b：2。
另外，对于不可见的标签，StringIndexer有是三种处理策略：
1， 抛出异常，这是默认行为
2， 跳过不可见的标签
3， 把不可见的标签，标记为numLabels(这个是无用的)。
还用上面的例子，数据如下：
|Id|Category|
|----|----|
|0|a|
|1|b|
|2|c|
|3|a|
|4|a|
|5|c|
|6|d|
|7|e|
如果你没有设置StringIndexer如何处理这些不可见的词，或者设置为了error，他将会抛出一个异常。然而，你如果设置setHandleInvalid("skip")，将会得到如下结果：
|Id|Category|CategoryIndex|
|----|----|----|
|0|a|0.0|
|1|b|2.0|
|2|c|1.0|
注意，包含d,e的行并没有出现。
如果，调用setHandleInvalid("keep")，会得到下面的结果：
|Id|Category|CategoryIndex|
|----|----|----|
|0|a|0.0|
|1|b|2.0|
|2|c|1.0|
|3|d|3.0|
|4|e|3.0|
注意，d，e获得的索引值是3.0
代码用例如下：
importorg.apache.spark.ml.feature.StringIndexer
val df = spark.createDataFrame(
Seq((0,"a"),(1,"b"),(2,"c"),(3,"a"),(4,"a"),(5,"c"))
).toDF("id","category")
val indexer =newStringIndexer()
.setInputCol("category")
.setOutputCol("categoryIndex")
val indexed = indexer.fit(df).transform(df)
indexed.show()
**IndexToString**
对称地StringIndexer，IndexToString将一列标签索引映射回包含作为字符串的原始标签的列。一个常见的用例是从标签生成索引StringIndexer，用这些索引对模型进行训练，并从预测索引列中检索原始标签IndexToString。但是，您可以自由提供自己的标签。
例如，假如我们有dataframe格式如下：
|Id|CategoryIndex|
|----|----|
|0|0.0|
|1|2.0|
|2|1.0|
|3|0.0|
|4|0.0|
|5|1.0|
使用IndexToString 并且使用categoryIndex作为输入列，originalCategory作为输出列，可以检索到原始标签如下：|Id|originalCategory|CategoryIndex|
|----|----|----|
|0|a|0.0|
|1|b|2.0|
|2|c|1.0|
|3|a|0.0|
|4|a|0.0|
|5|c|1.0|
代码案例如下：
importorg.apache.spark.ml.attribute.Attribute
importorg.apache.spark.ml.feature.{IndexToString,StringIndexer}
valdf=spark.createDataFrame(Seq(
(0,"a"),
(1,"b"),
(2,"c"),
(3,"a"),
(4,"a"),
(5,"c")
)).toDF("id","category")
valindexer=newStringIndexer()
.setInputCol("category")
.setOutputCol("categoryIndex")
.fit(df)
valindexed=indexer.transform(df)
println(s"Transformed string column '${indexer.getInputCol}' "+
s"to indexed column '${indexer.getOutputCol}'")
indexed.show()
valinputColSchema=indexed.schema(indexer.getOutputCol)
println(s"StringIndexer will store labels in output column metadata: "+
s"${Attribute.fromStructField(inputColSchema).toString}
")
valconverter=newIndexToString()
.setInputCol("categoryIndex")
.setOutputCol("originalCategory")
valconverted=converter.transform(indexed)
println(s"Transformed indexed column '${converter.getInputCol}' back to original string "+
s"column '${converter.getOutputCol}' using labels in metadata")
converted.select("id","categoryIndex","originalCategory").show()
**推荐阅读：**
**1，[SparkMLlib的数据类型讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484349&idx=2&sn=14f50cd925991205d5a971b4e434b8ee&chksm=9f38e095a84f698332725aa5072c8aca2acef3582ae4c5c2eedd592eea7c15fcb9f9b5ec8db7&scene=21#wechat_redirect)**
**2，[如何将RDD或者MLLib矩阵zhuanzhi](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484349&idx=1&sn=32927ca184a284a0d299fa99a9ba2629&chksm=9f38e095a84f69837217c729583d5a82e9d88d0ed0d4a177b40af9226eb579e9c1db5bff925c&scene=21#wechat_redirect)**
**3，[SparkML模型选择（超参数调整）与调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484330&idx=1&sn=49095988b3bb00259f79432622ad324f&chksm=9f38e082a84f69943044b515643ce051a4d88154fbe0574b24240aa85a501a5846d83c9bc714&scene=21#wechat_redirect)**
**4，[Spark度量系统相关讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483941&idx=1&sn=aa6e5490363db569222806af563c2940&chksm=9f38e10da84f681b34cb4bebf769e3fa8f711475aa7345d2f9fdd867e8f5b1f1a5fa0a172f3f&scene=21#wechat_redirect)**
**本文主要参考翻译整理自Spark官网，打原创标签纯属为了保证，翻译劳动成果，谢谢大家谅解。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)

**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

