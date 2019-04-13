
# 剑指offer——二进制中1的个数 - manduner的博客 - CSDN博客


2019年03月09日 11:24:30[Manduner_TJU](https://me.csdn.net/manduner)阅读数：10


# 1，问题描述
输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
# 2，解题思路
（1）利用Integer.toBinaryString()方法先将整数（包含有符号整数）转换为二进制字符串str形式
（2）利用str.toCharArray()将（1）中的字符串转化为字符数组
（3）遍历字符数组，先将字符转化整型数字，然后再加和。
**此题较简单：就是将二进制中的所有数进行加和，返回加和之后的数就是1的个数。**
# 3，源码
```python
public class Solution {
    public int NumberOf1(int n) {
        if(n==0){
            return 0;
        }else{
            char[] ch = Integer.toBinaryString(n).toCharArray();
            int count = 0;
            for(char x : ch){
                int x_new = x - '0'; //字符转整型数字
                count += x_new;
            }
            return count;
        }
    }
}
```


