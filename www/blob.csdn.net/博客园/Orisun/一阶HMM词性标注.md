# 一阶HMM词性标注 - Orisun - 博客园







# [一阶HMM词性标注](https://www.cnblogs.com/zhangchaoyang/articles/2567610.html)





手头的语料库依然是msr_training.utf8和msr_test.utf8，它来自于自于SIGHAN Bakeoff 2005的 icwb2-data.rar

1.rmspace.cpp研究院的训练文档是已经分好词，但我们并不需要这个结果，我们要使用计算所有分词系统重新进行分词并进行词性标注，所以第一步要把训练文档中行内的空格去掉。

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

int main(int argc,char *argv[]){
    if(argc<3){
        cerr<<"Usage:"<<argv[0]<<" inputfile outputfile"<<endl;
        return 1;
    }

    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    if(!(ifs && ofs)){
        cerr<<"open file failed."<<endl;
        return 1;
    }

    string line,word,line_out;
    while(getline(ifs,line)){
        line_out.clear();
        istringstream strstm(line);
        while(strstm>>word)
            line_out+=word;
        ofs<<line_out<<endl;
    }

    ifs.close();
    ofs.close();
    return 0;
}


2.对第1步得到的输出文件还需要稍作修善，即把每行句首和句尾的双引号去掉。这个可以用vim来完成：1,$s/^“//g         1,$s/”$//g

3.wordseg.cpp对第2步得到的输出文件进行分词。g++ wordseg.cpp -o wordseg -I/home/orisun/master/ICTCLAS50_Linux_RHAS_32_C/API -lICTCLAS50运行命令时注意要把libICTCLAS50.so拷贝到当前目录下。

#include <string>
#include <iostream>
#define OS_LINUX
#include "ICTCLAS50.h"
using namespace std;
 
int main(int argc, char *argv[])
{
    if (argc < 2) {      //命令行中需要给定要处理的文件名
        cout << "Usage:command filename" << endl;
        return 1;
    }
    string filename = argv[1];
    string outfile = filename + ".ws";
    string initPath = "/home/orisun/master/ICTCLAS50_Linux_RHAS_32_C/API";
    if (!ICTCLAS_Init(initPath.c_str())) {
        cout << "Init fails" << endl;
        return -1;
    }
    ICTCLAS_FileProcess(filename.c_str(), outfile.c_str(), CODE_TYPE_UTF8,1);
    ICTCLAS_Exit();
    return 0;
}


4.由于我们要做的是词性标注，所以先要对测试文档进行分词。仍然使用wordseg.cpp。

5.rmpos.cpp计算所的分词系统在分词的同时也做了词性标注（修改配置文件Configure.xml是不起作用的）,所以现在还得把测试文本中标注好的词性去掉。

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

int main(int argc,char *argv[]){
    if(argc<3){
        cerr<<"Usage:"<<argv[0]<<" inputfile outputfile"<<endl;
        return 1;
    }

    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    if(!(ifs && ofs)){
        cerr<<"open file failed."<<endl;
        return 1;
    }

    string line,word,line_out,chinese;
    while(getline(ifs,line)){
        line_out.clear();
        istringstream strstm(line);
        while(strstm>>word){
        	string::size_type pos=word.find("/");
        	chinese=word.substr(0,pos);
            line_out+=chinese+" ";
        }
        ofs<<line_out<<endl;
    }

    ifs.close();
    ofs.close();
    return 0;
}


6.对训练文本（即第3步的输出）也实行rmpos.cpp。

7.createdict.cpp第5步和第6步生成了训练集和测试集中出现的所有词语和标点符号，现在要把它们都存入GDBM数据库。

#include<sys/stat.h>
#include<gdbm.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

int main(int argc,char *argv[]){
	if(argc<2){
		cerr<<"Usage: "<<argv[0]<<" inputfile";
		return 1;
	}
	
	ifstream ifs(argv[1]);
	if(!ifs){
		cerr<<"open file failed."<<endl;
		return 1;
	}
	
	GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open("dict_db",0,GDBM_WRCREAT,S_IRUSR | S_IWUSR,NULL);
   	char v='w';
	datum key,value;
	value.dptr=&v;
	value.dsize=1;
	
	string line,word;
	while(getline(ifs,line)){
    	istringstream strstm(line);
    	while(strstm>>word){
    		char *chinese=const_cast<char*>(word.c_str());
    		key.dptr=chinese;
    		key.dsize=word.size();
    		//cout<<chinese<<"\t"<<word.size()<<endl;
    		gdbm_store(dbm_ptr,key,value,GDBM_REPLACE);
    	}
    }
    
    ifs.close();
	gdbm_close(dbm_ptr);
	return 0;
}


