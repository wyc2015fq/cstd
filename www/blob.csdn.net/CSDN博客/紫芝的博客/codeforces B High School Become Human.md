# codeforces B. High School: Become Human - 紫芝的博客 - CSDN博客





2018年06月02日 22:22:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：67









Year 2118. Androids are in mass production for decades now, and they do all the work for humans. But androids have to go to school to be able to solve creative tasks. Just like humans before.

It turns out that high school struggles are not gone. If someone is not like others, he is bullied. Vasya-8800 is an economy-class android which is produced by a little-known company. His design is not perfect, his characteristics also could be better. So he is bullied by other androids.

One of the popular pranks on Vasya is to force him to compare xy

 with yx
. Other androids can do it in milliseconds while Vasya's memory is too small to store such big numbers.

Please help Vasya! Write a fast program to compare xy
 with yx

 for Vasya, maybe then other androids will respect him.



Input

On the only line of input there are two integers x

 and y (1≤x,y≤109

).



Output

If xy<yx

, then print '<' (without quotes). If xy>yx, then print '>' (without quotes). If xy=yx

, then print '=' (without quotes).



Examples



input
Copy

5 8



output
Copy

>



input
Copy

10 3



output
Copy

<



input
Copy

6 6



output
Copy

=





Note

In the first example 58=5⋅5⋅5⋅5⋅5⋅5⋅5⋅5=390625

, and 85=8⋅8⋅8⋅8⋅8=32768
. So you should print '>'.

In the second example 103=1000<310=59049

.

In the third example 66=46656=66

.

题意：

输入两个数，x,y，如果x^y>y^x就输出>，小于就输出<，等于就输出=。

思路：

由数据范围，肯定不能乘算，所以可以变换一下   两边取对数 log（x^y） 与 log（y^x）比较


 即：y*log（x） 与 x*log（y）比较 





```cpp
#include <bits/stdc++.h>
using namespace std;
#define eps 1e-6
int main()
{
    int x,y;
    double xx;
    scanf("%d %d",&x,&y);
    xx = y*log10(x) - x*log10(y);
    if(fabs(xx) <= eps)
        printf("=\n");
    else
    {
        if(xx < eps)
            printf("<\n");
        else
            printf(">\n");
    }
    return 0;
}
```


大佬的AC代码


```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int x,y;
    cin>>x>>y;
    if(log(x)*y>(log(y)*x))puts(">");
    else if(log(y)*x>(log(x)*y)) puts("<");
    else puts("=");
}
```

错误

若x=2,y=4，则2^4=4^2


```cpp
#include <bits/stdc++.h>
using namespace std;
#define eps 1e-6
int main()
{
    int  x,y;
    double xx;
    scanf("%d %d",&x,&y);
     if(x==y)
        printf("=\n");
    else
    {
        xx = y*log10(x) - x*log10(y);
        if(xx < eps)
            printf("<\n");
        else
            printf(">\n");
    }
    return 0;
}
```




