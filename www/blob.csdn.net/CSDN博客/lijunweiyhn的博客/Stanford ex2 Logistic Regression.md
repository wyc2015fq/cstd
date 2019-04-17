# Stanford ex2: Logistic Regression - lijunweiyhn的博客 - CSDN博客





2016年04月17日 11:39:19[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：230标签：[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[ML Stanford 实验小结](https://blog.csdn.net/lijunweiyhn/article/category/6175921)









代码资源地址：http://download.csdn.net/detail/lijunweiyhn/9491877

       这一部分，介绍了逻辑回归并用其进行分类。 

       逻辑回归的分类分为线性和非线性的。废话不多说，先看线性部分。







 ————————————————线性分类————————————————

在这一部分，应用的背景是两项成绩和最后是否录取。是一个有监督的过程。

Part 1: Plotting 

读入原数据，并显示，原数据为100*3。




Part 2: Compute Cost and Gradient 


X是两项成绩，100*2，y是label，为100*1。故m=100，n=2。在X前面加一列成为100*3，多的一列你懂的是X with subscript 0。初始化θ向量，为3*1。然后计算损失函数和梯度，公式如下：




J= -1 * sum( y .* log( sigmoid(X*theta) ) + (1 - y ) .* log( (1 - sigmoid(X*theta)) ) ) / m ;

grad = ( X' * (sigmoid(X*theta) - y ) )/ m ;


这里是把整体数据样本当做一个集合来进行计算的。




Part 3: Optimizing using fminunc


用系统自带的优化函数进行优化，求min J(θ)下的θ向量。完了之后画出决策边界即可。在线性分类里面，决策边界画法是：



           plot_x = [min(X(:,2))-2,  max(X(:,2))+2];

           plot_y = (-1./theta(3)).*(theta(2).*plot_x + theta(1));  %其实就是θ0+θ1*x1+θ2*x2=0

           plot(plot_x, plot_y)；




Part 4: Predict and Accuracies


预测直接把新数据代入Hθ(x)即可。二值化分类，假设的伯努利模型，那么概率大于0.5就判定为1，反之判定为0。




 ————————————————非线性分类————————————————


非线性分类需要对J(θ)做正则化处理。




读入的X的Size为118*2，同样的一个是x坐标，一个是y坐标。

Part 1: Regularized Logistic Regression 


先把X用核函数进行处理，让其映射到高维空间。在这里的映射最高次是6次方。对应的生成顺序是，x1的1次方，x2的1次方，和他们交叉项的各个次方组合；x1的2次方，x2的2次方，和他们之间的各个次方组合.....等等。完了之后X从两维变成了28维。所以初始化向量θ为28*1。同样的，计算损失函数和梯度。




Part 2: Regularization and Accuracies


同样的，运行优化函数。算出每一个θ。想记录的是这个画图的过程。

    u = linspace(-1, 1.5, 50);

    v = linspace(-1, 1.5, 50);



    z = zeros(length(u), length(v));



    for i = 1:length(u)

        for j = 1:length(v)

            z(i,j) = mapFeature(u(i), v(j))*theta;

        end

    end



    z = z'; % important to transpose z before calling contour

    contour(u, v, z, [0, 0], 'LineWidth', 2)





  我们把横轴和纵轴画网格，分别从-1开始一直到1.5结束，均分成50个网格。z是记录每一个坐标交叉点出的值矩阵。对每一个点上的坐标值，我们对其做核函数运算，完了之后是118*28，然后对每一个点乘以我们得到的θ向量，就相当于对每个位置的点做了logistic分类，把结果存在z矩阵中。然后画出回归线即可。

   回归线的函数有些人可能不知道怎么用或则不知道什么意思，我举个例大家去试试大概就有个概念了。

   让xx=(-5:1:5); 

       yy=(-5:1:5);

    执行：

for i = 1:length(xx)
for j = 1:length(yy)
z(i,j) = xx(i)^2+yy(j)^2;     
end               
         end





     然后输入:

figure;contour(xx, yy, z, [50,50], 'LineWidth', 2);
figure;contour(xx, yy, z, [13,13], 'LineWidth', 2);
figure;contour(xx, yy, z, [4,4], 'LineWidth', 2);
figure;contour(xx, yy, z, [2,2], 'LineWidth', 2);
figure;contour(xx, yy, z, [2,2], 'LineWidth', 2);


     看看有什么效果吧。

















