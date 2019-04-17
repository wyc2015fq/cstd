# 编写C语言版本的卷积神经网络CNN之四：CNN的学习及测试结果的比较 - tostq的专栏 - CSDN博客





2016年06月30日 21:51:09[tostq](https://me.csdn.net/tostq)阅读数：5562








原创文章


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



**Tips:**

      关于在线学习和批量学习

**批量学习方法：**突触权值的调整在训练样本集合的所有N个例子都出现后进行，这个N个例子构成训练的一个回合，每个回合的训练样本的样例都是随机选择的，而权值的调整是靠所有N个例子的总体平均来实现。

批量学习方法的优点在于能够精确估计当前梯度向量（即代价函数对权值向量W的导数），因此，保证能最快下降到局部极小点的收敛性，另外能满足并行计算的要求，当前也增加存储压力。

**在线学习方法：**即样例是一个接一个输入网络中，只有当前一样本输入完成权值调整后，后一样本才能进入网络。在线学习方法相比于批量学习方法，其更容易执行，而且也不易陷入局部极值点，另外比批量学习需要更少的存储空间。

本文这个版本的CNN采用是在线学习方法，DeepLearnToolbox的版本默认是采用的是批量学习，可以将批量数改为1也就成了在线学习方法。




前面几节我们重点介绍CNN的结构和学习方法，这一节我们观察这个网络的实验结果。

首先前面也说了，虽然称C语言版本的CNN，但博主为了方便调试还是用了VS2010来编写。




**一、CNN的学习过程及误差曲线**

这里要提前说明三点：

1、学习过程的要花费很长的时间，笔者的电脑跑了两个多小时，DeepLearnToolbox的Matlab版本大概只要20分钟左右，这主要是由于Matlab并行运算的缘故。

2、程序对于内存空间要求很大，虽然博主尽量仔细处理分配的空间，但还是需要2G多内存才能完成整个学习过程，另外在程序运行前，最好设置一个较大的堆。

3、为了方便测试与观察结果，博主将程序产生相关数据保存起来，放到了Matlab内观察。

最终结果：本文的C版本同DeepLearnToolbox的Matlab版本的训练结果几乎是一致的。训练误差图如下所示，可以看到最终的误差大概都在0.02左右。

![](https://img-blog.csdn.net/20160630211545389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**二、CNN权重的保存**



```cpp
void savecnn(CNN* cnn, const char* filename)
{
    FILE  *fp=NULL;
    fp=fopen(filename,"wb");
    if(fp==NULL)
        printf("write file failed\n");

    int i,j,r;
    // C1的数据
    for(i=0;i<cnn->C1->inChannels;i++)
        for(j=0;j<cnn->C1->outChannels;j++)
            for(r=0;r<cnn->C1->mapSize;r++)
                fwrite(cnn->C1->mapData[i][j][r],sizeof(float),cnn->C1->mapSize,fp);

    fwrite(cnn->C1->basicData,sizeof(float),cnn->C1->outChannels,fp);

    // C3网络
    for(i=0;i<cnn->C3->inChannels;i++)
        for(j=0;j<cnn->C3->outChannels;j++)
            for(r=0;r<cnn->C3->mapSize;r++)
                fwrite(cnn->C3->mapData[i][j][r],sizeof(float),cnn->C3->mapSize,fp);

    fwrite(cnn->C3->basicData,sizeof(float),cnn->C3->outChannels,fp);

    // O5输出层
    for(i=0;i<cnn->O5->outputNum;i++)
        fwrite(cnn->O5->wData[i],sizeof(float),cnn->O5->inputNum,fp);
    fwrite(cnn->O5->basicData,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}
```




**三、测试正确识别率**



```cpp
int vecmaxIndex(float* vec, int veclength)// 返回向量最大数的序号
{
    int i;
    float maxnum=-1.0;
    int maxIndex=0;
    for(i=0;i<veclength;i++){
        if(maxnum<vec[i]){
            maxnum=vec[i];
            maxIndex=i;
        }
    }
    return maxIndex;
}

// 测试cnn函数
float cnntest(CNN* cnn, ImgArr inputData,LabelArr outputData,int testNum)
{
    int n=0;
    int incorrectnum=0;  //错误预测的数目
    for(n=0;n<testNum;n++){
        cnnff(cnn,inputData->ImgPtr[n].ImgData);
        if(vecmaxIndex(cnn->O5->y,cnn->O5->outputNum)!=vecmaxIndex(outputData->LabelPtr[n].LabelData,cnn->O5->outputNum))
            incorrectnum++;
        cnnclear(cnn);
    }
    return (float)incorrectnum/(float)testNum;
}
```




      经过测试函数，本文的最终错误识别率为3%左右

    （本篇有点短，还用了很多代码凑数，水了点>_<，最后附上CNN项目代码）

     项目代码地址：[https://github.com/tostq/DeepLearningC/tree/master/CNN](https://github.com/tostq/DeepLearningC/tree/master/CNN)






