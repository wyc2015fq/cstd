# Sigmund的99行Matlab拓扑优化程序简析 - cocoonyang的专栏 - CSDN博客





2018年05月29日 12:34:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：3118








# 引言

Sigmund在2001年在Structural and Multidisciplinary Optimization 发表一篇名为 “A 99 line topology optimization code written in Matlab”论文。该论文附带一个使用Matlab编写的拓扑优化程序。这个只有99行代码的程序基于Matlab环境构建了一个完整的拓扑优化流程：前处理(构建有限元仿真模型), 有限元模型分析计算,拓扑优化迭代和后处理(分析结果显示)。Sigmund在论文中对这个程序的拓扑优化流程做了详细解释。本文仅从程序设计角度解析这段代码。

Sigmund的99行Matlab拓扑优化程序使用模块化方法设计，主要包含以下几个模块： 

 - 程序主流程 

 - 有限元模型求解模块 

 - Filter模块 

 - 单元刚度阵计算模块：计算平面四边形单元的刚度矩阵 

 - 优化模块： 使用优化准则法更新设计变量 

 - 目标函数计算和灵敏度分析模块 

下面几个小节首先列出Sigmund99行拓扑优化程序完整代码，接着解释程序中5个全局变量在优化流程中的功用，然后对程序的各个功能模块做详细解析，最后总结程序的局限性并列出扩展程序功能时所需要修改的相应模块。
# 99行拓扑优化代码

Sigmund的99行Matlab拓扑优化程序如下所示

