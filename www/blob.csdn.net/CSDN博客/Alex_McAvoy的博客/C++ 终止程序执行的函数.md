# C++ 终止程序执行的函数 - Alex_McAvoy的博客 - CSDN博客





2018年03月11日 12:47:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：2202








## 概述



C++中，除return语句可以终止函数执行外，还提供多种终止程序的方式，以下是三种常见的终止函数。除此之外，这三种函数还常用于处理系统的异常错误。



## abort函数
- 函数原型：void abort(void);
- 功能：中断程序执行，返回C++主窗口。
- 头文件：cstdlib
- 实例
	
```cpp
#include<iostream>
#include<cstdlib>
using namespace std;
int main()
{
	int a,b;
	cin>>a>>b;
	if(b==0)
   	{
        cout<<"除数为零"<endl;
        abort();
    }
    else
        cout<<a/b<<endl;
	return 0;
}
```


## assert函数
- 函数原型： void assert(int expression);
- 功能： 计算表达式expression的值。若值为false，终止程序执行，显示中断执行所在文件和程序行，返回C++主窗口。
- 头文件： cassert
- 实例
	
```cpp
#include<iostream>
#include<cassert>
using namespace std;
void analyze(int score);
int main()
{
	int score;
	cin>>score;
	analyze(score);
    cout<<"成绩正常"<<endl;
	return 0;
}
void analyze(int score)
{
    assert(score>=0);//score<0时终止程序
    assert(score<=100);//score>100时终止程序
}
```


## exit函数
- 函数原型： void exit(int status);
- 功能： 中断程序执行，返回退出代码，回到C++主窗口。（参数status是整型常量，终止程序时把它作为退出代码返回操作系统，C++看不到exit的返回值。该参数通常为0或1，可以省略）
- 头文件： cstdlib
- 实例
	
```cpp
#include<iostream>
#include<cstdlib>
using namespace std;
int main()
{
	int a,b;
	cin>>a>>b;
	if(b==0)
   	{
        cout<<"除数为零"<endl;
        exit(1);
    }
    else
        cout<<a/b<<endl;
	return 0;
}
```




