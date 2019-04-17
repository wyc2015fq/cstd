# Java机器学习库ML之五样本不均衡 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月23日 10:06:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2390
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









样本不均衡的问题是指训练集中类别之间的样本数据量比例失衡。有研究表明，部分学习任务，在类别出现1∶35比例时就会使某些分类学习方法无效，甚至1∶10比例也会。样本不均衡导致的问题有：

1）少数类所包含的信息就会很有限，从而难以确定少数类数据的分布，即在其内部难以发现规律，进而造成少数类的识别率低；

2）数据碎片，很多分类算法采用分治法，样本空间的逐渐划分会导致数据碎片问题，这样只能在各个独立的子空间中寻找数据的规律，对于少数类来说每个子空间中包含了很少的数据信息，一些跨空间的数据规律就不能被挖掘出来；

3）不恰当的归纳偏置，许多归纳推理系统在存在不确定时往往倾向于把样本分类为多数类。

一般解决方法有：

1、扩大数据集：当遇到类别不均衡问题时，增加样本数据是一个最简单的思路，有没有那么多样本就很难说了。

2、对数据进行重采样：对小类的数据样本进行采样来增加小类的数据样本个数，即过采样（over-sampling ，采样的个数大于该类样本的个数）。对大类的数据样本进行采样来减少该类数据样本的个数，即欠采样（under-sampling，采样的次数少于该类样本的个数）。

1）过抽样是抽样处理不平衡数据的最常用方法，基本思想就是通过改变训练数据的分布来消除或减小数据的不平衡。通过抽样方法通过增加少数类样本来提高少数类的分类性能，最简单的办法是简单复制少数类样本，缺点是可能导致过拟合，没有给少数类增加任何新的信息。改进的过抽样方法通过在少数类中加入随机高斯噪声或产生新的合成样本等方法。对svm作用待观察。

2）欠抽样方法通过减少多数类样本来提高少数类的分类性能，最简单的方法是通过随机地去掉一些多数类样本来减小多数类的规模，缺点是会丢失多数类的一些重要信息，不能够充分利用已有的信息。

3、尝试不同的分类学习算法

强烈建议不要对待每一个分类都使用自己喜欢而熟悉的分类算法。应该使用不同的算法对其进行比较，因为不同的算法使用于不同的任务与数据。决策树往往在类别不均衡数据上表现不错。它使用基于类变量的划分规则去创建分类树，因此可以强制地将不同类别的样本分开。目前流行的决策树算法有：C4.5、C5.0、CART和Random Forest等。



这里通过ML库作了一个Bagging法的思想来处理样本不均衡。bagging使用bootstrap抽样，其基本思想是：将一个弱学习算法使用多次（每次使用一个放回的重复抽样），并对结果进行投票，最后选取投票多的结果作为最终的结果（也可以通过得票的平均值用于连续值的预测）。bagging对使用的分类算法有要求，必须为不稳定的算法（不稳定是指数据集的小的变动能够使分类结果显著的变动）比如：决策树，神经网络算法、svm等。本文这里直接用KNN来做案例，参考代码如下：



```java
/**
 * This file is part of the Java Machine Learning Library
 * 
 * The Java Machine Learning Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * The Java Machine Learning Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with the Java Machine Learning Library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Copyright (c) 2006-2012, Thomas Abeel
 * 
 * Project: http://java-ml.sourceforge.net/
 * 
 */
package com.gddx;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
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

/**
 * This tutorial show how to use a the k-nearest neighbors classifier.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialKNN {
    /**
     * Shows the default usage of the KNN algorithm.
     */
    public static void main(String[] args)throws Exception {

        /* Load a data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");
        Sampling s = Sampling.SubSampling;
        int iretSam=10;//有放回重复采样10次
        List<String> lOut=new ArrayList<String>();
        while (iretSam>0){
        	Pair<Dataset, Dataset> sam_data = s.sample(data, (int) (data.size() * 0.8));
            Classifier knn = new KNearestNeighbors(5);
            knn.buildClassifier(sam_data.x());
            /* Counters for correct and wrong predictions. */
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
            //预测
            Dataset pre_data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\pre_iris.data"),",");
	        for(Instance inst:pre_data){
	        	double[] values = new double[4]; 
	        	for(int i=0;i<4;i++) values[i]=inst.value(i);
	        	Instance pre_inst = new DenseInstance(values); //无标记，4列特征参与训练
	        	Object pre_classvalue = knn.classify(pre_inst);//预测结果
	        	Object id=(int)inst.value(4);
	        	lOut.add(id+"|"+pre_classvalue);//输出id+标记
	        }
	        iretSam--;
        } 
        //重复采样和训练后，用投票法决定类别
        Map<String,Integer> mISe=new HashMap<String,Integer>();//Iris-setosa
        Map<String,Integer> mIVe=new HashMap<String,Integer>();//Iris-versicolor
        Map<String,Integer> mIVi=new HashMap<String,Integer>();//Iris-virginica
        for(String sout:lOut){ 	
        	String id=sout.split("\\|")[0];
        	String classvalue=sout.split("\\|")[1];
        	if(classvalue.equals("Iris-setosa")){
        		if(mISe.containsKey(id)){
        			int value=mISe.get(id);
        			value++;
        			mISe.put(id, value);
        		}else mISe.put(id, 1);
        	}else if (classvalue.equals("Iris-versicolor")){
        		if(mIVe.containsKey(id)){
        			int value=mIVe.get(id);
        			value++;
        			mIVe.put(id, value);
        		}else mIVe.put(id, 1);
        	}else if (classvalue.equals("Iris-virginica")){
        		if(mIVi.containsKey(id)){
        			int value=mIVi.get(id);
        			value++;
        			mIVi.put(id, value);
        		}else mIVi.put(id, 1);
        	}		
        }
        for(int i=1;i<=12;i++){  
        	String key=String.valueOf(i);
        	int mISe_value=0;
        	if (mISe.containsKey(key)) mISe_value= mISe.get(key);  
            int mIVe_value=0;
            if (mIVe.containsKey(key)) mIVe_value= mIVe.get(key); 
            int mIVi_value=0;
            if (mIVi.containsKey(key)) mIVi_value = mIVi.get(key); 
            if(mISe_value>=mIVe_value && mISe_value>=mIVi_value)
            	System.out.println("样本："+key+"的类别是：Iris-setosa");
            else if(mIVe_value>=mISe_value && mIVe_value>=mIVi_value)
            	System.out.println("样本："+key+"的类别是：Iris-versicolor");
            else if(mIVi_value>=mISe_value && mIVi_value>=mIVe_value)
            	System.out.println("样本："+key+"的类别是：Iris-virginica");
        }  
    }

}
```


 数据集到ML库官方下载：http://java-ml.sourceforge.net/content/databases



