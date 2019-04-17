# 机器学习知识点(十六)集成学习AdaBoost算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月13日 16:59:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2314
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









理解http://blog.csdn.net/fjssharpsword/article/details/61913092中AdaBoost算法，从网上找了一套简单的代码加以理解。

1、基分类器，实现一个简单的分类

1）对象类



```java
package sk.adaboost;

public class Instance {

	public double[] dim;	//各个维度值
	public int label;		//类别标号
	
	public Instance(double[] dim, int label) {
		this.dim = dim;
		this.label = label;
	}
}
```


2）接口类



```java
package sk.adaboost;

public abstract class Classifier {

	public double errorRate;
	public int errorNumber;
	
	public abstract int classify(Instance instance) ;

}
```




3）基学习器算法



```java
package sk.adaboost;

public class SimpleClassifier extends Classifier{

	double threshold ;	//分类的阈值
	int dimNum;			//对哪个维度分类
	int fuhao = 1;		//对阈值两边的处理
	
	public int classify(Instance instance) {
		
		if(instance.dim[dimNum] >= threshold) {
			return fuhao;
		}else {
			return -fuhao;
		}
	}
	
	/**
	 * 训练出threshold和fuhao
	 * @param instances
	 * @param W 样例的权重
	 * @param dim 对样例的哪个维度进行训练
	 */
	public void train(Instance[] instances, double[] W, int dimNum) {
		
		errorRate = Double.MAX_VALUE;
		this.dimNum = dimNum;
		double adaThreshold = 0;
		int adaFuhao = 0;
		for(Instance instance : instances) {
			threshold = instance.dim[dimNum];
			for(int fuhaoIt = 0; fuhaoIt < 2; fuhaoIt ++) {
				fuhao = -fuhao;
				double error = 0;
				int errorNum = 0;
				for(int i = 0; i< instances.length; i++) {
					if(classify(instances[i]) != instances[i].label) {
						error += W[i];
						errorNum++;
					}
				}
				if(errorRate > error){
					errorRate = error;
					errorNumber = errorNum;
					adaThreshold = threshold;
					adaFuhao = fuhao;
				}
			}
		}
		threshold = adaThreshold;
		fuhao = adaFuhao;
	}
}
```


4）adaboost集成学习算法



```java
package sk.adaboost;

import java.util.ArrayList;
import java.util.List;

public class Adaboost {

	Instance[] instances;
	List<Classifier> classifierList = null;	//各个弱分类器
	List<Double> alphaList = null;			//每个弱分类器的权重
	
	public Adaboost(Instance[] instances) {
		
		this.instances = instances;
	}
	
	public List<String> adaboost(int T) {//T个基学习器
		
		int len = this.instances.length;
		double[] W = new double[len];	//初始权重
		for(int i = 0; i < len; i ++) {
			W[i] = 1.0 / len;
		}
		classifierList = new ArrayList<Classifier>();
		alphaList = new ArrayList<Double>();
		List<String> iHP=new ArrayList<String>();
		for(int t = 0; t < T; t++) {//T轮
			Classifier cf = getMinErrorRateClassifier(W);
			classifierList.add(cf);
			double errorRate = cf.errorRate;
			//计算弱分类器的权重
			double alpha = 0.5 * Math.log((1 - errorRate) / errorRate);
			alphaList.add(alpha);
			//更新样例的权重
			double z = 0;
			for(int i = 0; i < W.length; i++) {
				W[i] = W[i] * Math.exp(-alpha * instances[i].label * cf.classify(instances[i]));
				z += W[i];
			}
			for(int i = 0; i < W.length; i++) {//规范化因子
				W[i] /= z;
			}
			iHP.add(String.valueOf(getErrorCount()));//预测结果插入
		}
		return iHP;
	}
	
	private int getErrorCount() {
		
		int count = 0;
		for(Instance instance : instances) {
			if(predict(instance) != instance.label)
				count ++;
		}
		return count;
	}
	
	/**
	 * 预测
	 * @param instance
	 * @return
	 */
	public int predict(Instance instance) {
		
		double p = 0;
		for(int i = 0; i < classifierList.size(); i++) {
			p += classifierList.get(i).classify(instance) * alphaList.get(i);
		}
		if(p > 0) return 1;
		return -1;
	}

	/**
	 * 得到错误率最低的分类器
	 * @param W
	 * @return
	 */
	private Classifier getMinErrorRateClassifier(double[] W) {
		
		double errorRate = Double.MAX_VALUE;
		SimpleClassifier minErrorRateClassifier = null;
		int dimLength = instances[0].dim.length;
		for(int i = 0; i < dimLength; i++) {
			SimpleClassifier sc = new SimpleClassifier();
			sc.train(instances, W, i);//基学习器训训练
			if(errorRate > sc.errorRate){
				errorRate  = sc.errorRate;
				minErrorRateClassifier = sc;
			}
		}
		return minErrorRateClassifier;
	}
	
}
```


这里面几个重要步骤要清晰：public List<String> adaboost(int T)算法中，先初始化权重然后开始T轮的基学习器算法执行和权重更新，private Classifier getMinErrorRateClassifier(double[] W)开展基学习器训练并返回错误率，返回的错误计算权重并更新分布。

5）测试adaboost算法，采用投票法的结合策略输出集成学习预测结果



```java
package sk.adaboost;

import java.util.List;

public class AdaboostTest {

public static void main(String[] args) {
		//模拟数据
		double[] ins1 = {0,3};
		double[] ins2 = {1,3};
		double[] ins3 = {2,3};
		double[] ins4 = {3,1};
		double[] ins5 = {4,1};
		double[] ins6 = {5,1};
		double[] ins7 = {6,3};
		double[] ins8 = {7,3};
		double[] ins9 = {8,0};
		double[] ins10 = {9,1};
		
		Instance instance1 = new Instance(ins1, 1);
		Instance instance2 = new Instance(ins2, 1);
		Instance instance3 = new Instance(ins3, 1);
		Instance instance4 = new Instance(ins4, -1);
		Instance instance5 = new Instance(ins5, -1);
		Instance instance6 = new Instance(ins6, -1);
		Instance instance7 = new Instance(ins7, 1);
		Instance instance8 = new Instance(ins8, 1);
		Instance instance9 = new Instance(ins9, 1);
		Instance instance10 = new Instance(ins10, -1);
		
		Instance[] instances = {instance1, instance2, instance3, instance4, instance5, instance6, instance7, instance8, instance9, instance10 };
		//集成学习，串行，基学习器之间存在强依赖关系
		Adaboost ab = new Adaboost(instances);
		List<String> iHP=ab.adaboost(10);
		//输出预测结果，根据多数头投票法的结合策略
		int pcount=0,ncount=0;
		for(String hp:iHP){
			if (hp.equals("1")) pcount++;//预测为正例的数字
			if (hp.equals("0")) ncount++;//预测为正例的数字
		}
		if (pcount>=ncount) System.out.println("1");
		else System.out.println("0");
	}
}
```


6）助于理解算法本身，实际应用中基学习器可以换成其他算法。
            


