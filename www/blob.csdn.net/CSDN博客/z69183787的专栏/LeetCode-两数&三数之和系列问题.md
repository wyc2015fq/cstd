# LeetCode-两数&三数之和系列问题 - z69183787的专栏 - CSDN博客
2018年05月14日 12:26:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：933

### 三数之和
[LintCode57 三数之和](https://link.jianshu.com?t=http://www.lintcode.com/zh-cn/problem/3sum/)
解题思路：先对数组排序，然后开始遍历，对于数组中的每一个元素，用两指针往中间夹，直到找出所有的解。时间复杂度 O(n^2).
> 
ps：为什么会想到对数组元素进行排序呢，排序是为了让元素之间呈现出某种规律，处理起来会简单很多。所以，当你觉得一个似乎无从下手的问题的时候，不妨尝试去寻找或制造一种“规律”，排序是手段之一。
```java
```java
public class Solution {
    /**
     * @param numbers : Give an array numbers of n integer
     * @return : Find all unique triplets in the array which gives the sum of zero.
     */
    public ArrayList<ArrayList<Integer>> threeSum(int[] numbers) {
        ArrayList<ArrayList<Integer>> res = new ArrayList<ArrayList<Integer>>();
        Arrays.sort(numbers);
        for (int i = 0; i < numbers.length; ++i) {
            int l = i + 1, r = numbers.length - 1;
            while (l < r) {
                ArrayList<Integer> stepRes = new ArrayList<Integer>();
                int sum = numbers[i] + numbers[l] + numbers[r];
                if (sum == 0) {
                    stepRes.add(numbers[i]);
                    stepRes.add(numbers[l]);
                    stepRes.add(numbers[r]);
                    if (!res.contains(stepRes)) // 去重
                        res.add(stepRes);
                }
                if (sum <= 0) ++l;
                else --r;
            }
        }
        return res;
    }
}
```
```
### 最接近的三数之和
[LintCode59 最接近的三数之和](https://link.jianshu.com?t=http://www.lintcode.com/zh-cn/problem/3sum-closest/)
解题的思路与上题大体一致~
```cpp
```cpp
public class Solution {
    /**
     * @param numbers: Give an array numbers of n integer
     * @param target : An integer
     * @return : return the sum of the three integers, the sum closest target.
     */
    
    public int threeSumClosest(int[] numbers, int target) {
        // if (numbers == null || numbers.length == 0) return 0;
        int res = Integer.MAX_VALUE;
        Arrays.sort(numbers); 
        for (int i = 0; i < numbers.length; ++i) { 
            int l = i + 1, r = numbers.length - 1;
            while (l < r) {
                int sum = numbers[i] + numbers[l] + numbers[r];
                if (Math.abs(sum - target) < Math.abs(res - target)) res = sum;
                if (sum <= target) ++l;
                else --r;
            }
        }
        return res;
    }
}
```
```
### 两数之和
[LintCode56 两数之和](https://link.jianshu.com?t=http://www.lintcode.com/zh-cn/problem/two-sum/)
解题思路：使用一个哈希表记录，比如说输入数据 [2,7,11,15],  9
9 - 2 = 7, 让 7 作为 key, 下标+1作为value, 这样遍历到数组中的 7 时就找到解了
非严格情况下，哈希表的查找的插入操作都可以认为是在 O(1) 时间完成的，所以这个解法的时间复杂度为 O(n).
```cpp
```cpp
public class Solution {
    /*
     * @param numbers : An array of Integer
     * @param target : target = numbers[index1] + numbers[index2]
     * @return : [index1 + 1, index2 + 1] (index1 < index2)
     */
    public int[] twoSum(int[] numbers, int target) {
        int[] res = new int[2];
        Hashtable<Integer, Integer> hash = new Hashtable<Integer, Integer>();
        for (int i = 0; i < numbers.length; ++i) {
            if (hash.containsKey(numbers[i])) {
                res[0] = hash.get(numbers[i]);
                res[1] = i + 1;
                return res;
            }
            hash.put(target - numbers[i], i + 1);
        }
        return res;
    }
}
```
```
> 
注意：这里不能使用类似以上两题的解法，因为题目要求返回下标，而排序会丢弃数组元素的下标信息，因此不能排序。
作者：CocoBird
链接：https://www.jianshu.com/p/19b0261c73b9
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
