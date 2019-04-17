# matlab 曲线平滑 - 家家的专栏 - CSDN博客





2011年10月13日 18:56:34[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4186








 x=[0 0.1 0.16 0.27 0.41 0.48 0.59 0.8];

y=[5 9 70 118 100 17 0 5];

y1=[22.8 22.8 22.8 22.8 22.8 22.8 22.8 22.8];

values1=spcrv([[x(1) x x(end)];[y(1) y y(end)]],3,1000);

values2=spcrv([[x(1) x x(end)];[y1(1) y1 y1(end)]],3,1000);

plot(values1(1,:),values1(2,:),'r',values2(1,:),

values2(2,:),'b') 