8.indexword.cpp对数据库中所有的词语（包含标点）进行序号的标记。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<gdbm.h>
#include<ctype.h>

#define DB_FILE_BLOCK "dict_db"

int main(int argc,char* argv[]){
	GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open(DB_FILE_BLOCK,0,GDBM_WRCREAT,S_IRUSR | S_IWUSR,NULL);
    datum key,data;
    long index=0;		//从0开始编号
    char index_str[10]={0};
    for(key=gdbm_firstkey(dbm_ptr);key.dptr;key=gdbm_nextkey(dbm_ptr,key)){
    	data=gdbm_fetch(dbm_ptr,key);
    	bzero(index_str,sizeof(index_str));
    	sprintf(index_str,"%ld",index++);
    	data.dptr=index_str;
    	data.dsize=sizeof(index_str);
    	gdbm_store(dbm_ptr,key,data,GDBM_REPLACE);
    }
    gdbm_close(dbm_ptr);
    return 0;
}


9.query.c和lookup.c（可选辅助）前者打印输出数据库中的所有数据，后者根据用户输出的key去GDBM中查询对应的value。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<gdbm.h>
#include<ctype.h>

#define DB_FILE_BLOCK "dict_db"

int main(int argc,char* argv[]){
	GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open(DB_FILE_BLOCK,0,GDBM_READER,S_IRUSR | S_IWUSR,NULL);
    datum key,data;
    for(key=gdbm_firstkey(dbm_ptr);key.dptr;key=gdbm_nextkey(dbm_ptr,key)){
    	data=gdbm_fetch(dbm_ptr,key);
    	printf("%s--%s\t",key.dptr,data.dptr);
    }
    printf("\n");
    gdbm_close(dbm_ptr);
    return 0;
}




#include<sys/stat.h>
#include<gdbm.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc,char *argv[]){
	char *word=(char*)malloc(50);
	GDBM_FILE dbm_ptr;
	dbm_ptr=gdbm_open("dict_db",0,GDBM_WRCREAT,S_IRUSR | S_IWUSR,NULL);
	datum key,value;
	
	while(1){
		printf("please input a word.\n");
		bzero(word,50);
		scanf("%s",word);
		if(strcmp(word,"exit")==0)
			break;
		key.dptr=word;
		key.dsize=strlen(word);
		value=gdbm_fetch(dbm_ptr,key);
		if(value.dsize==0){
			printf("%s not exist in dict.\n",word);
		}
		else{
			printf("%s--%s\n",key.dptr,value.dptr);
		}
	}
	gdbm_close(dbm_ptr);
	return 0;
}


10.AMatrix.cpp统计训练文本（当然是第3步的输出）生成状态转移矩阵和初始状态概率矩阵，分别写入A.mat和PI.mat。

header.h头文件中主要包含ICTCLAS的词性标注集和Good-Turing平滑算法。

#ifndef _HEADER_H
#define _HEADER_H

#include<vector>
#include<list>
#include<map>

using namespace std;

const int POS_NUM=97;		//计算所汉语词性标记集去掉标点符号共有POS_NUM个元素
/*POS_NUM种词性，即POS_NUM种状态*/
string posarr[POS_NUM]={"n","nr","nr1","nr2","nrj","nrf","ns","nsf","nt","nz",
						"nl","ng","t","tg","s","f","v","vd","vn","vshi",
						"vyou","vf","vx","vi","vl","vg","a","ad","an","ag",
						"al","b","bl","z","r","rr","rz","rzt","rzs","rzv",
						"ry","ryt","rys","ryv","Rg","m","mq","Mg","q","qv",
						"qt","d","dl","dg","p","pba","pbei","c","cc","u",
						"uzhe","ule","uguo","ude1","ude2","ude3","usuo","udeng","uyy","udh",
						"uls","uzhi","ulian","e","y","o","h","k","x","xx",
						"xu","w","wkz","wky","wyz","wyy","wj","ww","wd","wf",
						"wn","wm","ws","wp","wb","wh","wt"};

