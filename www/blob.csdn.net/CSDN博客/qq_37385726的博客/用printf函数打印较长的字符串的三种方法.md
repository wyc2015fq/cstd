# 用printf函数打印较长的字符串的三种方法~ - qq_37385726的博客 - CSDN博客





2017年03月16日 12:17:42[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1337









```
#include
#include
#include
#include
int main(void)
{
	printf("Here is one way to printf a ");
	printf("long string.\n");
	printf("Here's another way to print a \
long string.\n");         //即使用\加回车的组合来进行断行
	printf("Here's another way to print a \
		   long string.\n");//使用的时候应从最左端开始，不然输入的缩进将进入到字符串，成为字符串的一部分
	printf("Here is the newest way to print a " 
		"long string.""\n");
	printf("Here is the newest way to print a "    "long string.""\n");//连续的字符串可以等效为一个字符串，前提是连续，中间不能有，
																		//——因为，就是参数的分隔符了，就不是一个连续的参数。但是可以使用空格回车
	system("pause");
	return 0;
}
```




