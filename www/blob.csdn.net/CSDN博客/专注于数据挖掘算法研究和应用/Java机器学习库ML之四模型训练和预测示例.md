# Java机器学习库ML之四模型训练和预测示例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月22日 17:19:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3499
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









基于ML库机器学习的步骤：

1）样本数据导入；

2）样本数据特征抽取和特征值处理（结合模型需要归一化或离散化）；这里本文没有做处理，特征选择和特征值处理本身就很大；

3）样本集划分训练集和验证集；

4）根据训练集训练模型；

5）用验证集评价模型；

6）导入测试集，并用模型预测输出预测结果；



```java
package com.vip;

import java.io.File;

import be.abeel.util.Pair;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.KNearestNeighbors;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.DefaultDataset;
import net.sf.javaml.core.DenseInstance;
import net.sf.javaml.core.Instance;
import net.sf.javaml.featureselection.scoring.GainRatio;
import net.sf.javaml.sampling.Sampling;
import net.sf.javaml.tools.data.FileHandler;

public class VIPClassifer {
	 public static void main(String[] args)throws Exception {
		    if (args.length != 2) {
				System.err.println("Usage: 输入训练集和测试集路径");
				System.exit(2);
			}
	        /* Load a data set 前面13列是训练特征，最后1列标记*/
	        Dataset ori_data = FileHandler.loadDataset(new File(args[0]), 13, "\\s+");
            //特征评分，可独立
	        //GainRatio ga = new GainRatio();        
	        //ga.build(ori_data);  /* Apply the algorithm to the data set */    
	        //for (int i = 0; i < ga.noAttributes(); i++)  
	         //   System.out.println(ga.score(i));         
	        //抽样训练集和验证集
	        Sampling s = Sampling.SubSampling;
	        Pair<Dataset, Dataset> sam_data = s.sample(ori_data, (int) (ori_data.size() * 0.8));
	        /*
	        Dataset train_data = new DefaultDataset();//80%训练
	        Dataset test_data = new DefaultDataset();//20%验证
	        int sample=0;
	        for(Instance inst:ori_data){
	        	double[] values = new double[] { inst.value(5),inst.value(6),inst.value(7),inst.value(8), inst.value(9),inst.value(16),inst.value(17)};
	        	Instance train_inst = new DenseInstance(values, inst.classValue());        	
	        	if(sample<4){
	        		sample++;
	        		train_data.add(train_inst);
	        	}else {
	        	    sample=0;
	        		test_data.add(train_inst); 
	        	}       	
	        }*/
	        //Contruct a KNN classifier that uses 5 neighbors to make a decision.
	        Classifier knn = new KNearestNeighbors(5);
	        knn.buildClassifier(sam_data.x());
	        //验证集
	        int correct = 0, wrong = 0;
	        /* Classify all instances and check with the correct class values */
	        for (Instance inst : sam_data.y()) {
	            Object predictedClassValue = knn.classify(inst);
	            Object realClassValue = inst.classValue();
	            if (predictedClassValue.equals(realClassValue))
	                correct++;
	            else
	                wrong++;
	        }
	        System.out.println("Correct predictions  " + correct);
	        System.out.println("Wrong predictions " + wrong);
	        //模型预测
	        /* Load a data set 前面13列是训练特征，最后2列是uid和spuid联合标识*/
	        Dataset pre_data = FileHandler.loadDataset(new File(args[1]),"\\s+");
	        System.out.println(pre_data.instance(0));
	        Dataset out_data = new DefaultDataset();
	        for(Instance inst:pre_data){
	        	double[] values = new double[13]; 
	        	for(int i=0;i<13;i++) values[i]=inst.value(i);
	        	Instance pre_inst = new DenseInstance(values); //无标记，13列特征参与训练
	        	Object pre_classvalue = knn.classify(pre_inst);//预测结果
	        	//pre_inst.setClassValue(pre_classvalue);//标注预测结果
	        	double[] u_spu_id=new double[]{inst.value(13),inst.value(14)};
	        	Instance out_inst = new DenseInstance(u_spu_id,pre_classvalue); //带标记
	        	out_data.add(out_inst);
	        }
	        //输出u_Id+spu_id+action_type
	        FileHandler.exportDataset(out_data, new File("/data1/DataFountain/output.txt"));
	 }
}
//java -XX:-UseGCOverheadLimit -Xmx10240m -jar vip.jar train_features_new.txt test_features_new.txt
```


在上面这个代码框架内，可以用不同模型，如SVM、RF(随机森林）等，也可以对特征值进行处理后选择特征来训练。





