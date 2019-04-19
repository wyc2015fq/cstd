# hdu  水果（map） - HJ - CSDN博客
2017年03月31日 22:25:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1219
Problem Description 
夏天来了,好开心啊,呵呵,好多好多水果~~ 
Joe经营着一个不大的水果店.他认为生存之道就是经营最受顾客欢迎的水果.现在他想要一份水果销售情况的明细表,这样Joe就可以很容易掌握所有水果的销售情况了.
Input 
第一行正整数N(0< N<=10)表示有N组测试数据. 
每组测试数据的第一行是一个整数M(0
```cpp
#include<iostream>
#include<string>
#include<map>
using namespace std;
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        string fruit,place;
        int num,n;
        cin>>n;
        map<string,map<string,int> >cc;       //二维map 
        map<string,map<string,int> >::iterator it;     //外维迭代器 
        while(n--)
        {
            cin>>fruit>>place>>num;
            cc[place][fruit]+=num;
        }
        for(it=cc.begin();it!=cc.end();it++)
        {
            cout<<it->first<<endl; 
            map<string,int>::iterator it2;     //内维迭代器 
            for(it2=it->second.begin();it2!=it->second.end();it2++)
              cout<<"   |----"<<it2->first<<"("<<it2->second<<")"<<endl;
        }
        if(t!=0)
          cout<<endl;
    }
    return 0;
}
```
