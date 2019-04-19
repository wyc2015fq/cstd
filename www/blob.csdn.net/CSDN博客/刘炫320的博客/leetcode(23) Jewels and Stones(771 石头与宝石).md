# leetcode(23): Jewels and Stones(771 石头与宝石) - 刘炫320的博客 - CSDN博客
2019年04月07日 14:02:21[刘炫320](https://me.csdn.net/qq_35082030)阅读数：59
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
# 1. 问题描述
> 
You’re given strings J representing the types of stones that are jewels, and S representing the stones you have.  Each character in S is a type of stone you have.  You want to know how many of the stones you have are also jewels.
The letters in J are guaranteed distinct, and all characters in J and S are letters. Letters are case sensitive, so “a” is considered a different type of stone from “A”.
(给定字符串J 代表石头中宝石的类型，和字符串 S代表你拥有的石头。 S 中每个字符代表了一种你拥有的石头的类型，你想知道你拥有的石头中有多少是宝石。
J 中的字母不重复，J 和 S中的所有字符都是字母。字母区分大小写，因此"a"和"A"是不同类型的石头。)
# 2. 例子
Example 1:
> 
Input: J = “aA”, S = “aAAbbbb”
Output: 3
Example 2:
> 
Input: J = “z”, S = “ZZ”
Output: 0
# 3. 思路
这道题是比较简单的一道题，但是是新题。其思路都非常清楚，一种比较笨的思路是，根据每一个宝石的类型，从石头中遍历出宝石的个数即可。假设宝石有n种类型，石头有m种类型，则时间复杂度为O（n*m）。
# 4.代码
```java
class Solution {
    public int numJewelsInStones(String J, String S) {
        int count = 0;
        for(int i=0;i<J.length();i++){
            for(int j=0;j<S.length();j++){
                if(J.charAt(i)==S.charAt(j))
                    count++;
            }
        }
        return count;
    }
}
```
# 5. 更进一步
其实如果我们熟悉哈希表，那么使用哈希表可以使得时间复杂度为O（m+n）。思路如下：我们首先统计遍历一下已有的石头类型及数量，然后把是宝石的类型的数量相加即可获得最终结果。
# 6. 代码
```java
class Solution {
    public int numJewelsInStones(String J, String S) {
        HashMap<Character, Integer> hashMap=new HashMap<>(); 
        for(int i = 0; i < S.length(); i++) {  
            char c = S.charAt(i);  
            hashMap.put(c, hashMap.getOrDefault(c, 0)+1);
        }  
		Integer sum=0;
		for(int i = 0; i < J.length(); i++) {  
	        char c = J.charAt(i);  
	        sum+=hashMap.getOrDefault(c, 0);  
	    }  
		return sum;
    }
}
```
# 7. 小结
今天我们尝试做了一个简单的题目来热身。但是这题目还是有一定问题的，如果不是按照时间复杂度来做理论上的证明，在leetcode网站上，第一种方法要比第二种方法速度还快，这里可能是由于，在leetcode网站中，其字符串长度约定为小于等于50，这样两者之间的差距不会太大，另外由于第二种方法每次的计算增加了很多取值和加法运算，使得额外运算量增加（尽管只是O（1）的时间复杂度的增加），导致差不多的性能。但是如果字符串长度非常长，我相信第二种方法更具有优势。
事实上也确实如此，根据本地实验结果如下（其中1为第二种解法，2为第一种解法），其结果也告诉我们，第一种解法对于宝石和石头个数较为敏感，而第二种解法与宝石和石头个数的总和个数有关。因此，正确的分析时间复杂度，比单纯的看Leetcode上的时间复杂度更有效（为了减少服务器运行损耗，一般的测试样例都比较小）。
宝石为2个，随机五千个石头，运行结果如下：
> 
177
1程序运行时间：6ms
177
2程序运行时间：1ms
宝石为2个，随机五千万个石头，运行结果如下：
> 
1924624
1程序运行时间：799ms
1924624
2程序运行时间：76ms
宝石为26个，随机五千万个石头，运行时间结果如下（平均情况）：
> 
24996090
1程序运行时间：733ms
24996090
2程序运行时间：815ms
宝石类型为52个时，随机的五千万个石头，运行时间结果如下（最坏情况）
> 
50000000
1程序运行时间：768ms
50000000
2程序运行时间：1711ms
