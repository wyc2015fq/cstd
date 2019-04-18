# 一个十分有用的标准C++写的split(string) 函数 - ljx0305的专栏 - CSDN博客
2012年08月23日 11:54:04[ljx0305](https://me.csdn.net/ljx0305)阅读数：1856
```cpp
标准串的：
/********************************************
the tokenize function for std::string
*********************************************/
#include <string>
#include <vector>
#include <iostream>
using namespace std;
typedef basic_string<char>::size_type S_T;
static const S_T npos = -1;
////trim指示是否保留空串，默认为保留。
vector<string> tokenize(const string& src, string tok, bool trim=false, string null_subst="")
{
	if( src.empty() || tok.empty() ) throw "tokenize: empty string\0";
	vector<string> v;
	S_T pre_index = 0, index = 0, len = 0;
	while( (index = src.find_first_of(tok, pre_index)) != npos )
	{
		if( (len = index-pre_index)!=0 )
			v.push_back(src.substr(pre_index, len));
		else if(trim==false)
			v.push_back(null_subst);
		pre_index = index+1;
	}
	string endstr = src.substr(pre_index);
	if( trim==false )
		v.push_back( endstr.empty()? null_subst:endstr );
	else if( !endstr.empty() )
		v.push_back(endstr);return v;
}
////使用一个完整的串delimit（而不是其中的某个字符）来分割src串,没有trim选项，即严格分割。
vector<string> split(const string& src, string delimit,string null_subst="")
{
	if( src.empty() || delimit.empty() ) 
		throw "split: empty string\0";
	vector<string> v;
	S_T deli_len = delimit.size();
	long index = npos, last_search_position = 0;
	while( (index=src.find(delimit, last_search_position))!=npos )
	{
	if(index==last_search_position)
		v.push_back(null_subst);
	else
		v.push_back( src.substr(last_search_position, index-last_search_position) );
	last_search_position = index + deli_len;
}
string last_one = src.substr(last_search_position);
v.push_back( last_one.empty()? null_subst:last_one );
return v;
}
// test
int main(void)
{
	string src = ",ab,cde;,,fg,," ;
	string tok = ",;" ;
	vector<string> v1 = tokenize(src, tok ,true);
	vector<string> v2 = tokenize(src, tok ,false, "<null>");
	cout<<"-------------v1:"<<endl;
	for(int i=0; i<v1.size();i++)
	{
		cout<<v1[i].c_str()<<endl;
	}
	cout<<"-------------v2:"<<endl;
	for(int j=0; j<v2.size();j++)
	{
		cout<<v2[j].c_str()<<endl;
	}
	try
	{
		string s = "######123#4###56########789###";
		string del = "";//"###";
		vector<string> v3 = split(s, del, "<null>");
		cout<<"-------------v3:"<<endl;
		for(int k=0; k<v3.size();k++)
		{
			cout<<v3[k].c_str()<<endl;
		}
	}
	catch (char *s)
	{
		cout<<s<<endl;
	}
	return 0;
}
```
引用:[http://hi.baidu.com/ztianl/blog/item/122b46453062e43a8694737d.html](http://hi.baidu.com/ztianl/blog/item/122b46453062e43a8694737d.html)
