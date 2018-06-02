% AdaBoost 强学习算法 对一组样本进行分类，获得AdaBoost算法对样本的预测输出
% AdaBoostClassfy 与 AdaBoostDecisionForSample 类似，其不同之处在于
% AdaBoostClassfy 判别一组样本, AdaBoostDecisionForSample 判别单个样本类别
% AdaBoostClassfy  输入的是所有待分类样本的特征值
% AdaBoostDecisionForSample 输入的是单个样本的特征值 
% 输入：
% Samples      待分类的样本, cntSamples x cntFeatures 矩阵
%              cntSamples    待分类的样本的数量
%              cntFeatures   特征空间维数
% Hypothesis   给定的AdaBoost强分类器,由T个弱分类器组成
% AlphaT       AdaBoost分类器的权重
% T            分类过程中使用的AdaBoost弱分类器的数量
% boostthresh  AdaBoost强分类器的阈值，默认为0.5
% 输出:
% predictOutput      AdaBoost对每个样本的预测输出，其值为0或者1
%                    行向量，1 x cntSamples
% predictConfidence  AdaBoost对每个样本的预测输出的置信度，值在[0 1]间
%                    行向量，1 x cntSamples
% 调用格式:
% [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,AlphaT,T)
% [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,AlphaT,T,boostThresh)
% 
% function [predictOutput,predictConfidence]=AdaBoostClassfy(Samples,Hypothesis,weak,AlphaT,T)
function [predictOutput]=AdaBoostClassfy(Samples,Hypothesis,weak,AlphaT,T)
cntSamples=size(Samples,1);        % 待分类的样本的数量
predictOutput=zeros(1,cntSamples); % 每个样本对强分类器的判别输出，0或者1
Hypothesis=Hypothesis(1:T,:);      % 取前T个弱分类器 
weak=weak(1:T,:);
AlphaT=AlphaT(1:T);                % 前T个弱分类器权重
for i=1:cntSamples              
    h=zeros(1,T);                  % 所有分类器每个样本的分类器输出 
    hzong1(i)=0;  
    for t=1:T
           j=Hypothesis(t,1);           % 第t个弱分类器的 特征列   
            score=weak{t,1};    
%             index=weak{t,2};
%             alpha=AlphaT(t);  
%             score=score(index);
            hzong1(i)=hzong1(i)+score(i);
           
     end
%      tempH=sum(AlphaT.*h);      
%      if(tempH>=(boostthresh*sum(AlphaT)))          % 将T个弱分类器组成强分类器,并输出样本i的类别 
%           predictOutput(i)=1;
%      end
%      predictConfidence(i)=tempH/(sum(AlphaT)+eps); % 置信度
% %       if(hzong1(i)>0)
% %           predictOutput(i)=1;
% %       end
%       if(hzong2(i)<hzong1(i))
%           predictOutput(i)=1;
%       end
%    allhzong(i)=hzong1(i);
end
sortall=sort(hzong1(1:100));  %对600个正样本得分进行排序 
for i=1:cntSamples  
    if (hzong1(i)>sortall(1))
         predictOutput(i)=1;
    end
end


