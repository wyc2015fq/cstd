# LibSVM笔记系列（2）——如何提升LibSVM分类效果 - xiahouzuoxin - CSDN博客





2013年07月18日 20:30:37[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：23425







===============================================================================
***1. 'svm-scale'的使用***

===============================================================================

[monkeyzx@CentOS libsvm-3.14]$ ./svm-scale

Usage: svm-scale [options] data_filename

options:

-l lower : x scaling lower limit (default -1)

-u upper : x scaling upper limit (default +1)

-y y_lower y_upper : y scaling limits (default: no y scaling)

-s save_filename : save scaling parameters to save_filename

-r restore_filename : restore scaling parameters from restore_filename


自己使用matlab写的归一化函数为

function x = scale(x, bound, col) 

% bound:: [bmin bmax] 

% x :: input matrix 


if nargin < 3 

    col = 0; 

end 

bmin = bound(1); 

bmax = bound(2); 

if col == 0 

    for i = 1:size(x,1) 

            xmin = min(x(i,:)); 

            xmax = max(x(i,:));     

            x(i,:) = bmin + (bmax-bmin) * ((x(i,:)-xmin) / (xmax-xmin)); 

    end 

else 

    for i = 1:size(x,2) 

        xmin = min(x(:,i)); 

        xmax = max(x(:,i));     

        x(:,i) = bmin + (bmax-bmin) * ((x(:,i)-xmin) / (xmax-xmin));     

    end 

end 


end



===============================================================================
***2. 实际使用LibSVM的步骤***

===============================================================================

为提高实际分类的精度和泛化能力，希望在使用LibSVM时至少按照以下步骤考虑：

（1）对数据归一化，如果数据都为正数则一般归一到[0,1]，否则一般归一到[-1,1]

（2）在没特殊要求前提下，考虑使用RBF核函数K(x,y)=exp{-gamma|x-y|^2}

（3）在训练时使用交叉验证，并使用GirdSearch寻找最佳的参数C和gamma

（4）尽量对训练集数据集整体训练

（5）使用测试数据测试模型的分类效果


===============================================================================
***3. 关于交叉验证(CrossValidation，简称CV)***

===============================================================================

CV是用来验证分类器的性能一种统计分析方法,基本思想是把在某种意义下将原始数据(dataset)进行分组,一部分做为训练集(train set),另一部分做为验证集(validation set),首先用训练集对分类器进行训练,在利用验证集来测试训练得到的模型(model),以此来做为评价分类器的性能指标.


常见的交叉验证方法有：


1).Hold-Out Method


将原始数据随机分为两组,一组做为训练集,一组做为验证集,利用训练集训练分类器,然后利用验证集验证模型,记录最后的分类准确率为此Hold-OutMethod下分类器的性能指标.此种方法的好处的处理简单,只需随机把原始数据分为两组即可,其实严格意义来说Hold-Out Method并不能算是CV,因为这种方法没有达到交叉的思想,由于是随机的将原始数据分组,所以最后验证集分类准确率的高低与原始数据的分组有很大的关系,所以这种方法得到的结果其实并不具有说服性.


2).K-fold Cross Validation(记为K-CV)


将原始数据分成K组(一般是均分),将每个子集数据分别做一次验证集,其余的K-1组子集数据作为训练集,这样会得到K个模型,用这K个模型最终的验证集的分类准确率的平均数作为此K-CV下分类器的性能指标.K一般大于等于2,实际操作时一般从3开始取,只有在原始数据集合数据量小的时候才会尝试取2.K-CV可以有效的避免过学习以及欠学习状态的发生,最后得到的结果也比较具有说服性.


3).Leave-One-Out Cross Validation(记为LOO-CV)


如果设原始数据有N个样本,那么LOO-CV就是N-CV,即每个样本单独作为验证集,其余的N-1个样本作为训练集,所以LOO-CV会得到N个模型,用这N个模型最终的验证集的分类准确率的平均数作为此下LOO-CV分类器的性能指标.相比于前面的K-CV,LOO-CV有两个明显的优点:

