# 【C语言】字符串函数strtok  按照指定字符串分割 - 紫芝的博客 - CSDN博客





2018年11月20日 15:47:17[紫芝](https://me.csdn.net/qq_40507857)阅读数：53
个人分类：[C/C++基础知识](https://blog.csdn.net/qq_40507857/article/category/7565821)









# C语言字符串函数 strtok()

## 函数原型

char *strtok(char *str,const char *delimiters);

## 参数
- str，待分割的字符串
- delimiters，分隔符字符串
- 该函数用来将字符串str分割成一个个片段。  参数str指向欲分割的字符串，参数delimiters则为分割字符串中包含的所有字符。

## 用法
- 当strtok()在参数s的字符串中发现参数delimiters中包涵的分割字符时，则会将该字符改为\0 字符。
- 在第一次调用时，strtok()必需给予参数s字符串，往后的调用则将参数s设置成NULL。每次调用成功则返回指向被分割出片段的指针。
- 

## 注意

需要注意的是，使用该函数进行字符串分割时，会破坏被分解字符串的完整，调用前和调用后的s已经不一样了。第一次分割之后，原字符串str是分割完成之后的第一个字符串，剩余的字符串存储在一个静态变量中，因此多线程同时访问该静态变量时，则会出现错误。

```cpp
#include <stdio.h>
#pragma warning(disable:4996)
#include <stdlib.h>
#include<string.h>

int main()
{
	char str[] = "我,是,中国,程序员";
	char *ptr;
	printf("开始前:  str=%s\n", str);
	printf("开始分割:\n");
	ptr = strtok(str, ",");
	while (ptr != NULL) {
		printf("ptr=%s\n", ptr);
		ptr = strtok(NULL, ",");
	}

	char s[]="aaa00bbb00ccc00ddd00";
	ptr=strtok(s,"00");
	while(ptr!=NULL){
        printf("%s\n",ptr);
        ptr=strtok(NULL,"00");
	}
	return 0;
}
```

![运行结果](https://img-blog.csdnimg.cn/20181120154621444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)



