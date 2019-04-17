# Java机器学习库ML之二Feature Selection（特征选择） - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月21日 15:25:07[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3111








机器学习中训练模型的前提必备工作就是特征选择，起到降维和降低计算开销的目的，当然在获取尽可能小的特征子集的同时，应不显著降低分类精度、不影响类分布、保持特征子集的稳定适应性强等。

ML库提供了特征选择方法，具体有：

1、递归特征消除 Recursive feature elimination (RFE)：

递归特征消除的主要思想是反复的构建模型（如SVM或者回归模型）然后选出最好的（或者最差的）的特征（可以根据系数来选），把选出来的特征排除，然后在剩余的特征上重复这个过程，直到所有特征都遍历了。这个过程中特征被消除的次序就是特征的排序。因此，这是一种寻找最优特征子集的贪心算法。参考代码如下：




```java
package com.gddx;

import java.io.File;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.featureselection.ranking.RecursiveFeatureEliminationSVM;
import net.sf.javaml.tools.data.FileHandler;

public class TutorialFeatureRanking {
    /**
     * Shows the basic steps to create use a feature ranking algorithm.
     * 
     * @author Thomas Abeel
     * 
     */
    public static void main(String[] args) throws Exception {
        /* Load the iris data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"), 4, ",");
        /* Create a feature ranking algorithm */
        RecursiveFeatureEliminationSVM svmrfe = new RecursiveFeatureEliminationSVM(0.2);
        /* Apply the algorithm to the data set */
        svmrfe.build(data);
        /* Print out the rank of each attribute */
        for (int i = 0; i < svmrfe.noAttributes(); i++)
            System.out.println(svmrfe.rank(i));
    }

}
```


2、Pearson相关系数 Pearson Correlation


皮尔森相关系数是体现特征和响应变量之间关系的方法，该方法衡量的是变量之间的线性相关性，结果的取值区间为[-1，1]，-1表示完全的负相关(这个变量下降，那个就会上升)，+1表示完全的正相关，0表示没有线性相关。参考代码如下：





```java
package com.gddx;

import java.io.File;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.distance.PearsonCorrelationCoefficient;
import net.sf.javaml.featureselection.subset.GreedyForwardSelection;
import net.sf.javaml.tools.data.FileHandler;

/**
 * Shows the basic steps to create use a feature subset selection algorithm.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialFeatureSubsetSelection {

    public static void main(String[] args) throws Exception {
        /* Load the iris data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"), 4, ",");
        /*
         * Construct a greedy forward subset selector that will use the Pearson
         * correlation to determine the relation between each attribute and the
         * class label. The first parameter indicates that only one, i.e. 'the
         * best' attribute will be selected.
         */
        GreedyForwardSelection ga = new GreedyForwardSelection(1, new PearsonCorrelationCoefficient());
        /* Apply the algorithm to the data set */
        ga.build(data);
        /* Print out the attribute that has been selected */
        System.out.println(ga.selectedAttributes());
    }
}
```



3、集成特征选择

    基于模型排序后的集成，参考代码如下：



```java
package com.gddx;

import java.io.File;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.featureselection.ensemble.LinearRankingEnsemble;
import net.sf.javaml.featureselection.ranking.RecursiveFeatureEliminationSVM;
import net.sf.javaml.tools.data.FileHandler;

/**
 * Tutorial to illustrate ensemble feature selection.
 * 
 * @author Thomas Abeel
 * 
 */
public class TutorialEnsembleFeatureSelection {
    /**
     * Shows the basic steps to use ensemble feature selection
     * 
     * @author Thomas Abeel
     * 
     */
    public static void main(String[] args) throws Exception {
        /* Load the iris data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"), 4, ",");
        /* Create a feature ranking algorithm */
        RecursiveFeatureEliminationSVM[] svmrfes = new RecursiveFeatureEliminationSVM[10];
        for (int i = 0; i < svmrfes.length; i++)
            svmrfes[i] = new RecursiveFeatureEliminationSVM(0.2);
        LinearRankingEnsemble ensemble = new LinearRankingEnsemble(svmrfes);
        /* Build the ensemble */
        ensemble.build(data);
        /* Print out the rank of each attribute */
        for (int i = 0; i < ensemble.noAttributes(); i++)
            System.out.println(ensemble.rank(i));
    }

}
```




4、特征评分：



```java
package com.gddx;

import java.io.File;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.featureselection.scoring.GainRatio;
import net.sf.javaml.tools.data.FileHandler;

public class TutorialFeatureScoring {
    /**
     * Shows the basic steps to create use a feature scoring algorithm.
     * 
     * @author Thomas Abeel
     * 
     */
    public static void main(String[] args) throws Exception {
        /* Load the iris data set */
        Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"), 4, ",");

        GainRatio ga = new GainRatio();
        /* Apply the algorithm to the data set */
        ga.build(data);
        /* Print out the score of each attribute */
        for (int i = 0; i < ga.noAttributes(); i++)
            System.out.println(ga.score(i));
    }

}
```


5、WekaAttributeSelection，这个主要还是用增益来选择特征，应该在输出上包括排序和分数，参考代码如下：



```java
package com.gddx;

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
import java.io.File;
import java.io.IOException;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.tools.data.FileHandler;
import net.sf.javaml.tools.weka.WekaAttributeSelection;
import weka.attributeSelection.ASEvaluation;
import weka.attributeSelection.ASSearch;
import weka.attributeSelection.GainRatioAttributeEval;
import weka.attributeSelection.Ranker;

/**
 * Tutorial how to use the Bridge to WEKA AS Evaluation , AS Search and
 * Evaluator algorithms in Java-ML
 * 
 * 
 * @author Irwan Krisna
 */
public class TutorialWekaAttributeSelection {

	public static void main(String[] args) throws IOException {
		/* Load data */
		Dataset data = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"),4, ",");
		/* Create a AS Evaluation algorithm */
		ASEvaluation eval = new GainRatioAttributeEval();
		/* Create a Weka's AS Search algorithm */
		ASSearch search = new Ranker();
		/* Wrap Wekas' Algorithms in bridge */
		WekaAttributeSelection wekaattrsel = new WekaAttributeSelection(eval,search);
		/*
		 * to apply algorithm to the data set and generate the new data based on
		 * the given parameters
		 */
		wekaattrsel.build(data);
		/* to retrieve the number of attributes */
		System.out.println("Total number of attributes:  "+ wekaattrsel.noAttributes());
		/* to display all the rank and score for each attribute */
		for (int i = 0; i < wekaattrsel.noAttributes() - 1; i++) {
			System.out.println("Attribute  " + i + "  Ranks  "
					+ wekaattrsel.rank(i) + " and Scores "
					+ wekaattrsel.score(i));
		}

	}

}
```
















