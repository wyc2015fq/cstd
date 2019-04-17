# c/c++文本单词查询 - 别说话写代码的博客 - CSDN博客





2017年08月02日 21:20:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：525标签：[c++																[单词查找																[词频统计](https://so.csdn.net/so/search/s.do?q=词频统计&t=blog)
个人分类：[c/c++](https://blog.csdn.net/qq_21997625/article/category/6988647)





                
文本查询
该程序将读取用户指定的任意文本文件，然后允许用户从该文件中查找单词。查询的结果是该单词出现的次数，并列出每次出现所在的行，如果某单词在同一行中多次出现，程序将只显示该行一次。行号按升序显示。



    要求：

    a. 它必须允许用户指明要处理的文件名字。


    b. 程序将存储该文件的内容，以便输出每个单词所在的原始行。

         vector<string> lines;//O(1)  完美hash

      或 map<int, string> lines;//O(logN)

      或 unorderedmap<int,string> lines;//O(1) 


    c. 它必须将每一行分解为各个单词，并记录每个单词所在的所有行。 在输出行号时，应保证以升序输出，并且不重复。 

  map<string, set<int> > word2Line;
  map<string, int> dict;


    d. 对特定单词的查询将返回出现该单词的所有行的行号。


    e. 输出某单词所在的行文本时，程序必须能根据给定的行号从输入 文件中获取相应的行。






```cpp
///
 /// @file    textQuery.cc
 /// @author  guoleida(496919313@qq.com)
 /// @date    2017-08-02 17:30:56
 ///
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::ifstream;
using std::vector;
using std::map;
using std::set;
using std::istringstream;

class TextQuery
{
public:
	void readFile(const string &filename);
	void query(const string & word);
private:
	vector<string> _lines;
	map<string,set<int> > _word2Line;
	map<string,int> _wordFreq;
};

void TextQuery::readFile(const string &filename)
{
	ifstream ifs(filename.c_str());
	if(!ifs.good())
	{
		cout <<"ifstream open file error" <<endl;
		return;
	}

	string line;
	int i=0;
	string word;
	while(getline(ifs,line))//一次读取一行
	{
		_lines.push_back(line);//将一行放入容器中
		istringstream iss(line);//获取一行
		i++;//行号加一
		while(iss >> word)//每次拿到一个单词
		{
			_wordFreq[word]++;//频次加一
			_word2Line[word].insert(i);//加入词出现的行号
		}
	}
}

void TextQuery::query(const string & word)
{
	cout<< word <<"共出现了"  << _wordFreq.find(word)->second << "次" <<endl;
	set<int>::const_iterator itset;
	vector<string>::const_iterator itvector;
	int i;//用来匹配行号
	for( itset = (_word2Line.find(word)->second).begin();
			itset != (_word2Line.find(word)->second).end(); ++itset)
	{
		cout<<"     (line "<<*itset <<")  ";
		i=0;
		for(itvector = _lines.begin();itvector != _lines.end(); ++itvector)
		{
			if(*itset == ++i)
				cout << *itvector <<endl;
		}
	}
	cout <<endl <<endl;
}
		


int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		cout <<"请输入文件名"<<endl;
		return 0;
	}
	TextQuery tq;
	tq.readFile(argv[1]);
	while(true)
	{
		cout<<"请输入你要查询的单词($结束)：";
		string s;
		cin>>s;
		if(s=="$")
			break;
		tq.query(s);
	}

	return 0;

}
```
运行结果如下：

![](https://img-blog.csdn.net/20170802212259451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=单词查找&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




