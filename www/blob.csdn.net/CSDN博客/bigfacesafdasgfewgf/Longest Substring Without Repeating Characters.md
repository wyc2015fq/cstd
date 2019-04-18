# Longest Substring Without Repeating Characters - bigfacesafdasgfewgf - CSDN博客





2015年01月13日 17:06:16[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：358标签：[leetcode																[substring](https://so.csdn.net/so/search/s.do?q=substring&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[Algorithm Probelm																[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)](https://blog.csdn.net/puqutogether/article/category/2812137)







**Longest Substring Without Repeating Characters**



Given a string, find the length of the longest substring without repeating characters. For example,
 the longest substring without repeating letters for "abcabcbb" is "abc", which the length is 3. For "bbbbb" the longest substring is "b", with the length of 1.





下面给出了两种解法，解法一是暴力搜索，时间复杂度为O(n^2)；

解法二的时间复杂度为O(n)。




具体解法的说明在代码中的注释下~





```cpp
#include<iostream>
#include<string>
using namespace std; 

/*
第一种思路，遍历每个字符作为start，遍历所有可能的长度。时间负责度为O(n^2);
另，查找重复的时候，可以使用hash，所以空间复杂度为O(256)
*/

int getLongestLength1(string str)
{
	int longestLength=0, tempLength=1; 
	int len=str.length(); 
	if(len==0)
		return 0; 

	int table[256]={0};   //记录字符是否出现，0/1
	for(int i=0; i<len; i++)
	{
		memset(table, 0, sizeof(table));  //重置为0
		table[str[i]]=1; 
		tempLength=1; 
		for(int j=i+1; j<len; j++)
		{
			if(table[str[j]]==0)  //如果目前还没有重复的
			{
				table[str[j]]=1; 
				tempLength++; 
			}
			else  //如果已经有重复的
			{
				break; 
			}
		}
		longestLength=longestLength>tempLength?longestLength:tempLength; 
	}

	return longestLength; 
}

/*
遍历每个字符，作为终止点，start可变，之间的长度就是候选长度；
注意更新start，它为离终止点最近的非重复字符的位置
*/
int getLongestLength2(string str)
{
	int longestLength=0, tempLength=1; 
	int len=str.length(); 
	if(len==0)
		return 0; 

	int table[256];  //记录字符的位置
	memset(table, -1, sizeof(table)); 
	int start=0; 
	for(int i=0; i<len; i++)
	{
		if(table[str[i]]!=-1)
		{
			if(table[str[i]]>=start)
			{
				start=table[str[i]]+1; 
			}
		}
		table[str[i]]=i; 
		tempLength=i-start; 
		cout<<i<<' '<<start<<endl; 

		longestLength=longestLength>tempLength?longestLength:tempLength; 
	}

	return longestLength+1; 
}

int main()
{
	string str="abbcad"; 
	int longestLength=getLongestLength1(str); 
	cout<<longestLength<<endl; 

	longestLength=getLongestLength2(str); 
	cout<<longestLength<<endl; 

	return 0; 
}
```








