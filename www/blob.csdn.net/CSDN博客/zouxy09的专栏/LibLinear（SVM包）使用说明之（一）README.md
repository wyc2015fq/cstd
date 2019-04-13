
# LibLinear（SVM包）使用说明之（一）README - zouxy09的专栏 - CSDN博客


2013年09月02日 19:53:45[zouxy09](https://me.csdn.net/zouxy09)阅读数：71446


**LibLinear（SVM包）使用说明之（一）README**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
本文主要是翻译liblinear-1.93版本的README文件。里面介绍了liblinear的详细使用方法。更多信息请参考：
[http://www.csie.ntu.edu.tw/~cjlin/liblinear/](http://www.csie.ntu.edu.tw/~cjlin/liblinear/)
在这里我用到的是LibLinear的Matlab接口，这个在下一博文中介绍。
LIBLINEAR是一个简单的求解大规模规则化线性分类和回归的软件包。它最讨人喜欢的特点就是速度快！！！它目前支持：
- L2-regularized logisticregression/L2-loss support vector classification/L1-loss support vectorclassification
- L1-regularized L2-loss support vectorclassification/L1-regularized logistic regression
- L2-regularized L2-loss support vectorregression/L1-loss support vector regression.
**一、何时用LIBLINEAR，而不是LIBSVM**
对一些大数据来说，有没有非线性映射，他们的性能差不多。如果不使用核，我们可以用线性分类或者回归来训练一个更大的数据集。这些数据往往具有非常高维的特征。例如文本分类Document classification。
注意：一般来说，liblinear是很快的，但它的默认配置在某些情况下可能会比较慢，例如数据没有经过scale或者C很大的时候。可以参考SVM的说明中的附录B来处理这些情况：
http://www.csie.ntu.edu.tw/~cjlin/papers/guide/guide.pdf
注意：如果你是个新手，而且你的数据集不是很大，那么建议你先考虑LIBSVM。
LIBSVM page:
http://www.csie.ntu.edu.tw/~cjlin/libsvm
更多也可以参考：
[liblinear与libsvm选择](http://blog.163.com/pz124578@126/blog/static/23522694201312124221734/)
[Libsvm和Liblinear的使用经验谈](http://blog.sina.com.cn/s/blog_5b29caf7010127vh.html)
LIBLINEAR: Alibrary for large linear classification
**二、快速入门**
按“安装”这一章节的说明来安装LIBLINEAR。安装完成后，就会得到两个程序，train和predict，分别是用来训练分类器和测试分类器的两个程序。
对于数据格式。请查看LIBSVM的README文件。需要注意的是，特征的索引是从1开始的，而不是0 。
这个包还包含了一个例子，分类的数据是`heart_scale'。
运行`trainheart_scale'，train程序就会读取对应的训练数据，然后输出训练好的分类器模型`heart_scale.model'。如果你有一个测试集，命名为heart_scale.t，那么你可以运行`predict heart_scale.t heart_scale.model output'来测试该分类器的识别准确率。这个output文件包含了分类器对测试集中每个样本预测得到的对应的类标签。
为了获得好的性能，有时候需要先对数据进行scale。可以查看LIBSVM的`svm-scale'程序来获得相关的信息。对于非常大和稀疏的数据，使用参数`-l 0'来保持训练中数据的稀疏性。
**建议的训练步骤（来源于libSVM，感觉对这个也有帮助，所以也标记在这）：**
1）将我们的训练数据和测试数据转换为该SVM软件包支持的格式；
2）对数据进行简单的尺度化scale；
3）先考虑用RBF核；
4）使用交叉检验方法去寻找最优的参数C和γ；
5）使用找到的最好的参数C和γ来训练整个训练集；
6）在测试集上测试。
**三、安装**
在Unix系统，使用make来编译`train' 和 `predict'程序。编译完后，直接无参数运行这两个程序，就会打印出相应的用法。
对于其他系统，我们通过Makefile来编译。例如在windows下，我们可以参考本文中的'BuildingWindows binaries'章节。或者可以直接使用我们预编译好的可执行文件。Windows的可执行文件在windows目录下。
本软件需要一些level-1 BLAS 子程序才能运行（BLAS（Basic LinearAlgebra Subprograms，基础线性代数程序集）是一个应用程序接口（API）标准，用以规范发布基础线性代数操作的数值库（如矢量或矩阵乘法）。该程序集最初发布于1979年，并用于建立更大的数值程序包（如LAPACK）。在高性能计算领域，BLAS被广泛使用。BLAS按照功能被分为三个级别，Level 1为矢量-矢量运算）。这些需要的函数已经包含在该软件包的blas目录里了。如果你的电脑已经存在了一个BLAS库，那么你可以通过修改Makefile文件来使用你电脑上的BLAS库。
注销以下行：
\#LIBS ?= -lblas
然后添加以下行：
LIBS ?= blas/blas.a
**四、程序train的用法**
**用法：**train [options] training_set_file [model_file]
**options:**
-s type : 对于多分类，指定使用的分类器（默认是1）：
0 -- L2-regularized logistic regression(primal)
1 -- L2-regularized L2-loss support vectorclassification (dual)
2 -- L2-regularized L2-loss support vectorclassification (primal)
3 -- L2-regularized L1-loss support vectorclassification (dual)
4 -- support vector classification by Crammerand Singer
5 -- L1-regularized L2-loss support vectorclassification
6 -- L1-regularized logistic regression
7 -- L2-regularized logistic regression (dual)
对于回归：
11-- L2-regularized L2-loss support vector regression (primal)
12-- L2-regularized L2-loss support vector regression (dual)
13-- L2-regularized L1-loss support vector regression (dual)
-c cost : 设置参数 C（默认是1）
-p epsilon : 设置epsilon-SVR的损失函数的参数epsilon（默认是0.1）
-e epsilon : 设置迭代终止条件的容忍度tolerance
-s0 and 2
|f'(w)|_2<= eps*min(pos,neg)/l*|f'(w0)|_2,
f是primal 函数，pos/neg 是对应的正样本和负样本数目（默认是0.01）
-s11
|f'(w)|_2<= eps*|f'(w0)|_2 （默认是0.001）
-s1, 3, 4 and 7
Dualmaximal violation <= eps; 和 libsvm相似（默认是0.1）
-s5 and 6
|f'(w)|_inf<= eps*min(pos,neg)/l*|f'(w0)|_inf,
f是primal 函数，pos/neg 是对应的正样本和负样本数目（默认是0.01）
-s12 and 13\n"
|f'(alpha)|_1<= eps |f'(alpha0)|,
f是dual（对偶）函数（默认是0.1）
-B bias : 如果bias >= 0，那样样本x变为[x; bias]，如果小于0，则不增加bias项（默认是-1）
-wi weight: 调整不同类别的参数C的权值（具体见README）
-v n: n-fold交叉检验模式。它随机的将数据划分为n个部分，然后计算它们的交叉检验准确率。
-q : 安静模式（无输出信息）
**Formulations公式（优化问题）:**
For L2-regularized logistic regression (-s0), we solve
min_w w^Tw/2 + C\sum log(1 + exp(-y_i w^Tx_i))
For L2-regularized L2-loss SVC dual (-s 1),we solve
min_alpha  0.5(alpha^T (Q + I/2/C) alpha) - e^T alpha
s.t.   0 <= alpha_i,
For L2-regularized L2-loss SVC (-s 2), wesolve
min_w w^Tw/2 + C\sum max(0, 1- y_i w^Tx_i)^2
For L2-regularized L1-loss SVC dual (-s 3),we solve
min_alpha  0.5(alpha^T Q alpha) - e^T alpha
s.t.   0 <= alpha_i <= C,
For L1-regularized L2-loss SVC (-s 5), wesolve
min_w \sum |w_j|+ C \sum max(0, 1- y_i w^Tx_i)^2
For L1-regularized logistic regression (-s6), we solve
min_w \sum |w_j|+ C \sum log(1 + exp(-y_i w^Tx_i))
For L2-regularized logistic regression (-s7), we solve
min_alpha  0.5(alpha^T Q alpha) + \sumalpha_i*log(alpha_i) + \sum (C-alpha_i)*log(C-alpha_i) - a constant
s.t.   0 <= alpha_i <= C,
where, Q is a matrix with Q_ij = y_i y_jx_i^T x_j.
For L2-regularized L2-loss SVR (-s 11), wesolve
min_w w^Tw/2 + C\sum max(0, |y_i-w^Tx_i|-epsilon)^2
For L2-regularized L2-loss SVR dual (-s12), we solve
min_beta  0.5(beta^T (Q + lambda I/2/C) beta) - y^Tbeta + \sum |beta_i|
For L2-regularized L1-loss SVR dual (-s13), we solve
min_beta  0.5(beta^T Q beta) - y^T beta + \sum |beta_i|
s.t.   -C <= beta_i <= C,
where, Q is a matrix with Q_ij = x_i^T x_j.
如果bias >= 0，那么w变为[w;w_{n+1}] ，x 变为[x; bias]。
primal-dual的关系表明了-s 1 和 -s 2学习到的是同样的模型。-s0 和 -s 7，-s 11 和 -s 12也是。
我们实现了一对多的多分类方法。在训练i类和non_i类的时候，它们的参数C分别是(weight from -wi)*C和C。如果只有两类，我们只训练一个模型。这时候使用weight1*C和weight2*C。看下面的例子。
我们还实现了多类SVM byCrammer and Singer (-s 4):
min_{w_m, \xi_i}  0.5 \sum_m ||w_m||^2 + C \sum_i \xi_i
s.t.  w^T_{y_i} x_i - w^T_m x_i>= \e^m_i - \xi_i \forall m,i
where,  e^m_i = 0 if y_i  = m,
e^m_i = 1 if y_i != m,
这里我们解dual 问题:
min_{\alpha}  0.5 \sum_m ||w_m(\alpha)||^2 + \sum_i \sum_me^m_i alpha^m_i
s.t.  \alpha^m_i <= C^m_i\forall m,i , \sum_m \alpha^m_i=0 \forall i
where, w_m(\alpha) = \sum_i \alpha^m_i x_i,
and C^m_i = C if m  = y_i,
C^m_i = 0 if m != y_i.
**五、程序predict的用法**
**用法：**predict [options] test_file model_file output_file
**options:**
-b probability_estimates: 是否输出概率估计。默认是0，不输出。只对logistic回归有用
-q : 安静模式（无输出信息）
需要注意的是-b只在预测阶段用到。这个和LIBSVM不同。
**六、例子**
> train data_file
默认参数的时候，训练的是L2损失函数的线性SVM
> train -s 0 data_file
-s 0指定训练一个logistic回归模型
> train -v 5 -e 0.001 data_file
-v 5指定5-fold的交叉检验模式。-e 0.001指定一个比默认值更小的迭代停止容忍度。
> train -c 10 -w1 2 -w2 5 -w3 2four_class_data_file
-c 10指定参数C是10，-w1 2指定第一类的权值w是2，这时候对应的C是w*C，其他同。例如我们要训练四类。-w1 2 -w2 5 -w3 2分别指定了类1的C=w*C=2*10=20，类2的C=w*C=5*10=50，类3同。类4没有指定，所以是C。其他对应的负类的C都是10 。（我们要训练四类的分类器，就需要四个二分类器，正负样本各需要一个参数C）
Train four classifiers:
positive        negative        Cp     Cn
class 1         class 2,3,4.    20     10
class 2        class 1,3,4.    50     10
class 3         class 1,2,4.    20     10
class 4         class 1,2,3.    10     10
> train -c 10 -w3 1 -w2 5two_class_data_file
如果只有两类，我们只训练一个模型。这时候两类的C值分别是10 和 50。
> predict -b 1 test_file data_file.modeloutput_file
-b 1指定输出每类估计得到的概率值。只对logistic回归有效。
**七、库的用法**
**1、函数train()**
**-Function:**model* train(const struct problem *prob,
const struct parameter *param);
这个函数根据给定的训练数据和参数构造一个线性分类器或者回归模型并返回。
**2、结构体struct problem**
结构体structproblem 描述我们要求解的问题：
struct problem
{
int l, n;
int *y;
struct feature_node **x;
double bias;
};
l表示训练数据的个数。如果bias>= 0，那么我们会在每个样本的末尾添加一个额外的值，这时候，样本x变为[x; bias]。n表示特征（样本）的维数（包括bias）。y是保存了目标值（期望输出或者样本标签）的数组。x是一个指针数组，每个元素指向一个保存了一个样本的稀疏表示的数组（结构体feature_node的数组）。
例如，如果我们有以下的训练数据：
LABEL       ATTR1   ATTR2  ATTR3   ATTR4   ATTR5
-----       -----   -----  -----   -----   -----
1           0       0.1    0.2     0       0
2           0       0.1    0.3    -1.2     0
1           0.4     0      0       0       0
2           0       0.1    0       1.4     0.5
3          -0.1    -0.2    0.1     1.1     0.1
还有bias = 1，那这个问题描述的结构体struct problem就可以描述为：
l= 5共五个样本
n= 6 特征的维数5+1=6
y-> 1 2 1 2 3 每个样本对应的标签值
x-> [ ] -> (2,0.1) (3,0.2) (6,1) (-1,?)
[ ] -> (2,0.1) (3,0.3) (4,-1.2) (6,1) (-1,?)
[ ] -> (1,0.4) (6,1) (-1,?)
[ ] -> (2,0.1) (4,1.4) (5,0.5) (6,1) (-1,?)
[ ] -> (1,-0.1) (2,-0.2) (3,0.1) (4,1.1) (5,0.1) (6,1) (-1,?)
x保存的是每个样本的稀疏表示，也就是0值就不保存，只保存非零值和其对应的索引号。例如第一个样本，第一个特征值是0，不管，第二个特征值是0.1，所以保存格式为(索引号, 特征值)，也就是(2,0.1)。同理，第三个特征值是0.2，所以保存(3,0.2)，后面同理。然后通过一个-1的索引号来标记这个样本的结束。保存为(-1,?)。
**3、结构体struct parameter**
结构体structparameter 描述一个线性分类器或者回归模型的参数：
struct parameter
{
int solver_type;
/* these are for training only*/
double eps;             /* stopping criteria */
double C;
int nr_weight;
int *weight_label;
double* weight;
double p;
};
下面介绍下各个成员变量，也就是各个参数，这些参数和在命令行中给train传入的参数是一致的：
solver_type是solver的类型，可以是以下的其中一种：
L2R_LR,L2R_L2LOSS_SVC_DUAL, L2R_L2LOSS_SVC, L2R_L1LOSS_SVC_DUAL, MCSVM_CS,L1R_L2LOSS_SVC, L1R_LR, L2R_LR_DUAL, L2R_L2LOSS_SVR, L2R_L2LOSS_SVR_DUAL,L2R_L1LOSS_SVR_DUAL.
对于分类器：
L2R_LR               L2-regularized logistic regression (primal)
L2R_L2LOSS_SVC_DUAL  L2-regularized L2-loss support vector classification (dual)
L2R_L2LOSS_SVC       L2-regularized L2-loss support vector classification (primal)
L2R_L1LOSS_SVC_DUAL  L2-regularized L1-loss support vector classification (dual)
MCSVM_CS              supportvector classification by Crammer and Singer
L1R_L2LOSS_SVC       L1-regularized L2-loss support vector classification
L1R_LR               L1-regularized logistic regression
L2R_LR_DUAL          L2-regularized logistic regression (dual)
对于回归模型：
L2R_L2LOSS_SVR       L2-regularized L2-loss support vector regression (primal)
L2R_L2LOSS_SVR_DUAL  L2-regularized L2-loss support vector regression (dual)
L2R_L1LOSS_SVR_DUAL  L2-regularized L1-loss support vector regression (dual)
C是约束violation的代价参数
P是supportvector regression的损失灵敏度
eps是迭代停止条件
nr_weight, weight_label,和 weight 用来改变对一些类的惩罚。默认是1 。这对于使用unbalanced 的输入数据或者不对称的误分类代价来训练分类器时是很有效的。
nr_weight是数组weight_label 和 weight的元素个数。每个weight[i]对应weight_label[i]。表示类weight_label[i] 的惩罚会被weight[i]进行尺度化。也就是C= weight_label[i]*C。
如果你不需要对任何类改变惩罚，直接设置nr_weight为0即可。
注意：为了避免错误的参数设置，在调用train()之前最好先调用check_parameter()来检查参数的正确性。
**4、结构体struct model**
结构体struct model 保存训练得到的模型：
struct model
{
struct parameter param;
int nr_class;           /* number of classes */
int nr_feature;
double *w;
int *label;             /* label of each class */
double bias;
};
param描述获得这个模型对应的参数设置。
nr_class 和 nr_feature分别是类和特征的个数。对于回归来说，nr_class = 2
数组w 的大小是nr_feature*nr_class，是每个特征对应的权值。对于多分类，我们使用一对多的方法，所以每个特征都会对应nr_class 个类的特征权值。权值的保存通过以下方式来组织：
+------------------+------------------+------------+
| nr_class weights | nr_class weights | ...
| for 1st feature  | for 2ndfeature  |
+------------------+------------------+------------+
如果bias >= 0，x 变为 [x; bias]。特征的数目或者维数就会加1，所以数组w的大小就变为(nr_feature+1)*nr_class。Bias的值保存在bias这个变量中。
数组 label 保存的是类的标签值。
**5、其他函数**
**-Function:**void cross_validation(const problem*prob, const parameter *param,
int nr_fold, double *target);
交叉检验函数。数据会被划分为nr_fold个folds。对每个fold，用剩余的fold去训练模型，然后用这个fold来校验，这个校验过程得到的预测标签都会保存在target这个数组中。
**-Function:**double predict(const model *model_, constfeature_node *x);
预测函数。对一个分类模型，传入一个样本x，会返回预测到的对应的类。对于一个回归模型，就会返回一个由模型计算得到的函数值。
**-Function:**double predict_values(const struct model*model_,
const struct feature_node *x, double* dec_values);
这个函数得到nr_w个保存在数组dec_values的决策值。当使用回归模型或者二分类时，nr_w=1。一个例外的情况是Crammer and Singer (-s 4)的多分类SVM。对于其他情况nr_w是类的数目。
我们实现了one-vs-therest一对多的多分类(-s 0,1,2,3,5,6,7)和由Crammer and Singer (-s 4)实现的多分类SVM。该函数返回具有最高决策值的类别。
**-Function:**double predict_probability(const structmodel *model_,
const struct feature_node *x, double* prob_estimates);
该函数得到nr_class的概率估计值。保存在prob_estimates数组中。nr_class可以通过函数get_nr_class获得。该函数返回最高概率对应的类别。概率的输出只在logistic回归时有效。
**-Function:**int get_nr_feature(const model *model_);
该函数返回模型的attributes的个数。
**-Function:**int get_nr_class(const model *model_);
该函数返回模型的类的个数。如果是回归模型，返回2.
**-Function:**void get_labels(const model *model_, int*label);
该函数输出标签的名字到一个label的数组中。
**-Function:**const char *check_parameter(const structproblem *prob,
const struct parameter *param);
该函数检测参数的有效性。其需要在train() 和 cross_validation()前调用。如果参数有效，那么返回NULL，否则返回其他的错误信息。
**-Function:**int save_model(const char*model_file_name,
const struct model *model_);
该函数将模型保存到一个文件中。返回0表示成功，-1表示失败。
**- Function:**struct model *load_model(const char*model_file_name);
该函数从一个文件中加载模型。指针为空，表示加载失败。
**-Function:**void free_model_content(struct model*model_ptr);
该函数清理内存。在一个模型结构的入口处可以调用。
**-Function:**void free_and_destroy_model(struct model**model_ptr_ptr);
该函数干掉一个模型，并释放其占用的内存。
**-Function:**void destroy_param(struct parameter*param);
该函数释放参数结构体占用的内存。
**-Function:**void set_print_string_function(void(*print_func)(const char *));
用户可以指定输出的格式。set_print_string_function(NULL);将信息输入到stdout。
**八、编译Windows下可执行文件**
Windows下可执行文件在目录windows下。可以通过VisualC++来编译得到。编译过程如下：
1、打开dos命令行窗口，定位到liblinear目录下。如果VC++的环境变量还没设置，敲入以下命令来实现：
"C:\Program Files\Microsoft VisualStudio 10.0\VC\bin\vcvars32.bat"
你可能需要根据你的VC++的版本来稍微修改上述命令。
2、输入
nmake -f Makefile.win clean all
**九、其他接口**
MATLAB/OCTAVE 接口查看matlab目录下的README文件。PYTHON接口查看python'目录下的README文件。
**十、其他信息**
如果你觉得LIBLINEAR 对你有帮助的话，pleasecite it as：
R.-E. Fan, K.-W. Chang, C.-J. Hsieh, X.-R.Wang, and C.-J. Lin.
LIBLINEAR: A Library for Large LinearClassification, Journal of
Machine Learning Research 9(2008),1871-1874. Software available at
http://www.csie.ntu.edu.tw/~cjlin/liblinear
For any questions and comments, please sendyour email to
cjlin@csie.ntu.edu.tw


