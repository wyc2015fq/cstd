# matlab中的四种取整函数的使用(fix, floor, ceil, round) - fighting！！！ - CSDN博客
2019年01月07日 19:44:37[dujiahei](https://me.csdn.net/dujiahei)阅读数：31
转自：[https://blog.csdn.net/qq_32252957/article/details/82968469](https://blog.csdn.net/qq_32252957/article/details/82968469)
(1)fix(x) : 截尾取整.
>> fix( [3.12 -3.12])
ans =
     3    -3
(2)floor(x):不超过x 的最大整数.(高斯取整)
>> floor( [3.12 -3.12])
ans =
     3    -4
(3)ceil(x) : 大于x 的最小整数
>> ceil( [3.12 -3.12])
ans =
     4    -3
(4)四舍五入取整
>> round(3.12 -3.12)
ans =
     0
>> round([3.12 -3.12])
ans =
     3    -3
MATLAB中四个取整函数具体使用方法如下：
综述：Matlab取整函数有: fix, floor, ceil, round. 4个
fix——朝零方向取整，
如fix(-1.3)=-1; fix(1.3)=1;
floor——朝负无穷方向取整，
如floor(-1.3)=-2; floor(1.3)=1;
ceil——朝正无穷方向取整，
如ceil(-1.3)=-1; ceil(1.3)=2;
round——四舍五入到最近的整数，
如round(-1.3)=-1;round(-1.52)=-2;round(1.3)=1;round(1.52)=2。
--------------------- 
作者：youaresherlock 
来源：CSDN 
原文：https://blog.csdn.net/qq_32252957/article/details/82968469 
