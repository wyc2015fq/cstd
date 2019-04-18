# MATLAB图形的透视、镂空和裁切实例 - 心纯净，行致远 - CSDN博客





2018年01月01日 12:56:34[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：901








**透视**

MATLAB在采用默认设置画mesh图形时，对叠压在后面的图形采取了消隐措施，后面的图形会被遮住。但有时却需要透视效果。为此，有以下两个控制消隐效果的命令



```
hidden off   % 透视被叠压的图形
hidden on   %消隐被叠压的图形
```


实例：


```
%图形的透视
>> [XO,YO,ZO]=sphere(30);   %产生单位球面的三维坐标
>> X=2*XO;    %产生半径为2的球面三维坐标
>> Y=2*YO;
>> Z=2*ZO;
>> surf(XO,YO,ZO);    %画单位球面
>> hold on
>> mesh(X,Y,Z),colormap(hot)   %采用hot色图
>> hold off
>> hidden off      %产生透视效果
>> axis equal,axis off   %不显示坐标轴
```
透视效果如下：

![](https://img-blog.csdn.net/20180101123349074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明：1，hidden对surf命令绘制的表面图不发生任何影响；

           2，alpha命令可对透明度进行更细致的控制。




**镂空**




演示：如何利用“非数”NaN，对图形进行镂空处理



```
>> clear
>> P=peaks(30);   %从MATLAB提供的双变量正态分布曲面获取数据
>> P(18:20,9:15)=NaN;   %为镂空赋值
>> surfc(P);    %带等位线的曲面图
>> colormap(hot)
>> light('position',[50,-10,5])
>> material([0.9,0.9,0.6,15,0.4])
>> grid off
>> box on
```
![](https://img-blog.csdn.net/20180101124518930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**裁切**

有时为了看清图形需要表现切面，应该把被切部分强制为0.

实例：



```
>> x=[-8:0.1:8];
>> y=x;
>> [X,Y]=meshgrid(x,y);
>> ZZ=X.^2-Y.^2;
>> ii=find(abs(X)>6|abs(Y)>6);   %确定超出[-6,6]范围格点下标
>> ZZ(ii)=zeros(size(ii));    %强制为0
>> surf(X,Y,ZZ)
>> shading interp;
>> colormap(copper)
>> light('position',[0,-15,1]);
>> lighting phong 
>> material([0.8,0.8,0.5,10,0.5])
```
![](https://img-blog.csdn.net/20180101125533872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







