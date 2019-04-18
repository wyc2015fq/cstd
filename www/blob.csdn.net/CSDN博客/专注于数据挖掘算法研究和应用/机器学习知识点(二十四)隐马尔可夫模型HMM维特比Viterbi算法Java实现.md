# 机器学习知识点(二十四)隐马尔可夫模型HMM维特比Viterbi算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月16日 14:54:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3548
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









                
1、隐马尔可夫模型HMM

   学习算法，看中文不如看英文，中文喜欢描述的很高深。

   http://www.comp.leeds.ac.uk/roger/HiddenMarkovModels/html_dev/main.html

   里面有HMM定义、前向算法、维特比算法、后向算法。



2、Viterbi是隐马尔科夫模型中用于确定（搜索）已知观察序列在HMM下最可能的隐藏序列。

   Viterb采用了动态规划的思想，利用后向指针递归地计算到达当前状态路径中的最可能（局部最优）路径。

   要理解，看wiki的一个动态图。

   https://en.wikipedia.org/wiki/File:Viterbi_animated_demo.gif

   求解最可能的隐状态序列是HMM的三个典型问题之一，通常用维特比算法解决。

   维特比算法就是求解HMM上的最短路径（-log(prob)，也即是最大概率）的算法。



3、HMM五元组

   1)obs:观测序列，m个；

   2)states:隐状态,n个；

   3)start_p:初始概率（隐状态）

   4)trans_p:转移概率（隐状态），n*n矩阵，描述时间序列上的隐状态概率；

   5)emit_p: 发射概率 （隐状态表现为显状态的概率），n*m矩阵；

   要理解HMM模型的基本定义和三个基本问题。





4、Viterbi算法简单实现：

   https://github.com/hankcs/Viterbi/tree/master/src/com/hankcs/algorithm

   算法本身理解是一回事，着手代码又是一回事，所以对于能够代码动手实现的，是很值得学习的，至少表明其深入理解了。

  算法中的思路是从观测的序列得到最大概率的隐状态，关键就是这一时刻的隐状态能得出下一个时刻隐状态的概率以及这一时刻显状态的概率。



```java
package sk.ml;


/**
 * 维特比算法
 */
public class Viterbi {
	/**
     * 求解HMM模型
     * @param obs 观测序列
     * @param states 隐状态
     * @param start_p 初始概率（隐状态）
     * @param trans_p 转移概率（隐状态）
     * @param emit_p 发射概率 （隐状态表现为显状态的概率）
     * @return 最可能的序列
     */
    public static int[] compute(int[] obs, int[] states, double[] start_p, double[][] trans_p, double[][] emit_p)
    {
        double[][] V = new double[obs.length][states.length];
        int[][] path = new int[states.length][obs.length];

        for (int y : states)
        {
            V[0][y] = start_p[y] * emit_p[y][obs[0]];
            path[y][0] = y;
        }

        for (int t = 1; t < obs.length; ++t)
        {
            int[][] newpath = new int[states.length][obs.length];

            for (int y : states)
            {
                double prob = -1;
                int state;
                for (int y0 : states)
                {
                    double nprob = V[t - 1][y0] * trans_p[y0][y] * emit_p[y][obs[t]];
                    if (nprob > prob)
                    {
                        prob = nprob;
                        state = y0;
                        // 记录最大概率
                        V[t][y] = prob;
                        // 记录路径
                        System.arraycopy(path[state], 0, newpath[y], 0, t);
                        newpath[y][t] = y;
                    }
                }
            }

            path = newpath;
        }

        double prob = -1;
        int state = 0;
        for (int y : states)
        {
            if (V[obs.length - 1][y] > prob)
            {
                prob = V[obs.length - 1][y];
                state = y;
            }
        }

        return path[state];
    }
}
```




5、网络上对HMM有两个比较经典的案例，一个是天气，一个是诊断。



   诊断见知乎上文章：https://www.zhihu.com/question/20136144



```java
package sk.ml;

import static sk.ml.DoctorExample.Status.*;
import static sk.ml.DoctorExample.Feel.*;

public class DoctorExample {
	enum Status
    {
        Healthy,//健康
        Fever,//感冒
    }
    enum Feel
    {
        normal,//舒服
        cold,//冷
        dizzy,//头晕
    }
    static int[] states = new int[]{Healthy.ordinal(), Fever.ordinal()};
    static int[] observations = new int[]{normal.ordinal(), cold.ordinal(), dizzy.ordinal()};
    static double[] start_probability = new double[]{0.6, 0.4};
    static double[][] transititon_probability = new double[][]{
            {0.7, 0.3},
            {0.4, 0.6},
    };
    static double[][] emission_probability = new double[][]{
            {0.5, 0.4, 0.1},
            {0.1, 0.3, 0.6},
    };

    public static void main(String[] args)
    {
        int[] result = Viterbi.compute(observations, states, start_probability, transititon_probability, emission_probability);
        for (int r : result)
        {
            System.out.print(Status.values()[r] + " ");
        }
        System.out.println();
    }
}
```
执行结果：




`Healthy Healthy Fever `



   天气见这里面的例子：http://www.comp.leeds.ac.uk/roger/HiddenMarkovModels/html_dev/main.html





```java
package sk.ml;

import static sk.ml.WeatherExample.Weather.*;
import static sk.ml.WeatherExample.Activity.*;
public class WeatherExample {
	enum Weather//隐状态
    {
        Rainy,//下雨
        Sunny,//天晴
    }
    enum Activity //显状态
    {
        walk,//散步
        shop,//购物
        clean,//清洁
    }
    static int[] states = new int[]{Rainy.ordinal(), Sunny.ordinal()};
    static int[] observations = new int[]{walk.ordinal(), shop.ordinal(), clean.ordinal()};
    static double[] start_probability = new double[]{0.6, 0.4};//初始状态
    static double[][] transititon_probability = new double[][]{//转移矩阵
            {0.7, 0.3},
            {0.4, 0.6},
    };
    static double[][] emission_probability = new double[][]{//观测矩阵
            {0.1, 0.4, 0.5},
            {0.6, 0.3, 0.1},
    };

    public static void main(String[] args)
    {
        int[] result = Viterbi.compute(observations, states, start_probability, transititon_probability, emission_probability);
        for (int r : result)
        {
            System.out.print(Weather.values()[r] + " ");
        }
        System.out.println();
    }
}
```
执行结果：`Sunny Rainy Rainy `



理解算法最好的就是代码和数学公式两边对应。



