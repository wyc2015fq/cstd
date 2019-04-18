# Java机器学习库ML之九交叉验证法(Cross Validation) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月27日 15:52:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2574
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









交叉验证(Cross Validation,CV)是用来验证分类器的性能一种统计分析方法，基本思想是把在某种意义下将原始数据(dataset)进行分组,一部分做为训练集(train set),另一部分做为验证集(validation set)。首先用训练集对分类器进行训练，再利用验证集来测试训练得到的模型(model)，以此来做为评价分类器的性能指标。常见CV的方法如下:

1)Hold-Out Method



将原始数据随机分为两组,一组做为训练集,一组做为验证集,利用训练集训练分类器,然后利用验证集验证模型,记录最后的分类准确率为此Hold-OutMethod下分类器的性能指标.此种方法的好处的处理简单,只需随机把原始数据分为两组即可,其实严格意义来说Hold-Out Method并不能算是CV,因为这种方法没有达到交叉的思想,由于是随机的将原始数据分组,所以最后验证集分类准确率的高低与原始数据的分组有很大的关系,所以这种方法得到的结果其实并不具有说服性.


2)K-fold Cross Validation(记为K-CV)

将原始数据分成K组(一般是均分),将每个子集数据分别做一次验证集,其余的K-1组子集数据作为训练集,这样会得到K个模型,用这K个模型最终的验证集的分类准确率的平均数作为此K-CV下分类器的性能指标.K一般大于等于2,实际操作时一般从3开始取,只有在原始数据集合数据量小的时候才会尝试取2.K-CV可以有效的避免过学习以及欠学习状态的发生,最后得到的结果也比较具有说服性.


3).Leave-One-Out Cross Validation(记为LOO-CV)


如果设原始数据有N个样本,那么LOO-CV就是N-CV,即每个样本单独作为验证集,其余的N-1个样本作为训练集,所以LOO-CV会得到N个模型,用这N个模型最终的验证集的分类准确率的平均数作为此下LOO-CV分类器的性能指标.相比于前面的K-CV,LOO-CV有两个明显的优点:

①a.每一回合中几乎所有的样本皆用于训练模型,因此最接近原始样本的分布,这样评估所得的结果比较可靠。


②b.实验过程中没有随机因素会影响实验数据,确保实验过程是可以被复制的。


但LOO-CV的缺点则是计算成本高,因为需要建立的模型数量与原始数据样本数量相同,当原始数据样本数量相当多时,LOO-CV在实作上便有困难几乎就是不显示,除非每次训练分类器得到模型的速度很快,或是可以用并行化计算减少计算所需的时间.





ML库中的交叉验证法，应该是采用K折交叉验证法，代码示例如下：



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

import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.KNearestNeighbors;
import net.sf.javaml.classification.evaluation.CrossValidation;
import net.sf.javaml.classification.evaluation.PerformanceMeasure;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.tools.data.FileHandler;

/**
 * This tutorial shows how you can do cross-validation with Java-ML
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialCrossValidation {
    /**
     * Default cross-validation with little options.
     */
    public static void main(String[] args) throws Exception {
        /* Load data */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");
    	//Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\train_features_npequal.txt"), 12, "\\s+");
        /* Construct KNN classifier */
        Classifier knn = new KNearestNeighbors(2);
        /* Construct new cross validation instance with the KNN classifier */
        CrossValidation cv = new CrossValidation(knn);
        /* Perform 5-fold cross-validation on the data set */
        Map<Object, PerformanceMeasure> p = cv.crossValidation(data);

        System.out.println("Accuracy=" + p.get("Iris-virginica").getAccuracy());
        System.out.println(p);

    }
}
```








