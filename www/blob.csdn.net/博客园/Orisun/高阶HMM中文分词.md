# 高阶HMM中文分词 - Orisun - 博客园







# [高阶HMM中文分词](https://www.cnblogs.com/zhangchaoyang/articles/2571110.html)





### 模型的建立

一句话中出现的汉字构成观察序列，如“希腊的经济结构较特殊”对应的观察序列O={希,腊,的,经,济,结,构,较,特,殊}。所有观察值的集合至少应该包含训练集和测试集中出现的所有汉字。

状态有4种：B表示词首的汉字；M表示词语中间的汉字；E表示词尾的汉字；S表示单独的汉字构成一个词。

举例：希/B腊/E　　的/S　　经/B济/M结/M构/E　　较/S　　特/B殊/E

### 文本的预处理

语料库用的是使用msr_training.utf8和msr_test.utf8
由于要做分词，我们的观察值是一个一个的汉字，从文本中提前汉字的方法自然是一次读取3个字节。如果文本中含有英文符号、英文字母、阿拉伯数字等对会对提取汉字的工作造成干扰。有一种去除单字节编码字符的方法是：先利用ICTCLAS进行分词和词性标注（wordseg.cpp），然后去除词性以下列字母开关的词(posfilter.cpp)：
m:数词，里面通常包含数字
x:字符串，包含英文字母
w:标点符号，可能包含英文标点符号
t:时间，可能包含数字
另外词性为nrf(音译人名，如“阿沛·阿旺晋美”)的词也应该去掉，因为包含一个点。

wordseg.cpp

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

posfilter.cpp

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<set>

using namespace std;

int main(int argc,char *argv[]){
    set<char> filter_set;
    filter_set.insert('m');
    filter_set.insert('x');
    filter_set.insert('w');
    filter_set.insert('t');
    if(argc<2){
        cout<<"usage: "<<argv[0]<<" inputfile"<<endl;
        return 1;
    }
    ifstream ifs(argv[1]);
    if(!ifs){
        cerr<<"open file "<<argv[1]<<" failed."<<endl;
        return 1;
    }
    string outfile(argv[1]);
    ofstream ofs((outfile+".posfilter").c_str());
    if(!ofs){
        cerr<<"open outputfile failed."<<endl;
        return 1;
    }

    string line,line_out,word;
    while(getline(ifs,line)){
        line_out.clear();
        istringstream strstm(line);
        while(strstm>>word){
            string::size_type pos=word.find("/");
            string post=word.substr(pos+1);
            char c=post.at(0);
            if(c=='w')
                line_out+=" ";
            if(filter_set.find(c)==filter_set.end() && post!="nrf"){       //词性不在被过滤的集合当中
                line_out+=word.substr(0,pos);       //对于训练集要追加空格，对测试集不能追加空格
            }
        }
        ofs<<line_out<<endl;
    }

    ifs.close();
    ofs.close();
    return 0;
}

另外由于ICTCKLAS词性标注也不是100%准确，采用上述方法并不能将单字节编码的字符去除干净，在BMES.cpp中会进行最后的检查，找到单字节字符后再手动将其删除即可。
最后请在train文档中手动去除℃和／

BMES.cpp

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

using namespace std;

int main(int argc,char *argv[]){
	if(argc<3){
		cout<<"Usage: "<<argv[0]<<" inputfile outputfile"<<endl;
		return 1;
	}
	
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	if(!(ifs&&ofs)){
		cerr<<"Open file failed!"<<endl;
		return 1;
	}
	
	string line,word,line_out;
	int lineno=0;
	while(getline(ifs,line)){
		lineno++;
		line_out.clear();
		istringstream strstm(line);
		while(strstm>>word){
			if(word.size()%3!=0){
				cout<<lineno<<": "<<word<<endl;
				//return 1;
			}
			int len = word.size()/3;			//词中包含多少个汉字
			if (len == 0)
				continue;
			string word_out;
			if (len == 1) {
				word_out=word;
				word_out+="/S";
			} else {
				/*拷贝词中的第1个汉字*/
				word_out.insert(word_out.size(),word,0,3);
				word_out+="/B";
				int i=1;
				/*逐个拷贝词中间的汉字*/
				for(;i<len-1;++i){
					word_out.insert(word_out.size(),word,3*i,3);
					word_out+="/M";
				}
				/*拷贝词的最后1个汉字*/
				word_out.insert(word_out.size(),word,3*len-3,3);
				word_out+="/E";
			}
			line_out+=word_out;
			line_out+="  ";
		}
		ofs<<line_out<<endl;
	}
	ifs.close();
	ofs.close();
	return 0;
}

