
# CSDN编程挑战（交换字符） - 嵌入式Linux - CSDN博客

2014年04月15日 11:13:31[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：629


如果字符串str3能够由str1和str2中的字符按顺序交替形成，那么称str3为str1和str2的交替字符串。例如str1="abc"，str2="def"，那么"adbecf", "abcdef", "abdecf", "abcdef", "adefbc"等等都为str1和str2的交替字符串。更形式化的，str3的生成算法如下：str3=""while str1不为空 or str2不为空: 把str1或str2的首字符加入到str3,并从str1或str2中删除相应的字符end给定str1,
 str2,和str3，判断str3是否为str1和str2的交替字符串。输入格式：多组数据，每组数据三行，分别是str1,str2,str3。str1,str2的长度在[1..100]范围内，str3的范围在[1..200]范围内。字符串只包含小写英文字母。输出格式：每组数据输出一行YES或者NO。


```python
#include<stdio.h>
/******************************************************************************/
//    DESCRIPTION:     
//    Global resource dependence :  
//    AUTHOR: Owen.wei
//    Note : 交替字符串
/******************************************************************************/
#define bool int
#define false 0
#define true 1
int main()
{
	bool Flag_1=false;
	bool Flag_2=false;
	unsigned int i=0;
	unsigned int j=0;
	char str_1[100];
	char str_2[100];
	char str_3[200];
	gets(str_1);
	gets(str_2);
	gets(str_3);
	if(strlen(str_3)!=(strlen(str_1)+strlen(str_2)))//step 1
	{
		printf("NO\n");
		return 0;
	}
	//printf("%d\n",strlen(str_2));
	for(i=0;i<strlen(str_3);i++)
	{
		//printf("%dof%d\n",i,strlen(str_3));
		Flag_1=false;//重新初始化这个bool变量
		Flag_2=false;//重新初始化这个bool变量
		for(j=0;j<strlen(str_1);j++)
		{
			if(str_3[i]==str_1[j])
			{
				//printf("str_3[%d]=%c\n",i,str_3[i]);
				//printf("str_1[%d]=%c\n",i,str_1[i]);
				if(i==(strlen(str_3)-1))//已经到了最后一个字符
				{
					printf("YES\n");
					break;
				}
				Flag_1=true;
				break;//继续执行下一次for循环
			}
		}
		if(Flag_1==true)
		{
			continue;//如果上面找到的话，就不到再找下面的了，直接重新开始找
		}
		for(j=0;j<strlen(str_2);j++)
		{
			if(str_3[i]==str_2[j])
			{
				//printf("str_3[%d]=%c\n",i,str_3[i]);
				//printf("str_1[%d]=%c\n",i,str_1[i]);
				if(i==(strlen(str_3)-1))//已经到了最后一个字符
				{
					printf("YES\n");
					break;
				}
				//continue;//继续执行下一次for循环
				Flag_2=true;
				break;//继续执行下一次for循环
			}
			if(j==(strlen(str_2)-1))
			{
				printf("NO\n");
				break;
			}
		}
		if(Flag_2==true)
		{
			continue;//如果上面找到的话，就不到再找下面的了，直接重新开始找
		}
	}
	return 0;
}
```


