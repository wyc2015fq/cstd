# 编写C语言版本的卷积神经网络CNN之一：前言与Minst数据集 - tostq的专栏 - CSDN博客





置顶2016年06月29日 22:12:07[tostq](https://me.csdn.net/tostq)阅读数：29767标签：[c语言																[cnn																[神经网络																[MINST](https://so.csdn.net/so/search/s.do?q=MINST&t=blog)
个人分类：[深度学习](https://blog.csdn.net/tostq/article/category/6290467)





原创文章
转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



**前言**

       卷积神经网络是深度学习的基础，但是学习CNN却不是那么简单，虽然网络上关于CNN的相关代码很多，比较经典的是tiny_cnn（C++）、DeepLearnToolbox（Matlab）等等，但通过C语言来编写CNN的却比较少，本人因为想在多核DSP下运行CNN，所以便尝试通过C语言来编写，主要参考的代码是DeepLearnToolbox的内容，DeepLearnToolbox是用Matlab脚本编写，是我看过的最为简单的CNN代码，代码清晰，阅读方便，非常适合新手入门学习。
本文的CNN代码是一个最基本的卷积网络，主要用于手写数字的识别，选择的训练测试是数据库是Minst手写数字库，主要是包括了一个基本的多层卷积网络框架、卷积层、Pooling层、及全连接的单层神经网络输出层，不过CNN其他重要的概念如Dropout、ReLu等暂时没有涉及，但是个人对于新手，学习卷积网络的基本结构及其误差反向传播方法是完全足够。

这里要注意的是，本文的方法并不是深度学习之父Yann. LeCun在1998年就已经提出的成熟算法LeNet-5卷积网络，而只是DeepLearnToolbox内的cnn代码的c语言实现，不过我们会比较二者之间的区别，因为二者的基本原理是相似的。另外，为了不使博客篇幅过长，所以博客中贴的代码并不完整，完整代码请见附件。

这篇博客总共分为四节：

第一节：前言，介绍项目结构及Minst数据集测试训练数据集

第二节：主要介绍CNN的网络结构、相关数据结构

第三节：重点介绍CNN学习训练过程的误差反向传播方法，采用的是在线训练方式

第四节：CNN的学习及测试结果的比较




**论文参考文献：**

Y. LeCun, L. Bottou, Y. Bengio and P. Haffner:Gradient-Based Learning Applied to Document Recognition, Proceedings of the IEEE, 86(11):2278-2324,
 November 1998




**一、代码结构**

本文的CNN代码是通过标准C编写，也不需要调用任何三方库文件，附件共享的文件是通过VS2010编译的项目文件（这里虽然是.cpp文件，但实际上完全是用C编写的，直接改成.c文件是完全可以使用的），当然也可以直接将相关的源文件导入到其他IDE上，也是能够运行的。

文件结构：

cnn.cpp cnn.h 存在关于CNN网络的函数、网络结构等

minst.cpp minst.h 处理Minst数据库的函数、数据结构等

mat.cpp mat.h 一些关于矩阵的函数，如卷积操作、180度翻转操作等

main.cpp 主函数和测试函数





**二、MINST数据库**

MINST数据库是由Yann提供的手写数字数据库文件，其官方下载地址[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)

这个里面还包含了对这个数据库进行识别的各类算法的结果比较及相关算法的论文

![](https://img-blog.csdn.net/20160629220713526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


数据库的里的图像都是28*28大小的灰度图像，每个像素的是一个八位字节（0~255）

![](https://img-blog.csdn.net/20160629220759683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这个数据库主要包含了60000张的训练图像和10000张的测试图像，主要是下面的四个文件

![](https://img-blog.csdn.net/20160629220853418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上述四个文件直接解压就可以使用了，虽然数据未压缩，但是我们还是需要将图像提取出来，方便我们进行操作

（1）存储图像数据的相关数据结构：

单张图像结构及保存图像的链表

![](https://img-blog.csdn.net/20160629220914480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图像实际数字标号（0~9），数据库里是用一个八位字节来表示，不过为了方便学习，需要用10位来表示。

这里的10位表示网络输出层的10个神经元，某位为1表示数字标号即为该位。

![](https://img-blog.csdn.net/20160629221001084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）读入图像数据的相关函数


```cpp
ImgArr read_Img(const char* filename) // 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;  
    int number_of_images = 0;  
    int n_rows = 0;  
    int n_cols = 0;  
    //从文件中读取sizeof(magic_number) 个字符到 &magic_number  
    fread((char*)&magic_number,sizeof(magic_number),1,fp); 
    magic_number = ReverseInt(magic_number);  
    //获取训练或测试image的个数number_of_images 
    fread((char*)&number_of_images,sizeof(number_of_images),1,fp);  
    number_of_images = ReverseInt(number_of_images);    
    //获取训练或测试图像的高度Heigh  
    fread((char*)&n_rows,sizeof(n_rows),1,fp); 
    n_rows = ReverseInt(n_rows);                  
    //获取训练或测试图像的宽度Width  
    fread((char*)&n_cols,sizeof(n_cols),1,fp); 
    n_cols = ReverseInt(n_cols);  
    //获取第i幅图像，保存到vec中 
    int i,r,c;

    // 图像数组的初始化
    ImgArr imgarr=(ImgArr)malloc(sizeof(MinstImgArr));
    imgarr->ImgNum=number_of_images;
    imgarr->ImgPtr=(MinstImg*)malloc(number_of_images*sizeof(MinstImg));

    for(i = 0; i < number_of_images; ++i)  
    {  
        imgarr->ImgPtr[i].r=n_rows;
        imgarr->ImgPtr[i].c=n_cols;
        imgarr->ImgPtr[i].ImgData=(float**)malloc(n_rows*sizeof(float*));
        for(r = 0; r < n_rows; ++r)      
        {
            imgarr->ImgPtr[i].ImgData[r]=(float*)malloc(n_cols*sizeof(float));
            for(c = 0; c < n_cols; ++c)
            { 
                // 因为神经网络用float型计算更为精确，这里我们将图像像素转为浮点型
                unsigned char temp = 0;  
                fread((char*) &temp, sizeof(temp),1,fp); 
                imgarr->ImgPtr[i].ImgData[r][c]=(float)temp/255.0;
            }  
        }    
    }

    fclose(fp);
    return imgarr;
}
```



（3）读入图像数据标号



```cpp
LabelArr read_Lable(const char* filename)// 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;  
    int number_of_labels = 0; 
    int label_long = 10;

    //从文件中读取sizeof(magic_number) 个字符到 &magic_number  
    fread((char*)&magic_number,sizeof(magic_number),1,fp); 
    magic_number = ReverseInt(magic_number);  
    //获取训练或测试image的个数number_of_images 
    fread((char*)&number_of_labels,sizeof(number_of_labels),1,fp);  
    number_of_labels = ReverseInt(number_of_labels);    

    int i,l;

    // 图像标记数组的初始化
    LabelArr labarr=(LabelArr)malloc(sizeof(MinstLabelArr));
    labarr->LabelNum=number_of_labels;
    labarr->LabelPtr=(MinstLabel*)malloc(number_of_labels*sizeof(MinstLabel));

    for(i = 0; i < number_of_labels; ++i)  
    {
        // 数据库内的图像标记是一位，这里将图像标记变成10位，10位中只有唯一一位为1，为1位即是图像标记  
        labarr->LabelPtr[i].l=10;
        labarr->LabelPtr[i].LabelData=(float*)calloc(label_long,sizeof(float));
        unsigned char temp = 0;  
        fread((char*) &temp, sizeof(temp),1,fp); 
        labarr->LabelPtr[i].LabelData[(int)temp]=1.0;    
    }

    fclose(fp);
    return labarr;    
}
```

 项目代码地址：[https://github.com/tostq/DeepLearningC/tree/master/CNN](https://github.com/tostq/DeepLearningC/tree/master/CNN)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)




