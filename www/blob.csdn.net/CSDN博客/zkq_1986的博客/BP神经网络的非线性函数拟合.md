# BP神经网络的非线性函数拟合 - zkq_1986的博客 - CSDN博客





2016年08月04日 11:02:02[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：10374








BP神经网络主要用到newff,sim 和train3个神经网络函数,各函数解释如下。

## 1.newff:BP神经网络参数设置函数

函数功能:构建一个BP神经网络。 

函数形式:net= newff(P,T,S,TF,BTF,BLF,PF,IPF,OPF,DDF) 

P:输入数据矩阵。 

T:输出数据矩阵。 

S:隐含层节点数。 

TF:节点传递函数,包括硬限幅传递函数hardlim,对称硬限幅传递函数hardlims,线性传 

递函数purelin,正切S型传递函数tansig,对数S型传递函数logsig。 

BTF:训练函数,包括梯度下降BP算法训练函数traingd,动量反传的梯度下降BP算法 

训练函数traingdm,动态自适应学习率的梯度下降BP算法训练函数traingda,动量反传和动 

态自适应学习率的梯度下降BP算法训练函数traingdx,Levenberg_Marquardt的BP算法训 

练函数trainlm。 

BLF:网络学习函数,包括BP学习规则learngd,带动量项的BP学习规则learngdm。 

北京航空航天大学出版社 

PF:性能分析函数,包括均值绝对误差性能分析函数mae,均方差性能分析函数mse。 

IPF:输入处理函数。 

OPF:输出处理函数。 

DDF:验证数据划分函数。 

一般在使用过程中设置前面6个参数,后面4个参数采用系统默认参数。
## 2.train:BP神经网络训练函数

函数功能:用训练数据训练BP神经网络。 

函数形式:[net,tr]=train(NET,X,T,Pi,Ai) 

NET:待训练网络。 

X:输入数据矩阵。 

T:输出数据矩阵。 

Pi:初始化输入层条件。 

Ai:初始化输出层条件。 

net:训练好的网络。 

tr:训练过程记录。 

一般在使用过程中设置前面3个参数,后面2个参数采用系统默认参数。
## 3.sim:BP神经网络预测函数

函数功能:用训练好的BP神经网络预测函数输出。 

函数形式:y=sim(net,x) 

net:训练好的网络。 

x:输入数据。 

y:网络预测数据。
# 2 多隐藏层神经网络

多隐藏层的效果要优于单隐藏层。从运行时间和预测精度两个方面比较单隐含层BP神经网络和双隐含层BP神经网络的性能,网络结构相同,训练迭代都为100次,比较10次预测结果平均值,比较结果如表1所列。

表1 BP神经网络预测结果比较
|网络类别|预测误差百分比|均方误差|运行时间/s|
|----|----|----|----|
|单隐藏层BP神经网络|1.64%|0.0076|8.0053|
|双隐藏层BP神经网络|1.60%|0.0016|9.8592|

# 3 隐藏层节点数

如果隐藏层含节点数太少,BP神经网络不能建立复杂的映射关系,网络预测误差较大。但是如果节点数过多,网络学习时间增加,并且可能出现“过拟合”现象,就是训练样本预测准确,但是其他样本预测误差较大。不同隐藏层节点数BP神经网络预测误差如表2所列。 

表2 2 不同隐藏层节点数BP神经网络预测误差
|隐藏层节点数|3|4|5|6|7|8|9|
|----|----|----|----|----|----|----|----|
|相对误差百分比|5.46%|1.75%|1.64%|0.32%|0.31%|0.29%|0.08%|
|均方误差|0.0094|0.0131|0.0076|0.0012|0.0004|0.0002|0.0001|

# 4 激活函数

MATLAB神经网络工具箱中newff函数提供了几种节点转移函数,主要包括以下三种。

```
1)logsig函数:
                  y =1/[1-exp(-x)]                        (2 2)
2)tansig函数:
                 y =2/[1-exp(-2x)]-1                     (2 3)
3)purelin函数:
                   y =x                                  (2 4)
```

在网络结构和权值、阈值相同的情况下,BP神经网络预测误差和均方误差、输出层节点转移函数的关系如表2 4所列。 

表3不同转移函数对应预测误差
|隐含层函数|输出层函数|误差百分比|均方误差|
|----|----|----|----|
|logsig|tansig|40.63%|0.9025|
|logsig|purelin|0.08%|0.0001|
|logsig|logsig|352.65%|181.2511|
|tansig|tansig|31.90%|1.1733|
|tansig|logsig|340.90%|162.9698|
|tansig|purelin|1.70%|0.0107|
|purelin|logsig|343.36%|143.76334|
|purelin|tansig|120.08%|113.0281|
|purelin|purelin|196.49%|99.0121|

从表3可以看出,隐含层和输出层函数的选择对BP神经网络预测精度有较大影响。一般隐含层节点转移函数选用logsig函数或tansig函数,输出层节点转移函数选用tansig或purelin函数。

# 5 网络拟合的局限性

BP神经网络虽然具有较好的拟合能力,但其拟合能力不是绝对的,对于一些复杂系统,BP神经网络预测结果会存在较大误差。比如对于以下函数拟合效果较差： 


${y = (x^2_1+x^2_2)^{0.25}（sin^2[50(x^2_1+x^2_2)^{0.1}]+1）}$

# 6 案例

```
hiddenLayerSize = hiddenLayerSize_start+2*(j-1);
    for i = 1:1000
    %     hiddenLayerSize = 20;
        net = fitnet(hiddenLayerSize);

        % Choose Input and Output Pre/Post-Processing Functions
        % For a list of all processing functions type: help nnprocess
    %     net.inputs{1}.processFcns = {'removeconstantrows','mapminmax'};
    %     net.outputs{2}.processFcns = {'removeconstantrows','mapminmax'};
        net.inputs{1}.processFcns ={};
        net.outputs{2}.processFcns = {};

        % Setup Division of Data for Training, Validation, Testing
        % For a list of all data division functions type: help nndivide
        net.divideFcn = 'dividerand';  % Divide data randomly
        net.divideMode = 'sample';  % Divide up every sample
        net.divideParam.trainRatio = 90/100;
        net.divideParam.valRatio = 5/100;
        net.divideParam.testRatio = 5/100;

        % For help on training function 'trainlm' type: help trainlm
        % For a list of all training functions type: help nntrain
        net.trainFcn = 'trainlm';  % Levenberg-Marquardt

        % Choose a Performance Function
        % For a list of all performance functions type: help nnperformance
        net.performFcn = 'mse';  % Mean squared error

        % Choose Plot Functions
        % For a list of all plot functions type: help nnplot
        net.plotFcns = {'plotperform','plottrainstate','ploterrhist', ...
          'plotregression', 'plotfit'};


        % Train the Network
        [net,tr] = train(net,inputs,targets);

        % Test the Network
        outputs = net(inputs);
        errors = gsubtract(targets,outputs);
        performance = perform(net,targets,outputs);

        % Recalculate Training, Validation and Test Performance
        trainTargets = targets .* tr.trainMask{1};
        valTargets = targets  .* tr.valMask{1};
        testTargets = targets  .* tr.testMask{1};
        trainPerformance = perform(net,trainTargets,outputs);
        valPerformance = perform(net,valTargets,outputs);
        testPerformance = perform(net,testTargets,outputs);

        neural_ret=outputs';
        e=rmse(neural_ret,y);
    %     Spearman_corr=corr(y,neural_ret,'type','Spearman')
        if min_error > e
            min_error = e;
            ott_best_net=net;
            max_corr=corr(y,neural_ret,'type','Spearman');
        end
    end
    max_corr;
```

# matlab中的阈值

matlab中阈值b是控制是否激活该神经元，如果大于阈值就激活，否则就不激活。






