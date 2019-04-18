# LeetCode  Happy Number 开心数字 - xcw0754 - 博客园
# [LeetCode  Happy Number 开心数字](https://www.cnblogs.com/xcw0754/p/4467815.html)
题意：
　　给出一个整数n，判断其是否为幸运数。
　　规则是，将n按十进制逐位拆出来后，每个位各自进行取平方，再将这些平方数求和作为新的数字n。若最后n=1，就是幸运数。
思路：
　　计算例子：n=47，接着n=4*4+7*7=65，接着n=6*6+5*5=61，接着....
　　注意有可能陷入无限循环，就是迭代的过程产生了一个环路，即n又重复出现了。
```
1 class Solution {
 2 public:
 3     bool isHappy(int n) {
 4         unordered_set<int> sett;
 5         while(n!=1)
 6         {
 7             if(sett.find(n)!=sett.end())
 8                 return false;
 9             sett.insert(n);
10             int tmp=0;
11             while(n)
12             {
13                 tmp+=((n%10)*(n%10));
14                 n/=10;
15             }
16             n=tmp;
17         }
18         return true;
19     }
20 };
Happy Number
```
python3
```
1 class Solution(object):
 2     def isHappy(self, n):
 3         """
 4         :type n: int
 5         :rtype: bool
 6         """
 7         sett=set([n])
 8         while n>1:
 9             n=sum([int(x)**2 for x in str(n)])
10             if n in sett:    return False
11             sett.add(n)
12         return True        
AC代码
```

