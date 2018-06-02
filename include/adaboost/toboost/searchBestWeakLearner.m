% 在特征列上获得最优的阈值分类器
% searchBestWeakLearner
%而KL-adboost是通过Kl散度来定义正负样本的特征直方统计图
% 输入:
% FeatureVector 特征向量，查找最佳弱分类器的特征列;列向量      
% Y             每个样本所属类别的标识;列向量
% weight        权重向量,存放当前每个样本的权重值;列向量
% [bestError,bestThresh,bestBias]=searchBestWeakLearner(FeatureVector,Y,weight)
function [auc,score]=searchBestWeakLearner(FeatureVector,Y,weight)
% function [auc,minfea,disval,score]=searchBestWeakLearner(FeatureVector,Y,weight)
error(nargchk(3,3,nargin));         % 输入3个参数,否则中止程序
% 检查输入特征向量与类标需为列向量
iptcheckinput(FeatureVector,{'logical','numeric'},{'column','nonempty','real'},mfilename, 'FeatureVector',1);
iptcheckinput(Y,{'logical','numeric'},{'column','nonempty','integer'},mfilename, 'Y',2);
iptcheckinput(weight,{'numeric'},{'column','nonempty','real'},mfilename, 'weight',3);

cntSamples=length(FeatureVector);    % 样本容量
if( length(Y)~=cntSamples || length(weight)~=cntSamples ) % 检查长度
    error('特征向量、样本类标、与样本权重必须具备相等的长度.');
end
%%%%%%%%
% % [FeatureVector,Findex]=sort(FeatureVector);
% % weight=weight(Findex);
% % Y=Y(Findex);
% [pfeature,pindex]=sort(pfeature);
% pweight=pweight(pindex);
% [nfeature,nindex]=sort(nfeature);
% nweight=nweight(nindex);
positiveCols= find(Y==1);        % 正训练样本标号
negativeCols= find(Y==0);        % 负训练样本标号
pweight=weight(positiveCols);%正样本权重
nweight=weight(negativeCols);%负样本权重
pfeature=FeatureVector(positiveCols);%正样本特征
nfeature=FeatureVector(negativeCols);%负样本特征
%%%%核密度估计算法%%%%%%%%%%%%%%%%%%%%%%%
po=zeros(1,cntSamples);
ne=zeros(1,cntSamples);

%%%%
for i=1:cntSamples
%        po(i)=kerneldistri(FeatureVector(i),pfeature,pweight);
%        ne(i)=kerneldistri(FeatureVector(i),nfeature,nweight);
         po(i)=kerneldistri(FeatureVector(i),pfeature);
         ne(i)=kerneldistri(FeatureVector(i),nfeature);       
end
% po=po./length(FeatureVector);
% ne=ne./length(FeatureVector);
score=1/2*log((po+eps)./(ne+eps));
% score=2./(1+exp(-8*(po-ne)))-1;
% score=score(Findex);
auc=newroc(score,Y,weight);
% score=score(Findex);
% Y=Y(Findex);
% weight=weight(Findex);

