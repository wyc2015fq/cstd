# 【AC自动机】Keywords Search _HDU2222 - CD's Coding - CSDN博客





2015年05月19日 00:34:41[糖果天王](https://me.csdn.net/okcd00)阅读数：439标签：[算法																[algorithm																[AC自动机																[Trie																[KMP](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[算法记忆](https://blog.csdn.net/okcd00/article/category/1650753)





Aho-Corasick automation

该算法在1975年产生于贝尔实验室，是著名的多模匹配算法之一。一个常见的例子就是给出n个单词，再给出一段包含m个字符的文章，让你找出有多少个单词在文章里出现过。要搞懂AC自动机，先得有模式树（字典树）Trie和KMP模式匹配算法的基础知识。AC自动机算法分为3步：构造一棵Trie树，构造失败指针和模式匹配过程。





看了下Cnblog上[Booble](http://www.cnblogs.com/Booble/archive/2010/12/05/1897121.html)的讲解，又去瞄了瞄[Kuangbin](http://www.cnblogs.com/kuangbin/p/3164106.html)的AC自动机专题，被这个算法深深的折服了（这两个链接点进去都是不错的讲解，记录于此备查备学）

AC自动机其实就是在Trie树上实现KMP，可以完成多模式串的匹配，常用指数高，是个需要被尽可能早日啃透的算法。







# Keywords Search
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 41555    Accepted Submission(s): 13179**



Problem Description


In the modern time, Search engine came into the life of everybody like Google, Baidu, etc.

Wiskey also wants to bring this feature to his image retrieval system.

Every image have a long description, when users type some keywords to find the image, the system will match the keywords with description of image and show the image which the most keywords be matched.

To simplify the problem, giving you a description of image, and some keywords, you should tell me how many keywords will be match.






Input


First line will contain one integer means how many cases will follow by.

Each case will contain two integers N means the number of keywords and N keywords follow. (N <= 10000)

Each keyword will only contains characters 'a'-'z', and the length will be not longer than 50.

The last line is the description, and the length will be not longer than 1000000.






Output


Print how many keywords are contained in the description.





Sample Input


1
5
she
he
say
shr
her
yasherhs






Sample Output


3






Author


Wiskey





Recommend


lcy   |   We have carefully selected several similar problems for you:  [2896](http://acm.hdu.edu.cn/showproblem.php?pid=2896)[3065](http://acm.hdu.edu.cn/showproblem.php?pid=3065)[2243](http://acm.hdu.edu.cn/showproblem.php?pid=2243)[2825](http://acm.hdu.edu.cn/showproblem.php?pid=2825)[3341](http://acm.hdu.edu.cn/showproblem.php?pid=3341)






先毕恭毕敬奉上邝神的代码：



```cpp
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define Maxn 500010	//Max number of n 
#define bk 26 //Branch-kind number

struct Trie	//AcMachine Trie Tree 
{
    int next[Maxn][bk], fail[Maxn], end[Maxn];
    int root,L;
    int newnode()
    {
        for(int i=0;i<bk;i++) next[L][i]=-1;	//init *Next
        end[L++] = 0;
        return L-1;
    }
    void init()
    {
        L = 0;
        root = newnode();
    }
    void insert(char buf[])
    {
        int len = strlen(buf);
        int now = root;
        for(int i = 0;i < len;i++)
        {
            if(next[now][buf[i]-'a'] == -1)
                next[now][buf[i]-'a'] = newnode();
            now = next[now][buf[i]-'a'];
        }
        end[now]++;
    }
    void build()
    {
        queue<int> Q;
        fail[root] = root;
        for(int i = 0;i < bk;i++)
            if(next[root][i] == -1)
                next[root][i] = root;
            else
            {
                fail[next[root][i]] = root;
                Q.push(next[root][i]);
            }
        while( !Q.empty() )
        {
            int now = Q.front();
            Q.pop();
            for(int i = 0;i < bk;i++)
                if(next[now][i] == -1)
                    next[now][i] = next[fail[now]][i];
                else
                {
                    fail[next[now][i]]=next[fail[now]][i];
                    Q.push(next[now][i]);
                }
        }
    }
    int query(char buf[])
    {
        int len = strlen(buf),
        	now = root,
        	res = 0;
        for(int i = 0;i < len;i++)
        {
            now = next[now][buf[i]-'a'];
            int temp = now;
            while( temp != root )
            {
                res += end[temp];
                end[temp] = 0;
                temp = fail[temp];
            }
        }
        return res;
    }
    void debug()
    {
        for(int i=0;i<L;i++)
        {
            printf("id = %3d,fail = %3d,end = %3d,\nchi = [",i,fail[i],end[i]);
            for(int j = 0;j < 26;j++) printf("%2d",next[i][j]);
            printf("]\n");
        }
    }
};

char buf[Maxn*2];
Trie ac;

int main()
{
    int T,n;
    scanf("%d",&T);
    for(int _T=1; _T<=T; _T++)
    {
        scanf("%d",&n);
        ac.init();
        for(int i = 0;i < n;i++)
        {
            scanf("%s",buf);
            ac.insert(buf);
        }
        ac.build();
        scanf("%s",buf);
        printf("%d\n",ac.query(buf));
        //ac.debug();
    }
    return 0;
}
```


这个所有操作都在Trie树中封装好的Struct写法实在是高超而精简，争取早日啃透然后灵活运用起来~~

Code：

然后上个ACCODE：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
#define N 500010
#define Maxn 51

char str[N*2], keyword[Maxn];
int head, tail;

struct node
{
	node *fail;
	node *next[26];
	int count;
	node() //init
	{
		fail = NULL;
		count = 0;
		for(int i = 0; i < 26; ++i)
			next[i] = NULL;
	}
}*q[N], *root;

void insert(char *str) //Insert into Trie
{
	int temp, len;
	node *p = root;
	len = strlen(str);
	for(int i = 0; i < len; ++i)
	{
		temp = str[i] - 'a';
		if(p->next[temp] == NULL)
			p->next[temp] = new node();
		p = p->next[temp];
	}
	p->count++;
}

void build_ac() //Init Fail_Pointer ,then BFS
{
	q[tail++] = root;
	while(head != tail)
	{
		node *p = q[head++]; //get the head
		node *temp = NULL;
		for(int i = 0; i < 26; ++i)
		{
			if(p->next[i] != NULL)
			{
				if(p == root) //If failed at 1st Letter, jump to root.
					p->next[i]->fail = root;
				else
				{
					temp = p->fail; //'Temp' to mark Fail-Pointer
					while(temp != NULL) //break if Find the Match Or Match-sequence is Null
					{
						if(temp->next[i] != NULL) //Find the Match
						{
							p->next[i]->fail = temp->next[i];
							break;
						}
						temp = temp->fail;
					}
					if(temp == NULL) //If Null,
						p->next[i]->fail = root;
				}
				q[tail++] = p->next[i]; //enqueue
			}
		}
	}
}

int query() //return appearance time
{
	int index, len, result;
	node *p = root; //Trie Entrance
	result = 0;
	len = strlen(str);
	for(int i = 0; i < len; ++i)
	{
		index = str[i] - 'a';
		while(p->next[index] == NULL && p != root) //jump to Fail_Pointer
			p = p->fail;
		p = p->next[index];
		if(p == NULL) p = root;
		node *temp = p; //Let temp record p's location. Then calc it.
		while(temp != root && temp->count != -1)
		{
			result += temp->count;
			temp->count = -1;
			temp = temp->fail;
		}
	}
	return result;
}

int main()
{
	int ncase, num;
	scanf("%d", &ncase);
	for(int _case=1;_case<=ncase;_case++)
	{
		head= tail = 0;
		root = new node();
		scanf("%d", &num);
		getchar();
		for(int i = 0; i < num; ++i)
		{
			gets(keyword);
			insert(keyword);
		}
		build_ac();
		scanf("%s", str);
		printf("%d\n", query());
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Trie&t=blog)](https://so.csdn.net/so/search/s.do?q=AC自动机&t=blog)](https://so.csdn.net/so/search/s.do?q=algorithm&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




