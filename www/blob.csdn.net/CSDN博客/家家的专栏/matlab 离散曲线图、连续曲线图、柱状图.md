# matlab 离散曲线图、连续曲线图、柱状图 - 家家的专栏 - CSDN博客





2013年04月11日 18:36:34[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4373








![](https://img-my.csdn.net/uploads/201304/11/1365672278_2259.png)

%曲线图code:

x=[1 2 3 4 5 ];

y1  =[ 57.9 75.7 76 80.2  97];

y4=[ 90.9 95 95 97.4  99];

y8 =[65.5 82 84 91.3 95];

plot(x,y1,'-.rv',x,y4,'--ks',x,y8,':mp');

legend('method1','method2','method3');

set(gca,'xtick', x_Matrix)

xlabel('number');

ylabel('Recognition rates(%)');



![](https://img-my.csdn.net/uploads/201304/11/1365680741_6802.png)

%连续曲线图

 x=[1 2 3 4 5 ];

y1  =[ 57.9 75.7 76 80.2  97];

y4=[ 90.9 95 95 97.4  99];

y8 =[65.5 82 84 91.3 95];

values1=spcrv([[x(1) x x(end)];[y1(1) y1 y1(end)]],3,1000);

values4=spcrv([[x(1) x x(end)];[y4(1) y4 y4(end)]],3,1000);

values8=spcrv([[x(1) x x(end)];[y8(1) y8 y8(end)]],3,1000);

plot(values1(1,:),values1(2,:),values4(1,:),values4(2,:),values8(1,:),values8(2,:)) ;



![](https://img-my.csdn.net/uploads/201304/11/1365680923_4566.png)

%柱状图code

y  =[ 57.9 75.7 76 80.2  97; 90.9 95 95 97.4  99;65.5 82 84 91.3 95];

bar(y);

legend('method1','method2','method3');

set(gca,'xtick', x_Matrix)

xlabel('number');

ylabel('Recognition rates(%)');



 如果一列或者一行数据是，如何画图，使得bar有不同的颜色。




clc;clear;


z=[8500;4200;8500;5800;4200;8500];


color=[4;2;3;1;5;6];


b=z; 


hold on


for i=1:length(z)


b(i)=bar(i,z(i));


ch = get(b(i),'children');


set(ch,'FaceVertexCData',color(i));


end


set(gca,'XTickLabel',{'    ','(Kma,Kfe)','(Kati,Kfe)','(Kati,Kco)','(Kmv,Kfe)','(Kati,Kmv)','(Kfe,Kco)'});


set(gcf,'color','white');


legend('Kma![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导','Kati![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导','Kco![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导','Kmv![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导','Kati![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导','Kfe![](http://zhidao.baidu.com/api/getdecpic?picenc=4070776569422a)主导');


ylabel('![](http://zhidao.baidu.com/api/getdecpic?picenc=40706368656e675f31422a)本（$）');

![](https://img-blog.csdn.net/20140911104403537)



转自：http://zhidao.baidu.com/link?url=fGxfFABiPluk2YOj1AnIqu0AEMnsjFrY7YnPcnXLvEzh5l7ZDTgyz8EgORvcTrdfNbJeblxFbtmo9NDGUn-ozgd5d6JOzEbmTATKRsKait3












