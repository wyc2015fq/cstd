# 【数值计算】一元多值函数求极值——利用fminbnd和图形观察 - 心纯净，行致远 - CSDN博客





2018年01月24日 10:34:08[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：721








一元多值函数求极值在实际科研中具有很重要的作用，其中求最大值和求最小值又有相似的地方，只要可以求出两者中的一者，对函数加一个负号就可以求出另一个最值。

下面是一个利用fminbnd函数，结合图形观察得出最小值的人工求解方法。**fminbnd函数的功能是求非线性函数极小值点，但注意求解的是某个区间内的其中一个极小值点，不一定会是该区间的最小值点。**下面是fminbnd函数使用格式：



```
[xmin,ymin,exitflag]=fminbnd（f,x,y);

%f为这个函数的句柄，x,y为搜索的范围，xmin,ymin为搜索到的其中一个极小值点的坐标，exitflag为1表示极小值点搜索成功，为0表示没有找到极小值点。exitflag可以缺省。
```
实例：下面演示非线性函数![](https://img-blog.csdn.net/20180124101544446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)在[-50,5]内求极小值。主要演示fminbnd只能求函数的极小值点；利用图形，观察函数在指定区间中的整体形态，来求解最小值；ezplot命令要求匿名函数采用数组运算。


解：

1）在整个指定区间采用优化算法求极小值



```
x1=-50;
x2=5;
yx=@(x)(sin(x).^2.*exp(-0.1*x)-0.5*sin(x).*(x+0.1)); %函数句柄结构
[xc0,fc0,exitflag]=fminbnd(yx,x1,x2) %在指定区间搜索极小值
xc0 =

   -8.4867


fc0 =

   -1.8621


exitflag =

     1
%由exitflag为1知，成功搜索到一个极小值
```



2）绘图指定区间函数图像





```
xx=-50:pi/200:5;
ezplot(yx,[-50,5]) %利用函数句柄在指定区间绘图
xlabel('x'),grid on
```
![](https://img-blog.csdn.net/20180124102541586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3）观察图形，缩小最值求解范围，重设fminbnd搜索区间



```
%在[-23,-18]之间的两个极值点最有可能是最小值点，分成[-23,-20]和[-20,-18]两端计算
xx=[-23,-20,-18];
fc=fc0; %以第一次搜索极小值为参考
xc=xc0;
for k=1:2
[xw,fw]=fminbnd(yx,xx(k),xx(k+1)); %与该区间的极小值进行比较，如果较小，则更新
if fw<fc0
fc0=fw;
xc0=xw;
end
end
fprintf('函数最小值%6.5f发生在x=%6.5f处\n',fc0,xc0)
函数最小值-3.34765发生在x=-19.60721处
```

这种方法具有重要意义，但是寻求最值的过程还需要人眼进行观察。由于函数求最值具有典型作用，可以考虑写成一个函数脚本，以便以后直接使用，包括可以写一个脚本，搜索指定区间的所有极值点。





