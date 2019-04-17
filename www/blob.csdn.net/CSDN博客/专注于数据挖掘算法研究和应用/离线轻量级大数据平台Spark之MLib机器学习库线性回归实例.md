# 离线轻量级大数据平台Spark之MLib机器学习库线性回归实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月31日 16:04:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2449
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、线性回归

线性回归是利用称为线性回归方程的函数对一个或多个自变量和因变量之间关系进行建模的一种回归分析方法，只有一个自变量的情况称为简单回归，大于一个自变量情况的叫做多元回归，在实际情况中大多数都是多元回归。

线性回归（Linear Regression）问题属于监督学习（Supervised Learning）范畴，又称分类（Classification）或归纳学习（Inductive Learning）。这类分析中训练数据集中给出的数据类型是确定的。机器学习的目标是，对于给定的一个训练数据集，通过不断的分析和学习产生一个联系属性集合和类标集合的分类函数（Classification Function）或预测函数）Prediction Function），这个函数称为分类模型（Classification Model——或预测模型（Prediction
 Model）。通过学习得到的模型可以是一个决策树、规格集、贝叶斯模型或一个超平面。通过这个模型可以对输入对象的特征向量预测或对对象的类标进行分类。

回归问题中通常使用最小二乘（Least Squares）法来迭代最优的特征中每个属性的比重，通过损失函数（Loss Function）或错误函数（Error Function)定义来设置收敛状态，即作为梯度下降算法的逼近参数因子。

实例中给出了如何导入训练集数据，将其解析为带标签点的RDD，然后使用了LinearRegressionWithSGD 算法来建立一个简单的线性模型来预测标签的值，最后计算了均方差来评估预测值与实际值的吻合度。

线性回归分析的整个过程可以简单描述为如下三个步骤：

寻找合适的预测函数，即上文中的 h(x) ，用来预测输入数据的判断结果。这个过程是非常关键的，需要对数据有一定的了解或分析，知道或者猜测预测函数的“大概”形式，比如是线性函数还是非线性函数，若是非线性的则无法用线性回归来得出高质量的结果。

构造一个Loss函数（损失函数），该函数表示预测的输出（h）与训练数据标签之间的偏差，可以是二者之间的差（h-y）或者是其他的形式（如平方差开方）。综合考虑所有训练数据的“损失”，将Loss求和或者求平均，记为 J(θ) 函数，表示所有训练数据预测值与实际类别的偏差。

显然， J(θ) 函数的值越小表示预测函数越准确（即h函数越准确），所以这一步需要做的是找到 J(θ) 函数的最小值。找函数的最小值有不同的方法，Spark中采用的是梯度下降法（stochastic gradient descent，SGD)。




2、Java开发

Java开发上，引入spark-mllib_2.11-2.0.1.jar、spark-mllib-local_2.11-2.0.1.jar、spark-catalyst_2.11-2.0.1.jar。

参考spark的example案例的代码，输出sklr.jar包并提交执行。

bin/spark-submit --class sk.mlib.LinearRegressionWithSGDDemo --master local /tmp/sklr.jar

代码如下：

```java
package sk.mlib;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;
import org.apache.spark.api.java.JavaDoubleRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.mllib.linalg.Vectors;
import org.apache.spark.mllib.regression.LabeledPoint;
import org.apache.spark.mllib.regression.LinearRegressionModel;
import org.apache.spark.mllib.regression.LinearRegressionWithSGD;

public class LinearRegressionWithSGDDemo {
	public static void main(String[] args) {
	    SparkConf conf = new SparkConf().setAppName("LinearRegressionWithSGDDemo");
	    JavaSparkContext sc = new JavaSparkContext(conf);
	    sc.setLogLevel("OFF");//关闭日志输出，方便查看输出结果
	    // Load and parse the data
	    String path = "/tmp/lpsa.data";
	    JavaRDD<String> data = sc.textFile(path);
	    JavaRDD<LabeledPoint> parsedData = data.map(
	      new Function<String, LabeledPoint>() {
	        public LabeledPoint call(String line) {
	          String[] parts = line.split(",");
	          String[] features = parts[1].split(" ");
	          double[] v = new double[features.length];
	          for (int i = 0; i < features.length - 1; i++) {
	            v[i] = Double.parseDouble(features[i]);
	          }
	          //打印map结果
	          System.out.println(Double.parseDouble(parts[0])+"|"+Vectors.dense(v));
	          return new LabeledPoint(Double.parseDouble(parts[0]), Vectors.dense(v));
	        }
	      }
	    );
	    parsedData.cache();

	    // Building the model
	    int numIterations = 100;
	    double stepSize = 0.00000001;
	    final LinearRegressionModel model =
	      LinearRegressionWithSGD.train(JavaRDD.toRDD(parsedData), numIterations, stepSize);
        
	    // Evaluate model on training examples and compute training error
	    JavaRDD<Tuple2<Double, Double>> valuesAndPreds = parsedData.map(
	      new Function<LabeledPoint, Tuple2<Double, Double>>() {
	        public Tuple2<Double, Double> call(LabeledPoint point) {
	          double prediction = model.predict(point.features());
	          //打印预测结果
	          System.out.println(prediction+"|"+point.label());
	          return new Tuple2<>(prediction, point.label());
	        }
	      }
	    );
	    double MSE = new JavaDoubleRDD(valuesAndPreds.map(
	      new Function<Tuple2<Double, Double>, Object>() {
	        public Object call(Tuple2<Double, Double> pair) {
	          //打印偏差结果
		      System.out.println(pair._1()+":"+pair._2()+"|"+Math.pow(pair._1() - pair._2(), 2.0));
	          return Math.pow(pair._1() - pair._2(), 2.0);
	        }
	      }
	    ).rdd()).mean();
	    System.out.println("training Mean Squared Error = " + MSE);

	    // Save and load model
	    model.save(sc.sc(), "/tmp/LinearRegressionWithSGDModel");
	    LinearRegressionModel sameModel = LinearRegressionModel.load(sc.sc(), "/tmp/LinearRegressionWithSGDModel");

	    sc.stop();
	  }
}
/*执行结果：
 * training Mean Squared Error = 7.451032827699878
 */
```
3、测试数据，在/spark-2.0.1-bin-hadoop2.7/data/mllib/ridge-data/lpsa.data





