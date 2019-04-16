# ZOJ 1610 Count the Colors【线段树】【暴力】 - 紫芝的博客 - CSDN博客





2018年11月03日 23:09:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：39








Painting some colored segments on a line, some previously painted segments may be covered by some the subsequent ones.

Your task is counting the segments of different colors you can see at last.

Input


The first line of each data set contains exactly one integer n, 1 <= n <= 8000, equal to the number of colored segments.

Each of the following n lines consists of exactly 3 nonnegative integers separated by single spaces:


x1 x2 c


x1 and x2 indicate the left endpoint and right endpoint of the segment, c indicates the color of the segment.

All the numbers are in the range [0, 8000], and they are all integers.

Input may contain several data set, process to the end of file.

Output


Each line of the output should contain a color index that can be seen from the top, following the count of the segments of this color, they should be printed according to the color index.

If some color can't be seen, you shouldn't print it.

Print a blank line after every dataset.

Sample Input


5

0 4 4

0 3 1

3 4 2

0 2 2

0 2 3

4

0 1 1

3 4 1

1 3 2

1 3 1

6

0 1 0

1 2 1

2 3 1

1 2 0

2 3 0

1 2 1

Sample Output


1 1

2 1

3 1

1 1

0 2

1 1

题目大意：给定一段区间，进行N次操做，每次为一段区间涂上一种颜色，最后输出每种颜色占的长度。

解题思路：线段树维护，最后在一一查询计数。

线段树

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int maxn = 8005;
#define lson(x) ((x)<<1)
#define rson(x) (((x)<<1)|1)
int lc[maxn << 2], rc[maxn << 2], s[maxn << 2];

inline void pushdown(int u) {
    if (s[u] != -1) {
        s[lson(u)] = s[rson(u)] = s[u];
        s[u] = -1;
    }
}

void build (int u, int l, int r) {
    lc[u] = l;
    rc[u] = r;
    s[u] = -1;

    if (l == r)
        return;

    int mid = (l + r) >> 1;
    build(lson(u), l, mid);
    build(rson(u), mid + 1, r);
}

void modify(int u, int l, int r, int w) {
    if (l <= lc[u] && rc[u] <= r) {
        s[u] = w;
        return;
    }

    pushdown(u);
    int mid = (lc[u] + rc[u]) >> 1;
    if (l <= mid)
        modify(lson(u), l, r, w);
    if (r > mid)
        modify(rson(u), l, r, w);
}

int query(int u, int x) {
    if (lc[u] == x && x == rc[u])
        return s[u];
    pushdown(u);
    int mid = (lc[u] + rc[u]) >> 1;
    if (x <= mid)
        return query(lson(u), x);
    else
        return query(rson(u), x);
}

int N, ans[maxn];

int main () {

    int l, r, c;
    while (scanf("%d", &N) == 1) {
        build (1, 1, 8000);
        for (int i = 0; i < N; i++) {
            scanf("%d%d%d", &l, &r, &c);
            modify(1, l+1, r , c);
        }

        int pre = -1;
        memset(ans, 0, sizeof(ans));
        for (int i = 1; i <= 8000; i++) {
            int k = query(1, i);
            if (k != pre && k != -1)
                ans[k]++;
            pre = k;
        }

        for (int i = 0; i <= 8000; i++) {
            if (ans[i])
                printf("%d %d\n", i, ans[i]);
        }
        printf("\n");
    }
    return 0;
}
```

暴力

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(8e3+10);
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ca=1;
		while(cin.hasNext()) {
			int[] color=new int[maxn];
			int[] ans=new int[maxn];
			
			int n=cin.nextInt();

			int Max=-1;
			for(int i=0;i<maxn;i++) {
				ans[i]=0;
				color[i]=-1;
			}
				
			for(int i=0;i<n;i++) {
				int x=cin.nextInt();
				int y=cin.nextInt();
				int c=cin.nextInt();
				Max=Math.max(Max, y);
				for(int j=x;j<y;j++)
					color[j]=c;
			}
				for(int i=0;i<Max;i++)
		       {
		          while(i!=0&&color[i]!=-1&&color[i]==color[i-1]) i++;
		          if(color[i]!=-1) ans[color[i]]++;
		       }
			for(int i=0;i<maxn;i++) {
				if(ans[i]!=0) {
					System.out.println(i+" "+ans[i]);
				}
			}
			System.out.println();
		}
		cin.close();
	}
}
```





