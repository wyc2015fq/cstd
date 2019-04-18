# matlab三大重要数组之字符串数组 - 心纯净，行致远 - CSDN博客





2018年01月23日 00:02:23[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：19359








字符串数组在matlab中尤其重要，很多字符串之间的连接通过简单的字符串操作就可以实现。

字符串的创建方式：在命令窗口中，先把待建立的字符放在“单引号对”中，再按enter。注意，单引号对必须是英文状态下输入，单引号对的作用时matlab用来识别送来内容“身份”（是变量名、数字等）所必须的。

**实例1：数值量与字符串的区别**



```
>> a=12345.6789

a =

   1.2346e+04

>> class(a)   %判别数据类别

ans =

double

>> a_s=size(a)     %确定数组的规模

a_s =

     1     1

>> b='S'

b =

S

>> class(b)

ans =

char

>> b_s=size(b)

b_s =

     1     1

>> whos   %列出工作内存中的变量细节
  Name      Size            Bytes  Class     Attributes

  a         1x1                 8  double              
  a_s       1x2                16  double              
  ans       1x4                 8  char                
  b         1x1                 2  char                
  b_s       1x2                16  double
```

**实例2，串的基本属性、标识和简单操作**

1）创建串数组：直接键入，赋给变量即可。



```
>> a='This is an example.'

a =

This is an example.
```

2）串数组的大小：在以上赋值后，变量a就是一个串数组，串数组中每个字符（每个都算）占据一个元素位。串数组的大小可以用下面的命令获得：



```
>> size(a)

ans =

     1    19
```



3）中文字符串数组：键入时注意单引号对仍然要在英文状态下输入，其他没有什么。



```
>> A='这是算例。'

A =

这是算例。
```

4）字符串的相互连接：字符串的这个功能应该会很有用！



```
>> ab=[A(1:4),'a.1-2',A(5)] %字符串数组也能援引

ab =

这是算例a.1-2。
```
**实例3，实现数值向字符串转换的函数int2str,num2str。**
1）int2str把整数数组转换成串数组（非整数将被四舍五入之后再进行转换）



```
>> A=eye(2,4);
>> A_str=int2str(A) %转换成2*10串数组。

A_str =

1  0  0  0
0  1  0  0

>> size(A) %数值数组的规模很容易理解

ans =

     2     4

>> size(A_str)  %字符串数组之所以显示每行10个，是因为每两个数字字符之间的有两个间隔。

ans =

     2    10
```

字符串数组数字之间两个间隔，可以通过查看每一个字符串的元素得知。



```
>> A_str(1,1)

ans =

1

>> A_str(1,2) 

ans =

 

>> A_str(1,3) 

ans =

 

>> A_str(1,4) 

ans =

0

>> A_str(1,5)

ans =

 

>> A_str(1,6) 

ans =

 

>> A_str(1,7) 

ans =

0

>> A_str(1,8) 

ans =

 

>> A_str(1,9) 

ans =

 

>> A_str(1,10) 

ans =

0
```

2）num2str把非整数数组转换为串数组（常用于图形中数据点的标识）





```
>> rng default %默认全局随机流操控，rng的用法后续会专门研究一下
>> B=rand(2,4); %生成数值矩阵
>> B3=num2str(B,3) %保持3位有效数字，转换为串

B3 =

0.815     0.127     0.632     0.278
0.906     0.913    0.0975     0.547
```

**实例4，在matlab中计算生成的图形上标出图名和最大值点坐标**



```
>> clear
>> a=2; %设置衰减系数
>> w=3; %设置振荡频率
>> t=0:0.01:10;
>> y=exp(-a*t).*sin(w*t); %计算函数值，产生函数数组
>> [y_max,i_max]=max(y); %找最大值元素位置，从下面结果可以看出，y_max是对应的y值，而i_max则是以0.01为步长对应的位置

y_max =

    0.4321


i_max = %最大值对应的是以0.01为步长的第34个位置

    34

>> t(i_max)  %通过位置可以计算出实际的x坐标

ans =

    0.3300

>> t_text=['t=',num2str(t(i_max))] %将横坐标数值转换为字符串

t_text =

t=0.33

>> t_text=['t=',num2str(t(i_max))];
>> y_text=['y=',num2str(y_max)];  %将纵坐标数值转换为字符串
>> max_text=char('maximum',t_text,y_text); %生成标志最大值点的三行字符串
>> tit=['y=exp(-',num2str(a),'t)*sin(',num2str(w),'t)'];    %生成图名的标题
>> plot(t,zeros(size(t)),'k') %画出纵坐标为0的基准线，黑色
>> plot(t,zeros(size(t))) 
>> hold on
>> plot(t,y,'b')
>> plot(t(i_max),y_max,'r.','MarkerSize',20) %用大红点标出最大值点
>> text(t(i_max)+0.3,y_max+0.05,max_text) %在图上书写最大值点的数据值，+0.3和+0.05的目的是将文字偏置放置
>> title(tit),xlabel('t'),ylabel('y')
>> hold off
```


![](https://img-blog.csdn.net/20180122235945101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



