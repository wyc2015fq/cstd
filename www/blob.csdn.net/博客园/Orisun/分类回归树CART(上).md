# 分类回归树CART(上) - Orisun - 博客园







# [分类回归树CART(上)](https://www.cnblogs.com/zhangchaoyang/articles/2709922.html)





分类回归树(CART,Classification And Regression Tree)也属于一种决策树，上回文我们介绍了[基于ID3算法的决策树](http://www.cnblogs.com/zhangchaoyang/articles/2196631.html)。作为上篇，这里只介绍CART是怎样用于分类的。

分类回归树是一棵二叉树，且每个非叶子节点都有两个孩子，所以对于第一棵子树其叶子节点数比非叶子节点数多1。

表1
|名称|体温|表面覆盖|胎生|产蛋|能飞|水生|有腿|冬眠|类标记|
|----|----|----|----|----|----|----|----|----|----|
|人|恒温|毛发|是|否|否|否|是|否|哺乳类|
|巨蟒|冷血|鳞片|否|是|否|否|否|是|爬行类|
|鲑鱼|冷血|鳞片|否|是|否|是|否|否|鱼类|
|鲸|恒温|毛发|是|否|否|是|否|否|哺乳类|
|蛙|冷血|无|否|是|否|有时|是|是|两栖类|
|巨蜥|冷血|鳞片|否|是|否|否|是|否|爬行类|
|鸽子|恒温|毛发|否|是|是|否|是|否|鸟类|
|蝙蝠|恒温|毛发|是|否|是|否|是|否|哺乳类|
|猫|恒温|皮|是|否|否|否|是|否|哺乳类|
|豹纹鲨|冷血|鳞片|是|否|否|是|否|否|鱼类|
|海龟|冷血|鳞片|否|是|否|有时|是|否|爬行类|
|豪猪|恒温|刚毛|是|否|否|否|是|是|哺乳类|
|猫头鹰|恒温|毛发|否|是|是|否|是|否|鸟类|
|鳗|冷血|鳞片|否|是|否|是|否|否|鱼类|
|蝾螈|冷血|无|否|是|否|有时|是|是|两栖类|

上例是属性有8个，每个属性又有多少离散的值可取。在决策树的每一个节点上我们可以按任一个属性的任一个值进行划分。比如最开始我们按：

1）表面覆盖为毛发和非毛发

2）表面覆盖为鳞片和非鳞片

3）体温为恒温和非恒温

等等产生当前节点的左右两个孩子。按哪种划分最好呢？有3个标准可以用来衡量划分的好坏：GINI指数、双化指数、有序双化指数。下面我们只讲GINI指数。

### GINI指数

总体内包含的类别越杂乱，GINI指数就越大（跟熵的概念很相似）。比如体温为恒温时包含哺乳类5个、鸟类2个，则：

$GINI=1-[(\frac{5}{7})^2+(\frac{2}{7})^2]=\frac{20}{49}$

体温为非恒温时包含爬行类3个、鱼类3个、两栖类2个,则

$GINI=1-[(\frac{3}{8})^2+(\frac{3}{8})^2+(\frac{2}{8})^2]=\frac{42}{64}$

所以如果按照“体温为恒温和非恒温”进行划分的话，我们得到GINI的增益（类比信息增益）：

$GINI\_Gain=\frac{7}{15}*\frac{20}{49}+\frac{8}{15}*\frac{42}{64}$

最好的划分就是使得GINI_Gain最小的划分。

### 终止条件

一个节点产生左右孩子后，递归地对左右孩子进行划分即可产生分类回归树。这里的终止条件是什么？什么时候节点就可以停止分裂了？直观的情况，当节点包含的数据记录都属于同一个类别时就可以终止分裂了。这只是一个特例，更一般的情况我们计算χ2值来判断分类条件和类别的相关程度，当χ2很小时说明分类条件和类别是独立的，即按照该分类条件进行分类是没有道理的，此时节点停止分裂。注意这里的“分类条件”是指按照GINI_Gain最小原则得到的“分类条件”。

假如在构造分类回归树的第一步我们得到的“分类条件”是：体温为恒温和非恒温。此时：
||哺乳类|爬行类|鱼类|鸟类|两栖类|
|----|----|----|----|----|----|
|恒温|5|0|0|2|0|
|非恒温|0|3|3|0|2|

我在《[独立性检验](http://www.cnblogs.com/zhangchaoyang/articles/2642032.html)》中讲述了χ2的计算方法。当选定置信水平后查表可得“体温”与动物类别是否相互独立。

还有一种方式就是，如果某一分支覆盖的样本的个数如果小于一个阈值，那么也可产生叶子节点，从而终止Tree-Growth。

### 剪枝

当分类回归树划分得太细时，会对噪声数据产生过拟合作用。因此我们要通过剪枝来解决。剪枝又分为前剪枝和后剪枝：前剪枝是指在构造树的过程中就知道哪些节点可以剪掉，于是干脆不对这些节点进行分裂，在[N皇后问题和背包问题](http://www.cnblogs.com/zhangchaoyang/articles/2676958.html)中用的都是前剪枝，上面的χ2方法也可以认为是一种前剪枝；后剪枝是指构造出完整的决策树之后再来考查哪些子树可以剪掉。

在分类回归树中可以使用的后剪枝方法有多种，比如：代价复杂性剪枝、最小误差剪枝、悲观误差剪枝等等。这里我们只介绍代价复杂性剪枝法。

对于分类回归树中的每一个非叶子节点计算它的表面误差率增益值α。

$\alpha=\frac{R(t)-R(T_t)}{|N_{T_t}|-1}$

$|N_{T_t}|$是子树中包含的叶子节点个数;

$R(t)$是节点t的误差代价，如果该节点被剪枝;

$R(t)=r(t)*p(t)$

r(t)是节点t的误差率;

p(t)是节点t上的数据占所有数据的比例。

$R(T_t)$是子树Tt的误差代价，如果该节点不被剪枝。它等于子树Tt上所有叶子节点的误差代价之和。

比如有个非叶子节点t4如图所示：

![](https://pic002.cnblogs.com/images/2012/103496/2012100121270768.png)

已知所有的数据总共有60条，则节点t4的节点误差代价为：

$R(t)=r(t)*p(t)=\frac{7}{16}*\frac{16}{60}=\frac{7}{60}$

子树误差代价为：

$R(T_t)=\sum{R(i)}=(\frac{2}{5}*\frac{5}{60})+(\frac{0}{2}*\frac{2}{60})+(\frac{3}{9}*\frac{9}{60})=\frac{5}{60}$

以t4为根节点的子树上叶子节点有3个，最终：

$\alpha=\frac{7/60-5/60}{3-1}=\frac{1}{6}$

找到α值最小的非叶子节点，令其左右孩子为NULL。当多个非叶子节点的α值同时达到最小时，取$|N_{T_t}|$最大的进行剪枝。



源代码。拿表1作为训练数据，得到剪枝前和剪枝后的两棵分类回归树，再对表1中的数据进行分类测试。

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<list>
#include<set>
#include<queue>
#include<utility>
#include<vector>
#include<cmath>

using namespace std;

//置信水平取0.95时的卡方表
const double CHI[18]={0.004,0.103,0.352,0.711,1.145,1.635,2.167,2.733,3.325,3.94,4.575,5.226,5.892,6.571,7.261,7.962};
/*根据多维数组计算卡方值*/
template<typename Comparable>
double cal_chi(Comparable **arr,int row,int col){
	vector<Comparable> rowsum(row);
	vector<Comparable> colsum(col);
	Comparable totalsum=static_cast<Comparable>(0);
	//cout<<"observation"<<endl;
	for(int i=0;i<row;++i){
		for(int j=0;j<col;++j){
			//cout<<arr[i][j]<<"\t";
			totalsum+=arr[i][j];
			rowsum[i]+=arr[i][j];
			colsum[j]+=arr[i][j];
		}
		//cout<<endl;
	}
	double rect=0.0;
	//cout<<"exception"<<endl;
	for(int i=0;i<row;++i){
		for(int j=0;j<col;++j){
			double excep=1.0*rowsum[i]*colsum[j]/totalsum;
			//cout<<excep<<"\t";
			if(excep!=0)
				rect+=pow(arr[i][j]-excep,2.0)/excep;
		}
		//cout<<endl;
	}
	return rect;
}

class MyTriple{
public:
	double first;
	int second;
	int third;
	MyTriple(){
		first=0.0;
		second=0;
		third=0;
	}
	MyTriple(double f,int s,int t):first(f),second(s),third(t){}
	bool operator< (const MyTriple &obj) const{
		int cmp=this->first-obj.first;
		if(cmp>0)
			return false;
		else if(cmp<0)
			return true;
		else{
			cmp=obj.second-this->second;
			if(cmp<0)
				return true;
			else
				return false;
		}
	}
};

typedef map<string,int> MAP_REST_COUNT;
typedef map<string,MAP_REST_COUNT> MAP_ATTR_REST;
typedef vector<MAP_ATTR_REST> VEC_STATI;

const int ATTR_NUM=8;		//自变量的维度
vector<string> X(ATTR_NUM);
int rest_number;		//因变量的种类数，即类别数
vector<pair<string,int> > classes;		//把类别、对应的记录数存放在一个数组中
int total_record_number;		//总的记录数
vector<vector<string> > inputData;		//原始输入数据

class node{
public:
	node* parent;		//父节点
	node* leftchild;		//左孩子节点
	node* rightchild;		//右孩子节点
	string cond;		//分枝条件
	string decision;		//在该节点上作出的类别判定
	double precision;		//判定的正确率
	int record_number;		//该节点上涵盖的记录个数
	int size;		//子树包含的叶子节点的数目
	int index;		//层次遍历树，给节点标上序号
	double alpha;	//表面误差率的增加量
	node(){
		parent=NULL;
		leftchild=NULL;
		rightchild=NULL;
		precision=0.0;
		record_number=0;
		size=1;
		index=0;
		alpha=1.0;
	}
	node(node* p){
		parent=p;
		leftchild=NULL;
		rightchild=NULL;
		precision=0.0;
		record_number=0;
		size=1;
		index=0;
		alpha=1.0;
	}
	node(node* p,string c,string d):cond(c),decision(d){
		parent=p;
		leftchild=NULL;
		rightchild=NULL;
		precision=0.0;
		record_number=0;
		size=1;
		index=0;
		alpha=1.0;
	}
	void printInfo(){
		cout<<"index:"<<index<<"\tdecisoin:"<<decision<<"\tprecision:"<<precision<<"\tcondition:"<<cond<<"\tsize:"<<size;
		if(parent!=NULL)
			cout<<"\tparent index:"<<parent->index;
		if(leftchild!=NULL)
			cout<<"\tleftchild:"<<leftchild->index<<"\trightchild："<<rightchild->index;
		cout<<endl;
	}
	void printTree(){
		printInfo();
		if(leftchild!=NULL)
			leftchild->printTree();
		if(rightchild!=NULL)
			rightchild->printTree();	
	}
};

int readInput(string filename){
	ifstream ifs(filename.c_str());
	if(!ifs){
		cerr<<"open inputfile failed!"<<endl;
		return -1;
	}
	map<string,int> catg;
	string line;
	getline(ifs,line);
	string item;
	istringstream strstm(line);
	strstm>>item;
	for(int i=0;i<X.size();++i){
		strstm>>item;
		X[i]=item;
	}
	while(getline(ifs,line)){
		vector<string> conts(ATTR_NUM+2);
		istringstream strstm(line);
		//strstm.str(line);
		for(int i=0;i<conts.size();++i){
			strstm>>item;
			conts[i]=item;
			if(i==conts.size()-1)
				catg[item]++;
		}
		inputData.push_back(conts);
	}
	total_record_number=inputData.size();
	ifs.close();
	map<string,int>::const_iterator itr=catg.begin();
	while(itr!=catg.end()){
		classes.push_back(make_pair(itr->first,itr->second));
		itr++;
	}
	rest_number=classes.size();
	return 0;
}

/*根据inputData作出一个统计stati*/
void statistic(vector<vector<string> > &inputData,VEC_STATI &stati){
	for(int i=1;i<ATTR_NUM+1;++i){
		MAP_ATTR_REST attr_rest;
		for(int j=0;j<inputData.size();++j){
			string attr_value=inputData[j][i];
			string rest=inputData[j][ATTR_NUM+1];
			MAP_ATTR_REST::iterator itr=attr_rest.find(attr_value);
			if(itr==attr_rest.end()){
				MAP_REST_COUNT rest_count;
				rest_count[rest]=1;
				attr_rest[attr_value]=rest_count;
			}
			else{
				MAP_REST_COUNT::iterator iter=(itr->second).find(rest);
				if(iter==(itr->second).end()){
					(itr->second).insert(make_pair(rest,1));
				}
				else{
					iter->second+=1;
				}
			}
		}
		stati.push_back(attr_rest);
	}
}

/*依据某条件作出分枝时，inputData被分成两部分*/
void splitInput(vector<vector<string> > &inputData,int fitIndex,string cond,vector<vector<string> > &LinputData,vector<vector<string> > &RinputData){
	for(int i=0;i<inputData.size();++i){
		if(inputData[i][fitIndex+1]==cond)
			LinputData.push_back(inputData[i]);
		else
			RinputData.push_back(inputData[i]);
	}
}

void printStati(VEC_STATI &stati){
	for(int i=0;i<stati.size();i++){
		MAP_ATTR_REST::const_iterator itr=stati[i].begin();
		while(itr!=stati[i].end()){
			cout<<itr->first;
			MAP_REST_COUNT::const_iterator iter=(itr->second).begin();
			while(iter!=(itr->second).end()){
				cout<<"\t"<<iter->first<<"\t"<<iter->second;
				iter++;
			}
			itr++;
			cout<<endl;
		}
		cout<<endl;
	}
}

void split(node *root,vector<vector<string> > &inputData,vector<pair<string,int> > classes){
	//root->printInfo();
	root->record_number=inputData.size();
	VEC_STATI stati;
	statistic(inputData,stati);
	//printStati(stati);
	//for(int i=0;i<rest_number;i++)
	//	cout<<classes[i].first<<"\t"<<classes[i].second<<"\t";
	//cout<<endl;
	/*找到最大化GINI指标的划分*/
	double minGain=1.0;		//最小的GINI增益
	int fitIndex=-1;
	string fitCond;
	vector<pair<string,int> > fitleftclasses;
	vector<pair<string,int> > fitrightclasses;
	int fitleftnumber;
	int fitrightnumber;
	for(int i=0;i<stati.size();++i){		//扫描每一个自变量
		MAP_ATTR_REST::const_iterator itr=stati[i].begin();
		while(itr!=stati[i].end()){			//扫描自变量上的每一个取值
			string condition=itr->first;		//判定的条件，即到达左孩子的条件
			//cout<<"cond 为"<<X[i]+condition<<"时：";
			vector<pair<string,int> > leftclasses(classes);		//左孩子节点上类别、及对应的数目
			vector<pair<string,int> > rightclasses(classes);	//右孩子节点上类别、及对应的数目
			int leftnumber=0;		//左孩子节点上包含的类别数目
			int rightnumber=0;		//右孩子节点上包含的类别数目
			for(int j=0;j<leftclasses.size();++j){		//更新类别对应的数目
				string rest=leftclasses[j].first;
				MAP_REST_COUNT::const_iterator iter2;
				iter2=(itr->second).find(rest);
				if(iter2==(itr->second).end()){		//没找到
					leftclasses[j].second=0;
					rightnumber+=rightclasses[j].second;
				}
				else{		//找到
					leftclasses[j].second=iter2->second;
					leftnumber+=leftclasses[j].second;
					rightclasses[j].second-=(iter2->second);
					rightnumber+=rightclasses[j].second;
				}
			}
			/**if(leftnumber==0 || rightnumber==0){
				cout<<"左右有一边为空"<<endl;
				
				for(int k=0;k<rest_number;k++)
					cout<<leftclasses[k].first<<"\t"<<leftclasses[k].second<<"\t";
				cout<<endl;
				for(int k=0;k<rest_number;k++)
					cout<<rightclasses[k].first<<"\t"<<rightclasses[k].second<<"\t";
				cout<<endl;
			}**/
			double gain1=1.0;			//计算GINI增益
			double gain2=1.0;
			if(leftnumber==0)
				gain1=0.0;
			else
				for(int j=0;j<leftclasses.size();++j)		
					gain1-=pow(1.0*leftclasses[j].second/leftnumber,2.0);
			if(rightnumber==0)
				gain2=0.0;
			else
				for(int j=0;j<rightclasses.size();++j)
					gain2-=pow(1.0*rightclasses[j].second/rightnumber,2.0);
			double gain=1.0*leftnumber/(leftnumber+rightnumber)*gain1+1.0*rightnumber/(leftnumber+rightnumber)*gain2;
			//cout<<"GINI增益："<<gain<<endl;
			if(gain<minGain){
				//cout<<"GINI增益："<<gain<<"\t"<<i<<"\t"<<condition<<endl;
				fitIndex=i;
				fitCond=condition;
				fitleftclasses=leftclasses;
				fitrightclasses=rightclasses;
				fitleftnumber=leftnumber;
				fitrightnumber=rightnumber;
				minGain=gain;
			}
			itr++;
		}
	}

	/*计算卡方值，看有没有必要进行分裂*/
	//cout<<"按"<<X[fitIndex]+fitCond<<"划分，计算卡方"<<endl;
	int **arr=new int*[2];
	for(int i=0;i<2;i++)
		arr[i]=new int[rest_number];
	for(int i=0;i<rest_number;i++){
		arr[0][i]=fitleftclasses[i].second;
		arr[1][i]=fitrightclasses[i].second;
	}
	double chi=cal_chi(arr,2,rest_number);
	//cout<<"chi="<<chi<<" CHI="<<CHI[rest_number-2]<<endl;
	if(chi<CHI[rest_number-2]){		//独立，没必要再分裂了
		delete []arr[0];	delete []arr[1];	delete []arr;
		return;				//不需要分裂函数就返回
	}
	delete []arr[0];	delete []arr[1];	delete []arr;
	
	/*分裂*/
	root->cond=X[fitIndex]+"="+fitCond;		//root的分枝条件
	//cout<<"分类条件:"<<root->cond<<endl;
	node *travel=root;		//root及其祖先节点的size都要加1
	while(travel!=NULL){
		(travel->size)++;
		travel=travel->parent;
	}
	
	node *LChild=new node(root);		//创建左右孩子
	node *RChild=new node(root);
	root->leftchild=LChild;
	root->rightchild=RChild;
	int maxLcount=0;
	int maxRcount=0;
	string Ldicision,Rdicision;
	for(int i=0;i<rest_number;++i){		//统计哪种类别出现的最多，从而作出类别判定
		if(fitleftclasses[i].second>maxLcount){
			maxLcount=fitleftclasses[i].second;
			Ldicision=fitleftclasses[i].first;
		}
		if(fitrightclasses[i].second>maxRcount){
			maxRcount=fitrightclasses[i].second;
			Rdicision=fitrightclasses[i].first;
		}
	}
	LChild->decision=Ldicision;
	RChild->decision=Rdicision;
	LChild->precision=1.0*maxLcount/fitleftnumber;
	RChild->precision=1.0*maxRcount/fitrightnumber;
	
	/*递归对左右孩子进行分裂*/
	vector<vector<string> > LinputData,RinputData;
	splitInput(inputData,fitIndex,fitCond,LinputData,RinputData);
	//cout<<"左边inputData行数:"<<LinputData.size()<<endl;
	//cout<<"右边inputData行数:"<<RinputData.size()<<endl;
	split(LChild,LinputData,fitleftclasses);
	split(RChild,RinputData,fitrightclasses);
}

/*计算子树的误差代价*/
double calR2(node *root){
	if(root->leftchild==NULL)
		return (1-root->precision)*root->record_number/total_record_number;
	else
		return calR2(root->leftchild)+calR2(root->rightchild);
}

/*层次遍历树，给节点标上序号。同时计算alpha*/
void index(node *root,priority_queue<MyTriple> &pq){
	int i=1;
	queue<node*> que;
	que.push(root);
	while(!que.empty()){
		node* n=que.front();
		que.pop();
		n->index=i++;
		if(n->leftchild!=NULL){
			que.push(n->leftchild);
			que.push(n->rightchild);
			//计算表面误差率的增量
			double r1=(1-n->precision)*n->record_number/total_record_number;		//节点的误差代价
			double r2=calR2(n);
			n->alpha=(r1-r2)/(n->size-1);
			pq.push(MyTriple(n->alpha,n->size,n->index));
		}
	}
}

/*剪枝*/
void prune(node *root,priority_queue<MyTriple> &pq){
	MyTriple triple=pq.top();
	int i=triple.third;
	queue<node*> que;
	que.push(root);
	while(!que.empty()){
		node* n=que.front();
		que.pop();
		if(n->index==i){
			cout<<"将要剪掉"<<i<<"的左右子树"<<endl;
			n->leftchild=NULL;
			n->rightchild=NULL;
			int s=n->size-1;
			node *trav=n;
			while(trav!=NULL){
				trav->size-=s;
				trav=trav->parent;
			}
			break;
		}
		else if(n->leftchild!=NULL){
			que.push(n->leftchild);
			que.push(n->rightchild);
		}
	}
}

void test(string filename,node *root){
	ifstream ifs(filename.c_str());
	if(!ifs){
		cerr<<"open inputfile failed!"<<endl;
		return;
	}
	string line;
	getline(ifs,line);
	string item;
	istringstream strstm(line);		//跳过第一行
	map<string,string> independent;		//自变量，即分类的依据
	while(getline(ifs,line)){
		istringstream strstm(line);
		//strstm.str(line);
		strstm>>item;
		cout<<item<<"\t";
		for(int i=0;i<ATTR_NUM;++i){
			strstm>>item;
			independent[X[i]]=item;
		}
		node *trav=root;
		while(trav!=NULL){
			if(trav->leftchild==NULL){
				cout<<(trav->decision)<<"\t置信度:"<<(trav->precision)<<endl;;
				break;
			}
			string cond=trav->cond;
			string::size_type pos=cond.find("=");
			string pre=cond.substr(0,pos);
			string post=cond.substr(pos+1);
			if(independent[pre]==post)
				trav=trav->leftchild;
			else
				trav=trav->rightchild;
		}
	}
	ifs.close();
}

int main(){
	string inputFile="animal";
	readInput(inputFile);
	VEC_STATI stati;		//最原始的统计
	statistic(inputData,stati);
	
//	for(int i=0;i<classes.size();++i)
//		cout<<classes[i].first<<"\t"<<classes[i].second<<"\t";
//	cout<<endl;
	node *root=new node();
	split(root,inputData,classes);		//分裂根节点
	priority_queue<MyTriple> pq;
	index(root,pq);
	root->printTree();
	cout<<"剪枝前使用该决策树最多进行"<<root->size-1<<"次条件判断"<<endl;
	/**
	//检验一个是不是表面误差增量最小的被剪掉了
	while(!pq.empty()){
		MyTriple triple=pq.top();
		pq.pop();
		cout<<triple.first<<"\t"<<triple.second<<"\t"<<triple.third<<endl;
	}
	**/
	test(inputFile,root);
	
	prune(root,pq);
	cout<<"剪枝后使用该决策树最多进行"<<root->size-1<<"次条件判断"<<endl;
	test(inputFile,root);
	return 0;
}


C4.5克服了ID3的2个缺点：

1.用信息增益选择属性时偏向于选择分枝比较多的属性值，即取值多的属性

2.不能处理连贯属性

详细可参考[这篇博客](http://blog.sina.com.cn/s/blog_68ffc7a40100urn3.html)。












