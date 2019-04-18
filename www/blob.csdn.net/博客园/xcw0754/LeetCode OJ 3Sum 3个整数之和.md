# LeetCode OJ 3Sum 3个整数之和 - xcw0754 - 博客园
# [LeetCode OJ 3Sum 3个整数之和](https://www.cnblogs.com/xcw0754/p/4385687.html)
题意：给一个vector<int>容器，要求每当找到3个元素之和为0时就将这3个数按大小顺序记下来，用一个二维vector返回。也就是vector< vector<int> >类型的。
思路：2sum是用的两个指针，那么3sum就可以利用2sum的思路解决，假如先挑一个元素出来，则还需挑2个元素，就可以用2sum的思路了。首先将n个元素排序先。怎么挑第1个元素？一共有n种选择，但是为了防止重复，后2个不用挑，留给两个指针来解决，也就是有n-2个元素。而两个指针的可以挑的有哪些呢？假设第1个元素是第i个，那么这两个指针应该在第i个之后的元素挑，为什么呢？
比如数组为：-2 -1 0 1 2
第一轮循环：
　　第一个元素：-2 
　　第二个元素：0
　　第三个元素：2
第2轮循环应该是：
　　第一个元素：-1
　　第二个元素：0　　（难道还想试试-2？ -2可以搭配的所有可能都已经试出来了，这里不用再重试了）
　　第三个元素：1
```
1 class Solution {
 2 public:
 3     vector<vector<int> > threeSum(vector<int> &num) {
 4         sort(num.begin(), num.end());    //排序
 5         vector<int> group(3,1);
 6         vector< vector<int> > ans;    
 7         int n=num.size(),sum,sum2,*pl,*pr,old=10086;
 8         for(int i=0; i<n-2; i++ )
 9         {
10             if( num[i] == old )    continue;//去重，首数字不能重复
11             else old = num[i];
12             sum2 = - num[i];//寻找余下两数之和
13             pl = &num[i+1];//左指针
14             pr = &num[n-1];//右指针
15             while(pl!=pr)
16             {
17                 sum = *pl + *pr;
18                 if( sum == sum2 )
19                 {
20                     if(    group[0]!=num[i] || group[1]!=*pl || group[2]!=*pr)//去重，防止找到的和上一次刚好一样
21                     {
22                         group[0] = num[i];
23                         group[1] = *pl;
24                         group[2] = *pr;
25                         ans.push_back(group);
26                     }
27                     pl++;
28                 }
29                 else if( sum > sum2 )    pr--;
30                 else    pl++;
31             }
32         }
33         return ans;
34     }
35 };
3Sum
```

