# Matlab 分类中confusion矩阵的计算 - 家家的专栏 - CSDN博客





2012年11月03日 20:26:48[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1580
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)









自己发现了。其实很简单，现在总结一下；

1，首先输入两个矩阵g1=[1 1 1 2 2 2 3 3 3 3]， g2=[1 1 2 2 2 2 3 1 3 3] 

    g1为正确值，g2为预测值；

2，然后利用C,order]=confusionmat(g1,g2) 命令就可以计算出来了。  


C =

     2     1     0

     0     3     0

     1     0     3


order =

     1

     2

     3


转自：[http://www.ilovematlab.cn/thread-82033-1-1.html](http://www.ilovematlab.cn/thread-82033-1-1.html)



如需转化为百分比，则：

temp=zeros(size(C,1),size(C,2));

for i=1:size(C,1)

    temp(i,:)=double(C(i,:))/double(sum(C(i,:)));

end



