
# 剑指offer——替换空格 - manduner的博客 - CSDN博客


2018年12月19日 10:35:58[Manduner_TJU](https://me.csdn.net/manduner)阅读数：25


# 1，问题描述
请实现一个函数，将一个字符串中的每个空格替换成“%20”。例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。
# 2，解题思路
（1）将StringBuffer对象转换成字符串
（2）正则表达式实现替换
# 3，源码
```python
public class Solution {
    public String replaceSpace(StringBuffer str) {
    	String str_tmp = str.toString();
        return str_tmp.replaceAll(" ","\\%20");
    }
}
```


