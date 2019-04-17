# 机器学习知识点(三十)LDA话题模型Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月17日 13:02:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3237
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









1、LDA数学定义

1）话题模型：传统的文本分类器，比如贝叶斯、kNN和SVM，只能将其分到一个确定的类别中。假设我给出3个分类“算法”“分词”“文学”让其判断，如果某个分类器将该文归入算法类，我觉得还凑合，如果分入分词，那我觉得这个分类器不够准确。


假设一个文艺小青年来看我的博客，他完全不懂算法和分词，自然也给不出具体的备选类别，有没有一种模型能够告诉这个白痴，这篇文章很可能(80%)是在讲算法，也可能(19%)是在讲分词，几乎不可能(1%)是在讲其它主题呢？



有，这样的模型就是话题模型。

2）LDA概念

潜在狄立克雷分配（Latent Dirichlet Allocation，LDA）主题模型是最简单的主题模型，它描述的是一篇文章是如何产生的。如图所示：


![](http://ww4.sinaimg.cn/large/6cbb8645jw1eoo8k4ft98j20pk0dogpu.jpg)


从左往右看，一个主题是由一些词语的分布定义的，比如蓝色主题是由2%几率的data，2%的number……构成的。一篇文章则是由一些主题构成的，比如右边的直方图。具体产生过程是，从主题集合中按概率分布选取一些主题，从该主题中按概率分布选取一些词语，这些词语构成了最终的文档（LDA模型中，词语的无序集合构成文档，也就是说词语的顺序没有关系）。



如果我们能将上述两个概率分布计算清楚，那么我们就得到了一个模型，该模型可以根据某篇文档推断出它的主题分布，即分类。由文档推断主题是文档生成过程的逆过程。



在《LDA数学八卦》一文中，对文档的生成过程有个很形象的描述：


![](http://ww1.sinaimg.cn/large/6cbb8645jw1eoo9wpkemgj20mc0fejst.jpg)


3）概率模型



LDA是一种使用联合分布来计算在给定观测变量下隐藏变量的条件分布（后验分布）的概率模型，观测变量为词的集合，隐藏变量为主题。

### 联合分布

LDA的生成过程对应的观测变量和隐藏变量的联合分布如下：



![](http://ww2.sinaimg.cn/large/6cbb8645jw1eoo900spbyj20ko02gjrv.jpg)

式子有点长，耐心地从左往右看：

式子的基本符号约定——β表示主题，θ表示主题的概率，z表示特定文档或词语的主题，w为词语。进一步说——

β1:K为全体主题集合，其中βk是第k个主题的词的分布（如图1左部所示）。第d个文档中该主题所占的比例为θd，其中θd,k表示第k个主题在第d个文档中的比例（图1右部的直方图）。第d个文档的主题全体为zd，其中zd,n是第d个文档中第n个词的主题（如图1中有颜色的圆圈）。第d个文档中所有词记为wd，其中wd,n是第d个文档中第n个词，每个词都是固定的词汇表中的元素。

p(β）表示从主题集合中选取了一个特定主题，p(θd)表示该主题在特定文档中的概率，大括号的前半部分是该主题确定时该文档第n个词的主题，后半部分是该文档第n个词的主题与该词的联合分布。连乘符号描述了随机变量的依赖性，用概率图模型表述如下：



![](http://ww2.sinaimg.cn/large/6cbb8645jw1eoo9qqnidvj20ce03jdfw.jpg)


比如，先选取了主题，才能从主题里选词。具体说来，一个词受两个随机变量的影响（直接或间接），一个是确定了主题后文档中该主题的分布θd，另一种是第k个主题的词的分布βk（也就是图2中的第二个坛子）。

### 后验分布

沿用相同的符号，LDA后验分布计算公式如下：




![](http://ww2.sinaimg.cn/large/6cbb8645jw1eooa69fd0vj20cu021weo.jpg)


分子是一个联合分布，给定语料库就可以轻松统计出来。但分母无法暴力计算，因为文档集合词库达到百万（假设有w个词语），每个词要计算每一种可能的观测的组合（假设有n种组合）的概率然后累加得到先验概率，所以需要一种近似算法。


基于采样的算法通过收集后验分布的样本，以样本的分布求得后验分布的近似。

θd的概率服从Dirichlet分布，zd,n的分布服从multinomial分布，两个分布共轭，所谓共轭，指的就是先验分布和后验分布的形式相同：

![](http://ww3.sinaimg.cn/large/6cbb8645jw1eoqqfhiki4j20ai01awej.jpg)


两个分布其实是向量的分布，向量通过这两个分布取样得到。采样方法通过收集这两个分布的样本，以样本的分布近似。

4）马氏链和Gibbs Sampling




这是一种统计模拟的方法。

### 马氏链


所谓马氏链指的是当前状态只取决于上一个状态。马氏链有一个重要的性质：状态转移矩阵P的幂是收敛的，收敛后的转移矩阵称为马氏链的平稳分布。给定p(x)，假如能够构造一个P，转移n步平稳分布恰好是p(x)。那么任取一个初始状态，转移n步之后的状态都是符合分布的样本。

### Gibbs Sampling

Gibbs Sampling是高维分布（也即类似于二维p(x,y)，三维p(x,y,z)的分布）的特化采样算法。

2、代码：

   参考：https://github.com/hankcs/LDA4j

   源于Gregor Heinrich的LdaGibbsSampler.java类。




```java
/*
 * (C) Copyright 2005, Gregor Heinrich (gregor :: arbylon : net) (This file is
 * part of the org.knowceans experimental software packages.)
 */
/*
 * LdaGibbsSampler is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 */
/*
 * LdaGibbsSampler is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 */
/*
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * Created on Mar 6, 2005
 */
package sk.ml;

import java.text.DecimalFormat;
import java.text.NumberFormat;

/**
 * Gibbs sampler for estimating the best assignments of topics for words and
 * documents in a corpus. The algorithm is introduced in Tom Griffiths' paper
 * "Gibbs sampling in the generative model of Latent Dirichlet Allocation"
 * (2002).<br>
 * Gibbs sampler采样算法的实现
 *
 * @author heinrich
 */
public class LdaGibbsSampler {

    /**
     * document data (term lists)<br>
     * 文档
     */
    int[][] documents;

    /**
     * vocabulary size<br>
     * 词表大小
     */
    int V;

    /**
     * number of topics<br>
     * 主题数目
     */
    int K;

    /**
     * Dirichlet parameter (document--topic associations)<br>
     * 文档——主题参数
     */
    double alpha = 2.0;

    /**
     * Dirichlet parameter (topic--term associations)<br>
     * 主题——词语参数
     */
    double beta = 0.5;

    /**
     * topic assignments for each word.<br>
     * 每个词语的主题 z[i][j] := 文档i的第j个词语的主题编号
     */
    int z[][];

    /**
     * cwt[i][j] number of instances of word i (term?) assigned to topic j.<br>
     * 计数器，nw[i][j] := 词语i归入主题j的次数
     */
    int[][] nw;

    /**
     * na[i][j] number of words in document i assigned to topic j.<br>
     * 计数器，nd[i][j] := 文档[i]中归入主题j的词语的个数
     */
    int[][] nd;

    /**
     * nwsum[j] total number of words assigned to topic j.<br>
     * 计数器，nwsum[j] := 归入主题j词语的个数
     */
    int[] nwsum;

    /**
     * nasum[i] total number of words in document i.<br>
     * 计数器,ndsum[i] := 文档i中全部词语的数量
     */
    int[] ndsum;

    /**
     * cumulative statistics of theta<br>
     * theta的累积量
     */
    double[][] thetasum;

    /**
     * cumulative statistics of phi<br>
     * phi的累积量
     */
    double[][] phisum;

    /**
     * size of statistics<br>
     * 样本容量
     */
    int numstats;

    /**
     * sampling lag (?)<br>
     * 多久更新一次统计量
     */
    private static int THIN_INTERVAL = 20;

    /**
     * burn-in period<br>
     * 收敛前的迭代次数
     */
    private static int BURN_IN = 100;

    /**
     * max iterations<br>
     * 最大迭代次数
     */
    private static int ITERATIONS = 1000;

    /**
     * sample lag (if -1 only one sample taken)<br>
     * 最后的模型个数（取收敛后的n个迭代的参数做平均可以使得模型质量更高）
     */
    private static int SAMPLE_LAG = 10;

    private static int dispcol = 0;

    /**
     * Initialise the Gibbs sampler with data.<br>
     * 用数据初始化采样器
     *
     * @param documents 文档
     * @param V         vocabulary size 词表大小
     */
    public LdaGibbsSampler(int[][] documents, int V) {

        this.documents = documents;
        this.V = V;
    }

    /**
     * Initialisation: Must start with an assignment of observations to topics ?
     * Many alternatives are possible, I chose to perform random assignments
     * with equal probabilities<br>
     * 随机初始化状态
     *
     * @param K number of topics K个主题
     */
    public void initialState(int K) {
        int M = documents.length;

        // initialise count variables. 初始化计数器
        nw = new int[V][K];
        nd = new int[M][K];
        nwsum = new int[K];
        ndsum = new int[M];

        // The z_i are are initialised to values in [1,K] to determine the
        // initial state of the Markov chain.

        z = new int[M][];   // z_i := 1到K之间的值，表示马氏链的初始状态
        for (int m = 0; m < M; m++) {
            int N = documents[m].length;
            z[m] = new int[N];
            for (int n = 0; n < N; n++) {
                int topic = (int) (Math.random() * K);
                z[m][n] = topic;
                // number of instances of word i assigned to topic j
                nw[documents[m][n]][topic]++;
                // number of words in document i assigned to topic j.
                nd[m][topic]++;
                // total number of words assigned to topic j.
                nwsum[topic]++;
            }
            // total number of words in document i
            ndsum[m] = N;
        }
    }

    public void gibbs(int K) {
        gibbs(K, 2.0, 0.5);
    }

    /**
     * Main method: Select initial state ? Repeat a large number of times: 1.
     * Select an element 2. Update conditional on other elements. If
     * appropriate, output summary for each run.<br>
     * 采样
     *
     * @param K     number of topics 主题数
     * @param alpha symmetric prior parameter on document--topic associations 对称文档——主题先验概率？
     * @param beta  symmetric prior parameter on topic--term associations 对称主题——词语先验概率？
     */
    public void gibbs(int K, double alpha, double beta) {
        this.K = K;
        this.alpha = alpha;
        this.beta = beta;

        // init sampler statistics  分配内存
        if (SAMPLE_LAG > 0) {
            thetasum = new double[documents.length][K];
            phisum = new double[K][V];
            numstats = 0;
        }

        // initial state of the Markov chain:
        initialState(K);

        System.out.println("Sampling " + ITERATIONS
                + " iterations with burn-in of " + BURN_IN + " (B/S="
                + THIN_INTERVAL + ").");

        for (int i = 0; i < ITERATIONS; i++) {

            // for all z_i
            for (int m = 0; m < z.length; m++) {
                for (int n = 0; n < z[m].length; n++) {

                    // (z_i = z[m][n])
                    // sample from p(z_i|z_-i, w)
                    int topic = sampleFullConditional(m, n);
                    z[m][n] = topic;
                }
            }

            if ((i < BURN_IN) && (i % THIN_INTERVAL == 0)) {
                System.out.print("B");
                dispcol++;
            }
            // display progress
            if ((i > BURN_IN) && (i % THIN_INTERVAL == 0)) {
                System.out.print("S");
                dispcol++;
            }
            // get statistics after burn-in
            if ((i > BURN_IN) && (SAMPLE_LAG > 0) && (i % SAMPLE_LAG == 0)) {
                updateParams();
                System.out.print("|");
                if (i % THIN_INTERVAL != 0)
                    dispcol++;
            }
            if (dispcol >= 100) {
                System.out.println();
                dispcol = 0;
            }
        }
        System.out.println();
    }

    /**
     * Sample a topic z_i from the full conditional distribution: p(z_i = j |
     * z_-i, w) = (n_-i,j(w_i) + beta)/(n_-i,j(.) + W * beta) * (n_-i,j(d_i) +
     * alpha)/(n_-i,.(d_i) + K * alpha) <br>
     * 根据上述公式计算文档m中第n个词语的主题的完全条件分布，输出最可能的主题
     *
     * @param m document
     * @param n word
     */
    private int sampleFullConditional(int m, int n) {

        // remove z_i from the count variables  先将这个词从计数器中抹掉
        int topic = z[m][n];
        nw[documents[m][n]][topic]--;
        nd[m][topic]--;
        nwsum[topic]--;
        ndsum[m]--;

        // do multinomial sampling via cumulative method: 通过多项式方法采样多项式分布
        double[] p = new double[K];
        for (int k = 0; k < K; k++) {
            p[k] = (nw[documents[m][n]][k] + beta) / (nwsum[k] + V * beta)
                    * (nd[m][k] + alpha) / (ndsum[m] + K * alpha);
        }
        // cumulate multinomial parameters  累加多项式分布的参数
        for (int k = 1; k < p.length; k++) {
            p[k] += p[k - 1];
        }
        // scaled sample because of unnormalised p[] 正则化
        double u = Math.random() * p[K - 1];
        for (topic = 0; topic < p.length; topic++) {
            if (u < p[topic])
                break;
        }

        // add newly estimated z_i to count variables   将重新估计的该词语加入计数器
        nw[documents[m][n]][topic]++;
        nd[m][topic]++;
        nwsum[topic]++;
        ndsum[m]++;

        return topic;
    }

    /**
     * Add to the statistics the values of theta and phi for the current state.<br>
     * 更新参数
     */
    private void updateParams() {
        for (int m = 0; m < documents.length; m++) {
            for (int k = 0; k < K; k++) {
                thetasum[m][k] += (nd[m][k] + alpha) / (ndsum[m] + K * alpha);
            }
        }
        for (int k = 0; k < K; k++) {
            for (int w = 0; w < V; w++) {
                phisum[k][w] += (nw[w][k] + beta) / (nwsum[k] + V * beta);
            }
        }
        numstats++;
    }

    /**
     * Retrieve estimated document--topic associations. If sample lag > 0 then
     * the mean value of all sampled statistics for theta[][] is taken.<br>
     * 获取文档——主题矩阵
     *
     * @return theta multinomial mixture of document topics (M x K)
     */
    public double[][] getTheta() {
        double[][] theta = new double[documents.length][K];

        if (SAMPLE_LAG > 0) {
            for (int m = 0; m < documents.length; m++) {
                for (int k = 0; k < K; k++) {
                    theta[m][k] = thetasum[m][k] / numstats;
                }
            }

        } else {
            for (int m = 0; m < documents.length; m++) {
                for (int k = 0; k < K; k++) {
                    theta[m][k] = (nd[m][k] + alpha) / (ndsum[m] + K * alpha);
                }
            }
        }

        return theta;
    }

    /**
     * Retrieve estimated topic--word associations. If sample lag > 0 then the
     * mean value of all sampled statistics for phi[][] is taken.<br>
     * 获取主题——词语矩阵
     *
     * @return phi multinomial mixture of topic words (K x V)
     */
    public double[][] getPhi() {
        double[][] phi = new double[K][V];
        if (SAMPLE_LAG > 0) {
            for (int k = 0; k < K; k++) {
                for (int w = 0; w < V; w++) {
                    phi[k][w] = phisum[k][w] / numstats;
                }
            }
        } else {
            for (int k = 0; k < K; k++) {
                for (int w = 0; w < V; w++) {
                    phi[k][w] = (nw[w][k] + beta) / (nwsum[k] + V * beta);
                }
            }
        }
        return phi;
    }

    /**
     * Print table of multinomial data
     *
     * @param data vector of evidence
     * @param fmax max frequency in display
     * @return the scaled histogram bin values
     */
    public static void hist(double[] data, int fmax) {

        double[] hist = new double[data.length];
        // scale maximum
        double hmax = 0;
        for (int i = 0; i < data.length; i++) {
            hmax = Math.max(data[i], hmax);
        }
        double shrink = fmax / hmax;
        for (int i = 0; i < data.length; i++) {
            hist[i] = shrink * data[i];
        }

        NumberFormat nf = new DecimalFormat("00");
        String scale = "";
        for (int i = 1; i < fmax / 10 + 1; i++) {
            scale += "    .    " + i % 10;
        }

        System.out.println("x" + nf.format(hmax / fmax) + "\t0" + scale);
        for (int i = 0; i < hist.length; i++) {
            System.out.print(i + "\t|");
            for (int j = 0; j < Math.round(hist[i]); j++) {
                if ((j + 1) % 10 == 0)
                    System.out.print("]");
                else
                    System.out.print("|");
            }
            System.out.println();
        }
    }

    /**
     * Configure the gibbs sampler<br>
     * 配置采样器
     *
     * @param iterations   number of total iterations
     * @param burnIn       number of burn-in iterations
     * @param thinInterval update statistics interval
     * @param sampleLag    sample interval (-1 for just one sample at the end)
     */
    public void configure(int iterations, int burnIn, int thinInterval,
                          int sampleLag) {
        ITERATIONS = iterations;
        BURN_IN = burnIn;
        THIN_INTERVAL = thinInterval;
        SAMPLE_LAG = sampleLag;
    }

    /**
     * Inference a new document by a pre-trained phi matrix
     *
     * @param phi pre-trained phi matrix
     * @param doc document
     * @return a p array
     */
    public static double[] inference(double alpha, double beta, double[][] phi, int[] doc) {
        int K = phi.length;
        int V = phi[0].length;
        // init

        // initialise count variables. 初始化计数器
        int[][] nw = new int[V][K];
        int[] nd = new int[K];
        int[] nwsum = new int[K];
        int ndsum = 0;

        // The z_i are are initialised to values in [1,K] to determine the
        // initial state of the Markov chain.

        int N = doc.length;
        int[] z = new int[N];   // z_i := 1到K之间的值，表示马氏链的初始状态
        for (int n = 0; n < N; n++) {
            int topic = (int) (Math.random() * K);
            z[n] = topic;
            // number of instances of word i assigned to topic j
            nw[doc[n]][topic]++;
            // number of words in document i assigned to topic j.
            nd[topic]++;
            // total number of words assigned to topic j.
            nwsum[topic]++;
        }
        // total number of words in document i
        ndsum = N;
        for (int i = 0; i < ITERATIONS; i++) {
            for (int n = 0; n < z.length; n++) {

                // (z_i = z[m][n])
                // sample from p(z_i|z_-i, w)
                // remove z_i from the count variables  先将这个词从计数器中抹掉
                int topic = z[n];
                nw[doc[n]][topic]--;
                nd[topic]--;
                nwsum[topic]--;
                ndsum--;

                // do multinomial sampling via cumulative method: 通过多项式方法采样多项式分布
                double[] p = new double[K];
                for (int k = 0; k < K; k++) {
                    p[k] = phi[k][doc[n]]
                            * (nd[k] + alpha) / (ndsum + K * alpha);
                }
                // cumulate multinomial parameters  累加多项式分布的参数
                for (int k = 1; k < p.length; k++) {
                    p[k] += p[k - 1];
                }
                // scaled sample because of unnormalised p[] 正则化
                double u = Math.random() * p[K - 1];
                for (topic = 0; topic < p.length; topic++) {
                    if (u < p[topic])
                        break;
                }
                if (topic == K) {
                    throw new RuntimeException("the param K or topic is set too small");
                }
                // add newly estimated z_i to count variables   将重新估计的该词语加入计数器
                nw[doc[n]][topic]++;
                nd[topic]++;
                nwsum[topic]++;
                ndsum++;
                z[n] = topic;
            }
        }

        double[] theta = new double[K];

        for (int k = 0; k < K; k++) {
            theta[k] = (nd[k] + alpha) / (ndsum + K * alpha);
        }
        return theta;
    }

    public static double[] inference(double[][] phi, int[] doc) {
        return inference(2.0, 0.5, phi, doc);
    }

    /**
     * Driver with example data.<br>
     * 测试入口
     *
     * @param args
     */
    public static void main(String[] args) {

        // words in documents
        int[][] documents = {
                {1, 4, 3, 2, 3, 1, 4, 3, 2, 3, 1, 4, 3, 2, 3, 6},
                {2, 2, 4, 2, 4, 2, 2, 2, 2, 4, 2, 2},
                {1, 6, 5, 6, 0, 1, 6, 5, 6, 0, 1, 6, 5, 6, 0, 0},
                {5, 6, 6, 2, 3, 3, 6, 5, 6, 2, 2, 6, 5, 6, 6, 6, 0},
                {2, 2, 4, 4, 4, 4, 1, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 0},
                {5, 4, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2}};  // 文档的词语id集合
        // vocabulary
        int V = 7;                                      // 词表大小
        int M = documents.length;
        // # topics
        int K = 2;                                      // 主题数目
        // good values alpha = 2, beta = .5
        double alpha = 2;
        double beta = .5;

        System.out.println("Latent Dirichlet Allocation using Gibbs Sampling.");

        LdaGibbsSampler lda = new LdaGibbsSampler(documents, V);
        lda.configure(10000, 2000, 100, 10);
        lda.gibbs(K, alpha, beta);

        double[][] theta = lda.getTheta();
        double[][] phi = lda.getPhi();

        System.out.println();
        System.out.println();
        System.out.println("Document--Topic Associations, Theta[d][k] (alpha="
                + alpha + ")");
        System.out.print("d\\k\t");
        for (int m = 0; m < theta[0].length; m++) {
            System.out.print("   " + m % 10 + "    ");
        }
        System.out.println();
        for (int m = 0; m < theta.length; m++) {
            System.out.print(m + "\t");
            for (int k = 0; k < theta[m].length; k++) {
                // System.out.print(theta[m][k] + " ");
                System.out.print(shadeDouble(theta[m][k], 1) + " ");
            }
            System.out.println();
        }
        System.out.println();
        System.out.println("Topic--Term Associations, Phi[k][w] (beta=" + beta
                + ")");

        System.out.print("k\\w\t");
        for (int w = 0; w < phi[0].length; w++) {
            System.out.print("   " + w % 10 + "    ");
        }
        System.out.println();
        for (int k = 0; k < phi.length; k++) {
            System.out.print(k + "\t");
            for (int w = 0; w < phi[k].length; w++) {
                // System.out.print(phi[k][w] + " ");
                System.out.print(shadeDouble(phi[k][w], 1) + " ");
            }
            System.out.println();
        }
        // Let's inference a new document
        int[] aNewDocument = {2, 2, 4, 2, 4, 2, 2, 2, 2, 4, 2, 2};
        double[] newTheta = inference(alpha, beta, phi, aNewDocument);
        for (int k = 0; k < newTheta.length; k++) {
            // System.out.print(theta[m][k] + " ");
            System.out.print(shadeDouble(newTheta[k], 1) + " ");
        }
        System.out.println();
    }

    static String[] shades = {"     ", ".    ", ":    ", ":.   ", "::   ",
            "::.  ", ":::  ", ":::. ", ":::: ", "::::.", ":::::"};

    static NumberFormat lnf = new DecimalFormat("00E0");

    /**
     * create a string representation whose gray value appears as an indicator
     * of magnitude, cf. Hinton diagrams in statistics.
     *
     * @param d   value
     * @param max maximum value
     * @return
     */
    public static String shadeDouble(double d, double max) {
        int a = (int) Math.floor(d * 10 / max + 0.5);
        if (a > 10 || a < 0) {
            String x = lnf.format(d);
            a = 5 - x.length();
            for (int i = 0; i < a; i++) {
                x += " ";
            }
            return "<" + x + ">";
        }
        return "[" + shades[a] + "]";
    }
}
```




要理解这段代码，还是要深入其定义的数学形式，统计学方面相关的分布知识。

参考：http://www.hankcs.com/nlp/lda-java-introduction-and-implementation.html

对于机器学习的很多知识点，目前先汇聚起来，后续需要应用时一个个专题研究。








