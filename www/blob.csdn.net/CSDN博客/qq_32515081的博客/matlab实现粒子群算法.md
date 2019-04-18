# matlab实现粒子群算法 - qq_32515081的博客 - CSDN博客
2018年04月02日 19:05:07[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：1527

**转自：https://blog.csdn.net/nightmare_dimple/article/details/74331679**
**粒子群算法**（Particle Swarm Optimization，PSO）是20世纪90年代兴起的一门学科，因其概念简明、实现方便、收敛速度快而为人所知。粒子群算法的基本思想是模拟鸟群随机搜寻食物的捕食行为，鸟群通过自身经验和种群之间的交流调整自己的搜寻路径，从而找到食物最多的地点。其中每只鸟的**位置**/路径则为自变量组合，每次到达的地点的食物密度即函数值。每次搜寻都会根据**自身经验**（自身历史搜寻的最优地点）和**种群交流**（种群历史搜寻的最优地点）调整自身搜寻**方向**和**速度**，这个称为跟踪极值，从而找到最优解。
    粒子群算法是一门新兴算法，此算法与遗传算法有很多相似之处，其收敛于全局最优解的概率很大。
①相较于传统算法计算速度非常快，全局搜索能力也很强；
②PSO对于种群大小不十分敏感，所以初始种群设为500-1000，速度影响也不大；
③粒子群算法适用于**连续函数极值**问题，对于非线性、多峰问题均有较强的全局搜索能力。
    其流程图如下：
![](https://img-blog.csdn.net/20170704155030355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Step1:**确定一个粒子的运动状态是利用**位置**和**速度**两个参数描述的，因此初始化的也是这两个参数；
**Step2:**每次搜寻的结果（函数值）即为**粒子适应度**，然后记录每个粒子的**个体历史最优位置**和**群体的历史最优位置**；
**Step3:**个体历史最优位置和群体的历史最优位置相当于产生了两个力，结合粒子本身的**惯性**共同影响粒子的运动状态，由此来**更新粒子的位置和速度**。
    粒子群算法中所涉及到的参数有：
**种群数量**：粒子群算法的最大特点就是速度快，因此初始种群取50-1000都是可以的，虽然初始种群越大收敛性会更好，不过太大了也会影响速度；
**       迭代次数**：一般取100~4000，太少解不稳定，太多浪费时间。对于复杂问题，进化代数可以相应地提高；
**       惯性权重**：该参数反映了个体历史成绩对现在的影响，一般取0.5~1；
**学习因子**：一般取0~4，此处要根据自变量的取值范围来定，并且学习因子分为个体和群体两种；
**空间维数：**粒子搜索的空间维数即为自变量的个数。
**位置限制：**限制粒子搜索的空间，即自变量的取值范围，对于无约束问题此处可以省略。
**速度限制：**如果粒子飞行速度过快，很可能直接飞过最优解位置，但是如果飞行速度过慢，会使得收敛速度变慢，因此设置合理的速度限制就很有必要了。
      不同于遗传算法，粒子群算法**不需要编码**，直接利用粒子的位置来表示自变量，每个粒子的位置都由自变量的个数和取值范围决定，而速度由自变量的个数和速度限制决定，形式如下，其中d代表空间维数(自变量数）：
![](https://img-blog.csdn.net/20170704155734872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        下面我们用一个例子来帮助理解，对于函数 f=x*sin(x)*cos(2*x)-2*x*sin(3*x) ，求其在区间[0,20]上该函数的最大值。首先我们需要画出函数的图像，如下图：
![](https://img-blog.csdn.net/20170704160022128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       多峰问题对于算法的检验效果最佳，而上图显然是一个简单的非等距、非等高的多峰一元函数。
- **初始化种群**
已知位置限制[0,20],由于**一维问题**较为简单，因此可以取初始种群N 为50，迭代次数为100,当然空间维数d 也就是1。
**        位置和速度的初始化**即在位置和速度限制内随机生成一个N x d 的矩阵，对于此题，位置初始化也就是在0~20内随机生成一个50x1的数据矩阵，而对于速度则不用考虑约束，一般直接在0~1内随机生成一个50x1的数据矩阵。
        此处的**位置约束**也可以理解为位置限制，而**速度限制**是保证粒子步长不超限制的，一般设置速度限制为[-1,1]。
        粒子群的另一个特点就是**记录每个个体的历史最优和种群的历史最优**，因此而二者对应的最优位置和最优值也需要初始化。其中每个个体的历史最优位置可以先初始化为当前位置,而种群的历史最优位置则可初始化为原点。对于最优值，如果求最大值则初始化为负无穷，相反地初始化为正无穷。
       每次搜寻都需要将当前的适应度和最优解同历史的记录值进行对比，如果超过历史最优值，则更新个体和种群的历史最优位置和最优解。
       通过之前的参数设定可以得到如下的初始分布图：
![](https://img-blog.csdn.net/20170704160600418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- **速度与位置的更新**
        速度和位置更新是粒子群算法的核心，其原理表达式和更新方式如下:
![](https://img-blog.csdn.net/20170704160743071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170704160814253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       每次更新完速度和位置都需要考虑速度和位置的限制，需要将其限制在规定范围内，此处仅举出一个常规方法，即将超约束的数据约束到边界（当位置或者速度超出初始化限制时，将其拉回靠近的边界处）。当然，你不用担心他会停住不动，因为每个粒子还有惯性和其他两个参数的影响。
代码如下:
**[plain]**[view plain](https://blog.csdn.net/nightmare_dimple/article/details/74331679#)[copy](https://blog.csdn.net/nightmare_dimple/article/details/74331679#)[print](https://blog.csdn.net/nightmare_dimple/article/details/74331679#)[?](https://blog.csdn.net/nightmare_dimple/article/details/74331679#)
- clc;clear;close all;  
- %% 初始化种群  
- f= @(x)x .* sin(x) .* cos(2 * x) - 2 * x .* sin(3 * x); % 函数表达式  
- figure(1);ezplot(f,[0,0.01,20]);  
- N = 50;                         % 初始种群个数  
- d = 1;                          % 空间维数  
- ger = 100;                      % 最大迭代次数       
- limit = [0, 20];                % 设置位置参数限制  
- vlimit = [-1, 1];               % 设置速度限制  
- w = 0.8;                        % 惯性权重  
- c1 = 0.5;                       % 自我学习因子  
- c2 = 0.5;                       % 群体学习因子   
- for i = 1:d  
-     x = limit(i, 1) + (limit(i, 2) - limit(i, 1)) * rand(N, d);%初始种群的位置  
- end  
- v = rand(N, d);                  % 初始种群的速度  
- xm = x;                          % 每个个体的历史最佳位置  
- ym = zeros(1, d);                % 种群的历史最佳位置  
- fxm = zeros(N, 1);               % 每个个体的历史最佳适应度  
- fym = -inf;                      % 种群历史最佳适应度  
- hold on  
- plot(xm, f(xm), 'ro');title('初始状态图');  
- figure(2)  
- %% 群体更新  
- iter = 1;  
- record = zeros(ger, 1);          % 记录器  
- while iter <= ger  
-      fx = f(x) ; % 个体当前适应度     
-      for i = 1:N        
-         if fxm(i) < fx(i)  
-             fxm(i) = fx(i);     % 更新个体历史最佳适应度  
-             xm(i,:) = x(i,:);   % 更新个体历史最佳位置  
-         end   
-      end  
- if fym < max(fxm)  
-         [fym, nmax] = max(fxm);   % 更新群体历史最佳适应度  
-         ym = xm(nmax, :);      % 更新群体历史最佳位置  
-  end  
-     v = v * w + c1 * rand * (xm - x) + c2 * rand * (repmat(ym, N, 1) - x);% 速度更新  
-     % 边界速度处理  
-     v(v > vlimit(2)) = vlimit(2);  
-     v(v < vlimit(1)) = vlimit(1);  
-     x = x + v;% 位置更新  
-     % 边界位置处理  
-     x(x > limit(2)) = limit(2);  
-     x(x < limit(1)) = limit(1);  
-     record(iter) = fym;%最大值记录  
- %     x0 = 0 : 0.01 : 20;  
- %     plot(x0, f(x0), 'b-', x, f(x), 'ro');title('状态位置变化')  
- %     pause(0.1)  
-     iter = iter+1;  
- end  
- figure(3);plot(record);title('收敛过程')  
- x0 = 0 : 0.01 : 20;  
- figure(4);plot(x0, f(x0), 'b-', x, f(x), 'ro');title('最终状态位置')  
- disp(['最大值：',num2str(fym)]);  
- disp(['变量取值：',num2str(ym)]);  
```
clc;clear;close all;
%% 初始化种群
f= @(x)x .* sin(x) .* cos(2 * x) - 2 * x .* sin(3 * x); % 函数表达式
figure(1);ezplot(f,[0,0.01,20]);
N = 50;                         % 初始种群个数
d = 1;                          % 空间维数
ger = 100;                      % 最大迭代次数     
limit = [0, 20];                % 设置位置参数限制
vlimit = [-1, 1];               % 设置速度限制
w = 0.8;                        % 惯性权重
c1 = 0.5;                       % 自我学习因子
c2 = 0.5;                       % 群体学习因子 
for i = 1:d
    x = limit(i, 1) + (limit(i, 2) - limit(i, 1)) * rand(N, d);%初始种群的位置
end
v = rand(N, d);                  % 初始种群的速度
xm = x;                          % 每个个体的历史最佳位置
ym = zeros(1, d);                % 种群的历史最佳位置
fxm = zeros(N, 1);               % 每个个体的历史最佳适应度
fym = -inf;                      % 种群历史最佳适应度
hold on
plot(xm, f(xm), 'ro');title('初始状态图');
figure(2)
%% 群体更新
iter = 1;
record = zeros(ger, 1);          % 记录器
while iter <= ger
     fx = f(x) ; % 个体当前适应度   
     for i = 1:N      
        if fxm(i) < fx(i)
            fxm(i) = fx(i);     % 更新个体历史最佳适应度
            xm(i,:) = x(i,:);   % 更新个体历史最佳位置
        end 
     end
if fym < max(fxm)
        [fym, nmax] = max(fxm);   % 更新群体历史最佳适应度
        ym = xm(nmax, :);      % 更新群体历史最佳位置
 end
    v = v * w + c1 * rand * (xm - x) + c2 * rand * (repmat(ym, N, 1) - x);% 速度更新
    % 边界速度处理
    v(v > vlimit(2)) = vlimit(2);
    v(v < vlimit(1)) = vlimit(1);
    x = x + v;% 位置更新
    % 边界位置处理
    x(x > limit(2)) = limit(2);
    x(x < limit(1)) = limit(1);
    record(iter) = fym;%最大值记录
%     x0 = 0 : 0.01 : 20;
%     plot(x0, f(x0), 'b-', x, f(x), 'ro');title('状态位置变化')
%     pause(0.1)
    iter = iter+1;
end
figure(3);plot(record);title('收敛过程')
x0 = 0 : 0.01 : 20;
figure(4);plot(x0, f(x0), 'b-', x, f(x), 'ro');title('最终状态位置')
disp(['最大值：',num2str(fym)]);
disp(['变量取值：',num2str(ym)]);
```
结果如下：
![](https://img-blog.csdn.net/20170704161251872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    由上图可以看出算法已成功找出了最优解，其最优解为18.3014，而其最大值为32.1462。
    如果想看粒子群算法中粒子的搜索过程的可以将代码中注释掉的三行代码放上去。效果是这样的：
![](https://img-blog.csdn.net/20170704163140122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmlnaHRtYXJlX2RpbXBsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将粒子群的搜索过程可视化了，并将其转存为了gif格式文件：
1.首先pause()，是在每次绘图之后暂停一段时间，单位是秒，再进行下一次绘图；
2.而当要转存为gif文件时，这其实就是一种无声的视频文件，因此我们需要每一帧的图像，可以用pause，但是用drawnow更好，它的原理是保持当前窗口不变，继续下一次绘图。
      因此将转存为gif的步骤加入到之前的代码中就是：
**[plain]**[view plain](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[copy](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[print](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[?](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)
- clc;clear;close all;  
- %% 初始化种群  
- f= @(x)x .* sin(x) .* cos(2 * x) - 2 * x .* sin(3 * x); % 函数表达式  
- N = 50;                         % 初始种群个数  
- d = 1;                          % 空间维数  
- ger = 100;                      % 最大迭代次数       
- limit = [0, 20];                % 设置位置参数限制  
- vlimit = [-1, 1];               % 设置速度限制  
- w = 0.8;                        % 惯性权重  
- c1 = 0.5;                       % 自我学习因子  
- c2 = 0.5;                       % 群体学习因子   
- for i = 1:d  
-     x = limit(i, 1) + (limit(i, 2) - limit(i, 1)) * rand(N, d);%初始种群的位置  
- end  
- v = rand(N, d);                  % 初始种群的速度  
- xm = x;                          % 每个个体的历史最佳位置  
- ym = zeros(1, d);                % 种群的历史最佳位置  
- fxm = zeros(N, 1);               % 每个个体的历史最佳适应度  
- fym = -inf;                      % 种群历史最佳适应度  
- %% 群体更新  
- iter = 1;  
- record = zeros(ger, 1);          % 记录器  
- while iter <= ger  
-      fx = f(x) ; % 个体当前适应度     
-      for i = 1:N        
-         if fxm(i) < fx(i)  
-             fxm(i) = fx(i);     % 更新个体历史最佳适应度  
-             xm(i,:) = x(i,:);   % 更新个体历史最佳位置  
-         end   
-      end  
- if fym < max(fxm)  
-         [fym, nmax] = max(fxm);   % 更新群体历史最佳适应度  
-         ym = xm(nmax, :);      % 更新群体历史最佳位置  
-  end  
-     v = v * w + c1 * rand * (xm - x) + c2 * rand * (repmat(ym, N, 1) - x);% 速度更新  
-     % 边界速度处理  
-     v(v > vlimit(2)) = vlimit(2);  
-     v(v < vlimit(1)) = vlimit(1);  
-     x = x + v;% 位置更新  
-     % 边界位置处理  
-     x(x > limit(2)) = limit(2);  
-     x(x < limit(1)) = limit(1);  
-     record(iter) = fym;%最大值记录  
-     x0 = 0 : 0.01 : 20;  
-     plot(x0, f(x0), 'b-', x, f(x), 'ro');title('状态位置变化')  
-     drawnow  
-     frame = getframe(1);  
-     im = frame2im(frame);  
-     [A,map] = rgb2ind(im,256);   
-     if iter == 1;  
-         imwrite(A,map,'E:\培训\优化算法\pso.gif','gif','LoopCount',Inf,'DelayTime',0.1);  
-     else  
-         imwrite(A,map,'E:\培训\优化算法\pso.gif','gif','WriteMode','append','DelayTime',0.1);  
-     end  
-     iter = iter+1;  
- end  
- disp(['最大值：',num2str(fym)]);  
- disp(['变量取值：',num2str(ym)]);  
```
clc;clear;close all;
%% 初始化种群
f= @(x)x .* sin(x) .* cos(2 * x) - 2 * x .* sin(3 * x); % 函数表达式
N = 50;                         % 初始种群个数
d = 1;                          % 空间维数
ger = 100;                      % 最大迭代次数     
limit = [0, 20];                % 设置位置参数限制
vlimit = [-1, 1];               % 设置速度限制
w = 0.8;                        % 惯性权重
c1 = 0.5;                       % 自我学习因子
c2 = 0.5;                       % 群体学习因子 
for i = 1:d
    x = limit(i, 1) + (limit(i, 2) - limit(i, 1)) * rand(N, d);%初始种群的位置
end
v = rand(N, d);                  % 初始种群的速度
xm = x;                          % 每个个体的历史最佳位置
ym = zeros(1, d);                % 种群的历史最佳位置
fxm = zeros(N, 1);               % 每个个体的历史最佳适应度
fym = -inf;                      % 种群历史最佳适应度
%% 群体更新
iter = 1;
record = zeros(ger, 1);          % 记录器
while iter <= ger
     fx = f(x) ; % 个体当前适应度   
     for i = 1:N      
        if fxm(i) < fx(i)
            fxm(i) = fx(i);     % 更新个体历史最佳适应度
            xm(i,:) = x(i,:);   % 更新个体历史最佳位置
        end 
     end
if fym < max(fxm)
        [fym, nmax] = max(fxm);   % 更新群体历史最佳适应度
        ym = xm(nmax, :);      % 更新群体历史最佳位置
 end
    v = v * w + c1 * rand * (xm - x) + c2 * rand * (repmat(ym, N, 1) - x);% 速度更新
    % 边界速度处理
    v(v > vlimit(2)) = vlimit(2);
    v(v < vlimit(1)) = vlimit(1);
    x = x + v;% 位置更新
    % 边界位置处理
    x(x > limit(2)) = limit(2);
    x(x < limit(1)) = limit(1);
    record(iter) = fym;%最大值记录
    x0 = 0 : 0.01 : 20;
    plot(x0, f(x0), 'b-', x, f(x), 'ro');title('状态位置变化')
    drawnow
    frame = getframe(1);
    im = frame2im(frame);
    [A,map] = rgb2ind(im,256); 
	if iter == 1;
		imwrite(A,map,'E:\培训\优化算法\pso.gif','gif','LoopCount',Inf,'DelayTime',0.1);
	else
		imwrite(A,map,'E:\培训\优化算法\pso.gif','gif','WriteMode','append','DelayTime',0.1);
	end
    iter = iter+1;
end
disp(['最大值：',num2str(fym)]);
disp(['变量取值：',num2str(ym)]);
```
         另外，大家可以看到，种群历史最优值处我们初始化为了-inf，这是因为我们需要求最大值。但是如果要求最小值应该怎么办呢？最好不要将这里改成+inf，如果这样做，我们后面的最优值更新中全部要改成最小值和<，太麻烦。那么最好的方式就是直接在适应度处加上负号，这样的话就相当于求最小值了，只不过要将最后的最优值取反。 
      并且我们之前所写的只是一维问题优化，代码优化程度不高，对此我给出一份优化程度较高的代码，以应对多维优化问题：
      问题是求21.5+x*sin(4*pi*x)+y*sin(20*pi*y)，x=[-3,12.1]；y=[4.1,5.8]的极值。
**[plain]**[view plain](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[copy](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[print](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[?](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)
- clc;clear;close all;tic  
- %% 参数设定  
- N = 500;  
- d = 2;  
- ger = 1000;  
- w = 0.8;  
- c1 = 0.9;  
- c2 = 0.9;  
- xlimit = [-3 12.1;4.1 5.8];  
- vlimit = [-1 1;-1 1];  
- %% 种群初始化  
- x = repmat(xlimit(:,1)',N,1)+repmat(diff(xlimit'),N,1).*rand(N,d);  
- v = repmat(vlimit(:,1)',N,1)+repmat(diff(vlimit'),N,1).*rand(N,d);  
- xm = x;  
- fxm = -inf*ones(N,1);  
- ym = xlimit(:,1)'+diff(xlimit').*rand(1,d);  
- fym = -inf;  
- %% 开始搜索  
- for i = 1 : ger  
-     y = f(x);  
-     for j = 1 : N  
-         if y(j)>fxm(j)  
-             fxm(j)=y(j);  
-             xm(j,:) = x(j,:);  
-             if y(j)>fym  
-                 fym = y(j);  
-                 ym = x(j,:);  
-             end  
-         end  
-     end  
-     v = w*v+c1*rand*(xm-x)+c2*rand*(repmat(ym,N,1)-x);  
-     x = x+v;  
-     x = min(x,repmat(xlimit(:,2)',N,1));  
-     x = max(x,repmat(xlimit(:,1)',N,1));  
-     v = min(v,repmat(vlimit(:,2)',N,1));  
-     v = max(v,repmat(vlimit(:,1)',N,1));  
- end  
- toc  
- disp(['最优解为:',num2str(ym)]);  
- disp(['最优值为:',num2str(fym)]);  
```
clc;clear;close all;tic
%% 参数设定
N = 500;
d = 2;
ger = 1000;
w = 0.8;
c1 = 0.9;
c2 = 0.9;
xlimit = [-3 12.1;4.1 5.8];
vlimit = [-1 1;-1 1];
%% 种群初始化
x = repmat(xlimit(:,1)',N,1)+repmat(diff(xlimit'),N,1).*rand(N,d);
v = repmat(vlimit(:,1)',N,1)+repmat(diff(vlimit'),N,1).*rand(N,d);
xm = x;
fxm = -inf*ones(N,1);
ym = xlimit(:,1)'+diff(xlimit').*rand(1,d);
fym = -inf;
%% 开始搜索
for i = 1 : ger
    y = f(x);
    for j = 1 : N
        if y(j)>fxm(j)
            fxm(j)=y(j);
            xm(j,:) = x(j,:);
            if y(j)>fym
                fym = y(j);
                ym = x(j,:);
            end
        end
    end
    v = w*v+c1*rand*(xm-x)+c2*rand*(repmat(ym,N,1)-x);
    x = x+v;
    x = min(x,repmat(xlimit(:,2)',N,1));
    x = max(x,repmat(xlimit(:,1)',N,1));
    v = min(v,repmat(vlimit(:,2)',N,1));
    v = max(v,repmat(vlimit(:,1)',N,1));
end
toc
disp(['最优解为:',num2str(ym)]);
disp(['最优值为:',num2str(fym)]);
```
子函数为： 
**[plain]**[view plain](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[copy](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[print](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)[?](https://blog.csdn.net/nightmare_dimple/article/details/74332503#)
- function y = f(x)  
- y = 21.5+x(:,1).*sin(4*pi*x(:,1))+x(:,2).*sin(20*pi*x(:,2));  
- end  
