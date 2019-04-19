# leetcode(19):Anagrams （字谜游戏） - 刘炫320的博客 - CSDN博客
2018年04月21日 17:23:34[刘炫320](https://me.csdn.net/qq_35082030)阅读数：152标签：[桶排序																[直方图](https://so.csdn.net/so/search/s.do?q=直方图&t=blog)](https://so.csdn.net/so/search/s.do?q=桶排序&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1.leetcode:49. Group Anagrams
## 1.1问题描述
> 
Given an array of strings, group anagrams together.
Example:
> 
Input: [“eat”, “tea”, “tan”, “ate”, “nat”, “bat”],
Output:
[
[“ate”,“eat”,“tea”],
[“nat”,“tan”],
[“bat”]
]
## 1.2思路
这道题主要意思是要把相同代码分布的词归为一组，如果我们知道桶排序的话，这就有点类似了，只不过我们要找到一个衡量角度，那就是字符串。经过排序后的字符串是一样的。
## 1.3代码
```java
public List<List<String>> groupAnagrams(String[] strs) {
       List<List<String>> result = new ArrayList<>();
		//对每一标识进行存储。
		HashMap<String, ArrayList<String>> hashMap=new HashMap<>();
		for (String string : strs) {
			char[] tag_string=string.toCharArray();
			Arrays.sort(tag_string);
			String tag=new String(tag_string);
			//如果没包含这个标识
			if(!hashMap.containsKey(tag)){
				hashMap.put(tag, new ArrayList<>());
			}
			hashMap.get(tag).add(string);
		}
		//输入结果。
		for (ArrayList<String> strings : hashMap.values()) {
			result.add(strings);
		}
		return result;
    }
```
# 2.leetcode:438.Find All Anagrams in a String
## 2.1问题描述
> 
Given a string s and a non-empty string p, find all the start indices of p’s anagrams in s.
Strings consists of lowercase English letters only and the length of both strings s and p will not be larger than 20,100.
The order of output does not matter.
Example 1:
> 
Input:
s: “cbaebabacd” p: “abc”
Output:
[0, 6]
Explanation:
The substring with start index = 0 is “cba”, which is an anagram of “abc”.
The substring with start index = 6 is “bac”, which is an anagram of “abc”.
## 2.2思路
其实我们完全可以参考第一个的思路来解决这个问题，也就是对每一个查询长度进行一个Sort然后进行比较。但是注意的是，这样做是肯定不行的，因为时间复杂度太高，尽管我给出代码。
## 2.3代码
```java
HashMap<String, List<Integer>> hashMap=new HashMap<>();
        char[] chars=s.toCharArray();
        char[] charp=p.toCharArray();
        Arrays.sort(charp);
        String pString=new String(charp);
        hashMap.put(pString, new ArrayList<>());
        for(int start=0,end=p.length()-1;end<s.length();start++,end++) {
        	char[] tempchars=Arrays.copyOfRange(chars, start, end+1);
        	Arrays.sort(tempchars);
        	String temps=new String(tempchars);
        	if(hashMap.containsKey(temps)) {
        		hashMap.get(temps).add(start);
        	}
        }
        return hashMap.get(pString);
```
## 2.4更进一步
这里我们可以看到，主要是在于，我们重复的对窗口内的字符进行排序了，那么我们如何保持目标的字母分布呢？在我们窗口的右侧，每前进一格，我们就要收录一格字符的分布，在窗口的左侧，我们每前进一格，就要放弃一格字符的分布，但是我们保持目标值与当前值得差值，这样可以省去每次的比较。
## 2.5代码
```java
List<Integer> list = new ArrayList<>();
		 if (s == null || s.length() == 0 || p == null || p.length() == 0) return list;
		 //表达出类似hash表的作用
		 int[] hash = new int[256]; 
		 //统计p内的字符
		 for (char c : p.toCharArray()) {
			 hash[c]++;
		 }
		 //开始和结束节点，count是P中字符的个数
		 int left = 0, right = 0, count = p.length();
		 while (right < s.length()) {
			 //如果它大于等于1，说明这个字符是目标字符
			 if (hash[s.charAt(right)] >= 1) {
				 count--;
			 }
			 hash[s.charAt(right)]--;
			 //右边移动
			 right++;
			 //如果确实都完全命中完毕
			 if (count == 0) {
				 //记录目前的值
				 list.add(left);
			 }
			 //如果目前遍历到达p的长度了
			 if (right - left == p.length() ) {
				 //如果left在hash表中，说明这个字符是要找的，否则的话，它是负数
				 if (hash[s.charAt(left)] >= 0) {
					 count++;
				 }     
				 hash[s.charAt(left)]++;
				 //左边的移动
				 left++;
			 }
		 }
		 return list;
```
## 2.6小结
这是另外一种思想了，除去桶排序，另一种思想是直方分布图，它可以解决的问题就是维持一个分布，省去了排序等操作。
# 3.总结
我们这次主要熟悉两种解法，一个是桶排序，依靠的是HashMap来实现。另一个是直方分布图的形式。
