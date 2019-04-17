# Lintcode8——旋转字符串 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:19:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：76








## 题目：

> 
给定一个字符串和一个偏移量，根据偏移量旋转字符串(从左向右旋转) 

  样例 

  对于字符串 “abcdefg”. 

  offset=0 => “abcdefg” 

  offset=1 => “gabcdef” 

  offset=2 => “fgabcde” 

  offset=3 => “efgabcd”  
要求在数组上原地旋转，所以原理是先把要旋转的一个字符保存起来，然后前面的字符一个一个往后递推，再在前面插入旋转字符  

## 题解：

```
public class Solution {
    /*
     * @param str: An array of char
     * @param offset: An integer
     * @return: nothing
     */
    public void rotateString(char[] str, int offset) {
        // write your code here
        if(str==null || str.length==0)   //考虑空字符串的情况
            return ;
        int len=str.length;
         if(offset>=len){   //考虑旋转偏移量大于字符串长度的情况
            offset%=len;
        }
        int c=0;
        for(int i=len-offset;i<len;i++){     //外循环的i递增
            char t=str[i]; 
            for(int j=i;j>c;j--){        //所以内循环要有j>c的条件而不是j>0
                str[j]=str[j-1];
            }
            str[c++]=t;
        }
    }
}
```

## 网上参考代码：

```
//思路：  
将字符串分为两部分，分别旋转 ，再整个字符串的旋转  
public class Solution {
    /**
     * @param str: an array of char
     * @param offset: an integer
     * @return: nothing
     */
    public void rotateString(char[] str, int offset) {
        // write your code here
        if (str == null || str.length == 0)
            return;

        offset = offset % str.length;
        reverse(str, 0, str.length - offset - 1);
        reverse(str, str.length - offset, str.length - 1);
        reverse(str, 0, str.length - 1);
    }

    private void reverse(char[] str, int start, int end) {             //注意这个字符串旋转的写法！！
        for (int i = start, j = end; i < j; i++, j--) {
            char temp = str[i];
            str[i] = str[j];
            str[j] = temp;
        }
    }
}
```





