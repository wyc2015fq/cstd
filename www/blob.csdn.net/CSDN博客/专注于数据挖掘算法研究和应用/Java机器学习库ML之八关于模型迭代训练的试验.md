# Java机器学习库ML之八关于模型迭代训练的试验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月27日 14:38:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1995








前文提到因为数据集过大，只能拆分然后依次迭代训练，实验发现对结果有所偏差，参考代码如下：



```java
package com.vip;

import java.io.File;
import java.util.Map;

import be.abeel.util.Pair;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.KNearestNeighbors;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.DefaultDataset;
import net.sf.javaml.core.DenseInstance;
import net.sf.javaml.core.Instance;
import net.sf.javaml.sampling.Sampling;
import net.sf.javaml.tools.data.FileHandler;

public class VIPClassifer {
	 public static void main(String[] args)throws Exception {	 
		    if (args.length != 1) {
				System.err.println("Usage: 请输入路径。。。");
				System.exit(2);
			}				    
		    //第一步：迭代加载样本并采样和训练模型
		    Sampling s = Sampling.SubSampling;   
		    Classifier knn = new KNearestNeighbors(2);  
            Dataset ds_validate=new DefaultDataset();  
            for(int i=0;i<=11;i++){//分成11份 
            	String filePath=args[0]+"/sam_"+String.format("%02d", i);  
            	Dataset ori_data = FileHandler.loadDataset(new File(filePath), 12, "\\s+"); //前面12列是训练特征，最后1列标记 
            	//抽样训练集和验证集           
                Pair<Dataset, Dataset> sam_data = s.sample(ori_data, (int) (ori_data.size() * 0.9));            
                knn.buildClassifier(sam_data.x());//样本集追加训练  
                for(Instance inst:sam_data.y()){//加入验证集  
                    ds_validate.add(inst);  
                }  
                System.out.println("训练完成第"+String.valueOf(i)+"份样本集");  
            }
            //第二步：利用验证集验证模型
            System.out.println("开始验证。。。");
	        int correct = 0, wrong = 0;
	        for (Instance inst : ds_validate) {
	            Object predictedClassValue = knn.classify(inst);
	            Object realClassValue = inst.classValue();
	            if (predictedClassValue.equals(realClassValue)) correct++;
	            else wrong++;
	        }
	        System.out.println("Correct predictions  " + correct);
	        System.out.println("Wrong predictions " + wrong);
	        //基于训练出的模型预测
	        System.out.println("开始测试。。。");
	        Dataset out_data = new DefaultDataset();    
	        for(int i=0;i<=59;i++){//分成59份 
	        	String filePath=args[0]+"/lim_"+String.format("%02d", i);
	        	//加载测试集，前面13列是训练特征，最后2列是uid和spuid联合标识
	        	Dataset pre_data = FileHandler.loadDataset(new File(filePath),"\\s+");
	        	for(Instance inst:pre_data){
	        		double[] values = new double[12]; 
		        	for(int j=0;j<11;j++) values[j]=inst.value(j);
		        	values[11]=inst.value(12);//第12列不参与训练
		        	Instance pre_inst = new DenseInstance(values); //无标记，12列特征参与训练
		        	Map<Object,Double> classProb=knn.classDistribution(pre_inst);//获取类别概率
		        	double[] pprob=new double[]{classProb.get("1")};//获取正例的概率
		        	Instance out_inst = new DenseInstance(pprob); //输出
		        	out_data.add(out_inst);
		        	System.out.println("测试完成第"+String.valueOf(i)+"份样本集"); 
	        	}
	        }
	        //输出u_Id+spu_id+action_type
	        FileHandler.exportDataset(out_data, new File(args[0]+"/output.txt"));
	 }
}
```


代码执行结果就是验证集发现，共有10万验证集，只有2个是正确的，具体原因不知道，可见通过拆分训练集来训练的方法似乎不可行。

另外发现处理特别慢，尤其是单条记录验证和预测时，对于模型训练dataset导入倒是很快，很不解。单条预测和验证一秒感觉只有几条，对于几百条记录来说，实在运行太久了。这个不知道ML库是怎么弄的。

如何解决大样本集的训练，一个是python支持比较好，一个就是spark mlib平台。

对于学习任务来说，总结三方面的工作：

1)样本：均衡采样、样本选择、交叉验证；

  2)特征：特征挖掘、特征选择、特征值处理；

  3)模型：参数调优、模型选择、学习方法；







