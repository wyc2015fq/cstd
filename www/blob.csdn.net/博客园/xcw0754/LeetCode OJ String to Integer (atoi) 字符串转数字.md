# LeetCode OJ String to Integer (atoi)  字符串转数字 - xcw0754 - 博客园
# [LeetCode OJ String to Integer (atoi)  字符串转数字](https://www.cnblogs.com/xcw0754/p/4072333.html)
```
1 #include <iostream>
 2 #include <assert.h>
 3 using namespace std;
 4 int ato(const char *str) {
 5     int i=0,e=0,s=0;
 6     int max=2147483647,min=-2147483648;
 7     int f=1;
 8     int tem[10]={0};
 9     unsigned int pan=0;
10     while(*str==' '){    //过滤掉连续空格
11         str++;
12         }
13     if(*str=='-'||*str=='+'||(*str<='9'&&*str>='0')){
14         if(*str=='-'||*str=='+'){    //过滤掉正负号
15             if(*str=='-')
16                 f=-1;
17             str++;
18         }
19         while(*str=='0')    //过滤掉前面的无用的0
20             str++;
21         if( *str<='9'&&*str>='0' ){        //过滤掉非数字的字符
22 
23             while( *str!='\0'){            //判断字符串是否结束
24                 if( *str>'9'||*str<'0' )    //字符串还没结束，就出现非数字字符
25                     break;        
26                 if( i==10 ){            //大于10位的数字都要作溢出处理
27                     if(f==-1)
28                         return min;
29                     else
30                         return max;
31                 }
32                 tem[i++]=*str-'0';
33                 e=e*10+(*str-'0');
34                 str++;
35             }
36             if(i==10){            //刚好10位,需要特殊处理
37                 if(f==-1){    //负数处理
38                     if(tem[0]>2){    //超过30,0000,0000
39                         return min;
40                     }
41                     for(i=0;i<10;i++){
42                         pan=pan*10+tem[i];
43                     }
44                     if(pan>=2147483648)  //  负数的绝对值大于等于2147483648
45                         return min;
46                 }
47                 else{    //正数处理
48                     if(tem[0]>2){
49                         return max;
50                     }
51                     for(i=0;i<10;i++){
52                         pan=pan*10+tem[i];
53                     }
54                     if(pan>=2147483647)  //  负数的绝对值大于等于2147483648
55                         return max;
56                 }
57             }
58             return f*e;
59         }
60         else
61             return 0;
62     }
63     return 0;
64 }
65 int main(){
66     char str[1000];
67     int qq=0;
68     while(cin>>str){
69         qq=ato(str);
70         cout<<qq<<endl;
71     }
72     return 0;
73 }
```
这个东西原来挺难考虑周全的，还好leetcode会指出是输入什么数据会输出错误，不然估计找一辈子了。
鉴于这么苛刻的条件，贴出自己的不简洁的代码，这道题第一次AC。
以后有好办法再修改。

