
function [Hypothesis,AlphaT,trainErrorRate,costTime,TPRate,FPRate]=trainAdaBoostLearner(X,Y,T,varargin)
error(nargchk(3,4,nargin)); % 必须输入3-4个参数,否则中止程序
iptcheckinput(X,{'numeric'},{'2d','real','nonsparse'}, mfilename,'X',1);
iptcheckinput(Y,{'logical','numeric'},{'row','nonempty','integer'},mfilename, 'Y', 2);
iptcheckinput(T,{'numeric'},{'row','nonempty','integer'},mfilename, 'T',3);
if( length(T) > 1 )              % 指定训练轮数的参数T长度应为1（不能为向量）
    error(['指定训练轮数的参数(T)长度(' num2str(length(T)) ')应为1(不能为向量).']);
end
[cntSamples,cntFeatures]=size(X); % cntSamples  训练数据集中样本数量
inverseControl=0;           % 控制循环退出标识，为0表示训练T轮，否则需要训练出指定数量的特征
cntSelectFeatures=0;        % 需要选择的特征数量 
if( nargin>3 )              % 若输入4个参数，则表示不会依照训练轮数为T的规则
    cntSelectFeatures=varargin{1};
    inverseControl=1;
    iptcheckinput(cntSelectFeatures,{'numeric'},{'row','nonempty','integer'},mfilename, 'cntSelectFeatures',4);
    if( length(cntSelectFeatures) > 1 ) % 指定特征数量的参数长度应为1（不能为向量）
        error(['指定特征数量的参数(cntSelectFeatures)长度(' num2str(length(cntSelectFeatures)) ')应为1.']);
    end
    if( cntSelectFeatures>=cntFeatures )
        error('需要选择的特征数量(cntSelectFeatures)过大！');
    end
end
if( cntSamples~=length(Y) ) % 训练样本X必须被监督
    error('训练样本X长度必须与类标向量长度相同') ;
end
computeCosttimeFlag=1;      % 计时标识符，为1表示对训练时间计时
if(computeCosttimeFlag==1)
    tic
end
X=ceil(X*10000)/10000;          % 训练数据整理：删除尾部数据，防止产生误差
positiveCols=find(Y==1);        % 正训练样本标号
negativeCols=find(Y==0);        % 负训练样本标号
if(isempty(positiveCols))     % 检查正样本数
    error('正样本数量为0,无法训练.');
end
if(isempty(negativeCols))     % 检查负样本数
    error('负样本数量为0,无法训练.');
