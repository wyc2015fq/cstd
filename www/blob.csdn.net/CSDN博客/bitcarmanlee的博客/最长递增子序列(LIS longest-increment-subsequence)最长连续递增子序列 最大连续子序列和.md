
# 最长递增子序列(LIS longest-increment-subsequence)最长连续递增子序列 最大连续子序列和 - bitcarmanlee的博客 - CSDN博客


2019年03月27日 10:09:09[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：52



## 1.最长递增子序列(LIS)
### 1.问题描述
给定一个数组，就数组最长递增子序列(子序列可以不连续)
### 2.解法
非常经典的动态规划问题，算法的时间复杂为O(n^2)，空间复杂度为O(n)。
关键是结果数组dp[i]怎么计算呢？
每次遍历所有j<i中数组的元素，判断array[j]是否小于array[i]。
如果是，检查dp[i]与dp[j]+1的大小，并且更新dp[i]。
`public static int lis() {
        int[] nums = {1, 3, 6, 2, 3, 4};
        int len = nums.length;
        int[] dp = new int[len];
        int result = 0;
        for(int i=0; i<len; i++) {
            dp[i] = 1;
            for(int j=0; j<i; j++) {
                if(nums[j] < nums[i]) {
                    dp[i] = dp[i] > dp[j] + 1 ? dp[i] : dp[j] + 1;
                }
            }
            result = dp[i] > result ? dp[i] : result;
        }
        return result;
    }`
### 3.O(NlgN)复杂度算法
上述解法的时间复杂度为O(N^2)。有没有更快速的解法呢？答案是可以的。
定义d[k]:长度为k的上升子序列的最末元素，若有多个长度为k的上升子序列，则记录最小的那个最末元素。
d中的元素是单调递增的，一旦是有序序列，我们处理起来相对就会容易很多，减小时间复杂度主要就是靠他了。
以a = [1, 3, 6, 2, 3, 4]为例，首先令len=0，循环从i=0开始。然后对a[i]：若a[i]>d[len]，len++, d[len]=a[i]，此时d = [1]。
i=1时，此时a[i] = 3，我们需要将a[i]有序地放入d中。因为d是个有序数组，我们用二分查找很容易找到a[i]应该插入的位置。假设此时插入的位置为position，如果position > len，很明显此时len++。具体到例子中，此时position=1，len=1, d = [1, 3]。
同理i=2时，此时position=3, len=2, d = [1, 3, 6]。
当i=3时，此时a[i] = 2，position=2, d = [1, 2, 6]。因为postion < len，所以len不更新，只是更新d。
当i=4时，position=3，d = [0, 1, 2, 3]，同理不更新len只更新d。
当i=5是，position=4，d = [0, 1, 2, 3, 4]。因为postion > len，所以len++，此时len=3。又因为len是从0开始计数，最后的结果为len+1=4。
`public void longestincsubarrayeasy() {
        int[] array = {1, 3, 6, 2, 3, 4};
        int len = 0;
        int[] tmp = new int[array.length];
        tmp[0] = array[0];
        for(int i=0; i<array.length; i++) {
            int position = binSearch(tmp, 0, len, array[i]);
            tmp[position] = array[i];
            if (position > len) {
                len = position;
            }
        }
        // 最终的长度为len+1
        System.out.println("len is: " + (len+1));
    }
    public int binSearch(int[] array, int left, int right, int target) {
        int mid ;
        if(array[right] < target) {
            return right + 1;
        } else {
            while(left < right) {
                mid = (left + right) / 2;
                if(array[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }
        }
        return left;
    }`
## 2.最长连续递增子序列
上面的问题是递增子序列，如果要求连续递增子序列怎么办呢？
其实这个问题比上面那一个要简单，就是找递增的数列。如果遇到非递增的情况，比较一下当前计数值与max的大小得到新的max，然后将当前计数值重置为1即可。
`public void contlongestincsubarray() {
        int[] nums = {1, 3, 5, 4, 7, 8, 9, 10, 1};
        int count = 1;
        int result = 0;
        int begin = 0;
        for(int i=1; i<nums.length; i++) {
            if (nums[i-1] < nums[i]) {
                count++;
            } else {
                // 更新begin的位置
                if (count > result) {
                    begin = i - count;
                }
                result = Math.max(result, count);
                count = 1;
            }
        }
        // 如果else中没有更新，保证最后要更新begin的位置与result
        if (count > result) {
            begin = nums.length - count;
        }
        result = Math.max(result, count);
        System.out.println("result is: " + result);
        System.out.println("begin is: " + begin);
    }`最后的输出结果为：
`result is: 5
begin is: 3`
## 3.最大连续子序列和
因为最大连续子序列和只可能是以位置0～n-1中某个位置结尾。当遍历到第i个元素时，判断在它前面的连续子序列和是否大于0，如果大于0，则以位置i结尾的最大连续子序列和为元素i和前门的连续子序列和相加；否则，则以位置i结尾的最大连续子序列和为元素i。
`public void subMaxSum() {
        int[] array = {1, 3, -2, 4, -5};
        int maxsum = array[0];
        int cursum = array[0];
        for(int i=1; i<array.length; i++) {
            cursum = cursum > 0 ? cursum + array[i] : array[i];
            if (cursum > maxsum) {
                maxsum = cursum;
            }
        }
        System.out.println(maxsum);
    }`

