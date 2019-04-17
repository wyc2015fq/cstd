# cena评测系统：自定义校验器（自定义评测插件编写） - 在思索中前行！ - CSDN博客





2016年04月29日 15:29:52[_Tham](https://me.csdn.net/txl16211)阅读数：2397







Cena评测系统，最受欢迎的信息学竞赛离线评测系统。

它是开放源程序的信息学竞赛评测系统，能满足大多数程序设计竞赛的测评需求。

**特色功能：**

通过局域网自动收取选手程序。

高效率的数据文件配置工具。

自动选用对应的编译器来编译选手程序，也能使用解释器或直接评测可执行文件。

能准确测出选手程序的运行时间和内存使用量，并可加入对运行时间和内存使用的限制。

自动比较选手程序的输出和标准输出的差异，并能将两者出现差异的具体位置告诉评测者。

支持使用自定义校验器，可评测有多解的试题。

可对评测结果进行统计分析。

可将评测结果以各种样式打印或导出。


**注意事项：**

Cena 0.8.1 安装程序： cena-0.8.1-20110710-setup.exe[1]  。如果它崩溃，就用这个补丁 cena-0.8.2-patch.zip 里的文件覆盖到安装目录。2011-08-21 发布0.8.2版本。但愿它不再崩溃。（截止2013年8月1日最新版）


**自定义校验器(Special Judge)编辑**

Cena评测软件提供了使用自定义校验器进行评分的功能。可以使用C++或者Pascal进行编写评测插件，其中Pascal需要载入软件所提供的运行库。cena的用处还是很大的，以下是一个关于自定义校验器的模板，使用时需要**放在data文件夹下，要有exe文件**。


cena的SJ主要是由pascal语言编写的，其原因是cena附的用于SJ的库函数是给pascal使用的。

那个库文件位于cena安装文件夹/lib/libcheck.pas。

使用时把写好的SJ程序和libcheck.pas放到同一目录下，然后编译出exe。

在cena的试题配置选项卡当中，"比较方式"选"使用自定义校验器"，然后下面"自定义校验器"一栏选择刚刚编译出来的exe就好了。

想用C++编写SJ的朋友，我也百度到了一个不错的模板。已经试用过了。下面会一起给出一个sample。当然C++代码的编译就和libcheck.pas没关系了，确实省事一些。




```cpp
{cena的帮助文件中对于libcheck.pas的说明}
{注意其中 fso : double 有误，这一变量名应为fsco}
libcheck.pas(位置：安装路径\lib\) 说明如下：
变量：
std: Text; { 用于读取标准输出文件 }
rep: Text; { 用于写入校验信息 }
fso: Double; { 为这个测试点的满分 }
过程：
procedure Score(AScore: Double); { 用于向评测系统报告得分 }
procedure Finish; { 结束 }
```

```cpp
{这个是CEOI 2011 Day1 T1 balloons的SPJ的代码}
{读入n并判断选手输出的n个浮点数是否在允许的误差范围内}
uses
	libcheck;
var
	n,i: longint;
	x,y: extended;
	bin,bout: text;
procedure ends(x:double); {返回得分为x并结束程序}
	begin
		score(x);
		writeln(rep,n,' lines checked');
		writeln(rep,'SJ by wyl8899');
		Finish;
		halt;
	end;
	begin
		assign(bin,'balloons.in'); reset(bin);
		readln(bin,n); close(bin);
		assign(bout,'balloons.out'); reset(bout);
		for i:=1 to n do
			begin
			readln(std,y);
			if seekeof(bout) then
				begin
					writeln(rep,'Output Too Short');
					ends(0);
				end;
				readln(bout,x);
				if (x-y>0.001001)or(y-x>0.001001) then
					begin
						writeln(rep,'Line ',i);
						writeln(rep,'Std:',y:0:3);
						writeln(rep,'W A:',x:0:3);
						ends(0);
					end;
				end;
				if not seekeof(bout) then
					begin
						writeln(rep,'Output Too Long');
						ends(0);
					end;
			ends(fsco);
	end.
```

```cpp
//CEOI 2011 Day1 T1 balloons的SPJ的代码 ( C++ 模板 )
#include<cstdio>
#include<cstdlib>
FILE *fscore,*freport,*fstd,*fin,*fout;
bool Judge()
{
    double x,y,dif;
    fscanf(fstd,"%lf",&x);
    fscanf(fout,"%lf",&y);
    dif=(x>y)?(x-y):(y-x);
    fprintf(freport,"Std:%.10lf\nYour Ans:%.10lf\nDiffer:%.10lf\n",x,y,dif);
    return dif<=0.000001;
}
int main(int argc,char *argv[])
{
    fscore=fopen("score.log","w");	//打开得分文件
    freport=fopen("report.log","w");//打开报告文件
    fstd=fopen(argv[2],"r");		//打开测试点标准输出文件
    int score=atoi(argv[1]);		//取得测试点的分数

    fin=fopen("elephant.in","r");	//打开测试点标准输入文件
    fout=fopen("elephant.out","r"); //打开用户的数据输出文件
    if (!fout)
    {
    	fprintf(fscore,"%d",0);		//返回0分
    	fprintf(freport,"no output");//报告Judge结果为no output
    }
    else if (Judge())  //Judge后结果为真
    {
        fprintf(fscore,"%d",score);//返回满分
        fprintf(freport,"right");  //报告Judge结果为right
    }
    else
    {
        fprintf(fscore,"%d",0);  //返回0分
        fprintf(freport,"wrong");//报告Judge结果为wrong
    }

    fclose(fscore);//关闭得分文件
    fclose(freport);//关闭报告文件
    return 0;
}
```
**实例演示：**


1299. bplusa

☆   输入文件：bplusa.in   输出文件：bplusa.out   评测插件

【题目描述】

输入一个整n，将其拆为两个非负整数a,b，使a,b的和等于n。

【输入格式】

输入数据只有一行，为一个整数。

【输出格式】

输出数据只有一行，两个整数，中间用一个空格隔开。

【样例输入】

5

【样例输出】

2 3

选手代码：





```cpp
#include<stdio.h>

int main()
{
	freopen("bplusa.in","r",stdin);
	freopen("bplusa.ans","w",stdout);
	int n;
	scanf("%d",&n);
	printf("1 %d",n-1);
	return 0;
}
```
Special Judge 代码(评测插件)

```cpp
#include<cstdio>
#include<cstdlib>
FILE *fscore,*freport,*fstd,*fin,*fout;
bool Judge()
{
	int n,x,y,dif;
	fscanf(fin,"%d",&n);
	fscanf(fout,"%d%d",&x,&y);
	dif=n==(x+y)?1:0;
	fprintf(freport,"Std:%d + :%d == n:%d\n",x,y,dif);
	return dif;
}
int main(int argc,char *argv[])
{
	fscore=fopen("score.log","w");	//打开得分文件
	freport=fopen("report.log","w");//打开报告文件
	fstd=fopen(argv[2],"r");		//打开测试点标准输出文件
	int score=atoi(argv[1]);		//取得测试点的分数

	fin=fopen("bplusa.in","r");		//打开测试点标准输入文件
	fout=fopen("bplusa.ans","r");	//打开用户的数据输出文件
	if (!fout)
	{
		fprintf(fscore,"%d",0);		//返回0分
		fprintf(freport,"no output");//报告Judge结果为no output
	}
	else if (Judge())	//Judge后结果为真
	{
		fprintf(fscore,"%d",score);	//返回满分
		fprintf(freport,"right");	//报告Judge结果为right
	}
	else
	{
		fprintf(fscore,"%d",0);//返回0分
		fprintf(freport,"wrong");//报告Judge结果为wrong
	}

	fclose(fscore);		//关闭得分文件
	fclose(freport);	//关闭报告文件
	return 0;
}
```

## **[CENA代码评测软件系列之一：简介](http://youthlin.com/2015988.html)**








