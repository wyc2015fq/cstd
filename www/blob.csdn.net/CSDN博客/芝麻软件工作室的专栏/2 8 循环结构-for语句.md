
# 2.8 循环结构-for语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月11日 10:27:50[seven-soft](https://me.csdn.net/softn)阅读数：218



## for语句的基本格式
“for语句”也称为“for循环”。语句格式为：
for
 (表达式1;表达式2;表达式3) 循环体语句
其中：表达式——3个表达式都是C++中任一合法表达式，包括逗号表达式，其中表达式2的值是逻辑型，即1或0。
循环体语句——可以是单一语句，也可以是复合语句。
## 执行流程
![](http://www.weixueyuan.net/uploads/allimg/121225/1-121225143511449.gif)
图2.7 for语句的执行流程图
for语句的执行流程示意图如左图所示，描述如下：
（1） 计算表达式1的值；
（2） 计算表达式2的值，若表达式的值为真（或非0），则执行循环体；
（3） 计算表达式3的值；
（4） 再次计算表达式2的值，并重复以上过程；
（5） 直到表达式2的值为假（或为0），则结束该循环。
从逻辑关系看：循环初始条件可在表达式1中，循环条件的判断可包含在表达式2中，而循环条件变量的修改可包含在表达式3中，也可放在循环体中。
如：在【例2.11】 计算1+2+3+…+100的值中的while语句：
i=1 , s=0 ;
while(i<=n)   {    sum+=i;  i++;  }
改用for语句，可描述为：
for(i=1 , s=0 ; i<=100 ; i++)  sum+=i;
## 省略形式
for语句中的三个表达式可部分或全部省略，但两个分号不能省略。
常见的是：将表达式1放在了for语句之前，或表达式3放在了循环体中，在for语句中就可相应地省略表达式1或表达式3。
如：上述for语句：
for(i=1 , s=0 ; i<=100 ; i++)  sum+=i;
可写成如下形式：
i=1; sum=0;
for ( ;i<=100; ) {
sum+=i;  i++;
}       //等效于while循环
表达式2也可省略，写成：
for (; ;) {…}
此时，系统约定表达式2的值为1，即恒为真，则等同于：for (; 1;) {…}。
## 应用举例
【例2.14】设计程序输出Fibonacii数列的前20项，要求每行输出5个数据。Fibonacii数列可用函数描述如下（[查看动画演示](http://www.weixueyuan.net/templets/default/cpp/flash/%E4%BE%8B2.14.swf)）：
![](http://www.weixueyuan.net/uploads/allimg/121225/1-121225143P5611.gif)
【分析】/*【例2.14】这是一个古罗马数学家伦纳德 斐波那契提出的有趣的问题。假定每对兔子每月生出一对小兔子，新生的一对小兔子三个月后又可以生小兔子，假定所有兔子都不会死，一年后会有多少对兔子。具体说，第一个月只有一对兔子，第二个月由于新生小兔子不能生育，仍然只有一对兔子，第三个月小兔子开始生育，因此当月有两对小兔子，此后每个月的兔子数都是上个月和当月新生兔子数之和。由此可抽象出一个数列：
0,1,1,2,3,5,8,……。
这个数列称为Fibonacii数列，设计程序输出Fibonacii数列的前20项，要求每行输出5个数据。
\#include<iostream>
\#include<iomanip>
using namespace std;
const int m=20;
int main(){
int n,fib0=0,fib1=1,fib2;
cout<<setw(15)<<fib0<<setw(15)<<fib1;
for(n=3;n<=m;n++){
fib2=fib0+fib1;
cout<<setw(15)<<fib2;
if(n%5==0)  cout<<endl;   //控制每行5个数据
fib0=fib1;  fib1=fib2;
}
return 0;
}
【例2.15】输入一个不超过9位的正整数，将其反向后输出。例如输入247，变成742输出。（[查看动画演示](http://www.weixueyuan.net/templets/default/cpp/flash/%E4%BE%8B2.15.swf)）
\#include <iostream>
using namespace std;
int main(){
int i,num,subscript;
int digit[9];
cout<<"输入一个不超过9位的正整数："<<endl;
cin>>num;
cout<<"原来整数为："<<num<<endl;
subscript=0;      //数组下标初值
do{
digit[subscript]=num%10;
num=num/10;
subscript++;     //修改下标
} while (num>0);
for(i=0;i<subscript;i++)  //整数的反向组合
num=num*10+digit[i];
cout<<"反向后整数为："<<num<<endl;
return 0;
}
## 重要提示
从执行流程看，for语句属于先判断型，因此与while语句是完全等同的。
表达式全部省略形式的循环，如不加控制将导致死循环。为避免死循环，循环体内必须用break语句来终止循环。
for语句的使用更为灵活，是三个循环语句中用得最多的一个。

