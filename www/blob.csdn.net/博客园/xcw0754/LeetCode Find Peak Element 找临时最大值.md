# LeetCode Find Peak Element 找临时最大值 - xcw0754 - 博客园
# [LeetCode Find Peak Element 找临时最大值](https://www.cnblogs.com/xcw0754/p/4358347.html)
Status: Accepted
Runtime: 9 ms
题意：给一个数组，用Vector容器装的，要求找到一个临时最高点，可以假设有num[-1]和num[n]两个元素，都是无穷小，那么当只有一个元素时，该元素就是最大的了。当然你也可以找最大值，二分法复杂度O(logn)。我的想法是找临时最高点，从左到右，理想情况下，从num[0]之后会值逐渐增大，只要遇到一个比前一元素小的，就找到了。复杂度O(n),这个最大值可能是num[n-1]。
代码：
```
1 class Solution {
 2 public:
 3     int findPeakElement(const vector<int> &num) {
 4         if(num.size()==1)    return 0;
 5         if(num.size()==2)    return num[0]>num[1]?0:1;
 6         int max=num[0],ind=0;
 7         
 8         for(int i=1;i<num.size();i++)
 9         {
10             if(num[i]>max)
11             {
12                 max=num[i];
13                 ind=i;
14             }
15             else    break;
16         }
17         return ind;
18     }
19 };
Find Peak Element
```
附上二分法的代码，因为LeetCode是要在一个类中的一个函数实现，无法更好地利用递归二分法。以下代码是别处COPY的，因为在实现时发现每次递归都需要复制一次数组num，也就需要nlogn的盏空间了，数组大的话就不好了。不推荐此法，当然也可以用非递归二分法。
Status: Accepted
Runtime: 10 ms
```
1 class Solution {
 2 public:
 3     int findPeakElement(const vector<int> &num) {
 4         return Helper(num, 0, num.size()-1);
 5     }
 6     int Helper(const vector<int> &num, int low, int high)
 7     {
 8         if(low == high)
 9             return low;
10         else
11         {
12             int mid1 = (low+high)/2;
13             int mid2 = mid1+1;
14             if(num[mid1] > num[mid2])
15                 return Helper(num, low, mid1);
16             else
17                 return Helper(num, mid2, high);
18         }
19     }
20 };
Find Peak Element
```

