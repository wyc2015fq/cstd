# Hello 2019 - PeterBishop - CSDN博客





2019年01月05日 10:50:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：28
个人分类：[CodeForces](https://blog.csdn.net/qq_40061421/article/category/7796219)









### 2019重新回到codeforces，发现菜的不行，搜索居然卡半天

### A（简单查找）

```cpp
#include<stdio.h>
#include<string.h>


int main() {
	char a[2];
	while (~scanf("%s", a))
	{
		
		char b[5][2];
		int f = 0;
		//printf("%s", a);
		for (int i = 0; i < 5; i++) {
			scanf("%s", b[i]);

		}

		for (int i = 0; i < 5; i++) {
			if ((b[i][0] == a[0]) || (b[i][1] == a[1]))
			{
				f = 1;
				break;
			}
		}
		if (f) {
			printf("YES\n");
		}
		else
		{
			printf("NO\n");
		}
	}
	
	
	return 0;
}
```

### B（暴力搜索）

```
#include <cstdio>
int a[20],n;
bool dfs(int p,int x){
	if(p==n){
		return (x%360==0);
	}
	return dfs(p+1,x+a[p+1])||dfs(p+1,x-a[p+1]);
}
int main(){
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		scanf("%d",&a[i]);
	}
	puts(dfs(0,0)?"YES":"NO");
}
```

### C（括号匹配）

```cpp
#include <iostream>
#include<algorithm>
#include<string.h>
#define MAXN 500010
using namespace std;
int n;
char s[MAXN];
int cnt1[MAXN], cnt2[MAXN];

int main()
{
	int i, j, ans = 0;
	cin >> n;
	for (i = 1; i <= n; i++) {//分别统计每一个字符串中左右括号的个数和出现的次数
		scanf("%s", s + 1);
		int l = strlen(s + 1);
		int t = 0;
		for (j = 1; j <= l; j++) {
			if (s[j] == '(') t++;
			else t--;
			if (t<0) break;
		}
		if (j>l) {
			cnt1[t]++;
		}

		t = 0;
		for (j = l; j >= 1; j--) {
			if (s[j] == ')') t++;
			else t--;
			if (t<0) break;
		}
		if (j<1) {
			cnt2[t]++;
		}
	}
	ans = cnt1[0] / 2;//字符串内出现了左右匹配的个数，除2是因为问的是pair坑死了。。。
	for (i = 1; i<MAXN; i++)
		ans += min(cnt1[i], cnt2[i]);
	printf("%d\n", ans);
	return 0;
}
```





