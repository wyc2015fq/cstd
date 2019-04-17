# htk网络和解码源码（二、声学模型） - xmdxcsj的专栏 - CSDN博客





2015年05月24日 17:06:58[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1608








### (一)         定义

#### 1.    HMM参数

转移概率：5*5矩阵，每行的和为1，最后一行的所有转移概率为0

生成概率：分为两类，一类是continuous density models，使用混合高斯模型表示

![](https://img-blog.csdn.net/20150524170343318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

另一类是discrete probability distributions，

![](https://img-blog.csdn.net/20150524170316158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 2.    HMM宏定义

Hmm在加载完一个hmm以后，会进行相关check，比如说高斯权重和为1等，通过配置文件设置CHKHMMDEFS为false将不会进行该类检查。

Stream：特征输入可以分成多个stream输入，比如维度为13维的mfcc特征，抽取的13维的特征可以分成两个stream输入，其中一个stream包含10维特征，另一个stream包含另外3维特征。详细参数含义见htkbook（112页）。

其他macro定义如下：

*~*s shared state distribution

*~*m shared Gaussian mixture component

*~*u shared mean vector

*~*v shared diagonal variance vector

*~*i shared inverse full covariance matrix

*~*c shared Choleski *L*T matrix

*~*x shared arbitrary transform matrix4

*~*t shared transition matrix

*~*d shared duration parameters

*~*w shared stream weight vector



*~*l logical HMM *~*h physical HMM

*~*o globaloption values *~*p tied mixture

*~*r regressionclass tree *~*j lineartransform

Hmm的定义可以分别存在多个文件中，比如mf1和mf2都可以存放模型的一部分定义，hlist用于存放HMM的名称，一行一个。如果存在没有定义的hmm，则尝试打开以该hmm命名的文件寻找相关的定义：

![](https://img-blog.csdn.net/20150524170407716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Tiedlist：Tiedlist中的每一行的第一个为logicalhmm（多个logical hmm和physical hmm的定义是一致的，用于共享hmm定义，相当与定义相同的hmm聚类合并成一个），第二个为physical hmm（是hmm存储的表示）。如果一行只有一个，则认为logical和physical是一样的。特点是无需进行log计算，实时解码速度快，准确率应该不如高斯。

#### 3.     Hmm set的种类

Hmm set的定义有以下几种：

l  PLAINHS：Otherwise

l  TIEDHS：If allmixture components are tied

高斯模型的定义是共享的，如下图：

![](https://img-blog.csdn.net/20150524170421179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

l  SHAREDHS：If itcontains any shared states (*~*s macros) orGaussians (*~*m macros)

l  DISCRETEHS：HMM setcontaining discrete output distributions

概率模型是离散的，相比于混合高斯连续函数而言的。离线模型对于输入symbols有固定的概率值，输入symbols的范围1至M，每次计算概率只需要查表操作即可。针对语音输入vector，找到与之最为贴近的symbol。

#### 4.    Input Linear Transforms

对特征输入进行线性变化，比如特征的降维变化，将原来的5维的MFCC经过矩阵变化为2维的MFCC。关键字是<InputXform>，可以通过设置SAVEINPUTXFORM= FALSE来设定model不保存线性变化。

#### 5.    transition

![](https://img-blog.csdn.net/20150524170610622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### (二)         程序实现

#### 数据结构

HLink：表示一个hmm

Mlink：hmm里面的定义单元，单元类型由type决定。有可能是一个hmm

#### tiedlist

MakeHMMSet函数调用InitHMMSet：读取tiedlist，初始化hset中的mtab

Tiedlist中的每一行的第一个为logicalhmm，第二个为physical hmm

1.      将读取的字符串放到hashtable这个全局变量中，获得labelid

2.      CreateHMM(hset,lId,pId)

将三音素放入到hset中的mtab中

Logical hmm =>”l”

Physical hmm=>”h”

#### hmmdef

读取三音素对应的状态的实现在GetHMMDef函数，将读取的状态存在HLink结构体，HLink存放在MLink的structure变量，存在hmmset的mtab数组。

1.      读取函数LoadHMMSet

首先调用LoadMacroFiles读取-H参数后面跟的hmmdef文件；然后根据根据三音素逐个读取其对应的hmmdef文件（以三音素的名字命名）。



forceHSKind：如果配置文件设置了HMMSETKIND，该参数为true。



2.      SetIndex函数：设置状态编号

GoNextState：如果nUse为正，表示unseen，通过设置nUse=-nUse将该state设置为seen

StateInfo里面的sIdx表示状态的id

MixPDF里面的mIdx表示高斯的序号。

遍历全局hash表mtab，如果存放的定义类型为状态’s’，将其sIdx进行依次加一；如果存放的定义类型为高斯模型’m’，将其mIdx进行依次加一



