# 离线轻量级大数据平台Spark之MLib机器学习库TF-IDF实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月02日 12:45:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2604
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









TF-IDF（termfrequency–inverse document frequency）是TF-IDF是一种统计方法，用以评估一字词对于一个文件集或一个语料库中的其中一份文件的重要程度。字词的重要性随着它在文件中出现的次数成正比增加，但同时会随着它在语料库中出现的频率成反比下降。主要思想是：如果某个词或短语在一篇文章中出现的频率TF高，并且在其他文章中很少出现，则认为此词或者短语具有很好的类别区分能力，适合用来分类。

在一份给定的文件里，词频 (term frequency, TF) 指的是某一个给定的词语在该文件中出现的次数。这个数字通常会被归一化（分子一般小于分母区别于IDF），以防止它偏向长的文件。（同一个词语在长文件里可能会比短文件有更高的词频，而不管该词语重要与否。）



![](https://img-blog.csdn.net/20161102124416712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


nij是i词在文件j中的出现次数，而分母则是在j文件中所有字词的出现次数之和。

逆向文件频率(inverse document frequency, IDF) 是一个词语普遍重要性的度量。某一特定词语的IDF，可以由总文件数目除以包含该词语之文件的数目，再将得到的商取对数得到。

![](https://img-blog.csdn.net/20161102124428947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




|D|是文件总数，j是包含i词的文件数，一般加1，避免分母为零。

某一特定文件内的高词语频率，以及该词语在整个文件集合中的低文件频率，可以产生出高权重的TF-IDF。因此，TF-IDF倾向于过滤掉常见的词语，保留重要的词语。

![](https://img-blog.csdn.net/20161102124442318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




spark.mllib中的算法接口是基于RDDs的；spark.ml中的算法接口是基于DataFrames的。

Spark平台ML库TF-IDF特征提取算法实例代码如下：



```java
package sk.mlib;

import java.util.Arrays;
import java.util.List;

import org.apache.spark.ml.feature.HashingTF;
import org.apache.spark.ml.feature.IDF;
import org.apache.spark.ml.feature.IDFModel;
import org.apache.spark.ml.feature.Tokenizer;
import org.apache.spark.ml.linalg.Vector;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.RowFactory;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.types.DataTypes;
import org.apache.spark.sql.types.Metadata;
import org.apache.spark.sql.types.StructField;
import org.apache.spark.sql.types.StructType;


public class TfIdfDemo {
	public static void main(String[] args) {
		
	    SparkSession spark = SparkSession
	      .builder()
	      .appName("TfIdfDemo")
	      .getOrCreate();

	    List<Row> data = Arrays.asList(
	      RowFactory.create(0.0, "I heard about Spark and i like spark"),
	      RowFactory.create(1.0, "I wish Java could use case classes for spark"),
	      RowFactory.create(2.0, "Logistic regression models of spark are neat and easy to use")
	    );
	    StructType schema = new StructType(new StructField[]{
	      new StructField("label", DataTypes.DoubleType, false, Metadata.empty()),
	      new StructField("sentence", DataTypes.StringType, false, Metadata.empty())
	    });
	    
	    Dataset<Row> sentenceData = spark.createDataFrame(data, schema);
	    Tokenizer tokenizer = new Tokenizer().setInputCol("sentence").setOutputCol("words");  
	    Dataset<Row> wordsData = tokenizer.transform(sentenceData);
	    
	    //打印结果  
	    //String[] cols=wordsData.columns(); 
	    //for (String col:cols){
	    //	System.out.println(col);
	    //}
	    //int rows=(int)wordsData.count();
	    //for (Row r : wordsData.select("label","sentence","words").takeAsList(rows)){
	    //	System.out.println(r.getDouble(0));
	    //	System.out.println(r.getString(1));
	    //	System.out.println(r.getAs(2));
	    //}
	    
	    //将每个词映射到哈希表，统计其频次，将单词映射成整数，作为hash索引
	    int numFeatures = 20;
	    HashingTF hashingTF = new HashingTF()
	      .setInputCol("words")
	      .setOutputCol("rawFeatures")
	      .setNumFeatures(numFeatures);//Hash分桶的数量设置，默认为2的20次方
	    Dataset<Row> featurizedData = hashingTF.transform(wordsData);
	   
	    //打印结果
	    int rows=(int)featurizedData.count();
	    for (Row r : featurizedData.select("label","sentence","words","rawFeatures").takeAsList(rows)){
	    	System.out.println(r.getDouble(0));
	    	System.out.println(r.getString(1));
	    	System.out.println(r.getAs(2));
	    	System.out.println(r.getAs(3));
	    }
	    //计算TF-IDF，对数log，分子是3份文件，分母是包含词条的文件数
	    // alternatively, CountVectorizer can also be used to get term frequency vectors
	    IDF idf = new IDF().setInputCol("rawFeatures").setOutputCol("features");
	    IDFModel idfModel = idf.fit(featurizedData);
	    Dataset<Row> rescaledData = idfModel.transform(featurizedData);
	    //打印结果
	    for (Row r : rescaledData.select("features", "label").takeAsList(3)) {
	      Vector features = r.getAs(0);
	      Double label = r.getDouble(1);
	      System.out.println(features);
	      System.out.println(label);
	    }
	    spark.stop();
	  }
}
/*
 * 执行结果：
 0.0
I heard about Spark and i like spark
WrappedArray(i, heard, about, spark, and, i, like, spark)
(20,[5,9,10,13,17],[2.0,2.0,1.0,1.0,2.0])
1.0
I wish Java could use case classes for spark
WrappedArray(i, wish, java, could, use, case, classes, for, spark)
(20,[2,5,7,9,13,15,16],[1.0,1.0,1.0,3.0,1.0,1.0,1.0])
2.0
Logistic regression models of spark are neat and easy to use
WrappedArray(logistic, regression, models, of, spark, are, neat, and, easy, to, use)
(20,[0,3,4,5,6,8,9,13,15,18],[1.0,1.0,1.0,1.0,1.0,1.0,1.0,2.0,1.0,1.0])
(20,[5,9,10,13,17],[0.0,0.0,0.6931471805599453,0.0,1.3862943611198906])
0.0
(20,[2,5,7,9,13,15,16],[0.6931471805599453,0.0,0.6931471805599453,0.0,0.0,0.28768207245178085,0.6931471805599453])
1.0
(20,[0,3,4,5,6,8,9,13,15,18],[0.6931471805599453,0.6931471805599453,0.6931471805599453,0.0,0.6931471805599453,0.6931471805599453,0.0,0.0,0.28768207245178085,0.6931471805599453])
2.0
 */
```





