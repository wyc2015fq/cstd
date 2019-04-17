# 【多文件操作】使用C++筛选合并多文件的尝试 - CD's Coding - CSDN博客





2015年07月17日 23:56:18[糖果天王](https://me.csdn.net/okcd00)阅读数：980








今天接到一个小任务，说是让把好多文件（大概2800来个，总大小超过1G）中的信息进行筛选，整合到一个文件里方便接下来的操作。

那么，无可避免的就需要用到多文件读写。那么，该如何批量打开多个文件进行读写操作呢？

众所周知 freopen(filename,r/w,i/ostream)的第一个参数filename需要是一个常量const char*，即便文件名是有序的，我们也没办法将变量传参进去。

于是，在搜索与自学中看到了这样一个实例：



```cpp
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
	int i;
	char filename[30],line[1001];
	ifstream File[5];
	ofstream File2;  
	File2.open("5.txt",ios::out);
	for(i=1;i<5;i++)
	{
		sprintf(filename,"%d.txt",i);
		File[i].open(filename,ios::in);
		while(!File[i].eof())
		{
			File[i].getline(line,1000);
			File2<<line<<endl;
		}
		File[i].close();
	}
	File2.close();
	system("pause");
	return 0;
}
```


用数组来申明文件输入输出流，使用正规的fsream函数来进行操作，简单易懂而且操作高效。

然而，关于当前文件夹究竟有哪些文件，也懒得一一输入进来，于是我们又需要这样一个函数来获得某个文件夹内，所有文件的名称（获得之后也可以通过文件读写把所有文件名写入一个文件中）



```cpp
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include<iostream>
#include<vector>
using namespace std;
int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *dirp;
	vector<std::string> filename;
	if( (dp=opendir("F:\\directory_name") )==NULL )
	perror("open dir error");
	
	while( (dirp=readdir(dp) )!=NULL )
	filename.push_back(dirp->d_name);
	for(int i=0;i<filename.size();i++)
	cout<<filename[i]<<endl;
	closedir(dp);
	return 0;
}
```


在掌握了这些之后，尝试着写了一个较大的程序，实现的工程应用如下：从CN-pair中获得事先输出的<Code-Name>键值对，存储在缓存中，接着对大数量的文件进行读取，每行三列以'\t'分割的数据经过处理，获得第二列中的数据，筛去<Code-Name>的值，依次存储于文件SUM中，与其相对应的频数则也依次存储于FREQ中：



```cpp
#include <map>
#include <cmath> 
#include <vector>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

map<string,string> mark;
vector<int> freq;

void get_CNpair()	//get <Code-Name> pair into Cache
{
	string c,n; 
	mark.clear();
	freopen("CN-pair","r",stdin);
	while(cin>>c>>n) mark[c]=n; 
	fclose(stdin);
}

int IncludeChinese(char *str)  //0: English  1:Chinese
{
   char c;
   while(1)
   {
       c=*str++;
       if (c==0) break;
	   if (c&0x80)
       	if (*str & 0x80) return 1;
   }
   return 0;
}

int main()
{
	get_CNpair(); 
	freq.clear();
	char filename[30],line[1024],t[1024];
	ifstream File[3333];
	ofstream File2;  
	File2.open("Sum.txt",ios::out);
	int i=1;
	for(map<string,string>::iterator mit=mark.begin();mit!=mark.end();++mit,++i)
	{
		//cout<<mit->first<<":"<<mit->second<<endl;
		char addr[6];
		for(int j=0;j<6;j++) addr[j]=(mit->first)[j];
		sprintf(filename,"%s",addr);
		File[i].open(filename,ios::in);
		//File2<<"#"<<mit->first<<"#"<<endl;
		while(!File[i].eof())
		{
			File[i].getline(line,1023);
			string anti=line;
			int flag=0,rj=0,temp=0;
			for(int ri=0;ri<strlen(line);ri++)
			{
				if(flag==0 && line[ri]=='\t') {flag=1;continue;}
				if(flag==0) continue;
				if(line[ri]=='\t')
				{
					temp=0; 
					ri++;
					while(line[ri]!='.') 
					{
						temp=temp*10+(int)(line[ri]-'0');
						ri++;
					}	break;
				}
				else
				{
					if(line[ri]!=32 && line[ri]>0 && line[ri]<127);
					else if(isdigit(line[ri]));
					else t[rj++]=line[ri];	
				}
			}
			t[rj]='\0';
			if(strlen(t)>0 && IncludeChinese(t)) 
			{
				string 	t_str=t,
						rpls1=mit->first,
						rpls2=mit->second;
				//cout<<rpls1<<rpls2<<endl;
				if(t_str.find(rpls1)!=t_str.npos)
				t_str=t_str.replace(t_str.find(rpls1),rpls1.length(),"");
				//cout<<"rpls1:"<<t_str<<":"<<rpls1<<endl;
				if(t_str.find(rpls2)!=t_str.npos)
				t_str=t_str.replace(t_str.find(rpls2),rpls2.length(),"");
				//cout<<"rpls2:"<<t_str<<":"<<rpls2<<endl;

				if(t_str.length()>0)
				{
					bool blank_flag=1;
					for(int bi=0;bi<t_str.length();bi++)
					{
						if(t_str[bi]!=' ')
						{
							blank_flag=0;
							break;
						}
					}
					if(blank_flag)
					{
						File2<<line<<endl;
						freq.push_back(temp);
					}
				}
			}
		}
		File[i].close();
	}
	File2.close();
	freopen("freq.txt","w",stdout);
	//for(vector<int>::iterator vd=freq.begin();vd!=freq.end();++vd)
	for(int i=0;i<freq.size();i++)
	cout<<freq[i]<<endl;
	fclose(stdout);
	return 0;
}
```








