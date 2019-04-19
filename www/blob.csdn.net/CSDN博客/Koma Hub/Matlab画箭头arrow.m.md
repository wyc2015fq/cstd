# Matlab画箭头arrow.m - Koma Hub - CSDN博客
2018年01月19日 16:10:18[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2054
源码网址[点击打开链接](http://www.ilovematlab.cn/forum.php?mod=viewthread&tid=17206&page=1&authorid=229)
```
function  arrow(P,V,color,LineWidth)
%起点P，V变化量
if 1 <= nargin < 3 
    color = 'b'; 
end
x0 = P(1);y0 = P(2); 
a = V(1); b = V(2); 
l = max(norm(V), eps);
u = [x0 x0 a]; v = [y0 y0 b]; 
hchek = ishold;
plot(u,v,color,'LineWidth',LineWidth)
hold on 
h = l - min(.2*l, .2) ;
v = min(.2*l/sqrt(3), .2/sqrt(3) );
a1 = (a*h -b*v)/l;
b1 = (b*h a*v)/l; 
plot([x0 a1, x0 a], [y0 b1, y0 b], color,'LineWidth',LineWidth) 
a2 = (a*h b*v)/l; 
b2 = (b*h -a*v)/l; 
plot([x0 a2, x0 a], [y0 b2, y0 b], color,'LineWidth',LineWidth)
if hchek == 0 
    hold off 
end
```
![](https://img-blog.csdn.net/20180119161454456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
