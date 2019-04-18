# LeetCode  Median of Two Sorted Arrays 找中位数（技巧） - xcw0754 - 博客园
# [LeetCode  Median of Two Sorted Arrays 找中位数（技巧）](https://www.cnblogs.com/xcw0754/p/4968630.html)
题意：
　　给两个有序（升or降）的数组，求两个数组合并之后的中位数。
思路：
　　按照找第k大的思想，很巧妙。将问题的规模降低，对于每个子问题，k的规模至少减半。 考虑其中一个子问题，在两个有序数组中找第k大，我们的目的只是将k的规模减半而已，所以可以对比nums1[k/2]和nums2[k/2]的大小，假设nums1[k/2]<=nums2[k/2]，那么nums1[0~k/2]这部分必定在0~k之中，那么将这部分删去，k减半，再递归处理。这里面可能有一些细节问题要考虑：
　　1. 如果其中1个数组的大小不及k/2。
　　2. 如果k=1了，两个数组依然还有元素，那么nums1[0]和nums2[0]必有一个为第k大。
　　3. 如果数组大小n+m是奇数还是偶数
　　4. 每个子问题的n大还是m大。
 　　复杂度：k规模每次要么减半，要么就是其中一个数组不够k/2，那么递归到下一次就O(1)解决了。所以复杂度O( logk )，而k=(n+m)/2，所以O( log(n+m) )。
```
1 class Solution {
 2 public:
 3     
 4     typedef vector<int>::iterator it;
 5     
 6     int findKth(it seq1,int size1,it seq2,int size2,int k)
 7     {
 8         if(size1==0)    return seq2[k-1];
 9         if(size1>size2)    return findKth(seq2,size2,seq1,size1,k);    
10         if(k==1)    return min(*seq1,*seq2);
11         
12         int p1=min(size1,k/2);    //保证p1<=p2
13         int p2=k-p1;
14 
15         if(seq1[p1-1]<seq2[p2-1])
16             return findKth(seq1+p1,size1-p1, seq2,size2, k-p1);
17         else
18             return findKth(seq1,size1, seq2+p2,size2-p2, k-p2);
19     }
20 
21 
22     double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
23         int n=nums1.size(), m=nums2.size();
24         if((n+m)&1)    
25             return findKth(nums1.begin(),n, nums2.begin(),m, (n+m+1)/2);
26         else    
27         {
28             double a=findKth(nums1.begin(),n, nums2.begin(),m, (n+m)/2);
29             double b=findKth(nums1.begin(),n, nums2.begin(),m, (n+m)/2+1);
30             return (a+b)/2;
31         }
32     }
33 };
AC代码
```

