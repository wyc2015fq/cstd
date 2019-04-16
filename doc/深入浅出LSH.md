[深入浅出LSH](http://www.cppblog.com/humanchao/archive/2018/02/24/215521.html)

通过这篇文章我们主要回答以下几个问题：

 

\1.    LSH解决问题的背景，即以图片相似性搜索为例，如何解决在海量数据中进行相似性查找？

\2.    图像相似性查找的连带问题：相似性度量，特征提取；

\3.    LSH的数学分析，即局部敏感HASH函数的数学原理，通过与、或构造提升查找的查全率与查准率

\4.    LSH具体实现，对着代码一步一步学习算法的具体实现

 

一、问题背景介绍

 

LSH即Locality Sensitive hash，用来解决在海量数据中进行相似性查找的问题，以图片搜索为例，见下图，用户向百度图搜提交待搜索图片，百度图搜将相似的图片返回展示。

 ![img](http://www.cppblog.com/images/cppblog_com/humanchao/1.png)

要实现这样一个应用，需要考虑以下几个问题：

 

\1.    提取用户提交的图片特征，即将提交图片转换成一个特征向量；

\2.    预先将所有收集到的图片进行特征提取；

\3.    把步骤1中提取的特征与步骤2中的特征集合，进行比对，并记录比对的相似性值；

\4.    对相似值由大到小排序，返回相似度最高的作为检索结果。

 

问题1，2是一个图片特征提取的问题，与LSH无关。LSH主要解决问题3与问题4，在海量特征集中找到相似特征。以下简述问题1，2，详细讲解问题3，4。

 

二、图像的特征提取

 

图像的特征提取属于个图像学研究领域，有很多经典的图像算法，分别从图像颜色，角度，形态不同角度进行图像的特征描述，包括最近流行的深度学习方法也可以做类似的事情。这里介绍一种比较容易理解的2种方法，一种是传统的颜色直方图，一种是自编码器。

 

\1.    颜色直方图就是将RGB图像中出现的颜色进行统计，将一张图像描述中一个256维度的特征向量，如下图所示：

 ![img](http://www.cppblog.com/images/cppblog_com/humanchao/2.gif)

\2.    自编码器的思想是通过神经网络进行特征提取，提取出针对学习样本的通用特征降维方法，用训练得到的统一方法对待测图像进行降维处理，得到一个特征向量。

 ![img](http://www.cppblog.com/images/cppblog_com/humanchao/3.jpg)

如图所示，神经网络的2端通过相同的数据限制，学习到中间的隐藏层权重，通过使用降维再升维的方法，使隐藏层输出最大限度的保存图像的主要特征，以使还原后的图像与原图像误差达到最小。

 

自编码器完成训练后，使用输入层到隐藏层的权重向量进行特征提取，将图像数据作为输入层输入，将隐藏层输出作为图像特征，以此作为图像的特征。

 

**三、****特征的相似性比较**

 

相似是一个形容词，对于一对图像使用不同的评价标准可能得到不同相似度量值。比如对于不同的业务场景，如相貌识别、形态识别、背景识别所采用相似度量方法可能完全是不同的。

 

同样从数学的角度进行特征的相似性比较也有不同的方法，如果把特征向量看成空间的一个点，那么可以把度量2个向量的相似性看做度量多维空间中的2个点的距离，这些度量方法包括：

 

**欧式距离![img](http://www.cppblog.com/images/cppblog_com/humanchao/4.png)**

**Jaccard****距离**

![img](http://www.cppblog.com/images/cppblog_com/humanchao/5.png)

**余弦距离**

![img](http://www.cppblog.com/images/cppblog_com/humanchao/6.png)

**曼哈顿距离**

![img](http://www.cppblog.com/images/cppblog_com/humanchao/7.png)

**闵可夫斯基距离**

![img](http://www.cppblog.com/images/cppblog_com/humanchao/8.png)

 

作为一个距离的度量函数d，必须满足以下一些性质

•      d(x,y)>=0,距离非负性

•      d(x,y)=0当切仅当x=y,只有点到自身的距离为0，其他距离都大于0

•      d(x,y)=d(y,x),距离对称性

•      d(x,y)<=d(x,z)+d(z,y),满足三角不等式

 

当完成了第1，第2步以后，即能提取图像的特征，又找到了方法可以度量图像特征的相似后，剩下的事情就是要预先将图像库的所有图像一一提取特征，在搜索时用同样的特征提取方法对待测图像提取特征，然后将待搜特征循环与图像库中的图像特征集合比较，找到相似度最高的。

 

四、局部敏感hash

 

由于图像库的特征通常数量很大，循环比较的时间复杂度过高，无法满足应用的响应需要。因此，必须要使用类似的索引技术来降低搜索的复杂性，多维索引有一种叫KD树的技术可以做类似的事情，但是对于图像搜索的场景另外一种LSH的技术更有用。

 

回顾问题的背景，现在已经预先将图像库中的图像都转换成了特征并组成特征集合（所有特征的维度都是相同的）：

 

[[20,….,………….61],

…,

[[31,….,………….55]]

 

同时也用相同的特征提取算法提取了用户提交图像的特征，如：

[1,2,3,4,5,6,….,60]

现在的问题是从第一个特征集中找到与用户图像特征相似度最高的一些特征。

 

用一个p稳定分布（p-stable distribution）定义如下：

•      **定义：对于一个实数集R上的分布D，如果存在P>=0，对任何n个实数v1,…,vn和n个满足D分布的变量X1,…,Xn，随机变量ΣiviXi（****向量点乘，一个整数）****和(Σi|vi|p)1/pX****（p(阶)范数，一个整数）****有相同的分布，其中X是服从D分布的一个随机变量，则称D为 一个p-稳定分布。比如p=1是柯西分布，p=2时是高斯分布。**

 

这里面有几个基本问题必须要进行背景了解，向量点乘、范数、正态分布、同分布。

向量点乘：2个向量相同维度相乘，再相加。

范数：是具有“长度”概念的函数。向量的0范数，向量中非零元素的个数。向量的1范数，为绝对值之和。向量的2范数，就是通常意义上的模。

正态分布：这个太长见了，不多解释。

同分布：2个数列同分布，意味着呈线性关系的两个数列，可以理解为同比例缩放。

 

用通俗的语言进行解析上述定义就是：

 

取满足正态分布的一个数列，与某个特征向量做向量点乘，得到一个数值，这个数值与这个特征向量本身的2阶范数（即向量每一维的元素绝对值的平方和再开方，代表向量的长度，也是一个数值），有同分布的性质。

 

假设有两个图像特征X1,X2，用这两个特征分别与同一个正态分布的数列做向量点乘，所得到的2个数值在一维上的距离与X1,X2在多维上的欧氏距离是同分布的。

 

设正态分布数值为D，待计算的两个图像特征为X1,X2，则有：

|X1.D-X2.D| = (X1 – X2).D 同分布于 (X1 – X2) 的2阶范数(即长度)。

(X1 – X2)是向量相减，得到一个新的向量，代表两个特征的距离向量。

 

 

用图像库中N个图像的N个特征分别与同一个正态分布的数列做向量点乘，得到的N个特征在一维上的点，我们用在一维上的点之间的距离度量多维空间的距离，当然这种相近是概率意义下的相近。为了简化计算，把一维上的线划分成段；为了提高算法的稳定性，向量点乘后增加一个随机的噪音。于是得到了一个新的哈希函数：

![img](http://www.cppblog.com/images/cppblog_com/humanchao/9.png)

a是p稳定生成的随机数列

b是(0,r)里的随机数, noise/随机噪音

r为直线上分段的段长

V待计算特征向量，即图像提取出来的特征

像这样一类hash函数称之为**局部敏感hash函数**，下面给出局部敏感hash函数的定义：

**将这样的一族hash函数 H={h:S→U} 称为是(d1,d2,p1,p2)敏感的，如果对于任意H中的函数h，满足以下2个条件：**

–     **如果d(O1,O2)<d1，那么Pr[h(O1)=h(O2)]≥p1**

–     **如果d(O1,O2)>d2，那么Pr[h(O1)=h(O2)]≤p2**

**其中，O1,O2∈S，表示两个具有多维属性的数据对象，d(O1,O2)为2个对象的相异程度，也就是相似度。**

通俗的讲就是：

•      N个值足够相似时

–     映射为同一hash值的概率足够大；

•      N个值不足够相似时

–     映射为同一hash值的概率足够小

可以用下图来表示：

![img](http://www.cppblog.com/images/cppblog_com/humanchao/10.png)

局部敏感HASH函数必要要满足如下条件：

[1]   进行相似性搜索时，在概率上可以保证返回值的相似性

[2]   函数族中的各函数，返回值在概率上相互**独立，概率独立可以带来以下好处**

a)     统计独立，满足叠加多个函数，提高查准率

b)     通过组合能够避免伪正例、伪反例

[3]   搜索效率

a)     hash比对比线性查询快

 

**五、****概率相似度度量与调节（与构造与或构造）**

既然是在概率意义下相似性度量，必然会存在着相近样本被hash到不同的hash值情况，同时也必然会存在不相近的样本被hash到相同的hash值情况，前一种称为伪反例，向一种称为伪正例。

伪正例通过与构造解决，即通过多个hash函数，计算同一个特征的多个hash值，只有当多个hash函数均相同时，才认为特征相似。这有效避免了不相似的特征被判定为相似特征的情况。

伪反例通过或构造解决，即通过多个hash函数，计算同一个特征的多个hash值，只有多个hash函数有一个hash值相同时，即认为特征相似。这有效避免了相似的特征被判定为不相似特征的情况。

结合与构造与或构造2种方案，可以生成r*b个函数，每r个hash函数带表一组与构造，每b组hash函数族代表一组或构造，当满足一个或构造后特征判定为相似。设一组特征hash的相似概率为s，则通过hash函数与/或构造后的相似概率为：

1-（1-s^r）^b

在这个概率函数中，s与hash函数的精度相关，属于不可调节参数。当r越大时，相似概率越小，当b越大时，相似概率越大。r与b作为LSH的超参数可以根据实际情况进行相应的调节。

**六、****代码分析**

下面的LIRE上找到一份LSH的代码实现，这份代码实现清晰明了，比网上大多数开源出来的代码要更容易理解。 

```cpp
/*
 * This file is part of the LIRE project: http://www.semanticmetadata.net/lire
 * LIRE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LIRE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LIRE; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * We kindly ask you to refer the any or one of the following publications in
 * any publication mentioning or employing Lire:
 *
 * Lux Mathias, Savvas A. Chatzichristofis. Lire: Lucene Image Retrieval 鈥�
 * An Extensible Java CBIR Library. In proceedings of the 16th ACM International
 * Conference on Multimedia, pp. 1085-1088, Vancouver, Canada, 2008
 * URL: http://doi.acm.org/10.1145/1459359.1459577
 *
 * Lux Mathias. Content Based Image Retrieval with LIRE. In proceedings of the
 * 19th ACM International Conference on Multimedia, pp. 735-738, Scottsdale,
 * Arizona, USA, 2011
 * URL: http://dl.acm.org/citation.cfm?id=2072432
 *
 * Mathias Lux, Oge Marques. Visual Information Retrieval using Java and LIRE
 * Morgan & Claypool, 2013
 * URL: http://www.morganclaypool.com/doi/abs/10.2200/S00468ED1V01Y201301ICR025
 *
 * Copyright statement:
 * ====================
 * (c) 2002-2013 by Mathias Lux (mathias@juggle.at)
 *  http://www.semanticmetadata.net/lire, http://www.lire-project.net
 *
 * Updated: 02.06.13 10:27
 */

package net.semanticmetadata.lire.indexing.hashing;

import java.io.*;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

/**
 * <p>Each feature vector v with dimension d gets k hashes from a hash bundle h(v) = (h^1(v), h^2(v), ![img](http://www.cppblog.com/Images/dot.gif), h^k(v)) with
 * h^i(v) = (a^i*v + b^i)/w (rounded down), with a^i from R^d and b^i in [0,w) <br/>
 * If m of the k hashes match, then we assume that the feature vectors belong to similar images. Note that m*k has to be bigger than d!<br/>
 * If a^i is drawn from a normal (Gaussian) distribution LSH approximates L2. </p>
 * <p/>
 * Note that this is just to be used with bounded (normalized) descriptors.
 *
 * @author Mathias Lux, mathias@juggle.at
 *         Created: 04.06.12, 13:42
 */
public class LocalitySensitiveHashing {
    private static String name = "lshHashFunctions.obj";
    // 一组正态分布数列，长250，比实际用到的长一些
    private static int dimensions = 250;           // max d
    // 50组正态分布数列（供与/或构造共同使用）
    public static int numFunctionBundles = 50;     // k
    // 一维线段的长度，公式中的r
    public static double binLength = 10;           // w

    private static double[][] hashA = null;  
    
    
    
    // a
    private static double[] hashB = null;        // b
    private static double dilation = 1d;         // defines how "stretched out" the hash values are.

    /**
     * Writes a new file to disk to be read for hashing with LSH.
     *
     * @throws java.io.IOException
     */
    // 产生hash函数，并存储到磁盘上
    public static void generateHashFunctions() throws IOException {
        File hashFile = new File(name);
        System.out.println(hashFile.getAbsolutePath());
        if (!hashFile.exists()) {
            ObjectOutputStream oos = new ObjectOutputStream(new GZIPOutputStream(new FileOutputStream(hashFile)));
            oos.writeInt(dimensions);
            oos.writeInt(numFunctionBundles);
            // 产生点乘结果相加的随机数噪声
            for (int c = 0; c < numFunctionBundles; c++) {
                oos.writeFloat((float) (Math.random() * binLength));
            }
            // 产生 numFunctionBundles 组正态分布数列，每组dimensions个数字
            for (int c = 0; c < numFunctionBundles; c++) {
                for (int j = 0; j < dimensions; j++) {
                    oos.writeFloat((float) (drawNumber() * dilation));
                }
            }
            oos.close();
        } else {
            System.err.println("Hashes could not be written: " + name + " already exists");
        }
    }

    // 指定路径名版本
    public static void generateHashFunctions(String name) throws IOException {
        File hashFile = new File(name);
        if (!hashFile.exists()) {
            ObjectOutputStream oos = new ObjectOutputStream(new GZIPOutputStream(new FileOutputStream(hashFile)));
            oos.writeInt(dimensions);
            oos.writeInt(numFunctionBundles);
            for (int c = 0; c < numFunctionBundles; c++) {
                oos.writeFloat((float) (Math.random() * binLength));
            }
            for (int c = 0; c < numFunctionBundles; c++) {
                for (int j = 0; j < dimensions; j++) {
                    oos.writeFloat((float) (drawNumber() * dilation));
                }
            }
            oos.close();
        } else {
            System.err.println("Hashes could not be written: " + name + " already exists");
        }
    }

    /**
     * Reads a file from disk and sets the hash functions.
     *
     * @return
     * @throws IOException
     * @see LocalitySensitiveHashing#generateHashFunctions()
     */
    public static double[][] readHashFunctions() throws IOException {
        return readHashFunctions(new FileInputStream(name));
    }

    // 读取存储在磁盘上LSH hash函数值，计算图像特征时反复使用同一套LSH函数
    public static double[][] readHashFunctions(InputStream in) throws IOException {
        ObjectInputStream ois = new ObjectInputStream(new GZIPInputStream(in));
        dimensions = ois.readInt();
        numFunctionBundles = ois.readInt();
        double[] tmpB = new double[numFunctionBundles];
        for (int k = 0; k < numFunctionBundles; k++) {
            tmpB[k] = ois.readFloat();
        }
        LocalitySensitiveHashing.hashB = tmpB;
        double[][] hashFunctions = new double[numFunctionBundles][dimensions];
        for (int i = 0; i < hashFunctions.length; i++) {
            double[] functionBundle = hashFunctions[i];
            for (int j = 0; j < functionBundle.length; j++) {
                functionBundle[j] = ois.readFloat();
            }
        }
        LocalitySensitiveHashing.hashA = hashFunctions;
        return hashFunctions;
    }

    /**
     * Generates the hashes from the given hash bundles.
     *
     * @param histogram
     * @return
     */
    // 计算一个特征的numFunctionBundles组hash特征
    public static int[] generateHashes(double[] histogram) {
        double product;
        int[] result = new int[numFunctionBundles];
        for (int k = 0; k < numFunctionBundles; k++) {
            product = 0;
            // 1. 向量点乘
            for (int i = 0; i < histogram.length; i++) {
                product += histogram[i] * hashA[k][i];
            }
            // 2. 加上随机噪音，除一维线段长度
            result[k] = (int) Math.floor((product + hashB[k]/*加上随机噪音*/) / binLength /*除一维线段长度*/);
        }
        return result;
    }


    /**
     * Returns a random number distributed with standard normal distribution based on the Box-Muller method.
     *
     * @return
     */
    // Box-Muller算法，产生正态分布数
    private static double drawNumber() {
        double u, v, s;
        do {
            u = Math.random() * 2 - 1;
            v = Math.random() * 2 - 1;
            s = u * u + v * v;
        } while (s == 0 || s >= 1);
        return u * Math.sqrt(-2d * Math.log(s) / s);
//        return Math.sqrt(-2d * Math.log(Math.random())) * Math.cos(2d * Math.PI * Math.random());
    }

    public static void main(String[] args) {
        try {
            generateHashFunctions();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


}

```

**七、总结**

本文描述了LSH的适用场景、算法原理与算法分析，并配合源代码帮助读者理解算法原理，了解算法的实现要点。
程序员长期养成了从代码反向学习知识的习惯，但是到了人工智能时代这种学习方式受到了很大的挑战，在一些复杂的数学背景知识面前，先了解背景知识是深入掌握的前提。



[参考文献：
](http://www.cppblog.com/Files/humanchao/LSH(Locality%20Sensitive%20Hashing).zip)

**Website:**

[[1\] ](http://www.cppblog.com/Files/humanchao/LSH(Locality%20Sensitive%20Hashing).zip)[http://people.csail.mit.edu/indyk/ ](http://people.csail.mit.edu/indyk/)（LSH原作者）

[2] <http://www.mit.edu/~andoni/LSH/> (E2LSH)



**Paper:**

[1] Approximate nearest neighbor: towards removing the curse of dimensionality

[2] Similarity search in high dimensions via hashing

[3] Locality-sensitive hashing scheme based on p-stable distributions 

[4] MultiProbe LSH Efficient Indexing for HighDimensional Similarity Search

[5] Near-Optimal Hashing Algorithms for Approximate Nearest Neighbor in High Dimensions

**Tutorial:**

[1] Locality-Sensitive Hashing for Finding Nearest Neighbors

[2] Approximate Proximity Problems in High Dimensions via Locality-Sensitive Hashing

[3] Similarity Search in High Dimensions



**Book:**

[1] Mining of Massive Datasets
[2] Nearest Neighbor Methods in Learning and Vision: Theory and Practice



**Cdoe:**

[1] http://sourceforge.net/projects/lshkit/?source=directory

[2] http://tarsos.0110.be/releases/TarsosLSH/TarsosLSH-0.5/TarsosLSH-0.5-Readme.html 

[3] http://www.cse.ohio-state.edu/~kulis/klsh/klsh.htm 

[4] http://code.google.com/p/likelike/ 

[5] https://github.com/yahoo/Optimal-LSH

[6] [OpenCV](http://lib.csdn.net/base/opencv) LSH（分别位于legacy module和flann module中）

