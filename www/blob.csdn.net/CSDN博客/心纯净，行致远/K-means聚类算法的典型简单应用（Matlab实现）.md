# K-means聚类算法的典型简单应用（Matlab实现） - 心纯净，行致远 - CSDN博客





2017年10月04日 20:55:31[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：988








%matlab code

% K-means Cluster

%load data.dat



%x,y的范围为0~50，x_data是一个1行100列的行矩阵

x_data = 50*rand(1,100);

y_data = 50*rand(1,100);



% x_data = data(:,1);表示data对应的横坐标

% y_data = data(:,2);表示data对应的纵坐标



data_size = length(x_data);

a = randsample(1:data_size,2);  %randsample函数表示在1~data_size范围内产生两个随机数

c1_x = x_data(a(1));           %将选定的第一个点的横坐标赋值给c1的x

c1_y = y_data(a(1));           %将选定的第一个点的纵坐标赋值给c1的y

c2_x = x_data(a(2));

c2_y = y_data(a(2));

iter = 2;             %用来表示点的大小

max_iter = 100;        %设定点大小程度的最大值

J_1 = 1;               %J_1表示误差平方和，用来评价聚类性能

J_2 = 1;

Figure



%下面的while循环是用来判断程序何时停止，当J_1或J_2两次的误差平方和小于0.1或者iter达到100，程序就会停止



while J_1>0.1 && J_2>0.1&& iter<max_iter

   c1_xx = 0;

   c1_yy = 0;

   c2_xx = 0;

   c2_yy = 0;  

   c1_num = 0;

   c2_num = 0; 

   for k=1:data_size

       Distance1 = (x_data(k)-c1_x)^2 + (y_data(k)-c1_y)^2 ;  %计算到类中心的距离

       Distance2 = (x_data(k)-c2_x)^2 + (y_data(k)-c2_y)^2 ;

       if Distance1 > Distance2      %将点k归到最近的类中

           lable(k) = 1;             %k距离c2比较近，就将其归到c2的类中

           c2_xx = c2_xx+x_data(k);

           c2_yy = c2_yy+y_data(k);

           c2_num = c2_num+1;       %统计归到类中的数据个数

       else

           lable(k) = 0;          %反之，归到c1的类中

           c1_xx = c1_xx+x_data(k);

           c1_yy = c1_yy+y_data(k);

           c1_num = c1_num+1;

       end

   end

   c1_xx = c1_xx/c1_num;     %计算c1,c2两个类中的坐标平均值来作为下一次类中心

   c1_yy = c1_yy/c1_num;

   c2_xx = c2_xx/c2_num;

c2_yy =c2_yy/c2_num;  



%计算本次和上次类中心的误差平方和作为评价阈值

J_1 =(c1_x-c1_xx)^2 + (c1_y-c1_yy)^2 ;

J_2 =(c2_x-c2_xx)^2 + (c2_y-c2_yy)^2 ;

   c1_x = c1_xx;   %将本次得到的新的类中心坐标赋给设定的类中心

   c1_y = c1_yy;

   c2_x = c2_xx;

   c2_y = c2_yy;

   iter = iter+3    %每循环一次将类中心点的大小加3,

   hold on

plot(c1_x,c1_y,'bp','MarkerSize',iter)   %MarkerSize表示点的大小，默认大小为6.0

hold on

   plot(c2_x,c2_y,'mp','MarkerSize',iter)

end



% figure

% plot(c1_x,c1_y,'kp','MarkerSize',iter+2)

% hold on

% plot(c2_x,c2_y,'rp','MarkerSize',iter+2)

for idx = 1:data_size

   if lable(idx) == 1

       hold on

       plot(x_data(idx),y_data(idx),'ro')

   else 

       hold on

       plot(x_data(idx),y_data(idx),'ko')

   end

end





