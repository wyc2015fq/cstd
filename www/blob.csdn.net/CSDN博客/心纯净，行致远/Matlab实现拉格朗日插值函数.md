# Matlab实现拉格朗日插值函数 - 心纯净，行致远 - CSDN博客





2018年04月01日 15:30:51[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：10916标签：[MATLAB																[数值计算																[拉格朗日](https://so.csdn.net/so/search/s.do?q=拉格朗日&t=blog)
个人分类：[【MatLab/Simulink】																[【数学基础】](https://blog.csdn.net/zhanshen112/article/category/7556201)](https://blog.csdn.net/zhanshen112/article/category/7184373)





```
function yy=lagrange(x1,y1,xx)
%本程序为Lagrange1插值，其中x1,y1
%为插值节点和节点上的函数值，输出为插值点xx的函数值，
%xx可以是向量。
syms x
n=length(x1);
for i=1:n
t=x1;t(i)=[];L(i)=prod((x-t)./(x1(i)-t));% L向量用来存放插值基函数
end
u=sum(L.*y1);
p=simplify(u) % p是简化后的Lagrange插值函数（字符串）
yy=subs(p,x,xx);    %p是以x为自变量的函数，并求xx处的函数值
end
```](https://so.csdn.net/so/search/s.do?q=数值计算&t=blog)](https://so.csdn.net/so/search/s.do?q=MATLAB&t=blog)




