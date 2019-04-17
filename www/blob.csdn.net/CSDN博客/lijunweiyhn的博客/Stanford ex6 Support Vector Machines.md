# Stanford ex6:Support Vector Machines - lijunweiyhn的博客 - CSDN博客





2016年04月17日 11:46:43[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：492标签：[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[ML Stanford 实验小结](https://blog.csdn.net/lijunweiyhn/article/category/6175921)









这部分讲的主要是SVM，分线性分类跟非线性分类，主要是划边界的区别。然后给出了一个垃圾邮件分类的应用实例。

     先来看SVM部分。




Part 1: Loading and Visualizing Data


      第一部分，当然是读入数据再画出来有个直观的感觉。X的的Size为51*2，当然了，SVM嘛，肯定是有监督的，y里面存放的就是label的数据，正样本为1，负样本为0。图像大致可以看出用一条直线可以把它们有效的分割开。




Part 2: Training Linear SVM


       第二部分是训练线性的SVM，用的是SMO算法。用的训练函数返回的是这几个值：

       model.X= X(idx,:);

       model.y= Y(idx);

       model.kernelFunction = kernelFunction;

       model.b= b;

       model.alphas= alphas(idx);

       model.w = ((alphas.*Y)'*X)';

       算出来的是X和Y是支持向量。直接在原数据上取数据集点中x坐标最小和最大的，分成100分，用

         yp = - (w(1)*xp + b)/w(2);画出来即可。







Part 3: Implementing Gaussian Kernel


      一个检测高斯核函数（或则RBF）的检测程序，在后续中非线性分类中有应用。




Part 4: Visualizing Dataset 2


      这一次分类问题变得复杂，点数也变得更加的多，这时候，线性的分类将不再适用，所以得使用非线性的分类。跟线性分类边界画法不同。这次通过网格来实现数据边界。



     x1plot = linspace(min(X(:,1)), max(X(:,1)), 100)';

       x2plot = linspace(min(X(:,2)), max(X(:,2)), 100)';

       [X1, X2] = meshgrid(x1plot, x2plot);

       vals = zeros(size(X1));

       for i = 1:size(X1, 2)

           this_X = [X1(:, i), X2(:, i)];

           vals(:, i) = svmPredict(model, this_X);

       end



　　% Plot the SVM boundary

　　hold on

　　contour(X1, X2, vals, [0 0], 'Color', 'b');

　　hold off;



       这里说下meshgrid这个函数，他生成的矩阵规格是以x1plot的长度为列数，以x2plot的长度为行数。所以X1和X2的Size都是100*100，举个例子就懂了。



生成采样点的网格，举个例子就明白了x=1:3;y=10:14;[X,Y] = meshgrid(x,y)
X =

     1     2     3
     1     2     3
     1     2     3
     1     2     3
     1     2     3

Y =

    10    10    10
    11    11    11
    12    12    12
    13    13    13
    14    14    14


用for循环控制，当我i=1时，相当于this_X的SIze为100*2,第一列是x坐标，是不变的，第二列是y坐标在改变，懂了吧。把this_X拿去跟训练好的model做数据预测。可以得到vals的第一列大小为100*1，它记录了第一列上面网格点的预测情况。

   由于svm的预测就是0和1，而不是正则化逻辑回归里面的还包含小数，所以vals矩阵非常规范的只有0跟1，当我用等高线画出来的时候，可以看出来效果非常好。




Part 6: Visualizing Dataset 3


     画出数据dataset3。




Part 7: Training SVM with RBF Kernel




这部分内容用到了交叉验证的技巧。此时的数据里有yval 和Xval，跟理论一样，保留了90%作为验证。想说下dataset3Params这个函数。





C_vec = [0.01 0.03 0.1 0.3 1 3 10 30]';

sigma_vec = [0.01 0.03 0.1 0.3 1 3 10 30]';

error_val = zeros(length(C_vec),length(sigma_vec));

error_train = zeros(length(C_vec),length(sigma_vec));

for i = 1:length(C_vec)

    for j = 1:length(sigma_vec)

      model= svmTrain(X, y, C_vec(i), @(x1, x2) gaussianKernel(x1, x2, sigma_vec(j))); 

      predictions = svmPredict(model, Xval);

      error_val(i,j) = mean(double(predictions ~= yval));

    end

end





 [minval,ind] = min(error_val(:));   % 0.03

[I,J] = ind2sub([size(error_val,1) size(error_val,2)],ind);

C = C_vec(I)         %   1

sigma = sigma_vec(J)  %   0.100




   这里用的ind2sub找最小值，技巧性较强。




   下面是垃圾邮件的分类问题











————————————————垃圾邮件的分类问题————————————
















