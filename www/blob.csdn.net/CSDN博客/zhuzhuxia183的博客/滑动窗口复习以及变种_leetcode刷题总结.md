# 滑动窗口复习以及变种_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月15日 18:15:38[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：39
个人分类：[滑动窗口																[数据结构和算法](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)](https://blog.csdn.net/zhuzhuxia183/article/category/8614188)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








## 额，其实我一开始也不知道滑动窗口的，哈哈哈哈哈
- 1、无重复字符的最长子串  [https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/](https://leetcode-cn.com/problems/longest-substring-without-repeating-characters/)

给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。

示例 1:

输入: “abcabcbb”

输出: 3

解释: 因为无重复字符的最长子串是 “abc”，所以其长度为 3。

示例 2:

输入: “bbbbb”

输出: 1

解释: 因为无重复字符的最长子串是 “b”，所以其长度为 1。

示例 3:

输入: “pwwkew”

输出: 3

解释: 因为无重复字符的最长子串是 “wke”，所以其长度为 3。

请注意，你的答案必须是 子串 的长度，“pwke” 是一个子序列，不是子串。
```
public int lengthOfLongestSubstring(String s) {
        int max_length = 0;
        int i=0,j=0;
        HashMap<Character,Integer> map = new HashMap<>();
        while (j<s.length()){
            if (map.containsKey(s.charAt(j))){
                i = Math.max(i,map.get(s.charAt(j)));
            }
            map.put(s.charAt(j),j+1);
            max_length = Math.max(max_length,j-i+1);
            j++;
        }
        return max_length;
    }
```

这里是通过i,j来表示我们的窗口为 i~j的位置为我们的窗口,emmm，值得注意的是我们表达的是左闭右闭的区间哈

当然这里我们可以直接存储一个128长度的数组可以的。

```
public int lengthOfLongestSubstring1(String s) {
		int[] index =new int[128];
        int length = s.length();
        int first_index = 0;
        int max_length = 0;
        for (int i = 0; i < length; i++) {
            first_index = Math.max(index[s.charAt(i)],first_index);
            index[s.charAt(i)]  = i+1;
            max_length = Math.max(max_length,i-first_index+1);
        }
        return max_length;
   }
```
- 2、与所有单词相关联的字串  [https://leetcode-cn.com/problems/substring-with-concatenation-of-all-words/](https://leetcode-cn.com/problems/substring-with-concatenation-of-all-words/)

给定一个字符串 s 和一些长度相同的单词 words。找出 s 中恰好可以由 words 中所有单词串联形成的子串的起始位置。

注意子串要与 words 中的单词完全匹配，中间不能有其他字符，但不需要考虑 words 中单词串联的顺序。

示例 1：

输入：

s = “barfoothefoobarman”,

words = [“foo”,“bar”]

输出：[0,9]

解释：

从索引 0 和 9 开始的子串分别是 “barfoor” 和 “foobar” 。

输出的顺序不重要, [9,0] 也是有效答案。

示例 2：

输入：

s = “wordgoodgoodgoodbestword”,

words = [“word”,“good”,“best”,“word”]

输出：[]
```
public List<Integer> findSubstring(String s, String[] words) {
        List<Integer> result = new ArrayList<>();
        if (s.isEmpty()||words.length== 0 ) return result;
        int size = words.length;
        int word_length = words[0].length();
        HashMap<String,Integer> map = new HashMap<>();
        for (String str:words) {
            if (map.containsKey(str)){
                map.put(str,map.get(str)+1);
            }else {
                map.put(str,1);
            }
        }
        for (int i = 0; i <= Math.min(word_length-1,s.length()-word_length*size); i++) {
            findSub(s,i,word_length,size,map,result);
        }
        return result;
    }

    private void findSub(String str, int str_start, int word_length, int size, HashMap<String, Integer> map, List<Integer> result) {
        HashMap<String,Integer> new_map = new HashMap<>();

        for (String str_word:map.keySet()) {
            new_map.put(str_word,0);
        }
        int index = 0;
        for (int i = str_start; i <=str.length()-word_length; i+=word_length) {//进行一次滑动窗口的查找
                String word = str.substring(i,i+word_length);
                if (!map.containsKey(word)){
                    str_start = i+word_length;
                    index = 0;
                    if(str_start>str.length()-word_length)
                        return;
                    for (String str_word:map.keySet()) {
                        new_map.put(str_word,0);
                    }
                }else{
                    while (new_map.get(word) == map.get(word)){
                        String str_start_word = str.substring(str_start,str_start+word_length);
                        new_map.put(str_start_word,new_map.get(str_start_word)-1);
                        str_start += word_length;
                        index--;
                    }
                    new_map.put(word,new_map.get(word)+1);
                    index++;
                    if (index==size){
                        result.add(str_start);
                        String str_start_word = str.substring(str_start,str_start+word_length);
                        new_map.put(str_start_word,new_map.get(str_start_word)-1);
                        str_start += word_length;
                        if(str_start>str.length()-word_length)
                            return;
                        index--;
                    }
                }
        }
        return;
    }
```

这个题目和上面的题目比较像，只有两个差别：

1、上面的窗口单位长度是1，下面的窗口单位长度可能不是1

2、下面的出现不是只有1，可能超过1

3、这里是字符串，不是字符，所以只能用hashMap结构
所以我们这里用map结构的话，不能存储位置，只能够存储次数，然后通过如果超过了，不断的往前走进行刷新进行尝试。