```matlab
% a 99 line topology optimization code by Ole Sigmund,October 1999 
clear 
nelx=60; 
nely=40; 
volfrac=0.5; 
penal=3.; 
rmin=1.5; 
% initialize 
x(1:nely,1:nelx)=volfrac; 
loop=0; 
change=1; 
% start ineration 
while change>0.01 
    loop=loop+1; 
    xold=x; 
    % FE analysis 
    [U]=FE(nelx,nely,x,penal); 
    % objective function and sensitivity analysis 
    [KE]=lk;; 
    c=0.; 
    for ely=1:nely 
        for elx=1:nelx 
            n1=(nely+1)*(elx-1)+ely; 
            n2=(nely+1)*elx    +ely; 
            Ue=U([2*n1-1;2*n1;2*n2-1;2*n2;2*n2+1;2*n2+2;2*n1+1;2*n1+2],1); 
            c=c+x(ely,elx)^penal*Ue'*KE*Ue; 
            dc(ely,elx)=-penal*x(ely,elx)^(penal-1)*Ue'*KE*Ue; 
        end 
    end 
    % filtering of sensitivities 
    [dc]=check(nelx,nely,rmin,x,dc); 
    % design update by the optimality criteria method 
    [x]=oc(nelx,nely,x,volfrac,dc); 
    % print result 
    change=max(max(x-xold)) 
    disp(['It.:' sprintf( '%4i',loop) '    Obj.:' sprintf('   %10.4f',c) ... 
        '   Vol.:' sprintf('%6.3f',sum(sum(x))/(nelx*nely)) ... 
        '   ch.:' sprintf('%6.3f',change)]) 
    % plot densities 
    colormap(gray);imagesc(-x);axis equal;axis tight; axis off;pause(1e-6); 
end 

% FE analysis 
function [U]=FE(nelx,nely,x,penal) 
[KE]=lk; 
K=sparse(2*(nelx+1)*(nely+1),2*(nelx+1)*(nely+1)); 
F=sparse(2*(nely+1)*(nelx+1),1);
U=sparse(2*(nely+1)*(nelx+1),1); 
for elx=1:nelx 
    for ely=1:nely 
        n1=(nely+1)*(elx-1)+ely; 
        n2=(nely+1)*elx    +ely; 
        edof=[2*n1-1;2*n1;2*n2-1;2*n2;2*n2+1;2*n2+2;2*n1+1;2*n1+2]; 
        K(edof,edof)=K(edof,edof)+x(ely,elx)^penal*KE; 
    end 
end 
% define loads and supports 
 ip=(nelx+1)*(nely+1); 
 F(2*ip,1)=-1;               

fixeddofs =[1:2*(nely+1)]; 
alldofs   =[1:2*(nely+1)*(nelx+1)]; 
freedofs  =setdiff(alldofs,fixeddofs); 
% solving 
U(freedofs,:)=K(freedofs,freedofs)\F(freedofs,:); 
U(fixeddofs,:)=0;

% mesh-independency filter 
function [dcn]=check(nelx,nely,rmin,x,dc) 
dcn=zeros(nely,nelx); 
for i=1:nelx 
    for j=1:nely 
        sum=0.0; 
        for k=max(i-floor(rmin),1):min(i+floor(rmin),nelx) 
            for l=max(j-floor(rmin),1):min(j+floor(rmin),nely) 
                fac=rmin-sqrt((i-k)^2+(j-l)^2); 
                sum=sum+max(0,fac); 
                dcn(j,i)=dcn(j,i)+max(0,fac)*x(l,k)*dc(l,k); 
            end 
        end 
        dcn(j,i)=dcn(j,i)/(x(j,i)*sum); 
    end 
end

% Element stiffness matrix 
function [KE]=lk 
E=1.; 
nu=0.3; 
k=[1/2-nu/6 1/8+nu/8 -1/4-nu/12 -1/8+3*nu/8 ... 
    -1/4+nu/12 -1/8-nu/8 nu/6   1/8-3*nu/8]; 
KE=E/(1-nu^2)*[ k(1)  k(2)  k(3)  k(4)  k(5)  k(6)  k(7)  k(8)   
                k(2)  k(1)  k(8)  k(7)  k(6)  k(5)  k(4)  k(3) 
                k(3)  k(8)  k(1)  k(6)  k(7)  k(4)  k(5)  k(2) 
                k(4)  k(7)  k(6)  k(1)  k(8)  k(3)  k(2)  k(5) 
                k(5)  k(6)  k(7)  k(8)  k(1)  k(2)  k(3)  k(4) 
                k(6)  k(5)  k(4)  k(3)  k(2)  k(1)  k(8)  k(7) 
                k(7)  k(4)  k(5)  k(2)  k(3)  k(8)  k(1)  k(6) 
                k(8)  k(3)  k(2)  k(5)  k(4)  k(7)  k(6)  k(1)];

% optimality criteria update 
function [xnew]=oc(nelx,nely,x,volfrac,dc) 
l1=0;
l2=100000;
move=0.2; 
while (l2-l1>1e-4) 
    lmid=0.5*(l2+l1); 
    xnew =max(0.001,max(x-move,min(1.,min(x+move,x.*sqrt(-dc./lmid))))); 
    if sum(sum(xnew))-volfrac*nelx*nely>0; 
        l1=lmid; 
    else 
        l2=lmid; 
    end 
end
```

# 变量初始化

```
nelx=60;   % x轴方向单元数量
nely=40;   % y轴方向单元数量
volfrac=0.5;  % 单元的材料体积比 
penal=3.;  %  惩罚因子，将设计变量从[0,1]转化为指数函数，
           %  主要目的是使单元材料密度更加"黑白分明", 
           %  惩罚因子就是这个指数函数的指数, 其值越大，效果越明显 
rmin=1.5;  % 过滤半径，目的是预防出现棋盘格现象
```

设计变量初始化
`x(1:nely,1:nelx)=volfrac;  % 所有设计变量的赋初始值 `
# 主程序

主程序迭代流程

