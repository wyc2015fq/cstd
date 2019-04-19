# leetcode(12):34. Search for a Range - 刘炫320的博客 - CSDN博客
2018年04月07日 11:44:37[刘炫320](https://me.csdn.net/qq_35082030)阅读数：89
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1.34. Search for a Range
## 1.1题目描述
> 
Given an array of integers sorted in ascending order, find the starting and ending position of a given target value. 
  给定一个已经升序排序的整形数组，找出给定目标值的开始位置和结束位置。 
  Your algorithm’s runtime complexity must be in the order of O(log n). 
  你的算法时间复杂度必须是 O(log n) 级别。 
  If the target is not found in the array, return [-1, -1]. 
  如果在数组中找不到目标，返回 [-1, -1]。 
  For example, 
  Given [5, 7, 7, 8, 8, 10] and target value 8, 
  return [3, 4].
## 1.2思路
其实之前也没想那么多，不过把代码实现后，才发现在英文的Leetcode上是100%，在中文的leetcode上是99.19%，这有点出乎意料，因此写下此题解析。 
这道题目就是排序然后查找，主要问题在于不仅仅查找一个数，而是查找这个数的开始和结束范围。题目中说了时间复杂度必须是O（logn）级别的，这就说明，一定是需要二分查找的。因此，很朴素的思想就是，我先二分查找到这个数，然后往左和往右找到它的边界即可。
## 1.3代码
```java
public int[] searchRange(int[] nums, int target) {
        //要返回的结果
        int[] range=new int[]{-1,-1};
        //二分查找起手式
        int start=0,end=nums.length-1;
        int mid;
        if(nums==null||nums.length==0)
            return range;
        while(start<=end){
            mid=start+(end-start)/2;
            //如果找到了
            if(target==nums[mid]){
                //查找左边界
                int begin=mid,over=mid;
                while(begin>=0&&nums[begin]==nums[mid]){
                    begin--;
                }
                //如果有不相同的
                if(begin>=0){
                    range[0]=begin+1;
                }
                //一直到头
                else{
                    range[0]=0;
                }
                //查找右边界
                while(over<=nums.length-1&&nums[over]==nums[mid]){
                    over++;
                }
                //如果有不相同的
                if(over<=nums.length-1){
                    range[1]=over-1;
                }
                //一直到头
                else{
                    range[1]=nums.length-1;
                }
                break;
            }
            //如果左偏
            else if(target>nums[mid]){
                start=mid+1;
            }
            //如果右偏
            else{
                end=mid-1;
            }
        }
        return range;
    }
```
## 1.4小结
这道题还是考察二分法的变种。相信二分法已经非常熟练了吧。
