# 大部分程序猿会遗漏的atoi



很多程序猿都说过在自己面试/笔试的时候写漏了atoi，我也不例外。atoi难就难在很少有人能考虑到它所涉及的各个方面，因此博主觉得有必要写一篇文章来详细阐述一下令不少程序猿遗憾的atoi。

## 函数原型

**【头文件】**#include <stdlib.h>

**【函数原型】**atoi() 函数用来将字符串转换成整数(int)，其原型为int atoi (const char * str);

**【函数说明】**atoi() 函数会扫描参数 str 字符串，跳过前面的空白字符（例如空格，tab缩进等，可以通），直到遇上数字或正负符号才开始做转换，而再遇到非数字或字符串结束时('\0')才结束转换，并将结果返回。

**【返回值】**返回转换后的整型数；如果 str 不能转换成 int 或者 str 为空字符串，那么将返回 0。

## 具体分体

**①空格情况：**当输入的字符前面有空格出现时，例如"    123",先通过循环跳过循坏再进行判断，如果全是空格，最后应该返回0。

**②包含非数字字符情况：**当输入的字符包含不是数字的字符，例如"1234abcd"，通过判断非数字字符直接返回即可。

**③正负号情况：**当字符前面出现'+' 、'-'号时，例如"+1234"，先判断正负号，如果存在，用一个标记位来记录这个状态，最后用过判断标记位来显示符号。

**④非数字情况：**输入的字符串中没有数字时，直接返回0即可。

**⑤溢出情况：**当字符串中的字符数字转换的整数太大或者太小，可以通过判断转换后的数字是否处于long的范围内。

**⑥返回值情况：**纯空格情况和非数字情况都会返回0，如果输入的字符恰好是"0"，就无法区分。因此可以通过设置一个全局变量state，通过state的状态来判断是否为正常返回。

## 源代码及注释

```c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <Windows.h>
enum State
{
	NORMAL,	  //正常
	UNNORMAL  //异常
}state;
state = UNNORMAL; //将初始状态赋为异常
int my_atoi(const char *str)
{
	int flag = 1;
	long long ret = 0;
	assert(str);
	while(*str == ' ') //遇到空格就跳过
	{
		str++;
	}
	if(*str == '\0')   //字符串已经遍历完，直接返回0
		return 0;
	if(*str == '+')		//判断正数
		str++;
	else if(*str == '-') //判断负数
	{
		flag = -1;		 //为负数时，先记录状态
		str++;
	}
	while(*str >= '0' && *str <= '9')	  //将字符数字转换成整型
	{
		ret = ret * 10 + flag*(*str - '0');
		if(ret >= INT_MAX || ret <= INT_MIN)  //如果存在溢出，跳出循环
			break;
		str++;
	}
	if(*str == '\0')
		state = NORMAL;
	return (int)ret;
}
int main()
{
	char arr[30] = {0};
	int ret = 0;
	scanf("%s",arr);
	ret = my_atoi(arr);
	if(state == UNNORMAL)
		printf("异常返回\n");
	printf("%d\n",ret);
	system("pause");
	return 0;
}
```

 