同样要把train文本和test文本中的所有汉字录入GDBM数据库中，然后对所有汉字标记序号。

train2dict.cpp

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<gdbm.h>
#include<ctype.h>

#define DB_FILE_BLOCK "dict_db"

int main(int argc,char* argv[]){
	if(argc<2){
		printf("Usage: %s BMES_marked_file.\n",argv[0]);
		exit(1);
	}
	
	FILE *fp;
	if((fp=fopen(argv[1],"r"))==NULL){
		perror("fopen");
		exit(1);
	}
	
	GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open(DB_FILE_BLOCK,0,GDBM_WRCREAT,S_IRUSR | S_IWUSR,NULL);
	
	char *v="w";
	datum key,value;
	value.dptr=v;
	value.dsize=1;
	
	char word[3]={0};
	char *line=NULL;			//循环从输入文件中读取一行，放在line中
	ssize_t read=0;
	size_t needlen=0;
	char slash='/';
	int line_no=0;
	while((read=getline(&line,&needlen,fp))!=-1){
		line_no++;
		char *begin=line;
		char *end=NULL;
		while((end=strchr(begin,slash))!=NULL){
			if(end-begin<3){
				printf("%d:%s\n",line_no,begin);
				break;
			}
			strncpy(word,end-3,3);
			key.dptr=word;
			key.dsize=3;
			//char tmp[4]={0};
			//strncpy(tmp,key.dptr,3);
			//printf("%s\t",tmp);
			gdbm_store(dbm_ptr,key,value,GDBM_REPLACE);
			begin=end+2;
		}
	}	
	free(line);
	fclose(fp);
	gdbm_close(dbm_ptr);
	return 0;
}

test2dict.cpp

#include<gdbm.h>
#include<ctype.h>
#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<cassert>
using namespace std;

int main(int argc,char* argv[]){
	if(argc<2){
		cout<<"Usage: "<<argv[0]<<" inputfile"<<endl;
		return 1;
	}

	ifstream ifs(argv[1]);
	assert(ifs);
	
	GDBM_FILE dbm_ptr;
    dbm_ptr = gdbm_open("dict_db",0,GDBM_WRCREAT,S_IRUSR | S_IWUSR,NULL);
	
	char v='w';
	datum key,value;
	value.dptr=&v;
	value.dsize=1;
	
	string str;
	while(ifs>>str){
		if(str.size()%3!=0){
			cout<<"size="<<str.size()<<"\t"<<"|"<<str<<"|"<<endl;
			return 1;
		}
		int len=str.size()/3;
		for(int i=0;i<len;++i){
			char word[3]={0};
			strncpy(word,str.c_str()+3*i,3);
			key.dptr=word;
			key.dsize=3;
			gdbm_store(dbm_ptr,key,value,GDBM_REPLACE);
			//char tmp[4]={0};
			//strncpy(tmp,key.dptr,3);
			//cout<<tmp<<"\t";
		}
	}
	ifs.close();
	gdbm_close(dbm_ptr);
	return 0;
}

indexword.cpp

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


