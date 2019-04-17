# 离线轻量级大数据平台Spark之MLib机器学习库朴素贝叶斯实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月03日 15:13:33[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3082
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









 1、朴素贝叶斯介绍

表示事件B已经发生的前提下，事件A发生的概率，叫做事件B发生下事件A的条件概率，公式为：


![](https://img-blog.csdn.net/20161103151050193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

贝叶斯定理： 

![](https://img-blog.csdn.net/20161103151119164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


从已知P(A|B)获得P(B|A)值。

假设A和B代表两类互相影响的事件，如B代表正常邮件和骚扰邮件事件、A代表邮件文本中出现特定词汇的事件。定义：Ai，其中i∈[1,m]，m是A事件总数；Bj，其中j∈[1,n]，n是B事件的总数。

根据条件概率公式，先根样本数求得得P(Ai| Bj)= P(AiBj)/ P(Bj)，即AB事件同时发生的概率除以B事件单独发生时间概率。接着要预测在A事件出现下发生事件B的概率，即：

P(Bj |Ai)=P(Bj)* P(A0| Bj) * P(A1| Bj) *…* P(An| Bj)

朴素贝叶斯算法考虑事件的相互独立性，有：多项式朴素贝叶斯（ multinomial naive Bayes ）和伯努利朴素贝叶斯（ Bernoulli naive Bayes）。MultinomialNB这个分类器以出现次数作为特征值，使用的TF-IDF也能符合这类分布。其他的朴素贝叶斯分类器如GaussianNB适用于高斯分布（正态分布）的特征，而BernoulliNB适用于伯努利分布（二值分布）的特征。





2、案例说明：

1）输入数据:天气情况和每天是否踢足球的记录表

日期踢足球
天气温度湿度风速

1号否(0)
晴天(0)热(0)
高(0)低(0)

2号否(0)
晴天(0)热(0)
高(0)高(1)

3号是(1)
多云(1)热(0)
高(0)低(0)

4号是(1)
下雨(2)舒适(1)
高(0)低(0)

5号是(1)
下雨(2)凉爽(2)
正常(1)低(0)

6号否(0)
下雨(2)凉爽(2)
正常(1)高(1)

7号是(1)
多云(1)凉爽(2)
正常(1)高(1)

8号否(0)
晴天(0)舒适(1)
高(0)低(0)

9号是(1)
晴天(0)凉爽(2)
正常(1)低(0)

10号是(1)
下雨(2)舒适(1)
正常(1)低(0)

11号是(1)
晴天(0)舒适(1)
正常(1)高(1)

12号是(1)
多云(1)舒适(1)
高(0)高(1)

13号是(1)
多云(1)热(0)
正常(1)低(0)

14号否(0)
下雨(2)舒适(1)
高(0)高(1)

15号？晴天(0)凉爽(2)高(0)高(1)

抽象为：

0,0 0 0 0

0,0 0 0 1

1,1 0 0 0

1,2 1 0 0

1,2 2 1 0

0,2 2 1 1

1,1 2 1 1

0,0 1 0 0

1,0 2 1 0

1,2 1 1 0

1,0 1 1 1

1,1 1 0 1

1,1 0 1 0

0,2 1 0 1

2）输出：如果15号的天气为(晴天，凉爽，湿度高，风速高，预测是否去踢球的概率）

3）计算过程：

假设15号去踢球，踢球概率为：

P(踢)=9/14

P(晴天|踢)=2/9

P(凉爽|踢)=3/9

P(湿度高|踢)=3/9

P(风速高|踢)=3/9

P(踢)由踢的天数除以总天数得到，P(晴天|踢)为踢球的同事是晴天除以踢的天数得到，其他以此类推。

P(踢|晴天,凉爽,湿度高,风速高)=P(踢)* P(晴天|踢)* P(凉爽|踢)* P(湿度高|踢) *P(风速高|踢)=9/14*2/9*3/9*3/9*3/9=0.00529



假设15号不去踢球，概率为：

P(不踢)=5/14

P(晴天|不踢)=3/5

P(凉爽|不踢)=1/5

P(湿度高|不踢)=4/5

P(风速高|不踢)=3/5

P(不踢|晴天,凉爽,湿度高,风速高)=

P(不踢)* P(晴天|不踢)* P(凉爽|不踢)* P(湿度高|不踢) *P(风速高|不踢)=5/14*3/5*1/5*4/5*3/5=0.02057

说明不去踢足球的概率比去踢足球的概率高。




3、Spark平台MLib库朴素贝叶斯处理流程如下图：

![](https://img-blog.csdn.net/20161103151418947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

代码和执行结果如下：



```java
package sk.mlib;

import scala.Tuple2;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.mllib.classification.NaiveBayes;
import org.apache.spark.mllib.classification.NaiveBayesModel;
import org.apache.spark.mllib.linalg.Vectors;
import org.apache.spark.mllib.regression.LabeledPoint;
import org.apache.spark.SparkConf;


public class NaiveBayesDemo {
	public static void main(String[] args) {
	    SparkConf sparkConf = new SparkConf().setAppName("NaiveBayesDemo");
	    JavaSparkContext jsc = new JavaSparkContext(sparkConf);
	    //加载数据
	    String path = "/tmp/NaiveBayesdata.txt";
	    JavaRDD<String> lines =jsc.textFile(path);
	    JavaRDD<LabeledPoint> parsedData = lines.map(new Function<String, LabeledPoint>() {
			public LabeledPoint call(String s) {
	          String[] sarray = s.split(",");
	          Double dLabel=Double.valueOf(sarray[0]);
	          String[] sFeatures=sarray[1].split(" ");
	          double[] values = new double[sFeatures.length];
	          for (int i = 0; i < sFeatures.length; i++) {
	            values[i] = Double.parseDouble(sFeatures[i]);
	          }
	          System.out.println(dLabel+":"+Vectors.dense(values));
	          LabeledPoint lp=new LabeledPoint(dLabel,Vectors.dense(values));
	          return lp; 
	        }
	      }
	    );
	    parsedData.cache();
	    //JavaRDD<LabeledPoint> inputData = MLUtils.loadLibSVMFile(jsc.sc(), path).toJavaRDD();
	     // 把数据的60%作为训练集，40%作为测试集.
	    JavaRDD<LabeledPoint>[] tmp = parsedData.randomSplit(new double[]{0.6, 0.4});
	    JavaRDD<LabeledPoint> training = tmp[0]; // training set
	    JavaRDD<LabeledPoint> test = tmp[1]; // test set
	    //获得训练模型,第一个参数为数据，第二个参数为平滑参数，默认为1，可改
	    final NaiveBayesModel model = NaiveBayes.train(training.rdd(), 1.0);
	    //对模型进行准确度分析
	    JavaPairRDD<Double, Double> predictionAndLabel =
	      test.mapToPair(new PairFunction<LabeledPoint, Double, Double>() {
	        @Override
	        public Tuple2<Double, Double> call(LabeledPoint p) {
	          return new Tuple2<>(model.predict(p.features()), p.label());
	        }
	      });
	    double accuracy = predictionAndLabel.filter(new Function<Tuple2<Double, Double>, Boolean>() {
	      @Override
	      public Boolean call(Tuple2<Double, Double> pl) {
	        return pl._1().equals(pl._2());
	      }
	    }).count() / (double) test.count();
        System.out.println("accuracy-->"+accuracy);
	    //保存和加载训练模型
	    model.save(jsc.sc(), "/tmp/myNaiveBayesModel");
	    NaiveBayesModel sameModel = NaiveBayesModel.load(jsc.sc(), "/tmp/myNaiveBayesModel");
	    //对新的事件进行概率预测
	    System.out.println("Prediction of (0.0, 2.0, 0.0, 1.0):"+sameModel.predict(Vectors.dense(0.0,2.0,0.0,1.0)));

	    jsc.stop();
	  }
}
/*
  执行结果：
 0.0:[0.0,0.0,0.0,0.0]
0.0:[0.0,0.0,0.0,1.0]
1.0:[1.0,0.0,0.0,0.0]
1.0:[2.0,1.0,0.0,0.0]
1.0:[2.0,2.0,1.0,0.0]
0.0:[2.0,2.0,1.0,1.0]
1.0:[1.0,2.0,1.0,1.0]
0.0:[0.0,1.0,0.0,0.0]
1.0:[0.0,2.0,1.0,0.0]
1.0:[2.0,1.0,1.0,0.0]
1.0:[0.0,1.0,1.0,1.0]
1.0:[1.0,1.0,0.0,1.0]
1.0:[1.0,0.0,1.0,0.0]
0.0:[2.0,1.0,0.0,1.0]
accuracy-->0.75，执行结果不一致，和模型训练本身有关系
Prediction of (0.0, 2.0, 0.0, 1.0):0.0   accuracy低于0.5时是1.0
 */
```





