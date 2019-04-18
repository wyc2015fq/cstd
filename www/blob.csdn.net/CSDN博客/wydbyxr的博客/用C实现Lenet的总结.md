# 用C实现Lenet的总结 - wydbyxr的博客 - CSDN博客
2017年09月05日 20:50:50[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：590
根据博客http://blog.csdn.net/tostq/article/category/6290467所写的一些总结。
```
cnn.cpp cnn.h 存在关于CNN网络的函数、网络结构等
   minst.cpp minst.h 处理Minst数据库的函数、数据结构等；包括输入和输出
   mat.cpp mat.h 一些关于矩阵的函数，如卷积操作、180度翻转操作等
   main.cpp 主函数和测试函数
```
cnn.h即博客2上半部分（包括 CNN*）
minst.h即博客1
和LeNet-5相比主要有以下三点不同：
（1）LeNet-5给输入图像增加了一圈黑边，使输入图像大小变成了32x32，这样的目的是为了在下层卷积过程中保留更多原图的信息。
（2）LeNet-5的卷积层C3只有16个模板，得到16个输出，而本文的卷积层C3由于是全连接，所以有6*12个模板，得到12个输出图像
（3）LeNet-5多了两种，分别是C5到F6的全连接神经网络层，和F6到OUTPUT高斯连接网络层。而本文的直接由采样层S4直接经过一层全连接神经网络层到OUTPUT。
两个函数，翻转其中一个，再滑动求积分，叫卷积（convultion）；不翻转就滑动求积分，叫做互相关（cross-correlation）。如果其中之一是偶函数，那么卷积和互相关效果相同。从定义上看，翻转这个操作就是一步操作而已，具体的物理意义只能在应用中找到。
作者还留了许多接口，为今后的扩充准备，例如int poolType;//Pooling的方法
全连接层可以近似等于1*1的卷积层
首先我们需要知道的是误差反向传播的学习方法，实际是梯度下降法求最小误差的权重过程。当然我们的目的是求误差能量关于参数（权重）的导数.
最大的难点不是在于bp，而是代码的理解
因为w是输入*输出，故在具体实现时w是二维的。这就是实现与理论的交互，两者一致（一一对应）但对应的rule需要不断的学习。
网络中没有补零
???*ReverseInt翻转头字节（仅针对英特尔处理器和其他低端机用户）
两个函数，翻转其中一个，再滑动求积分，叫卷积（convultion）；不翻转就滑动求积分，叫做互相关（cross-correlation）。如果其中之一是偶函数，那么卷积和互相关效果相同。从定义上看，翻转这个操作就是一步操作而已，具体的物理意义只能在应用中找到。
最直观的理解就是：卷积是拉链操作。请想象一条拉链：把它底端固定在一起，上边左右完全拉开，扯直，使得固定端处于中心，那么左边这半条的顶端，相对于右边半条来说完全相反。而当你保持其中一边不动，把拉链拉起来的操作，会使得另一边翻转过来（当然拉链其实是旋转），也就是乘了 -1。
以信号处理为例，卷积意味着把输入信号在时间轴上翻转，然后跟信号处理系统的描述方程（冲激响应）叠加积分。为什么要翻转？因为这样才符合现实：输入信号的 0 秒先跟冲激响应的 0 秒叠加，然后输入信号的 1 秒和冲激响应的 1 秒叠加，以此类推。当你把这两个函数分别画出来上下并列的时候，它们就好象合并的拉链，0 点处在同一侧，而卷积实际上是要把它们画在同一个轴上滑动，同时却必须保证输入信号的 0 点先遇到冲激响应函数的 0 点——怎么办呢？就好像拉链被拉开了：翻转一下。
#########################################################################################################################
#初始化cnnsetup
inputsize.r是高（行）；.c是宽（列）
cnn->O5->outputNum//输出数据的数目
typedef struct MinstImg{
int c;           // 图像宽
int r;           // 图像高
float** ImgData; // 图像数据二维动态数组
}MinstImg;
typedef struct MinstImgArr{
int ImgNum;        // 存储图像的数目
MinstImg* ImgPtr;  // 存储图像数组指针
}*ImgArr;              // 存储图像数据的数组
typedef struct MinstLabel{
int l;            // 输出标记的长
float* LabelData; // 输出标记数据
}MinstLabel;
typedef struct MinstLabelArr{
int LabelNum;
MinstLabel* LabelPtr;
}*LabelArr;              // 存储图像标记的数组
C1；它的初始化前要赋初值28*28；即将struct Covlayer分配好空间（使用malloc和calloc麦捞克）在过程中，权重空间的初始化：先行再列调用，[r][c]；这里是一个四维数组（和计算参数量时一致）；赋值随机数float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2;covL->mapData[i][j][r][c]=randnum*sqrt((float)6.0/(float)(mapSize*mapSize*(inChannels+outChannels)));
权重的梯度变化的初始化：dmapData，赋值为0
basi偏置的初始化：赋值为0
激活函数和局部梯度的初始化：赋值为0
typedef struct convolutional_layer{
int inputWidth;   //输入图像的宽
int inputHeight;  //输入图像的长
int mapSize;      //特征模板的大小，模板一般都是正方形
```
int inChannels;   //输入图像的数目  
int outChannels;  //输出图像的数目  
// 关于特征模板的权重分布，这里是一个四维数组  
// 其大小为inChannels*outChannels*mapSize*mapSize大小  
// 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式  
// 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接  
float**** mapData;     //存放特征模块的数据  
float**** dmapData;    //存放特征模块的数据的局部梯度  
float* basicData;   //偏置，偏置的大小，为outChannels  
bool isFullConnect; //是否为全连接  
bool* connectModel; //连接模式（默认为全连接）  
// 下面三者的大小同输出的维度相同  
float*** v; // 进入激活函数的输入值  
float*** y; // 激活函数后神经元的输出  
// 输出像素的局部梯度  
float*** d; // 网络的局部梯度,δ值
```
}CovLayer;
采样层S2，avgPooling；它是不需要赋初值的，只需要开辟空间
只需要对d、y赋零
// 采样层 pooling
typedef struct pooling_layer{
int inputWidth;   //输入图像的宽
int inputHeight;  //输入图像的长
int mapSize;      //特征模板的大小
```
int inChannels;   //输入图像的数目  
int outChannels;  //输出图像的数目  
int poolType;     //Pooling的方法  
float* basicData;   //偏置  
float*** y; // 采样函数后神经元的输出,无激活函数  
float*** d; // 网络的局部梯度,δ值
```
}PoolLayer;
卷积层C3，同C1很类似,但这里是全连接
采样层S4，同S2很类似
;
输出层O5的处理  ， 首先需要将前面的多维输出展开成一维向量  ；需要init weight，且赋值过程需要约束到[-1,1]这个区间
float** wData; // 权重数据，为一个inputNum*outputNum大小
float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2; //gen a rand num in [-1,1]outL->wData[i][j]=randnum*sqrt((float)6.0/(float)(inputNum+outputNum))
// 输出层 全连接的神经网络
typedef struct nn_layer{
int inputNum;   //输入数据的数目
int outputNum;  //输出数据的数目
```
float** wData; // 权重数据，为一个inputNum*outputNum大小  
float* basicData;   //偏置，大小为outputNum大小  
// 下面三者的大小同输出的维度相同  
float* v; // 进入激活函数的输入值  
float* y; // 激活函数后神经元的输出  
float* d; // 网络的局部梯度,δ值  
bool isFullConnect; //是否为全连接
```
}OutLayer;
#################################################################################################################traincnn
##cnnff：
I)C1
1.赋值；
2~5是for outChannels
2.cov；3.addmat；4.free(mapout)，即mapout=cover（）中间变量。这几个是同一个for inChannels
5.activation_Sigma
```
2.cov中
cov：rotate180+correlation（有先后顺序）
rotate180：outputData[r][c]=mat[outSizeH-r-1][outSizeW-c-1]
correlation：有三种类型。首先，按full的来（计算得到变大了mapsize个的output），matEdgeExpand（给二维矩阵边缘扩大，增加addw大小的0值边，）；	
再根据type选择，matEdgeShrink（ 给二维矩阵边缘缩小，擦除mapsize大小的边）
5.activation_Sigma
temp=input（这个）+bas;//初始偏置bas是0
return (float)1.0/((float)(1.0+exp(-temp)));
```
II)S2
1.赋值；
2.avgPooling函数，
验错：if(outputSize.c!=outputW||outputSize.r!=outputH)
printf(“ERROR: output size is wrong!!”)
4个for
采样层没有激活函数，所以φ的导数为1
III)O5
1.flat to 1D vector；即O5inData[i*outSize.r*outSize.c+r*outSize.c+c]=cnn->S4->y[i][r][c];2.nnff函数，即E（w*05inData）+b
3.activation_Sigma：wData设置时就是二维的，这与卷积层不同；这时全连接
4.free(O5inData)
##cnnbp，这函数只是计算各层误差梯度δ
I)将误差保存到网络中
cnn->e[i]=cnn->O5->y[i]-outputData[i]
II)δ5。局域梯度δ5=-sig`()
III)δ4。输出层O5传递到S4层的局域梯度δ4值,代码级就是乘累加的过程；即相当于与其相连的下层输出的局域梯度δ5值乘上相连权重w5的总和
IV)δ3。
S4是平均Pooling层，S4的输出就是该像素邻域内的所有像素的平均值；即u4=（1/4）*（y3的和）；δ3=（1/S）*（δ4*sig`）
代码级：
1.上采样：C3e=UpSample（）。即宽的扩充，res[j][i+m]=mat[j/upr][i/upc]；再高的扩充，res[j+n][i]=res[j][i]。这个是矩阵的上采样（等值内插），upc及upr是内插倍数
2.计算：cnn->C3->d[i][r][c]=C3e[r][c]*sigma_derivation(cnn->C3->y[i][r][c])/(float)(cnn->S4->mapSize*cnn->S4->mapSize);
sigma_derivation：即激活函数求导，y*(1-y);
V)δ2。S2层
1.correlation（）；卷积操作本来就包括了180的翻转，这里可以认为是反卷积即翻转360（不转），故直接使用correlation
2.addmat（）
VI）δ1。
so the diff（微分） to next neural’s diff is no change这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
即δ3一模一样。
##保存savecnndata
combine_strings（a，b）：保存到a的文件名下
intTochar：将数字（迭代的次数）转换成字符串
savecnndata：保存CNN网络中的相关数据；包括y、d、v、basicdata、mapData、wData
```
float* v; //进入激活函数的输入值  
float* y; // 激活函数后神经元的输出  
float* d; // 网络的局部梯度,δ值  
权重	
偏置
```
没有保存   float**** dmapData;    //存放特征模块的数据的局部梯度
## cnnapplygrads
这里存在权重的主要是卷积层和输出层， 更新这两个地方的权重就可以了
I)C1:
权重：  float** flipinputData=rotate180(inputData,ySize);//inputData翻转
float** C1dk=cov(cnn->C1->d[i],dSize,flipinputData,ySize,valid);//inputdata根据d进行卷积
multifactor(C1dk,C1dk,mapSize,-1*opts.alpha);// 矩阵乘以系数；即△w*alpha
addmat
偏置：  cnn->C1->basicData[i]=cnn->C1->basicData[i]-opts.alpha*summat(cnn->C1->d[i],dSize);矩阵各元素的和
使用参数：inputData、alpha、d、basicData、mapdata
II)C3相似：
使用参数：inputData变成y
III)O5：
首先需要将前面的多维输出展开成一维向量
O5inData[i*outSize.r*outSize.c+r*outSize.c+c]=cnn->S4->y[i][r][c];cnn->O5->wData[j][i]=cnn->O5->wData[j][i]-opts.alpha*cnn->O5->d[j]*O5inData[i];//w=w-alpha*d*y
使用参数：y、wData、basicData、alpha
##cnnclear
d、v、y重置为0.0
##计算并保存误差能量
设l
l=l+e*eL(n)=E（0.99*L(n-1)+0.01*l/2）
float* e; // 训练误差；位于输出层，cnn->e[i]=cnn->O5->y[i]-LabelData[i]
float* L; // 瞬时误差能量；代表整个网络的误差
###############################################################################################savecnn
I)保存的是train完模型，这点与之前的savecnndata（保存该次迭代的局部值，以供更新使用）不同。
写文件到output/minst.cnn中
写参数，即C1、C3、O5的权重和偏置
II)保存误差
将cnn->L保存到output/cnn_loss.ma文件
##############################################################################################testcnn
I)设初值
testNum=10000;
float incorrectRatio=0.0;
II）导入importcnn(cnn,“output/minst.cnn”)
将savecnn得到的.cnn读取到变量cnn中（即C1、C3、O5的w和b）；
注意L不读取
III)cnntest（）
1.赋初值，incorrectnum=0;  //错误预测的数目
for（testNum）
2.cnnff(），即train中的前向传播部分（完全一致）
3.得出测试结果
if(vecmaxIndex(cnn->O5->y,cnn->O5->outputNum)!=vecmaxIndex(outputData->LabelPtr[n].LabelData,cnn->O5->outputNum))
incorrectnum++;
vecmaxIndex(float* vec, int veclength)// 返回向量最大数的序号
```
4.cnnclear(），d、v、y重置为0.0
```
end
IV)printf最终结果
accuracy:%f. test finished!!\n",1-incorrectRatio
