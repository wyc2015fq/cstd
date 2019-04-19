# Spark 提供的概率与统计算法 - 基本统计算法 - BlueSky - CSDN博客
2016年04月16日 09:56:33[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：7810
spark-mllib 使用到了Breeze线性代数包，Breeze 采用了 metlib-java包来优化数字处理，但是由于版权问题，Breeze 中缺省不包含 netlib-java 的 native proxy，需要在项目中自行引用。
        compile "org.apache.spark:spark-mllib_${scalaMajorVersion}:${sparkVersion}"
        compile "com.github.fommil.netlib:all:1.1.2"
以下是 spark-mllib 提供的几种基本算法
一、 Summay statistics
    对单个数组而言，Summay statistics 有最大（max）,最小（min）, 平均（mean）, 方差（variance），非零（nonzeros）, 总计（count）等算法，它们通过 MultivariateStatusticaSummary 类来提供计算支持：
**import****org.apache.spark.mllib.linalg.Vector**
**import****org.apache.spark.mllib.stat.**{**MultivariateStatisticalSummary**,**Statistics**}
```
**val** observations**:** RDD[Vector] **=** ... *// an RDD of Vectors*
```
*// Compute column summary statistics.*
**val**summary**:**MultivariateStatisticalSummary=**Statistics**.colStats(observations)
println(summary.mean)*// a dense vector containing the mean value for each column*
println(summary.variance)*// column-wise variance*
println(summary.numNonzeros)*// number of nonzeros in each column*
二、 Correlations
Correlations 是两个数组之间的关系的计算，Spark 目前支持 Pearson 和 Spearman 两类相关性分析。Spark 通过 Statistics 类提供了相关性分析的大部分算法。支持两类数据类型的输入，分别是 RDD[Double]
 和 RDD[Vector] 相应的输出分别是 Double 和 Matrix
Pearson 相关系数用来衡量两个数据（集）是否在同一个层面上。统计学依据数据的计量尺度将数据划分为四大类，即定距型数据（Interval Scale）、定序型数据（Ordinal Scale）、定类型数据（Nominal Scale）和定比型数据 (Ratio
 Scale)。Pearson 用来计算定距变量，相关系数的绝对值越大，相关性越强：相关系数越接近于1或-1，[相关度](http://baike.baidu.com/view/2483063.htm)越强，相关系数越接近于0，相关度越弱。
- 0.8-1.0 极强相关
- 0.6-0.8 强相关
- 0.4-0.6 中等程度相关
- 0.2-0.4 弱相关
- 0.0-0.2 极弱相关或无相关
Spearman相关系数的介绍参考：
[](http://baike.baidu.com/link?url=6OPMQwQFMnKUltwCjJ4h12oGp5n77pxsgVZhjQKpXXEDqDhDkUmOwkj4k79HnRP6kbgIz4A2HsxoaVZiOFobfK)[http://baike.baidu.com/link?url=6OPMQwQFMnKUltwCjJ4h12oGp5n77pxsgVZhjQKpXXEDqDhDkUmOwkj4k79HnRP6kbgIz4A2HsxoaVZiOFobfK](http://baike.baidu.com/link?url=6OPMQwQFMnKUltwCjJ4h12oGp5n77pxsgVZhjQKpXXEDqDhDkUmOwkj4k79HnRP6kbgIz4A2HsxoaVZiOFobfK)
**import****org.apache.spark.SparkContext**
**import****org.apache.spark.mllib.linalg._**
**import****org.apache.spark.mllib.stat.Statistics**
```
**val** sc**:** SparkContext = ...
```
**val**seriesX**:**RDD[Double]**=**...*// a series*
**val**seriesY**:**RDD[Double]**=**...*// must have the same number of partitions and cardinality as seriesX*
```
```
*// compute the correlation using Pearson's method. Enter "spearman" for Spearman's method. If a*
*// method is not specified, Pearson's method will be used by default.*
**val**correlation**:**Double=**Statistics**.corr(seriesX,seriesY,"pearson")
```
**val** data**:** RDD[Vector] **=** ... *// note that each Vector is a row and not a column*
```
*// calculate the correlation matrix using Pearson's method. Use "spearman" for Spearman's method.*
*// If a method is not specified, Pearson's method will be used by default.*
**val**correlMatrix**:**Matrix=**Statistics**.corr(data,"pearson")

三、 Stratified sampling (分层取样)
    Stratified sampling 算法是直接集成到键值对类型 RDD[(K, V)] 的 sampleByKey 和 sampleByKeyExact 方法提供支持，无需通过额外的  spark.mllib 库来支持。
    分层取样顾名思义，就是将数据根据不同的特征分成不同的组，然后按特定条件从不同的组中获取样本并重新组成新的数组。因此 sampleByKey 方法需要作用于一个键值对数组，其中 key 用于分类，value可以是任意数。然后我们通过 fractions 参数来定义分类条件和采样几率，因此 fractions 参数被定义成一个 Map[K, Double] 类型，Key是键值的分层条件，Double 是该满足条件的 Key 条件的采样比例，1.0 代表 100%。
**import****org.apache.spark.SparkContext**
**import****org.apache.spark.SparkContext._**
**import****org.apache.spark.rdd.PairRDDFunctions**
```
**val** sc**:** SparkContext = ...
```
**val**data**=**...*// an RDD[(K, V)] of any key value pairs*
**val**fractions**:**Map[K, Double]**=**...*// specify the exact fraction desired from each key*
```
```
*// Get an exact sample from each stratum*
**val**approxSample**=**data.sampleByKey(withReplacement**=****false**,fractions)
**val**exactSample**=**data.sampleByKeyExact(withReplacement**=****false**,fractions)
// 假设存在以下数组，第一个数字是userId, 后面的数据是某种业务数据
`val list = List(`
`(2147481832,23355149,1)，`
`(2147481832,97301062,1)，`
`(2147481832,21348702,1)，`
`(2147481832,54102337,1)，`
`(2147481832,16822060,1)，`
`(2147481832,11382119,1)，`
`(2147481832,85220256,1)，`
`(2147481832,20137598,1)，`
`(2147481832,48653889,1)，`
`(2147481832,91918798,1)，`
...)
// 我们将它转成Map, 以 userId 为 key，其他数据为
 value
`val data = sc.parallelize(list.toSeq).map(x => (x._1,(x._2,x._3)))`
//
 然后抽取userId 的唯一值做为 fractions的 key，并且假设每个用户的被采样几率都是 80%.
`val fractions = data.map(_._1).distinct.map(x => (x,0.8)).collectAsMap`
// 获得采样数据
`val sampleData = data.sampleByKey(withReplacement = false,fractions)`
sampleByKey 和 sampleByKeyExact 的区别在于 sampleByKey 并不对过滤全量数据，因此只得到近似值，而 sampleByKeyExtra 会对全量数据做采样计算，因此耗费大量的计算资源，但是结果会更准确。
四、 Hypothesis testing (假定检测)
spark通过 Statistics 类来支持 Pearson's chi-squared （卡方检测），主要是比较两个及两个以上样本率( 构成比）以及两个分类变量的关联性分析。其根本思想就是在于比较理论频数和实际频数的吻合程度或拟合优度问题。卡方检测有两种用途，分别是“适配度检定”（Goodness
 of fit）以及“独立性检定”（independence）。
Goodness fo fit（适合度检验）： 执行多次试验得到的观测值，与假设的期望数相比较，观察假设的期望值与实际观测值之间的差距，称为卡方适合度检验，即在于检验二者接近的程度。比如掷色子。
Indenpendence(独立性检验)：卡方独立性检验是用来检验两个属性间是否独立。其中一个属性做为行，另外一个做为列，通过貌似相关的关系考察其是否真实存在相关性。比如天气温变化和肺炎发病率。
假定检测的基本思路是，首先我们假定一个结论，然后为这个结论设置期望值，用实际观察值来与这个值做对比，并设定一个阀值，如果计算结果大于阀值，则假定不成立，否则成立。
根据以上表述，我们需要确定四个值：
1) 结论：结论一般是建立在零假设( Null Hypothesis)的基础上的。零假设即认为观测值与理论值的差异是由于随机误差所致。比如：“掷色子得到的各种结果概率相同”——这个结论显然我们认定的前提是即便不同也是随机因素导致。
2) 期望值：期望值也就是理论值，理论值可以是某种平均数，比如我们投掷120次色子，要维护结论正确，那么每个数字的出现理论值就应该都是20
3) 观测值：也就是实际得到的值
4) 阀值：阀值是根据自由度和显著性水平计算出来的（excel 中的 chiinv() 函数）。自由度=(结果选项数-1)x(对比组数-1)，比如我们将两组掷色子值做比较，那么自由度就是(6-1)x(2-1)=5。显著性水平(a)是原假设为正确的，而我们确把原假设当做错误加以拒绝，犯这种错误的概率，依据拒绝区间所可能承担的风险来决定，一般选择0.05或0.01。
最后就是计算卡方值：卡方值是各组 （观测值－理论值）^2/理论值  的总和。最后就是比较方差值和阀值。如果小于阀值则接受结论，否则拒绝结论。或者根据卡方值反算概率p值(excel 中的 chidist() 函数)，将它和显著性水平比较，小于则拒绝，大于则接受。
更多例子参考：[http://www.zwbk.org/MyLemmaShow.aspx?lid=420659](http://www.zwbk.org/MyLemmaShow.aspx?lid=420659)
**import****org.apache.spark.SparkContext**
**import****org.apache.spark.mllib.linalg._**
**import****org.apache.spark.mllib.regression.LabeledPoint**
**import****org.apache.spark.mllib.stat.Statistics._**
```
**val** sc**:** SparkContext = ...
```
/**************************************************/
**val** vec**:**Vector[Double]=...*// a vector composed of the frequencies of events*
**val** expected**:**Vector[Double]=...*// a vector composed of the frequencies of expected events*
```
```
*// compute the goodness of fit. If a second vector to test against is not supplied as a parameter,*
**val**goodnessOfFitForExpectedTestResult**=****Statistics**.chiSqTest(vec,
 expected)
println(goodnessOfFitForExpectedTestResult)*// summary of the test including the p-value, degrees of freedom,*
*// test statistic, the method used, and the null hypothesis.*
*// the test runs against a uniform distribution(均匀分布).*
*// “均匀分布” 自动计算出，所以无需提供 expected 参数，默认值为：1.0/size*
**val**goodnessOfFitTestResult**=****Statistics**.chiSqTest(vec)
println(goodnessOfFitTestResult)
/**************************************************/
**val**mat**:**Matrix=...*// a contingency matrix. Matrix 中的元素为 Double*
```
```
*// conduct Pearson's independence test on the input contingency matrix*
**val**independenceTestResult**=****Statistics**.chiSqTest(mat)
```
println(independenceTestResult) 
  *// summary of the test including the p-value, degrees of freedom...*
```
/**************************************************/
```
**val** obs**:** RDD[LabeledPoint] **=** ... *// (feature, label) pairs.*
```
*// The contingency table is constructed from the raw (feature, label) pairs and used to conduct*
*// the independence test. Returns an array containing the ChiSquaredTestResult for every feature*
*// against the label.*
**val**featureTestResults**:**Array[ChiSqTestResult]**=****Statistics**.chiSqTest(obs)
**var**i**=**1featureTestResults.foreach{result**=>**
`    println(s"Column $i:\n$result")`
i+=1
}*// summary of the test*
除了 Pearson's chi-squared 外 Statistics 还提供了 1-sample, 2-sided Kolmogorov-Smirnov 检测，效果类似，具体不介绍了。
五、Streaming Significance Testing
显著性检验就是事先对总体形式做出一个假设，然后用样本信息来判断这个假设（原假设）是否合理，即判断真实情况与原假设是否显著地有差异。或者说，显著性检验要判断样本与我们对总体所做的假设之间的差异是否纯属偶然，还是由我们所做的假设与总体真实情况不一致所引起的。spark.mllib 实现了一个在线测试用以支持类似A/B测试这样的用例。
Spark 通过 StreamingTest 来提供支持，它接受两个设置参数：
- peacePeriod: 用于设置一段冷静期来忽略stream中开头的一段脏数据。
- windowSize: 采集窗口，如果给0，则不分批次一次处理全量数据
StreamingTest 接受的数据参数是 DStreaming[(Boolean, Double)] 类型，第一个 Boolean 表明该组数据是 control 组（false）还是 treatment 组（true），第二个 Double 是检测数据，所谓 control 和 treatment 组就是分别存放参照数据的组和修正数据的组
六、Random data generation
RandomRDDs 是一个工具集，用来生成含有随机数的RDD，可以按各种给定的分布模式生成数据集，例如按正态分布生成随机数：
**import****org.apache.spark.SparkContext**
**import****org.apache.spark.mllib.random.RandomRDDs._**
```
**val** sc**:** SparkContext = ...
```
*// 生成100个服从正态分配N(0,1)的RDD〔Double〕，并且分布在 10 个分区中*
**val**u**=**normalRDD(sc,100L,10)
*// 转换为 N(1,4) 正态分布.*
**val**v**=**u.map(x**=>**1.0+2.0*x)
七、Kernel density estimation
Spark ML 提供了一个工具类 KernelDensity 用于核密度估算，核密度估算的意思是根据已知的样本估计未知的密度，属於非参数检验方法之一。核密度估计的原理是。观察某一事物的已知分布，如果某一个数在观察中出现了，我们可以认为这个数的概率密度很大，和这个数比较近的数的概率密度也会比较大，而那些离这个数远的数的概率密度会比较小。并最终根据所有的数拟合出未知的全貌
详细介绍：[](http://blog.163.com/zhuandi_h/blog/static/1802702882012111092743556/)[http://blog.163.com/zhuandi_h/blog/static/1802702882012111092743556/](http://blog.163.com/zhuandi_h/blog/static/1802702882012111092743556/)
**import****org.apache.spark.mllib.stat.KernelDensity**
**import****org.apache.spark.rdd.RDD**
```
**val** data**:** RDD[Double] **=** ... *// an RDD of sample data*
```
*// Construct the density estimator with the sample data and a standard deviation for the Gaussian*
*// kernels*
**val**kd**=****new****KernelDensity**()
```
.setSample(data)
  .setBandwidth(3.0)
```
*// Find density estimates for the given values*
**val**densities**=**kd.estimate(**Array**(-1.0,2.0,5.0))
