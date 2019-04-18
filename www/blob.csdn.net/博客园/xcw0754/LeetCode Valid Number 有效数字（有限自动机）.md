# LeetCode Valid Number 有效数字（有限自动机） - xcw0754 - 博客园
# [LeetCode Valid Number 有效数字（有限自动机）](https://www.cnblogs.com/xcw0754/p/4692322.html)
题意：判断一个字符串是否是一个合法的数字，包括正负浮点数和整形。
思路：有限自动机可以做，画个图再写程序就可以解决啦，只是实现起来代码的长短而已。
　　下面取巧来解决，分情况讨论：
（1）整数
（2）浮点数
（3）整数e整数
（4）浮点数e整数
　　只有以上4种情况。但是要数之前可能带1个符号，这个可以直接过滤1个，而不影响结果。而且，其包含关系是从上到下扩展的，（1）扩展到（2），（3）扩展到 （4）。
　　那么先解决符号 e 之前的，必须满足：正负号至多1个，有数字1个以上，点至多1个。
　　如果没有e，那么可以结束了。若有e，跳过1个e，再继续。
　　解决e后面的，必须满足：正负号至多1个，有数字1个以上，没有点。
```
1 class Solution {
 2 public:
 3     bool isNumber(string s) {
 4         int i=0, d=0;
 5         while( !s.empty() && s[s.size()-1]==' '  )       //直接删后缀空格
 6             s.erase(s.end()-1);
 7 
 8         while(i<s.size() && s[i]==' ')        //忽略前缀空格
 9             i++;
10         if( s[i]=='+' || s[i]=='-' )  i++;      //这个符号出现了等于没出现
11 
12         while(i<s.size() && isdigit(s[i]))
13             i++,d++;
14         if(i<s.size()&&s[i]=='.')   i++;
15         while(i<s.size() && isdigit(s[i]))
16             i++,d++;
17         if(!d)  return false;   //保证e之前不为空，且合法
18 
19         //以上是e前的情况
20         if(i==s.size()) return true;
21         else if(s[i]!='e')  return false; //有e出现则e后不能空。无e则出错
22 
23         //下面e后的情况
24         if(s[++i]=='+'||s[i]=='-')    i++;    //可以有符号
25 
26         d=0;
27         while(i<s.size()&& isdigit(s[i]))       //只能出现整数了
28             i++,d++;
29         if(d && i==s.size()) return true;
30         else    return false;
31     }
32 };
AC代码
```

