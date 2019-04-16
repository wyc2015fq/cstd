# Linux c 开发 - 常用字符串操作全集 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月22日 18:01:46[initphp](https://me.csdn.net/initphp)阅读数：1094
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









字符串在c语言的开发中经常会用到，所以写这篇总结性的文章，在学习和使用的过程中可以帮助自己。

#### 1. 拷贝字符串 strcpy

功 能：拷贝一个字符串到另一个

用 法：char *stpcpy(char *destin, char *source);

注意：destin必须长度大于源source，否则就装不下了




```cpp
int main(void) {
	char x[10];
	char *y = "hello";
	stpcpy(x, y);
	printf("%s\n", x);
	return EXIT_SUCCESS;
}
```


#### 2. 字符串连接 strcat



功 能： 字符串拼接函数

用 法：char *strcat(char *destin, char *source);

注意：连接的字符串有足够的空间可以容纳连接后的字符串





```cpp
int main(void) {
	//字符串连接
	char x[100] = "Test:";
	char *y = "woshi";
	char *z = "shen";
	strcat(x, y);
	strcat(x, z);
	puts(x);
	return EXIT_SUCCESS;
}
```




#### 3. 在一个字符串中查找字符strchr



功 能：在一个串中查找给定字符的第一个匹配之处

用 法： char *strchr(char *str, char c);

注意：返回一个指针，如果为NULL则没找到

```cpp
int main(void) {
	//查找字符串的匹配处
	char *x = "woshixixihaha";
	char c = 'w';
	char *ptr = strchr(x, c);
	if (ptr) {
		printf("find it");
	} else {
		printf("not find it");
	}
	return EXIT_SUCCESS;
}
```


#### 4. 字符串比较strcmp



功 能：串比较

用 法：int strcmp(char *str1, char *str2);

注意：看Asic码，str1>str2，返回值 >0；两串相等，返回0

```cpp
int main(void) {
	//串比较
	char *x = "woshixixihaha";
	char *y = "woshixixihaha";
	int ptr = strcmp(x, y);
	if (ptr == 0) {
		printf("相等");
	} else {
		printf("不相等");
	}
	return EXIT_SUCCESS;
}
```




#### 5. 字符串长度 strlen



功能：计算字符串长度，计算字符串以零结尾的长度

用 法：  int strlen(char *s);

注意：返回int类型，长度

```cpp
int main(void) {
	//串比较
	char *x = "woshixixihahA";
	int len = strlen(x);
	printf("Len:%d", len);
	return EXIT_SUCCESS;
}
```


#### 6. 交换字节 swab



功 能:：交换字节

用 法：void swab (char *from, char *to, int nbytes);

注意：交换的时候顺序会变





```cpp
int main(void) {
	//串比较
	char x[5] = "hello";
	char y[5] = "";
	swab(x, y, strlen(x));
	printf("x:%s y:%s", x, y);
	return EXIT_SUCCESS;
}
```


