end
weight=ones(cntSamples,1);       % 权值向量;列向量;一行代表一个样本的权重
weight(positiveCols)=1/(2*length(positiveCols));      % 正训练样例的初始权值
weight(negativeCols)=1/(2*length(negativeCols));      % 负训练样例的初始权值
AlphaT=zeros(1,T);              % T轮训练获取的每个弱假设的权值
trainErrorRate=zeros(1,T);      % 第 1 轮至第 T 轮的训练错误率
costTime=zeros(1,T);            % 第 1 轮至第 T 轮花费的时间
trainOutput=zeros(1,cntSamples); % 临时变量:使用前t个分类器对训练数据进行分类的结果  
h=zeros(1,cntSamples);           % 临时变量:使用第t轮最优分类器进行分类 的结果      
TPRate = zeros(1,T);             % 第1至T轮检测率
FPRate = zeros(1,T);             % 第1至T轮误检率
t=1; 
curFeaSize=0;                   % 当前已经选择的特征的数量
count_time=0;
%%%%%%%%%
while(true)                     % 训练轮数
    t_times=cputime;
    minauc=10000;
    weight=weight/(sum(weight));% 权值归一化
    for j=1:cntFeatures         % 对每个特征值,获取最优的分类器(假设)
        [tempauc,tempscore]=searchBestWeakLearner(X(:,j),Y',weight); % 搜索获取j列最优的假设
         if(tempauc<minauc)                        % 选择特征列对应组成的分类器
              minauc=tempauc;   %对应特征列上的样本统计直方图K-L散度值   
              score=tempscore;
%               index=tempindex;
%               tweight=tempweight;
                Hypothesis(t,:)=[j];
%               Hypothesis(t,:)=[j,value,histdisval];  % 第t轮 最优弱分类器
              weak{t,1}=score; 
           %%weak{t,2}=index; 
%          weak{t,3}=tweight;
        end
    end
    [h]=AdaBoostWeakLearnerClassfy(X, Hypothesis(t,:),weak(t,:));   % 使用第t轮最优弱分类器进行分类   
%     h=predict;
    errorRate=sum(weight(h~=Y));                 % 计算第t轮分类错误率  
%     AlphaT(t)=aw;    % 计算第t轮权重
    AlphaT(t)=0.5*log((1-errorRate)/(errorRate+eps));    % 计算第t轮权重   
%       AlphaT(t)=(1+(0.6).^t)*log((1-errorRate)/(errorRate+eps));
%     if(errorRate>eps)                                  % 减小被正确分类的样本的权值
% %         % 这里是否会存在这样的问题，当某次训练误差为0的时候，权重不再更新，导致没法后续训练
% %         % 按照道理，训练误差是一个波动下降的过程，某次为0并不代表紧接下来的下一次是0.  12.03.2013
%         weight(h==Y)=weight(h==Y)*(errorRate/(1-errorRate)); 
          weight(h==Y)=weight(h==Y)*exp(- AlphaT(t));
%           weight(h~=Y)=weight(h~=Y)*exp(AlphaT(t));
           
%     end
%          weight=weight*exp(-AlphaT(t).*h.*Y);
    % 下面是计算当前轮数(第t轮)下训练错误率
    [trainOutput]=AdaBoostClassfy(X,Hypothesis,weak,AlphaT,t);% yes!2013.04.09
    [curErrorRate,curTPRate,curFPRate]=calPredictErrorRate(Y,trainOutput);
    trainErrorRate(t) = curErrorRate;
    TPRate(t) = curTPRate;
    FPRate(t) = curFPRate;
   
    if(inverseControl==0)            % 采用训练轮数限制循环
        if(computeCosttimeFlag==1)
            costTime(t)=toc;
        end
        if(t>=T)                     % 达到期望的训练轮数                       
             break;
        end
    else                             % 采用提取的特征数量限制循环
        [SelectFeaNo]=analysisHypothesisFeature(Hypothesis(1:t,:),0);
        if( length(SelectFeaNo)>curFeaSize )
            curFeaSize=length(SelectFeaNo);
            if( computeCosttimeFlag==1 )
                costTime(curFeaSize)=toc;
            end
        end
        if( curFeaSize>=cntSelectFeatures )% 达到期望的特征数量
            break;
        end
    end
    elaspetime=cputime-t_times;
    count_time=count_time+elaspetime;
    disp(['已经筛选了 ',num2str(t),' 个特征','还剩余 ',num2str(T-t),' 个特征'])
    disp(['该特征消耗时间：',num2str(elaspetime),' 秒'])
    disp(['到目前总共消耗时间：',num2str(count_time),' 秒'])
     %%%%显示每次选择的识别率和误识别率
     disp(['第',num2str(t),'次训练错误率trainErrorRate为：',num2str(trainErrorRate(t))]);
     disp(['第',num2str(t),'次检测率TPRate为：',num2str(TPRate(t))]);
     disp(['第',num2str(t),'次误识别率FPRate为：',num2str(FPRate(t))]);
     disp(['选出的第',num2str(t),'个特征序号为：',num2str(Hypothesis(t,1))]);
     %%%%%%%%%%%%%%%%%%%%%%
    fprintf('\n')
    t=t+1;
end

if(computeCosttimeFlag==1)
    costTime=costTime(costTime~=0);% 输出训练时间
else
    costTime=0;
end

if(t<T)     
    Hypothesis=Hypothesis(1:t,:);      % 强分类器
    AlphaT=AlphaT(1:t);                % 强分类器权重
    trainErrorRate=trainErrorRate(1:t);% 训练错误率
end

