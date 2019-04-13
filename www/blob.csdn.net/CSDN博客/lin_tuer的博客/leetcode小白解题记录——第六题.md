
# leetcode小白解题记录——第六题 - lin_tuer的博客 - CSDN博客


2016年09月14日 20:29:50[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：283


6. ZigZag Conversion

The string`"PAYPALISHIRING"`is written in a zigzag pattern on a given number of rows like
 this: (you may want to display this pattern in a fixed font for better legibility)
P   A   H   N
A P L S I I G
Y   I   RAnd then read line by line:`"PAHNAPLSIIGYIR"`
`convert("PAYPALISHIRING",
 3)``should return``"PAHNAPLSIIGYIR"``.`

ZigZag：
![](https://img-blog.csdn.net/20160914203358539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个问题在leetcode里面是简单的，我们住需要把字符串拆成numRows行，存成三行的字符数组就可以了。

```java
public class Solution {
    public String convert(String s, int numRows) {
        
        //主要利用字符串的拼接 +
        int len=s.length();
        if(len==0 || len==1 || numRows==1) return s;
        String[] ans=new String[numRows];
        //给字符串赋值
        Arrays.fill(ans, "");
        int row=0;int a=1;
        for(int i=0;i<len;i++){
            ans[row]+=s.charAt(i);
            row+=a;
            if(row>=numRows){
                row=row-2;
                a=-1;
            }
            if(row<0){
                row=1;
                a=1;
            }
        }
        //将 ans 的每一行拼接起来
        String str=new String();
        for(int j=0;j<numRows;j++){
            str+=ans[j];
        }
        return str;
    }
}
```


