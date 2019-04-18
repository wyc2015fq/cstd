# Java机器学习库ML之一Dataset和Instance - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月21日 14:55:18[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3324








Java机器学习库ML官网：http://java-ml.sourceforge.net/

对于一个机器学习库来说，最基础就是数据处理能力，ml库给了dataset和instance两个类，dataset是矩阵，instance是行（可以理解是一个list，或一个double数组）。

本文给出最基本的Dataset和Instance操作，可以完成对一个矩阵的遍历，定位到每一行里的每一个列，可惜的是dataset本身好像不具备行列切片，这个和python里面的pandas库差别就大了。

参考代码如下：



```java
package com.gddx;

import java.io.File;
import java.io.IOException;

import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.DefaultDataset;
import net.sf.javaml.core.DenseInstance;
import net.sf.javaml.core.Instance;
import net.sf.javaml.core.SparseInstance;
import net.sf.javaml.tools.InstanceTools;
import net.sf.javaml.tools.data.FileHandler;

public class TutorialDataset {

   public static void main(String[]args) throws IOException{
	    //dataset
	    //创建一个空的dataset，并随机赋值
        Dataset data = new DefaultDataset();
        for (int i = 0; i < 5; i++) {
            Instance tmpInstance = InstanceTools.randomGaussianInstance(3);
            tmpInstance.setClassValue("标记");
            data.add(tmpInstance);
        }//创建一个5行3列的矩阵
        System.out.println(data.instance(0)); //打印dataset的第一行
        /*案例2*/
        //从文件中导入形成一个dataset，前4列是特征值，最后1列是标记，列分隔符是逗号
        Dataset dataFile = FileHandler.loadDataset(new File("D:\\tmp\\iris.data"), 4, ",");
        for(Instance inst:dataFile){
        	System.out.println(inst.classValue());//显示标记
        	System.out.println(inst.values());//显示特征值
        }
        //instance      
        double[] values = new double[] { 0.1, 2, 3 };/* values of the attributes. */
        Instance instance = new DenseInstance(values);
        System.out.println("Instance with only values set: ");
        System.out.println(instance);
        Instance instanceWithClassValue = new DenseInstance(values, 1);
        System.out.println("Instance with class value set to 1: ");
        System.out.println(instanceWithClassValue);
        /* Create instance with 10 attributes */
        Instance instancesparse = new SparseInstance();
        /* Set the values for particular attributes */
        instancesparse.put(1, 1.0);
        instancesparse.put(2, 2.0);
        instancesparse.put(3, 4.0);
        System.out.println(instancesparse.values());
   }	
}
```





