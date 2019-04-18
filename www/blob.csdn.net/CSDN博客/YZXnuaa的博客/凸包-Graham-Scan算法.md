# 凸包-Graham-Scan算法 - YZXnuaa的博客 - CSDN博客
2018年03月23日 19:44:22[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：165
个人分类：[基础算法](https://blog.csdn.net/YZXnuaa/article/category/7523136)
（1）问题：
给定二维平面点集，求最小的包含所有点的凸多边形。
（2）Gramham-Scan算法：
Gramham-Scan是一种灵活的凸包算法，其总的时间复杂度仅为O(n*log(n))。
步骤：
Step1: 选定x坐标最小（相同情况y最小）的点作为极点，这个点必在凸包上；
Step2: 将其余点按极角排序，在极角相同的情况下比较与极点的距离，离极点比较近的优先；
Step3: 用一个栈S存储凸包上的点，先将按极角和极点排序最小的两个点入栈；
Step4: 按序扫描每个点，检查栈顶的两个元素与这个点构成的折线段是否“拐”向右侧（叉积小于等于零）；
Step5: 如果满足，则弹出栈顶元素，并返回Step4再次检查，直至不满足。将该点入栈，并对其余点不断执行此操作；
Step6: 最终栈中元素为凸包的顶点序列。
（3）模板（来自kuangbin模板）
**[cpp]**[view plain](https://blog.csdn.net/thinking2013/article/details/40620209#)[copy](https://blog.csdn.net/thinking2013/article/details/40620209#)
- #include <iostream>
- #include <cstdio>
- #include <cmath>
- #include <algorithm>
- usingnamespace std;  
- 
- constdouble eps = 1e-8;  
- struct Point{  
- double x, y;  
- };  
- 
- constint MAXN = 1010;  
- Point list[MAXN];  
- int stack[MAXN], top;  
- 
- /***
- * 叉积
- * a×b>0, 则b在a的逆时针方向；
- * a×b<0, 则b在a的顺时针方向；
- * a×b=0, 则a与b共线，但可能同向也可能反向。
- */
- double crossProduct(Point a, Point b){  
- return a.x*b.y - a.y*b.x;  
- }  
- 
- int sgn(double x){  
- if(fabs(x) < eps) return 0;  
- if(x < 0) return -1;  
- elsereturn 1;  
- }  
- 
- Point sub(Point a, Point b){  
-     Point p;  
-     p.x = a.x - b.x;  
-     p.y = a.y - b.y;  
- return p;  
- }  
- 
- double dist(Point a, Point b){  
- return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));  
- }  
- 
- //相对于极点list[0]的极角排序
- bool cmp(Point p1, Point p2){  
- double temp  = crossProduct(sub(p1, list[0]), sub(p2, list[0]));  
- if(sgn(temp)>0) returntrue;  
- elseif(sgn(temp)==0 && sgn(dist(p1, list[0])-dist(p2, list[0]))<=0) returntrue;  
- elsereturnfalse;  
- }  
- 
- /*
- * 求凸包，Graham算法
- * 点的编号0~n-1
- * 返回凸包结果Stack[0~top-1]为凸包的编号
- */
- void Graham(int n){  
-     Point p0 = list[0];  
- int k = 0;  
- for(int i=1;i<n;i++){  
- if(p0.y>list[i].y || (p0.y==list[i].y && p0.x>list[i].x)){  
-             p0 = list[i];  
-             k = i;  
-         }  
-     }  
-     swap(list[k], list[0]);  
-     sort(list+1, list+n, cmp);  
- 
-     stack[0] = 0;  
- if(n==1){top = 1; return;}  
-     stack[1] = 1;  
- if(n==2){top = 2; return;}  
- 
-     top = 2;  
- for(int i=2;i<n;i++){  
- while(top>1 && sgn(crossProduct(sub(list[stack[top-1]], list[stack[top-2]]), sub(list[i], list[stack[top-2]])))<=0){  
-             top--;  
-         }  
-         stack[top++] = i;  
-     }  
- }  
