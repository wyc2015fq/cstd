# KMP模板 - Vmetrio - 博客园







# [KMP模板](https://www.cnblogs.com/wangmengmeng/p/5487187.html)





学习链接:

http://blog.csdn.net/v_july_v/article/details/7041827



代码:

```
1 //优化过后的next 数组求法
 2 void GetNextval(char* p, int next[])
 3 {
 4     int pLen = strlen(p);
 5     next[0] = -1;
 6     int k = -1;
 7     int j = 0;
 8     while (j < pLen - 1)
 9     {
10         //p[k]表示前缀，p[j]表示后缀  
11         if (k == -1 || p[j] == p[k])
12         {
13             ++j;
14             ++k;
15             //较之前next数组求法，改动在下面4行
16             if (p[j] != p[k])
17                 next[j] = k;   //之前只有这一行
18             else
19                 //因为不能出现p[j] = p[ next[j ]]，所以当出现时需要继续递归，k = next[k] = next[next[k]]
20                 next[j] = next[k];
21         }
22         else
23         {
24             k = next[k];
25         }
26     }
27 }
28 
29 int KmpSearch(char* s, char* p)
30 {
31     int i = 0;
32     int j = 0;
33     int sLen = strlen(s);
34     int pLen = strlen(p);
35     while (i < sLen && j < pLen)
36     {
37         //①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++    
38         if (j == -1 || s[i] == p[j])
39         {
40             i++;
41             j++;
42         }
43         else
44         {
45             //②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]    
46             //next[j]即为j所对应的next值      
47             j = next[j];
48         }
49     }
50     if (j == pLen)
51         return i - j;
52     else
53         return -1;
54 }
```














