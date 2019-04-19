# leetcode(13):38. Count and Say - 刘炫320的博客 - CSDN博客
2018年04月08日 12:42:07[刘炫320](https://me.csdn.net/qq_35082030)阅读数：93
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1.38. Count and Say
## 1.1问题描述
> 
The count-and-say sequence is the sequence of integers with the first five terms as following: 
  数数并说序列是一个整数序列，第二项起每一项的值为对前一项的计数，其前五项如下： 
  1.     1 
  2.     11 
  3.     21 
  4.     1211 
  5.     111221 
  1 is read off as “one 1” or 11. 
  11 is read off as “two 1s” or 21. 
  21 is read off as “one 2, then one 1” or 1211. 
  Given an integer n, generate the nth term of the count-and-say sequence. 
  给一个正整数 n ，输出数数并说序列的第 n 项。 
  Note: Each term of the sequence of integers will be represented as a string. 
  注意：该整数序列的每项都输出为字符串。 
  Example 1: 
  Input: 1 
  Output: “1” 
  Example 2: 
  Input: 4 
  Output: “1211”
## 1.2思路
这道题可以说非常具有西方色彩了，我们中国人并不能理解这道题目，但是西方确实是oneonetwo的形式来解释。这道题也是采用模拟法即可。关键是在处理字符串上，也就是统计一个数，然后统计连续出现的次数。
## 1.3代码
```java
public static String countAndSay(int n) {
        return countAndSay("1", 1, n);
    }
    public static String countAndSay(String n,int count,int target){
        //如果到了我们要求的数目就退出。
        if(count==target){
            return n;
        }
        else{
            char[] chars=n.toCharArray();
            //使用StringBuffer要快上N倍
            StringBuffer sbBuffer=new StringBuffer();
            char nums=chars[0];
            int counts=1;
            //统计字母出现的次数
            for(int i=1;i<chars.length;i++){
                if(chars[i]==nums){
                    counts++;
                }
                else{
                    sbBuffer.append(counts);
                    sbBuffer.append(nums);
                    nums=chars[i];
                    counts=1;
                }
            }
            //不要忘记了最后的一个
            sbBuffer.append(counts);
            sbBuffer.append(nums);
            //递归实现
            return countAndSay(sbBuffer.toString(), count+1, target);
        }
    }
```
## 1.4小结
这里我们使用递归来实现的，其实也是可以使用循环来实现的。
