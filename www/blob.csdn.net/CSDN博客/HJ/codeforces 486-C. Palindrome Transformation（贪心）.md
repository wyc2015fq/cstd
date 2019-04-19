# codeforces 486-C. Palindrome Transformation（贪心） - HJ - CSDN博客
2017年07月13日 13:39:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：253
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Nam is playing with a string on his computer. The string consists of n lowercase English letters. It is meaningless, so Nam decided to make the string more beautiful, that is to make it be a palindrome by using 4 arrow keys: left, right, up, down.
There is a cursor pointing at some symbol of the string. Suppose that cursor is at position i (1 ≤ i ≤ n, the string uses 1-based indexing) now. Left and right arrow keys are used to move cursor around the string. The string is cyclic, that means that when Nam presses left arrow key, the cursor will move to position i - 1 if i > 1 or to the end of the string (i. e. position n) otherwise. The same holds when he presses the right arrow key (if i = n, the cursor appears at the beginning of the string).
When Nam presses up arrow key, the letter which the text cursor is pointing to will change to the next letter in English alphabet (assuming that alphabet is also cyclic, i. e. after ‘z’ follows ‘a’). The same holds when he presses the down arrow key.
Initially, the text cursor is at position p.
Because Nam has a lot homework to do, he wants to complete this as fast as possible. Can you help him by calculating the minimum number of arrow keys presses to make the string to be a palindrome? 
Input
The first line contains two space-separated integers n (1 ≤ n ≤ 105) and p (1 ≤ p ≤ n), the length of Nam’s string and the initial position of the text cursor.
The next line contains n lowercase characters of Nam’s string. 
Output
Print the minimum number of presses needed to change string into a palindrome. 
Examples 
Input
8 3 
aeabcaez
Output
6
Note
A string is a palindrome if it reads the same forward or reversed.
In the sample test, initial Nam’s string is:![这里写图片描述](http://codeforces.com/predownloaded/81/e1/81e1abed7a3d05fca3e8190d2891d02bada8288d.png) (cursor position is shown bold).
In optimal solution, Nam may do 6 following steps: 
![这里写图片描述](http://codeforces.com/predownloaded/5d/dc/5ddc8cd52e1b8e0ed87a1ca54f8eac9bb476fb6b.png)
The result,![这里写图片描述](http://codeforces.com/predownloaded/ea/34/ea34d79d56e442a04e1a6f31cb06b95221c0b516.png) , is now a palindrome.
题目大意： 
给定一个长度为n的字符串，只由小写字母组成。现在在位置P处有一个光标，光标有左右移动的操作，也有上下改变字符的操作。请输出最少操作数使得这个串为回文串。
解题思路： 
贪心。如果需要改动的字符超过两个，那么需要分两种情况来处理： 
1）如果p在需要改动的字符之间，那么先选择最近的一边，然后迂回，去远的一边。 
2）否则直接一条路走过去即可
```cpp
#include<iostream>
#include<cmath>
using namespace std;
#define maxn 1000005
char s[maxn];
int a[maxn];
int main()
{
    int n,p;
    while(cin>>n>>p>>s)
    {
        int cnt=0,ans=0;
        for(int i=0;i<n/2;i++)
          if(s[i]!=s[n-i-1])
          {
              a[cnt++]=i;
              ans+=abs(s[i]-s[n-i-1])>12?26-abs(s[i]-s[n-i-1]):abs(s[i]-s[n-i-1]);
          }
        p=p>n/2?n-p:p-1;
        ans+=cnt==0?0:cnt==1?abs(a[0]-p):(p>=a[0]&&p<=a[cnt-1])?min(p-a[0],a[cnt-1]-p)+a[cnt-1]-a[0]:max(abs(a[0]-p),abs(a[cnt-1]-p));
        cout<<ans<<endl;
    }
    return 0;
}
```