```matlab
% start ineration 
while change>0.01 
    loop=loop+1;  % loop是迭代计数
    xold=x;       % 将设计变量保存在 变量xold中
    % FE analysis 
    [U]=FE(nelx,nely,x,penal);  % 有限元模型分析，将计算得到的各个节点位移值保存在数组U中 
    % objective function and sensitivity analysis 
    [KE]=lk;   % 计算单元刚度阵 
    c=0.;      % 保存目标函数(柔度)的变量，
    %  遍历所有单元    
    for ely=1:nely 
        for elx=1:nelx 
            % 根据单元在设计域中位置 计算单元节点编号
            n1=(nely+1)*(elx-1)+ely;  % 左上角节点编号  
            n2=(nely+1)*elx    +ely;  % 右上角节点编号  
            % 根据以上两个节点的编号 推演出 单元所有节点的自由度编号
            % 这就限制了此程序只能使用四边形单元剖分四边设计域所得到的有限元模型
            %
            % 从数组 U 中根据节点自由度编号 提取该单元的 位移向量 
            Ue=U([2*n1-1;2*n1;2*n2-1;2*n2;2*n2+1;2*n2+2;2*n1+1;2*n1+2],1);
            %  计算单元柔度，叠加到目标函数变量中 
            %  使用SIMP(Solid Isotropic Material with Penalization)材料插值模型
            c=c+x(ely,elx)^penal*Ue'*KE*Ue; 
            %  计算灵敏度  
            dc(ely,elx)=-penal*x(ely,elx)^(penal-1)*Ue'*KE*Ue; 
        end 
    end 
    % filtering of sensitivities 
    [dc]=check(nelx,nely,rmin,x,dc); 
    % design update by the optimality criteria method 
    [x]=oc(nelx,nely,x,volfrac,dc); 
    % print result 
    change=max(max(x-xold)) 
    disp(['It.:' sprintf( '%4i',loop) '    Obj.:' sprintf('   %10.4f',c) ... 
        '   Vol.:' sprintf('%6.3f',sum(sum(x))/(nelx*nely)) ... 
        '   ch.:' sprintf('%6.3f',change)]) 
    % plot densities 
    colormap(gray);imagesc(-x);axis equal;axis tight; axis off;pause(1e-6); 
end
```

优化模型为：



