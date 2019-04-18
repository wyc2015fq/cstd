# Plot小图模式代码 - 心纯净，行致远 - CSDN博客





2019年03月12日 19:09:47[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：26标签：[Plot																[论文																[小图](https://so.csdn.net/so/search/s.do?q=小图&t=blog)
个人分类：[【工程实践经验】](https://blog.csdn.net/zhanshen112/article/category/7684804)





```
%%
%绘制TTCxmin,TTCy,TTCxmax
x=0:4.5/100:4.5;
figure('color',[1 1 1]);    %将背景设置为白色
% set(gcf,'position',[400,400,800,550]); %设定figure属性，[left,buttom,width,height]
TTCxmin_plot=zeros(1,101);
TTCy_plot=zeros(1,101);
TTCxmax_plot=zeros(1,101);
for i=1:101
    TTCxmin_plot(i)=TTCxmin((i-1)*45+1);
    TTCy_plot(i)=TTCy((i-1)*45+1);
    TTCxmax_plot(i)=TTCxmax((i-1)*45+1);
end
plot(x,TTCxmin_plot,'marker','o','linewidth',2);
hold on
plot(x,TTCy_plot,'marker','.','linewidth',2);
plot(x,TTCxmax_plot,'marker','x','linewidth',2);
title('The TTC of Experiment 1','Fontsize',20,'Fontname', 'Times New Roman')
xlabel('Time unit: s','Fontsize', 20,'Fontname', 'Times New Roman');
ylabel('TTCxmin\TTCy\TTCxmax','Fontsize',20, 'Fontname', 'Times New Roman');
set(gca,'Fontsize', 20);
h3=legend({'TTCxmin','TTCy','TTCxmax'},'FontSize',20, 'Fontname', 'Times New Roman');
set(h3,'Box','off');
box off;



x=0:4.8/100:4.8;
figure('color',[1 1 1]);    %将背景设置为白色
% set(gcf,'position',[0,0,920,620]); %设定figure属性，[left,buttom,width,height]
SRFlateral_plot=zeros(1,101);
Dy_plot=zeros(1,101);
for i=1:101
    SRFlateral_plot(i)=SRFlateral((i-1)*48+1);
    Dy_plot(i)=Dy((i-1)*48+1);
end
plot(x,SRFlateral_plot,'linestyle','-','linewidth',2,'marker','o');
hold on
plot(x,Dy_plot,'linestyle','-','linewidth',2,'marker','x');
title('The Result of Experiment 1','Fontsize',20,'Fontname', 'Times New Roman')
xlabel('Time unit: s','Fontsize', 20,'Fontname', 'Times New Roman');
ylabel('SRFlateral/Dy unit:m','Fontsize',20, 'Fontname', 'Times New Roman');
set(gca,'Fontsize', 20);
h3=legend({'SRFlateral','Dy'},'FontSize',20, 'Fontname', 'Times New Roman');
set(h3,'Box','off');
box off;

save D:\我的研究\仿真结果\数据\Exp1SRFlateral.mat SRFlateral
```](https://so.csdn.net/so/search/s.do?q=论文&t=blog)](https://so.csdn.net/so/search/s.do?q=Plot&t=blog)




