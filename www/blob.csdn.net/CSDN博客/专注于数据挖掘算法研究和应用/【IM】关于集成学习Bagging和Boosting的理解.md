# 【IM】关于集成学习Bagging和Boosting的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年10月11日 11:35:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：84
个人分类：[Algorithm](https://blog.csdn.net/fjssharpsword/article/category/6309933)
集成学习在各大比赛中非常流程，如XGboost、LGBM等，对其基学习器决策树及其剪枝等，可参考：
[https://blog.csdn.net/fjssharpsword/article/details/54861274](https://blog.csdn.net/fjssharpsword/article/details/54861274)
集成学习可参考：
[https://blog.csdn.net/fjssharpsword/article/details/61913092](https://blog.csdn.net/fjssharpsword/article/details/61913092)
![](https://img-blog.csdn.net/201810111133152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于Adaboost的matlab源码和图示：
```
>> n=50;x=randn(n,2);y=2*(x(:,1)>x(:,2))-1;b=5000;
>> a=50;Y=zeros(a,a);yy=zeros(size(y));w=ones(n,1)/n;
>> X0=linspace(-3,3,a); [X(:,:,1) X(:,:,2)] = meshgrid(X0);
>> for j=1:b
      wy=w.*y;d=ceil(2*rand);[xs,xi]=sort(x(:,d));
      el=cumsum(wy(xi));eu=cumsum(wy(xi(end:-1:1)));
      e=eu(end-1:-1:1)-el(1:end-1);
      [em,ei]=max(abs(e));c=mean(xs(ei:ei+1));s=sign(e(ei));
      yh=sign(s*(x(:,d)-c));R=w'*(1-yh.*y)/2;
      t=log((1-R)/R)/2;yy=yy+yh*t;w=exp(-yy.*y);w=w/sum(w);
      Y=Y+sign(s*(X(:,:,d)-c))*t;
   end
>> figure(1);clf;hold on;axis([-3 3 -3 3]);
>> colormap([1 0.7 1;0.7 1 1]);contourf(X0,X0,sign(Y));
>> plot(x(y==1,1),x(y==1,2),'bo');
>> plot(x(y==-1,1),x(y==-1,2),'rx');
```
![](https://img-blog.csdn.net/20181011113422720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于Adaboost通过指数损失来推导：
![](https://img-blog.csdn.net/20181011113448807?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