$ \left\{ \begin{array}{c c c l } min & c(x) & = & U^{T}KU = \sum_{e = 1}^{n} (x_{e})^{p} u_{e}^{T}K_{e} u_{e}   \\ s.t. & & \\  & \frac{V_{\rho}}{V} & = & f  \\  & KU & = & F  \\  & x_{min} & < & x < 1        \\ \end{array} \right.$

代码 x(ely,elx)^penal*Ue’*KE*Ue 就是计算 $(x_{e})^{p} u_{e}^{T}K_{e} u_{e} $

对$x_{e}$ 的梯度是 


$\frac{dc}{dx} = - p (x_{e})^{p-1} u_{e}^{T}K_{e} u_{e} $

此程序使用的SIMP(Solid Isotropic Material with Penalization)材料插值模型，具体代码为: 

 x(ely,elx)^penal ; 
网格划分为

```
1-------(nely+1)+1------------------------------------nely+1)*nelx
|  (1,1)  |
2-------(nely+1)+2
|              
...     -----------(nely+1)*(elx-1)+ely----(nely+1)*elx+ely    
|                            |       (elx,ely)      |
...
|
nely+1---(nely+1)*2----------
```

单元节点编号顺序为：

```
Node_1             Node_2
(nely+1)*(elx-1)+ely----(nely+1)*elx+ely    
      |       (elx,ely)      |
(nely+1)*(elx-1)+ely+1----(nely+1)*elx+ely+1  
      Node_4                Node_3
```

# 过滤器模块

```
%%%%%%%%%% MESH-INDEPENDENCY FILTER %%%%%%%%%%%%%%%%%%%
function [dcn]=check(nelx,nely,rmin,x,dc)
% dcn 清零，dcn 用来保存 更新的 目标函数灵敏度
dcn=zeros(nely,nelx);
% 遍历所有单元
for i = 1:nelx
  for j = 1:nely
    sum=0.0;
    % 遍历于这个单元相邻的单元
    for k = max(i-floor(rmin),1):min(i+floor(rmin),nelx)
      for l = max(j-floor(rmin),1):min(j+floor(rmin),nely)

        %  sqrt((i-k)^2+(j-l)^2) 是计算此单元与相邻单元的距离
        fac = rmin-sqrt((i-k)^2+(j-l)^2);
        sum = sum + max(0,fac);

        dcn(j,i) = dcn(j,i) + max(0,fac)*x(l,k)*dc(l,k);
      end
end

    dcn(j,i) = dcn(j,i)/(x(j,i)*sum);
  end
end
```

Sigmund的论文中对此函数有详细解释，论文中符号与代码中变量名的对应关系如下表所示:
|变量|论文中符号|
|----|----|
|rmin|$r_{min}$|
|fac|$H_{f}$|
|dc|$ \frac{c}{x_{e}} $|
|dcn|$\bar {  \frac{c}{x_{e}} } $|

# 优化迭代模块

```
% optimality criteria update 
function [xnew]=oc(nelx,nely,x,volfrac,dc) 
l1=0;
l2=100000;
move=0.2; 
while (l2-l1>1e-4) 
    lmid=0.5*(l2+l1); 
    xnew =max(0.001,max(x-move,min(1.,min(x+move,x.*sqrt(-dc./lmid))))); 
    if sum(sum(xnew))-volfrac*nelx*nely>0; 
        l1=lmid; 
    else 
        l2=lmid; 
    end 
end
```

这段代码就是实现下面这个方程式: 


$x_{e}^{new} =\left \{\begin{array}{ c c c c l}max(x_{min}, x_{e} - m) & if &                        &      & x_{e} B_{e}^{\eta} \leq max(x_{min}, x_{e}- m)  \\x_{e} B_{e}^{\eta}      & if & max(x_{min}, x_{e}- m) & \leq & x_{e} B_{e}^{\eta} \leq min(1, x_{e}+m ) \\min(1, x_{e}+m )        & if &   min(1, x_{e}+m )     & \leq & x_{e} B_{e}^{\eta} \\\end{array}\right  .$

# 有限元模型求解模块

```matlab
% FE analysis 
function [U]=FE(nelx,nely,x,penal) 
[KE]=lk; %计算单元刚度矩阵  
K=sparse(2*(nelx+1)*(nely+1),2*(nelx+1)*(nely+1)); 
F=sparse(2*(nely+1)*(nelx+1),1);
U=sparse(2*(nely+1)*(nelx+1),1); 
%组装整体刚度矩阵  
for elx=1:nelx 
    for ely=1:nely 
        n1=(nely+1)*(elx-1)+ely; 
        n2=(nely+1)*elx    +ely; 
        edof=[2*n1-1;2*n1;2*n2-1;2*n2;2*n2+1;2*n2+2;2*n1+1;2*n1+2]; 
        K(edof,edof)=K(edof,edof)+x(ely,elx)^penal*KE; 
    end 
end 
% define loads and supports 
 ip=(nelx+1)*(nely+1); 
 F(2*ip,1)=-1;  % 施加载荷              

% 施加位移约束  
fixeddofs =[1:2*(nely+1)]; 
alldofs   =[1:2*(nely+1)*(nelx+1)]; 
% 所有不受约束的节点自由度 
freedofs  =setdiff(alldofs,fixeddofs); 
% solving 求解线性方程组, 得到节点自由度的位移值
U(freedofs,:)=K(freedofs,freedofs)\F(freedofs,:); 
U(fixeddofs,:)=0;  % 受约束的节点自由度的位移值 设为  0
```

# 单元刚度阵模块

```
% Element stiffness matrix 
function [KE]=lk 
E=1.; 
nu=0.3; 
k=[1/2-nu/6 1/8+nu/8 -1/4-nu/12 -1/8+3*nu/8 ... 
    -1/4+nu/12 -1/8-nu/8 nu/6   1/8-3*nu/8]; 
KE=E/(1-nu^2)*[ k(1)  k(2)  k(3)  k(4)  k(5)  k(6)  k(7)  k(8)   
                k(2)  k(1)  k(8)  k(7)  k(6)  k(5)  k(4)  k(3) 
                k(3)  k(8)  k(1)  k(6)  k(7)  k(4)  k(5)  k(2) 
                k(4)  k(7)  k(6)  k(1)  k(8)  k(3)  k(2)  k(5) 
                k(5)  k(6)  k(7)  k(8)  k(1)  k(2)  k(3)  k(4) 
                k(6)  k(5)  k(4)  k(3)  k(2)  k(1)  k(8)  k(7) 
                k(7)  k(4)  k(5)  k(2)  k(3)  k(8)  k(1)  k(6) 
                k(8)  k(3)  k(2)  k(5)  k(4)  k(7)  k(6)  k(1)];
```

4节点矩形单元的4个节点在单元局部坐标系中坐标分别为：
|Node|x|y|
|----|----|----|
|$(x_{1}, y_{1})$|0|0|
|$(x_{2}, y_{2})$|1|0|
|$(x_{3}, y_{3})$|1|1|
|$(x_{4}, y_{4})$|0|1|

各节点形函数设为 


$\left \{    \begin{array}{c c c c c} N_{1}  & =   &  (x - 1 ) (y - 1)  \\   N_{2}  & =   & - x (y - 1)  \\  N_{3}  & =  &  x y  \\ N_{4}  & =  &  -(x - 1 ) y  \\    \end{array}  \right .$

以各节点形函数为基函数构建线性单元的位移场。单元内任一点$(x,y)$ 处的位移可由四个节点的变形通过单元形函数求得： 


$\left \{    \begin{array}{c c l}u(x,y) & = & \sum_{i=1}^{4}N_{i}(x,y)u_{i} \\v(x,y) & = & \sum_{i=1}^{4}N_{i}(x,y)v_{i} \\    \end{array}  \right  .$

其中$u_{i}, v_{i}$ 分别为各个节点在单元局部坐标系中$x$ 坐标轴向和$y$ 坐标轴向的位移分量。 

根据几何方程，应变为： 


$\xi_{xx} = \frac{\partial u }{\partial x} = \frac{\partial N_{1}}{\partial x} u_{1} + \cdots + \frac{\partial N_{4}}{\partial x} u_{4}\\$


$\xi_{yy} = \frac{\partial v}{\partial y} = \frac{\partial N_{1}}{\partial y} v_{1} + \cdots + \frac{\partial N_{4}}{\partial y} v_{4} \\$



$\xi_{xy} = \frac{\partial v}{\partial x} + \frac{\partial u }{\partial y}       = \frac{\partial N_{1}}{\partial y} u_{1} + \frac{\partial N_{1}}{\partial x} v_{1} + \cdots        + \frac{\partial N_{4}}{\partial y} u_{4} + \frac{\partial N_{4}}{\partial x} v_{4}$

则单元B阵为 


$B =\left [    \begin{array}{c c  c}\frac{\partial N_{1}(x,y)}{\partial x} & \cdots & 0 \\  0  & \cdots & \frac{\partial N_{4}(x,y)}{\partial y}   \\\frac{\partial N_{1}(x,y)}{\partial y}   & \cdots & \frac{\partial N_{4}(x,y)}{\partial x}  \\    \end{array}  \right ]$

材料本构阵为： 


$D =\frac{E}{1-\nu^{2}}\left [    \begin{array}{c c c}1  &  \nu  & 0 \\\nu & 1 & 0  \\0 &  0  & \frac{1-\nu}{2} \\    \end{array}  \right ]$

单元刚度阵为： 


$K = \int B^{T} D B ds$

其中 


$K_{11} =\frac{E}{1-\nu^{2}} \int_{0}^{1}\int_{0}^{1} ( (y-1)^{2} + \frac{1-\nu}{2}(x-1)^{2} )dxdy = \frac{E}{1-\nu^{2}}( \frac{1}{2} - \frac{ \nu }{6})$

其余的$K_{ij} $依次类推。
# 程序的局限性

Sigmund的99行Matlab拓扑优化程序 主要关注于拓扑优化算法是验证.   以牺牲了程序的通用性为代价，短短的99行代码中完成一个完整的拓扑优化流程。整个程序代码简洁、计算高效，但各程序模块的功能能省就省，往往使用最简单最直接的方法实现，存在一定的局限性，例如： 

 - 设计域默认是矩形 

 - 只能处理一种单元– 平面四边形单元  

 - 只能使用一种各向同性材料 

 - Filter函数依赖于设计域网格划分方式
# 修改程序

如果要改变优化模型和有限元模型，调整项与相应的功能模块如下表所示：
|调整项|功能模块|函数名|
|----|----|----|
|优化模型|主程序||
|优化算法|目标函数计算模块|OC|
|边界条件|有限元模型求解模块|FE|
|材料类型|单元刚度阵模块|lk|
|过滤算法|过滤器模块|check|
|材料插值模型|主程序||