我处理好的语料库可以在这里下载：[MSR分词语料](http://files.cnblogs.com/zhangchaoyang/MSR%E5%88%86%E8%AF%8D%E8%AF%AD%E6%96%99.zip)

### 二阶HMM中文分词

利用Maxmum Likelihood学习二阶HMM模型参数

AMatrix.cpp

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>

using namespace std;

const int SNUM=4;       //SNUM种隐藏状态
const char state[SNUM]={'B','M','E','S'};
int A1[SNUM][SNUM];     //记录一阶Markov状态转移的次数
int A2[SNUM][SNUM][SNUM];   //记录二阶Markov状态转移的次数
int PI[SNUM];       //记录各种状态出现的次数

inline int stateIndex(char state){
    switch(state){
        case 'B':return 0;
                 break;
        case 'M':return 1;
                 break;
        case 'E':return 2;
                 break;
        case 'S':return 3;
                 break;
        default:return -1;
                break;
    }
}
//由于隐藏状态只有4种，训练语料足够多，所有可能的状态转移在训练语料中都会出现，所以不使用任何平滑算法
inline void noneturing(const int count[],double prob[],int len){
	double total=0.0;
	for(int i=0;i<len;++i)
		total+=count[i];
	if(total==0.0){	
		for(int i=0;i<len;++i)
			prob[i]=0.0;
	}
	else{
		for(int i=0;i<len;++i)
			prob[i]=count[i]/total;
	}
}	

int main(int argc,char *argv[]){
    if(argc<2){
        cout<<"usage: "<<argv[1]<<" BMES_marked_file"<<endl;
        return 1;
    }
    ifstream ifs(argv[1]);
    if(!ifs){
        cerr<<"open inputfile "<<argv[1]<<" failed."<<endl;
        return 1;
    }

    string line;
    int lineno=0;
    while(getline(ifs,line)){
    	lineno++;
        char state;       //状态
        int i,j,k;
        string::size_type local;
        if((local=line.find("/",local+1))!=string::npos){     //抽取句子的第1个状态
		    state=line.at(local+1);
		    //cout<<state<<endl;
		    i=stateIndex(state);
		    PI[i]++;
		    if((local=line.find("/",local+1))!=string::npos){       //抽取句子的第2个状态
		        state=line.at(local+1);
		        //cout<<state<<endl;
		        j=stateIndex(state);
		        PI[j]++;
		        A1[i][j]++;
		        while((local=line.find("/",local+1))!=string::npos){       //逐个抽取句子中的后续状态
		            state=line.at(local+1);
		            //cout<<state<<endl;
		            k=stateIndex(state);
		            PI[k]++;
		            A1[j][k]++;
		            A2[i][j][k]++;
		            i=j;
		            j=k;
		        }
			}
		}
    }
    ifs.close();

/*    for(int i=0;i<SNUM;++i){
        for(int j=0;j<SNUM;++j){
            cout<<A1[i][j]<<"\t";
        }
        cout<<endl;
    }
*/
    ofstream ofs1("A1.mat");
    ofstream ofs2("A2.mat");
    ofstream ofs3("PI.mat");
    if(!(ofs1 && ofs2 && ofs3)){
        cerr<<"create matrix file failed."<<endl;
        return 1;
    }
    ofs1<<setprecision(8);
    ofs2<<setprecision(8);
    ofs3<<setprecision(8);

    double PImatrix[SNUM];
    noneturing(PI,PImatrix,SNUM);
    for(int i=0;i<SNUM;++i){
        ofs3<<PImatrix[i]<<"\t";
    }
    ofs3<<endl;

    for(int i=0;i<SNUM;++i){
        double arr[SNUM];
        noneturing(A1[i],arr,SNUM);
        for(int j=0;j<SNUM;++j){
            ofs1<<arr[j]<<"\t";
        }
        ofs1<<endl;
    }

    for(int i=0;i<SNUM;++i){
        for(int j=0;j<SNUM;++j){
            double arr[SNUM];
            noneturing(A2[i][j],arr,SNUM);
            for(int k=0;k<SNUM;++k){
                ofs2<<arr[k]<<"\t";
            }
            ofs2<<endl;
        }
    }

    ofs1.close();
    ofs2.close();
    ofs3.close();

    return 0;
}

gt.h包含Good-Turing平滑算法

#ifndef _HEADER_H
#define _HEADER_H

#include<vector>
#include<list>
#include<map>

using namespace std;

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
	
	if(N==0){
		for(int i=0;i<len;++i)
			prob[i]=0.0;
		return;
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

BMatrix.cpp

#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>
#include"gt.h"
#include<sys/stat.h>
#include<gdbm.h>

const int SNUM = 4;
const int ONUM = 4782;		//字典数据库中共有ONUM项

int B1[SNUM][ONUM];		//记录一阶状态发射的次数
int B2[SNUM][SNUM][ONUM];	//记录二阶状态发射的次数

GDBM_FILE dbm_ptr;

inline int stateIndex(char state)
{
	switch (state) {
	case 'B':
		return 0;
		break;
	case 'M':
		return 1;
		break;
	case 'E':
		return 2;
		break;
	case 'S':
		return 3;
		break;
	default:
		return -1;
		break;
	}
}

inline int observIndex(string chinese)
{
	//cout << chinese << endl;
	datum key, value;
	key.dptr = const_cast < char *>(chinese.c_str());
	key.dsize = 3;
	value = gdbm_fetch(dbm_ptr, key);
	int index = atoi(value.dptr);
	return index;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		cout << "usage: " << argv[0] << " inputfile" << endl;
		return 1;
	}
	ifstream ifs(argv[1]);
	if (!ifs) {
		cerr << "open file " << argv[1] << " failed." << endl;
		return 1;
	}
	dbm_ptr = gdbm_open("dict_db", 0, GDBM_READER, S_IRUSR | S_IWUSR, NULL);

	string line;
	//int line_no = 1;
	while (getline(ifs, line)) {
		//cout << line_no++ << endl;
		string::size_type local;
		if ((local = line.find("/")) != string::npos) {
			char s = line.at(local + 1);	//第1个状态
			string chinese = line.substr(local - 3, 3);	//第1个观察值
			int j = stateIndex(s);
			int k = observIndex(chinese);
			B1[j][k]++;
			int i;
			while ((local =
				line.find("/", local + 1)) != string::npos) {
				s = line.at(local + 1);	//下1个状态
				chinese = line.substr(local - 3, 3);	//下1个观测值
				i = j;
				j = stateIndex(s);
				k = observIndex(chinese);
				B1[j][k]++;
				B2[i][j][k]++;
			}
		}
	}
	ifs.close();
	gdbm_close(dbm_ptr);

	ofstream ofs1("B1.mat");
	ofstream ofs2("B2.mat");
	if (!(ofs1 && ofs2)) {
		cerr << "create outputfile failed." << endl;
		return 1;
	}
	ofs1 << setprecision(8);
	ofs2 << setprecision(8);
	double arr[ONUM] = { 0.0 };
	for (int i = 0; i < SNUM; ++i) {
		goodturing(B1[i], arr, ONUM);
		for (int j = 0; j < ONUM; ++j)
			ofs1 << arr[j] << "\t";
		ofs1 << endl;
	}
	for (int i = 0; i < SNUM; ++i) {
		for (int j = 0; j < SNUM; ++j) {
			goodturing(B2[i][j], arr, ONUM);
			for (int k = 0; k < ONUM; ++k)
				ofs2 << arr[k] << "\t";
			ofs2 << endl;
		}
	}
	ofs1.close();
	ofs2.close();

	return 0;
}


Viterbi算法进行分词

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stack>
#include<sys/stat.h>
#include<gdbm.h>
#include<cstdlib>

using namespace std;

const string DB_FILE_BLOCK="dict_db";
GDBM_FILE dbm_ptr;
const int SNUM=4;		//隐藏状态集合大小
const int ONUM=4782;	//观察值集合大小

/*二阶HMM模型参数*/
double PI[SNUM];
double A1[SNUM][SNUM];
double A2[SNUM][SNUM][SNUM];
double B1[SNUM][ONUM];
double B2[SNUM][SNUM][ONUM];

inline int stateIndex(char state)
{
	switch (state) {
	case 'B':
		return 0;
		break;
	case 'M':
		return 1;
		break;
	case 'E':
		return 2;
		break;
	case 'S':
		return 3;
		break;
	default:
		return -1;
		break;
	}
}

inline int observIndex(string chinese)
{
	//cout << chinese << endl;
	datum key, value;
	key.dptr = const_cast < char *>(chinese.c_str());
	key.dsize = 3;
	value = gdbm_fetch(dbm_ptr, key);
	int index = atoi(value.dptr);
	return index;
}

/*从文件中读出HMM模型参数*/
void initHMM(string f1,string f2,string f3,string f4,string f5){
	ifstream ifs1(f1.c_str());
	ifstream ifs2(f2.c_str());
	ifstream ifs3(f3.c_str());
	ifstream ifs4(f4.c_str());
	ifstream ifs5(f5.c_str());
	if(!(ifs1 && ifs2 && ifs3 && ifs4 && ifs5)){
		cerr<<"Open file failed!"<<endl;
		exit(1);
	}
	
	//读取PI
	string line;
	if(getline(ifs1,line)){
		istringstream strstm(line);
		string word;
		for(int i=0;i<SNUM;++i){
			strstm>>word;
			PI[i]=atof(word.c_str());
		}	
	}else{
		cerr<<"Read PI failed!"<<endl;
		exit(1);
	}
	
	//读取A1
	for(int i=0;i<SNUM;++i){
		getline(ifs2,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<SNUM;++j){
			strstm>>word;
			A1[i][j]=atof(word.c_str());
		}
	}
	//读取A2
	for(int i=0;i<SNUM;++i){
		for(int j=0;j<SNUM;++j){
			getline(ifs3,line);
			istringstream strstm(line);
			string word;
			for(int k=0;k<SNUM;++k){
				strstm>>word;
				A2[i][j][k]=atof(word.c_str());
			}
		}
	}
	
	//读取B1
	for(int i=0;i<SNUM;++i){
		getline(ifs4,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<ONUM;++j){
			strstm>>word;
			B1[i][j]=atof(word.c_str());
		}
	}
	//读取B2
	for(int i=0;i<SNUM;++i){
		for(int j=0;j<SNUM;++j){
			getline(ifs5,line);
			istringstream strstm(line);
			string word;
			for(int k=0;k<ONUM;++k){
				strstm>>word;
				B2[i][j][k]=atof(word.c_str());
			}
		}
	}
	
	ifs1.close();
	ifs2.close();
	ifs3.close();
	ifs4.close();
	ifs5.close();
}

/*Viterbi算法进行分词*/
void viterbi(string sentence,string &result){
	if(sentence.size()==0)
		return;
	result.clear();
	int time=sentence.size()/3;		//观察序列的长度
	if(time<3){	//观测序列中只有1个字或2个字，我们认为这就是1个词
		result=sentence.append(" ");
		return;
	}
	double ***Q=new double **[SNUM];	//动态申请并初始化Q和Path数组
	int ***Path=new int**[SNUM];
	for(int i=0;i<SNUM;++i){
		Q[i]=new double *[SNUM];
		Path[i]=new int *[SNUM];
		for(int j=0;j<SNUM;++j){
			Q[i][j]=new double[ONUM];
			Path[i][j]=new int[ONUM];
			for(int k=0;k<ONUM;++k){
				Q[i][j][k]=0.0;
				Path[i][j][k]=0;
			}
		}
	}
	
	//给Q和Path矩阵的第1个面赋值
	string chinese1=sentence.substr(0,3);
	int o1=observIndex(chinese1);
	string chinese2=sentence.substr(3,3);
	int o2=observIndex(chinese2);
	for(int i=0;i<SNUM;++i){
		for(int j=0;j<SNUM;++j){
			Q[i][j][0]=PI[i]*A1[i][j]*B1[i][o1]*B2[i][j][o2];
			Path[i][j][0]=-1;
		}
	}
	
	//给Q和Path矩阵的后续层面赋值		
	for(int t=1;t<time-1;++t){
		string chinese=sentence.substr(3*(t+1),3);
		int ot=observIndex(chinese);
		for(int j=0;j<SNUM;++j){
			for(int k=0;k<SNUM;++k){
				double max=-1.0;
				int maxindex=-1;
				for(int i=0;i<SNUM;++i){
					double product=Q[i][j][t-1]*A2[i][j][k];
					if(product>max){
						max=product;
						maxindex=i;
					}
				}
				Q[j][k][t]=max*B2[j][k][ot];
				Path[j][k][t]=maxindex;
			}
		}
	}
	
	//找Q矩阵最后一层的最大值
	double max=-1.0;
	int maxindexi=-1;
	int maxindexj=-1;
	for(int i=0;i<SNUM;++i){
		for(int j=0;j<SNUM;++j){
			if(Q[i][j][time-2]>max){
				max=Q[i][j][time-2];
				maxindexi=i;
				maxindexj=j;
			}
		}
	}
	//从maxindexj,maxindexi出发，根据Path矩阵找出最可能的状态序列
	stack<int> st;
	st.push(maxindexj);
	st.push(maxindexi);
	for(int t=time-3;t>=0;--t){
		int maxindexk=Path[maxindexi][maxindexj][t+1];
		st.push(maxindexk);
		maxindexj=maxindexi;
		maxindexi=maxindexk;
	}
	//释放三维数组
	for(int i=0;i<SNUM;++i){
		for(int j=0;j<SNUM;++j){
			delete []Q[i][j];
			delete []Path[i][j];
		}
		delete []Q[i];
		delete []Path[i];
	}
	delete []Q;
	delete []Path;
	
	//根据标记好的状态序列分词
	int pos=0;
	//cout<<sentence<<endl;
	while(!st.empty()){
		int mark=st.top();
		st.pop();
		result.insert(result.size(),sentence,pos,3);
		if(mark==2 || mark==3){		//状态是E和S
			result.append("  ");
		}
		pos+=3;
	}
	result.append("\t");
}

int main(int argc,char *argv[]){
	if(argc<3){
		cout<<"Usage: "<<argv[0]<<" inputfile outputfile"<<endl;
		return 1;
	}
	
	dbm_ptr = gdbm_open(DB_FILE_BLOCK.c_str(),0,GDBM_READER,S_IRUSR | S_IWUSR,NULL);
	initHMM("PI.mat","A1.mat","A2.mat","B1.mat","B2.mat");
	
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	if(!(ifs&&ofs)){
		cerr<<"Open file failed!"<<endl;
		return 1;
	}
	
	string line,line_out;
	int lineno=0;
	//循环读取每一行
	while(getline(ifs,line)){
		lineno++;
		//cout<<"line="<<line<<endl;
		line_out.clear();
		istringstream strstm(line);
		string sentence;
		string result;
		while(strstm>>sentence){
			//if(sentence.size()<6){
			//	cout<<lineno<<": "<<sentence<<endl;
			//	continue;
			//}
			//cout<<"sentence="<<sentence<<endl;
			viterbi(sentence,result);
			line_out+=result;
		}
		ofs<<line_out<<endl;
	}
	ifs.close();
	ofs.close();
	gdbm_close(dbm_ptr);
	return 0;
}


### 一阶HMM中文分词

一阶HMM的模型参数就是二阶HMM模型中的PI、A1、B1。

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

using namespace std;

const string DB_FILE_BLOCK="dict_db";
const int WORDS_NUM=4782;
GDBM_FILE dbm_ptr;

double PI[4];		//初始状态概率矩阵
double A[4][4];		//状态转移矩阵
double B[4][WORDS_NUM];		//发射矩阵

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
		for(int i=0;i<4;++i){
			strstm>>word;
			PI[i]=atof(word.c_str());
		}	
	}else{
		cerr<<"Read PI failed!"<<endl;
		exit(1);
	}
	
	//读取A
	for(int i=0;i<4;++i){
		getline(ifs2,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<4;++j){
			strstm>>word;
			A[i][j]=atof(word.c_str());
		}
	}
	
	//读取B
	for(int i=0;i<4;++i){
		getline(ifs3,line);
		istringstream strstm(line);
		string word;
		for(int j=0;j<WORDS_NUM;++j){
			strstm>>word;
			B[i][j]=atof(word.c_str());
		}
	}
	
	ifs1.close();
	ifs2.close();
	ifs3.close();
}

