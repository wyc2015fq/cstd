# 机器学习知识点(二十五)Java实现隐马尔科夫模型HMM之jahmm库 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月16日 15:52:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3816
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









                
1、隐马尔可夫模型HMM的应用场景，关乎于序列和状态变化的都可以。

   发现java有可库，专为开发HMM，可惜只能在CSDN上有得下载。


2、jahmm是java开发隐马尔科夫模型的一个jar包。

   下载jahmm-0.6.1.rar包，源地址是google（国内无法访问），好在CSDN资源上有得下载，我也上传一个（零积分可下载）：

    http://download.csdn.net/detail/fjssharpsword/9843680

   包里有jar还有html可以参照学习；userguide.pdf作为入门看，html是jar中类和函数说明，要重点看。

   可惜包里没有源码，有兴趣可以jar反编译看看。



3、工程中引入jahmm-0.6.1.jar，导入结构图如下：

![](https://img-blog.csdn.net/20170516155200404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



4、参照userguide.pdf建立的demo代码如下：



```java
package sk.ml;

import java.io.FileReader;
import java.io.Reader;
import java.util.List;

import be.ac.ulg.montefiore.run.jahmm.Hmm;
import be.ac.ulg.montefiore.run.jahmm.ObservationInteger;
import be.ac.ulg.montefiore.run.jahmm.ObservationVector;
import be.ac.ulg.montefiore.run.jahmm.OpdfInteger;
import be.ac.ulg.montefiore.run.jahmm.OpdfIntegerFactory;
import be.ac.ulg.montefiore.run.jahmm.OpdfMultiGaussianFactory;
import be.ac.ulg.montefiore.run.jahmm.io.ObservationSequencesReader;
import be.ac.ulg.montefiore.run.jahmm.io.ObservationVectorReader;
import be.ac.ulg.montefiore.run.jahmm.learn.KMeansLearner;

public class HMMDemo {
	public static void main(String args[]) throws Exception{
		Reader reader = new FileReader("D:\\tmp\\test.seq");
		List<List<ObservationVector >> seqs = ObservationSequencesReader.readSequences (new ObservationVectorReader (),reader);
		reader.close ();
		KMeansLearner <ObservationVector > kml = new KMeansLearner <ObservationVector >(3, new OpdfMultiGaussianFactory(2) , seqs);
		Hmm <ObservationVector> fittedHmm = kml.learn ();
		System.out.println(fittedHmm.nbStates());
		/*
		//给定高斯分布的均值和协方差矩阵
		 double [] mean = {2.0, 4.0};
		 double [][] covariance = { {3.0, 2.0}, {2.0, 4.0} };
		 OpdfMultiGaussian omg = new OpdfMultiGaussian (mean,covariance);
		 //根据该分布生成10000个观测向量
		 ObservationVector[] obs = new ObservationVector[10000];
		 for (int i = 0; i < obs.length; i++)
			 obs[i] = omg.generate();
		 //找到适合这些观测的高斯分布
		 omg.fit(obs);
		 */
		/*OpdfIntegerFactory factory = new OpdfIntegerFactory(2);
		Hmm<ObservationInteger> hmm = new Hmm<ObservationInteger>(2, factory);
		    
		hmm.setPi(0, 0.95);
		hmm.setPi(1, 0.05);

		hmm.setOpdf(0, new OpdfInteger(new double[] { 0.95, 0.05 }));
		hmm.setOpdf(1, new OpdfInteger(new double[] { 0.2, 0.8 }));
		
		hmm.setAij (0, 1, 0.05);
		hmm.setAij (0, 0, 0.95);
		hmm.setAij (1, 0, 0.1);
		hmm.setAij (1, 1, 0.9);*/
	}
}
```




test.seq文件：保存成txt文件，结束要新行。



```
# A simple data file
[ 1.1 2.2 ];[ 4.4 5.5 ];[ 4.3 6.0 ];[ 7.7 8.8 ];
[ 0.5 1.5 ];[ 1.5 2.5 ];[ 4.5 5.5 ];[ 8.0 8.0 ];[ 7.0 8.0];
```




   从userguide.pdf中找出的代码不痛不痒，看来要仔细看源码才能知道中间算法和具体实例应用。
            


