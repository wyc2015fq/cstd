# 程序员的浪漫：教你如何用MATLAB向女生表白 - Machine Learning with Peppa - CSDN博客





2018年04月10日 17:50:34[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：4769








MATLAB可以实现很多数学中有趣的函数图形，利用函数+编程语言向女生发送一段代码，编译执行后，你的心意就会被收到啦，这样浪漫的感觉只有程序员才懂。

注：以下函数皆可用Python绘图库复现

### 平面心形图像

经过网上搜索，平面坐标系中，下面函数最像心形图像，而且函数形式简单，不需要分段。

221329x2+(y−(x2)1/3)2=9

改变右面的数值可以变化心形图像的大小。



**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)



- > ezplot('x^2+(y-(x^2)^(1/3))^2=9')  



![](https://img-blog.csdn.net/20161111192728622)



还有一个函数：



2322130−x2y3+(x2+y2−1)3=0



**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)



- ezplot('-x^2*y^3+(x^2+y^2-1)^3=0',[-1.5,1.5])  



![](https://img-blog.csdn.net/20161111193755361)

另外一个函数：



1721617220017x2−16|x|y+17y2=200





**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)



- ezplot('17*x.^2-16*abs(x).*y+17*y.^2=200'  




![](https://img-blog.csdn.net/20161111194533287)


另外一个分段函数：



22−−√2−−−−−−−−−√2.142–√−−√−−−−−−−−−√f(x)=2x2−x2g(x)=−2.142−|x|







**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)



- x=linspace(-2,2,1000);  
- y1=sqrt(2*sqrt(x.^2)-x.^2);  
- y2=-2.14*sqrt(sqrt(2)-sqrt(abs(x)));  
- plot(x,y1,'b',x,y2,'b');  
- axis([-2.5,2.5,-3,1.5]);  


![](https://img-blog.csdn.net/20161111203451402)

另外一个：

1631352234x=16(sint)3y=13cost−5cos(2t)−2cos(3t)−cos(4t)







**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)




- t=linspace(-6,6,1000);  
- x=16*(sin(t)).^3;  
- y=13*cos(t)-5*cos(2*t)-2*cos(3*t)-cos(4*t);  
- plot(x,y);  





![](https://img-blog.csdn.net/20161111204311459)








### 立体心形图像

立体心形貌似只能由笛卡尔方程得到：

292421323923800(x2+9y2/4+z2−1)3−x2z3−9y2z3/80=0



画图时用 isosurface 和 patch  函数：

图形1：






**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)




- f=@(x,y,z)(x.^2+ (9./4).*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3 - (9./80).*y.^2.*z.^3;  
- [x,y,z]=meshgrid(linspace(-3,3));  
- val=f(x,y,z);  
- [p,v]=isosurface(x,y,z,val,0);  
- patch('faces',p,'vertices',v,'facevertexcdata',jet(size(v,1)),'facecolor','w','edgecolor','flat');  
- view(3);grid on;axis equal;  




![](https://img-blog.csdn.net/20161111212408612)





图形2：






**[plain]**[view plain](https://blog.csdn.net/robert_chen1988/article/details/53123462#)[copy](https://blog.csdn.net/robert_chen1988/article/details/53123462#)




- f=@(x,y,z)(x.^2+ (9./4).*y.^2 + z.^2 - 1).^3 - x.^2.*z.^3 - (9./80).*y.^2.*z.^3;  
- [x,y,z]=meshgrid(linspace(-1.5,1.5));  
- val=f(x,y,z);  
- isosurface(x,y,z,val,0);   
- axis equal;view(3);colormap([1 0.2 0.2])  


![](https://img-blog.csdn.net/20161111212412763)



