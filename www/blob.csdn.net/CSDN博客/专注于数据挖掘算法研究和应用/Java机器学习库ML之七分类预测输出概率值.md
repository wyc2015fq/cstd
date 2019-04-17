# Java机器学习库ML之七分类预测输出概率值 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月26日 10:44:32[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2657








场景：一般分类预测直接输出类别标记，不过有些情况需要输出对应类别的概率值，比如判定为正例的概率是0.6，而判定为负例的概率是0.3，那自然标记为正例，这里就是看ML用classDistribution输出各类别的概率值。参考代码如下：



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
import java.util.Map;
import java.util.Random;

import be.abeel.util.Pair;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.tree.RandomForest;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.DenseInstance;
import net.sf.javaml.core.Instance;
import net.sf.javaml.sampling.Sampling;
import net.sf.javaml.tools.data.FileHandler;

/**
 * Tutorial for the random forest classifier.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialRandomForest {

    /**
     * Shows the default usage of the random forest algorithm.
     */
    public static void main(String[] args) throws Exception {
        /* Load a data set */
        Dataset ori_data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");
        Sampling s = Sampling.SubSampling;
        Pair<Dataset, Dataset> sam_data = s.sample(ori_data, (int) (ori_data.size() * 0.8));
        /*
         * Contruct a RF classifier that uses 5 neighbors to make a decision.
         */
        Classifier rf = new RandomForest(50, false, 3, new Random());
        rf.buildClassifier(sam_data.x());//80%样本训练

        /* 输出预测的类别概率 */
        for(Instance inst:sam_data.y()){  //20%样本验证
        	Map<Object,Double> mprob=rf.classDistribution(inst);//输出类别的概率，[0,1]
            System.out.println(mprob);    	
        }

    }

}
```


执行结果：



```
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=0.7400000000000003, Iris-versicolor=0.25999999999999995}
{Iris-virginica=0.0, Iris-setosa=1.0000000000000004, Iris-versicolor=0.0}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.08, Iris-setosa=0.0, Iris-versicolor=0.9200000000000005}
{Iris-virginica=0.46000000000000013, Iris-setosa=0.0, Iris-versicolor=0.5400000000000001}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=0.0, Iris-setosa=0.0, Iris-versicolor=1.0000000000000004}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=0.32, Iris-setosa=0.0, Iris-versicolor=0.6800000000000003}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=1.0000000000000004, Iris-setosa=0.0, Iris-versicolor=0.0}
{Iris-virginica=0.9000000000000005, Iris-setosa=0.0, Iris-versicolor=0.1}
```





