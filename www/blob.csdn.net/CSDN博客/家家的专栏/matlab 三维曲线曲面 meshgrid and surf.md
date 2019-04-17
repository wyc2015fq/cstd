# matlab 三维曲线曲面 meshgrid and surf - 家家的专栏 - CSDN博客





2014年01月14日 19:55:37[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1811
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









                
x=0:0.1:2*pi;

y=x;

[X,Y]=meshgrid(x,y);

Z=sin(X)+sin(Y);

%3D spline

plot3(X,Y,Z);



%3D surface

figure;

surf(X,Y,Z)




![](https://img-blog.csdn.net/20140114200251109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140114200258125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




