# n*n阶矩阵逆时针输出 - z69183787的专栏 - CSDN博客
2015年08月24日 17:53:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2839
问题描述：n*n阶矩阵逆时针输出。
即：
输入：5
输出：
01 16 15 14 13
02 17 24 23 12
03 18 25 22 11
04 19 20 21 10
05 06 07 08 09
思路： 经分析，可以发现此题输出格式犹如转圈圈。当n为偶数的时候刚好转了n/2圈；当n为奇数的时候，同样也是转了n/2圈，因为n/2取整。又根据转圈特色，可以把转圈分为向下、向右、向上、向左分别去实现。这时注意n为奇数时有个正中心点。
**[cpp]**[view
 plain](http://blog.csdn.net/pet8766/article/details/7495989#)[copy](http://blog.csdn.net/pet8766/article/details/7495989#)
- /*c++实现逆时针旋转的矩阵*/
- #include<iostream>
- #include<iomanip>
- usingnamespace std;  
- #define N 1000
- int a[N][N];  
- void fun(int n)  
- {  
- int m=1,i,j;  
- for(i=0;i<n;i++)//初始化矩阵元素为0
-     {  
- for(j=0;j<n;j++)  
-             a[i][j] = 0;  
-     }  
- for(i=0;i<n/2;i++)//只旋转n/2次
-     {  
- for(j=i;j<n-i;j++)//向下                       {
- if(a[j][i]==0)  
-                 a[j][i] = m++;  
-         }  
- for(j=i+1;j<n-i;j++)//向右
-         {  
- if(a[n-i-1][j]==0)  
-                 a[n-i-1][j]= m++;  
-         }  
- for(j=n-i-2;j>i;j--)//向上
-         {  
- if(a[j][n-i-1]==0)  
-                 a[j][n-i-1]=m++;  
-         }  
- for(j=n-i-1;j>i;j--)//向左
-         {  
- if(a[i][j]==0)  
-                 a[i][j]=m++;  
-         }  
-     }  
- if(n%2==1)//注意！当n为奇数时，有个正中心点
-         a[n/2][n/2] = m;  
- for(i=0;i<n;i++)//格式化输出
-     {  
- for(j=0;j<n;j++)  
-         {  
-             cout<<setw(2)<<setfill('0')<<a[i][j]<<"\t";  
-         }  
-         cout<<endl;  
-     }  
- }  
- int main()  
- {  
- int n;  
-     cout<<"请输入 n 的值：";  
-     cin>>n;   
-     fun(n);  
- return 0;  
- }   
那么如果顺时针输出的话也同样很容易就可以解决了。
**[cpp]**[view
 plain](http://blog.csdn.net/pet8766/article/details/7495989#)[copy](http://blog.csdn.net/pet8766/article/details/7495989#)
- /*C++实现顺时针旋转的矩阵*/
- #include<iostream>
- #include<iomanip>
- usingnamespace std;  
- #define N 100
- int a[N][N];  
- void fun(int n)  
- {  
- int m=1,i,j;  
- for(i=0;i<n;i++)//初始化矩阵元素为0
-     {  
- for(j=0;j<n;j++)  
-             a[i][j] = 0;  
-     }  
- for(i=0;i<n/2;i++)//只旋转n/2次
-     {  
- for(j=i;j<n-i;j++)//向右
-         {  
- if(a[i][j]==0)  
-                 a[i][j] = m++;  
-         }  
- for(j=i;j<n-i;j++)//向下
-         {  
- if(a[j][n-i-1]==0)  
-                 a[j][n-i-1]=m++;  
-         }  
- for(j=n-i-1;j>i;j--)//向左
-         {  
- if(a[n-i-1][j]==0)  
-                 a[n-i-1][j]=m++;  
-         }  
- for(j=n-i-1;j>i;j--)//向上
-         {  
- if(a[j][i]==0)  
-                 a[j][i]= m++;  
-         }  
-     }  
- if(n%2==1)//注意！当n为奇数时，有个正中心点
-         a[n/2][n/2] = m;  
- for(i=0;i<n;i++)//格式化输出
-     {  
- for(j=0;j<n;j++)  
-         {  
-             cout<<setw(2)<<setfill('0')<<a[i][j]<<"\t";  
-         }  
-         cout<<endl;  
-     }  
- }  
- int main()  
- {  
- int n;  
-     cout<<"请输入 n 的值：";  
-     cin>>n;   
-     fun(n);  
- return 0;  
- }   
- 
```java
public static void main(String[] args) {
        int n = 5;
        int[][] a = new int[n][n];
        int m = 1, i, j;
        for (i = 0; i < n; i++)//初始化矩阵元素为0
        {
            for (j = 0; j < n; j++)
                a[i][j] = 0;
        }
        for (i = 0; i < n / 2; i++)//只旋转n/2次
        {
            for (j = i; j < n - i; j++)//向右
            {
                if (a[i][j] == 0)
                    a[i][j] = m++;
            }
            for (j = i; j < n - i; j++)//向下
            {
                if (a[j][n - i - 1] == 0)
                    a[j][n - i - 1] = m++;
            }
            for (j = n - i - 1; j > i; j--)//向左
            {
                if (a[n - i - 1][j] == 0)
                    a[n - i - 1][j] = m++;
            }
            for (j = n - i - 1; j > i; j--)//向上
            {
                if (a[j][i] == 0)
                    a[j][i] = m++;
            }
        }
        if (n % 2 == 1)//注意！当n为奇数时，有个正中心点
            a[n / 2][n / 2] = m;
        for (i = 0; i < n; i++)//格式化输出
        {
            for (j = 0; j < n; j++) {
                System.out.print(a[i][j] + "\t");
            }
            System.out.println();
        }
    }
```
