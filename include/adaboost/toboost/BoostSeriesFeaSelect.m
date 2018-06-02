% 基于 AdaBoost 特征选择
% 参数selectFeaSize有效时，选取指定数量的特征
% 否则，按轮数T循环
% features: 训练特征
% Y: 训练样本标号
% feaSelectRate: 选择样本占总样本比例
% trainingRate: 训练样本占总样本比例
% T: 训练轮数
% selectFeaSize: 要选择的特征数目
% testTimes: 测试次数
% outputTips:
function [SelectFeaNo costTime Hypothesis AlphaT]=BoostSeriesFeaSelect(features,Y,T,selectFeaSize,outputTips)

[Hypothesis,AlphaT,SelectFeaNo,costTime]=BoostSeriesFeaSelectCmpOnce(features,Y,T,selectFeaSize,outputTips);

save('D:\我的文档\liuhaimei\mat\OMdata\Hypothesis.mat','Hypothesis') % Hypothesis   给定的AdaBoost强分类器,由T个弱分类器组成
save('D:\我的文档\liuhaimei\mat\OMdata\AlphaT.mat','AlphaT')% AlphaT     AdaBoost分类器的权重 
save('D:\我的文档\liuhaimei\mat\OMdata\SelectFeaNo.mat','SelectFeaNo') %   选择的特征向量
save('D:\我的文档\liuhaimei\mat\OMdata\costTime.mat','costTime') 
% 进行特征选择，然后进行分类 
function [hypothesis alphaT SelectFeaNo costTime]...
    =BoostSeriesFeaSelectCmpOnce(features,Y,T,selectFeaSize,outputTips)
    feaSelectFeatures=features;  % 获得特征选择数据
    feaSelectY=Y;                % 特征选择数据的类标
    
    [hypothesis,alphaT,SelectFeaNo,costTime]=AdaBoostBasedCombineFeaSelect(feaSelectFeatures,feaSelectY,T,selectFeaSize,outputTips);
    save feaselect602 hypothesis alphaT SelectFeaNo costTime ;
%     [trainError,testError,testTP,testFP,Hypothesis,AlphaT,classfyCostTime]=AdaBoostAfterFeaSelect(classfyFeatures,classfyY,trainingRate,T,SelectFeaNo);




