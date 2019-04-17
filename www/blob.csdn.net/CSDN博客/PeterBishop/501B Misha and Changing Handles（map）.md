# 501B Misha and Changing Handles（map） - PeterBishop - CSDN博客





2018年07月12日 09:32:22[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：21
个人分类：[CodeForces](https://blog.csdn.net/qq_40061421/article/category/7796219)









```cpp
#include <cstdio>
#include <cstring> //题意是说一对用户改名字，可能有一个用户改了很多名字，求用户数目
struct node
{
	char old[25];//旧的名字 
	char new[25];//新的名字 
}a[1010];
int main()
{
	int n;
	scanf("%d",&n);
	int num=0;//记录用户个数 
	for(int i=0;i<n;i++)
	{
		char s1[25],s2[25];
		scanf("%s",s1);
		scanf("%s",s2);
		int flag=0;
		for(int j=0;j<num;j++)//在已知的用户里面查找是否修改为最新id 
		{
			if(strcmp(a[j].new,s1)==0)
			{
				flag=1;
				strcpy(a[j].new,s2);
			}
		}
		if(flag==0)//找不到，那么新建一个用户 
		{
			strcpy(a[num].old,s1);
			strcpy(a[num].new,s2);
			num++;
		}
	}
	printf("%d\n",num);
	for(int i=0;i<num;i++)
	{
		printf("%s %s\n",a[i].old,a[i].new);
	}
	return 0;
}
```




