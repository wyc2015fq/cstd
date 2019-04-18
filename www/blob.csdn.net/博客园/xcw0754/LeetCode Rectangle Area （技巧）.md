# LeetCode  Rectangle Area （技巧） - xcw0754 - 博客园
# [LeetCode  Rectangle Area （技巧）](https://www.cnblogs.com/xcw0754/p/4934418.html)
题意：
　　分别给出两个矩形的左下点的坐标和右上点的坐标，求他们覆盖的矩形面积？
思路：
　　不需要模拟，直接求重叠的部分的长宽就行了。问题是如果无重叠部分，注意将长/宽给置为0。
```
1 class Solution {
 2 public:
 3     int getArea(int A,int B,int C,int D){return (C-A)*(D-B);}
 4     int computeArea(int A, int B, int C, int D, int E, int F, int G, int H)
 5     {
 6         int x= min(C,G)>max(A,E)? min(C,G)-max(A,E) : 0;
 7         int y= min(D,H)>max(B,F)? min(D,H)-max(B,F) : 0;
 8         return getArea(A,B,C,D) + getArea(E,F,G,H) - x*y;
 9     }
10 };
AC代码
```

