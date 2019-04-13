
# LibLinear（SVM包）使用说明之（三）实践 - zouxy09的专栏 - CSDN博客


2013年09月02日 20:02:26[zouxy09](https://me.csdn.net/zouxy09)阅读数：14518


**LibLinear（SVM包）使用说明之（三）实践**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
我们在UFLDL的教程中，Exercise: Convolution and Pooling这一章节，已经得到了cnnPooledFeatures.mat特征。在该练习中，我们使用的是softmax分类器来分类的。在这里我们修改为用SVM来替代softmax分类器。SVM由Liblinear软件包来提供。这里是四分类问题，所以Liblinear会根据我们传入的训练样本训练四个二分类器，以实现四分类。以前由softmax分类器得到的准确率是80.406%。在这里换成Liblinear后，准确率变为80.75%。在这里差别不是很大。
在本文的例子中，我们增加了scale和Cross Validation，Cross Validation是用来选择一个最好的参数C的（不知道自己这两个步骤有没有正确，如有错误，还望大家提醒，谢谢）。
具体的代码如下：
`%// Classification by LibLinear
%// LibLinear: http://www.csie.ntu.edu.tw/~cjlin/liblinear/
%// Author : zouxy
%// Date   : 2013-9-2
%// HomePage : http://blog.csdn.net/zouxy09
%// Email  : zouxy09@qq.com
clear; clc;
%%% step1: load data
fprintf(1,'step1: Load data...\n');
% pooledFeaturesTrain大小为400*2000*3*3
% pooledFeaturesTest大小为400*3200*3*3
% 第一维是特征个数，也就是特征图个数，第二维是样本个数，第三维是特征图的宽，
% 第四维是特征图的高
load cnnPooledFeatures.mat;
load stlTrainSubset.mat % loads numTrainImages, trainImages, trainLabels
load stlTestSubset.mat  % loads numTestImages,  testImages,  testLabels
% B = permute(A,order) 按照向量order指定的顺序重排A的各维
train_X = permute(pooledFeaturesTrain, [1 3 4 2]);
% 将每个样本的特征拉成一个列向量，每个样本一个列，矩阵大小为3600*2000
train_X = reshape(train_X, numel(pooledFeaturesTrain) / numTrainImages, numTrainImages);
train_Y = trainLabels; % 2000*1
test_X = permute(pooledFeaturesTest, [1 3 4 2]);
test_X = reshape(test_X, numel(pooledFeaturesTest) / numTestImages, numTestImages);
test_Y = testLabels;
% release some memory
clear trainImages testImages pooledFeaturesTrain pooledFeaturesTest;
%%% step2: scale the data
fprintf(1,'step2: Scale data...\n');
% Using the same scaling factors for training and testing sets, 
% we obtain much better accuracy. Note: scale each attribute(feature), not sample
% scale to [0 1]
% when a is a vector, b = (a - min(a)) .* (upper - lower) ./ (max(a)-min(a)) + lower
lower = 0;
upper = 1.0;
train_X = train_X';
X_max = max(train_X);
X_min = min(train_X);
train_X = (train_X - repmat(X_min, size(train_X, 1), 1)) .* (upper - lower) ...
			./ repmat((X_max - X_min), size(train_X, 1), 1) + lower;
test_X = test_X';
test_X = (test_X - repmat(X_min, size(test_X, 1), 1)) .* (upper - lower) ...
			./ repmat((X_max - X_min), size(test_X, 1), 1) + lower;
% Note: before scale the accuracy is 80.4688%, after scale it turns to 80.1875%,
% and took more time. So is that my scale operation wrong or other reasons?
% After adding bias, Accuracy = 80.75% (2584/3200)
%%% step3: Cross Validation for choosing parameter
fprintf(1,'step3: Cross Validation for choosing parameter c...\n');
% the larger c is, more time should be costed
c = [2^-6 2^-5 2^-4 2^-3 2^-2 2^-1 2^0 2^1 2^2 2^3];
max_acc = 0;
tic;
for i = 1 : size(c, 2)
	option = ['-B 1 -c ' num2str(c(i)) ' -v 5 -q'];
	fprintf(1,'Stage: %d/%d: c = %d, ', i, size(c, 2), c(i));
	accuracy = train(train_Y, sparse(train_X), option);	
	if accuracy > max_acc
		max_acc = accuracy;
		best_c = i;
	end
end
fprintf(1,'The best c is c = %d.\n', c(best_c));
toc;
%%% step4: train the model
fprintf(1,'step4: Training...\n');
tic;
option = ['-c ' num2str(c(best_c)) ' -B 1 -e 0.001'];
model = train(train_Y, sparse(train_X), option);
toc;
%%% step5: test the model
fprintf(1,'step5: Testing...\n');
tic;
[predict_label, accuracy, dec_values] = predict(test_Y, sparse(test_X), model);
toc;`

