# codeforces 3-A. Shortest path of the king（暴力） - HJ - CSDN博客
2017年08月31日 13:22:19[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：191
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/3/A](http://codeforces.com/problemset/problem/3/A)
八个方向八种情况考虑清楚即可。
```cpp
#include<iostream>
#include<cmath> 
using namespace std;
int main()
{
    char a[10],b[10];
    while(cin>>a>>b)
    {
        int x1=a[0]-'a'+1,y1=a[1]-'0',x2=b[0]-'a'+1,y2=b[1]-'0';
        int ans=max(fabs(x1-x2),fabs(y1-y2));
        cout<<ans<<endl;
        while(ans--)
        {
            if(x1!=x2)
            {
                if(x1>x2)
                {
                    if(y1==y2)      //正左方 
                    {
                        x1--;
                        cout<<"L"<<endl;
                    }
                    else if(y1>y2)     //左下 
                    {
                        y1--;
                        x1--;
                        cout<<"LD"<<endl; 
                    }
                    else             //左上 
                    {
                        y2--;
                        x1--;
                        cout<<"LU"<<endl;
                    }
                }
                else
                {
                    if(y1==y2)          //正右方 
                    {
                        x2--;
                        cout<<"R"<<endl;
                    }
                    else if(y1>y2)
                    {
                        y1--;
                        x2--;
                        cout<<"RD"<<endl;
                    }
                    else
                    {
                        y2--;
                        x2--;
                        cout<<"RU"<<endl;
                    }
                }
            }
            else
            {
                if(y1>y2)         //下降 
                {
                    y1--;
                    cout<<"D"<<endl;
                }
                else                      //上升 
                {
                    y2--;
                    cout<<"U"<<endl;
                }
            }
        }
    }
    return 0;
}
```
