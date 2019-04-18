# puts()函数详解 - 心纯净，行致远 - CSDN博客





2018年11月14日 22:47:52[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：523








# 功能

puts()函数用来向标准输出设备屏幕**输出字符串并换行**。

**具体是把字符串输出到屏幕上，将‘\0’转换为回车换行。调用方式是：puts(str)。其中str是字符串数组名或者字符串指针。实际上，数组名就是指针。**

# 用法

int puts(const char *string)

puts()函数包含在头文件<stdio.h>中

# 实例

**1、输出字符串数组**

```cpp
#include <stdio.h>
int main(){
	char str[]="hello world";
	puts(str); 
	return 0;
} 

/*output:
hello world

按任意键退出
*/
```

注意输出的“hello world”后面有一个换行。

**2、从指定字符位置开始输出**

```cpp
#include <stdio.h>
int main(){
	char str[]="hello world";
	puts(str+2); 
	return 0;
} 

/*output:
llo world

按任意键退出
*/
```

# 说明

1、puts()只能输出字符串，不能输出数值或者进行格式转换，即不能要求输出格式增加空格、换行（指的是输出内容的中间进行换行）等要求；

2、可以将字符串直接写入puts()。如：puts("hello world");

3、puts()和 [printf](https://baike.baidu.com/item/printf)的用法一样,puts()函数的作用与语句“printf("%s\n",s);的作用相同。注意：puts在输出字 符串后会自动输出一个回车符。

4、puts()函数的一种实现方案如下：

```cpp
int puts(const char * string)　
{　
    const char * t = string;　
    const char * v = string;　
    int i = 0;　
    while(*t!='\0')　
    {　
        i++;　
        t++;　
    }　
    int j = 0;　
    for(j;j<=i;j++)　
        putchar((v[j]));　
    putchar('\n');
    return 0;　
}
```





