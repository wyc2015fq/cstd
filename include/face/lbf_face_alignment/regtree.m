 function rt=regtree(Deltap,max_depth,dataSet,pixelpair)
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %函数功能：构建决策树
 %输入：
 %Deltap：归一化的形状偏差
 %max_depth：最大深度
 %dataSet：样本特征
 %pixelpair：特征对应的角度与极半径
 %输出：
 %rt：树模型
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        num_internode=2^max_depth - 1 - 2^(max_depth - 1);
        [m,n] = size(Deltap);
        max_numnodes = 2^max_depth - 1;%最大结点数
        ind_samples = cell(max_numnodes, 1);%结点存放样本的变量
        rtissplit     = zeros(max_numnodes, 1);%结点是否进行分支操作，0为分支，1为不分枝
        %rt.pnode       = zeros(max_numnodes, 1);%结点的父节点
        rtdepth       = zeros(max_numnodes, 1);%结点的深度
        %rt.cnodes      = zeros(num_internode, 2);%结点的子结点
        %rt.isleafnode  = zeros(max_numnodes, 1);%结点是否是叶结点
        rt.feat        = zeros(num_internode, 4);%结点的特征参数
        rt.thresh      = zeros(num_internode, 1);%结点的阈值
        %根节点初始化如下：
        ind_samples{1} = 1:m;
        rtissplit(1)     = 0;
        %rt.pnode(1)       = 0;
        rtdepth(1)       = 1;
        %rt.cnodes(1, 1:2) = [0 0];
        %rt.isleafnode(1)  = 1;
        rt.feat(1, :)     = zeros(1, 4);
        rt.thresh(1)      = 0;
        
        num_nodes = 1;%结点数初始化为1
        num_leafnodes = 1;%叶节点数初始化为1
        stop = 0;%循环控制参数
        while(~stop)
            num_nodes_iter = num_nodes;
            num_split = 0;
            for n = 1:num_nodes_iter
                if ~rtissplit(n)
                    if rtdepth(n) == max_depth
                        rtissplit(n) = 1;
                    else
                         %未达到最大层数，则进行以下操作
                         [thresh, feat, lcind, rcind] = splitnode(ind_samples{n},Deltap,dataSet,pixelpair,rtdepth(n),max_depth);
                        %父节点赋值
                        %rt.feat(n, :)   = feat;
                        rt.feat(n, :)   = [cos(feat(1))*feat(3),cos(feat(2))*feat(4),sin(feat(1))*feat(3),sin(feat(2))*feat(4)];
                        rt.thresh(n)    = thresh;
                        rtissplit(n)   = 1;
                        %rt.cnodes(n, :) = [num_nodes+1 num_nodes+2];
                        %rt.isleafnode(n)   = 0;
                        %子节点赋值
                        ind_samples{num_nodes+1} = lcind;
                        rtissplit(num_nodes+1)     = 0;
                        %rt.pnode(num_nodes+1)       = n;
                        rtdepth(num_nodes+1)       = rtdepth(n) + 1;
                        %rt.cnodes(num_nodes+1, :)   = [0 0];
                        %rt.isleafnode(num_nodes+1)     = 1;
                        
                        ind_samples{num_nodes+2} = rcind;
                        rtissplit(num_nodes+2)     = 0;
                        %rt.pnode(num_nodes+2)       = n;
                        rtdepth(num_nodes+2)       = rtdepth(n) + 1;
                        %rt.cnodes(num_nodes+2, :)   = [0 0];
                        %rt.isleafnode(num_nodes+2)  = 1;
                        
                        num_split = num_split + 1;        %结点达到最大层时，不再进行此操作
                        num_leafnodes = num_leafnodes + 1;%叶节点数更新
                        num_nodes     = num_nodes + 2;    %结点数更新
                    end
                end
            end
            
            if num_split == 0
                stop = 1;
            else
                %rt.num_leafnodes = num_leafnodes;
                %rt.num_nodes     = num_nodes;           
                %rt.id_leafnodes = find(rt.isleafnode == 1); 
            end            
        end
end

 function [thresh, feat, lcind, rcind] = splitnode(ind_samples,Deltap,dataSet,pixelpair,depth,max_depth)
 %函数功能：
 %确定最佳阈值，最佳特征，以及数据划分
 
%首先判断结点的样本集是否为空
if isempty(ind_samples)
    thresh = 0;
    feat = [0 0 0 0];
    rcind = [];
    lcind = [];
    return;
end

    Tr_Data=dataSet(ind_samples,:);%结点所有样本的特征   
    DeltaS=Deltap(ind_samples,:);
    m = size(Tr_Data);%数据集的大小 
    bestS = inf;%方差初始化,无穷大
      
    %找到最佳的位置和最优的值  
    for j = 1:m(:,2)%得到特征列数 
        [datasort,dataindex]=sort(Tr_Data(:,j));    %第j个特征，所有样本值从小到大排序
        deltas=DeltaS(dataindex,:);                 %排序后对应的点偏差
        [norepdata,norepindex]= unique(datasort);   %剔除排序后重复的值
        
        %这段是随机找阈值
        ki=ceil(length(norepindex)*0.25);ko=ceil(length(norepindex)*0.75);
        pp=norepindex(ki:ko);
        i=pp(randint(1,1,[1 length(pp)]));
        indexn=find(i==pp);
        threshn = norepdata(indexn+ki-1);
        %%%完%%%%%
        
        %这段是对每层及每个结点中的样本数有个数目约束
        lnum = sum(Tr_Data(:,j) <= threshn);
        rnum = sum(Tr_Data(:,j) > threshn);
        if lnum<2^(max_depth-depth)||rnum<2^(max_depth-depth),continue;end;%限制每层最低样本数目
        %%%完%%%%%
        
        %第j个特征，随机阈值的权重方差
        S=[var(deltas(1:i,1))+var(deltas(1:i,2))]*i*length(datasort)^-1+[var(deltas(i+1:end,1))+var(deltas(i+1:end,2))]*(length(datasort)-i)*length(datasort)^-1;
        if S < bestS  
             bestS = S;%最佳方差重新赋值
             fea = j;  %最佳的特征列
             thresh = threshn;%最佳的阈值
        end
        
    end 
    
    lcind = ind_samples(Tr_Data(:,fea) <= thresh);%左结点
    rcind = ind_samples(Tr_Data(:,fea) > thresh); %右结点
    feat=pixelpair(fea,:);%特征参数
end
