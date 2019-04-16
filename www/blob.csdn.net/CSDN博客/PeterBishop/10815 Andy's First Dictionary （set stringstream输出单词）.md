# 10815  Andy's First Dictionary （set stringstream输出单词） - PeterBishop - CSDN博客





2018年07月11日 11:04:41[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：24
个人分类：[UVA](https://blog.csdn.net/qq_40061421/article/category/7793583)









```cpp
#include<iostream>  
#include<string>  
#include<set>  
#include<sstream>  
using namespace std;  
  
set<string> dict;//set存储单词
  
int main(){  
    string s,buf;  
    while (cin>>s){  
        for (int i=0;i<s.length();i++)  
          if (isalpha(s[i])) s[i]=tolower(s[i]);//将非单词部分转为空格，方便后面stringstream处理 
          else s[i]=' ';  
        stringstream ss(s);  
        while (ss>>buf) dict.insert(buf);//set内部自动排序
    }  
    for (set<string>::iterator it=dict.begin();it!=dict.end();++it) 
      cout<<*it<<endl;//NOTICE output by point  
    return 0;  
}
```




