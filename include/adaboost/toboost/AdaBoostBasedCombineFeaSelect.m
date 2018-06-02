% 基于 AdaBoost 的针对特征组合的特征选择
% 基本思想: 首先获得分类能力好的弱分类器,然后逐步加入其他特征,以获得效果较好的特征组合
% 直接调用AdaBoost训练分类器，然后通过分析AdaBoost分类器获得分类器所选择的特征
%
% 输入:
% feaSelectFeatures     进行特征选择的训练样本
% feaSelectY            进行特征选择的训练样本的类标
% T                     训练轮数
% SeleFeaSize           需要选择的特征数量
%
% 输出：
% SelectFeaNo           选择的特征向量
% costTime              特征选择过程中所花费的时间
% trainErrorRate        可选输出参数，特征选择过程中所达到的错误率
% overlapNum            可选输出参数，重复特征数量
%
function [hypothesis,alphaT,SelectFeaNo,costTime]=...
    AdaBoostBasedCombineFeaSelect(feaSelectFeatures,feaSelectY,T,selectFeaSize,varargin)

outputTips=0;
if( nargin==5 )
    outputTips = varargin{1};
end


% 通过AdaBoost训练获得分类器
if(selectFeaSize<=0)              % 若选择特征数量参数无效，则采用T控制循环
%     [hypothesis,alphaT,trainErrorRate,costTime]=AdaBoostTrain(feaSelectFeatures,feaSelectY,T);
    [hypothesis,alphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(feaSelectFeatures,feaSelectY,T);
else
%     [hypothesis,alphaT,trainErrorRate,costTime]=AdaBoostTrain(feaSelectFe
%     atures,feaSelectY,T,selectFeaSize);
     [hypothesis,alphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(feaSelectFeatures,feaSelectY,T,selectFeaSize);
end

% save('D:\我的文档\liuhaimei\mat\OMdata\trainErrorRate.mat','trainErrorRate')
% save('D:\我的文档\liuhaimei\mat\OMdata\TPRate.mat','TPRate')
% save('D:\我的文档\liuhaimei\mat\OMdata\FPRate.mat','FPRate')%将训练结果存放到指定文件夹中
save resultRate trainErrorRate TPRate FPRate;    %将训练结果存放到当前程序路径下，程序移动不需要更改存放路径
 
% 通过分析AdaBoost获得分类器所选择的特征
% SelectFeaNo是选出的特征向量;
% hypoFeaList是特征量与其出现次数的对应关系
% overlapNum是重复的特征数量
% 是否可以允许出现重复的特征？？？？？
[SelectFeaNo,hypoFeaList,overlapNum]=analysisHypothesisFeature(hypothesis,outputTips);

% 分析特征，获得选择特征的名称

% if(nargout==6)                   % 输出六个参数
%     varargout{1}=overlapNum;     % 重复特征数量
% end
% if(nargout==7)                   % 输出七个参数
%     varargout{1}=trainErrorRate; % 训练错误率
%     varargout{2}=overlapNum;     % 重复特征数量
% end

if(outputTips==1)
    disp('基于 AdaBoost 的特征选择...');
    disp(strcat('选择特征数量：',num2str(length(SelectFeaNo)),'.'));
    disp(strcat(' AdaBoost 训练轮数：',num2str(size(hypothesis,1)),'.'));
    disp(strcat('重复特征数量：',num2str(overlapNum),'.'));
    disp('---------------------------------------');
end