void goodturing(const int count[],double prob[],int len){
	map<int, list<int> > count_map;		//map可以自动按key排好序
	int N=0;			
	for(int i=0;i<len;++i){
		int c=count[i];
		N+=c;
		map<int, list<int> >::const_iterator itr;
		itr=count_map.find(c);
		if(itr==count_map.end()){
			list<int> l;
			l.push_back(i);
			count_map[c]=l;
		}
		else{
			count_map[c].push_back(i);
		}
	}

	map<int, list<int> >::const_iterator iter=count_map.begin();
	while(iter!=count_map.end()){
		double pr;
		int r=iter->first;
		int nr=iter->second.size();
		if(++iter!=count_map.end()){
			int r_new=iter->first;
			if(r_new=r+1){
				int nr_1=iter->second.size();
				pr=(1.0+r)*nr_1/(N*nr);
			}
			else{
				pr=1.0*r/N;
			}
		}
		else{
			pr=1.0*r/N;
		}
		list<int> l=(--iter)->second;
		list<int>::const_iterator itr1=l.begin();
		while(itr1!=l.end()){
			int index=*itr1;
			itr1++;
			prob[index]=pr;
		}
		++iter;
	}
	
	//概率归一化
	double sum=0;
	for(int i=0;i<len;++i)
		sum+=prob[i];
	for(int i=0;i<len;++i)
		prob[i]/=sum;
}

#endif




#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<iterator>
#include<cassert>
#include"header.h"

int A[POS_NUM][POS_NUM];	//记录状态间转移的次数
int PI[POS_NUM];			//记录各种状态出现的次数

					
inline int indexof(string search){
	for(int i=0;i<POS_NUM;++i){
		if(search==posarr[i]){
			return i;
		}
	}
	return -1;
}
				
int main(int argc,char *argv[]){
	if(argc<2){
		cout<<"Usage:"<<argv[0]<<" pos_tagged_file"<<endl;
		return 1;
	}
	//打开输入文件
	ifstream ifs(argv[1]);
	if(!ifs){
		cerr<<"open file "<<argv[1]<<" failed."<<endl;
		return 1;
	}

	string line,word;
	while(getline(ifs,line)){
		istringstream strstm(line);
		string pre,post;		//pre是前一个状态，post是后一个状态
		strstm>>word;
		string::size_type pos=word.find("/");
		post=word.substr(pos+1);
		int index1,index2;
		index2=indexof(post);
		if(index2<0){
			cout<<post<<" not exist"<<endl;
			return 1;
		}
		PI[index2]++;
		while(strstm>>word){
			pre=post;
			pos=word.find("/");
			post=word.substr(pos+1);
			//cout<<"pre="<<pre<<"\tpost="<<post<<endl;
			index1=indexof(pre);
			//if(index1<0){
			//	cout<<pre<<" not exist"<<endl;
			//	return 1;
			//}
			index2=indexof(post);
			//if(index2<0){
			//	cout<<post<<" not exist"<<endl;
			//	return 1;
			//}
			A[index1][index2]++;
			PI[index2]++;
		}
	}
	ifs.close();
	
	ofstream ofs1("A.mat");
	ofstream ofs2("PI.mat");
	if(!(ofs1 && ofs2)){
		cerr<<"create file failed."<<endl;
		return 1;
	}
	ofs1<<setprecision(8);
	ofs2<<setprecision(8);
	double arr_out[POS_NUM]={0.0};
	for(int i=0;i<POS_NUM;++i){
		goodturing(A[i],arr_out,POS_NUM);
		for(int j=0;j<POS_NUM;++j){
			ofs1<<arr_out[j]<<"\t";
		}
		ofs1<<endl;
	}
	goodturing(PI,arr_out,POS_NUM);
	for(int j=0;j<POS_NUM;++j){
		ofs2<<arr_out[j]<<"\t";
	}
	ofs2<<endl;
	
	ofs1.close();
	ofs2.close();
	return 0;
}


