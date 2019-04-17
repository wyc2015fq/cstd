# 【水模拟】#25 A. IQ test - CD's Coding - CSDN博客





2014年06月03日 22:07:04[糖果天王](https://me.csdn.net/okcd00)阅读数：706
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)









A. IQ test

time limit per test:  2 seconds

memory limit per test:  256 megabytes



Bob is preparing to pass IQ test. The most frequent task in this test is to find out which one of the given *n* numbers differs from the others. Bob observed that one number usually differs from the others in evenness. Help Bob — to check his answers, he needs a program that among the given *n* numbers finds one that is different in evenness.

Input

The first line contains integer *n* (3 ≤ *n* ≤ 100) — amount of numbers in the task. The second line contains *n* space-separated natural numbers, not exceeding 100. It is guaranteed, that exactly one of these numbers differs from the others in evenness.

Output

Output index of number that differs from the others in evenness. Numbers are numbered from 1 in the input order.



Sample test(s)

input

```
5
2 4 7 8 10
```

output

```
3
```

input

```
4
1 2 1 1
```

output

```
2
```





【Update 2018/08/31】

四年多之前的代码被翻出来说代码太长了，emmmm，以前刚开始写代码的时候比较弱用博客来记录一下过题，

确实代码看起来比较乱，但也没有那么多时间来回过头全都重新处理下……



这道题的话，如果想要代码看起来简单一点：

```python
input()
l = [int(x)%2 for x in input().split()]
print(l.index(sum(l)==1)+1)
```

```cpp
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    int t, n, b1=0, b2=0, p1=0, p2=0;
    vector<int> v;
    cin>>t;
    for (int i=0;i<t;i++){
        cin>>n;
        v.push_back(n);
    }
    
    for (int i=0;i<v.size();i++){
        if (v[i]%2==0){
			p1 = i + 1;
			b1++;
		}
        else if (v[i]%2!=0){
			p2 = i + 1;
			b2++;
		}
    }
    
    if (b1==1) cout << p1;
    else if (b2==1) cout << p2;
    return 0;
}
```





【Update 2016/01/18】

以前的我就那么懒得写题意么……

而且感觉上……代码拖好长……



题意：给出一堆不少于三个的数字，其中有且只有一个在奇偶性上和其他的不一样，数字从1开始标号位置（1,2,3,4,5...），请输出那个不一样的数字所在的位置。

那么，我们只要看前三个就知道这个“和其他的不一样”的数字的奇偶性了，

前三个中只要有不少于两个数的奇偶性一致，那我们就反过来可以知道不一样的奇偶性了不是吗？



```cpp
#include <iostream>
using namespace std;
int main()
{
	bool flag=false;
	int n=0,t=0,ans=0;
	int arr[101]; cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>t; arr[i]=(t%2+2)%2;
		if(i==2)
		{
			if((arr[0]+arr[1]+arr[2])%3==0) flag=true; //Same, Get Flag
			else 
			{
				if(arr[0]==arr[1]) ans = 2;
				if(arr[0]==arr[2]) ans = 1;
				if(arr[1]==arr[2]) ans = 0;
				ans++;
			}
		}
		if(flag && i>2 && arr[i]!=arr[0])
		{
			ans=i+1;
			flag = false;	
		}
	}
	cout<<ans<<endl;
	return 0;
}
```

这是刚刚写的，也没有想得太多，如果有哪里不对的地方请务必留言提醒我~ 谢谢~










