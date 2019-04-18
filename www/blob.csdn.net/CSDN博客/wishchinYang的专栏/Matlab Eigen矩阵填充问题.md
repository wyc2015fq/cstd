# Matlab/Eigen矩阵填充问题 - wishchinYang的专栏 - CSDN博客
2015年09月07日 11:34:31[wishchin](https://me.csdn.net/wishchin)阅读数：1082
 Matlab进行矩阵填充时可以填充空矩阵，相当于空矩阵不存在，例如一下代码：
```
P_RES = [ P_xv          P_xvy                       P_xv*dy_dxv';
          P_yxv         P_y                         P_yxv*dy_dxv';
          dy_dxv*P_xv   dy_dxv*P_xvy                dy_dxv*P_xv*dy_dxv'+...
                                                    dy_dhd*Padd*dy_dhd'];
```
结果是可行的。
而使用C++ Eigen时，使用的填充代码是这样的：
```cpp
Eigen::MatrixXf  P13,P23,P33,P31,P32;
    P13 =   P_xv*dy_dxv.transpose();//13*6的矩阵
    P23 =   P_yxv*dy_dxv.transpose();//0*6的矩阵
    P33 =   dy_dxv*P_xv*dy_dxv.transpose() + dy_dhd*Padd*dy_dhd.transpose();//6*6的矩阵
    P31 =   dy_dxv*P_xv ;//6*13的矩阵
    P32 =   dy_dxv*P_xvy ;//6*0的矩阵
    P_RES<< 
        P_xv  ,    P_xvy  ,   P13 ,
        P_yxv ,     P_y   ,   P23 ,
        P31  ,      P32 ,     P33 ;
```
矩阵的填充形式是这样的：
```cpp
P_RES<< 
        P_xv(13*13)  ,    P_xvy(13*0)  ,    13*6 ,
        P_yxv(0*13) ,     P_y(0,0)     ,     0*6 ,
        6*13       ,          6*0      ,     6*6 ;
```
然而出现错误！
原因：   
/1./P_RES需要Resize！
```cpp
P_RES.resize(19,19);
```
**注意事项**：
      在Eigen矩阵填充时，注意子矩阵行列的对齐。
对于
[  13*13      13*6
    6*13        6*6      ] 这种填充方式是可以的
而
[  13*13      6*13
    13*6        6*6      ] 这种填充方式是不可以的
