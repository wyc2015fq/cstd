# 【C语言】C语言中常用函数源代码【strncpy ，strncat ，strncmp】 - csdn_baotai的博客 - CSDN博客

2018年06月03日 16:45:17[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：131


# 【strncpy】

函数描述：C 库函数 char *strncpy(char *dest, const char *src, size_t n) 把 src 所指向的字符串复制到 dest，最

                    多复制 n 个字符。当 src 的长度小于 n     时，dest 的剩余部分将用空字节填充。

```cpp
char* my_strncpy(char*dest, const char*src, int n)
{
	assert(dest);
	assert(src);
	char*p = dest;
	while (n--)
	{
		*p++ = *src++;
	}
	*p = 0;
	return dest;
}
```

## 【strncat】

函数描述：C 库函数 char *strncat(char *dest, const char *src, size_t n) 把 src 所指向的字符串追加到 dest 所指向

                  的字符串的结尾，直到 n 字符长度为止。


```cpp
char* my_strncpy(char*dest, const char*src, int n)
{
	assert(dest);
	assert(src);
	char*p = dest;
	while (n--)
	{
		*p++ = *src++;
	}
	*p = 0;
	return dest;
}
```

【strncmp】

##### 函数描述：C 库函数 int strncmp(const char *str1, const char *str2, size_t n) 把 str1 和 str2 进行比较，最多比较

#####                   前 n 个字节。

```cpp
int my_strncmp(const char*str1, const char*str2, int n)
{
	assert(str1);
	assert(str2);
	while (n--)
	{
		if (*str1 == *str2)
		{
			str1++;
			str2++;
		}
		else
		{
			if ((*str1 - *str2)<0)
				return -1;
			else
				return 1;
		}
	}
	return 0;
}
```



