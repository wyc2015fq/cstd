# UVA Stacks of Flapjacks 栈排序 - xcw0754 - 博客园
# [UVA Stacks of Flapjacks 栈排序](https://www.cnblogs.com/xcw0754/p/4447144.html)
题意:给一个整数序列，输出每次反转的位置，输出0代表排序完成。给一个序列1 2 3 4 5，这5就是栈底，1是顶，底到顶的位置是从1~5，每次反转是指从左数第i个位置，将其及其左边所有的数字都反转，假如反转位置2，则1 2 3 4 5就变成 4 3 2 1 5. 问怎样经过最少次数的反转能得到升序12345。
思路：每次产生一个最大的数字到右边，经过n次就升序了。每次产生可能需要两次反转，也可能1次就搞定，可能0次。0次是因为该数已经在最终位置上；1次搞定是因为最大数已经在栈顶（最左边），一反转就到达了它最终的位置上；2次是因为最大数不在栈顶，需要经过多一次反转，将它搞到栈顶先，再按1次的情况处理。难度主要在输入输出上。
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <algorithm>
 4 #include <stdio.h>
 5 using namespace std;
 6 int a[50],sorted_b[50];
 7 int main()
 8 {
 9     //freopen("input.txt", "r", stdin);
10     char c;
11     int tmp;
12     while(1)
13     {
14         int i=0;
15         while(scanf("%d", &tmp),c=getchar())
16         {
17             cout<<(a[i]=sorted_b[i]=tmp);
18             i++;
19             if(c=='\n'||c==EOF) break;
20             else    cout<<" ";
21         }
22         printf("\n");
23         sort(sorted_b, sorted_b+i);
24         for(int j=i-1; j>0; j--)
25         {
26             if(sorted_b[j]==a[j])   continue;   //相同则略过
27             if(a[0]!=sorted_b[j])         //如果最大已经在栈顶，省去一次翻转
28             {
29                 int k;
30                 for(k=1; k<j; k++)     //寻找sorted_b[j]
31                 {
32                     if(a[k]==sorted_b[j])   break;
33                 }
34                 cout<<i-k<<" ";             //此位置要翻
35                 reverse(a,a+k+1);
36             }
37             cout<<i-j<<" ";
38             reverse(a,a+j+1);
39         }
40         cout<<"0"<<endl;
41         if(c==EOF) return 0;
42     }
43     return 0;
44 }
AC代码
```

