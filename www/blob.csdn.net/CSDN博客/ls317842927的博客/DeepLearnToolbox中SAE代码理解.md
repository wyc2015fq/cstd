# DeepLearnToolbox中SAE代码理解 - ls317842927的博客 - CSDN博客





2016年11月15日 08:50:33[ls317842927](https://me.csdn.net/ls317842927)阅读数：4040








**Dropout**是指在模型训练时随机让网络某些隐含层节点的权重不工作，不工作的那些节点可以暂时认为不是网络结构的一部分，但是它的权重得保留下来（只是暂时不更新而已），因为下次样本输入时它可能又得工作了。 

训练神经网络模型时，如果训练样本较少，为了**防止模型过拟合**，Dropout可以作为一种trikc供选择。Dropout是hintion最近2年提出的，源于其文章Improving neural networks by preventing co-adaptation of feature detectors.中文大意为：通过阻止特征检测器的共同作用来提高神经网络的性能。

**Denoise Autoencoder**(简称dAE)是指当采用无监督的方法分层预训练深度网络的权值时，为了**学习到较鲁棒的特征**，可以在网络的可视层（即数据的输入层）引入随机噪声，这里并不是加入高斯噪声，而是**以一定概率使输入层节点的值清为0**。这种方法由Bengio在08年提出，见其文章Extracting and composing robust features with denoising autoencoders.使用dAE时，可以用被破坏的输入数据重构出原始的没被破坏的数据，所以它训练出来的特征会更鲁棒。 

Bengio对dAE的直观解释为：1.dAE有点类似人体的感官系统，比如人眼看物体时，如果物体某一小部分被遮住了，人依然能够将其识别出来，2.多模态信息输入人体时（比如声音，图像等），少了其中某些模态的信息有时影响也不大。3.普通的autoencoder的本质是学习一个相等函数，即输入和重构后的输出相等，这种相等函数的表示有个缺点就是当测试样本和训练样本不符合同一分布，即相差较大时，效果不好，明显，dAE在这方面的处理有所进步。 

当在训练深度网络时，且采用了无监督方法预训练权值，通常，Dropout和Denoise Autoencoder在使用时有一个小地方不同：Dropout在分层预训练权值的过程中是不参与的，只是后面的微调部分引入；而Denoise Autoencoder是在每层预训练的过程中作为输入层被引入，在进行微调时不参与。另外，一般的重构误差可以采用均方误差的形式，但是如果输入和输出的向量元素都是位变量，则一般采用交叉熵来表示两者的差异。 
**test_example_SAE.m**
```matlab
function test_example_SAE
sae = saesetup([327 200 20]);%1*2cell，[327 200 327]和[200 20 200]两个神经网络
    sae.ae{1}.activation_function       = 'sigm';%只改变第一个神经网络的参数
    sae.ae{1}.learningRate              = 1;
    sae.ae{1}.inputZeroMaskedFraction   = 0.5;
    opts.numepochs =   1;
    opts.batchsize = 100;
    [reducedim_x,sae] = saetrain(sae, X, opts);%reducedim_x是降维后的20维的训练样本
```

**saesetup.m**

```matlab
function sae = saesetup(size)%[327 200 20]
    for u = 2 : numel(size)%3
        sae.ae{u-1} = nnsetup([size(u-1) size(u) size(u-1)]);%sae.ae{1} = nnsetup([327 200 327]);以此为例注释
                                                             %sae.ae{2} = nnsetup([200 20 200]);
    end
end
```

**nnsetup.m**

```matlab
function nn = nnsetup(architecture)
%NNSETUP creates a Feedforward Backpropagate Neural Network
% nn = nnsetup(architecture) returns an neural network structure with n=numel(architecture)
% layers, architecture being a 1 x n vector of layer sizes e.g. [784 100 10]

    nn.size   = architecture;%构建神经网络[327 200 327]
    nn.n      = numel(nn.size);%n=3

    nn.activation_function              = 'tanh_opt';   %  Activation functions of hidden layers: 'sigm' (sigmoid) or 'tanh_opt' (optimal tanh).
    nn.learningRate                     = 2;            %  learning rate Note: typically needs to be lower when using 'sigm' activation function and non-normalized inputs.
    nn.momentum                         = 0.5;          %  Momentum
    nn.scaling_learningRate             = 1;            %  学习率的收缩因子；Scaling factor for the learning rate (each epoch)
    nn.weightPenaltyL2                  = 0;            %  W的L2规则项的系数；L2 regularization
    nn.nonSparsityPenalty               = 0;            %  稀疏惩罚项的系数；Non sparsity penalty
    nn.sparsityTarget                   = 0.05;         %  稀疏因数（Sparse AE）Sparsity target
    nn.inputZeroMaskedFraction          = 0;            %  Used for Denoising AutoEncoders
    nn.dropoutFraction                  = 0;            %  Dropout level (http://www.cs.toronto.edu/~hinton/absps/dropout.pdf)
    nn.testing                          = 0;            %  Internal variable. nntest sets this to one.
    nn.output                           = 'sigm';       %  output unit 'sigm' (=logistic), 'softmax' and 'linear'

    for i = 2 : nn.n   
        % weights and weight momentum
        nn.W{i - 1} = (rand(nn.size(i), nn.size(i - 1)+1) - 0.5) * 2 * 4 * sqrt(6 / (nn.size(i) + nn.size(i - 1)));
        %W{1}=[200x328],W{2}=[327x201]
        nn.vW{i - 1} = zeros(size(nn.W{i - 1}));

        % 平均激活值（Sparse AE）;average activations (for use with sparsity)
        nn.p{i}     = zeros(1, nn.size(i)); % p{1}=[],p{2}=[1x200],p{3}=[1x327]
        %对于神经网络[327 200 327]，隐藏层节点个数是200和327
    end
end
```

**saetrain.m**

```matlab
function [reducedim_x,sae] = saetrain(sae, x, opts)
    reducedim_x = {};  
    for i = 1 : numel(sae.ae);%sae 1*2cell，[327 200 327]和[200 20 200]两个神经网络
        disp(['Training AE ' num2str(i) '/' num2str(numel(sae.ae))]);
        sae.ae{i} = nntrain(sae.ae{i}, x, x, opts);%训练出最终模型sae.ae{i}（更新后的W），输入的ae模型的W是随机初始化的
        t = nnff(sae.ae{i}, x, x);%前向传播，算出每层的特征（激活值）

         x = t.a{2};%取中间层的特征
        %remove bias term
         x = x(:,2:end);%中间层的特征去除bias后 是下一个ae的输入x
         reducedim_x = x;

    end
end
```

**nntrain.m**

```matlab
function [nn, L]  = nntrain(nn, train_x, train_y, opts, val_x, val_y)
%NNTRAIN trains a neural net
% [nn, L] = nntrain(nn, x, y, opts) trains the neural network nn with input x and
% output y for opts.numepochs epochs, with minibatches of size opts.batchsize.
%  用输入x和输出y训练一个神经网络，循环opts.numepochs次，每次样本数量为opts.batchsize
%Returns a neural network nn with updated activations,
% errors, weights and biases, (nn.a, nn.e, nn.W, nn.b) and L, the sum
% squared error for each training minibatch.
%返回更新后的激活值，误差值，权重W，偏置量b和每个batch样本的均方误差
assert(isfloat(train_x), 'train_x must be a float');
assert(nargin == 4 || nargin == 6,'number ofinput arguments must be 4 or 6')

loss.train.e               = [];
loss.train.e_frac          = [];
loss.val.e                 = [];
loss.val.e_frac            = [];
opts.validation = 0;
if nargin == 6
    opts.validation = 1;
end

fhandle = [];
if isfield(opts,'plot') && opts.plot == 1
    fhandle = figure();
end

m = size(train_x, 1);%414
train_x_dims  =  size(train_x,2);%327
train_y_dims  =  size(train_y,2);%327，这里的train_x和train_y都是训练样本X

batchsize = opts.batchsize;%100
numepochs = opts.numepochs;%1 循环次数

% numbatches = m / batchsize;
numbatches = ceil(m / batchsize);%5

assert(rem(numbatches, 1) == 0, 'numbatches must be a integer');

L = zeros(numepochs*numbatches,1);%记录每次循环，每个batch样本的均方误差
n = 1;
for i = 1 : numepochs
    tic;
        kk = randperm(m);
    for l = 1 : numbatches

        batch_x=zeros(batchsize,train_x_dims);%[100x327]初始化为0
        batch_y=zeros(batchsize,train_y_dims);%[100x327]初始化为0

        end_indice=min(l*batchsize,m);
        batch_x(1:end_indice-(l-1)*batchsize,1:train_x_dims) = train_x(kk((l - 1) * batchsize + 1 : end_indice), :);
        %最后一个batch样本不足100个！！

        %Add noise to input (for use in denoising autoencoder)
        if(nn.inputZeroMaskedFraction ~= 0)
            batch_x = batch_x.*(rand(size(batch_x))>nn.inputZeroMaskedFraction);
            %rand(size(batch_x))产生和batch_x矩阵一样大小的矩阵，元素是随机0~1之间的数字。
            %而rand(size(batch_x))>nn.inputZeroMaskedFraction使随机数字的矩阵大于0.5的为1，小于0.5的为0。
            %batch_x.*(rand(size(batch_x))>nn.inputZeroMaskedFraction)
            %将batch_x与这个只有0和1的矩阵 对应元素相乘，使batch_x很多特征变成0
            %
        end
        %融合特征其实不需要y，y用来给nnff函数算错误率

        batch_y(1:end_indice-(l-1)*batchsize,1:train_y_dims) = train_y(kk((l - 1) * batchsize + 1 :end_indice), :);

        nn = nnff(nn, batch_x, batch_y);%计算每层的a,即sigm(数据x*W),得到output（前向传播）
        nn = nnbp(nn);%根据output得到所有层dW(W的偏导数)，bp算法
        nn = nnapplygrads(nn);%根据dW更新所有层W，批量梯度下降算法法

        L(n) = nn.L;

        n = n + 1;
    end%一个batch结束，用每个batch都对所有层W更新一次

    t = toc;

    if opts.validation == 1
        loss = nneval(nn, loss, train_x, train_y, val_x, val_y);
        str_perf = sprintf('; Full-batch train mse = %f, val mse = %f', loss.train.e(end), loss.val.e(end));
    else
        loss = nneval(nn, loss, train_x, train_y);
        str_perf = sprintf('; Full-batch train err = %f', loss.train.e(end));
    end
    if ishandle(fhandle)
        nnupdatefigures(nn, fhandle, loss, opts, i);
    end

    disp(['epoch ' num2str(i) '/' num2str(opts.numepochs) '. Took ' num2str(t) ' seconds' '. Mini-batch mean squared error on training set is ' num2str(mean(L((n-numbatches):(n-1)))) str_perf]);
    nn.learningRate = nn.learningRate * nn.scaling_learningRate;%学习率收缩
end%一次循环结束
end
```

**nnff.m**

```matlab
function nn = nnff(nn, x, y)
%NNFF performs a feedforward pass
% nn = nnff(nn, x, y) returns an neural network structure with updated
% layer activations, error and loss (nn.a, nn.e and nn.L)
%进行前向传播，得到各层激活值，最后一层的误差和均方误差
    n = nn.n;%nn=[327 200 327],n=3
    m = size(x, 1);%m=100

    x = [ones(m,1) x];% [x0 x]=[100x328],第一列是偏置量
    nn.a{1} = x;%第一层激活值是输入x

    %feedforward pass
    for i = 2 : n-1
        switch nn.activation_function 
            case 'sigm'
                % Calculate the unit's outputs (including the bias term)
                nn.a{i} = sigm(nn.a{i - 1} * nn.W{i - 1}');%a{2}  [x0 x] *[W0 =x0W0+Wx=bias+Wx=[100x328]*[328x200] =[100x200]
                                                           %               W]
            case 'tanh_opt'
                nn.a{i} = tanh_opt(nn.a{i - 1} * nn.W{i - 1}');
        end

        %dropout
        if(nn.dropoutFraction > 0)
            if(nn.testing)
                nn.a{i} = nn.a{i}.*(1 - nn.dropoutFraction);
            else
                nn.dropOutMask{i} = (rand(size(nn.a{i}))>nn.dropoutFraction);%随机0和1的矩阵
                nn.a{i} = nn.a{i}.*nn.dropOutMask{i};%将激活值以一定概率清为0（处理同denoise AE）
            end
        end

        %calculate running exponential activations for use with sparsity
        if(nn.nonSparsityPenalty>0)
            nn.p{i} = 0.99 * nn.p{i} + 0.01 * mean(nn.a{i}, 1);%p{i}初始化为0
            %mean(nn.a{i}, 1)求a各列的均值，得到[1x200]，为平均激活值（参见Sparse AE）
        end

        %Add the bias term
        nn.a{i} = [ones(m,1) nn.a{i}];
    end
    switch nn.output %计算输出层的值
        case 'sigm'
            nn.a{n} = sigm(nn.a{n - 1} * nn.W{n - 1}');
        case 'linear'
            nn.a{n} = nn.a{n - 1} * nn.W{n - 1}';
        case 'softmax'
            nn.a{n} = nn.a{n - 1} * nn.W{n - 1}';
            nn.a{n} = exp(bsxfun(@minus, nn.a{n}, max(nn.a{n},[],2)));
            nn.a{n} = bsxfun(@rdivide, nn.a{n}, sum(nn.a{n}, 2)); 
    end

    %error and loss
    nn.e = y - nn.a{n};%最后一层的误差

    switch nn.output
        case {'sigm', 'linear'}
            nn.L = 1/2 * sum(sum(nn.e .^ 2)) / m; %均方误差
        case 'softmax'
            nn.L = -sum(sum(y .* log(nn.a{n}))) / m;
    end
end
```

**nnbp.m**

```matlab
function nn = nnbp(nn)
%NNBP performs backpropagation
% nn = nnbp(nn) returns an neural network structure with updated weights 

    n = nn.n;
    sparsityError = 0;
    switch nn.output
        case 'sigm'
            d{n} = - nn.e .* (nn.a{n} .* (1 - nn.a{n}));%计算最后一层的残差d{n}（公式见bp算法第二步）
            %nn.e = y - nn.a{n};
            %nn.a{n} = sigm(nn.a{n - 1} * nn.W{n - 1}');

        case {'softmax','linear'}
            d{n} = - nn.e;
    end
    for i = (n - 1) : -1 : 2%从倒数第二层往前，计算残差d{i}
        % Derivative of the activation function
        switch nn.activation_function 
            case 'sigm'
                d_act = nn.a{i} .* (1 - nn.a{i});%sigm函数的导数
            case 'tanh_opt'
                d_act = 1.7159 * 2/3 * (1 - 1/(1.7159)^2 * nn.a{i}.^2);
        end

        if(nn.nonSparsityPenalty>0)
            pi = repmat(nn.p{i}, size(nn.a{i}, 1), 1);%平均激活值
            sparsityError = [zeros(size(nn.a{i},1),1) nn.nonSparsityPenalty * (-nn.sparsityTarget ./ pi + (1 - nn.sparsityTarget) ./ (1 - pi))];
        end

        % Backpropagate first derivatives
        if i+1==n % in this case in d{n} there is not the bias term to be removed  求倒数第二层的残差时，使用的最后一层残差d{n}没有偏置量           
            d{i} = (d{i + 1} * nn.W{i} + sparsityError) .* d_act; % 公式见bp算法第三步（Sparse AE）
        else % in this case in d{i} the bias term has to be removed
            d{i} = (d{i + 1}(:,2:end) * nn.W{i} + sparsityError) .* d_act;
        end

        if(nn.dropoutFraction>0)
            d{i} = d{i} .* [ones(size(d{i},1),1) nn.dropOutMask{i}];
        end

    end%从倒数第二层往前

    for i = 1 : (n - 1)%计算W的偏导数dW
        if i+1==n
            nn.dW{i} = (d{i + 1}' * nn.a{i}) / size(d{i + 1}, 1);%除以样本数量
        else
            nn.dW{i} = (d{i + 1}(:,2:end)' * nn.a{i}) / size(d{i + 1}, 1);      
        end
    end
end
```

**nnapplygrads.m**

```matlab
function nn = nnapplygrads(nn)
%NNAPPLYGRADS updates weights and biases with calculated gradients
% nn = nnapplygrads(nn) returns an neural network structure with updated
% weights and biases

    for i = 1 : (nn.n - 1)
        if(nn.weightPenaltyL2>0)%nn.dW已经除以了样本数量
            dW = nn.dW{i} + nn.weightPenaltyL2 * [zeros(size(nn.W{i},1),1) nn.W{i}(:,2:end)];
        else
            dW = nn.dW{i};
        end

        dW = nn.learningRate * dW;%批量梯度下降法中第三步

        if(nn.momentum>0)
            nn.vW{i} = nn.momentum*nn.vW{i} + dW;
            dW = nn.vW{i};
        end

        nn.W{i} = nn.W{i} - dW;
    end
end
```

参考资料： 

1、[Deep learning：四十一(Dropout简单理解)](http://www.cnblogs.com/tornadomeet/p/3258122.html)

2、[Deep learning：四十二(Denoise Autoencoder简单理解)](http://www.cnblogs.com/tornadomeet/p/3261247.html)




