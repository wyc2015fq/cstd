# randperm(N) 产生一个1到N的数组 - 家家的专栏 - CSDN博客





2011年08月31日 15:33:09[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2629








                
 设给定一个数组a=[2.70 3.01 3.82 3.22 4.12 3.73 3.24 2.90 3.01 3.43 3.86 3.95 3.06 3.25],怎么用MATLAB实现序列a的随机打乱序列b？

使用randperm(N)函数产生一个1到N的数组

譬如

A=randperm(length(a));

for i=1:length(a)

    b(i)=a(A(i));

end
            


