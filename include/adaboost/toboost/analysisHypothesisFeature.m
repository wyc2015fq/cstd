% 分析每个获取的弱假设中的特征分布，按特征出现的次数由多到少排名
% 例如输入：[selectFeaSequence,hypoFeaList,overlapNum]=analysisHypothesisFeature(hypothesis)
%          输入: hypothesis是待分析的假设
%          返回：selectFeaSequence是选出的特征向量;hypoFeaList是特征量与其出现次数的对应关系
%               overlapNum是重复的特征数量

function [selectFeaSequence,hypoFeaList,overlapNum]=analysisHypothesisFeature(varargin)
if nargin==1              
    hypothesis=varargin{1};    
    isDisplay=1;               %默认是显示特征分析
elseif(nargin==2)
    hypothesis=varargin{1};
    isDisplay=varargin{2};
else
    error('输入参数错误！返回.');
end
    

% 输入多个假设，则分析其异同
%  hypothesis  阈值     偏置 特征列
hypoNum=size(hypothesis,1); % 弱假设数量
hypoFeaList=zeros(hypoNum,2); % 统计特征出现次数
hypoFeaList(:,1)=hypothesis(:,1);%弱假设矩阵中表示特征列所在的行为第一行
hypoFeaList(:,2)=1;

% 寻找出相同的特征列，并统计相同的次数.
% 其中：hypoFeaList(:,1)是特征列序号
% hypoFeaList(:,2)是特征列序号重复的次数，以第一次出现为准进行统计
% 如：没有统计之前的 hypoFeaList  和统计之后的 hypoFeaList
%  hypoFeaList(统计前)    hypoFeaList(统计后)
%      1     1                  1     3
%      2     1                  2     2
%      9     1                  9     1
%      2     1                  2     0
%      8     1                  8     1
%      5     1                  5     1
%     10     1                 10     1
%      1     1                  1     0
%      4     1                  4     1
%      1     1                  1     0
% Noted by Li.12.03.2013
for i=1:(hypoNum-1)
    for j=(i+1):hypoNum
        if ((hypoFeaList(i,2)~=0)&&(hypoFeaList(j,2)~=0)&&(hypoFeaList(i,1)==hypoFeaList(j,1)))
            hypoFeaList(i,2)=hypoFeaList(i,2)+1;
            hypoFeaList(j,2)=0;
        end
    end
end

    
overlapIndex=find(hypoFeaList(:,2)==0);   % 重复的特征
overlapNum=length(overlapIndex);          % 重复的特征数量
hypoFeaList(overlapIndex,:)=[];           % 删除重复的特征编号

hypoFeaList=sortrows(hypoFeaList,2); % 按照特征出现的次数由多到少排序
if(isDisplay==1)
    overlapInfo=strcat('重复的特征数量:',num2str(overlapNum));
    disp(overlapInfo); % 显示重复的特征的名字
end
hypoFeaList=sortrows(hypoFeaList,1);  % 将特征索引由小到大排列
selectFeaSequence=hypoFeaList(:,1).'; % 保存选择的特征索引值
    