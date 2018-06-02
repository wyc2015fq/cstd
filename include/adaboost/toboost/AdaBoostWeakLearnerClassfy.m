% AdaBoost 弱假设对一组样本进行分类，获得AdaBoost弱假设对所有样本的预测输出
% AdaBoost 弱假设为二值分类器
% AdaBoostWeakLearnerClassfy
% 输入：
% Samples      待分类的样本, cntSamples x cntFeatures 矩阵
%              cntSamples    待分类的样本的数量
%              cntFeatures   特征空间维数
% weakLearner  给定的弱分类器，此处采用的是二值分类器
% 输出:
% predictOutput  AdaBoost弱分类器对每个样本的预测输出，其值为0或者1
%                输出行向量，1 x cntSamples
function [predictOutput]=AdaBoostWeakLearnerClassfy(Samples,hypo,weak)
j= hypo(1);              %特征列序号
val=Samples(:,j);        %对应特征列上的样本
nsample=length(val);
score=weak{1};         %每个样本的得分分布
% index=weak{2};
% score=score(index);
% weight=weak{3};
% Y=Y(index);
% weight=weight(index);
for i=1:nsample
   predictOutput(i)=score(i)>0;
end
% erroRate=sum(weight(predictOutput~=Y));

