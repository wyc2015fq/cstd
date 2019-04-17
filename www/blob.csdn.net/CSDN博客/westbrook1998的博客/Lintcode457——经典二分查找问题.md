# Lintcode457——经典二分查找问题 - westbrook1998的博客 - CSDN博客





2018年02月27日 16:41:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33标签：[Lintcode																[算法																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)





> 
在一个排序数组中找一个数，返回该数出现的任意位置，如果不存在，返回-1  

  您在真实的面试中是否遇到过这个题？  

   样例 给出数组 [1, 2, 2, 4, 5, 5]. 

  对于 target = 2, 返回 1 或者 2.  

  对于 target = 5, 返回 4 或者 5.  

  对于 target = 6, 返回 -1.
经典的二分查找，用了递归和非递归的两种方法 

代码：

```java
import java.awt.*;
import java.util.Arrays;

public class Solution {
    /**
     * 递归的方法
     * @param nums
     * @param target
     * @return
     */
    public int findPosition(int[] nums, int target) {
        if(nums.length==0 || target>nums[nums.length-1] || target<nums[0]){
            return -1;
        }
        else{
            int mid=nums.length/2;
            if(target==nums[mid]){
                return mid;
            }
            else if(target<nums[mid]){
                return new Solution().findPosition(Arrays.copyOfRange(nums,0,mid),target);
            }
            else if(target>nums[mid]){
                int t=new Solution().findPosition(Arrays.copyOfRange(nums,mid,nums.length),target);
                //这里添加一个对-1的判断 否则会出错
                if(t==-1){
                    return t;
                }
                return t+mid;
            }
            else{
                return -1;
            }
        }
    }

    /**
     * 不用递归的方法
     * @param nums
     * @param target
     * @return
     */
    public int findPositionTwo(int[] nums, int target){
        if(nums.length==0 || target>nums[nums.length-1] || target<nums[0]){
            return -1;
        }
        else{
            int start=0,end=nums.length-1,mid;
            while(start<end-1){
                mid=start+(end-start)/2;
                if(target==nums[mid]){
                    return mid;
                }
                else if(target>nums[mid]){
                    start=mid;
                }
                else if(target<nums[mid]){
                    end=mid;
                }
            }
            return -1;
        }
    }
    public static void main(String[] args){
        int[] nums={100,156,189,298,299,300,1001,1002,1003,1004};
        int target=300;
        System.out.println(new Solution().findPositionTwo(nums,target));
    }
}
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)




