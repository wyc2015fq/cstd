# 计量经济与时间序列_ADF单位根检验步骤 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [计量经济与时间序列_ADF单位根检验步骤](https://www.cnblogs.com/noah0532/p/8753005.html)





1　　ADF检验也叫扩展的迪克富勒检验，主要作用是检测序列的平稳性，也是最常用检测序列平稳性的检验方法。

2　　何为：平稳性？单位根？(略)，见这部分随便的其他内容有讲解。是建模对数据的先决条件。

3　　ADF检验的三种情形：

![](https://images2018.cnblogs.com/blog/1328368/201804/1328368-20180409012932803-2022064151.png)

4　　在MATLAB中常用的adf检验的操作：

4.1　　经过差分使序列平稳。

**　　% 如果结果h=0，表示拒绝原假设，数据不平稳**
**　　% 如果结果h=1，表示不拒绝原假设，数据平稳**

**　　% 注意这里和上图不一样，原假设为平稳1,。**

%% 单位根检验实例
% Load Canadian inflation rate data.
load Data_Canada
Y = DataTable.INF_C;
% Test the time series for a unit root.
h = adftest(Y);
disp(h);
% 显示结果：
% h =
%   logical
%    0   % 非平稳
% 如果结果h=0，表示拒绝原假设，数据不平稳
% 如果结果h=1，表示不拒绝原假设，数据平稳

%% 使序列进行平稳，进行一阶差分
Y1 = diff(Y);  % 经过一阶差分
h1 = adftest(Y1);
disp(h1);
% h1 =
%   logical
%    1  % 平稳
% 如果结果h=0，表示拒绝原假设，数据不平稳
% 如果结果h=1，表示不拒绝原假设，数据平稳




4.2　　adf检验的其他输出内容，详细见MATLAB，doc中的内容

%% 另外，adf检验还有其他的输出内容。
[h2,pValue,stat,cValue,reg] = adftest(Y1);
% h2 =
%   logical
%    1
% pValue =    % p值
%    1.0000e-03
% stat =
%    -4.8186
% cValue =
%    -1.9478
% reg = 
%   包含以下字段的 struct:
% 
%        num: 40
%       size: 39
%      names: {'a'}
%      coeff: 0.2285
%         se: 0.1601
%        Cov: 0.0256
%     tStats: [1×1 struct]
%      FStat: [1×1 struct]
%        yMu: 0.0028
%     ySigma: 1.5513
%       yHat: [39×1 double]
%        res: [39×1 double]
%     DWStat: 1.8378
%        SSR: 4.6502
%        SSE: 86.7950
%        SST: 91.4452
%        MSE: 2.2841
%       RMSE: 1.5113
%        RSq: 0.0509
%       aRSq: 0.0509
%         LL: -70.9449
%        AIC: 143.8898  % AIC信息准则
%        BIC: 145.5533  % BIC信息准则
%        HQC: 144.4866  % HQC信息准则




4.3　　据上图，三种情形在MATLAB的adftest中的选择。

%% 输入adf检验其他的内容
[h3,~,~,~,reg3] = adftest(Y1,'model','AR');
[h4,~,~,~,reg4] = adftest(Y1,'model','ARD');
[h5,~,~,~,reg5] = adftest(Y1,'model','TS');
disp([h3,h4,h5]);
disp([reg3.AIC,reg4.AIC,reg5.AIC;reg3.BIC,reg4.BIC,reg5.BIC...
    ;reg3.HQC,reg4.HQC,reg5.HQC]);
% 'model','AR'；表示无截距项，无趋势项的单位根检验
% 'model','ARD'；表示有截距项，无趋势项的单位根检验
% 'model','TS'；表示由截距项，有趋势项的单位根检验
% 另外，AIC,BIC,HQC准则在reg结构体内，可以调用用值观察那种情形平稳
% **默认为'AR'形式，也就是不含截距项，不含趋势项的单位根检验**



4.4　　如果选择滞后项，可以用用户设定的方式，如果滞后项设定为0，为DF检验。建议用户这里不用自己设定

%% 滞后项
[h6,~,~,~,reg6] = adftest(Y1,'model','AR','lags',0); %DF检验形式
[h7,~,~,~,reg7] = adftest(Y1,'model','AR','lags',[0,1,2]);% 也可以对趋势项进行遍历




5　　检验流程

![](https://images2018.cnblogs.com/blog/1328368/201804/1328368-20180409020435052-1448581928.png)














