# matlab数据拟合 - qq_32515081的博客 - CSDN博客
2018年12月17日 21:29:15[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：83
- cftool工具箱主要是针对数据拟合的。使用起来特别的强大，尤其对于数据的处理超级方便，可以直接对于数据拟合，并且可以预设各种的拟合方案。这里注意的是非线性的也可以进行拟合，例如：幂律，高斯等等。
[https://jingyan.baidu.com/article/6f2f55a16aba04b5b93e6cca.html](https://jingyan.baidu.com/article/6f2f55a16aba04b5b93e6cca.html)
[https://blog.csdn.net/rayna00/article/details/45060237](https://blog.csdn.net/rayna00/article/details/45060237)
[https://www.cnblogs.com/tensory/p/6590779.html](https://www.cnblogs.com/tensory/p/6590779.html)
[http://blog.sina.com.cn/s/blog_4d4afb6d0100t37n.html](http://blog.sina.com.cn/s/blog_4d4afb6d0100t37n.html)
- polyfit(x,y,n)。用多项式求过已知点的表达式，其中x为源数据点对应的横坐标，可为行向量、矩阵，y为源数据点对应的纵坐标，可为行向量、矩阵，n为你要拟合的阶数，一阶直线拟合，二阶抛物线拟合，并非阶次越高越好，看拟合情况而定。
[https://blog.csdn.net/misayaaaaa/article/details/71156256](https://blog.csdn.net/misayaaaaa/article/details/71156256)
[https://blog.csdn.net/wendingzhulu/article/details/43062845](https://blog.csdn.net/wendingzhulu/article/details/43062845)
- fittype。
[http://www.javaxxz.com/thread-366786-1-1.html](http://www.javaxxz.com/thread-366786-1-1.html)
```
%fittype function
%for Y=wm1.*N./power(1+a1*N,b1);
%when 0<ab-a<1,there's a peak,up first and down secondly;
%when 1<ab-a,all down;
%when ab-a<0,all up.
wm1=5,a1=2,b1=1.1;
N = linspace(1,100);
Y = wm1.*N./power(1+a1*N,b1);
plot(N,Y)
myfittype = fittype('wm.*N./power(1+a*N,b)','dependent',{'Y'},'independent',{'N'},...
    'coefficients',{'wm','a','b'});
myfit = fit(N',Y',myfittype)
a=myfit.a
b=myfit.b
wm=myfit.wm
plot(myfit,N,Y)
```
- 非线性拟合lsqcurvefit、nlinfit、lsqnonlin
lsqcurvefit纵轴最佳拟合的范围在0到100（绝对值），实际拟合情况还需要根据数据特点来定，但是比nlinfit、lsqnonlin效果要好很多。
[https://www.cnblogs.com/Juli016/articles/5211239.html](https://www.cnblogs.com/Juli016/articles/5211239.html)
[https://blog.csdn.net/u014356002/article/details/70242501](https://blog.csdn.net/u014356002/article/details/70242501)
[https://blog.csdn.net/huahua19891221/article/details/81841220](https://blog.csdn.net/huahua19891221/article/details/81841220)
```
%lsqcurvefit function
%首先制造一些假数据，用来检验函数的可行性；
%当然也可以直接把真实数据赋值给N、Y两个变量.
wm1=50,a1=2,b1=2; %函数中的三个参数.
N=1:100;
Y=wm1*N./power(1+a1*N,b1)+rand(size(N)); %待检验的函数；并加入均匀分布的噪声
scatter(N,Y)
hold on;
a=[1 2 3]; %变量设置，分别对应函数中的三个参数.
f=@(a,x)a(1)*x./power(1+a(2)*x,a(3)); %设定函数变量及其参数.
a=lsqcurvefit(f,a,N,Y) %利用相应函数提取参数.
x=1:100;
f2=a(1)*x./power(1+a(2)*x,a(3))； %得到的拟合函数
plot(f2)
```
```
%lsqnonlin function
wm1=10,a1=1,b1=0.9;
N=1:100;
Y=wm1*N./power(1+a1*N,b1)+rand(size(N));
scatter(N,Y)
hold on;
fun=@(a)a(1)*N./power(1+a(2)*N,a(3))-Y;
x0=[1,1,1];
a=lsqnonlin(fun,x0)
f2=a(1)*N./power(1+a(2)*N,a(3));
plot(N,f2)
```
