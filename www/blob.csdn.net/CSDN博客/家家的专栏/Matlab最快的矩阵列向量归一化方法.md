# Matlab最快的矩阵列向量归一化方法 - 家家的专栏 - CSDN博客





2014年08月04日 11:28:07[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4743








From 组长[卢参义](http://blog.sciencenet.cn/home.php?mod=space&uid=242887)的博客： 
  http://blog.sciencenet.cn/blog-242887-496374.html




在使用matlab撰写代码时可能会遇到的一个问题就是矩阵列向量的归一化。在优化matlab代码方面，有一个较普遍适用的观点是尽可能地把数据向量化、矩阵化，因为matlab擅长矩阵运算。因此在一些matlab优化方面的教材中会看到一种向量化的矩阵列向量归一化方法，如下

vMag = sqrt(sum(X.^2));

X = X./vMag(ones(1,size(X,1)),:);

但事实上这种方法不仅耗内存而且又慢，目前我所知道的最快速的方法是

for i = 1 : num

   X(:,i) = X(:,i) / norm(X(:,i)) ;

end

这种方法相比于前一种方法在计算速度方面至少可以提升一倍。具体测试如下：

X = rand(5000,5000) ;

[dim,num] = size(X) ;

tic

for i = 1 : num

   X(:,i) = X(:,i) / norm(X(:,i)) ;

end

t1 = toc




tic

vMag = sqrt(sum(X.^2));

X = X./vMag(ones(1,size(X,1)),:);

t2= toc

返回结果：

t1 =

    0.5162



t2 =

    1.0806



如果把数据弄得更大，如

X = rand(10000,5000) ;

那么第一种方法内存都不足！返回结果如下，



t1 =

    0.9821



??? Out of memory. Type HELP MEMORY for your options.

Error in ==> Experiment_YaleB at 12

vMag = sqrt(sum(X.^2));




因此，不是所有的向量化操作都会提高运行效率，唯一的检验标准就是实践！



