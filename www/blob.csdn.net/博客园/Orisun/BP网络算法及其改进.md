# BP网络算法及其改进 - Orisun - 博客园







# [BP网络算法及其改进](https://www.cnblogs.com/zhangchaoyang/articles/2269523.html)





书上的一个例子，是要识别英文字符C、I、T。

![](https://pic002.cnblogs.com/images/2011/103496/2011113019094082.png)![](https://pic002.cnblogs.com/images/2011/103496/2011113019094984.png)![](https://pic002.cnblogs.com/images/2011/103496/2011113019100354.png)

则XC=(1,1,1,1,0,0,1,1,1),XI=(0,1,0,0,1,0,0,1,0),XT=(1,1,1,0,1,0,0,1,0)。

1.标准BP算法

网络图我就不画了。

输入层X=(x0,x1,x2,...xi,...,xn)

隐藏层Y=(y0,y1,y2,...,yj,...,ym)

x0=y0=-1

输出层O=(o1,o2,...,ok,...,ol)

期望输出D=(d1,d2,...,dk,...,dl)

输入层到隐藏层的权重Vij

隐藏层到输出层的权重Wjk

对于输出层：

净输入

$$net_k=\sum_{j=0}^{m}W_{jk}Y_j\;\;k=1,2,...,l$$

输出

$$O_k=f(net_k)$$

对于隐藏层：

净输入

$$net_j=\sum_{i=0}^{n}V_{ij}X_i\;\;j=1,2,...,m$$

输出

$$Y_j=f(net_j)$$

变换函数f(x)采用单极形Sigmoid函数：

$$f(x)=\frac{1}{1+e^{-x}}\ldots\ldots\ldots\ldots(1)$$

函数具有性质：

$$f^{'}(x)=f(x)[1-f(x)]\ldots\ldots\ldots\ldots(2)$$

 采用批训练法，误差是所有样本的均方误差和：

$$E=\frac{1}{2}\sum_{p=1}^{P}\sum_{k=1}^{l}(D_k^p-O_k^p)^2$$

显然误差E是网络权值W和V的函数，E分别对W和V求偏导得到E的梯度，要减小误差E，则权值W和V调整的方向就应该是误差梯度的反方向。结合（1）式（2）式可得权值调整公式：

$$\bigtriangleup{W}_{jk}=\eta\sum_{p=1}^{P}(D_k^{p}-O_k^{p})O_k^{p}(1-O_k^{p})Y_j^{p}\ldots\ldots\ldots(3)$$ 

$${\bigtriangleup}V_{ij}=\eta\sum_{p=1}^{P}\{\sum_{k=1}^{l}[(D_k^{p}-O_k^{p})O_k^{p}(1-O_k^{p})W_{jk}]Y_j^{p}(1-Y_j^{p})X_i^{p}\}\ldots(4)$$

上代码:

首先从复旦语料库全体训练集中随机挑取360个训练样本（每类40个）作为神经网络的输入

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)View Code 


#/usr/bin/perl

srand();
my $outf="/home/orisun/master/fudan_corpus/tc_ann.txt";
open OUT,">$outf" or die "Can't open file:$!";
my $dir_prefix="/home/orisun/master/fudan_corpus/train_vec/";
my @cat=qw/C3-Art_ws C7-History_ws C11-Space_ws C19-Computer_ws C31-Enviornment_ws C32-Agriculture_ws C34-Economy_ws C38-Politics_ws C39-Sports_ws/;
foreach(0..$#cat){
    $dir=$dir_prefix.$cat[$_];
    opendir(DIR,"$dir") or die "Can't open directory:$!";
    @files=grep {/^[^\.]/} readdir(DIR);    #文件不能以.开头
    foreach(1..40){
        $index=rand($#files);
        print OUT $dir."/".$files[$index]."\n";
    }
}
close OUT;
```



#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cassert>

using namespace std;

const int dim=9;	//样本向量的维度
const int in_count=dim+1;	//输入层节点数
const int hidden_count=5;	//隐藏层节点数
const int out_count=3;	//输出层节点数
const int P=3;	//样本数
const int iter_lim=1500;	//最大迭代次数
const double Epsilon=0.03;	//允许误差
double Eta=0.2;	//学习率
double W[hidden_count][out_count]={0};	//从隐藏层到输出层的权值
double V[in_count][hidden_count-1]={0};	//从输入层到隐藏层的权值

/**
 * 单极性Sigmoid函数
 */
inline double sigmoid(double activation,double response){
    double ex=-activation/response;
    return 1.0/(pow(M_E,ex)+1);
}

/**
 * 初始网络权值W和V，赋予[0,1]上的随机数
 */
void initParam(){
	srand(time(0));
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			W[i][j]=rand()/(double)RAND_MAX;
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			V[i][j]=rand()/(double)RAND_MAX;
	}
}

void printWeight(){
	cout<<"W="<<endl;
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			cout<<W[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"V="<<endl;
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			cout<<V[i][j]<<"\t";
		cout<<endl;
	}
}

/**
 * 给定输入，求网络的输出
 */
void getOutput(double (&input)[in_count],double (&Y)[hidden_count],double (&output)[out_count]){
	assert(input[0]==-1);
	assert(Y[0]==-1);
	for(int j=1;j<hidden_count;++j){
		double net=0.0;		//隐藏层的净输入
		for(int i=0;i<in_count;++i)
			net+=input[i]*V[i][j];
		Y[j]=sigmoid(net,1);	//把净输入抛给S形函数，得到隐藏层的输出
	}
	for(int k=0;k<out_count;++k){
		double net=0.0;		//输出层的净输入
		for(int j=0;j<hidden_count;++j)
			net+=Y[j]*W[j][k];
		output[k]=sigmoid(net,1);	//把净输入抛给S形函数，得到输出层的输出
		//cout<<output[k]<<"\t";
	}
	//cout<<endl;
}

/**
 * 批训练法根据样本总体误差调整权重W和V
 */
void adjustWeight(double (&input)[P][in_count],double (&Y)[P][hidden_count],
					double (&output)[P][out_count],double (&D)[P][out_count]){
	double delte_W[hidden_count][out_count]={0};	//数组必须显式地赋0,否则它的初始值是一个随机的数
	double delte_V[in_count][hidden_count]={0};
	for(int j=0;j<hidden_count;++j){
		for(int k=0;k<out_count;++k){
			for(int p=0;p<P;++p){
				delte_W[j][k]+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*Y[p][j];
			}
			delte_W[j][k]*=Eta;
			W[j][k]+=delte_W[j][k];
		}
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count;++j){
			for(int p=0;p<P;++p){
				double tmp=0.0;
				for(int k=0;k<out_count;++k){
					tmp+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*W[j][k];
				}
				delte_V[i][j]+=tmp*Y[p][j]*(1-Y[p][j])*input[p][i];
			}
			delte_V[i][j]*=Eta;
			V[i][j]+=delte_V[i][j];
		}
	}
}

/**
 * 计算所有样本的均方误差和
 */
double getMSE(double (&output)[P][out_count],double (&D)[P][out_count]){
	double error=0.0;
	for(int p=0;p<P;++p){
		for(int k=0;k<out_count;++k){
			error+=pow((D[p][k]-output[p][k]),2);
		}
	}
	error/=2;
	return error;
}

int main(){
	initParam();
	double X[P][in_count]={{-1,1,1,1,1,0,0,1,1,1},		//"C"
							{-1,0,1,0,0,1,0,0,1,0},		//"I"
							{-1,1,1,1,0,1,0,0,1,0}};	//"T"
	double D[P][out_count]={{1,0,0},		//"C"
							{0,1,0},		//"I"
							{0,0,1}};		//"T"
	double Y[P][hidden_count]={{-1},{-1},{-1}};
	double O[P][out_count]={0};
	
	int iteration=iter_lim;
	//printWeight();
	while(iteration-->0){
		for(int p=0;p<P;++p)
			getOutput(X[p],Y[p],O[p]);
		double err=getMSE(O,D);
		cout<<"第"<<iter_lim-1-iteration<<"次迭代误差:"<<err<<endl;
		//printWeight();
		if(err<Epsilon){		//如果误差小于允许的误差，则退出迭代
			cout<<"误差小于允许误差，迭代退出。"<<endl;
			break;
		}
		adjustWeight(X,Y,O,D);
	}
	
	//使用原样本进行测试
	double Out[out_count]={0};
	for(int p=0;p<P;++p){
		getOutput(X[p],Y[p],Out);
		for(int k=0;k<out_count;k++)
			cout<<Out[k]<<"\t";
		cout<<endl;
	}
	
	return 0;
}


运行输出：

……

……

第1262次迭代误差:0.0304531
第1263次迭代误差:0.0304104
第1264次迭代误差:0.0303678
第1265次迭代误差:0.0303252
第1266次迭代误差:0.0302828
第1267次迭代误差:0.0302405
第1268次迭代误差:0.0301984
第1269次迭代误差:0.0301563
第1270次迭代误差:0.0301143
第1271次迭代误差:0.0300725
第1272次迭代误差:0.0300307
第1273次迭代误差:0.0299891
误差小于允许误差，迭代退出。
0.941014 0.0105744 0.0708488 
0.0195598 0.901582 0.115183 
0.066168 0.0872207 0.873328

2.增加动量项

 该方法是从前一次的权值调整量中取出一部分叠加到本次权值调整量中。

$${\bigtriangleup}W(t)=\eta\delta{X}+\alpha{\bigtriangleup}W(t-1)$$

其中$\alpha$是动量系数，在(0,1)上取值。动量项反应了以前积累的调整经验，对于t时刻的调整起阻尼作用。当误差曲面出现骤然起伏时，可以减小振荡趋势，提高训练速度。

目前BP算法中都增加了动量项，以至于有动量项的BP算法成为一种新的标准算法。

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cassert>

using namespace std;

const int dim=9;	//样本向量的维度
const int in_count=dim+1;	//输入层节点数
const int hidden_count=5;	//隐藏层节点数
const int out_count=3;	//输出层节点数
const int P=3;	//样本数
const int iter_lim=1500;	//最大迭代次数
const double Epsilon=0.03;	//允许误差
double Eta=0.2;	//学习率
double W[hidden_count][out_count]={0};	//从隐藏层到输出层的权值
double V[in_count][hidden_count-1]={0};	//从输入层到隐藏层的权值
double alpha=0.8;	//动量系数
double pre_delte_W[hidden_count][out_count]={0};	//上一次的权值调整量
double pre_delte_V[in_count][hidden_count]={0};		//数组必须显式地赋0,否则它的初始值是一个随机的数

/**
 * 单极性Sigmoid函数
 */
inline double sigmoid(double activation,double response){
    double ex=-activation/response;
    return 1.0/(pow(M_E,ex)+1);
}

/**
 * 初始网络权值W和V，赋予[0,1]上的随机数
 */
void initParam(){
	srand(time(0));
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			W[i][j]=rand()/(double)RAND_MAX;
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			V[i][j]=rand()/(double)RAND_MAX;
	}
}

void printWeight(){
	cout<<"W="<<endl;
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			cout<<W[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"V="<<endl;
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			cout<<V[i][j]<<"\t";
		cout<<endl;
	}
}

/**
 * 给定输入，求网络的输出
 */
void getOutput(double (&input)[in_count],double (&Y)[hidden_count],double (&output)[out_count]){
	assert(input[0]==-1);
	assert(Y[0]==-1);
	for(int j=1;j<hidden_count;++j){
		double net=0.0;		//隐藏层的净输入
		for(int i=0;i<in_count;++i)
			net+=input[i]*V[i][j];
		Y[j]=sigmoid(net,1);	//把净输入抛给S形函数，得到隐藏层的输出
	}
	for(int k=0;k<out_count;++k){
		double net=0.0;		//输出层的净输入
		for(int j=0;j<hidden_count;++j)
			net+=Y[j]*W[j][k];
		output[k]=sigmoid(net,1);	//把净输入抛给S形函数，得到输出层的输出
		//cout<<output[k]<<"\t";
	}
	//cout<<endl;
}

/**
 * 批训练法根据样本总体误差调整权重W和V
 */
void adjustWeight(double (&input)[P][in_count],double (&Y)[P][hidden_count],
					double (&output)[P][out_count],double (&D)[P][out_count]){
	double delte_W[hidden_count][out_count]={0};	//数组必须显式地赋0,否则它的初始值是一个随机的数
	double delte_V[in_count][hidden_count]={0};
	for(int j=0;j<hidden_count;++j){
		for(int k=0;k<out_count;++k){
			for(int p=0;p<P;++p){
				delte_W[j][k]+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*Y[p][j];
			}
			delte_W[j][k]*=Eta;
			W[j][k]+=delte_W[j][k]+alpha*pre_delte_W[j][k];
			pre_delte_W[j][k]=delte_W[j][k];
		}
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count;++j){
			for(int p=0;p<P;++p){
				double tmp=0.0;
				for(int k=0;k<out_count;++k){
					tmp+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*W[j][k];
				}
				delte_V[i][j]+=tmp*Y[p][j]*(1-Y[p][j])*input[p][i];
			}
			delte_V[i][j]*=Eta;
			V[i][j]+=delte_V[i][j]+alpha*pre_delte_V[i][j];
			pre_delte_V[i][j]=delte_V[i][j];
		}
	}
}

/**
 * 计算所有样本的均方误差和
 */
double getMSE(double (&output)[P][out_count],double (&D)[P][out_count]){
	double error=0.0;
	for(int p=0;p<P;++p){
		for(int k=0;k<out_count;++k){
			error+=pow((D[p][k]-output[p][k]),2);
		}
	}
	error/=2;
	return error;
}

int main(){
	initParam();
	double X[P][in_count]={{-1,1,1,1,1,0,0,1,1,1},		//"C"
							{-1,0,1,0,0,1,0,0,1,0},		//"I"
							{-1,1,1,1,0,1,0,0,1,0}};	//"T"
	double D[P][out_count]={{1,0,0},		//"C"
							{0,1,0},		//"I"
							{0,0,1}};		//"T"
	double Y[P][hidden_count]={{-1},{-1},{-1}};
	double O[P][out_count]={0};
	
	int iteration=iter_lim;
	//printWeight();
	while(iteration-->0){
		for(int p=0;p<P;++p)
			getOutput(X[p],Y[p],O[p]);
		
		double err=getMSE(O,D);
		cout<<"第"<<iter_lim-1-iteration<<"次迭代误差:"<<err<<endl;
		//printWeight();
		if(err<Epsilon){		//如果误差小于允许的误差，则退出迭代
			cout<<"误差小于允许误差，迭代退出。"<<endl;
			break;
		}
		adjustWeight(X,Y,O,D);
	}
	
	//使用原样本进行测试
	double Out[out_count]={0};
	for(int p=0;p<P;++p){
		getOutput(X[p],Y[p],Out);
		for(int k=0;k<out_count;k++)
			cout<<Out[k]<<"\t";
		cout<<endl;
	}
	
	return 0;
}


只需要五百多次迭代就退出了。

3.自适应调节学习率

学习率$\eta$实际上是步长。从误差曲面上看，在平坦区域内$\eta$太小会使训练次数增加；在误差变化剧烈的区域内$\eta$太大会跨过较窄的坑凹处，使训练出现振荡，反而增加了迭代次数。

这里给出一种$\eta$自适应调整的方法：经过权值调整后如果总误差上升，则本次调整无效，$\eta$乘以$\beta,(\beta<1)$，当总误差开始下降后，再让$\eta$乘以$\theta,(\theta>1)$。

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cassert>

using namespace std;

const int dim=9;	//样本向量的维度
const int in_count=dim+1;	//输入层节点数
const int hidden_count=5;	//隐藏层节点数
const int out_count=3;	//输出层节点数
const int P=3;	//样本数
const int iter_lim=1500;	//最大迭代次数
const double Epsilon=0.03;	//允许误差
double Eta=0.2;	//学习率
const double beta=0.9;		//调整无效时减小学习率
const double theta=1.1;		//调整有效时增大学习率
double W[hidden_count][out_count]={0};	//从隐藏层到输出层的权值
double V[in_count][hidden_count-1]={0};	//从输入层到隐藏层的权值
double alpha=0.8;	//动量系数
double pre_delte_W[hidden_count][out_count]={0};	//上一次的权值调整量
double pre_delte_V[in_count][hidden_count]={0};		//数组必须显式地赋0,否则它的初始值是一个随机的数
double pre_E=RAND_MAX;		//上一次的系统误差

/**
 * 单极性Sigmoid函数
 */
inline double sigmoid(double activation,double response){
    double ex=-activation/response;
    return 1.0/(pow(M_E,ex)+1);
}

/**
 * 初始网络权值W和V，赋予[0,1]上的随机数
 */
void initParam(){
	srand(time(0));
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			W[i][j]=rand()/(double)RAND_MAX;
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			V[i][j]=rand()/(double)RAND_MAX;
	}
}

void printWeight(){
	cout<<"W="<<endl;
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			cout<<W[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"V="<<endl;
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			cout<<V[i][j]<<"\t";
		cout<<endl;
	}
}

/**
 * 给定输入，求网络的输出
 */
void getOutput(double (&input)[in_count],double (&Y)[hidden_count],double (&output)[out_count]){
	assert(input[0]==-1);
	assert(Y[0]==-1);
	for(int j=1;j<hidden_count;++j){
		double net=0.0;		//隐藏层的净输入
		for(int i=0;i<in_count;++i)
			net+=input[i]*V[i][j];
		Y[j]=sigmoid(net,1);	//把净输入抛给S形函数，得到隐藏层的输出
	}
	for(int k=0;k<out_count;++k){
		double net=0.0;		//输出层的净输入
		for(int j=0;j<hidden_count;++j)
			net+=Y[j]*W[j][k];
		output[k]=sigmoid(net,1);	//把净输入抛给S形函数，得到输出层的输出
		//cout<<output[k]<<"\t";
	}
	//cout<<endl;
}

/**
 * 批训练法根据样本总体误差调整权重W和V
 */
void adjustWeight(double (&input)[P][in_count],double (&Y)[P][hidden_count],
					double (&output)[P][out_count],double (&D)[P][out_count]){
	double delte_W[hidden_count][out_count]={0};	//数组必须显式地赋0,否则它的初始值是一个随机的数
	double delte_V[in_count][hidden_count]={0};
	for(int j=0;j<hidden_count;++j){
		for(int k=0;k<out_count;++k){
			for(int p=0;p<P;++p){
				delte_W[j][k]+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*Y[p][j];
			}
			delte_W[j][k]*=Eta;
			W[j][k]+=delte_W[j][k]+alpha*pre_delte_W[j][k];		//加入动量项
			pre_delte_W[j][k]=delte_W[j][k];
		}
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count;++j){
			for(int p=0;p<P;++p){
				double tmp=0.0;
				for(int k=0;k<out_count;++k){
					tmp+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*W[j][k];
				}
				delte_V[i][j]+=tmp*Y[p][j]*(1-Y[p][j])*input[p][i];
			}
			delte_V[i][j]*=Eta;
			V[i][j]+=delte_V[i][j]+alpha*pre_delte_V[i][j];		//加入动量项
			pre_delte_V[i][j]=delte_V[i][j];
		}
	}
}

/**
 * 计算所有样本的均方误差和
 */
double getMSE(double (&output)[P][out_count],double (&D)[P][out_count]){
	double error=0.0;
	for(int p=0;p<P;++p){
		for(int k=0;k<out_count;++k){
			error+=pow((D[p][k]-output[p][k]),2);
		}
	}
	error/=2;
	return error;
}

int main(){
	initParam();
	double X[P][in_count]={{-1,1,1,1,1,0,0,1,1,1},		//"C"
							{-1,0,1,0,0,1,0,0,1,0},		//"I"
							{-1,1,1,1,0,1,0,0,1,0}};	//"T"
	double D[P][out_count]={{1,0,0},		//"C"
							{0,1,0},		//"I"
							{0,0,1}};		//"T"
	double Y[P][hidden_count]={{-1},{-1},{-1}};
	double O[P][out_count]={0};
	
	int iteration=iter_lim;
	//printWeight();
	while(iteration-->0){
		for(int p=0;p<P;++p)
			getOutput(X[p],Y[p],O[p]);
		
		double err=getMSE(O,D);
		cout<<"第"<<iter_lim-1-iteration<<"次迭代误差:"<<err<<endl;
		//printWeight();
		if(err<Epsilon){		//如果误差小于允许的误差，则退出迭代
			cout<<"误差小于允许误差，迭代退出。"<<endl;
			break;
		}
		//动态调整学习率
		if(err>pre_E)
			Eta*=beta;
		else if(err<pre_E)
			Eta*=theta;
		pre_E=err;
		adjustWeight(X,Y,O,D);
	}
	
	//使用原样本进行测试
	double Out[out_count]={0};
	for(int p=0;p<P;++p){
		getOutput(X[p],Y[p],Out);
		for(int k=0;k<out_count;k++)
			cout<<Out[k]<<"\t";
		cout<<endl;
	}
	
	return 0;
}


只进行了四十多次迭代就退出了。

4.引入陡度因子

误差曲面进入平坦区是因为神经元输出进入了变换函数的饱和区。

$$O_k=\frac{1}{1+e^{\frac{-net_k}{\lambda}}}$$

当${\bigtriangleup}E$接近于0，而Dk-Ok仍然较大时，可以判断已进入了平坦区，此时我们令$\lambda>1$就可以压缩神经元的净输入，使之退出变换函数的饱和区。当退出平坦区后再令$\lambda=1$。

当然引入陡度因子后，（1）就改变了，相应的（2）、（3）、（4）式都要变，这里我们忽略该变化，权值调整公式还按照原先的来。

该方法对提高BP算法的收敛速度十分有效。

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cassert>

using namespace std;

const int dim=9;	//样本向量的维度
const int in_count=dim+1;	//输入层节点数
const int hidden_count=5;	//隐藏层节点数
const int out_count=3;	//输出层节点数
const int P=3;	//样本数
const int iter_lim=1500;	//最大迭代次数
const double Epsilon=0.03;	//允许误差
double Eta=0.2;	//学习率
const double beta=0.9;		//调整无效时减小学习率
const double theta=1.1;		//调整有效时增大学习率
double lambda=1;		//陡度因子
double W[hidden_count][out_count]={0};	//从隐藏层到输出层的权值
double V[in_count][hidden_count-1]={0};	//从输入层到隐藏层的权值
double alpha=0.8;	//动量系数
double pre_delte_W[hidden_count][out_count]={0};	//上一次的权值调整量
double pre_delte_V[in_count][hidden_count]={0};		//数组必须显式地赋0,否则它的初始值是一个随机的数
double pre_E=RAND_MAX;		//上一次的系统误差

/**
 * 单极性Sigmoid函数
 */
inline double sigmoid(double activation,double response){
    double ex=-activation/response;
    return 1.0/(pow(M_E,ex)+1);
}

/**
 * 初始网络权值W和V，赋予[0,1]上的随机数
 */
void initParam(){
	srand(time(0));
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			W[i][j]=rand()/(double)RAND_MAX;
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			V[i][j]=rand()/(double)RAND_MAX;
	}
}

void printWeight(){
	cout<<"W="<<endl;
	for(int i=0;i<hidden_count;++i){
		for(int j=0;j<out_count;++j)
			cout<<W[i][j]<<"\t";
		cout<<endl;
	}
	cout<<"V="<<endl;
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count-1;++j)
			cout<<V[i][j]<<"\t";
		cout<<endl;
	}
}

/**
 * 给定输入，求网络的输出
 */
void getOutput(double (&input)[in_count],double (&Y)[hidden_count],double (&output)[out_count]){
	assert(input[0]==-1);
	assert(Y[0]==-1);
	for(int j=1;j<hidden_count;++j){
		double net=0.0;		//隐藏层的净输入
		for(int i=0;i<in_count;++i)
			net+=input[i]*V[i][j];
		Y[j]=sigmoid(net,lambda);	//把净输入抛给S形函数，得到隐藏层的输出
	}
	for(int k=0;k<out_count;++k){
		double net=0.0;		//输出层的净输入
		for(int j=0;j<hidden_count;++j)
			net+=Y[j]*W[j][k];
		output[k]=sigmoid(net,lambda);	//把净输入抛给S形函数，得到输出层的输出
		//cout<<output[k]<<"\t";
	}
	//cout<<endl;
}

/**
 * 批训练法根据样本总体误差调整权重W和V
 */
void adjustWeight(double (&input)[P][in_count],double (&Y)[P][hidden_count],
					double (&output)[P][out_count],double (&D)[P][out_count]){
	double delte_W[hidden_count][out_count]={0};	//数组必须显式地赋0,否则它的初始值是一个随机的数
	double delte_V[in_count][hidden_count]={0};
	for(int j=0;j<hidden_count;++j){
		for(int k=0;k<out_count;++k){
			for(int p=0;p<P;++p){
				delte_W[j][k]+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*Y[p][j];
			}
			delte_W[j][k]*=Eta;
			W[j][k]+=delte_W[j][k]+alpha*pre_delte_W[j][k];		//加入动量项
			pre_delte_W[j][k]=delte_W[j][k];
		}
	}
	for(int i=0;i<in_count;++i){
		for(int j=0;j<hidden_count;++j){
			for(int p=0;p<P;++p){
				double tmp=0.0;
				for(int k=0;k<out_count;++k){
					tmp+=(D[p][k]-output[p][k])*output[p][k]*(1-output[p][k])*W[j][k];
				}
				delte_V[i][j]+=tmp*Y[p][j]*(1-Y[p][j])*input[p][i];
			}
			delte_V[i][j]*=Eta;
			V[i][j]+=delte_V[i][j]+alpha*pre_delte_V[i][j];		//加入动量项
			pre_delte_V[i][j]=delte_V[i][j];
		}
	}
}

/**
 * 计算所有样本的均方误差和
 */
double getMSE(double (&output)[P][out_count],double (&D)[P][out_count]){
	double error=0.0;
	for(int p=0;p<P;++p){
		for(int k=0;k<out_count;++k){
			error+=pow((D[p][k]-output[p][k]),2);
		}
	}
	error/=2;
	return error;
}

int main(){
	initParam();
	double X[P][in_count]={{-1,1,1,1,1,0,0,1,1,1},		//"C"
							{-1,0,1,0,0,1,0,0,1,0},		//"I"
							{-1,1,1,1,0,1,0,0,1,0}};	//"T"
	double D[P][out_count]={{1,0,0},		//"C"
							{0,1,0},		//"I"
							{0,0,1}};		//"T"
	double Y[P][hidden_count]={{-1},{-1},{-1}};
	double O[P][out_count]={0};
	
	int iteration=iter_lim;
	//printWeight();
	while(iteration-->0){
		for(int p=0;p<P;++p)
			getOutput(X[p],Y[p],O[p]);
		
		double err=getMSE(O,D);
		cout<<"第"<<iter_lim-1-iteration<<"次迭代误差:"<<err<<endl;
		//printWeight();
		if(err<Epsilon){		//如果误差小于允许的误差，则退出迭代
			cout<<"误差小于允许误差，迭代退出。"<<endl;
			break;
		}
		//动态调整学习率
		if(err>pre_E)
			Eta*=beta;
		else if(err<pre_E)
			Eta*=theta;
		
		//动态调整陡度因子
		if(err-pre_E<0.0004 && pre_E-err<0.004 && err>0.3)	//误差变化量接近于0（进入平坦区）,而误差仍很大
			lambda=2;
		else if(err-pre_E>0.0004 || pre_E-err>0.004)	//退出平坦区
			lambda=1;
　　　　　　　　　　pre_E=err;
		adjustWeight(X,Y,O,D);
	}
	
	//使用原样本进行测试
	double Out[out_count]={0};
	for(int p=0;p<P;++p){
		getOutput(X[p],Y[p],Out);
		for(int k=0;k<out_count;k++)
			cout<<Out[k]<<"\t";
		cout<<endl;
	}
	
	return 0;
}


还是四十多次迭代后误差就达到了要求。

翻译成java版：

package neuralnetwork;


/**
 * Back Propagation Network
 * 
 * @Author:zhangchaoyang
 * @Since:2014-10-1
 * @Version:
 */
public class BPNetwork {

	private final int inputSize;// 输入层节点个数
	private final int hiddenSize;// 隐藏层节点个数
	private final int outputSize;// 输出层节点个数
	private double[][] w;// 隐藏层到输出层的权值矩阵
	private double[][] v;// 输入层到隐藏层的权值矩阵
	private double[][] preDeltaOfW;// 隐藏层到输出层的权值矩阵在上一次的迭代中的变化量
	private double[][] preDeltaOfV;// 输入层到隐藏层的权值矩阵在上一次的迭代中的变化量
	private final int population;// 训练样本的个数
	private final double[][] x;// 输入值
	private double[][] y;// 隐藏层的输出值
	private double[][] o;// 实际输出值
	private final double[][] d;// 期望输出值
	private static final double TOLERANCE = 0.03;// 系统误判小于该值时停止迭代训练
	private static final int ITERATION = 1500;// 训练时的最大迭代次数
	private static final double ALPHA = 0.8;// 运量项系数
	private double ETA = 0.2;// 学习率，即步长
	private static final double BETA = 0.9;// 学习率动态调整系数。本次权值调整使系统误判增加时，学习率乘以该值
	private static final double THETA = 1.1;// 学习率动态调整系数。本次权值调整使系统误判减小时，学习率乘以该值
	private double LAMBDA = 1;// 陡度因子。我们对sigmoid函数稍做变形，让自变量x除以该值。进入平坦区域（即系统误判变化量很小）后，该值应该大于1，退出平坦区域后恢复成1
	private static final double FLAT = 0.004;// 前后两次迭代系统误差之差小于该值时说明进入平坦区域，陡度因子需要调整

	public BPNetwork(int hiddenSize, int population, double[][] x, double[][] d) {
		this.population = population;
		assert x.length == population;
		this.inputSize = x[0].length + 1;// 要加集偏置，所以这里维度要加1
		assert d.length == population;
		this.outputSize = d[0].length;
		this.hiddenSize = hiddenSize;
		this.x = new double[population][];
		for (int i = 0; i < population; i++) {
			this.x[i] = new double[inputSize];
			this.x[i][0] = 1;// x[i][0]是偏置，值为1
			for (int j = 1; j < inputSize; j++) {
				this.x[i][j] = x[i][j - 1];
			}
		}
		this.d = d;
		// 把所有权值初始化为[0,1)上的随机值
		w = new double[hiddenSize][];
		preDeltaOfW = new double[hiddenSize][];
		for (int i = 0; i < hiddenSize; i++) {
			w[i] = new double[outputSize];
			preDeltaOfW[i] = new double[outputSize];
			for (int j = 0; j < outputSize; j++) {
				w[i][j] = Math.random();
				preDeltaOfW[i][j] = 0;
			}
		}
		v = new double[inputSize][];
		preDeltaOfV = new double[inputSize][];
		for (int i = 0; i < inputSize; i++) {
			v[i] = new double[hiddenSize];
			preDeltaOfV[i] = new double[hiddenSize];
			for (int j = 1; j < hiddenSize; j++) {// 没必要给v[i][0]赋值，因为隐藏层的第一个神经元没有和任意一个输入层的神经元相连，它是偏置，值为1
				v[i][j] = Math.random();
				preDeltaOfV[i][j] = 0;
			}
		}
		// 初始化隐藏层和实际输出层的值
		y = new double[population][];
		for (int i = 0; i < population; i++) {
			y[i] = new double[hiddenSize];
			y[i][0] = 1;// y[i][0]是偏置，值为1
		}
		o = new double[population][];
		for (int i = 0; i < population; i++) {
			o[i] = new double[outputSize];
		}
	}

	/**
	 * sigmoid激活函数
	 * 
	 * @param net
	 *            自变量
	 * @param lambda
	 *            陡度因子
	 * @return
	 */
	private double sigmoid(double net, double lambda) {
		return 1.0 / (1 + Math.pow(Math.E, 1.0 - net / lambda));
	}

	/**
	 * 前向传播，由输入层计算出输出层的值
	 */
	private void forward() {
		for (int i = 0; i < population; i++) {
			for (int j = 1; j < hiddenSize; j++) {
				double net = 0;// 隐藏层的净输入
				for (int k = 0; k < inputSize; k++) {
					net += x[i][k] * v[k][j];
				}
				y[i][j] = sigmoid(net, LAMBDA);// 隐藏层的输出
			}
			for (int j = 0; j < outputSize; j++) {
				double net = 0;// 输出层的净输入
				for (int k = 0; k < hiddenSize; k++) {
					net += y[i][k] * w[k][j];
				}
				o[i][j] = sigmoid(net, LAMBDA);// 输出层的输出
			}
		}
	}

	/**
	 * 后向反馈，由系统误判的梯度下降，调整各层的权值向量
	 */
	private void backward() {
		double[][] deltaOfW = new double[hiddenSize][];
		for (int i = 0; i < hiddenSize; i++) {
			deltaOfW[i] = new double[outputSize];
		}
		double[][] deltaOfV = new double[inputSize][];
		for (int i = 0; i < inputSize; i++) {
			deltaOfV[i] = new double[hiddenSize];
		}
		double[][] delta = new double[population][];
		for (int i = 0; i < population; i++) {
			delta[i] = new double[outputSize];
			for (int j = 0; j < outputSize; j++) {
				delta[i][j] = (d[i][j] - o[i][j]) * o[i][j] * (1 - o[i][j]);
			}
		}
		for (int i = 0; i < hiddenSize; i++) {
			for (int j = 0; j < outputSize; j++) {
				for (int k = 0; k < population; k++) {
					deltaOfW[i][j] += delta[k][j] * y[k][i];
				}
				deltaOfW[i][j] = ETA * deltaOfW[i][j] + ALPHA
						* preDeltaOfW[i][j];
				w[i][j] += deltaOfW[i][j];
				preDeltaOfW = deltaOfW;
			}
		}
		for (int i = 0; i < inputSize; i++) {
			for (int j = 1; j < hiddenSize; j++) {
				for (int k = 0; k < population; k++) {
					double temp = 0;
					for (int l = 0; l < outputSize; l++) {
						temp += delta[k][l] * w[j][l];
					}
					deltaOfV[i][j] += temp * y[k][j] * (1 - y[k][j]) * x[k][i];
				}
				deltaOfV[i][j] = ETA * deltaOfV[i][j] + ALPHA
						* preDeltaOfV[i][j];
				v[i][j] += deltaOfV[i][j];
				preDeltaOfV = deltaOfV;
			}
		}
	}

	/**
	 * 计算系统平方误判
	 * 
	 * @return
	 */
	private double getSE() {
		double error = 0;
		for (int i = 0; i < population; i++) {
			for (int j = 0; j < outputSize; j++) {
				error += Math.pow(d[i][j] - o[i][j], 2);
			}
		}
		error /= 2;
		return error;
	}

	/**
	 * 训练权值参数
	 * 
	 * @param maxIteration
	 *            训练最大迭代次数
	 * @param tolerance
	 *            系统误判低于该值时结束训练
	 */
	public void train(int maxIteration, double tolerance) {
		double preError = Double.MAX_VALUE;
		for (int itr = 0; itr < maxIteration; itr++) {
			forward();// 由输入计算隐藏层和输出层的值
			double error = getSE();
			System.out
					.println("iteration " + itr + " square error is " + error);
			if (error < tolerance) {
				System.out
						.println("square error less than tolerance, parameter training finished.");
				break;
			}
			// 动态调整学习率
			if (error > preError) {
				System.out.println("decrease ETA");
				ETA *= BETA;
			} else if (error < preError) {
				System.out.println("increase ETA");
				ETA *= THETA;
			}
			// 动态调整陡度因子
			if (Math.abs(preError - error) < FLAT && error > 10 * tolerance) { // 误差变化量很小（进入平坦区）,而误差仍很大
				System.out.println("increase LAMBDA");
				LAMBDA = 2;
			} else if (Math.abs(preError - error) > FLAT) {// 退出平坦区域
				LAMBDA = 1;
			}
			preError = error;
			backward();// 反向调整权值
		}
	}

	/**
	 * 训练权值参数
	 */
	public void train() {
		train(ITERATION, TOLERANCE);
	}

	public double[][] getW() {
		return w;
	}

	public double[][] getV() {
		return v;
	}

	/**
	 * 根据一个样本的输入，计算该样本的输出
	 * 
	 * @param input
	 * @return
	 */
	public double[] getOutput(double[] x) {
		assert x.length == inputSize - 1;
		double[] input = new double[inputSize];
		input[0] = 1;
		for (int i = 1; i < inputSize; i++) {
			input[i] = x[i - 1];
		}
		double[] hidden = new double[hiddenSize];
		hidden[0] = 1;
		double[] output = new double[outputSize];
		for (int j = 1; j < hiddenSize; j++) {
			double net = 0;// 隐藏层的净输入
			for (int k = 0; k < inputSize; k++) {
				net += input[k] * v[k][j];
			}
			hidden[j] = sigmoid(net, LAMBDA);// 隐藏层的输出
		}
		for (int j = 0; j < outputSize; j++) {
			double net = 0;// 输出层的净输入
			for (int k = 0; k < hiddenSize; k++) {
				net += hidden[k] * w[k][j];
			}
			output[j] = sigmoid(net, LAMBDA);// 输出层的输出
		}
		return output;
	}

	public static void main(String[] args) {
		int population = 3;
		double[][] x = new double[population][];
		x[0] = new double[] { 1, 1, 1, 1, 0, 0, 1, 1, 1 };// "C"
		x[1] = new double[] { 0, 1, 0, 0, 1, 0, 0, 1, 0 };// "I"
		x[2] = new double[] { 1, 1, 1, 0, 1, 0, 0, 1, 0 };// "T"
		double[][] d = new double[population][];
		d[0] = new double[] { 1, 0, 0 };// "C"
		d[1] = new double[] { 0, 1, 0 };// "I"
		d[2] = new double[] { 0, 0, 1 };// "T"
		int hiddenSize = 5;
		BPNetwork network = new BPNetwork(hiddenSize, population, x, d);
		network.train();
		double[][] w = network.getW();
		System.out.println("W=");
		for (int i = 0; i < hiddenSize; i++) {
			for (int j = 0; j < 3; j++) {
				System.out.print(w[i][j] + "\t");
			}
			System.out.println();
		}
		double[][] v = network.getV();
		System.out.println("V=");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < hiddenSize; j++) {
				System.out.print(v[i][j] + "\t");
			}
			System.out.println();
		}
		double[] output = network.getOutput(x[0]);// "C"
		System.out.print("expected [1,0,0], actual is ");
		for (int i = 0; i < output.length; i++) {
			System.out.print(output[i] + "\t");
		}
		System.out.println();
		output = network.getOutput(x[1]);// "I"
		System.out.print("expected [0,1,0], actual is ");
		for (int i = 0; i < output.length; i++) {
			System.out.print(output[i] + "\t");
		}
		System.out.println();
		output = network.getOutput(x[2]);// "T"
		System.out.print("expected [0,0,1], actual is ");
		for (int i = 0; i < output.length; i++) {
			System.out.print(output[i] + "\t");
		}
	}
}




多层感知器的主要功能
- 非线性映射能力
- 泛化能力：当向网络输入训练时未曾见过的非样本数据时，网络也能完成由输入空间到输出空间的正确映射，这种能力称为泛化能力。
- 容错能力。它允许输入样本上带有较大误差甚至个别错误。

误差曲线与BP算法的局限性
- 存在平坦区域
平坦即梯度小，梯度公式为：
 $$\frac{{\partial}E}{{\partial}W_{ik}}=-\delta_k^oY_j$$
$\delta_k^o=(D_k-O_k)O_k(1-O_k)$这意味着有3种可能进入平坦区域：Ok充分接近于Dk，此时对应着误差的某个谷点；Ok接近于0；Ok接近于1。
只要调整方向正确，调整时间足够长，总可以退出平坦区域进入某个谷点。
- 存在多个极小点
极小点的特点是梯度为0，误差曲面的这一特点使以误差梯度降低为权值调整依据的BP算法无法识别局部极小点，因此BP算法经常会陷入局部极小点无法自拔。 












