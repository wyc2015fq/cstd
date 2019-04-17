# c++ 统计文本词频 - 别说话写代码的博客 - CSDN博客





2017年07月26日 20:02:55[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：948








```cpp

```

```cpp
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::sort;
struct Record
{
     string word;
     int count;
};

class WordStatic
{
public:
     void readFile(const char *filename);
     void writeFile(const char *filename);
     static bool sortType(const Record &v1,const Record &v2)
     {
         return v1.count>v2.count;//逆序排列
     }
     void begin_sort()
     {
         cout <<"共有"<<_words.size() <<"个不重复单词"<<endl;
         long l=0;
         for(int i=0;i<_words.size();++i)
              l+=_words[i].count;
         cout <<"单词个数为"<<l<<endl;
         sort(_words.begin(),_words.end(),sortType);
     }
private:
     vector<Record> _words;
};

void WordStatic::readFile(const char *filename)//c++的string类无法作为open的参数
{
     ifstream ifs(filename);//打开文件流
     if(!ifs.good())//打开不正常
     {
         cout << "ifstream open file error" <<endl;
         return;
     }
     string line;
     int i;
//   WordStatic sta;
     while(getline(ifs,line))//一次读取一行
     {
         istringstream iss(line);//获取一行
         string word;
         while(iss>>word) //每次拿到一个单词
         {
              for(i=0;i<_words.size();++i)
              {
                   if(word == _words[i].word)
                   {
                       ++_words[i].count;
                       break;
                   }
              }
              if(i== _words.size())
              {
                   Record record;
                   record.word = word;
                   record.count = 1;
                   _words.push_back(record);
              }
         }
     }
}

void WordStatic::writeFile(const char *filename)//c++的string类无法作为open的参数
{
     ofstream ofs(filename);
     if(!ofs.good())
     {
         cout << "ofstream open error "<<endl;
         return ;
     }

     for(int i=0;i<_words.size();++i)
     {
         ofs << _words[i].word <<" "<<_words[i].count <<endl;
     }
     ofs.close();
}

int main(int arc,char **argv)
{
     WordStatic ws;
     ws.readFile(argv[1]);
     ws.begin_sort();
     ws.writeFile(argv[2]);
     return 0;
}
```

```cpp

```

```cpp

```
 ///
 /// @file    wordStarix.cc
 /// @author  guoleida(496919313@qq.com)
 /// @date    2017-08-02 08:48:22
 ///
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::map;
using std::istringstream;

class WordStatic
{
public:
	void readFile(const char * filename);
	void writeFile(const char * filename);
private:
	map<string,int> words;
};


void WordStatic::readFile(const char * filename)
{
	ifstream ifs(filename);//打开文件流
	if(!ifs.good())
	{
		cout <<"ifstream open file error"<<endl;
		return;
	}
	string word;
	while( ifs >> word)
	{
		words[word]++;
	}

}

void WordStatic::writeFile(const char * filename)
{
	ofstream ofs(filename);
	if(!ofs.good())
	{
		cout << "ofs open file error"<<endl;
		return;
	}
	map<string,int>::const_iterator it;
	int count=0;
	for(it = words.begin();it != words.end();++it)
	{	
		ofs << it->first <<"  " << it->second <<endl;
		count+=it->second;
	}
	cout <<"共有"<<count << "词" <<endl;
}

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		cout <<"请输入输入输出文件名"<<endl;
		return 0;
	}

	time_t begin,end;
	time(&begin);
	WordStatic w;
	w.readFile(argv[1]);
	w.writeFile(argv[2]);
	time(&end);
	cout <<"用时" << end-begin <<"s" <<endl;
	return 0;
}








