# Java机器学习库ML之十一线性SVM - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月27日 16:57:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1933








线性SVM的原理就不多说了，最强大的就是libsvm库（ml库也是用这个），参考：http://blog.csdn.net/fjssharpsword/article/details/53883340

这里直接给出ML库的示例代码：



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


import libsvm.SelfOptimizingLinearLibSVM;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.Instance;
import net.sf.javaml.tools.data.FileHandler;


/**
 * This tutorial show how to use a the LibSVM classifier.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialSelfOptimizingLibSVM {
    /**
     * Shows the default usage of the LibSVM algorithm.
     */
    public static void main(String[] args) throws Exception {

        /* Load a data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");
        /*
         * Contruct a LibSVM classifier with default settings.
         */
        Classifier svm = new SelfOptimizingLinearLibSVM();
        svm.buildClassifier(data);

        /*
         * Load a data set, this can be a different one, but we will use the
         * same one.
         */
        Dataset dataForClassification = FileHandler.loadDataset(new File("D:\\tmp\\javaml-0.1.7-src\\UCI-small\\iris\\iris.data"), 4, ",");
        /* Counters for correct and wrong predictions. */
        int correct = 0, wrong = 0;
        /* Classify all instances and check with the correct class values */
        for (Instance inst : dataForClassification) {
            Object predictedClassValue = svm.classify(inst);
            Object realClassValue = inst.classValue();
            if (predictedClassValue.equals(realClassValue))
                correct++;
            else
                wrong++;
        }
        System.out.println("Correct predictions  " + correct);
        System.out.println("Wrong predictions " + wrong);

    }

}
```


发现一个linearsvm的网站http://www.linearsvm.com/，说可以处理超大数据集，可以试验下。



对ML库的序列学习就基本到此，总结三点：

1）还是python scikit-learn好用，其次是spark mlib库，java ml还是有所缺的；

2）Java ML库的源码可以继续研究，对于机器学习库主要就是三个部分：数据处理、方法实现、模型评价；

3）Java ML库的API可以参考：http://java-ml.sourceforge.net/api/0.1.7/，但是这个说明文档实在是过于简单。