/*Viterbi算法进行分词*/
void viterbi(string sentence,string &result){
	if(sentence.size()==0)
		return;
	result.clear();
	int row=sentence.size()/3;		//观察序列的长度
	double **Q=new double*[row];	//初始化Q矩阵
	for(int i=0;i<row;++i)
		Q[i]=new double[4]();
	int **Path=new int*[row];	//初始化Path矩阵
	for(int i=0;i<row;++i)
		Path[i]=new int[4]();
	
	//给Q和Path矩阵的第1行赋值
	datum key,data;
	char chinese[3]={0};
	char *bp=const_cast<char*>(sentence.c_str());
	strncpy(chinese,bp,3);		//读取句子中的第1个汉字	
	key.dptr=chinese;
	key.dsize=3;
	data=gdbm_fetch(dbm_ptr,key);		//从数据库中获取汉字对应的index，该index对应发射矩阵的列
	int colindex=atoi(data.dptr);
	for(int i=0;i<4;++i){
		Path[0][i]=-1;
		Q[0][i]=PI[i]*B[i][colindex];
	}
	
	//给Q和Path矩阵的后续行赋值
	for(int i=1;i<row;++i){
		bp=const_cast<char*>(sentence.c_str()+i*3);
		strncpy(chinese,bp,3);	//读取句子中的下一个汉字
		key.dptr=chinese;
		data=gdbm_fetch(dbm_ptr,key);
		colindex=atoi(data.dptr);
		for(int j=0;j<4;++j){
			double max=-1.0;
			int maxindex=-1;
			for(int k=0;k<4;++k){
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
	for(int i=0;i<4;++i){
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
	int pos=0;
	//cout<<sentence<<endl;
	while(!st.empty()){
		int mark=st.top();
		st.pop();
		result.insert(result.size(),sentence,pos,3);
		if(mark==2 || mark==3){		//状态是E和S
			result.append("  ");
		}
		pos+=3;
	}
	result.append("\t");
}

int main(int argc,char *argv[]){
	if(argc<3){
		cout<<"Usage: "<<argv[0]<<" inputfile outputfile"<<endl;
		return 1;
	}
	
	dbm_ptr = gdbm_open(DB_FILE_BLOCK.c_str(),0,GDBM_READER,S_IRUSR | S_IWUSR,NULL);
	initHMM("PI.mat","A1.mat","B1.mat");
	
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	if(!(ifs&&ofs)){
		cerr<<"Open file failed!"<<endl;
		return 1;
	}
	
	string line,line_out;
	//循环读取每一行
	while(getline(ifs,line)){
		//cout<<"line="<<line<<endl;
		line_out.clear();
		istringstream strstm(line);
		string sentence;
		string result;
		while(strstm>>sentence){
			//cout<<"sentence="<<sentence<<endl;
			viterbi(sentence,result);
			line_out+=result;
		}
		ofs<<line_out<<endl;
	}
	ifs.close();
	ofs.close();
	gdbm_close(dbm_ptr);
	return 0;
}


对比一下一阶和二阶HMM分词的效果：

![](https://pic002.cnblogs.com/images/2012/103496/2012063016484021.png)

左边是二阶，右边是一阶。目测上去似乎是伯仲之间，至少我们写二阶HMM代码的effort并没有paid off。

为了得到精确的数字，我用SCWS分词系统对msr_test进行了分词，以它的结果作为标准答案（即认为SCWS分词精度为100%），用我的HMM分词结果逐字与之对比BMES标记，HMM一阶的精度为：

总共:153769
出错:38330
正确率:0.750729991090532

二阶HMM的分词精度为：

总共:153769
出错:35372
正确率:0.769966638269092

## 使用CRF++进行分词

使用的template模板为：

U01:%x[0,0]
U02:%x[-1,0]/%x[0,0]
U03:%x[-2,0]/%x[-1,0]/%x[0,0]
U04:%x[0,0]/%x[1,0]
U05:%x[0,0]/%x[1,0]/%x[2,0]

CRF相比于HMM的便利之处就在于你可以任意的定义特征函数。上面template文件中的U02相当于一阶HMM，U03相当于二阶HMM，而U04和U05则认为一个字的tag跟它后面的字也有关系。

![](https://pic002.cnblogs.com/images/2012/103496/2012111815564037.png)

训练中一些参数的说明：

ter：迭代次数

terr：标记错误率

serr：句字错误率

obj：当前对象的值。当这个值收敛到一个确定值的时候，训练完成

diff：与上一个对obj之间的相对差

diff非要减小到0.00000CRF++才肯罢休（第92次迭代的时候已经达到了0.00003），迭代次数到达上限105时它终于退出了。

还是以SCWS的分词结果作为标准，CRF++对msr_test的分词准确率为：

总共:153769
出错:17801
正确率:0.884235

下面的awk代码用于统计CRF++的分词准确率

```
#!/usr/bin/awk -f

BEGIN{
    total=0;
    error=0;
}
$0!~/^$/{    #不能是空行
    total++;
    if($2!=$3){        #第2列和第3列不相同
        error++;
    }
}
END{
    print "总共:" total;
    print "出错:" error;
    print "正确率:" (1-error/total);
}
```












