# c++ 枚举与字符串 比较 - wishchinYang的专栏 - CSDN博客
2013年07月17日 14:34:58[wishchin](https://me.csdn.net/wishchin)阅读数：1379
# 读取字符串，然后将这个字符转换为对应的枚举。
# 如：从屏幕上输入'a'，则转换为set枚举中对应的a，源代码如下:
```cpp
//关键函数为char2enum(str,temp);
#include  
using namespace std;
enum set {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z};
void char2enum(char ch , set &em)
{
     // 实现字符转枚举，其中参数em为set的引用类型，来把ch转换的结果传出
     char start = 'a';
     em = (set)((int)ch - (int)start);
}
void main()
{    <pre name="code" class="cpp">   char str;
   set temp;
   cout >str; // 获取从屏幕中输入的字符
   char2enum(str, temp); // 将字符str转换为枚举temp
   switch(temp) // 根据枚举的值，进行输出 ，，关键语句！！！
    {
     case a:
     cout<<'a'<<endl;break;
     case b:
     cout<<'b'<<endl;break;
     case c:
     cout<<'c'<<endl;break;
     case d:
     cout<<'d'<<endl;break;
     case e:
     cout<<'e'<<endl;break;
     case f:
     cout<<'f'<<endl;break;
     // case g:...
     // case h:...
     }
}
```
# 测试结果： please input a char: a 回车 a // 为输出内容
