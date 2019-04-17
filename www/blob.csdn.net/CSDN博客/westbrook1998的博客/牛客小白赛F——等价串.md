# 牛客小白赛F——等价串 - westbrook1998的博客 - CSDN博客





2018年06月17日 11:17:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：129








> 
链接：[https://www.nowcoder.com/acm/contest/134/F](https://www.nowcoder.com/acm/contest/134/F)

  来源：牛客网 

  题目描述  

  一串长度为 n 的字符串 A 和一串长度为 m 的字符串 B。并且这两串字符串只会含有 0 或 1 。 

  铁子可以对字符串 A 执行两种操作，两种操作可以执行任意次。 

  操作1(无情替换):铁子可以用 11 替换掉 0 ，也可以用 00 替换掉 1 . 

  操作2(极限删除):铁子可以删除掉 111 ，也可以删除 000 . 

  现在问，字符串 A 可以变成字符串 B 吗？ 

  输入描述: 

  第一行有一个整数T，表示有T(1<=T<=1000)组测试数据。 

  接下来的每组数据，第一行有两个整数n，m(1<=n,m<=100)，表示字符串A和字符串B的长度。 

  接下来有两行字符串，分别表示字符串A和字符串B。 

  输出描述: 

  对于每组测试数据，如果字符串A可以变为字符串B，则输出一行”YES”,否则输出一行”NO”.输出不包括引号。 

  示例1 

  输入 

  复制 

  3 

  3 4 

  010 

  1110 

  3 4 

  010 

  1111 

  7 2 

  0001000 

  00 

  输出 

  复制 

  YES 

  NO 

  YES 

  说明 

  对于第一个样例，铁子可以对字符串A使用一次无情替换可以变成1110
安慰自己现在能勉强看懂小白赛题解了…  

这题仔细看其实发现这些对换都是对称的 那我们只需要把两个串都变换成0的串 又由于三个0可以消去 所以题目就是求出两个串变成0后0的个数模3的余数是否相等

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
char a[150];
char b[150];
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        int m,n;
        scanf("%d%d",&m,&n);
        scanf("%s",a);
        scanf("%s",b);
        int al=0;
        int bl=0;
        for(int i=0;i<m;i++){
            if(a[i]=='0'){
                al++;
            }
            else{
                al+=2;
            }
            if(al>=3){
                al-=3;
            }
        }
        for(int i=0;i<n;i++){
            if(b[i]=='0'){
                bl++;
            }
            else{
                bl+=2;
            }
            if(bl>=3){
                bl-=3;
            }
        }
        printf("%d %d\n",al,bl);
        if(al==bl){
            printf("YES\n");
        }
        else{
            printf("NO\n");
        }
    }
    return 0;
}
```





