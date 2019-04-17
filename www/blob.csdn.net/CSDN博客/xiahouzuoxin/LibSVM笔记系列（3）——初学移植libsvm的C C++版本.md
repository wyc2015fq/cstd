# LibSVM笔记系列（3）——初学移植libsvm的C/C++版本 - xiahouzuoxin - CSDN博客





2013年07月19日 11:08:26[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：14313
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)









在[LibSVM笔记系列（1）](http://blog.csdn.net/xiahouzuoxin/article/details/9369131)中已经提到在g++环境中编译LibSVM只需要一个make命令那样简单。

本文将介绍

（1）LibSVM的编译文件结构

（2）svm.h中重要数据结构及函数分析

（3）svm_train.c 和 svm_predict.c源代码分析

（4）如何将LibSVM移植到eclipse IDE环境中




***1. LibSVM编译文件结构***

要了解一个工程是如何编译链接的，最好的办法是参考Makefile。

CXX ?= g++

CFLAGS = -Wall -Wconversion -O3 -fPIC

SHVER = 2

OS = $(shell uname)


all: svm-train svm-predict svm-scale


lib: svm.o
if [ "$(OS)" = "Darwin" ]; then \
SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,libsvm.so.$(SHVER)"; \
else \
SHARED_LIB_FLAG="-shared -Wl,-soname,libsvm.so.$(SHVER)"; \
fi; \
$(CXX) $${SHARED_LIB_FLAG} svm.o -o libsvm.so.$(SHVER)


svm-predict: svm-predict.c svm.o
$(CXX) $(CFLAGS) svm-predict.c svm.o -o svm-predict -lm

svm-train: svm-train.c svm.o
$(CXX) $(CFLAGS) svm-train.c svm.o -o svm-train -lm

svm-scale: svm-scale.c
$(CXX) $(CFLAGS) svm-scale.c -o svm-scale

svm.o: svm.cpp svm.h
$(CXX) $(CFLAGS) -c svm.cpp

clean:
rm -f *~ svm.o svm-train svm-predict svm-scale libsvm.so.$(SHVER)


tags:
ctags --c++-kinds=+p --fields=+iaS --extra=+q -R 



从上面的Makefile可以看出，之前的make命令执行的是all目标，all目标包括svm-train, svm-predict和svm-scale三个依赖，因此使用make编译后的可执行文件有svm-train, svm-predict和svm-scale共3个。

下图描述了生成svm-predict和svm-train目标的过程，支持向量机的所有相关的核心代码在svm.cpp中实现，svm_train.c和svm_predict.c中只是提供了训练和预测的主函数接口而已。我们将svm修改移植时只需要仿照svm_train.c或svm_predict.c进行修改就可以了。

![](https://img-blog.csdn.net/20130719104333562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


****2. svm.h中重要数据结构及函数分析（参考README）****


大部分函数在头文件“svm.h”，因此在自己的C/C++文件中必须#include "svm.h"，而且

要将svm.cpp一同编译到自己的程序当中。可以参考svm-train.c和svm-predict.c。在

svm.h中定义了LIBSVM_VERSION以及声明了'extern int libsvm_version'。


在对测试数据分类前，需要使用训练数据构建一个'svm_model'，这个模型可以存到文件

中或立即被使用。


- Function: struct svm_model *svm_train(const struct svm_problem *prob,
const struct svm_parameter *param);


    This function constructs and returns an SVM model according to

    the given training data and parameters.


    struct svm_problem

    {

        int l;                 // 训练数据的数量

        double *y;             // 包含标签值的数组

        struct svm_node **x;   // 指针数组，每一个指针指向一个训练的特征向量

    };



    比如，数据如下:


    LABELATTR1ATTR2ATTR3ATTR4ATTR5

    ------------------------------

      1 0
 0.1 0.2
 0 0

      2 0
 0.1 0.3
-1.2 0

      1 0.4
 0 0 0 0

      2 0
 0.1 0
 1.4 0.5

      3-0.1
-0.2 0.1
 1.1 0.1


    则svm_problem结构体成员的值如下:


    l = 5


    y -> 1 2 1 2 3


    x -> [ ] -> (2,0.1) (3,0.2) (-1,?)
[ ] -> (2,0.1) (3,0.3) (4,-1.2) (-1,?)
[ ] -> (1,0.4) (-1,?)
[ ] -> (2,0.1) (4,1.4) (5,0.5) (-1,?)
[ ] -> (1,-0.1) (2,-0.2) (3,0.1) (4,1.1) (5,0.1) (-1,?)



   注意x，其中的(index,value)存储在'svm_node'结构体中：


   struct svm_node
{
int index;         // 特征向量中非0元素的索引
double value;      // 索引值位置对应的特征值
};        


   index = -1 表示向量的结束。注意，index的值是递增的。


   struct svm_parameter 描述了SVM model的参数：

struct svm_parameter
{
int svm_type;
int kernel_type;
int degree;
/* for poly */
double gamma;
/* for poly/rbf/sigmoid */
double coef0;
/* for poly/sigmoid */

/* these are for training only */
double cache_size; /* in MB */
double eps;
/* stopping criteria */
double C;
/* for C_SVC, EPSILON_SVR, and NU_SVR */
int nr_weight;/* for C_SVC */
int *weight_label;/* for C_SVC */
double* weight;/* for C_SVC */
double nu;
/* for NU_SVC, ONE_CLASS, and NU_SVR */
double p;
/* for EPSILON_SVR */
int shrinking;/* use the shrinking heuristics */
int probability; /* do probability estimates */
};


    svm_type 可以为C_SVC, NU_SVC, ONE_CLASS, EPSILON, NU_SVR.


    C_SVC:C-SVM classification

    NU_SVC:nu-SVM classification

    ONE_CLASS:one-class-SVM

    EPSILON_SVR:epsilon-SVM regression

    NU_SVR:nu-SVM regression


    kernel_type可以为LINEAR, POLY, RBF, SIGMOID.


    LINEAR:u'*v

    POLY:(gamma*u'*v + coef0)^degree

    RBF:exp(-gamma*|u-v|^2)

    SIGMOID:tanh(gamma*u'*v + coef0)

    PRECOMPUTED: kernel values in training_set_file


    注意：在还要使用svm_train返回的svm_model的情况下，不能将svm_problem的内存空间释放，

    因为svm_model中含有指向svm_problem结构的指针。


    注意：为了避免传入错误的参数，应该在使用svm_train之前使用svm_check_parameter()检验。


    svm_model存储了训练过程获得的模型，不建议直接访问该结构体中的实体，而应该使用接口

    函数获得结构体中成员的值。


struct svm_model
{
struct svm_parameter param;/* parameter */
int nr_class;
/* number of classes, = 2 in regression/one class svm */
int l;
/* total #SV */
struct svm_node **SV;   /* SVs (SV[l]) */
double **sv_coef;/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
double *rho;
/* constants in decision functions (rho[k*(k-1)/2]) */
double *probA;/* pairwise probability information */
double *probB;
int *sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

/* for classification only */

int *label;
/* label of each class (label[k]) */
int *nSV;
/* number of SVs for each class (nSV[k]) */
/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
/* XXX */
int free_sv;
/* 1 if svm_model is created by svm_load_model*/
/* 0 if svm_model is created by svm_train */
};



    SV和sv_coef是支持向量和对应的系数。假设有k类，对于第j类中的数据，对应的sv_coef包括

    支持向量的系数是一个 (k-1)y*alpha 的向量，比如有4类，则sv_coef和SV如下：


        +-+-+-+--------------------+

        |1|1|1|                    |

        |v|v|v|  SVs from class 1  |

        |2|3|4|                    |

        +-+-+-+--------------------+

        |1|2|2|                    |

        |v|v|v|  SVs from class 2  |

        |2|3|4|                    |

        +-+-+-+--------------------+

        |1|2|3|                    |

        |v|v|v|  SVs from class 3  |

        |3|3|4|                    |

        +-+-+-+--------------------+

        |1|2|3|                    |

        |v|v|v|  SVs from class 4  |

        |4|4|4|                    |

        +-+-+-+--------------------+


    参考svm_train()，其中给sv_coef赋了值。


    rho是平衡项（-b）。probA和probB是用于概率输出的参数。如果有k类，则有k*(k-1)/2

    个二元分类问题，按照如下的方式对齐：


    1 vs 2, 1 vs 3, ..., 1 vs k, 2 vs 3, ..., 2 vs k, ..., k-1 vs k.


    sv_indices[0,...,nSV-1]值在[1,...,num_training_data]范围内，标识训练数据集中的支持向量。


    label 包含了训练数据集中的标签。


    nSV包含了训练数据集中每类的支持向量数。


    free_sv是一个标志，决定是否在free_model_content(struct svm_model*)中

    和free_and_destroy_model(struct svm_model**)中将SV的空间释放。如果SV是由svm_train()得到

    的，则设置为0，不能释放，如果是通过svm_load_model加载的，则设置为1。


- Function: double svm_predict(const struct svm_model *model,

                               const struct svm_node *x);


    For a classification model, the predicted class for x is returned.

    For a regression model, the function value of x calculated using

    the model is returned. For an one-class model, +1 or -1 is

    returned.



***3. svm_train.c的源代码分析***


注意，虽然看到的是.c的文件，但里面实际使用了不少C++特有的东西，因此，在后面eclipse移植过程中最好“新建C++”工程。

![](https://img-blog.csdn.net/20130719105644187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


svm_predict.c的代码分析类似，svm_predict的过程比svm_train更简单。



****4. 将LibSVM移植到eclipse IDE的过程****

前提要求：配置好的eclipse C/C++环境，或者使用VC环境也一样

移植文件：svm.cpp  svm.h  svm-train.c

拷贝数据文件：heart_scale




（1）新建C++工程

（2）将“移植文件”拷贝到工程目录并添加到工程

（3）添加命令行参数，eclipse中，在“Run Configurations”的Arguments选项卡中添加heart_scale heart_scale.model。

![](https://img-blog.csdn.net/20130719115942984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



（4）编译运行结果，与make结果相同

*

optimization finished, #iter = 162

nu = 0.431029

obj = -100.877288, rho = 0.424462

nSV = 132, nBSV = 107

Total nSV = 132










