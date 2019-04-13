
# 2.7 循环结构-do-while语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月11日 10:27:12[seven-soft](https://me.csdn.net/softn)阅读数：209



## 基本格式
“do-while语句”也称为“直到循环”。语句格式为：
do 
 循环体语句 while(表达式);
其中：表达式——是C++中任一合法表达式，包括逗号表达式；其值是逻辑型，即1或0。
循环体语句——可以是单一语句，也可以是复合语句。
## 执行流程
![](http://www.weixueyuan.net/uploads/allimg/121225/1-121225142233948.gif)
图2.6 do-while语句的执行流程
do-while语句的执行流程示意图如左图所示，描述如下：
（1） 执行一次循环体语句；
（2） 计算表达式的值，若表达式的值为真（或非0），则重复上述过程；
（3） 直到表达式的值为假（或为0）时，结束循环。
## 应用举例
【例2.12】用迭代法求x=a1/2的近似值，要求前后两个迭代根之差小于10-5。求平方根的迭代公式为：xn+1
 = (xn+a/xn)/2。（[查看动画演示](http://www.weixueyuan.net/templets/default/cpp/flash/C++%20%E6%B1%82%E5%B9%B3%E6%96%B9%E6%A0%B9.swf)）
\#include<iostream>
\#include<cmath>
using namespace std;
int main(){
float x0,x1,a;
cout<<"输入一个正数："<<endl;
cin>>a;
if(a<0)  cout<<a<<"不能开平方!"<<endl;
else {          //有实数解的情况
x1=a/2;         //x1用作保存结果
do {
x0=x1;
x1=(x0+a/x0)/2;
} while (fabs(x1-x0)>=1e-5);
cout<< a<<"的平方根为："<<x1<<endl;
}
return 0;
}
【例2.13】输入一段文本，统计文本的行数、单词数及字符数。假定单词之间以空格或跳格或换行符间隔，假定文本没有空行。（[查看动画演示](http://www.weixueyuan.net/templets/default/cpp/flash/C++%20%E6%96%87%E6%9C%AC%E7%BB%9F%E8%AE%A1%E7%A4%BA%E4%BE%8B.swf)）
\#include<iostream>
using namespace std;
int main(){
char ch;
int nline=0,nword=0,nch=0;
int isword=0;
cout<<"输入一段文本（无空行）："<<endl;
do{
ch=cin.get();
if(ch=='\n')  nline++;      //遇换行符行数+1
if(ch!=' '&& ch!='\t'&&ch!='\n'&&ch!=EOF){  //读到非间隔符
if(!isword)  nword++;     //在单词的起始处给单词数+1
nch++;        //字符数加+1
isword=1;
}
else  isword=0;       //读到间隔符
} while(ch!=EOF);       //读到文本结束符为止
cout<<"行数："<<nline<<endl;
cout<<"单词数："<<nword<<endl;
cout<<"字符数："<<nch<<endl;
return 0;
}
## 重要提示
do-while语句在绝大多数情况下都能代替while语句。
while语句与do-while语句的区别是：do-while语句无论条件表达式的值是真是假，循环体都将至少执行一次；而若条件表达式的初值为假，则while语句循环体一次也不会执行。