11.BMatrix.cpp统计训练文本（当然是第3步的输出）生成发射矩阵，写入B.mat。

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
#include<cassert>
#include<cstdlib>
#include<gdbm.h>
#include<sys/stat.h>
#include"header.h"

const int TERM_NUM=70000;

int matrix[POS_NUM][TERM_NUM]={0.0};	//混淆矩阵（或称发射矩阵）

inline int indexof(string search){
	for(int i=0;i<POS_NUM;++i){
		if(search==posarr[i]){
			return i;
		}
	}
	return -1;
}

int main(int argc,char *argv[]){
	if(argc<2){
		cout<<"Usage: "<<argv[0]<<" pos_tagged_file"<<endl;
		return 1;
	}
	
	ifstream ifs(argv[1]);
	if(!ifs){
		cerr<<"open file "<<argv[1]<<" failed."<<endl;
		return 1;
	}
	
	GDBM_FILE dbm_ptr;
	dbm_ptr=gdbm_open("dict_db",0,GDBM_READER,S_IRUSR|S_IWUSR,NULL);
	datum key,value;
	
	string line,word,term,pos;
	string slash="/";
	while(getline(ifs,line)){
		istringstream strstm(line);
		while(strstm>>word){
			string::size_type loc=word.find(slash);
			assert(loc!=string::npos);
			term=word.substr(0,loc);		//词语
			pos=word.substr(loc+1);			//词性
			//cout<<term<<"\t"<<pos<<endl;
			int rowindex=indexof(pos);
			assert(rowindex>=0);
			key.dsize=term.size();
			key.dptr=const_cast<char*>(term.c_str());
			value=gdbm_fetch(dbm_ptr,key);
			int colindex=atoi(value.dptr);
			//cout<<rowindex<<"\t"<<colindex<<endl;
			matrix[rowindex][colindex]++;
		}
	}
	
	ifs.close();
	gdbm_close(dbm_ptr);
	
	//将发射矩阵写入文件
	ofstream ofs("B.mat");
	if(!ofs){
		cerr<<"create file B.mat failed."<<endl;
		return 1;
	}
	ofs<<setprecision(8);
	double arr_out[TERM_NUM]={0.0};
	for(int i=0;i<POS_NUM;++i){
		goodturing(matrix[i],arr_out,TERM_NUM);
		for(int j=0;j<TERM_NUM;++j){
			ofs<<arr_out[j]<<"\t";
		}
		ofs<<endl;
	}
	ofs.close();
	
	return 0;
}


12.postag.cpp对测试文本（第5步的输出）进行词性标注。

#include<sys/stat.h>
#include<ctype.h>
#include<gdbm.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<stack>
#include<vector>
#include"header.h"

const string DB_FILE_BLOCK="dict_db";
const int TERM_NUM=70000;
const int TERM_MAXLEN=100;
GDBM_FILE dbm_ptr;

double PI[POS_NUM];				//初始状态概率矩阵
double A[POS_NUM][POS_NUM];		//状态转移矩阵
double B[POS_NUM][TERM_NUM];	//发射矩阵

/*从文件中读出HMM模型参数*/
void initHMM(string f1,string f2,string f3){
	ifstream ifs1(f1.c_str());
	ifstream ifs2(f2.c_str());
	ifstream ifs3(f3.c_str());
	if(!(ifs1 && ifs2 && ifs3)){
		cerr<<"Open file failed!"<<endl;
		exit(1);
	}
	
	//读取PI
	string line;
	if(getline(ifs1,line)){
		istringstream strstm(line);
		string word;
		for(int i=0;i<POS_NUM;++i){
			strstm>>word;
			PI[i]=atof(word.c_str());
		}	
	}else{
		cerr<<"Read PI failed!"<<endl;
		exit(1);
	}
	
	//读取A
	for(int i=0;i<POS_NUM;++i){
		getline(ifs2,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<POS_NUM;++j){
			strstm>>word;
			A[i][j]=atof(word.c_str());
		}
	}
	
	//读取B
	for(int i=0;i<POS_NUM;++i){
		getline(ifs3,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<TERM_NUM;++j){
			strstm>>word;
			B[i][j]=atof(word.c_str());
		}
	}
	
	ifs1.close();
	ifs2.close();
	ifs3.close();
}

