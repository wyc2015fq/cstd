# Lintcode——157. 判断字符串是否没有重复字符 - westbrook1998的博客 - CSDN博客





2018年04月06日 23:12:31[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：221








> 
实现一个算法确定字符串中的字符是否均唯一出现


方法一： 

暴力双循环遍历，出现有相等的字符即返回false

```
public class Solution {
    /*
     * @param str: A string
     * @return: a boolean
     */
    public boolean isUnique(String str) {
        // write your code here
        for(int i=0;i<str.length()-1;i++){
            for(int j=i+1;j<str.length();j++){
                if(str.charAt(i)==str.charAt(j)){
                    return false;
                }
            }
        }
        return true;
    }
}
```

方法二： 

判断每一个字符的首索引和尾索引是否相同，不同即为有多个，直接返回false

```
public class Solution {
    /*
     * @param str: A string
     * @return: a boolean
     */
    public boolean isUnique(String str) {
        // write your code here
        for(int i=0;i<str.length();i++){
            char c=str.charAt(i);
            if(str.indexOf(c)!=str.lastIndexOf(c)){
                return false;
            }
        }
        return true;
    }
}
```



