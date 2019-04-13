
# LibLinear（SVM包）使用说明之（二）MATLAB接口 - zouxy09的专栏 - CSDN博客


2013年09月02日 19:56:43[zouxy09](https://me.csdn.net/zouxy09)阅读数：25175


**LibLinear（SVM包）使用说明之（二）MATLAB接口**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
**一、介绍**
LIBLINEAR是一个简单的求解大规模规则化线性分类和回归的软件包。本文介绍在Matlab中如何使用该软件包。(http://www.csie.ntu.edu.tw/~cjlin/liblinear)
**二、安装**
在Windows系统中，预编译好的可执行文件在..\windows目录下。但我们提供的是64bit的，如果你的电脑是32bit的，则需要按照下面的步骤重新编译：
我们建议使用make.m来编译。直接在MATLAB or Octave中运行make即可在当前目录生成四个文件：'libsvmread.mex', 'libsvmwrite.mex', 'train.mex',和'predict.mex'。
On MATLAB or Octave:
>> make
如果在MATLAB中，make.m出错了，可以尝试运行'mex -setup'来给mex选择一个合适的编译器。需要先确定你的编译器是可用的。
在Unix系统，如果make.m和mex -setup都无效，请用Makefile。注意我们的MATLAB是安装在'/usr/local/matlab'中的。如果不是的话，需要在Makefile的MATLABDIR改为我们matlab的安装目录。
**三、用法**
**1****、训练**
matlab> model = train(training_label_vector, training_instance_matrix [,'liblinear_options', 'col']);
参数如下：
-training_label_vector：
训练样本的标签，mx1向量。注意必须是double类型。
-training_instance_matrix：
训练样本，mxn矩阵。每个样本为n维。它必须是稀疏的，而且必须是double类型。
-liblinear_options:
字符串格式的训练选项，格式和LIBLINEAR一样。
-col:
如果设置了'col'，training_instance_matrix的每一列是一个样本。否则每个行是一个样本。
注意：这里好像只接受两个字符串的参数。所以传多个参数的时候，要按以下的方式来传，例如：（predict函数同样）
train(train_Y,  train_X, '-c 1 -v 5', ' col');        //正确
train(train_Y,  train_X, '-c 1’, ‘ -v 5', ' col');   //错误
train(train_Y,  train_X, ' col');                 //错误
train(train_Y,  train_X, ' ', ' col');            //正确
**2****、测试**
matlab> [predicted_label, accuracy, decision_values/prob_estimates] = predict(testing_label_vector, testing_instance_matrix, model [, 'liblinear_options', 'col']);
参数如下：
-testing_label_vector:
测试样本的标签，mx1向量。如果未知，那么就简单使用随机值。注意必须是double类型。
-testing_instance_matrix:
训练样本，mxn矩阵。每个样本为n维。它必须是稀疏的，而且必须是double类型。
-model:
训练得到的模型。
-liblinear_options:
字符串格式的测试选项，格式和LIBLINEAR一样。
-col:
如果设置了'col'，training_instance_matrix的每一列是一个样本。否则每个行是一个样本。
**四、返回的模型结构**
train函数会返回一个模型，这样我们可以用以未来的预测。模型是以结构形式保存的，组织为[Parameters, nr_class, nr_feature, bias, Label, w]:
-Parameters:参数
-nr_class:类的个数，如果是回归，那么这个数是2.
-nr_feature:训练数据的样本维数（不包括bias项）。
-bias:如果bias >= 0，我们会在每个样本的最后添加一个额外的特征。
-Label:每个类的标签，对回归来说，为空。
-w:一个nr_w x n权值矩阵。n是nr_feature（特征维数）或者nr_feature+1（存在bias项时）。如果nr_class=2，并且-s不是4（不是Crammer and Singer的多类SVM），那么nr_w是1，对于其他情况，nr_w等于nr_class。
如果指定了-v，那么就是交叉校验模式，返回的模型就是一个标量，对于分类器来说就是交叉校验的准确率，对回归来说，是均方误差。
**五、预测结果**
函数predict有三个输出。第一个是预测的标签构成的向量predicted_label。第二个输出是准确率，是一个包含准确率（对分类来说），均方误差和平方相关系数（对回归来说）的向量。第三个是一个包含决策值或者概率值（如果指定了'-b 1'）的矩阵。如果k是类的数目，k’是分类器的数目（如果k=2那k'=1，否则k'=k），那对决策值矩阵，每行包括k’个二分类器的结果。对概率值矩阵，每行包括k个值，分别表示测试样本为为各类的概率值。注意的是，类的序列在这里和模型结构中的'Label'的域是一样的。
**六、其他工具**
[label_vector, instance_matrix] = libsvmread('data.txt');
Libsvmread函数可以读LIBSVM格式的文件。两个输出分别是标签labels和样本集instances，它们可以作为svmtrain或者svmpredict的输入。
libsvmwrite('data.txt', label_vector, instance_matrix]
libsvmwrite可以将参数以LIBSVM的格式写到文件中。instance_matrix必须是个稀疏矩阵。类型必须是double型。对windows，`libsvmread.mexw64'和`libsvmwrite.mexw64'经编译后已经躺在目录`..\windows'下了。
**七、例子**
我们提供了数据heart_scale供训练和测试。
matlab> [heart_scale_label, heart_scale_inst] = libsvmread('../heart_scale'); %读数据
matlab> model = train(heart_scale_label, heart_scale_inst, '-c 1'); %训练
matlab> [predict_label, accuracy, dec_values] = predict(heart_scale_label, heart_scale_inst, model);  %测试
对概率估计，需要在测试阶段加上选项'-b 1'：
matlab> [predict_label, accuracy, prob_estimates] = predict(heart_scale_label, heart_scale_inst, model, '-b 1');
需要注意的是，上面的指令的运行需要我们确认我们所在的目录，和那些mex文件所在的目录。我们也可以在Matlab目录下新建一个test_heart_scale.m文件：
clear; clc;
tic;
[heart_scale_label, heart_scale_inst] = libsvmread('../heart_scale'); %读数据
model = train(heart_scale_label, heart_scale_inst, '-c 1'); %训练
[predict_label, accuracy, dec_values] = predict(heart_scale_label, heart_scale_inst, model);  %测试
toc;
运行上述文件，输出：
.......*
optimization finished, \#iter = 71
Objective value = -121.108744
nSV = 194
Accuracy = 84.0741% (227/270)
Elapsed time is 0.006373 seconds.
好快啊！！！


