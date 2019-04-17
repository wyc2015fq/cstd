# leetcode第7题，Reverse Integer - zqjackking的博客 - CSDN博客





2017年02月18日 20:30:16[zqjackking](https://me.csdn.net/zqjackking)阅读数：1932








Reverse digits of an integer. 

Example1: x = 123, return 321 

Example2: x = -123, return -321 

Note: 

The input is assumed to be a 32-bit signed integer. Your function should return 0 when the reversed integer overflows. 

一开始题目有点没看懂。不知道为什么会溢出。其实是因为，给你的是十进制，而二进制有符号32位能表示的位数是有限的。所以反转后还是可能溢出的。
```
int reverse(int x) {
 long  res = 0;
 while(x) {
     res = res*10 + x%10;
     x /= 10;
 }
 return (res<INT_MIN || res>INT_MAX) ? 0 : res;
}
```

```
一开始的答案如上所示，INT_MIN和INT_MAX（头文件：limits.h）分别是-2147483648（-2^31）和2147483647（2^31-1）,是int能表示的最小的和最大的数。
这个答案被accept了，但是只beat了5%左右的人。
后来看到了某大神的答案。
```

```
int reverse(int x) {
int ans = 0;
while (x) {
    int temp = ans * 10 + x % 10;
    if (temp / 10 != ans)  //如果不等的话说明temp已经溢出了
        return 0;
    ans = temp;
    x /= 10;
}
return ans;
}
```

这个能自动检测是否溢出，而不是靠与INT_MAX和INT_MIN比较来检测溢出，这个比较过程非常耗时。




