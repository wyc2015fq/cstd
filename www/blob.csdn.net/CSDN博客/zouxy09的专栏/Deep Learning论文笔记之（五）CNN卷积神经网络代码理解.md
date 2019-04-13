
# Deep Learning论文笔记之（五）CNN卷积神经网络代码理解 - zouxy09的专栏 - CSDN博客


2013年08月16日 00:55:18[zouxy09](https://me.csdn.net/zouxy09)阅读数：184060


**Deep Learning论文笔记之（五）CNN卷积神经网络代码理解**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
自己平时看了一些论文，但老感觉看完过后就会慢慢的淡忘，某一天重新拾起来的时候又好像没有看过一样。所以想习惯地把一些感觉有用的论文中的知识点总结整理一下，一方面在整理过程中，自己的理解也会更深，另一方面也方便未来自己的勘察。更好的还可以放到博客上面与大家交流。因为基础有限，所以对论文的一些理解可能不太正确，还望大家不吝指正交流，谢谢。
本文的代码来自githup的Deep Learning的toolbox，（在这里，先感谢该toolbox的作者）里面包含了很多Deep Learning方法的代码。是用Matlab编写的（另外，有人翻译成了C++和python的版本了）。本文中我们主要解读下CNN的代码。详细的注释见代码。
在读代码之前，最好先阅读下我的上一个博文：
Deep Learning论文笔记之（四）CNN卷积神经网络推导和实现
http://blog.csdn.net/zouxy09/article/details/9993371
里面包含的是我对一个作者的CNN笔记的翻译性的理解，对CNN的推导和实现做了详细的介绍，看明白这个笔记对代码的理解非常重要，所以强烈建议先看懂上面这篇文章。
下面是自己对代码的注释：
**cnnexamples.m**
`clear all; close all; clc;
addpath('../data');
addpath('../util');
load mnist_uint8;
train_x = double(reshape(train_x',28,28,60000))/255;
test_x = double(reshape(test_x',28,28,10000))/255;
train_y = double(train_y');
test_y = double(test_y');
%% ex1 
%will run 1 epoch in about 200 second and get around 11% error. 
%With 100 epochs you'll get around 1.2% error
cnn.layers = {
    struct('type', 'i') %input layer
    struct('type', 'c', 'outputmaps', 6, 'kernelsize', 5) %convolution layer
    struct('type', 's', 'scale', 2) %sub sampling layer
    struct('type', 'c', 'outputmaps', 12, 'kernelsize', 5) %convolution layer
    struct('type', 's', 'scale', 2) %subsampling layer
};
% 这里把cnn的设置给cnnsetup，它会据此构建一个完整的CNN网络，并返回
cnn = cnnsetup(cnn, train_x, train_y);
% 学习率
opts.alpha = 1;
% 每次挑出一个batchsize的batch来训练，也就是每用batchsize个样本就调整一次权值，而不是
% 把所有样本都输入了，计算所有样本的误差了才调整一次权值
opts.batchsize = 50; 
% 训练次数，用同样的样本集。我训练的时候：
% 1的时候 11.41% error
% 5的时候 4.2% error
% 10的时候 2.73% error
opts.numepochs = 10;
% 然后开始把训练样本给它，开始训练这个CNN网络
cnn = cnntrain(cnn, train_x, train_y, opts);
% 然后就用测试样本来测试
[er, bad] = cnntest(cnn, test_x, test_y);
%plot mean squared error
plot(cnn.rL);
%show test error
disp([num2str(er*100) '% error']);`
**cnnsetup.m**
`function net = cnnsetup(net, x, y)
    inputmaps = 1;
	% B=squeeze(A) 返回和矩阵A相同元素但所有单一维都移除的矩阵B，单一维是满足size(A,dim)=1的维。
	% train_x中图像的存放方式是三维的reshape(train_x',28,28,60000)，前面两维表示图像的行与列，
	% 第三维就表示有多少个图像。这样squeeze(x(:, :, 1))就相当于取第一个图像样本后，再把第三维
	% 移除，就变成了28x28的矩阵，也就是得到一幅图像，再size一下就得到了训练样本图像的行数与列数了
    mapsize = size(squeeze(x(:, :, 1)));
	% 下面通过传入net这个结构体来逐层构建CNN网络
	% n = numel(A)返回数组A中元素个数
	% net.layers中有五个struct类型的元素，实际上就表示CNN共有五层，这里范围的是5
    for l = 1 : numel(net.layers)   %  layer
        if strcmp(net.layers{l}.type, 's') % 如果这层是 子采样层
            % subsampling层的mapsize，最开始mapsize是每张图的大小28*28
			% 这里除以scale=2，就是pooling之后图的大小，pooling域之间没有重叠，所以pooling后的图像为14*14
			% 注意这里的右边的mapsize保存的都是上一层每张特征map的大小，它会随着循环进行不断更新
			mapsize = floor(mapsize / net.layers{l}.scale);
            for j = 1 : inputmaps % inputmap就是上一层有多少张特征图
                net.layers{l}.b{j} = 0; % 将偏置初始化为0
            end
        end
        if strcmp(net.layers{l}.type, 'c') % 如果这层是 卷积层
            % 旧的mapsize保存的是上一层的特征map的大小，那么如果卷积核的移动步长是1，那用
			% kernelsize*kernelsize大小的卷积核卷积上一层的特征map后，得到的新的map的大小就是下面这样
			mapsize = mapsize - net.layers{l}.kernelsize + 1;
			% 该层需要学习的参数个数。每张特征map是一个(后层特征图数量)*(用来卷积的patch图的大小)
			% 因为是通过用一个核窗口在上一个特征map层中移动（核窗口每次移动1个像素），遍历上一个特征map
			% 层的每个神经元。核窗口由kernelsize*kernelsize个元素组成，每个元素是一个独立的权值，所以
			% 就有kernelsize*kernelsize个需要学习的权值，再加一个偏置值。另外，由于是权值共享，也就是
			% 说同一个特征map层是用同一个具有相同权值元素的kernelsize*kernelsize的核窗口去感受输入上一
			% 个特征map层的每个神经元得到的，所以同一个特征map，它的权值是一样的，共享的，权值只取决于
			% 核窗口。然后，不同的特征map提取输入上一个特征map层不同的特征，所以采用的核窗口不一样，也
			% 就是权值不一样，所以outputmaps个特征map就有（kernelsize*kernelsize+1）* outputmaps那么多的权值了
			% 但这里fan_out只保存卷积核的权值W，偏置b在下面独立保存
            fan_out = net.layers{l}.outputmaps * net.layers{l}.kernelsize ^ 2;
            for j = 1 : net.layers{l}.outputmaps  %  output map
                % fan_out保存的是对于上一层的一张特征map，我在这一层需要对这一张特征map提取outputmaps种特征，
				% 提取每种特征用到的卷积核不同，所以fan_out保存的是这一层输出新的特征需要学习的参数个数
				% 而，fan_in保存的是，我在这一层，要连接到上一层中所有的特征map，然后用fan_out保存的提取特征
				% 的权值来提取他们的特征。也即是对于每一个当前层特征图，有多少个参数链到前层
				fan_in = inputmaps * net.layers{l}.kernelsize ^ 2;
                for i = 1 : inputmaps  %  input map
					% 随机初始化权值，也就是共有outputmaps个卷积核，对上层的每个特征map，都需要用这么多个卷积核
					% 去卷积提取特征。
					% rand(n)是产生n×n的 0-1之间均匀取值的数值的矩阵，再减去0.5就相当于产生-0.5到0.5之间的随机数
					% 再 *2 就放大到 [-1, 1]。然后再乘以后面那一数，why？
					% 反正就是将卷积核每个元素初始化为[-sqrt(6 / (fan_in + fan_out)), sqrt(6 / (fan_in + fan_out))]
					% 之间的随机数。因为这里是权值共享的，也就是对于一张特征map，所有感受野位置的卷积核都是一样的
					% 所以只需要保存的是 inputmaps * outputmaps 个卷积核。
                    net.layers{l}.k{i}{j} = (rand(net.layers{l}.kernelsize) - 0.5) * 2 * sqrt(6 / (fan_in + fan_out));
                end
                net.layers{l}.b{j} = 0; % 将偏置初始化为0
            end
			% 只有在卷积层的时候才会改变特征map的个数，pooling的时候不会改变个数。这层输出的特征map个数就是
			% 输入到下一层的特征map个数
            inputmaps = net.layers{l}.outputmaps; 
        end
    end
	
	% fvnum 是输出层的前面一层的神经元个数。
	% 这一层的上一层是经过pooling后的层，包含有inputmaps个特征map。每个特征map的大小是mapsize。
	% 所以，该层的神经元个数是 inputmaps * （每个特征map的大小）
	% prod: Product of elements.
	% For vectors, prod(X) is the product of the elements of X
	% 在这里 mapsize = [特征map的行数 特征map的列数]，所以prod后就是 特征map的行*列
    fvnum = prod(mapsize) * inputmaps;
	% onum 是标签的个数，也就是输出层神经元的个数。你要分多少个类，自然就有多少个输出神经元
    onum = size(y, 1);
	% 这里是最后一层神经网络的设定
	% ffb 是输出层每个神经元对应的基biases
    net.ffb = zeros(onum, 1);
	% ffW 输出层前一层 与 输出层 连接的权值，这两层之间是全连接的
    net.ffW = (rand(onum, fvnum) - 0.5) * 2 * sqrt(6 / (onum + fvnum));
end`
**cnntrain.m**
`function net = cnntrain(net, x, y, opts)
    m = size(x, 3); % m 保存的是 训练样本个数
    numbatches = m / opts.batchsize;
	% rem: Remainder after division. rem(x,y) is x - n.*y 相当于求余
	% rem(numbatches, 1) 就相当于取其小数部分，如果为0，就是整数
    if rem(numbatches, 1) ~= 0
        error('numbatches not integer');
    end
	
    net.rL = [];
    for i = 1 : opts.numepochs
		% disp(X) 打印数组元素。如果X是个字符串，那就打印这个字符串
        disp(['epoch ' num2str(i) '/' num2str(opts.numepochs)]);
        % tic 和 toc 是用来计时的，计算这两条语句之间所耗的时间
		tic;
		% P = randperm(N) 返回[1, N]之间所有整数的一个随机的序列，例如
		% randperm(6) 可能会返回 [2 4 5 6 1 3]
		% 这样就相当于把原来的样本排列打乱，再挑出一些样本来训练
        kk = randperm(m);
        for l = 1 : numbatches
			% 取出打乱顺序后的batchsize个样本和对应的标签
            batch_x = x(:, :, kk((l - 1) * opts.batchsize + 1 : l * opts.batchsize));
            batch_y = y(:,    kk((l - 1) * opts.batchsize + 1 : l * opts.batchsize));
			% 在当前的网络权值和网络输入下计算网络的输出
            net = cnnff(net, batch_x); % Feedforward
			% 得到上面的网络输出后，通过对应的样本标签用bp算法来得到误差对网络权值
			%（也就是那些卷积核的元素）的导数
            net = cnnbp(net, batch_y); % Backpropagation
			% 得到误差对权值的导数后，就通过权值更新方法去更新权值
            net = cnnapplygrads(net, opts);
            if isempty(net.rL)
                net.rL(1) = net.L; % 代价函数值，也就是误差值
            end
            net.rL(end + 1) = 0.99 * net.rL(end) + 0.01 * net.L; % 保存历史的误差值，以便画图分析
        end
        toc;
    end
    
end`
**cnnff.m**
`function net = cnnff(net, x)
    n = numel(net.layers); % 层数
    net.layers{1}.a{1} = x; % 网络的第一层就是输入，但这里的输入包含了多个训练图像
    inputmaps = 1; % 输入层只有一个特征map，也就是原始的输入图像
    for l = 2 : n   %  for each layer
        if strcmp(net.layers{l}.type, 'c') % 卷积层
            %  !!below can probably be handled by insane matrix operations
			% 对每一个输入map，或者说我们需要用outputmaps个不同的卷积核去卷积图像
            for j = 1 : net.layers{l}.outputmaps   %  for each output map
                %  create temp output map
				% 对上一层的每一张特征map，卷积后的特征map的大小就是 
				% （输入map宽 - 卷积核的宽 + 1）* （输入map高 - 卷积核高 + 1）
				% 对于这里的层，因为每层都包含多张特征map，对应的索引保存在每层map的第三维
				% 所以，这里的z保存的就是该层中所有的特征map了
                z = zeros(size(net.layers{l - 1}.a{1}) - [net.layers{l}.kernelsize - 1 net.layers{l}.kernelsize - 1 0]);
                for i = 1 : inputmaps   %  for each input map
                    %  convolve with corresponding kernel and add to temp output map
					% 将上一层的每一个特征map（也就是这层的输入map）与该层的卷积核进行卷积
					% 然后将对上一层特征map的所有结果加起来。也就是说，当前层的一张特征map，是
					% 用一种卷积核去卷积上一层中所有的特征map，然后所有特征map对应位置的卷积值的和
					% 另外，有些论文或者实际应用中，并不是与全部的特征map链接的，有可能只与其中的某几个连接
                    z = z + convn(net.layers{l - 1}.a{i}, net.layers{l}.k{i}{j}, 'valid');
                end
                %  add bias, pass through nonlinearity
				% 加上对应位置的基b，然后再用sigmoid函数算出特征map中每个位置的激活值，作为该层输出特征map
                net.layers{l}.a{j} = sigm(z + net.layers{l}.b{j});
            end
            %  set number of input maps to this layers number of outputmaps
            inputmaps = net.layers{l}.outputmaps;
        elseif strcmp(net.layers{l}.type, 's') % 下采样层
            %  downsample
            for j = 1 : inputmaps
                %  !! replace with variable
				% 例如我们要在scale=2的域上面执行mean pooling，那么可以卷积大小为2*2，每个元素都是1/4的卷积核
				z = convn(net.layers{l - 1}.a{j}, ones(net.layers{l}.scale) / (net.layers{l}.scale ^ 2), 'valid'); 
				% 因为convn函数的默认卷积步长为1，而pooling操作的域是没有重叠的，所以对于上面的卷积结果
				% 最终pooling的结果需要从上面得到的卷积结果中以scale=2为步长，跳着把mean pooling的值读出来
                net.layers{l}.a{j} = z(1 : net.layers{l}.scale : end, 1 : net.layers{l}.scale : end, :);
            end
        end
    end
    %  concatenate all end layer feature maps into vector
	% 把最后一层得到的特征map拉成一条向量，作为最终提取到的特征向量
    net.fv = [];
    for j = 1 : numel(net.layers{n}.a) % 最后一层的特征map的个数
        sa = size(net.layers{n}.a{j}); % 第j个特征map的大小
		% 将所有的特征map拉成一条列向量。还有一维就是对应的样本索引。每个样本一列，每列为对应的特征向量
        net.fv = [net.fv; reshape(net.layers{n}.a{j}, sa(1) * sa(2), sa(3))];
    end
    %  feedforward into output perceptrons
	% 计算网络的最终输出值。sigmoid(W*X + b)，注意是同时计算了batchsize个样本的输出值
    net.o = sigm(net.ffW * net.fv + repmat(net.ffb, 1, size(net.fv, 2)));
end`
**cnnbp.m**
`function net = cnnbp(net, y)
    n = numel(net.layers); % 网络层数
    %  error
    net.e = net.o - y; 
    %  loss function
	% 代价函数是 均方误差
    net.L = 1/2* sum(net.e(:) .^ 2) / size(net.e, 2);
    %%  backprop deltas
	% 这里可以参考 UFLDL 的 反向传导算法 的说明
	% 输出层的 灵敏度 或者 残差
    net.od = net.e .* (net.o .* (1 - net.o));   %  output delta
	% 残差 反向传播回 前一层
    net.fvd = (net.ffW' * net.od);              %  feature vector delta
    if strcmp(net.layers{n}.type, 'c')         %  only conv layers has sigm function
        net.fvd = net.fvd .* (net.fv .* (1 - net.fv));
    end
    %  reshape feature vector deltas into output map style
    sa = size(net.layers{n}.a{1}); % 最后一层特征map的大小。这里的最后一层都是指输出层的前一层
    fvnum = sa(1) * sa(2); % 因为是将最后一层特征map拉成一条向量，所以对于一个样本来说，特征维数是这样
    for j = 1 : numel(net.layers{n}.a) % 最后一层的特征map的个数
		% 在fvd里面保存的是所有样本的特征向量（在cnnff.m函数中用特征map拉成的），所以这里需要重新
		% 变换回来特征map的形式。d 保存的是 delta，也就是 灵敏度 或者 残差
        net.layers{n}.d{j} = reshape(net.fvd(((j - 1) * fvnum + 1) : j * fvnum, :), sa(1), sa(2), sa(3));
    end
	% 对于 输出层前面的层（与输出层计算残差的方式不同）
    for l = (n - 1) : -1 : 1
        if strcmp(net.layers{l}.type, 'c')
            for j = 1 : numel(net.layers{l}.a) % 该层特征map的个数
                % net.layers{l}.d{j} 保存的是 第l层 的 第j个 map 的 灵敏度map。 也就是每个神经元节点的delta的值
				% expand的操作相当于对l+1层的灵敏度map进行上采样。然后前面的操作相当于对该层的输入a进行sigmoid求导
				% 这条公式请参考 Notes on Convolutional Neural Networks
				% for k = 1:size(net.layers{l + 1}.d{j}, 3)
					% net.layers{l}.d{j}(:,:,k) = net.layers{l}.a{j}(:,:,k) .* (1 - net.layers{l}.a{j}(:,:,k)) .*  kron(net.layers{l + 1}.d{j}(:,:,k), ones(net.layers{l + 1}.scale)) / net.layers{l + 1}.scale ^ 2;
				% end
				net.layers{l}.d{j} = net.layers{l}.a{j} .* (1 - net.layers{l}.a{j}) .* (expand(net.layers{l + 1}.d{j}, [net.layers{l + 1}.scale net.layers{l + 1}.scale 1]) / net.layers{l + 1}.scale ^ 2);
            end
        elseif strcmp(net.layers{l}.type, 's')
            for i = 1 : numel(net.layers{l}.a) % 第l层特征map的个数
                z = zeros(size(net.layers{l}.a{1}));
                for j = 1 : numel(net.layers{l + 1}.a) % 第l+1层特征map的个数
                     z = z + convn(net.layers{l + 1}.d{j}, rot180(net.layers{l + 1}.k{i}{j}), 'full');
                end
                net.layers{l}.d{i} = z;
            end
        end
    end
    %%  calc gradients
	% 这里与 Notes on Convolutional Neural Networks 中不同，这里的 子采样 层没有参数，也没有
	% 激活函数，所以在子采样层是没有需要求解的参数的
    for l = 2 : n
        if strcmp(net.layers{l}.type, 'c')
            for j = 1 : numel(net.layers{l}.a)
                for i = 1 : numel(net.layers{l - 1}.a)
					% dk 保存的是 误差对卷积核 的导数
                    net.layers{l}.dk{i}{j} = convn(flipall(net.layers{l - 1}.a{i}), net.layers{l}.d{j}, 'valid') / size(net.layers{l}.d{j}, 3);
                end
				% db 保存的是 误差对于bias基 的导数
                net.layers{l}.db{j} = sum(net.layers{l}.d{j}(:)) / size(net.layers{l}.d{j}, 3);
            end
        end
    end
	% 最后一层perceptron的gradient的计算
    net.dffW = net.od * (net.fv)' / size(net.od, 2);
    net.dffb = mean(net.od, 2);
    function X = rot180(X)
        X = flipdim(flipdim(X, 1), 2);
    end
end`
**cnnapplygrads.m**
`function net = cnnapplygrads(net, opts)
    for l = 2 : numel(net.layers)
        if strcmp(net.layers{l}.type, 'c')
            for j = 1 : numel(net.layers{l}.a)
                for ii = 1 : numel(net.layers{l - 1}.a)
					% 这里没什么好说的，就是普通的权值更新的公式：W_new = W_old - alpha * de/dW（误差对权值导数）
                    net.layers{l}.k{ii}{j} = net.layers{l}.k{ii}{j} - opts.alpha * net.layers{l}.dk{ii}{j};
                end
            end
            net.layers{l}.b{j} = net.layers{l}.b{j} - opts.alpha * net.layers{l}.db{j};
        end
    end
    net.ffW = net.ffW - opts.alpha * net.dffW;
    net.ffb = net.ffb - opts.alpha * net.dffb;
end`
**cnntest.m**
`function [er, bad] = cnntest(net, x, y)
    %  feedforward
    net = cnnff(net, x); % 前向传播得到输出
	% [Y,I] = max(X) returns the indices of the maximum values in vector I
    [~, h] = max(net.o); % 找到最大的输出对应的标签
    [~, a] = max(y); 	 % 找到最大的期望输出对应的索引
    bad = find(h ~= a);  % 找到他们不相同的个数，也就是错误的次数
    er = numel(bad) / size(y, 2); % 计算错误率
end`


