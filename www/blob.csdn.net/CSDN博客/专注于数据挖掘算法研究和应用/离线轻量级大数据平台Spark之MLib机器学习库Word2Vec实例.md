# 离线轻量级大数据平台Spark之MLib机器学习库Word2Vec实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月07日 15:13:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3897
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Word2Vecword2vec能将文本中出现的词向量化，可以在捕捉语境信息的同时压缩数据规模。Word2Vec实际上是两种不同的方法：Continuous Bag of Words (CBOW) 和 Skip-gram。CBOW的目标是根据上下文来预测当前词语的概率。Skip-gram刚好相反：根据当前词语来预测上下文的概率。这两种方法都利用人工神经网络作为它们的分类算法。起初，每个单词都是一个随机 N 维向量。经过训练之后，该算法利用 CBOW 或者 Skip-gram 的方法获得了每个单词的最优向量。 

实例代码如下：



```java
package sk.mlib;

import java.util.Arrays;
import java.util.List;

import org.apache.spark.ml.feature.Word2Vec;
import org.apache.spark.ml.feature.Word2VecModel;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.RowFactory;
import org.apache.spark.sql.SparkSession;
import org.apache.spark.sql.types.*;

public class Word2VecDemo {
	public static void main(String[] args) {
	    SparkSession spark = SparkSession.builder().appName("Word2VecDemo").getOrCreate();

	    // Input data: Each row is a bag of words from a sentence or document.
	    List<Row> data = Arrays.asList(
	      RowFactory.create(Arrays.asList("Hi I heard about Spark".split(" "))),
	      RowFactory.create(Arrays.asList("I wish Java could use case classes".split(" "))),
	      RowFactory.create(Arrays.asList("Logistic regression models are neat".split(" ")))
	    );
	    StructType schema = new StructType(new StructField[]{
	      new StructField("text", new ArrayType(DataTypes.StringType, true), false, Metadata.empty())
	    });
	    Dataset<Row> documentDF = spark.createDataFrame(data, schema);

	    // Learn a mapping from words to Vectors.
	    Word2Vec word2Vec = new Word2Vec()
	      .setInputCol("text")
	      .setOutputCol("result")
	      .setVectorSize(3)
	      .setMinCount(0);
	    Word2VecModel model = word2Vec.fit(documentDF);
	    Dataset<Row> result = model.transform(documentDF);
	    for (Row r : result.select("text","result").takeAsList(3)) {
	      System.out.println(r);
	    }
	    
	    spark.stop();
	  }
}
/*
 执行结果：
 [WrappedArray(Hi, I, heard, about, Spark),[-0.028139343485236168,0.04554025698453188,-0.013317196490243079]]
[WrappedArray(I, wish, Java, could, use, case, classes),[0.06872416580361979,-0.02604914902310286,0.02165239889706884]]
[WrappedArray(Logistic, regression, models, are, neat),[0.023467857390642166,0.027799883112311366,0.0331136979162693]]
 */
```





