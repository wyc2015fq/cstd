# VTI介质Thomsen参数关于入射角的敏感性曲线 - Koma Hub - CSDN博客
2018年05月15日 10:22:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：99
个人分类：[Geophysics																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/7221460)
简单做个备份
VTI介质ADCIGs中剩余曲率根据角度的变化可以进一步分析得出反演策略，根据相速度公式求解的深度残差关于Thomsen参数的偏导数：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
float deltaz(float v, float e, float d, float theta)
{
    float vp=2000;
    float beta=0.0;
    float t=1;
    theta *= 3.1415926/180.0;
    
    return (v-vp)*t/(2.0*cos(theta)*cos(beta))
            *sqrt(0.5 + e*sin(theta-beta)*sin(theta-beta)
                    + 0.5*sqrt( pow(1+2*e*sin(theta-beta)*sin(theta-beta),2.0 )
                                - 2*(e-d)*sin(2*theta-2*beta)*sin(2*theta-2*beta))  );
}
int main(int argc, char **argv)
{
    float diff = 0.1;
    float theta, dzv, dze, dzd;
    int i;
    
    FILE *fp1 = fopen("dz.txt","w");
    
    for(i=0;i<90;i++){
        
        theta = i*1.0;
        dzv = deltaz(1800-diff, 0.5, 0.3, theta) - deltaz(1800, 0.5, 0.3, theta);
        dze = deltaz(1800, 0.5+diff, 0.3, theta) - deltaz(1800, 0.5, 0.3, theta);
        dzd = deltaz(1800, 0.5, 0.3+diff, theta) - deltaz(1800, 0.5, 0.3, theta);
    
        printf("%f, %f, %f\n",dzv,dze,dzd);
        fprintf(fp1,"%f     %f     %f\n",dzv,dze,dzd);
    }
}
```
得出关系曲线：
![](https://img-blog.csdn.net/20180301112915378)
