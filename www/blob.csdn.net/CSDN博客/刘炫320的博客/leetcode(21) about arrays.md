# leetcode(21) about arrays - 刘炫320的博客 - CSDN博客
2018年07月29日 11:14:39[刘炫320](https://me.csdn.net/qq_35082030)阅读数：125
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
今天主要是介绍一些关于数组相关的题目。
# 1.leetcode387 First Unique Character in a string
## 1.1题目描述
> 
Given a string, find the first non-repeating character in it and return it’s index. If it doesn’t exist, return -1.
Example
> 
s = “leetcode”
return 0.
s = “loveleetcode”,
return 2
## 1.2思路
这道题可以有这么几个思路，如果是正常的想法的话。我们只需要记录一下每个字符出现的位置即可，如果遇到了重复出现的，那么就直接赋值为-1，这样再去遍历一遍我们的记录，找到最小的索引即可。可以知道时间复杂度是O(n).空间复杂度也是如此。
## 1.3代码
```java
public int firstUniqChar(String s) {
        HashMap<Character, Integer> map=new HashMap<>();
        char[] chars=s.toCharArray();
        int index=0;
        while (index<chars.length) {
			if(map.containsKey(chars[index])){
				map.put(chars[index], -1);
			}
			else{
				map.put(chars[index], index);
			}
			index++;
		}
        int count=0;
        int min=Integer.MAX_VALUE;
        for (Integer integer : map.values()) {
        	if(integer>=0){
        		min=integer>min?min:integer;
        	}	
        	else{
        			count++;
        		}
		}
        if(count==map.values().size()){
        	return -1;
        }
        return min; }
```
## 1.4更近一步
如果我们不从字符串想，而从字符本身来看的话,我们只需要检测一个字符在字符串的从头检测和从尾检测的位置是否一致
就能知道是否是唯一的了。然后把唯一的记录下来，与最小的比较一下，记录最小值即可。这样时间复杂度是O（n），你问我问什么是这个。首先一点,java里的indexof的方法竟然是最简单的暴力匹配，没有什么高深的算法，但是为什么出奇的快，就是因为我们indexof的字符串都不是很长，直接匹配反而更快。我们只需要查询$26\times 2 \times n$次即可获得最终结果，可以说也是相当的快了。
## 1.5代码
```java
public int firstUniqChar(String s) {
        char c;
        int location = s.length();
        for(c = 'a'; c <= 'z'; c++){
            int temp = s.indexOf(c);
            if (temp != -1 && temp == s.lastIndexOf(c)) {
                location = Math.min(temp, location);
            }
        }
        return location == s.length() ? -1 : location;
    }
```
# 2. leetcode268. Missing Number
## 2.1题目描述
> 
Given an array containing n distinct numbers taken from 0, 1, 2, …, n, find the one that is missing from the array.
Example
> 
Input: [3,0,1]
Output: 2
Input: [9,6,4,2,3,5,7,0,1]
Output: 8
## 2.2思路
直观上我们使用乘以-1的方法好像是可以的，但是写到后面发现好像是不行，因为这里有0到n共n+1个数中缺少一个。因此很难确定是否是0，因为这个值没有正负，最简单的可以使用二分查找，这样的时间复杂度为O（nlogn）。
## 2.3代码
```java
public int missingNumber(int[] nums) { //binary search
    Arrays.sort(nums);
    int left = 0, right = nums.length, mid= (left + right)/2;
    while(left<right){
        mid = (left + right)/2;
        if(nums[mid]>mid) right = mid;
        else left = mid+1;
    }
    return left;
}
```
## 2.4进一步思考
事实上主要是由于排序需要花费时间，如果是排好序的，那么使用二分查找可以达到基于查找算法的下限O（logn），但是这里不能够使用。但是我们注意到一个偷懒的方法，既然个体不能够解决，那么是不是求和可以做到呢？这样时间复杂度可以为O（n）
## 2.5代码
```java
public int missingNumber(int[] nums) { //sum
    int len = nums.length;
    int sum = (0+len)*(len+1)/2;
    for(int i=0; i<len; i++)
        sum-=nums[i];
    return sum;
}
```
## 2.6更上一层楼
上述的思路基本正确了，但是忽略了一点，那就是如果n很大，那么sum怕是放不下会溢出，那么该怎么办呢？别急，我们可以这样做。
## 2.7代码
```java
public int missingNumber(int[] nums) {
        int sum=0;
        for(int i=0;i<nums.length;i++){
        	sum+=(i-nums[i]);
        }
        sum+=nums.length;
        return sum;
    }
```
## 2.8异或骚操作
这样就完美的解决了溢出的问题。当然如果你喜欢用异或来做，那自然是最好不过了。思路和上面的一样。
## 2.9代码
```java
public int missingNumber(int[] nums) { //xor
    int res = nums.length;
    for(int i=0; i<nums.length; i++){
        res ^= i;
        res ^= nums[i];
    }
    return res;
}
```
# 3.leetcode350. Intersection of Two Arrays II
## 3.1题目描述
> 
Given two arrays, write a function to compute their intersection.
Example
> 
Given nums1 = [1, 2, 2, 1], nums2 = [2, 2], return [2, 2].
## 3.2思路
这道题目其实有很多更深一步的挖掘，其中一个就是给定的数组排好序了，这样会容易一些。我们首先确定nums1要比nums2短，这样最终的集合一定是短的集合的子集。另外我们通过查询
## 3.3代码
```java
public int[] intersect(int[] nums1, int[] nums2) {
        if(nums1.length>nums2.length){
        	return intersect(nums2, nums1);
        }
        Arrays.sort(nums1);
        Arrays.sort(nums2);
        // the nums1.length<nums2.length;
        ArrayList<Integer> result=new ArrayList<>();
        int nums1index=0,nums2index=0;
        for(nums1index=0;nums1index<nums1.length;nums1index++){
	        //if nums1 larger than nums2，nums2 keep moving。
        	while(nums2index<nums2.length&&nums1[nums1index]>nums2[nums2index]){
        		nums2index++;
        	}
        	//if Nums1 is equal to nums2，add the number  to result.
        	if(nums2index<nums2.length&&nums1[nums1index]==nums2[nums2index]){
        		result.add(nums1[nums1index]);
        		nums2index++;
        	}
        }
        int[] res=new int[result.size()];
        for(int i=0;i<result.size();i++){
        	res[i]=result.get(i);
        }
        return res;
    }
```
# 4.小结
上面主要是提及了3个关于数组的题目，由于最近网络不是太好，leetcode时常抽风，因此写起来会慢很多。
