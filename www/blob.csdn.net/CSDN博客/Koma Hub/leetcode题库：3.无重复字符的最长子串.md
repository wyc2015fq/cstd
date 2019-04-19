# leetcode题库：3.无重复字符的最长子串 - Koma Hub - CSDN博客
2018年04月22日 11:52:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：115
题目描述：
/**
 *题目：无重复字符的最长子串
 *  给定一个字符串，找出不含有重复字符的最长子串的长度。
 *
 *示例：
 *  给定 "abcabcbb" ，没有重复字符的最长子串是 "abc" ，那么长度就是3。
 *  给定 "bbbbb" ，最长的子串就是 "b" ，长度是1。
 *  给定 "pwwkew" ，最长子串是 "wke" ，长度是3。
 *      请注意答案必须是一个子串，"pwke" 是 子序列  而不是子串。
*/
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <string.h>
/* 内存分配上有些问题，有待改进 */
/**
 *题目：无重复字符的最长子串
 *  给定一个字符串，找出不含有重复字符的最长子串的长度。
 *
 *示例：
 *  给定 "abcabcbb" ，没有重复字符的最长子串是 "abc" ，那么长度就是3。
 *  给定 "bbbbb" ，最长的子串就是 "b" ，长度是1。
 *  给定 "pwwkew" ，最长子串是 "wke" ，长度是3。
 *      请注意答案必须是一个子串，"pwke" 是 子序列  而不是子串。
*/
char * Substring(char *s, int begin, int len)
{
	if(begin + len -1 > strlen(s))
		return NULL;
	int i;
	char *str = malloc(sizeof(char)*len);
	for(i=0;i<len;i++)
	{
		str[i] = s[i+begin];
	}
	str[i] = '\0';
	return str;
}
int repeat(char *s)
{
	int i,j,len = strlen(s);
	for(i=0;i<len-1;i++)
	{
		for(j=i+1;j<len;j++)
		{
			//printf("%c, %c\n",s[i],s[j]);
			if(s[i] == s[j])
				return -1;
		}
	}
	return 1;
}
int lengthOfLongestSubstring(char* s) 
{
    int begin, len;printf("-----\n");
	int c = 1;
	char *substring;
	for(begin = 0;begin < strlen(s); begin++)
	{
		for(len=1; len <= strlen(s)-begin; len++)
		{
			
			substring = Substring(s,begin,len);
			
			if(repeat(substring)>0)
			{
				if(c < strlen(substring))
					c = strlen(substring);
			}
			printf("%d, %d, %s, %d, %d\n",
					begin, len,substring,c,repeat(substring));
			free(substring);
		}
	}
	return c;		
}
int main( int argc, char **argv)
{
	char *str = "aaaaaa";
	printf("%d\n",lengthOfLongestSubstring(str) );
	return 0;
}
```
测试：
```
D:\test>a.exe aaaaa
-----
1
D:\test>a.exe abcabcbb
-----
3
D:\test>a.exe abcabcdd
-----
4
```
