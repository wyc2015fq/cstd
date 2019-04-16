# Matlab高级教程_第四篇：白噪声的MATALB生成方式 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第四篇：白噪声的MATALB生成方式](https://www.cnblogs.com/noah0532/p/10188437.html)





1. 白噪声主要是高斯白噪声。

2. 为什么是高斯白噪声？

　　高斯白噪声：1）这个噪声它是一个随机信号。2）“白”是指其功率谱的常数，这样他的自相关函数是狄拉克函数（冲激函数），由于它的自相关函数是冲激函数，这说明信号只与它自己相关，它的时延信号就相关，也可以形象地说这种信号是“翻脸不认人”；功率谱是常数，人们形象的用白色光包含七彩光来比喻，这种频谱又称为“白谱”。3）“高斯”是指这个噪声信号的信号波形中幅度大小的分布满足高斯分布。

　　另外高斯分布的熵最大。

clear;clc;
temp =  []
temptemp = []

%--------------------------------
for i = 1:length(temp)
    y=randn(500,1) 

    y=y/std(y)

    y=y-mean(y)

    a=0

    b=sqrt(3)
    t1 = temp(i) +  b*y
    t2 = temp(i)  -  b*y

    temptemp(i,1) = (  max(t1)  )
     temptemp(i,2) = (  min(t2)  )
    clear y a b t1 t2 
end












