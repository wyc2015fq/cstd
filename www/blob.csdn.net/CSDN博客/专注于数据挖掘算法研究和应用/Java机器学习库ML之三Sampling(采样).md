# Java机器学习库ML之三Sampling(采样) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月21日 17:17:21[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2489








场景：从样本集中采样80%用于训练，20%用于验证。

参考代码如下：



```java

```
package com.gddx;

import java.io.File;
import java.util.Map;

import libsvm.LibSVM;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.evaluation.EvaluateDataset;
import net.sf.javaml.classification.evaluation.PerformanceMeasure;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.sampling.Sampling;
import net.sf.javaml.tools.data.FileHandler;
import be.abeel.util.Pair;

/**
 * Sample program illustrating how to use sampling.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialSampling {

	public static void main(String[] args) throws Exception {

		Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");

		Sampling s = Sampling.SubSampling;
		Pair<Dataset, Dataset> datass = s.sample(data, (int) (data.size() * 0.8));
		System.out.println(datass.x().instance(0));//训练集
		System.out.println(datass.y().instance(0));//测试集
		Classifier c = new LibSVM();
		c.buildClassifier(datass.x());
		Map<Object,PerformanceMeasure> pms = EvaluateDataset.testDataset(c, datass.y());
		System.out.println(pms);
        /*
		for (int i = 0; i < 5; i++) {
			Pair<Dataset, Dataset> datas = s.sample(data, (int) (data.size() * 0.8), i);
			Classifier c = new LibSVM();
			c.buildClassifier(datas.x());
			Map<Object,PerformanceMeasure> pms = EvaluateDataset.testDataset(c, datas.y());
			System.out.println(pms);

		}*/

	}
}





