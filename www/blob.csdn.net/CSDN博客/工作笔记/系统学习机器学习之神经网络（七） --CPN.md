# 系统学习机器学习之神经网络（七） --CPN - 工作笔记 - CSDN博客





2017年01月09日 10:55:40[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5599








原文：http://blog.csdn.net/u014070279/article/details/47299987

1987年，美国学者Robert Hecht-Nielsen提出了对偶传播神经网络模型（Counter Propagation Network，CPN），CPN最早是用来实现样本选择匹配系统的。CPN 网能存储二进制或模拟值的模式对，因此这种网络模型也可用于联想存储、模式分类、函数逼近、统计分析和数据压缩等用途。

# 1.    网络结构与运行原理

网络结构如图所示，各层之间的神经元全互联连接。从拓扑结构看，CPN网与三层BP网络相近，但实际上CPN是由自组织网和Grossberg外星网组合而成。隐层为竞争层，采用无监督的竞争学习规则，而输出层为Grossberg层（见[系统学习机器学习之神经网络（六） --GrossBerg网络](http://blog.csdn.net/app_12062011/article/details/54287051)），采用有监督信号的Widrow-Hoff规则或Grossberg规则学习。

![](https://img-blog.csdn.net/20170109105403671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


网络各层按两种学习规则训练好之后，运行阶段首先向网络送入输入变量，隐含层对这些输入进行竞争计算，获胜者成为当前输入模式类的代表，同时该神经元成为如下图(a)所示的活跃神经元，输出值为1而其余神经元处于非活跃状态，输出值为0。竞争取胜的隐含神经元激励输出层神经元，使其产生如下图(b)所示的输出模式。由于竞争失败的神经元输出为0，不参与输出层的整合。因此输出就由竞争胜利的神经元的外星权重确定。

![](https://img-blog.csdn.net/20170109105418968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 2.    学习算法

网络学习分为两个阶段：第一阶段是竞争学习算法对隐含层神经元的内星权向量进行训练；第二阶段是采用外星学习算法对隐含层的神经元的外星权向量进行训练。

因为内星权向量采用的是竞争学习规则，跟前几篇博文所介绍的算法步骤基本类似，这里不做介绍，值得说明的是竞争算法并不设置优胜临域，只对获胜神经元的内星权向量进行调节。

下面重点介绍一下外星权向量的训练步骤:

（1）   输入一个模式以及对应的期望输入，计算网络隐节点净输入，隐节点的内星权向量采用上一阶段中训练结果。

（2）   确定获胜神经元使其输出为1。

（3）   调整隐含层到输出层的外星权向量，调整规则如下：

![](https://img-blog.csdn.net/20170109105431187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


β为外星规则学习速率，为随时间下降的退火函数。O(t)为输出层神经元的输出值。

    由以上规则可知，只有获胜神经元的外星权向量得到调整，调整的目的是使外星权向量不断靠近并等于期望输出，从而将该输出编码到外星权向量中。

# 3.    改进CPN网

## （1）   双获胜神经元CPN

指的是在完成训练后的运行阶段允许隐层有两个神经元同时竞争获得胜利，这两个获胜神经元均取值为1，其他神经元则取值为0。于是有两个获胜神经元同时影响网络输出。下图给出了一个例子，表明了CPN网能对复合输入模式包含的所有训练样本对应的输出进行线性叠加，这种能力对于图像的叠加等应用十分合适。

![](https://img-blog.csdn.net/20170109105446281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## （2）   双向CPN网

将CPN网的输入层和输出层各自分为两组，如下图所示。双向CPN网的优点是可以同时学习两个函数，例如：Y＝f（X）；X′＝f（Y′）

![](https://img-blog.csdn.net/20170109105500000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当两个函数互逆时，有X ＝X′，Y＝Y′。双向CPN可用于数据压缩与解压缩，可将其中一个函数f作为压缩函数，将其逆函数g作为解压缩函数。

事实上，双向CPN网并不要求两个互逆函数是解析表达的，更一般的情况是f和g是互逆的映射关系，从而可利用双向CPN实现互联想。

# 4.    CPN网应用

下图给出了CPN网用于烟叶颜色模式分类的情况，输入样本分布在下图(a)所示的三维颜色[空间](http://www.07net01.com/tags-%E7%A9%BA%E9%97%B4-0.html)中，该空间的每个点用一个三维向量表示，各分量分别代表烟叶的平均色调H，平均亮度L和平均饱和度S。可以看出颜色模式分为4类，分别对应红棕色，橘黄色，柠檬色和青黄色。下图(b)给出了CPN网络结构，隐层共设了10个神经元，输出层设4个神经元，学习速率为随训练时间下降的函数，经过2000次递归之后，网络分类的正确率达到96%。

![](https://img-blog.csdn.net/20170109105525281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






**CPN神经网络设计步骤如下：**

**![](https://img-blog.csdn.net/20150805165648989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150805165658618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**C代码如下：（本代码按照步骤完成，同时也对照MATLAB代码修改，但是并没有预测功能，只能判断已输入的样本属于哪一类。贴出代码，希望大牛能帮帮忙）**

#include "stdio.h"

#include "stdlib.h"

#include "math.h"

#include "time.h"

#include "vector"

using namespace std;



#define INF 99999

#define dimIn 4   //输入样本维数

#define dimOut 1  //期望输出样本维数 

#define competeNum 40  //竞争神经元数

#define maxLoop  6000   //训练次数

#define alpha 0.41    //输入层到竞争层的学习率

#define delta 0.32     //竞争层到输出层的学习率



typedef vector<double> doubleVector;



vector<doubleVector> dataIn;   //输入训练样本

vector<doubleVector> dataOut;  //期望输出样本

vector<doubleVector> W;     //输入层到竞争层的权值

vector<doubleVector> V;     //竞争层到输出层的权值

doubleVector B;  //二值输出向量

vector<doubleVector> Y;  //输出层输出向量



void getFileData(char *File1, char *File2);   //获取样本

double EucDistance(doubleVector X);   //计算欧式距离

void NormalizationX();   //输入样本归一化

void NormalizationW();   //连接权值归一化

void NormalizationV();   //输出层权值归一化

void Initialize();     //初始化权值

void CPN_Train();      //开始训练

int chooseWg(doubleVector X);  //选择权重与X距离最近的向量

void Use_CPN();  //使用CPN网络

doubleVector round(doubleVector src);   //四舍五入



//主函数

void main()

{
int i, j;
char *File1 = "in.txt";
char *File2 = "out.txt";


getFileData(File1, File2);  //获取样本


NormalizationX();  //输入样本归一化


srand(time(NULL));
Initialize();   //初始化权值


CPN_Train();      //开始训练

Use_CPN();
//使用CPN网络


}





//使用CPN网络

void Use_CPN()

{
int i, g;
double num, dis;
doubleVector test;
doubleVector out_sam;


while(1)
{
test.clear();
out_sam.clear();


printf("输入[测试](http://lib.csdn.net/base/softwaretest)样本:\n");
for(i=0; i<dimIn; i++)
{
scanf("%lf", &num);
test.push_back(num);
}


dis = EucDistance(test);
for(i=0; i<dimIn; i++)
test[i] = test[i]/dis;


g = chooseWg(test);


for(i=0; i<dimOut; i++)
out_sam.push_back(V[i][g]);


out_sam = round(out_sam);   //四舍五入


printf("测试结果：\n");
for(i=0; i<out_sam.size(); i++)
printf("%lf  ", out_sam[i]);


printf("\n\n");


}



}





//开始训练

void CPN_Train()

{
int i, g, k;
int t=0;  //计步器


double dis;


Y = dataOut;
while(t<maxLoop)
{
for(i=0; i<dataIn.size(); i++)
{
NormalizationW();
g = chooseWg(dataIn[i]);  //选择权值


//权值修正W
for(k=0; k<dataIn[i].size(); k++)
W[g][k] = W[g][k]+alpha*(dataIn[i][k]-W[g][k]);


//归一化
dis = EucDistance(W[g]);
for(k=0; k<dimIn; k++)
W[g][k] = W[g][k]/dis;




//修正权重V
for(k=0; k<dimOut; k++)
V[k][g] = V[k][g]+delta*(dataOut[i][k]-V[k][g]);



}


t++;  //计步器加1
}



}





//四舍五入

doubleVector round(doubleVector src)

{
int i;
doubleVector dst;


for(i=0; i<src.size(); i++)
{
if(src[i]-(int)src[i]>=0.5)
dst.push_back((int)src[i]+1);
else
dst.push_back((int)src[i]);


}

return dst;

}





//选择权重与X距离最近的向量

int chooseWg(doubleVector X)

{
int i, j, label;
double sum, max=0;


label = 0;
for(i=0; i<competeNum; i++)
{
sum = 0;
for(j=0; j<dimIn; j++)
sum += X[j]*W[i][j];


if(sum>max)
{
max = sum;
label = i;
}
}


return label;

}





//初始化权值

void Initialize()

{
int i, j;
doubleVector temp;


//输入层到竞争层的权值
for(i=0; i<competeNum; i++)
{
temp.clear();
for(j=0; j<dimIn; j++)
temp.push_back((double)(rand()%RAND_MAX)/RAND_MAX);
W.push_back(temp);
}


//竞争层到输出层的权值
for(i=0; i<dimOut; i++)
{
temp.clear();
for(j=0; j<competeNum; j++)
temp.push_back((double)(rand()%RAND_MAX)/RAND_MAX);
V.push_back(temp);
}

}





//连接权值归一化

void NormalizationW()

{
int i, j;
double dis;

for(i=0; i<competeNum; i++)
{
dis = EucDistance(W[i]);
for(j=0; j<dimIn; j++)
W[i][j] = W[i][j]/dis;
}



}





//计算欧式距离

double EucDistance(doubleVector X)

{
int i;
double sum=0;


for(i=0; i<X.size(); i++)
sum += X[i]*X[i];


sum = sqrtf(sum);


return sum;

}





//输入样本归一化

void NormalizationX()

{
int i, j;
double dis;


for(i=0; i<dataIn.size(); i++)
{
dis = EucDistance(dataIn[i]);
for(j=0; j<dimIn; j++)
dataIn[i][j] = dataIn[i][j]/dis;
}



}





//获取样本

void getFileData(char *File1, char *File2)

{
int i;
double num;


FILE *fp1 = fopen(File1, "r");
FILE *fp2 = fopen(File2, "r");


if(fp1==NULL || fp2==NULL)
{
printf("Open file error!!!\n");
exit(0);
}


doubleVector temp;


//输入的样本
i = 1;
temp.clear();
while(fscanf(fp1, "%lf", &num)!=EOF)
{
temp.push_back(num);
if(i%dimIn==0)
{
dataIn.push_back(temp);
temp.clear();
}
i++;
}


//输出的样本
i = 1;
temp.clear();
while(fscanf(fp2, "%lf", &num)!=EOF)
{
temp.push_back(num);
if(i%dimOut==0)
{
dataOut.push_back(temp);
temp.clear();
}
i++;
}

}



