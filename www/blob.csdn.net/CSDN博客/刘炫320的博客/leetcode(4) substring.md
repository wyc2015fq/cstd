# leetcode(4):substring - 刘炫320的博客 - CSDN博客
2018年03月06日 20:48:05[刘炫320](https://me.csdn.net/qq_35082030)阅读数：215
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
今天我们来讲字符子串的问题。
# 1.leetcode#3. Longest Substring Without Repeating Characters
## 1.1问题描述
> 
Given a string, find the length of the longest substring without repeating characters.
Examples:
> 
Given “abcabcbb”, the answer is “abc”, which the length is 3. 
  Given “bbbbb”, the answer is “b”, with the length of 1. 
  Given “pwwkew”, the answer is “wke”, with the length of 3. Note that the answer must be a substring, “pwke” is a subsequence and not a substring.
## 1.2.思路
我们首先就应该想到这么一个思路，那就是找一个容器存储最近见过的字母的位置，然后如果我们又重新见到这个词，就观察一下上一个重复的字母和当前见过字母的上一位置谁离当前位置最近，然后把这个距离与全局最长的距离比较，获取最终的全局最长的字符串。另外一个就是再更新下刚刚见过这个词的最近的一次位置。
## 1.3代码
```java
public int lengthOfLongestSubstring(String s) {
            //一个存储曾经见过的字母的容器
         if (s.length()==0) return 0;
            HashMap<Character, Integer> map = new HashMap<Character, Integer>();
            int max=0;
            for (int i=0, j=0; i<s.length(); ++i){
                //如果包含这个字母
                if (map.containsKey(s.charAt(i))){
                    //当前已观察的最长位置，也就是距离当前节点最近的不重复的终止位置
                    j = Math.max(j,map.get(s.charAt(i))+1);
                }
                //放进去当前值与位置
                map.put(s.charAt(i),i);
                //最大位置为全局max和当前的max；
                max = Math.max(max,i-j+1);
            }
            return max;
        }
```
## 1.4更进一步
还有更快的算法，那就是每走一步，就去判断候选字符串中，是否存在重复的子串，如果出现重复的子串了，就要把候选字符串截掉这一部分，然后再记录当前最长的不重复子串的位置。如果不包含的话，就把当前最长的长度++,如果当前最长长度比全局长度还大，那么就替换最长的长度。
## 1.5代码
```java
public int lengthOfLongestSubstring(String s) {
         char[] chars = s.toCharArray();
            if(2 > chars.length){
                return chars.length;
            }
            int max = 0;
            int split_at = 0;
            int cur_len = 1;
            for(int i=1;i<chars.length;i++){
                //从分割点开始查找
                int j = split_at;
                //目的是找到最长的不重复的串的截止位置
                for(;j<i;j++){
                    if(chars[i] == chars[j]){
                        break;
                    }
                }
                //如果j小于i的话，说明当前包含重复的字了
                if(j < i){
                    //重新确定分割点在目前点的下一个。
                    split_at = j+1;
                    //当前重复的长度为i-j
                    cur_len = i-j;
                }
                //如果j=i的话，说明当前没有重复的字母，就要把当前长度++；
                else{
                    cur_len++;
                }
                //如果当前长度大于最大值，则替换。
                if(cur_len > max) max = cur_len;
            }
            return max;
     }
```
# 2.小结
本次相关的题目不多，还有一道相关题目是付费的，因此没有摘录。在本次学习中，我们主要学会了这么一个思路，那就是当在一个序列中，查找一个最值的问题的时候，我们至少需要遍历一遍，然后才能找到最值。另外一点，我们还需要一个记录在遍历过程中的次值。而后面的优化方向就是尽可能的让无用的搜索减少，例如此题中更快的解法就是不再额外存储一次该字符串，直接使用索引位置来抛弃无用的数据。
