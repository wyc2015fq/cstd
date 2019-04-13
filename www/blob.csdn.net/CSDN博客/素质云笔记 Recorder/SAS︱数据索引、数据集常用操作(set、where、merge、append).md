
# SAS︱数据索引、数据集常用操作(set、where、merge、append) - 素质云笔记-Recorder... - CSDN博客

2016年10月18日 15:06:53[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5875标签：[SAS																](https://so.csdn.net/so/search/s.do?q=SAS&t=blog)[数据合并																](https://so.csdn.net/so/search/s.do?q=数据合并&t=blog)[双set																](https://so.csdn.net/so/search/s.do?q=双set&t=blog)[索引																](https://so.csdn.net/so/search/s.do?q=索引&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=双set&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数据合并&t=blog)个人分类：[SAS																](https://blog.csdn.net/sinat_26917383/article/category/6461415)
[
																								](https://so.csdn.net/so/search/s.do?q=数据合并&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=SAS&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=SAS&t=blog)

代码部分大多来源于姚志勇老师的《SAS编程与数据挖掘商业案例》。

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
——————————————————————————————————


# 一、数据索引

**数据索引的创建有三种方式：data步骤、sql步骤、datasets步骤。**
其中还是有点困惑在data与datasets的区别之上，datasets是对逻辑库中数据集进行操作的方式，而data之后是代表程序的开始。

## 1、data方式创建索引

`data idx(index=(x));
input x y;
cards;
1 2
;
run;`
解读：其中设定(index=(x))为建立索引。

## 2、使用sql的方式

`/*使用sql方式,先有数据，后利用sql方式建立索引*/
proc sql;
crate index y on idx(y) ;      
/*单一索引*/
create index xy on idx(y,x);   
/*复合索引*/
quit;`

## 3、使用datasets方式

`proc datasets lib=work;
modify idx;
index create z/nomiss unique;       
/*单一索引*/
index create (x y z)/nomiss unique;
/*复合索引*/
quit;`解释：需要注意的是，/nomiss这个都是需要的。同时前面带了几个变量就是以这几个变量为唯一索引。有点注意的是，可以同时以“xy”两个变量作为索引变量。

## 4、索引的删除

有全部删除也有部分删除。
`/*删除索引*/
proc sql;
drop index X on idx;
quit;`注意利用sql的方式来调用。drop用来删除。
`/*删除全部索引*/
proc datasets;
modify idx;
index delete _all_;
quit;`全部索引的删除，可以用调用datasets用_all_来进行删除。
——————————————————————————————————

# 二、数据集操作

数据集操作算是数据处理的精髓，一般来说可以用到以下的一些语句：
**/*data，创建数据集*/**
**/*set，读数据，纵向合并数据集*/**
**/*by，控制set merge modify update，分组变量*/**
**/*merge，横向合并数据集*/**
**/*update，更新SAS数据集*/**
**/*modify，修改SAS数据集*/**
**/*put，显示或输出数据行*/**
**/*file，规定当前put语句的输出问及那*/**
**/*infile，规定data步要读入的外部文件*/**


## 1、set
### （1）set-keep联合 提取特定用户
`/*set-keep-挑选变量*/
data keep;
set sashelp.class(keep=name sex);    
/*查看数据，sashelp逻辑库的class数据集，keep相当于 class[,c("name","sex")]   keep代表提取元素，而drop代表剔除元素*/
run;`
keep相当于可以从数据集中抽取变量，keep相当于 class[,c("name","sex")]   keep代表提取元素，而drop代表剔除元素

### （2）set-rename 修改变量名称
`/*set-rename-修改变量名称*/
data keep;
set sashelp.class(keep=name sex rename=(name=name_new sex=sex_new));    
run;`代码的格式是，rename=（原姓名=现姓名）

### （3）set-where 条件选择
`/*set-where-按条件选中*/
data keep;
set sashelp.class(keep=name sex where=(sex='M'));    
run;`where也可以用在数据集的括号中，同时也是where=的样式。其中，sex='M'中的等于，也可以写成sex eq 'M',其中的eq代表等于（此外ne代表不等于）

### （4）set-in 临时变量
`/*set-in-临时单个变量*/
/*可以说是SAS跟R最大的区别的一点就是，SAS内容都是不直接放在内存之中，而是放在数据集中，如果要对数据集的内容进行一些操作，需要先赋值成一些临时变量*/
data keep;
set one(in=a) two(in=b); 
/*one变量变成临时变量a，two变量变成临时变量b，同时合并one two变量*/
in_one=a;
in_two=b;
/*将临时变量a b 赋值新变量名称in_one、In_two*/
if a=1 then flag=1;else flag=0;
/*构造一个新变量flag，为满足某种条件*/
run;`in的作用就是临时变量放入缓存，需要两步才能调用，步骤是先将变量用in赋值为某变量，由于临时变量不能输出，需要用实际变量名称进行输出，需要再赋值一次。
本段代码中，还有一个用if 来重新构造一个变量flag，当a=1时候flag为1，这样的形式就可以构造一个新的变量。

### （5）set-nobs 计总数
`/*set-nobs-数据总数，譬如nrow的意思*/
data nobs11(keep=total);
set sashelp.class nobs=total_obs;
/*将class数据集通过nobs转给临时变量total_obs，然后传给实际变量total，再传出*/
total=total_obs;
output;
stop;
run;`
利用nobs=total_obs，以及total=total_obs的操作来计数。
先用函数obs计数，然后传给临时变量total_obs放入缓存，缓存内容需要再传导给实际变量total。
此外，注意还有output+stop代表单独输出为数据表，而stop的意思是停留在一个单元格，不然就会生成19*1的一列数值，里面都填充着数字19。

### （6）set-end 标记最后
`/*set-end*/
data end;
set sashelp.class end=last_obs;
flag=last_obs;
run;
/*新变量flag，在最后一个变量时间标记为1，是否结尾的标示*/`

### （7）set-point+nobs选中最后一个值

`/*set-point+nobs组合使用*/
data name;
set sashelp.class nobs=last point=last;
	/*	先把数据集数量通过Nobs传给last，然后point整体数量就代表最后*/
output;
stop;
run;`
### （8）数据集合并——横向合并

`/*set-数据集合并*/
data concatenat;
set sashelp.class sashelp.class(obs=10);
/*横向合并，同时sashelp.class(obs=10)代表切片*/
run;`

set 数据集1 数据集2就代表数据集可以横向合并，同时注意   数据集（obs=10）就代表数据集的前10行内容。

### （9）数据集切割
`/*set-数据集切割*/
data d1 d2;
set sashelp.class;
if _n_le 10 then output d1;
else output d2;
run;`
用if来对数据进行切割，用_n_来进行定位，不过这段代码没运行成功。。

### （10）双set
**双set的设置是set用法的最精髓**，也算是SAS为什么效率高的一个原因，简单的理解就是，可以同时读两份文件。
`proc datasets lib=chapt2;  /*逻辑库所在位置*/
modify income;              
/*更新数据集名称*/
index create dtefit=(flightid fltdate)/nomiss unique;
/*dtefit代表索引名称，flightid fltdate代表两个变量名称，复合索引*/
quit;
data profit;
set chapt2.expenses;
set chapt2.income key=dteflt;
/*按索引查找相同的内容，然后组成同一个数据集*/
profit = sum(income,-expenses);
run;
/*双set，相当于数据合并，但是当数据出现缺失值的时候，这样的代码不能解决问题，而且系统会自动填充缺失值，所以需要以下的内容的代码来补充*/`双set，相当于数据合并，但是当数据出现缺失值的时候，这样的代码不能解决问题，而且系统会自动填充缺失值，所以需要以下的内容的代码来补充

`data profit errors;
set chapt2.expenses;
set chapt2.income key=dteflt;
if _iorc_=0 then do;
profit=sum(income,-expenses);
output profit;
end;
else do;
_error_=0;
output errors;
end;
run;`

## 2、merge用法——横向合并
`/*merge 横向合并*/
proc sort data=chapt3.merge_a;by x;run;
proc sort data=chapt3.merge_c;by=x;run;
data d;
merge chapt3.merge_a chapt3.merge_c;
by x;
run;`
SAS合并需要预先进行一些内容的排序，才能进行合并。
**排序：proc sort data=逻辑库.数据集; by=变量名称；run；**
**合并：merge 数据集1 数据集2；by x；**
注意这里合并需要by，同时By是作为单独的代码。

## 3、where 按条件选择
### （1）where-between/and
前面set和where联用可以得到比较好的效果。还有一些可能：
`Where x between 10 and 20;/* X[10,20] */
Where x not between 10 and 20;
Where x between y*0.1 and y*0.5;
where x between 'a' and 'c';`
where-between/and可以作为切片的一种。同时数据集(obs=10)也是切片的一种方式。
`where x in(1,2);
/*选择变量等于某值的情况*/`
选择变量服从某些特征的。

### (2)where在缺失值的应用
`/*where选中缺失值*/
Where x is missing;
where x is null;
/* 数值型变量，定位缺失值，is.na()*/`
有点像R中的is.na()函数。
`Where x;/*选择数值变量x非0非缺失值的观测值*/
Where x and y; /*字符型变量，选择观测值*/
Where x ne '';`
还有一些比较特殊的写法，譬如where x就直接代表选中了非0非缺失值的数据，比较方便。
x ne ''代表，x不等于空格。

### （3）where选中字符型
`where x like 'D_an';
where x like '%ab%' or x like '%z%';
/*字符型匹配，下划线代表一个字符，%代表可以有很多字符*/`
跟SQL里面一样用like来定位字符型内容。其中需要注意的是，D_an代表D和an其中可以容纳一个字符；而D%an代表D和an中可以容纳多个字符。

## 4、append函数——横向合并、添加
`/*append base= data= force 语句*/
/*base是元数据集，data需要添加在后面的数据集，force是强行添加，不太用*/
proc append base=null data=sashelp.class(where=(sex='M'));run;`利用proc append来启动函数，
proc append base=基础数据集 data=添加数据集
**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


