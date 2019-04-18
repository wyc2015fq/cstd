# 离线轻量级大数据平台Spark之MLib机器学习库聚类算法KMeans实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月31日 14:47:42[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3172
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、KMeans算法

所谓聚类问题，就是给定一个元素集合D，其中每个元素具有n个可观察属性，使用某种算法将D划分成k个子集，要求每个子集内部的元素之间相异度尽可能低，而不同子集的元素相异度尽可能高。其中每个子集叫做一个簇。与分类不同，分类是示例式学习，要求分类前明确各个类别，并断言每个元素映射到一个类别。而聚类是观察式学习，在聚类前可以不知道类别甚至不给定类别数量，是无监督学习的一种。

聚类算法是机器学习（或者说是数据挖掘更合适）中重要的一部分，除了最为简单的K-Means聚类算法外，比较常见的还有层次法（CURE、CHAMELEON等）、网格算法（STING、WaveCluster等）等。K-means聚类属于无监督学习，而回归、朴素贝叶斯、SVM等都是有类别标签y的，也就是说样例中已经给出了样例的分类。而聚类的样本中却没有给定y，只有特征x。

K-Means属于基于平方误差的迭代重分配聚类算法，其核心思想十分简单：

l随机选择K个中心点；

l计算所有点到这K个中心点的距离，选择距离最近的中心点为其所在的簇；

l简单地采用算术平均数（mean）来重新计算K个簇的中心；

l重复步骤2和3，直至簇类不再发生变化或者达到最大迭代值；

l输出结果。

K-Means算法的结果好坏依赖于对初始聚类中心的选择，容易陷入局部最优解，对K值的选择没有准则可依循，对异常数据较为敏感，只能处理数值属性的数据，聚类结构可能不平衡。

实例中进行如下步骤：

装载数据，数据以文本文件方式进行存放；

将数据集聚类，设置2个类和20次迭代，进行模型训练形成数据模型；

打印数据模型的中心点；

使用误差平方之和来评估数据模型；

使用模型测试单点数据；

保存模型并加载模型。

2、Java开发

      Java开发上，引入spark-mllib_2.11-2.0.1.jar、spark-mllib-local_2.11-2.0.1.jar、spark-catalyst_2.11-2.0.1.jar。

     参考spark的example案例的代码，输出skkm.jar包并提交执行。

bin/spark-submit --class sk.mlib.KMeansDemo --master local /tmp/skkm.jar

具体代码如下：



```java
package sk.mlib;

import java.util.Arrays;
import java.util.logging.Level;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaDoubleRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.mllib.clustering.KMeans;
import org.apache.spark.mllib.clustering.KMeansModel;
import org.apache.spark.mllib.linalg.Vector;
import org.apache.spark.mllib.linalg.Vectors;

public class KMeansDemo {

	public static void main(String[] args) throws Exception {
		String path = "/tmp/kmeansData.txt";
		SparkConf conf = new SparkConf().setAppName("JavaKMeansExample");
	    JavaSparkContext jsc = new JavaSparkContext(conf);
	    jsc.setLogLevel("OFF");//关闭日志输出，方便查看输出结果
	    // Load and parse data
	    JavaRDD<String> data = jsc.textFile(path);
	    JavaRDD<Vector> parsedData = data.map(new Function<String, Vector>() {
	        public Vector call(String s) {
	          String[] sarray = s.split(" ");
	          double[] values = new double[sarray.length];
	          for (int i = 0; i < sarray.length; i++) {
	            values[i] = Double.parseDouble(sarray[i]);
	          }
	          return Vectors.dense(values);
	        }
	      }
	    );
	    parsedData.cache();
	    // Cluster the data into two classes using KMeans
	    // 将数据集聚类，2个类，20次迭代，进行模型训练形成数据模型
	    int numClusters = 2;
	    int numIterations = 20;
	    KMeansModel clusters = KMeans.train(parsedData.rdd(), numClusters, numIterations);
	    // 打印数据模型的中心点
	    System.out.println("Cluster centers:");
	    for (Vector center: clusters.clusterCenters()) {
	      System.out.println(" " + center);
	    }
	    // 使用误差平方之和来评估数据模型
	    double cost = clusters.computeCost(parsedData.rdd());
	    System.out.println("Cost: " + cost);
       
	    // Evaluate clustering by computing Within Set Sum of Squared Errors
	    double WSSSE = clusters.computeCost(parsedData.rdd());
	    System.out.println("Within Set Sum of Squared Errors = " + WSSSE);
	    
	    // 使用模型测试单点数据
	    // JavaDoubleRDD testdata1=jsc.parallelizeDoubles(Arrays.asList(0.25,0.25,0.25));
	    double[] testdata1={0.25,0.25,0.25};
	    double[] testdata2={8.12,8.12,8.12};
	    System.out.println("Vectors 0.25 0.25 0.25 is belongs to clusters:" + clusters.predict(Vectors.dense(testdata1)));
	    System.out.println("Vectors 8.12,8.12,8.12 is belongs to clusters:" + clusters.predict(Vectors.dense(testdata2)));
	    
	    // Save and load model
	    clusters.save(jsc.sc(), "/tmp/KMeansModel");
	    KMeansModel sameModel = KMeansModel.load(jsc.sc(),"/tmp/KMeansModel");
	    // 打印导出的数据模型的中心点
	    System.out.println("Cluster centers:");
	    for (Vector center: sameModel.clusterCenters()) {
	      System.out.println(" " + center);
	    }
	    jsc.stop();
	}
	
}

/*测试数据kmeansData.txt：
0.0 0.0 0.0
0.1 0.1 0.1
0.2 0.2 0.2
9.0 9.0 9.0
9.1 9.1 9.1
9.2 9.2 9.2
*/
//提交执行：bin/spark-submit --class sk.mlib.KMeansDemo --master local /tmp/skkm.jar
/*执行结果：
 Cluster centers:
 [9.1,9.1,9.1]
 [0.1,0.1,0.1]
Cost: 0.11999999999994547
Within Set Sum of Squared Errors = 0.11999999999994547
Vectors 0.25 0.25 0.25 is belongs to clusters:1
Vectors 8.12,8.12,8.12 is belongs to clusters:0
Cluster centers:
 [9.1,9.1,9.1]
 [0.1,0.1,0.1]
*/
```





