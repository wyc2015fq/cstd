# leetcode 13 roman to integer - zqjackking的博客 - CSDN博客





2017年02月22日 21:56:27[zqjackking](https://me.csdn.net/zqjackking)阅读数：152标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[深度学习-caffe](https://blog.csdn.net/zqjackking/article/category/6556862)









感觉13roman to integer比12integer to roman 要难一些。但是前者的难度是easy，后者难度是medium. 

首先看下 integer to roman，

```
string roman[][10]  = {  
            {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},  
            {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},  
            {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},  
            {"", "M", "MM", "MMM"}  
        }; 
int digit=0,temp=0;
string r;
while(num!=0)
{
       temp=num%10;
       r=roman[digit][temp]+r;
       num/=10;
       digit++;
}
return r;
```

就是给个二维数组，自己去数组里面找就OK了。

roman to integer的话。要考虑后面的数字比前面的大这种情况（比如IX,CD）。自己一开始写的代码是：

```cpp
map<char,int> a={ {'I',1},
                        {'V',5},
                        {'X',10},
                        {'L',50},
                        {'C',100},
                        {'D',500},
                        {'M',1000} };
 int len=s.size();
 int sum=0;
 int b[len];
 int i;
 for(i=len-1;i>0;)
 {
     if(a[s[i]]<=a[s[i-1]])
     {
         b[i]=1; 
         i--;
     }
     else
     {
         b[i]=1,b[i-1]=-1;
         i-=2;
     }
 }
 if(i==0)
    b[i]=1;
 for(i=0;i<len;i++)
 {
     sum+=a[s[i]]*b[i];
 }
 return sum;
```

再看看大神的代码：

```cpp
unordered_map<char, int> T = { { 'I' , 1 },
                                   { 'V' , 5 },
                                   { 'X' , 10 },
                                   { 'L' , 50 },
                                   { 'C' , 100 },
                                   { 'D' , 500 },
                                   { 'M' , 1000 } };

int sum = T[s.back()];
for (int i = s.length() - 2; i >= 0; --i) 
{
   if (T[s[i]] < T[s[i + 1]])
   {
       sum -= T[s[i]];
   }
   else
   {
       sum += T[s[i]];
   }
}

return sum;
```

感到惭愧啊。其实思路是一样的，就是实现起来自己拐了很多弯。



