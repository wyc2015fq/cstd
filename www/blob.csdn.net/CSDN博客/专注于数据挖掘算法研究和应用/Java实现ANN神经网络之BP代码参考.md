# Java实现ANN神经网络之BP代码参考 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月21日 17:22:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4424








## 神经网络的计算过程


神经网络结构如下图所示，最左边的是输入层，最右边的是输出层，中间是多个隐含层，隐含层和输出层的每个神经节点，都是由上一层节点乘以其权重累加得到，标上“+1”的圆圈为截距项b，对输入层外每个节点：Y=w0*x0+w1*x1+…+wn*xn+b，由此我们可以知道神经网络相当于一个多层逻辑回归的结构。

![图片描述](https://img-blog.csdn.net/20160217224445477)


（图片来自[UFLDL Tutorial](http://ufldl.stanford.edu/wiki/index.php/UFLDL_Tutorial)）



算法计算过程：输入层开始，从左往右计算，逐层往前直到输出层产生结果。如果结果值和目标值有差距，再从右往左算，逐层向后计算每个节点的误差，并且调整每个节点的所有权重，反向到达输入层后，又重新向前计算，重复迭代以上步骤，直到所有权重参数收敛到一个合理值。由于计算机程序求解方程参数和数学求法不一样，一般是先随机选取参数，然后不断调整参数减少误差直到逼近正确值，所以大部分的机器学习都是在不断迭代训练，下面我们从程序上详细看看该过程实现就清楚了。

## 神经网络的算法程序实现


神经网络的算法程序实现分为初始化、向前计算结果，反向修改权重三个过程。

### 1. 初始化过程


由于是n层神经网络，我们用二维数组layer记录节点值，第一维为层数，第二维为该层节点位置，数组的值为节点值；同样，节点误差值layerErr也是相似方式记录。用三维数组layer_weight记录各节点权重，第一维为层数，第二维为该层节点位置，第三维为下层节点位置，数组的值为某节点到达下层某节点的权重值，初始值为0-1之间的随机数。为了优化收敛速度，这里采用动量法权值调整，需要记录上一次权值调整量，用三维数组layer_weight_delta来记录，截距项处理：程序里将截距的值设置为1，这样只需要计算它的权重就可以了，

### 2. 向前计算结果


采用S函数1/(1+Math.exp(-z))将每个节点的值统一到0-1之间，再逐层向前计算直到输出层，对于输出层，实际上是不需要再用S函数的，我们这里将输出结果视为0到1之间的概率值，所以也采用了S函数，这样也有利于程序实现的统一性。

### 3. 反向修改权重


神经网络如何计算误差，一般采用平方型误差函数E，如下：

![图片描述](https://img-blog.csdn.net/20160217225112749)


也就是将多个输出项和对应目标值的误差的平方累加起来，再除以2。实际上逻辑回归的误差函数也是这个，至于为什么要用这个函数来计算误差，它从数学上的合理性是什么，怎么得来的，这个我建议程序员们不想当数学家的话，先不去深究了，现在我们要做的是如何把这个函数E误差取它的最小值，需要对其进行求导，如果有些求导数学基础的话，倒可以尝试去推导下如何从函数E对权重求导得到下面这个公式的：

![图片描述](https://img-blog.csdn.net/20160217225135062)


不会推导也没有关系，我们只需要运用结果公式就可以了，在我们的程序里用layerErr记录了E对权重求导后的最小化误差，再根据最小化误差去调整权重。


注意这里采用动量法调整，将上一次调整的经验考虑进来，避免陷入局部最小值，下面的k代表迭代次数，mobp为动量项，rate为学习步长：



Δw(k+1) = mobp*Δw(k)+rate*Err*Layer



也有很多使用下面的公式，效果上的差别不是太大：



Δw(k+1) = mobp*Δw(k)+(1-mobp)rate*Err*Layer



为了提升性能，注意程序实现是在一个while里面同时计算误差和调整权重，先将位置定位到倒数第二层（也就是最后一层隐含层）上，然后逐层反向调整，根据L+1层算好的误差来调整L层的权重，同时计算好L层的误差，用于下一次循环到L-1层时计算权重，以此循环下去直到倒数第一层（输入层）结束。

### 小结


在整个计算过程中，节点的值是每次计算都在变化的，不需要保存，而权重参数和误差参数是需要保存的，需要为下一次迭代提供支持，因此，如果我们构思一个分布式的多机并行计算方案，就能理解其他框架中为什么会有一个Parameter Server的概念。


参考代码：



```java
package sk.ann;

import java.util.Arrays;
import java.util.Random;

public class BpDeep{
    public double[][] layer;//神经网络各层节点
    public double[][] layerErr;//神经网络各节点误差
    public double[][][] layer_weight;//各层节点权重
    public double[][][] layer_weight_delta;//各层节点权重动量
    public double mobp;//动量系数
    public double rate;//学习系数

    public BpDeep(int[] layernum, double rate, double mobp){
        this.mobp = mobp;
        this.rate = rate;
        layer = new double[layernum.length][];
        layerErr = new double[layernum.length][];
        layer_weight = new double[layernum.length][][];
        layer_weight_delta = new double[layernum.length][][];
        Random random = new Random();
        for(int l=0;l<layernum.length;l++){
            layer[l]=new double[layernum[l]];
            layerErr[l]=new double[layernum[l]];
            if(l+1<layernum.length){
                layer_weight[l]=new double[layernum[l]+1][layernum[l+1]];
                layer_weight_delta[l]=new double[layernum[l]+1][layernum[l+1]];
                for(int j=0;j<layernum[l]+1;j++)
                    for(int i=0;i<layernum[l+1];i++)
                        layer_weight[l][j][i]=random.nextDouble();//随机初始化权重
            }   
        }
    }
    //逐层向前计算输出
    public double[] computeOut(double[] in){
        for(int l=1;l<layer.length;l++){
            for(int j=0;j<layer[l].length;j++){
                double z=layer_weight[l-1][layer[l-1].length][j];
                for(int i=0;i<layer[l-1].length;i++){
                    layer[l-1][i]=l==1?in[i]:layer[l-1][i];
                    z+=layer_weight[l-1][i][j]*layer[l-1][i];
                }
                layer[l][j]=1/(1+Math.exp(-z));
            }
        }
        return layer[layer.length-1];
    }
    //逐层反向计算误差并修改权重
    public void updateWeight(double[] tar){
        int l=layer.length-1;
        for(int j=0;j<layerErr[l].length;j++)
            layerErr[l][j]=layer[l][j]*(1-layer[l][j])*(tar[j]-layer[l][j]);

        while(l-->0){
            for(int j=0;j<layerErr[l].length;j++){
                double z = 0.0;
                for(int i=0;i<layerErr[l+1].length;i++){
                    z=z+l>0?layerErr[l+1][i]*layer_weight[l][j][i]:0;
                    layer_weight_delta[l][j][i]= mobp*layer_weight_delta[l][j][i]+rate*layerErr[l+1][i]*layer[l][j];//隐含层动量调整
                    layer_weight[l][j][i]+=layer_weight_delta[l][j][i];//隐含层权重调整
                    if(j==layerErr[l].length-1){
                        layer_weight_delta[l][j+1][i]= mobp*layer_weight_delta[l][j+1][i]+rate*layerErr[l+1][i];//截距动量调整
                        layer_weight[l][j+1][i]+=layer_weight_delta[l][j+1][i];//截距权重调整
                    }
                }
                layerErr[l][j]=z*layer[l][j]*(1-layer[l][j]);//记录误差
            }
        }
    }

    public void train(double[] in, double[] tar){
        double[] out = computeOut(in);
        updateWeight(tar);
    }
    
    public static void main(String[] args){
        //初始化神经网络的基本配置
        //第一个参数是一个整型数组，表示神经网络的层数和每层节点数，比如{3,10,10,10,10,2}表示输入层是3个节点，输出层是2个节点，中间有4层隐含层，每层10个节点
        //第二个参数是学习步长，第三个参数是动量系数
        BpDeep bp = new BpDeep(new int[]{2,10,2}, 0.15, 0.8);

        //设置样本数据，对应上面的4个二维坐标数据
        double[][] data = new double[][]{{1,2},{2,2},{1,1},{2,1}};
        //设置目标数据，对应4个坐标数据的分类
        double[][] target = new double[][]{{1,0},{0,1},{0,1},{1,0}};

        //迭代训练5000次
        for(int n=0;n<5000;n++)
            for(int i=0;i<data.length;i++)
                bp.train(data[i], target[i]);

        //根据训练结果来检验样本数据
        for(int j=0;j<data.length;j++){
            double[] result = bp.computeOut(data[j]);
            System.out.println(Arrays.toString(data[j])+":"+Arrays.toString(result));
        }

        //根据训练结果来预测一条新数据的分类
        double[] x = new double[]{3,1};
        double[] result = bp.computeOut(x);
        System.out.println(Arrays.toString(x)+":"+Arrays.toString(result));
    }
}
```





