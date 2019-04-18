# LeetCode  ZigZag Conversion（将字符串排成z字型） - xcw0754 - 博客园
# [LeetCode  ZigZag Conversion（将字符串排成z字型）](https://www.cnblogs.com/xcw0754/p/4200117.html)
```
1 class Solution {
 2 public:
 3     string convert(string s, int nRows) {
 4        string a="";
 5        int len=s.length();
 6        if(len<=nRows||nRows==1)    return s;    //只有n个，不足一个周期||排成一行
 7        int teams=len/(nRows*2-2);    //teams个完整的周期
 8        int rest=len%(nRows*2-2);    //最后一个不足完整周期的元素数
 9        int k=0,i=0,j=0;
10        for(i=0;i<nRows;i++){
11            k=i;
12            for(j=0;j<teams;j++){
13                a+=s[k];
14                if(i!=0 && i!=nRows-1){    //每个周期内有两个
15                    a+=s[(nRows*2-2)*(j+1)-i];
16                }
17                k+=nRows*2-2;
18            }
19            if(i<rest)
20                a+=s[k];
21            if(rest>nRows && rest>(nRows*2-2-i) && (nRows*2-2-i)>=nRows)
22                a+=s[(nRows*2-2)*(j+1)-i];
23        }    
24        return a;
25     }
26 };
ZigZag Conversion
```
题意：将一串字母按Z字（类似而已，具体看leetcode题目那样）排列，再按行串到一起返回。
思路：按周期，每n-2个为一个周期，最后可能不足一个周期，要特殊处理。因为一个周期内的Z字是有折叠的，所以一个周期内有的需要两个加起来。
吐槽：这个方法不精简，但是运行时间96ms。上网艘了一下别人的精简代码，确实很漂亮，但是时间达到168ms。那我还是暂时用这个好了。数据分析说有的居然50几ms就搞定，求代码喔~

