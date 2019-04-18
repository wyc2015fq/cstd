# 机器学习知识点(九)BP神经网络Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月09日 09:31:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2648
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









为深入理解机器学习中BP神经网络算法，从网上找到的Java实现代码。

1、BP神经网络结构如下图所示，最左边的是输入层，最右边的是输出层，中间是多个隐含层，隐含层和输出层的每个神经节点，都是由上一层节点乘以其权重累加得到，标上“+1”的圆圈为截距项b（功能神经元结点的阈值），对输入层外每个节点：Y=w0*x0+w1*x1+…+wn*xn+b，神经网络相当于一个多层逻辑回归的结构。

![](http://img1.tuicool.com/UnEnqyv.png!web)


算法计算过程：输入层开始，从左往右计算，逐层往前直到输出层产生结果。如果结果值和目标值有差距，再从右往左算，逐层向后计算每个节点的误差，并且调整每个节点的所有权重，反向到达输入层后，又重新向前计算，重复迭代以上步骤，直到所有权重参数收敛到一个合理值。由于计算机程序求解方程参数和数学求法不一样，一般是先随机选取参数，然后不断调整参数减少误差直到逼近正确值，所以大部分的机器学习都是在不断迭代训练，下面我们从程序上详细看看该过程实现就清楚了。


2、参考代码：



```java
package sk.ml;

import java.util.Random;
import java.util.Arrays;

public class BpDeep {
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
                layer[l][j]=1/(1+Math.exp(-z));//激活函数sigmoid
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

        //设置样本数据，对应4个二维坐标数据
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


执行结果：





```
[1.0, 2.0]:[0.9778071886179536, 0.022187459499041468]
[2.0, 2.0]:[0.023368034603225344, 0.9769182145372335]
[1.0, 1.0]:[0.017374300590125305, 0.9821152168609936]
[2.0, 1.0]:[0.9784447508283497, 0.02175057450509929]
[3.0, 1.0]:[0.9853953389964808, 0.01690955109770975]
```


参考网址：http://www.tuicool.com/articles/MfYjQfV