a.每一回合中几乎所有的样本皆用于训练模型,因此最接近原始样本的分布,这样评估所得的结果比较可靠。

b.实验过程中没有随机因素会影响实验数据,确保实验过程是可以被复制的。

但LOO-CV的缺点则是计算成本高,因为需要建立的模型数量与原始数据样本数量相同,当原始数据样本数量相当多时,LOO-CV在实作上便有困难几乎就是不显示,除非每次训练分类器得到模型的速度很快,或是可以用并行化计算减少计

算所需的时间.


在LibSVM中，可以通过设置svm-train的-v参数设置交叉验证，-v表示交叉验证的分组数。


===============================================================================
***4. 关于GridSearch***

===============================================================================

在使用RBF核函数的情况下，一般需要优化的参数有：C和gamma。C是惩罚系数，C太小，容易发生过拟合，C太大，拟合的结果和实际结果相差太大；gamma是RBF的系数，gamma的取值直接影响到能否找到一个能将多类数据分开的最优超平面。因此，建议在交叉验证过程中使用简单的GridSearch算法找到最佳的C和gamma。


一般情况下不推荐使用遗传算法或粒子群优化算法，因为这些算法相对复杂，而且耗时又不一定能找到全局最优解，相对而言，GirdSearch简单也一般能找到一个恰当的解。


在使用GridSearch时，一般使用对数格点。

E.g.:v=5 C=2^(−5),2^(−3),..., 2^15 , gamma = 2^(−15),2^(−13),...,2^13


一个在matlab上实现GridSearch的函数如下：

function [bestacc bestc bestg] = svm_girdsearch(label, data, ... 

    cbound, gbound, cgstep, svm_cmd, v, opts_t, ker_func) 

% cmin:cstep:cmax  :: cost  

% gmin:gstep:gmax  :: gamma 

% v                :: cross validation n, default 5 

% t                :: '-t' for svmtrain 

% ker_func         :: function handle, used only if opts_t==4 

%                     ker_func(tr_data, gamma) 


if nargin < 8 

    opts_t = 2; 

elseif opts_t == 4 && nargin < 9 

    error('none of @ker_func.\n'); 

end 

if nargin < 7 

    v = 5; 

end 

if nargin < 6 

    svm_cmd = ''; 

end 

if nargin < 5 

    cstep = 0.8; 

    gstep = 0.8;    

else 

    cstep = cgstep(1); 

    gstep = cgstep(2); 

end 

if nargin < 4 

    gmax = 8; 

    gmin = -8;    

else 

    gmax = gbound(2); 

    gmin = gbound(1); 

end 

if nargin < 3 

    cmax = 8; 

    cmin = -8; 

else 

    cmax = cbound(2); 

    cmin = cbound(1); 

end 


[C, G] = meshgrid(cmin:cstep:cmax, gmin:gstep:gmax); 

[m n] = size(C); 

acc = zeros(m,n); 


eps = 10^(-2); 

basenum = 2; 

bestacc = 0; 

bestc = 1; 

bestg = 0.1; 

for i = 1:m 

    for j = 1:n  

        tmp_gamma = basenum^G(i,j); 

        tmp_C = basenum^C(i,j); 


        cmd = [svm_cmd, ' -c ',num2str(tmp_C), ' -g ',... 

            num2str(tmp_gamma), ' -t ', num2str(opts_t), ' -q '];         

        if opts_t ~= 4 

            acc(i,j) = cross_validation(label, data, cmd, v, 'svmtrain');  // 交叉验证函数（自己写的）

        else 

            ker = ker_func(data, basenum^G(i,j));      // 自定义核函数时，这里不用太关注 

            acc(i,j) = cross_validation(label, ker, cmd, v, 'svmtrain');             

        end 


        if acc(i,j) > bestacc 

            bestacc = acc(i,j); 

            bestg = tmp_gamma; 

            bestc = tmp_C; 

        end 


        % 当精度差别不大时，选择C值小的一组参数值 

        if abs(bestacc - acc(i,j)) < eps && tmp_C < bestc 

            bestacc = acc(i,j); 

            bestg = tmp_gamma; 

            bestc = tmp_C;             

        end 


        fprintf('cross validation (g=%s,C=%s) finished %d%%.\n', ... 

            num2str(tmp_gamma), num2str(tmp_C), floor(100*(i*n+j)/(m*n)) ); 

    end 

