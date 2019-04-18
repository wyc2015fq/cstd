# LeetCode  Add Binary 两个二进制数相加 - xcw0754 - 博客园
# [LeetCode  Add Binary 两个二进制数相加](https://www.cnblogs.com/xcw0754/p/4125037.html)
```
1 class Solution {
 2 public:
 3     string addBinary(string a, string b) {
 4         if(a==""&&b=="")    return "";
 5         if(a=="")    return b;
 6         if(b=="")    return a;
 7         char *pa=&a[0],*pb=&b[0];
 8         int na=0,nb=0;
 9         int i=0;
10         int t=0,m=0;
11         while(*pa!='\0'){
12             pa++;
13             na++;
14         }
15         pa--;
16         while(*pb!='\0'){
17             pb++;
18             nb++;
19         }
20         pb--;
21         if(na>=nb){        //存a中
22             for(i=na-1;i>-1;i--){
23                 if(i>na-nb-1){
24                     m=(*pa-'0') + (*pb-'0')+t;
25                     pa--;
26                     pb--;
27                 }
28                 else if( i<na-nb &&t ==0 )
29                         return a;
30                 else{
31                     m=(*pa-'0')+t;
32                     pa--;
33                 }
34                 t=m/2;        //进位
35                 m=m%2;        //当前位
36                 a[i]=m+'0';
37             }
38             if(t>0)
39                 return ("1"+a);
40             else
41                 return a;
42         }
43         else{            //存b中
44             for(i=nb-1;i>-1;i--){
45                 if(i>nb-na-1){
46                     m=(*pb-'0') + (*pa-'0')+t;
47                     pa--;
48                     pb--;
49                 }
50                 else if( i<nb-na &&t ==0 )
51                         return b;
52                 else{
53                     m=(*pb-'0')+t;
54                     pb--;
55                 }
56                 t=m/2;        //进位
57                 m=m%2;        //当前位
58                 b[i]=m+'0';
59             }
60             if(t>0)
61                 return ("1"+b);
62             else
63                 return b;
64         }
65     }
66 };
```
题意：给出两个字符串，里面分别存有一个二进制数，将二进制相加后返回结果，依然是字符串型。
思路：两个指针，从后面开始将两个二进制数相加，如果有一个比较长，那么另一个指针肯定先扫完一个字符串，此时将结果保存在较长的那个字符串中返回。在一个字符串已扫完的情况下，另一个字符串的未扫完部分扔要判断是否需要继续加，这取决于进位是否为0，若为0，连后面未扫部分都不用扫了，直接返回这个扔有未扫的字符串。若进位不为0，那么就要一个个继续扫下去了，只要满足一个条件就能不用继续往下扫，此条件是进位为0。若未扫完的那个字符串扫到完了，需再考虑一次进位。
吐槽：代码是草稿代码，但是AC了，从比较容易理解的角度写的。

