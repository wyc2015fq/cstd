# 分别使用c、shell语言实现文件内容合并 -- 2014百度面试题目 - sxf_123456的博客 - CSDN博客
2017年04月25日 10:51:48[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：627
# 请分别使用c语言和shell语言完成文件内容的合并？
### 要求
一个文件A中内容以行存储，如果命名为a1，a2，a3....行，另一个文件B中为b1，b2，b3，b4，b5....。A和B文件内容不一定等长，使用c、shell分别实现内容合并，形式为a1b1a2b2a3b3a4b4b5....。
### 解析
程序环境为Linux环境下gcc编译。
假设A.txt内容为
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cccccccccccccccccccccccccccccccccccccccccccc
eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
1111111111111111111111111111111111111111111111
333333333333333333333333333333
B.txt中内容为
bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
ddddddddddddddddddddddd
ffffffffffffffffffffffffffffffffffffffff
222222222222222222222222222222222
44444444444444444444444444444444444444444444444
555555555555555555555555
6666666666666666666666666666666666666666666666666666666666
假设B文件中内容比A文件中内容多。
### 实现
先使用c语言实现，是c中基本的文件操作。
```cpp
#include <stdio.h>
#include <stdlib.h>
int maxline = 1024;
int main()
{
	FILE *fp1,*fp2,*fp3;
	char str1[maxline];
	char str2[maxline];
	if((fp1 = fopen("/home/atlas/zxl/a.txt","r")) == NULL)
	{
		printf("open file failed\n");
		exit(0);
	}
	if((fp2 = fopen("/home/atlas/zxl/b.txt","r")) == NULL)
	{
		printf("open file failed\n");
		exit(0);
	}
	if((fp3 = fopen("/home/atlas/zxl/c.txt","wb+")) == NULL)
	{
		printf("open file failed\n");
		exit(0);
	}
	while(fgets(str1,maxline,fp1) && fgets(str2,maxline,fp2))
	{
		fputs(str1,fp3);
		fputs(str2,fp3);
	}
	while(fgets(str1,maxline,fp1))
	{
		fputs(str1,fp3);
	}
	while(fgets(str2,maxline,fp2))
	{
		fputs(str2,fp3);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
```
合并后的结果为：
![](https://img-blog.csdn.net/20130924110800125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2R6eGwxOTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用shell脚本的话很简单！！
方法一：直接使用awk神器
`awk '{getline line < "b.txt";print $0;print line}' a.txt > c.txt`ps：这条命令执行后，如果b文件内容长度大于a的，则把b给截断了，没有达到要求，还在解决中，如果你有好方法请告知！谢谢
方法二：使用paste命令，以‘\n’为分隔符，然后使用sed命令删除空白行
```
paste -d "\n" a.txt b.txt > c.txt;
sed  /^$/d c.txt
```
结果为：
![](https://img-blog.csdn.net/20130924112408125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2R6eGwxOTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ok，到此为止，我们使用了c语言和shell工具完成了目标。
如果你有更好的方法请留言交流！
