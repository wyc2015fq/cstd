# matlab 画 曲面 曲线 直线 - 家家的专栏 - CSDN博客





2014年05月12日 10:16:54[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1280








**参考网址：http://www.cnblogs.com/tornadomeet/archive/2013/03/15/2961660.html**

**线：**

hold on;

plot(x(:,2),y,'*')

plot(x(:,2), x*theta, '-');

plot(x(:,2), x*theta1, 'r-');

hold off;

**面：**

figure;

surf(theta0_vals,theta1_vals,J);



**曲线：**

figure;

contour(theta0_vals,theta1_vals,J, logspace(-2, 2, 15));





![](https://img-blog.csdn.net/20140512101355734)![](https://img-blog.csdn.net/20140512101404640)![](https://img-blog.csdn.net/20140512101404640)![](https://img-blog.csdn.net/20140512101411796)![](https://img-blog.csdn.net/20140512101411796)



