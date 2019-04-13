
# SAS︱操作语句（if、do、select、retain、array）、宏语言、统计量、运算符号 - 素质云笔记-Recorder... - CSDN博客

2016年10月18日 18:01:35[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：9494标签：[宏																](https://so.csdn.net/so/search/s.do?q=宏&t=blog)[SAS 9.4																](https://so.csdn.net/so/search/s.do?q=SAS 9.4&t=blog)[if																](https://so.csdn.net/so/search/s.do?q=if&t=blog)[retain																](https://so.csdn.net/so/search/s.do?q=retain&t=blog)[运算符号																](https://so.csdn.net/so/search/s.do?q=运算符号&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=retain&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=if&t=blog)个人分类：[SAS																](https://blog.csdn.net/sinat_26917383/article/category/6461415)
[
																								](https://so.csdn.net/so/search/s.do?q=if&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=SAS 9.4&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=SAS 9.4&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=宏&t=blog)


**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
SAS中的一些常见的符号。运算符是一种符号
①比较算符
②算术算符
③逻辑算符
④其它算符

**运算符号**
|比较算符|含义|
|\#NAME?|等于|
|^= 、 NE|不等于|
|> 、 GT|大于|
|< 、 LT|小于|
|>= 、 GE|大于等于|
|<= 、 LE|小于等于|
|IN|等于列表中的一个|
**算术算符**


|比较算符|含义|
|**|乘方|
|*|乘|
|/|除|
|+|加|
|-|减|
**逻辑运算符**

|运算符
|含义
|
|^、NOT
|逻辑非
|
|&、AND
|逻辑与
|
||、OR
|逻辑或
|
——————————————————————————————————————————————————————————

# 一、操作语句

## 1、if语句
`/*if语句*/
if x then delete;
if x=1 then y=1;
else
if x=2 then y=2;else
y=3;
/*抒写结构很有趣*/
if status in (1,2) then newstat="single";
else newstat="not single";
/*表达式的写法*/`if代表判断，为真则执行then后面的内容。
其中newstat代表新的变量。
### 用途1：筛选变量
`data d1 d2;  
set sashelp.class;  
if _n_ le 10 then output d1;  
else output d2;  
run;`其中的le 代表小于等于10,

### 用途2：do强化if的效率
`If age>14 then hcm=30.5*height/12;
if age>14 then do;hcm=30.5*height/12;end;`对比一下两个的关系，第一个是传统的if-then的组合，后面加上do-end，看上去多余，但是这样做之后，可以极大的增加运算效率。

## 2、select语句
`select语句
select;
when (a=1) x=a*10;
when (a=2);
when (a in (3.4.5)) x=a*100;
otherwise;
end;`类似if-then代表着如果when满足a=1，则x=a*10。
不过需要注意繁杂的的otherwiese 以及end作为必要后缀。

## 3、do语句
DO语句必须由一个END语句来结束。DO和END语句之间的这些语句称为一个DO组。
**do语句有三类样式：简单DO语句；循环DO语句；do over语句**
**第一类的简单do，可见if中的用途2，强化了if的效率**
第二类do，实质是循环,下面有一些常用写do语句的方式：
`do i=1 to n ;
do i=n to 1 by –1;
do i=1 to k-1, k+1 to n;
do i=2, 3, 5, 7, 11, 13, 17;
do i=0.1 to 0.9 by 0.1, 1 to 10 by 1, 20 to 100 by 10;
do i='saturday', 'sunday';
do i=‘A’ to ‘Z’;`
一个例子：自然数求和、自然数平方和
`自然数求和、自然数平方和
data a2;
t=0; s=0;
  do n=1 to 100;
  t=t + n;/*n累加*/
  s=s + n**2;/*n2累加*/
  output;
  end;
run;`
## 4、array语句
ARRAY语句用于定义数组。数组通常由一组变量构成。利用数组可以简化很多复杂的数据处理过程。 SAS系统引用数组等价于引用构成数组的那一组变量。

`data aa2;
array c{4} (2,4,6,8);
run;`

## 5、retain语句——变量存储
利用retain语句可以很方便地实现观测值累加、累乘、缺失观测填充等复杂的数据处理。
(1)用于填充缺失值：
`data a12;set resdat.ymret;
retain yret1;
if yret^=. then yret1=yret;
run;`在yret^=.的情况下为yret1赋值为yret,其它情况并没有为yret1赋值。由于retain语句的作用，yret1保留前面的值, 这样yret1不会有缺失值，从而起到填充的作用 。

(2)分组加上组序号：
`分组，加组序号
proc sort data=a12;by stock year;run;
data a13;set a12;by stock year;
retain num 0;
if first.stock then num=num+1;
run;`

——————————————————————————————————————————————————————————


# 二、宏语言
SAS宏的一些特性:
1）可以降低重复性任务的工作量，便于重复调用。例如：可以将宏文件永久存储，每次只需要调用相应的宏就可以完成分析，而无需重新运行。
2）使程序模块化，同时便于修改宏参数，而无需重新运行，可以大幅提高运行效率。


## 1、sas宏变量——可复用的值

一个可以广泛复用，一直保存在缓存中的变量内容。一种在程序（这里一般是指一个宏程序）外定义，则在整个SAS任何地方都可以被调用。另外一种则是在程序内部，则作用在该程序出现宏变量后面，在程序外则不可以被调用，否则会出错。（如果一个程序内部定义有宏变量变成全局变量，则需要使用%global）（这句话参考自：[SAS 定义宏的三种用法](http://bbs.pinggu.org/thread-2171465-1-1.html)）
### （1）%let
`%let a=XXX;
%put &a;
%put here is &a;
显示：
xxx
here is xxx`通过%let定义宏变量，格式：%let 宏变量名=宏变量值；
定义了宏变量之间，需要print一下，就要使用%put，相当于print 一下。
还有这样的用法：
`%let var=you;
data w1;
X="here is &var";
run;`直接用在的文本之中，作为嵌入式。
### (2)%str()
另外宏变量的定义方式有：当宏变量一个表达式，使用：%str()
%let print=%str(proc print; run;);

### (3)%eval()
如果宏变量是数值计算表达式，则需要根据数值类型使用二个不同的宏变量：
%eval_r()   用于计算整数例如：%eval_r(1+2)
%sysevalf() 用于计算浮点数或者空值例如： %eval_r(1.0+2.3)
还有一种就是%eval（），%eval把字符型值变成数值型。


## 2、宏程序——可以复用程序（函数雏形）
**类似R中的Function，不过这里可以完整将代码存在一个函数里面，下次可以重复调用。**
`% macro test;
data a2;
X=1;
run;
%mend test;
%test;`%macro test;  以及 %mend test; 代表着宏程序的开始与结尾，下次调用的话，就直接%test;即可。
不过，要下次开机之后再用的话，必须地永久得对宏进行存储，就像函数一样。
存储的代码：

`/*存储宏程序*/
libname test '';
options mstored sasmstore=test;
% macro test/store;
data a2;
X=1;
run;
%mend test;
/*调用宏程序*/
libname test '';
options mstored sasmstore=test;
%test;`其中，options mstored sasmstore=宏名称;  是关键，同时，% macro test/store;存储时候中需要加入/store

## 3、宏参数——类函数
其实觉得宏程序其实没啥用，宏程序的升级版，宏参数才是最有用的，宏参数类似函数一样。
`%MACRO A;
        1
%MEND A;
%MACRO B(x,y);
        %let z=%eval(&x+&y);
        %put &z;
%MEND B;
%B(13,%A);`
来看看来自经管之家论坛一篇帖子的内容，我觉得把宏变量、宏参数演绎得淋漓尽致。
通过%macro  %mend把A定义为1，以便可以复用；
然后%eval把字符型值变成数值型（关于这个需要深究一下，还有%str的用法）。
其中%B就代表调用函数，然后进行计算。

——————————————————————————————————————————————————————————


# 三、统计量
对数据集的操作。
`data m15;set m14;
  data1=nobs**2;/*乘方用**表示*/
  data2=int(size);/*int:取整*/
  data3=ceil(size);/*返回大于等于变量的最小整数值*/
  data4=floor(size);/*返回小于等于变量的最大整数值*/
  data5=sqrt(data1);/*平方根*/
data6=exp(nobs-1);/*e的x次方*/
  data7=log10(asset);/*以10为底的对数*/
  data8=round(size,0.01);/*保留2位小数后的四舍五入结果*/
  data9=mod(nobs,2);/*求前者除以后者的余数*/
  data10=sign(log(mv)-log(asset));/*符号函数，对正数返回1,0返回0，负数返回-1*/
  data11=max(log(mv),log(asset));
  data12=min(log(mv),log(asset));
run;`
里面包括：乘方、取整、平方根、指数、对数、最大、最小值。
**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


