# c++拼接字符串效率比较（+=、append、stringstream、sprintf） - fanyun的博客 - CSDN博客
2017年07月30日 21:36:12[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：583
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

      最近写的程序用到大量拼接字符串，为了提高拼接效率，比较了一下+=、append、stringstream、sprintf四种拼接字符串的方法。
**测试方法**
      比较方法是写了4个函数，分别用+=、append、stringstream、sprintf的方式来拼接字符串，拼接方法是将s1="abcedfg"，s2="hijklmn"，s3="opqrst"三个字符串拼接到一起，总共循环60次。然后在main函数中依次调用这4个函数，并打时间戳来计时。为了使时间差异更明显，可以取循环N（N可以为100或是1000000等）次调用的时间。代码如下：
```cpp
#include <iostream>
#include <string>
#include <sys/time.h>
#include <sstream>
#include <stdio.h>
using namespace std;
#define OUT_IN_REPEATE_NUM 10000
#define IN_REPEATE_NUM 60
string s1="abcedfg";
string s2="hijklmn";
string s3="opqrst";
void  plusTest(string& ret)
{
    for(int i=0; i<IN_REPEATE_NUM; i++)
    {
        ret += s1;
        ret += s2;
        ret += s3;
    }
}
void  appendTest(string& ret)
{
    for(int i=0; i<IN_REPEATE_NUM; i++)
    {
        ret.append(s1);
        ret.append(s2);
        ret.append(s3);
    }
}
void sprintfTest(string& ret)
{
    const size_t length=26*IN_REPEATE_NUM;
    char tmp[length];
    char* cp = tmp;
    size_t strLength=s1.length()+s2.length()+s3.length();
    for(int i=0; i<IN_REPEATE_NUM; i++)
    {
        sprintf(cp,"%s%s%s", s1.c_str(), s2.c_str(),s3.c_str());
        cp+=strLength;
    }
    ret = tmp;
}
void  ssTest(string& ret)
{
    stringstream ss;
    for(int i=0; i<IN_REPEATE_NUM; i++)
    {
        ss<<s1;
        ss<<s2;
        ss<<s3;
    }
    ret = ss.str();
}
int main() {
    string ss, plus, append, sprintf;
    struct timeval sTime, eTime;
    gettimeofday(&sTime, NULL);
    for(int i=0; i<OUT_IN_REPEATE_NUM; i++)
    {
        sprintf="";
        sprintfTest(sprintf);
    }
    gettimeofday(&eTime, NULL);
    long SprintfTime = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec); //exeTime 单位是微秒
    gettimeofday(&sTime, NULL);
    for(int i=0; i<OUT_IN_REPEATE_NUM; i++)
    {
        append="";
        appendTest(append);
    }
    gettimeofday(&eTime, NULL);
    long AppendTime = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec); //exeTime 单位是微秒
    gettimeofday(&sTime, NULL);
    for(int i=0; i<OUT_IN_REPEATE_NUM; i++)
    {
        ss="";
        ssTest(ss);
    }
    gettimeofday(&eTime, NULL);
    long SsTime = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec); //exeTime 单位是微秒
    gettimeofday(&sTime, NULL);
    for(int i=0; i<OUT_IN_REPEATE_NUM; i++)
    {
        plus="";
        plusTest(plus);
    }
    gettimeofday(&eTime, NULL);
    long PlusTime = (eTime.tv_sec-sTime.tv_sec)*1000000+(eTime.tv_usec-sTime.tv_usec); //exeTime 单位是微秒
    cout<<"PlusTime is :   "<<PlusTime<<endl;
    cout<<"AppendTime is : "<<AppendTime<<endl;
    cout<<"SsTime is :     "<<SsTime<<endl;
    cout<<"SprintfTime is :"<<SprintfTime<<endl;
    if(ss==sprintf && append==plus && ss==plus)
    {
        cout<<"They are same"<<endl;
    }
    else
    {
        cout<<"Different!"<<endl;
        cout<<"Sprintf: "<<sprintf<<endl;
        cout<<"ss:        "<<ss<<endl;
        cout<<"Plus:     "<<plus<<endl;
        cout<<"Append:"<<append<<endl;
    }
}
```
**测试结果：     **
在Linux环境下用g++编译以上代码，运行结果如下（时间单位为μm）：
|外层循环1000000次|外层循环100000次|外层循环10000次|外层循环1000次|外层循环100次|
|----|----|----|----|----|
|PlusTime is :     3405450AppendTime is : 4020078SsTime is :        7835499SprintfTime is :  14875433They are same|PlusTime is :      337229AppendTime is : 401719SsTime is :        788242SprintfTime is :  1517999They are same|PlusTime is :     32177AppendTime is : 40265SsTime is :        78928SprintfTime is :  150839They are same|PlusTime is :      3402AppendTime is : 4074SsTime is :        7984SprintfTime is :   15425They are same|PlusTime is :      369AppendTime is : 429SsTime is :         921SprintfTime is :   1591They are same|
 结论：
根据以上结果，如果是使用Linux系统并且是g++（gcc）编译器，大量拼接字符串的效率从高到低依次为：+=、append()、stringstream、sprintf()。
