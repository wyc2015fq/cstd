# 优化算法——粒子群算法(PSO) - YZXnuaa的博客 - CSDN博客
2018年03月21日 16:56:03[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：275
# 一、粒子群算法的概述
    粒子群算法(PSO)属于群智能算法的一种，是通过模拟鸟群捕食行为设计的。假设区域里就只有一块食物（即通常优化问题中所讲的最优解），鸟群的任务是找到这个食物源。鸟群在整个搜寻的过程中，通过相互传递各自的信息，让其他的鸟知道自己的位置，通过这样的协作，来判断自己找到的是不是最优解，同时也将最优解的信息传递给整个鸟群，最终，整个鸟群都能聚集在食物源周围，即我们所说的找到了最优解，即问题收敛。
# 二、粒子群算法的流程
    粒子群算法通过设计一种无质量的粒子来模拟鸟群中的鸟，粒子仅具有两个属性：速度![](http://latex.codecogs.com/gif.latex?V)和位置![](http://latex.codecogs.com/gif.latex?X)，速度代表移动的快慢，位置代表移动的方向。每个粒子在搜索空间中单独的搜寻最优解，并将其记为当前个体极值![](http://latex.codecogs.com/gif.latex?P_%7Bbest%7D)，并将个体极值与整个粒子群里的其他粒子共享，找到最优的那个个体极值作为整个粒子群的当前全局最优解![](http://latex.codecogs.com/gif.latex?G_%7Bbest%7D)，粒子群中的所有粒子根据自己找到的当前个体极值![](http://latex.codecogs.com/gif.latex?P_%7Bbest%7D)和整个粒子群共享的当前全局最优解![](http://latex.codecogs.com/gif.latex?G_%7Bbest%7D)来调整自己的速度和位置。粒子群算法的思想相对比较简单，主要分为：1、初始化粒子群；2、评价粒子，即计算适应值；3、寻找个体极值![](http://latex.codecogs.com/gif.latex?P_%7Bbest%7D)；4、寻找全局最优解![](http://latex.codecogs.com/gif.latex?G_%7Bbest%7D)；5、修改粒子的速度和位置。下面是程序的流程图：
![](https://img-blog.csdn.net/20140611131036890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（PSO流程）
下面我们具体解释下流程图里面的每一个步骤：
## 1、初始化
   首先，我们需要设置最大的速度区间，防止超出最大的区间。位置信息即为整个搜索空间，我们在速度区间和搜索空间上随机初始化速度和位置。设置群体规模![](http://latex.codecogs.com/gif.latex?m)。
## 2、个体极值与全局最优解
   个体极值为每个粒子找到的历史上最优的位置信息，并从这些个体历史最优解中找到一个全局最优解，并与历史最优解比较，选出最佳的作为当前的历史最优解。
## 3、更新速度和位置的公式
   更新公式为：
![](http://latex.codecogs.com/gif.latex?V_%7Bid%7D&space;=&space;%5Comega&space;V_%7Bid%7D&space;+&space;C_1random(0,1)%5Cleft&space;(&space;P_%7Bid%7D-X%7Bid%7D&space;%5Cright&space;)+C_2random(0,1)%5Cleft&space;(&space;P_%7Bgd%7D-X_%7Bid%7D&space;%5Cright&space;))
![](http://latex.codecogs.com/gif.latex?X_%7Bid%7D&space;=&space;X_%7Bid%7D+V_%7Bid%7D)
其中，![](http://latex.codecogs.com/gif.latex?%5Comega)称为惯性因子，![](http://latex.codecogs.com/gif.latex?C_1)和![](http://latex.codecogs.com/gif.latex?C_2)称为加速常数，一般取![](http://latex.codecogs.com/gif.latex?C_1=C_2%5Cin%5Cleft&space;%5B&space;0,4&space;%5Cright&space;%5D)。![](http://latex.codecogs.com/gif.latex?random(0,1))表示区间![](http://latex.codecogs.com/gif.latex?%5Cleft&space;%5B&space;0,1&space;%5Cright&space;%5D)上的随机数。![](http://latex.codecogs.com/gif.latex?P_%7Bid%7D)表示第![](http://latex.codecogs.com/gif.latex?i)个变量的个体极值的第![](http://latex.codecogs.com/gif.latex?d)维。![](http://latex.codecogs.com/gif.latex?P_%7Bgd%7D)表示全局最优解的第![](http://latex.codecogs.com/gif.latex?d)维。
## 4、终止条件
有两种终止条件可以选择，一是最大代数：![](http://latex.codecogs.com/gif.latex?G_%7Bmax%7D)；二是相邻两代之间的偏差在一个指定的范围内即停止。我们在实验中选择第一种。
# 三、实验
    我们选择的测试函数是：Griewank。其基本形式如下：
![](http://latex.codecogs.com/gif.latex?min&space;f%5Cleft&space;(&space;x_i&space;%5Cright&space;)=%5Csum_%7Bi&space;=&space;1%7D%5E%7BN%7D%5Cfrac%7Bx_i%5E2%7D%7B4000%7D-%5Cprod_%7Bi=1%7D%5E%7BN%7Dcos%5Cleft&space;(&space;%5Cfrac%7Bx_i%7D%7B%5Csqrt%7Bi%7D%7D&space;%5Cright&space;)+1)
图像为：
![](https://img-blog.csdn.net/20140611134514843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（Griewank函数图像）
在实验中我们选择的维数是20；MATLAB程序代码如下：
主程序：
**[plain]**[view plain](http://blog.csdn.net/google19890102/article/details/30044945#)[copy](http://blog.csdn.net/google19890102/article/details/30044945#)
- c1=2;%学习因子  
- c2=2;%学习因子  
- Dimension=20;  
- Size=30;  
- Tmax=500;  
- Velocity_max=1200;%粒子最大速度  
- 
- F_n=2;%测试函数名  
- 
- Fun_Ub=600;%函数上下界  
- Fun_Lb=-600;  
- Position=zeros(Dimension,Size);%粒子位置  
- Velocity=zeros(Dimension,Size);%粒子速度  
- Vmax(1:Dimension)=Velocity_max;%粒子速度上下界  
- Vmin(1:Dimension)=-Velocity_max;  
- Xmax(1:Dimension)=Fun_Ub;%粒子位置上下界，即函数自变量的上下界  
- Xmin(1:Dimension)=Fun_Lb;  
- [Position,Velocity]=Initial_position_velocity(Dimension,Size,Xmax,Xmin,Vmax,Vmin);  
- 
- Pbest_position=Position;%粒子的历史最优位置，初始值为粒子的起始位置，存储每个粒子的历史最优位置  
- Gbest_position=zeros(Dimension,1);%全局最优的那个粒子所在位置，初始值认为是第1个粒子  
- 
- for j=1:Size  
-     Pos=Position(:,j);%取第j列，即第j个粒子的位置  
-     fz(j)=Fitness_Function(Pos,F_n,Dimension);%计算第j个粒子的适应值  
- end  
- [Gbest_Fitness,I]=min(fz);%求出所有适应值中最小的那个适应值，并获得该粒子的位置  
- Gbest_position=Position(:,I);%取最小适应值的那个粒子的位置，即I列  
- 
- for itrtn=1:Tmax  
- time(itrtn)=itrtn;  
- 
- Weight=1;  
- r1=rand(1);  
- r2=rand(1);  
- for i=1:Size  
-    Velocity(:,i)=Weight*Velocity(:,i)+c1*r1*(Pbest_position(:,i)-Position(:,i))+c2*r2*(Gbest_position-Position(:,i));  
- end  
- %限制速度边界  
- for i=1:Size  
-     for row=1:Dimension  
-         if Velocity(row,i)>Vmax(row)  
-             Veloctity(row,i)=Vmax(row);  
-         elseif Velocity(row,i)<Vmin(row)  
-             Veloctity(row,i)=Vmin(row);  
-         else  
-         end  
-     end  
- end  
- 
- Position=Position+Velocity;  
- 
- %限制位置边界  
- for i=1:Size  
-     for row=1:Dimension  
-         if Position(row,i)>Xmax(row)  
-             Position(row,i)=Xmax(row);  
-         elseif Position(row,i)<Xmin(row)  
-             Position(row,i)=Xmin(row);  
-         else  
-         end  
-     end  
- end  
- 
-   for j=1:Size  
-      P_position=Position(:,j)';%取一个粒子的位置  
-      fitness_p(j)=Fitness_Function(P_position,F_n,Dimension);  
-      if fitness_p(j)< fz(j) %粒子的适应值比运动之前的适应值要好，更新原来的适应值  
-          Pbest_position(:,j)=Position(:,j);  
-          fz(j)=fitness_p(j);  
-      end  
-      if fitness_p(j)<Gbest_Fitness  
-          Gbest_Fitness=fitness_p(j);  
-      end  
-   end  
-   [Gbest_Fitness_new,I]=min(fz);%更新后的所有粒子的适应值，取最小的那个，并求出其编号  
-    Best_fitness(itrtn)=Gbest_Fitness_new; %记录每一代的最好适应值  
-    Gbest_position=Pbest_position(:,I);%最好适应值对应的个体所在位置  
- end  
- plot(time,Best_fitness);  
- xlabel('迭代的次数');ylabel('适应度值P_g');  
初始化：
**[plain]**[view plain](http://blog.csdn.net/google19890102/article/details/30044945#)[copy](http://blog.csdn.net/google19890102/article/details/30044945#)
- function [Position,Velocity] = Initial_position_velocity(Dimension,Size,Xmax,Xmin,Vmax,Vmin)  
-   for i=1:Dimension  
-       Position(i,:)=Xmin(i)+(Xmax(i)-Xmin(i))*rand(1,Size); % 产生合理范围内的随机位置，rand(1,Size)用于产生一行Size个随机数  
-       Velocity(i,:)=Vmin(i)+(Vmax(i)-Vmin(i))*rand(1,Size);  
-   end  
- end  
适应值计算：
**[plain]**[view plain](http://blog.csdn.net/google19890102/article/details/30044945#)[copy](http://blog.csdn.net/google19890102/article/details/30044945#)
- function Fitness=Fitness_Function(Pos,F_n,Dimension)  
-  switch F_n  
-     case 1  
-         Func_Sphere=Pos(:)'*Pos(:);  
-         Fitness=Func_Sphere;  
-     case 2  
-         res1=Pos(:)'*Pos(:)/4000;  
-         res2=1;  
-         for row=1:Dimension  
-             res2=res2*cos(Pos(row)/sqrt(row));  
-         end  
-         Func_Griewank=res1-res2+1;  
-         Fitness=Func_Griewank;  
- end  
最终的收敛曲线：
![](https://img-blog.csdn.net/20140611134709343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ29vZ2xlMTk4OTAxMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（收敛曲线）
