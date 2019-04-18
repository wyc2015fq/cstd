# 【C语言】c语言常用的几个函数源代码【strlen,strcpy,strcat,strstr】 - csdn_baotai的博客 - CSDN博客

2018年05月18日 17:38:36[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：292


# 【strlen】

函数说明：C 库函数 size_t strlen(const char *str) 计算字符串 str 的长度，直到空结束字符，但不包括空结束字符。

```cpp
int my_strlen(const char *str)//测试字符字符串长度
{
	int count = 0;
	while (*str)
	{
		count++;
		str++;
	}
	return count;
}

int my_strlen1(const char *str)//应用递归测试字符串长度
{
	if (*str == '0')
		return 0;
	else
		return 1 + my_strlen1(str + 1);
}

int my_strlen2(const char *str)//利用两个指针分别指向字符串的头部和尾部，长度就是两指针之差
{
	char*p = str;
	while (*p != '\0')
	{
		p++;
	}
	return p - str;
}
```

```cpp

```

# 【strcpy】

函数说明：C 库函数 char *strcpy(char *dest, const char *src) 把 src 所指向的字符串复制到 dest。
char *my_strcpy(char *dest, const char*src)
{
	char *ret = dest;
	assert(dest != NULL);
	assert(src != NULL);

	while ((*dest = *src))
	{
		dest++;
		src++;

	}
	return ret;//因为ret是char*类型的，所以返回值类型是char*类型的。
}

```cpp

```

# 【strcat】

函数说明：C 库函数 char *strcat(char *dest, const char *src) 把 src 所指向的字符串追加到 dest 所指向的字符串的结尾。
char *my_strcat(char *dest, const char*src)//字符串拼接函数，将src拼接在dest后面，并且覆盖dest最后的\0.
{
	assert(dest!=NULL);
	assert(src!= NULL);
	char*ret = dest;
	while (*dest)
	{
		dest++;
	}
	while (*dest = *src)
	{
		dest++;
		src++;
	}
	return ret;
}

```cpp

```

# 【strstr】

函数说明：C 库函数 char *strstr(const char *haystack, const char *needle) 在字符串 haystack 中查找第一次出现字符串 needle 的位置，不包含终止符 '\0'。
char *my_strstr(const char *str1, const char*str2)//函数功能：判断str2是否为str1的子串，如果是则返回cp，如果不是则返回NULL.
{
	assert(str1);
	assert(str2);

	char *cp = (char*)str1;//
	char *substr = (char*)str2; //定义两个指针*cp, *substr的原因：由于str1, str2都是const修饰的，不能对其进行修改。
	char*s1 = NULL;
	
	if (*str2 == '/0')
		return NULL;
	while (*cp)
	{
		s1 = cp;
		substr = str2;
		if (*s1&&*substr && (*s1 == *substr));//判断字符串是否相等
		{
			s1++;
			substr++;
		}
		if (*substr == '/0')
		{
			return cp;
		}
		cp++;
	}
}

# **【strcmp】**

函数说明：C 库函数 int strcmp(const char *str1, const char *str2) 把 str1 所指向的字符串和 str2 所指向的字符串进行比较。

## 参数
- **str1** -- 要进行比较的第一个字符串。
- **str2** -- 要进行比较的第二个字符串。

## 返回值

该函数返回值如下：
- 如果返回值 < 0，则表示 str1 小于 str2。
- 如果返回值 > 0，则表示 str2 小于 str1。
- 如果返回值 = 0，则表示 str1 等于 str2。

```cpp
int my_strcmp(const char *src, const char *dest)
{
	int ret;
	while ((ret = *(unsigned char *)src - *(unsigned char*)dest) && dest)
	{
		src++;
		dest++;
	}
		if (ret < 0)
			ret = -1;
		else if (ret>0)
			ret = 1;
		else if (ret = 0)
			ret = 0;
		return(ret);
}
```

# **【memcpy】**

函数说明：C 库函数 void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字符到存储区 str1

```cpp
void*memcpy(void*dest, const void*src, size_t count)//strcmp不会考虑其内存相互重叠的问题
{
	void*ret = dest;
	while (count--)
	{
		*(char*)dest = *(char*)src;//memcpy是按字节拷贝
		dest = (char*)dest + 1;
		src = (char*)src + 1;
	}
	return(ret);
}
```

# **【memmove】**

函数说明：C 库函数 void *memmove(void *str1, const void *str2, size_t n) 从 str2 复制 n 个字符到 str1，但是在重叠内存块这方面，memmove() 是比 memcpy() 更安全的方法。如果目标区域和源区域有重叠的话，memmove() 能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中，复制后源区域的内容会被更改。如果目标区域与源区域没有重叠，则和 memcpy() 函数功能相同。

```cpp
void *memmove(void *dest, const*src, size_t count)
{
	void *ret = dest;
	
	if (dest <= src || (char*)dest >= ((char*)src + count))//dest和src的内存区域没有重叠的情况下
	{
		while (count--)
			*(char*)dest = *(char*)src;
		dest = (char*)dest + 1;
		src = (char*)src + 1;
	}
	else//出现dest和src有内存区域重叠的情况
	{
		dest = (char*)dest + count - 1;
		src = (char*)dest + count - 1;
		while (count--)
		{
			*(char*)dest = *(char*)src;
			dest = (char*)dest - 1;
			src = (char*)src - 1;
		}
	}
	return (ret);
}
```



