# 【IM】关于聚类评价算法的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年10月26日 16:25:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：169
个人分类：[Algorithm](https://blog.csdn.net/fjssharpsword/article/category/6309933)









谱聚类是基于拉普拉斯特征映射的k近邻聚类，matlab代码如下：

```
>> n=500;c=2;k=10;t=randperm(n);a=linspace(0,2*pi,n/2)';
>> x=[a.*cos(a) a.*sin(a);(a+pi).*cos(a) (a+pi).*sin(a)];
>> x=x+rand(n,2);x=x-repmat(mean(x),[n,1]);x2=sum(x.^2,2);
>> d=repmat(x2,1,n)+repmat(x2',n,1)-2*x*x';[p,i]=sort(d);
>> W=sparse(d<=ones(n,1)*p(k+1,:));W=(W+W'~=0);
>> D=diag(sum(W,2));L=D-W;[z,v]=eigs(L,D,c-1,'sm');
>> m=z(t(1:c),:);s0(1:c,1)=inf;z2=sum(z.^2,2);
>> for o=1:1000
       m2=sum(m.^2,2);
       [u,y]=min(repmat(m2,1,n)+repmat(z2',c,1)-2*m*z');
       for t=1:c
           m(t,:)=mean(z(y==t,:));s(t,1)=mean(d(y==t));
       end
       if norm(s-s0)<0.001,break,end
       s0=s;
   end
>>  figure(1);clf;hold on;axis([-10 10 -10 10])
>> plot(x(y==1,1),x(y==1,2),'bo')
>> plot(x(y==2,1),x(y==2,2),'rx')
```

结果如：

![](https://img-blog.csdnimg.cn/20181026162247764.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_27,color_FFFFFF,t_70)

聚类依赖超参的选择，如k值，如何确定最佳k值，下面理解通过互信息来评价聚类的算法：

![](https://img-blog.csdnimg.cn/2018102616235379.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_27,color_FFFFFF,t_70)

参考matlab代码如：

```
>> n=500;a=linspace(0,2*pi,n/2)';
>> x=[a.*cos(a) a.*sin(a);(a+pi).*cos(a) (a+pi).*sin(a)];
>> x=x+rand(n,2);x=x-repmat(mean(x),[n,1]);x2=sum(x.^2,2);
>> y=[ones(1,n/2) zeros(1,n/2)];
>> d=repmat(x2,1,n)+repmat(x2',n,1)-2*x*x';
>> hhs=2*[0.5 1 2].^2;ls=10.^[-5 -4 -3];m=5；
>> u=floor(m*[0:n-1]/n)+1;u=u(randperm(n));
>> g=zeros(length(hhs),length(ls),m);
>> for hk=1:length(hhs)
       hh=hhs(hk);k=exp(-d/hh);
       for j=unique(y),for i=1:m
          ki=k(u~=i,y==j);kc=k(u==i,y==j);
          Gi=ki'*ki*sum(u~=i&y==j)/(sum(u~=i)^2);
          Gc=kc'*kc*sum(u==i&y==j)/(sum(u==i)^2);
          hi=sum(k(u~=i&y==j,y==j),1)'/sum(u~=i);
          hc=sum(k(u==i&y==j,y==j),1)'/sum(u==i);
          for lk=1:length(ls)
              l=ls(lk);a=(Gi+l*eye(sum(y==j)))\hi;
              g(hk,lk,i)=g(hk,lk,i)+a'*Gc*a/2-hc'*a;
    end,end,end,
end
>> g=mean(g,3);[gl,ggl]=min(g,[],2);[ghl,gghl]=min(gl);
>> L=ls(ggl(gghl));HH=hhs(gghl);s=-1/2;
>> for j=unique(y)
       k=exp(-d(:,y==j)/HH);h=sum(k(y==j,:),2)/n;t=sum(y==j);
       s=s+h'*((k'*k*t/(n^2)+L*eye(t))\h)/2;
end
>> disp(sprintf('Information=%g',s));
Information=0.498697
```

理解互信息法评价聚类，要理解KL散度，以及近似密度估计函数的思路。

而谱聚类本身则要理解拉普拉斯特征映射（降维方法）。



