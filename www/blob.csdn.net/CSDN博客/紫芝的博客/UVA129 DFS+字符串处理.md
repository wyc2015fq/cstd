# UVA129 DFS+字符串处理 - 紫芝的博客 - CSDN博客





2018年04月06日 21:56:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：36
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)











# Krypton Factor



You have been employed by the organisers of a Super Krypton FactorContest in which contestants have very high mental and physical abilities. Inone section of the contest the contestants are tested on their ability torecall a sequenace of characters which has been read to them by the QuizMaster. Many of the contestants are very good at recognising patterns.Therefore, in order to add some difficulty to this test, the organisers havedecided that sequences containing certain types of repeated subsequences shouldnot be used. However, they do not wish to remove all subsequences that arerepeated, since in that case no single character could be repeated. This initself would make the problem too easy for the contestants. Instead it isdecided to eliminate all sequences containing an occurrence of two adjoiningidentical subsequences. Sequences containing such an occurrence will be called“easy”. Other sequences will be called “hard”.

For example, the sequence ABACBCBAD is easy, since it contains anadjoining repetition of the subsequence CB. Other examples of easy sequencesare:

•     BB

•     ABCDACABCAB

•     ABCDABCD

Some examples ofhard sequences are:

•     D

•     DC

•     ABDAB

•     CBABCBA

In order to provide theQuiz Master with a potentially unlimited source of questions you are asked towrite a program that will read input lines from standard input and will writeto standard output.

# Input

Each input line contains integers *n *and *L *(in that order), where *n>*0 and *L *is in the range 1≤*L*≤26. Input isterminated by a line containing two zeroes.

# Output

For each input line prints out the *n*-th hardsequence (composed of letters drawn from the first *L *letters inthe alphabet), in increasing alphabetical order (Alphabetical ordering herecorresponds to the normal ordering encountered in a dictionary), followed (onthe next line) by the length of that sequence. The first sequence in thisordering is ‘A’. You mayassume that for given *n *and *L *there do exist at least *n *hard sequences.

As such a sequence is potentially very long,split it into groups of four (4) characters separated by a space. If there aremore than 16 such groups, please start a new line for the 17th group. Yourprogram may assume a maximum sequence length of 80.

For example,with *L*=3, the first 7 hard sequences are:

A

AB

ABA

ABAC

ABACA

ABACAB

ABACABA

# Sample Input

7    3

30  3

0    0

# Sample Output

ABACABA

7

ABAC    ABCA     CBAB     CABA     CABC     ACBA     CABA

28

```cpp
/*
uva 129 相邻重复子串
题意：
输入正整数 n,l,输出由1前 l个字符组成，字典序第 n小的、 
没有两个相邻重复的子串

思路：
判断ABCABA是否包含重复子串的方法：
运用回溯法，只要比较当前串的后缀，因为回溯法的特性，
所以只考虑当前的情况即可，前面的不用管，这样递归可以保证
从头到尾都是成立的，后缀要从一个比较到 n+1/2个，奇数多考虑一个
*/
```

```cpp
#include<stdio.h>
#include<string.h>
int s[100];
int n, l,num;//用num存储字典序

bool judge(int cur)
{
	int j, k;
	for (j = 1; j * 2 <= cur + 1; j++)//后缀长度从1判断到cur+1，+1是考虑到奇数情况
	{
		bool flag = true;
		for (k = 0; k < j; k++)
			if (s[cur - k] != s[cur - k - j])//前后不相同才合法
			{
				flag = false;
				break;
			}
		if (flag) //说明某一后缀长度时出现相同的情况，说明不合法
			return false;
	}
	return true;
}

void output(int cur)
{
	for (int i = 0; i <= cur; i++)
	{
		if (i % 4 == 0 && i>0)
		{
			if (i % 64 == 0 && i>0)
				putchar('\n');
			else
				putchar(' ');
		}
		putchar(s[i]);
	}
	printf("\n%d\n", cur + 1);
}

bool dfs(int cur)//cur为字母个数
{
	int i;
	for (i = 0; i < l; i++)
	{
		s[cur] = 'A'+i;
		if (judge(cur))
		{
			num++;//每次只要判断正确就+1，但个数可能不变
			if (num == n)
			{
				output(cur);
				return true;
			}
			if (dfs(cur+1))
			return true;	
		}	
	}
	return false;
	
}

int main()
{
	while (scanf("%d%d", &n, &l) != EOF, n + l)
	{
		num = 0;
		dfs(0); 
	}
}
```