end 


% plot relationship bwtween g/c and acc 

figure; 

meshc(C,G,acc); 

axis([cmin,cmax,gmin,gmax,30,100]); 

xlabel('log2c','FontSize',10); 

ylabel('log2g','FontSize',10); 

zlabel('Accuracy(%)','FontSize',10); 

firstline = '[GridSearchMethod]';  

secondline = ['Best c=',num2str(bestc),' g=',num2str(bestg), ... 

    ' CVAccuracy=',num2str(bestacc),'%']; 

title({firstline;secondline},'Fontsize',10); 


end 

===============================================================================
***5. 非平衡数据***

===============================================================================

按照支持向量机使用超平面分类的原理，不平衡数据不会对超平面造成影响，但不平衡样本数据会对惩罚项产生影响。因此，在使用非平衡数据时可以使用LibSVM的-w选项平衡惩罚系数。

还有一种方式：选择另一种精度评估函数（LibSVM中使用测试样本中正确分类的样本数与总测试样本数的比值作为分类精度结果），详细参考LibSVM源码目录下的FAQ.html.



===============================================================================
***6. 自定义核函数（转自http://www.matlabsky.com/forum.php?mod=viewthread&tid=15296）***

===============================================================================

使用libsvm工具箱时，可以指定使用工具箱自带的一些核函数（-t参数），主要有：


    -t kernel_type : set type of kernel function (default 2)

            0 -- linear: u'*v

            1 -- polynomial: (gamma*u'*v + coef0)^degree

            2 -- radial basis function: exp(-gamma*|u-v|^2)

            3 -- sigmoid: tanh(gamma*u'*v + coef0)


但有时我们需要使用自己的核函数，这时候可以用 -t 4参数来实现：


    -t kernel_type : set type of kernel function (default 2)

            4 -- precomputed kernel (kernel values in training_instance_matrix)


使用-t 4参数时，再有了核函数后，需要给出核矩阵，关于核函数以及核函数构造相关的知识，大家可以看看相关书籍，在此不特别深入说明。


比如线性核函数 是 K(x,x') = (x * x')，设训练集是train_data，设训练集有150个样本 , 测试集是test_data，设测试集有120个样本

则 训练集的核矩阵是 ktrain1 = train_data*train_data'

     测试集的核矩阵是 ktest1 = test_data*train_data'

想要使用-t 4参数还需要把样本的序列号放在核矩阵前面 ，形成一个新的矩阵，然后使用svmtrain建立支持向量机，再使用svmpredict进行预测即可。形式与使用其他-t参数少有不同，如下：


    ktrain1 = train_data*train_data';

    Ktrain1 = [(1:150)',ktrain1];

    model_precomputed1 = svmtrain(train_label, Ktrain1, '-t 4');  % 注意此处的 输入 Ktrain1

    ktest1 = test_data*train_data';

    Ktest1 = [(1:120)', ktest1];

    [predict_label_P1, accuracy_P1, dec_values_P1] = svmpredict(test_label,Ktest1,model_precomputed1); % 注意此处输入Ktest1


下面是一个整体的小例子，大家可以看一下：


    %% Use_precomputed_kernelForLibsvm_example

    % faruto

    % last modified by 2011.04.20

    %%

    tic;

    clear;

    clc;

    close all;

    format compact;

    %%

    load heart_scale.mat;

    % Split Data

    train_data = heart_scale_inst(1:150,:);

    train_label = heart_scale_label(1:150,:);

    test_data = heart_scale_inst(151:270,:);

    test_label = heart_scale_label(151:270,:);


    %% Linear Kernel

    model_linear = svmtrain(train_label, train_data, '-t 0');

    [predict_label_L, accuracy_L, dec_values_L] = svmpredict(test_label, test_data, model_linear);


    %% Precomputed Kernel One

    % 使用的核函数 K(x,x') = (x * x')

    % 核矩阵

    ktrain1 = train_data*train_data';

    Ktrain1 = [(1:150)',ktrain1];

    model_precomputed1 = svmtrain(train_label, Ktrain1, '-t 4');

    ktest1 = test_data*train_data';

    Ktest1 = [(1:120)', ktest1];

    [predict_label_P1, accuracy_P1, dec_values_P1] = svmpredict(test_label, Ktest1, model_precomputed1);


    %% Precomputed Kernel Two

    % 使用的核函数 K(x,x') = ||x|| * ||x'||

    % 核矩阵

    ktrain2 = ones(150,150);

    for i = 1:150

        for j = 1:150

            ktrain2(i,j) = sum(train_data(i,:).^2)^0.5 * sum(train_data(j,:).^2)^0.5;

        end

    end

    Ktrain2 = [(1:150)',ktrain2];

    model_precomputed2 = svmtrain(train_label, Ktrain2, '-t 4');


    ktest2 = ones(120,150);

    for i = 1:120

        for j = 1:150

            ktest2(i,j) = sum(test_data(i,:).^2)^0.5 * sum(train_data(j,:).^2)^0.5;

        end

    end

    Ktest2 = [(1:120)', ktest2];

    [predict_label_P2, accuracy_P2, dec_values_P2] = svmpredict(test_label, Ktest2, model_precomputed2);

    %% Precomputed Kernel Three

    % 使用的核函数 K(x,x') = (x * x') / ||x|| * ||x'||

    % 核矩阵

    ktrain3 = ones(150,150);

    for i = 1:150

        for j = 1:150

            ktrain3(i,j) = ...

            train_data(i,:)*train_data(j,:)'/(sum(train_data(i,:).^2)^0.5 * sum(train_data(j,:).^2)^0.5);

        end

    end

    Ktrain3 = [(1:150)',ktrain3];

    model_precomputed3 = svmtrain(train_label, Ktrain3, '-t 4');


    ktest3 = ones(120,150);

    for i = 1:120

        for j = 1:150

            ktest3(i,j) = ...

            test_data(i,:)*train_data(j,:)'/(sum(test_data(i,:).^2)^0.5 * sum(train_data(j,:).^2)^0.5);

        end

    end

    Ktest3 = [(1:120)', ktest3];

    [predict_label_P3, accuracy_P3, dec_values_P3] = svmpredict(test_label, Ktest3, model_precomputed3);



    %% Display the accuracy

    accuracyL = accuracy_L(1) % Display the accuracy using linear kernel

    accuracyP1 = accuracy_P1(1) % Display the accuracy using precomputed kernel One

    accuracyP2 = accuracy_P2(1) % Display the accuracy using precomputed kernel Two

    accuracyP3 = accuracy_P3(1) % Display the accuracy using precomputed kernel Three

    %%

    toc;


运行结果：

    Accuracy = 85% (102/120) (classification)

    Accuracy = 85% (102/120) (classification)

    Accuracy = 67.5% (81/120) (classification)

    Accuracy = 84.1667% (101/120) (classification)

    accuracyL =

        85

    accuracyP1 =

        85

    accuracyP2 =

       67.5000

    accuracyP3 =

       84.1667

    Elapsed time is 1.424549 seconds.

关于核函数这里多说一下，核函数的正确选取依赖产生分类问题的实际问题的特点，因为不同的实际问题对相似程度有着不同的度量，核函数可以看作一个特征提取的过程，选择正确的核函数有助于提高分类准确率。

核函数的构造可以直接构造，也可以通过变换来得到。




**Refrences:**

1. 林智仁的SVM使用方法讲义：http://download.csdn.net/detail/xiahouzuoxin/5778941




