# leetcode（2）：3sum - 刘炫320的博客 - CSDN博客
2018年03月04日 17:31:53[刘炫320](https://me.csdn.net/qq_35082030)阅读数：125标签：[leetcode																[sum																[减治法																[递归																[HashMap](https://so.csdn.net/so/search/s.do?q=HashMap&t=blog)](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=减治法&t=blog)](https://so.csdn.net/so/search/s.do?q=sum&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
在上一讲中，我们讲解了2个数之和的问题，这一次，我们讲解3个数之和的问题。
# 1. leetcode#15. 3Sum
## 1.1问题描述
> 
Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero. 
  Note: The solution set must not contain duplicate triplets.
Example
> 
For example, given array S = [-1, 0, 1, 2, -1, -4], 
  A solution set is: 
  [ 
    [-1, 0, 1], 
    [-1, -1, 2] 
  ]
## 1.2思路
这道题和有2个数之和的题目类似，我们只需要把目标值当成第三个数就可以了，另外一点就是要去除重复的值和保证所有的值出现。
## 1.3代码
```java
public List<List<Integer>> threeSum(int[] nums) {
        int[] num=nums;
        //数组排序
        Arrays.sort(num);
        //结果集合
        List<List<Integer>> res = new LinkedList<>(); 
        //遍历数组内每一个值
        for (int i = 0; i < num.length-2; i++) {
            //如果i是第一个，或者当前值没有统计过。
            if (i == 0 || (i > 0 && num[i] != num[i-1])) {
                //lo为开始值，hi为结束为止，sum为要求的和
                int lo = i+1, hi = num.length-1, sum = 0 - num[i];
                while (lo < hi) {
                    //如果前后和等于当前值
                    if (num[lo] + num[hi] == sum) {
                        //添加三元组
                        res.add(Arrays.asList(num[i], num[lo], num[hi]));
                        //把重复的过滤掉
                        while (lo < hi && num[lo] == num[lo+1]) lo++;
                        while (lo < hi && num[hi] == num[hi-1]) hi--;
                        lo++; hi--;
                    } 
                    //如果前后和小于其和，则把前面的++
                    else if (num[lo] + num[hi] < sum) lo++;
                    //否则后面的--;
                    else hi--;
               }
            }
        }
        return res;
    }
```
这里我们使用的是一个循环加2个指针完成的，时间复杂度为O（$n^2$）。那么有没有更好的办法呢？
## 1.4更近一步
更快的算法则是对数据优先进行手工处理，然后采用分治的方法来实现。例如这里有3个数，其和为0。那么有什么特殊的部分嘛？没错，其解只有4类:零3，正1零1负1，正1负2，正2负1。这时候就是需要对数组进行一个预处理，排序，并且找到0的个数。 
全为0的时候，不用解释。 
当正1零1负1时，只需要迭代正数部分只要能找到对应的负数即可，而且有个小技巧就是保证负数在我们约束的范围内。 
当正1负2时，则有2种情况，第一种是正数为偶数，此时存在两个相同的负数为正数的一半。第二种是正数为奇数，则需要在负数中找到两个和即可（还是用Map）。 
当正2负1时与上一种情况相同。
## 1.5代码
```java
public List<List<Integer>> threeSum(int[] nums) {
        //存放结果值
        List<List<Integer>> res = new ArrayList<>();
        int min;
        int max;
        int[] hash;
        int len = nums.length;
        //如果长度小于3，则没有结果
        if (len < 3)
            return res;
        //对结果进行排序
        Arrays.sort(nums);
        //0的个数
        int zeroCount;
        //最后一个负数的位置（包括0）
        int lastNeg = Arrays.binarySearch(nums, 0);
        //第一个正数的位置
        int firstPos = lastNeg;
        //如果数组包含负数且不存在0
        if(lastNeg < 0){
            //0的个数为0
            zeroCount = 0;
            //找到最后一个负数位置
            lastNeg = -(lastNeg + 1) - 1;
            //找到第一个正数的位置
            firstPos = lastNeg + 1;
        }
        else{
            //则找到第一个真负数
            while(lastNeg > -1 && nums[lastNeg] == 0)
                lastNeg--;
            //找到第一个真正数
            while(firstPos < len && nums[firstPos] == 0)
                firstPos++;
            //统计0的个数
            zeroCount = firstPos - lastNeg - 1;
        }
        //最小值
        min = nums[0];
        //最大值
        max = nums[len - 1];
        //建立hash表
        hash = new int[max - min + 1];
        for (int v : nums) {
            hash[v - min]++;
        }
        //如果有至少3个0，则添加一个结果进入。
        if (zeroCount >= 3) { // 至少包含了3个0
            res.add(Arrays.asList(0, 0, 0));
        }
        //如果包含了0，只考虑1个0的，因为2个0的无法组成
        if (zeroCount > 0 ) { // 包含了0
            //迭代正数, 看是否能找到绝对值相等的负数
            for (int i = firstPos; i < len; i++) { 
                //去除重复的数
                if(i > firstPos && nums[i] == nums[i - 1])
                    continue;
                //如果其反数在数组内，并且存在这个数
                if (-nums[i] - min > -1 && hash[-nums[i] - min] > 0) 
                    //把这个数添加进去
                    res.add(Arrays.asList(0, nums[i], -nums[i]));
            }
        }
        // 一个正数两个负数
        for (int i = firstPos; i < len; i++) { //对所有正数迭代, 看在负数中能否找到两个相应负数
            //去除重复的
            if(i > firstPos && nums[i] == nums[i - 1])
                    continue;
            int half;
            //如果该数是奇数
            if(nums[i] % 2 != 0)
                //一半等于更小的那个半数
                half = -(nums[i] / 2 + 1);
            else{
                half = -(nums[i] / 2);
                //如果存在两个相同的数
                if(half - min > -1 && hash[half - min] > 1)
                    res.add(Arrays.asList(nums[i],half, half));
            }
            //从最后一个负数开始搜索，一直到该数的反数的一半
            for(int j = lastNeg; j > -1 && nums[j] > half; j--){
                //去重
                if(j < lastNeg && nums[j] == nums[j + 1])
                    continue;
                //如果剩下的那个还在
                if((-nums[i] - nums[j]) - min > -1 && hash[(-nums[i] - nums[j]) - min] > 0)
                    res.add(Arrays.asList(nums[i],nums[j], -nums[i] - nums[j]));
            }
        }
        // 一个负数两个正数
        for (int i = lastNeg; i > -1; i--) { //对负数迭代, 看在负数中能否找到两个相应正数
            if(i < lastNeg && nums[i] == nums[i + 1])
                    continue;
            int half;
            if(nums[i] % 2 != 0)
                half = -(nums[i] / 2 - 1);
            else{
                half = -(nums[i] / 2);
                if(half - min < hash.length && hash[half - min] > 1)
                    res.add(Arrays.asList(nums[i], half, half));
            }
            for(int j = firstPos; j < len && nums[j] < half; j++){
                if(j > firstPos && nums[j] == nums[j - 1])
                    continue;
                if((-nums[i] - nums[j]) - min < hash.length && hash[(-nums[i] - nums[j]) - min] > 0)
                    res.add(Arrays.asList(nums[i], nums[j], -nums[i] - nums[j]));
            }
        }
        return res;
    }
```
这里我们学习了另一种思想，那就是当出现有限分类且数目较少时，完全可以通过人工区分再搜索。但是这道题还有衍生题。
# 2.leetcode#16. 3Sum Closest
## 2.1题目描述
> 
Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.
Example
> 
For example, given array S = {-1 2 1 -4}, and target = 1. 
  The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
## 2.2思路
这道题和上一道题思路基本上是一样的，还是和第一次一样左右缩小，然后记录下每一步中最小值和最大值即可。速度已经非常快了。
## 2.3代码
```java
public int threeSumClosest(int[] nums, int target) {
        //边界判断
        if (nums.length <= 2)
            return -1;
        int ret = 0;
        //排序数组
        Arrays.sort(nums);
        int distance = java.lang.Integer.MAX_VALUE;
       //三个指针，固定两个移动一个，比较sum与target，计算tmp_distance与当前最小distance比较，
        //distance始终保留当前最小distance，ret保留最小distance对应的和
        for(int i = 0; i < nums.length-2; i++)
        {
            int j = i+1;
            int k = nums.length-1;
            while(j<k)
            {
                int tmp_val = nums[i]+nums[j]+nums[k];
                int tmp_distance;
                if(tmp_val < target)
                {
                    tmp_distance =target - tmp_val;
                    if(tmp_distance < distance)
                    {
                        distance = tmp_distance;
                        ret = nums[i]+nums[j]+nums[k];
                    }
                    j++;
                }
                else if(tmp_val > target)
                {
                    tmp_distance = tmp_val - target;
                    if(tmp_distance < distance)
                    {
                        distance = tmp_distance;
                        ret = nums[i]+nums[j]+nums[k];
                    }
                    k--;
                }
                else 
                {
                    ret = nums[i]+nums[j]+nums[k];
                    return ret;
                }
            }
        }
        return ret;
    }
```
## 2.4更近一步
但是仍然可以有优化的空间，那就是左右界限是否还是需要一个一个的来减少。试想，如果最小的3个数都已经大于目标值了，那是不是就可以认定如果它比目前的最小值还小，就是最小值了。因为越往后，值越大。这种情况的另一面是，最大的2个数都小于目标值的话，那就可以直接取最大的那个数作为最小值了，而不用把这一次搜索完全遍历。
当然如果这2种情况都没有出现的话，我们就老老实实的一个一个遍历 
，仍然可以使用2个最小数小于最小值来加快搜索。
## 2.5代码
```java
public int threeSumClosest2(int[] nums,int target) {
        //如果小于3个直接结束
        if(nums.length<3) return 0;
        Arrays.sort(nums);
        int min = Integer.MAX_VALUE;int result =Integer.MAX_VALUE;
        //同样开始遍历
        for(int i=0;i<nums.length-2;i++)
        {
            //如果最小的数都已经是大于目标数了。
            if(3*nums[i]>target) 
            {
                int sum3 = nums[i]+nums[i+1]+nums[i+2];
                //判断是否小于目标最小值
                if(Math.abs(sum3-target)<min)  return sum3;
            }
            //左边开始
            int left = i+1;
            //右边结束
            int right = nums.length-1;
            //目标值
            int sum = target - nums[i];
            //如果最大的2个数都小于目标值
            if(2*nums[right]<sum) {
                //直接跳到最大的部分
                int sum2 = nums[i]+nums[right]+nums[right-1];
                //如果小于目标最小值，就替换 
                if(Math.abs(sum2-target)<min){
                     min = Math.abs(target-sum2);
                     result = sum2;
                 }
               continue;
            }
            //正常查找
            while(left<right)
            {
                //全部加起来，如果等于最好
                int temp = nums[i] + nums[left]+nums[right];
                if(temp==target) return target;
                //如果最小的2个数加起来就大于sum了
                if(2*nums[left]>sum) 
                {
                //选取最小的2个数
                  int sumsum = nums[i]+nums[left]+nums[left+1];
                  //如果最小的两个数小于当前最小值，就替换掉
                  if(Math.abs(sumsum-target)<min){
                      min = Math.abs(target-sumsum);
                      result = sumsum;
                    }
                   break;
                }
                //正常情况的话，只需要判断即可。
                else if(Math.abs(target-temp)<min)
                {
                    min = Math.abs(target-temp);
                    result = temp;
                }
                //进行修正
                if(temp<target) 
                   left++;
                else right --;
            }
        }
        return result;
    }
```
# 3leetcode#18. 4Sum
## 3.1题目描述
> 
Given an array S of n integers, are there elements a, b, c, and d in S such that a + b + c + d = target? Find all unique quadruplets in the array which gives the sum of target. 
  Note: The solution set must not contain duplicate quadruplets.
Example
> 
For example, given array S = [1, 0, -1, 0, -2, 2], and target = 0. 
  A solution set is: 
  [ 
    [-1,  0, 0, 1], 
    [-2, -1, 1, 2], 
    [-2,  0, 0, 2] 
  ]
## 3.2思路
其实就是比上面一题多了一层了，现在是4个数之和为0了。当然朴素的思路就是，仍然是遍历，但是需要有条件的遍历，那就是根据约束条件，尽可能的缩小搜索空间。
## 3.3代码
```java
public List<List<Integer>> fourSum(int[] nums, int target) {
            List<List<Integer>> res = new ArrayList<>();
            if(nums.length<4) return res;
            Arrays.sort(nums);
            int n = nums.length;
            //第四个数字
            for(int i=0;i<n-3;i++){
                //直接不可能的
                if(nums[i]+nums[i+1]+nums[i+2]+nums[i+3]>target)break;
                //可以换下一个了
                if(nums[i]+nums[n-1]+nums[n-2]+nums[n-3]<target)continue;
                //避免重复
                if(i>0 && nums[i]==nums[i-1]) continue;
                //第三个数字
                for(int j=i+1;j<n-2;j++){
                    //划线
                    if(nums[i]+nums[j]+nums[j+1]+nums[j+2]>target) break;
                    if(nums[i]+nums[j]+nums[n-2]+nums[n-1]<target) continue;
                    if(j>i+1 && nums[j]==nums[j-1]) continue;
                    int low = j+1, high = n-1;
                    //正常两个数的查找
                    while(low<high){
                        int sum = nums[i]+nums[j]+nums[low]+nums[high];
                        if(sum==target){
                            res.add(Arrays.asList(nums[i],nums[j],nums[low],nums[high]));
                            while(low<high && nums[low]==nums[low+1])low++;
                            while(low<high && nums[high]==nums[high-1])high--;
                            low++;
                            high--;
                        }
                        else if(sum<target) low++;
                        else high--;
                    }
                }
            }
            return res;
     }
```
非常朴素的思想，就是一个一个的来遍历。
# 4.leetcode#454. 4Sum II
## 4.1题目描述
> 
Given four lists A, B, C, D of integer values, compute how many tuples (i, j, k, l) there are such that A[i] + B[j] + C[k] + D[l] is zero. 
  To make problem a bit easier, all A, B, C, D have same length of N where 0 ≤ N ≤ 500. All integers are in the range of -228 to 228 - 1 and the result is guaranteed to be at most 231 - 1.
Example:
> 
Input: 
  A = [ 1, 2] 
  B = [-2,-1] 
  C = [-1, 2] 
  D = [ 0, 2] 
  Output: 
  2 
  Explanation: 
  The two tuples are: 
  1. (0, 0, 0, 1) -> A[0] + B[0] + C[0] + D[1] = 1 + (-2) + (-1) + 2 = 0 
  2. (1, 1, 0, 0) -> A[1] + B[1] + C[0] + D[0] = 2 + (-1) + (-1) + 0 = 0
## 4.2思路
这道题如果按照之前的想法，如论如何都没有办法保证时间，那是因为这两有着本质的区别，就在于目的：只要求计数，不要求详情。这样就可以使用另一种思路来解决。这个思路就是，两两求和，然后再和另一个匹配，因为每一次求和都有唯一性，所以互不干扰。这样就可以使用我们之前说的HashMap来解决。
## 4.3代码
```java
public  static int fourSumCount(int[] A, int[] B, int[] C, int[] D) {
         Map<Integer, Integer> map = new HashMap<>();
         //前两个的和放在map里   
         for(int i=0; i<C.length; i++) {
                for(int j=0; j<D.length; j++) {
                    int sum = C[i] + D[j];
                    map.put(sum, map.getOrDefault(sum, 0) + 1);
                }
            }
            int res=0;
            //用后两个来遍历
            for(int i=0; i<A.length; i++) {
                for(int j=0; j<B.length; j++) {
                    res += map.getOrDefault(-1 * (A[i]+B[j]), 0);
                }
            }
            return res;  
            }
```
# 5.小结
在本讲中，我们主要是以3个数求和展开，看其各种变种。主要收获有以下几点： 
1.在使用暴力搜索时，一定要根据约束条件，尽可能的减少搜索空间。（比如有序的数组上下界等） 
2.对于如果手工加工比使用计算机更容易区分的话，使用手工加工后的结果。 
3.对于不同规模而出现类似的问题，可以使用剪治法来让问题简化。 
4.如果不要求详情而只要求统计时，可能需要一些统计的方法，例如使用HashMap等让统计更加快速。
