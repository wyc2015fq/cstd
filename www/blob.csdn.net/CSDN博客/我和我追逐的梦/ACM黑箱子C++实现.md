
# ACM黑箱子C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月11日 16:24:10[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1273标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=output&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=output&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=output&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

**ACM黑箱子**
**Description**
**有一个黑箱子，里面会按升序存储整数，你可以对黑箱子下达下面的指令：**
**a. ADD n 将n加入黑箱子**
**b. Get 获得一个数，这个数在黑箱子里的序号（从0开始计数）是Get的出现次数。**
**黑箱子中最初存了一个数0，现给你一个操作序列，要你输出Get命令时获的那个数。****输入：**
**每行是一个命令，如果命令是”ADD”，则后面空一格，有一个整数。输入时保证GET命令不会越界**
**输出：**
**每行输出一个整数，整数为对应Get获得值。**
**Sample Input**
**ADD 3**
**GET**
**ADD 1**
**GET**
**ADD -4**
**ADD 2**
**ADD 8**
**GET**
**GET**
**ADD -1000**
**ADD 2**
**GET**
**Sample Output**
**3**
**3**
**1**
**2**
**2**
**\#include<iostream>**
**using namespace std;**
**char com[3];**
**int a[1000];**
**int main()**
**{**
**int num,sum_add,sum_get;**
**a[0]=0;**
**num=sum_add=sum_get=0;**
**scanf("%s",com);**
**while(sum_get<=sum_add+1)**
**{**
**if(com[0]=='A')**
**{**
**sum_add++;**
**scanf("%d",&num);**
**int i=sum_add-1;**
**while(a[i]>num&&i>=0) { a[i+1]=a[i];i--;}**
**a[i+1]=num;**
**}**
**else**
**{**
**sum_get++;**
**cout<<a[sum_get]<<endl;**
**}**
**scanf("%s",com);**
**}**
**return 0;**
**}**



