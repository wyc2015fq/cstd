# leetcode(5): two array - 刘炫320的博客 - CSDN博客
2018年03月12日 10:47:44[刘炫320](https://me.csdn.net/qq_35082030)阅读数：116标签：[二分检索](https://so.csdn.net/so/search/s.do?q=二分检索&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
# 1.leetcode#4. Median of Two Sorted Arrays
## 1.1问题描述
> 
here are two sorted arrays nums1 and nums2 of size m and n respectively. 
  Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
Example
> 
Example 1: 
  nums1 = [1, 3] 
  nums2 = [2] 
  The median is 2.0 
  Example 2: 
  nums1 = [1, 2] 
  nums2 = [3, 4] 
  The median is (2 + 3)/2 = 2.5
## 1.2思路
这道题就是考虑两个有序数组中位数的。那么对于这种有约束条件的两个变量，我们可以采用“一动一不动”的思想，即只对一个进行搜索，另外一个根据约束条件决定。而一旦要求是O(logn)的时间复杂度，多半是要使用二分检索的思路。而每次搜索的目标就是找到当前的平均值。
那么中位数可以使用一个位置把nums1切成左右两部分，把nums2切成左右两部分，nums1切点位置为i，nums2切点位置为j=(m+n+1)/2-i，这样就能保证“一动一不动”。然后再考虑满足不满足条件，那就是左边的nums1和nums2的最大值都要小于右边nums1和nums2的最小值。最后确定平均数左右值。
## 1.3 代码
```java
public double findMedianSortedArrays(int[] nums1, int[] nums2) {
        int m = nums1.length;
        int n = nums2.length;
        //保证m<n的即nums1比nums2要短,即假设搜索短的那个num1
        if (m > n) {
            return findMedianSortedArrays(nums2, nums1);
        }
        int i = 0, j = 0;
        //定义i的最小值为0,最大值为m，初始化搜索nums1
        int imin = 0, imax = m;
        //定义中间值的位置
        int half = (m + n + 1) / 2;
        //定义最左边的大数为0，最右边的小数为0
        double maxLeft = 0, minRight = 0;
        //当imin小于imax的时候
        while(imin <= imax){
            //找到nums1搜索的位置
            i = (imin + imax) / 2;
            //找到nums2搜索的位置
            j = half - i;
            //如果i和j都没在边界，并且小数部分的Nums2的值大于大数部分的Nums1的值，那么中间值肯定还要往右边靠。
            //这里主要是约束i=m的边界
            if(i<m && nums1[i]< nums2[j - 1]){
                //左边界为当前num1搜索位置的左边一个
                imin = i + 1;
            //如果小数部分nums1的最大值大于大数部分nums2的最小值，那么肯定中位数要往小数部分靠。
            }else if(i > 0 && nums1[i - 1] > nums2[j]){
                //右边界为当前num1搜索位置的右边一个
                imax = i - 1;
            }
            //如果i是适合的时候
            //也就是说，nums1大数部分都大于num1，2的小数部分
            //或者是nums1小数部分小于num1，2的大数部分
            else{
                //如果nums1比nums2所有的都大
                if(i == 0){
                    //左边最大的就是nums2左边最后一个
                    maxLeft = (double)nums2[j - 1];
                }else if(j == 0){
                    //左边最大的为nums1左边最后一个
                    maxLeft = (double)nums1[i - 1];
                }else{
                    //看两者谁更大
                    maxLeft = (double)Math.max(nums1[i - 1], nums2[j - 1]);
                }
                //这时候就已经得出结论了，可以退出了。
                break;
            }
        }
        //如果有1个中位数的话，就返回最左边的那个最大值
        if((m + n) % 2 == 1){
            return maxLeft;
        }
        //如果是两个数之间的平均数
        else{
            //如果当前搜索到尽头了
            if(i == m){
                //右边最小的为nums2的第j个
                minRight = (double)nums2[j];
            }else if(j == n){
                //右边最小的为
                minRight = (double)nums1[i];
            }else{
                //最小的为nums1的第i个和nums2的第j个中较小的那个
                minRight = (double)Math.min(nums1[i], nums2[j]);
            }
            //返回最大与最小的中间值
            return (double)(maxLeft + minRight) / 2;
        }
    }
```
这一题是Hard级别的，是不是有点难了？下面我们放松一下，来一道简单题。
# 2.leetcode#747. Largest Number At Least Twice of Others
## 2.1问题描述
> 
In a given integer array nums, there is always exactly one largest element. 
  Find whether the largest element in the array is at least twice as much as every other number in the array. 
  If it is, return the index of the largest element, otherwise return -1.
Example
> 
Example 1: 
  Input: nums = [3, 6, 1, 0] 
  Output: 1 
  Explanation: 6 is the largest integer, and for every other number in the array x, 
  6 is more than twice as big as x.  The index of value 6 is 1, so we return 1. 
  Example 2: 
  Input: nums = [1, 2, 3, 4] 
  Output: -1 
  Explanation: 4 isn’t at least as big as twice the value of 3, so we return -1.
## 2.2思路
这道题就比较简单了，只需要遍历一遍数组，找到最大值和次大值，再进行比较即可。
## 2.3代码
```java
public int dominantIndex(int[] nums) {
        int index=-1;
        //如果为0，则返回-1；
        if(nums.length==0){
            return -1;
        }
        //如果为1，则返回第一个值
        else if(nums.length==1){
            return 0;
        }
        //否则的话
        else{
            //记录最大值
            int maxvalue=0;
            //记录次大值
            int sub_maxvalue=-1;
            //遍历数组
            for(int i=0;i<nums.length;i++){
                //如果当前值大于次大值
                if(nums[i]>sub_maxvalue){
                    //如果它又大于最大值
                    if(nums[i]>maxvalue){
                        //最大值退位
                        sub_maxvalue=maxvalue;
                        //最大值更替
                        maxvalue=nums[i];
                        //记录当前位置
                        index=i;
                    }
                    else{
                        //只替换次大值
                        sub_maxvalue=nums[i];
                    }
                }
            }
            //判断是否满足约束条件
            if(maxvalue>=2*sub_maxvalue){
                return index;
            }
            else{
                return -1;
            }
        } 
    }
```
# 3.leetcode#162. Find Peak Element
## 3.1问题描述
> 
A peak element is an element that is greater than its neighbors. 
  Given an input array where num[i] ≠ num[i+1], find a peak element and return its index. 
  The array may contain multiple peaks, in that case return the index to any one of the peaks is fine. 
  You may imagine that num[-1] = num[n] = -∞.
Example
> 
For example, in array [1, 2, 3, 1], 3 is a peak element and your function should return the index number 2. 
  Your solution should be in logarithmic complexity.
## 3.2思路
既然是log的时间复杂度，那么我们首先就应该想到是二分检索。这题目简单在了两点，第一点，没有两个相邻的数字是相等的。第二点，首尾都是最小值，一定会出现至少一个峰。也就是说，对于首来说，如果另一端点有一个大值，那么峰就在这边，否则就在另一边。
## 3.3代码
```java
public int findPeakElement2(int[] nums) {
        if (nums.length == 0) {
            return -1;
        }
        int start = 0, end = nums.length-1; 
        //搜索过程
        while(start + 1 <  end) {
            //二分检索
            int mid = (start + end) / 2;
            //因为二分检索包含了分界的那个点，因此不会出现边界情况
            //如果中间值小于左边的，说明峰值在左边
            if(nums[mid] < nums[mid - 1]) {
                end = mid; 
            }
            //否则在右边
            else{ 
                start = mid;
            } 
        }
        //判断约束条件
        if(nums[start] < nums[end]) {
            return end;
        } else { 
            return start;
        }
    }
```
# 4.leetcode#744. Find Smallest Letter Greater Than Target
## 4.1问题描述
> 
Given a list of sorted characters letters containing only lowercase letters, and given a target letter target, find the smallest element in the list that is larger than the given target. 
  Letters also wrap around. For example, if the target is target = ‘z’ and letters = [‘a’, ‘b’], the answer is ‘a’.
Example
> 
Input: 
  letters = [“c”, “f”, “j”] 
  target = “a” 
  Output: “c” 
  Input: 
  letters = [“c”, “f”, “j”] 
  target = “c” 
  Output: “f” 
  Input: 
  letters = [“c”, “f”, “j”] 
  target = “d” 
  Output: “f” 
  Input: 
  letters = [“c”, “f”, “j”] 
  target = “g” 
  Output: “j” 
  Input: 
  letters = [“c”, “f”, “j”] 
  target = “j” 
  Output: “c” 
  Input: 
  letters = [“c”, “f”, “j”] 
  target = “k” 
  Output: “c”
## 4.2思路
同样的题目，只要是涉及到查找，那么基本上都是二分检索，只需要在循环中进行限制条件，然后转向，即可。最后判断符合条件并输出。
## 4.3代码
```java
public static char nextGreatestLetter(char[] letters, char target) {
        int start=0,end=letters.length-1;
        //二分查找
        while(start+1<end){
            int mid=(start+end)/2;
            //限制条件
            if(letters[mid]<=target){
                start=mid;
            }
            else{
                end=mid;
            }
        }
        //判断返回条件
        if(letters[start]>target){
            return letters[start];
        }
        else if(letters[end]>target){
            return letters[end];
        }
        else{
            return letters[0];
        }
    }
```
# 5.小结
我们这次都是进行二分检索的题目，基本上大致思路都已经明白，也就是说，给定一个串，搜索一个结果。一般是限定一个范围，然后逐渐二分，在二分的判断上，使用约束条件来进行分支即可。另外，在处理边界问题时，可以考虑在二分的时候考虑边界元素，最后再一并处理，这样会使编码简单化，而并不会显式的增加时间复杂度。
