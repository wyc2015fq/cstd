
# 2.5 分支结构-switch语句 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月11日 10:25:51[seven-soft](https://me.csdn.net/softn)阅读数：368


除了用嵌套if语句外，C++中还提供switch语句，又称为“开关语句”，用来实现多分支（多选一）。
switch语句的功能是：根据给定条件从多个分支语句序列中选择一个作为执行入口。
## 基本格式
switch语句的基本格式如下：
switch
 (表达式) {
case 常量表达式1:《语句序列1》《break;》   //《》中的内容可省
……
case 常量表达式n:《语句序列n》《break;》   //同上，下同
《default:语句序列》
}
其中：表达式——称为“条件表达式”，用作判断条件，取值为整型、字符型、布尔型或枚举型。
常量表达式——由常量构成，取值类型与条件表达式相同。
语句序列——可以是一个语句也可以是一组语句。
## 执行流程
switch语句的执行流程如下：
（1） 求条件表达式的值，并在常量表达式中找到与之相等的分支作为执行入口；
（2） 顺序执行该分支的语句序列，直到遇到break语句或开关语句的关括号“}”为止；
（3） 当条件表达式的值与所有常量表达式的值均不相等时，若有default分支，则执行其语句序列，否则跳出switch语句，执行后续语句。
## 重要提示
各个分支（包括default）出现的次序可以任意，通常将default放在最后。
每个常量表达式的取值必须各不相同，否则将引起歧义。
允许多个常量表达式对应同一个语句序列。因为：每一个case分支都只作为开关语句的执行入口，执行完该分支后，若无break语句，还将接着执行其后的所有分支中的语句序列。例如：
char score;
cin>>score;
switch (score) {
case ′A′: case ′a′:                      //score='A'和score='a' 时，
cout<<″excellent″; break;    //执行同一语句序列；
case ′B′: case ′b′:                     //score='B'和score='b' 时，
cout<<″good″; break;          //执行同一语句序列；
default: cout<<″fair″;
}
由上可见：break语句可选。若无break语句，则会出现多个常量表达式共用同一语句序列的情况！利用这一点可以精简程序，但往往容易产生歧义甚至错误结果！因此，为保证逻辑的正确实现，通常每个case 分支都与break语句联用。
switch语句的可读性在形式上比嵌套if语句好，但受其条件表达式的取值类型所限，不是所有多选一的问题都可由开关语句完成。不过在有些情况下，可以经过处理使得条件表达式取值符合数据类型的要求，从而用开关语句实现。
## 应用举例
【例2.9】运输公司对所运货物实行分段计费。设运输里程为s，计费情况如下：设每公里每吨基本运费为p，货物重量为w，折扣为d，总运费f的计算公式为：f=p*w*s*(1-d)。设计程序，当输入p、w和s后，计算运费f。
|s<250|不打折扣|
|250<=s<500|2%折扣|
|500<=s<1000|5%折扣|
|1000<=s<2000|8%折扣|
|2000<=s<3000|10%折扣|
|3000<=s|15%折扣|
\#include <iostream>
using namespace std;
int main(){
int c,s;
double p,w,d,f;
cout<<"输入运输单价p，重量w和里程s:"<<endl;
cin>>p>>w>>s;
f=0;
c=s/250;
switch(c){
default:d=0.15;f+=p*w*(s-3000)*(1-d);s=3000;
case 8: case 9: case 10: case 11: d=0.1;f+=p*w*(s-2000)*(1-d);s=2000;
case 4: case 5: case 6:  case 7: d=0.08;f+=p*w*(s-1000)*(1-d);s=1000;
case 2: case 3: d=0.05;f+=p*w*(s-500)*(1-d);s=500;
case 1: d=0.02;f+=p*w*(s-250)*(1-d);s=250;
case 0: d=0;f+=p*w*s*(1-d);
}
cout<<"运输单价:"<<p<<'\t'<<"重量:"<<w<<'\t'<<"里程:"<<s<<endl;
cout<<"折扣后运费:"<<f<<endl;
return 0;
}

【例2.10】设计一个计算器程序，实现加、减、乘、除运算。
\#include <iostream>
using namespace std;
int main(){
float num1,num2;
char op;
cout<<"输入操作数1，运算符，操作数2："<<endl;
cin>>num1>>op>>num2;
switch(op){
case '+':  cout<<num1<<op<<num2<<"="<<num1+num2<<endl; break;
case '-':  cout<<num1<<op<<num2<<"="<<num1-num2<<endl; break;
case '*':  cout<<num1<<op<<num2<<"="<<num1*num2<<endl; break;
case '/':  cout<<num1<<op<<num2<<"="<<num1/num2<<endl; break;
default :  cout<<op<<"是无效运算符!";
}
return 0;
}