/*Viterbi算法进行词性标注*/
void viterbi(vector<string> terms,string &result){
	if(terms.size()==0)
		return;
	result.clear();
	int row=terms.size();		//观察序列的长度
	double **Q=new double*[row];	//初始化Q矩阵
	for(int i=0;i<row;++i)
		Q[i]=new double[POS_NUM]();
	int **Path=new int*[row];	//初始化Path矩阵
	for(int i=0;i<row;++i)
		Path[i]=new int[POS_NUM]();
	
	//给Q和Path矩阵的第1行赋值
	datum key,data;
	char chinese[TERM_MAXLEN]={0};
	char *bp=const_cast<char*>(terms[0].c_str());
	strncpy(chinese,bp,terms[0].size());		//读取句子中的第1个词	
	key.dptr=chinese;
	key.dsize=terms[0].size();
	data=gdbm_fetch(dbm_ptr,key);		//从数据库中获取汉字对应的index，该index对应发射矩阵的列
	int colindex=atoi(data.dptr);
	for(int i=0;i<POS_NUM;++i){
		Path[0][i]=-1;
		Q[0][i]=PI[i]*B[i][colindex];
	}
	
	//给Q和Path矩阵的后续行赋值
	for(int i=1;i<row;++i){
		bp=const_cast<char*>(terms[i].c_str());
		strncpy(chinese,bp,terms[i].size());	//读取句子中的下一个汉字
		key.dptr=chinese;
		key.dsize=terms[i].size();
		data=gdbm_fetch(dbm_ptr,key);
		colindex=atoi(data.dptr);
		for(int j=0;j<POS_NUM;++j){
			double max=-1.0;
			int maxindex=-1;
			for(int k=0;k<POS_NUM;++k){
				double product=Q[i-1][k]*A[k][j];
				if(product>max){
					max=product;
					maxindex=k;
				}
			}
			Q[i][j]=max*B[j][colindex];
			Path[i][j]=maxindex;
		}
	}
	
	//找Q矩阵最后一行的最大值
	double max=-1.0;
	int maxindex=-1;
	for(int i=0;i<POS_NUM;++i){
		if(Q[row-1][i]>max){
			max=Q[row-1][i];
			maxindex=i;
		}
	}
	//从maxindex出发，根据Path矩阵找出最可能的状态序列
	stack<int> st;
	st.push(maxindex);
	for(int i=row-1;i>0;--i){
		maxindex=Path[i][maxindex];
		st.push(maxindex);
	}
	//释放二维数组
	for(int i=0;i<row;++i){
		delete []Q[i];
		delete []Path[i];
	}
	delete []Q;
	delete []Path;
	
	//根据标记好的状态序列分词
	int mark=-1;
	for(int i=0;i<terms.size();++i){
		mark=st.top();
		st.pop();
		result+=terms[i]+"/"+posarr[mark]+"\t";
	}
}

int main(int argc,char *argv[]){
	if(argc<3){
		cout<<"Usage: "<<argv[0]<<" inputfile outputfile"<<endl;
		return 1;
	}
	
	dbm_ptr = gdbm_open(DB_FILE_BLOCK.c_str(),0,GDBM_READER,S_IRUSR | S_IWUSR,NULL);
	initHMM("PI.mat","A.mat","B.mat");
	
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	if(!(ifs&&ofs)){
		cerr<<"Open file failed!"<<endl;
		return 1;
	}
	
	string line;
	//循环读取每一行
	while(getline(ifs,line)){
		istringstream strstm(line);
		string term;
		vector<string> term_vec;
		string result;
		while(strstm>>term){
			term_vec.push_back(term);
		}
		viterbi(term_vec,result);
		ofs<<result<<endl;
	}
	ifs.close();
	ofs.close();
	gdbm_close(dbm_ptr);
	return 0;
}


看一下效果吧，左边是ICTCLAS的pos-tagging结果，作为标准答案，右边是我用一阶HMM词性标注的结果。

使用简单的加1平滑：

![](https://pic002.cnblogs.com/images/2012/103496/2012062908365279.png)

可以看到词性标注准确度还很低，并且"mq"贡献了大部分的错误率。

使用Good-Turing平滑后的效果，大体上已经看不出有什么错误：

![](https://pic002.cnblogs.com/images/2012/103496/2012062908420971.png)












