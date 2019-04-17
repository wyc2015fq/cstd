# 计算直线与平面交点的JavaScript脚本代码 - cocoonyang的专栏 - CSDN博客





2016年10月11日 13:57:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：934








                
已知直线过点 m(x_m, y_m, z_m ), 其方向向量为 V_l( vl_x,  vl_y,  vl_z ).

平面过点 n(x_n, y_n, z_n), 法线方向为 V_p( vp_x,  vp_y,  vp_z ).


直线的参数方程为


x = x_m + vl_x * t 

y = y_m + vl_y * t 

z = z_m + vl_z * t 


平面方程为：

vp_x * (x - x_n) + vp_y * (x - x_n) +  vp_z * (x - x_n)  = 0



将直线参数方程和平面方程联立求解得


t = ( ( x_n - x_m ) * vp_x + ( y_n - y_m ) * vp_y + ( z_n - z_m ) * vp_z ) 

/ ( vp_x * vl_x  + vp_y * vl_y + vp_z * vl_z ) 


将 t 带入直线方程中，就可求得交点坐标。   



```java
//
//
//  计算直线与平面交点的JavaScript脚本代码  
//  参考http://blog.csdn.net/abcjennifer/article/details/6688080
//

function CalPlaneLineIntersectPoint( planeVector,  planePoint,  lineVector,  linePoint)  
{  
    var returnResult = Array();  
    var vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t,vpt;  
    vp1 = planeVector[0];  
    vp2 = planeVector[1];  
    vp3 = planeVector[2];  
    n1 = planePoint[0];  
    n2 = planePoint[1];  
    n3 = planePoint[2];  
    v1 = lineVector[0];  
    v2 = lineVector[1];  
    v3 = lineVector[2];  
    m1 = linePoint[0];  
    m2 = linePoint[1];  
    m3 = linePoint[2];  
    vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;  
    //首先判断直线是否与平面平行  
    if (vpt == 0)  
    {  
        return  returnResult  ;  
    }  
    else  
    {  
        t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;  
        returnResult[0] = m1 + v1 * t;  
        returnResult[1] = m2 + v2 * t;  
        returnResult[2] = m3 + v3 * t;  
    }  
    return returnResult;  
}
```











































