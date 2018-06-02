function [tree,data,err] = binaryTreeTrain( data, varargin )
% Train binary decision tree classifier.
%
% Highly optimized code for training decision trees over binary variables.
% Training a decision stump (depth=1) over 5000 features and 10000 training
% examples takes 70ms on a single core machine and *7ms* with 12 cores and
% OpenMP enabled (OpenMP is enabled by default, see toolboxCompile). This
% code shares similarities with forestTrain.m but is optimized for binary
% labels. Moreover, while forestTrain is meant for training random decision
% forests, this code is tuned for use with boosting (see adaBoostTrain.m).
%
% For more information on how to quickly boost decision trees see:
%   [1] R. Appel, T. Fuchs, P. Doll醨, P. Perona; "Quickly Boosting
%   Decision Trees ?Pruning Underachieving Features Early," ICML 2013.
% The code here implements a simple brute-force strategy with the option to
% sample features used for training each node for additional speedups.
% Further gains using the ideas from the ICML paper are possible. If you
% use this code please consider citing our ICML paper.
%
% During training each feature is quantized to lie between [0,nBins-1],
% where nBins<=256. Quantization is expensive and should be performed just
% once if training multiple trees. Note that the second output of the
% algorithm is the quantized data, this can be reused in future training.
%
% USAGE
%  [tree,data,err] = binaryTreeTrain( data, [pTree] )
%
% INPUTS
%  data       - data for training tree
%   .X0         - [N0xF] negative feature vectors
%   .X1         - [N1xF] positive feature vectors
%   .wts0       - [N0x1] negative weights
%   .wts1       - [N1x1] positive weights
%   .xMin       - [1xF] optional vals defining feature quantization
%   .xStep      - [1xF] optional vals defining feature quantization
%   .xType      - [] optional original data type for features
%  pTree      - additional params (struct or name/value pairs)
%   .nBins      - [256] maximum number of quanizaton bins (<=256)
%   .maxDepth   - [1] maximum depth of tree
%   .minWeight  - [.01] minimum sample weigth to allow split
%   .fracFtrs   - [1] fraction of features to sample for each node split
%   .nThreads   - [inf] max number of computational threads to use
%
% OUTPUTS       ？？？？K是是什么值？？？是K=2*(N0+N1)吗？？？
%  tree       - learned decision tree model struct w the following fields
%   .fids       - [Kx1] feature ids for each node
%   .thrs       - [Kx1] threshold corresponding to each fid
%   .child      - [Kx1] index of child for each node (1-indexed)
%   .hs         - [Kx1] log ratio (.5*log(p/(1-p)) at each node
%   .weights    - [Kx1] total sample weight at each node
%   .depth      - [Kx1] depth of each node
%  data       - data used for training tree (quantized version of input)
%  err        - decision tree training error
%
% EXAMPLE
%
% See also binaryTreeApply, adaBoostTrain, forestTrain
%
% Piotr's Image&Video Toolbox      Version 3.21
% Copyright 2013 Piotr Dollar.  [pdollar-at-caltech.edu]
% Please email me if you find bugs, or have suggestions or questions!
% Licensed under the Simplified BSD License [see external/bsd.txt]

% get parameters
dfs={'nBins',256,'maxDepth',1,'minWeight',.01,'fracFtrs',1,'nThreads',1e5};%指明树的的最大深度！！
[nBins,maxDepth,minWeight,fracFtrs,nThreads]=getPrmDflt(varargin,dfs,1);
assert(nBins<=256);

% get data and normalize weights
dfs={ 'X0','REQ', 'X1','REQ', 'wts0',[], 'wts1',[], ...
  'xMin',[], 'xStep',[], 'xType',[] };
[X0,X1,wts0,wts1,xMin,xStep,xType]=getPrmDflt(data,dfs,1);
[N0,F]=size(X0); [N1,F1]=size(X1); assert(F==F1);
if(isempty(xType)), xMin=zeros(1,F); xStep=ones(1,F); xType=class(X0); end
assert(isfloat(wts0)); if(isempty(wts0)), wts0=ones(N0,1)/N0; end
assert(isfloat(wts1)); if(isempty(wts1)), wts1=ones(N1,1)/N1; end
w=sum(wts0)+sum(wts1); if(abs(w-1)>1e-3), wts0=wts0/w; wts1=wts1/w; end

% quantize data to be between [0,nBins-1] if not already quantized
if( ~isa(X0,'uint8') || ~isa(X1,'uint8') )
  xMin = min(min(X0),min(X1))-.01;
  xMax = max(max(X0),max(X1))+.01;
  xStep = (xMax-xMin) / (nBins-1);
  X0 = uint8(bsxfun(@times,bsxfun(@minus,X0,xMin),1./xStep));
  X1 = uint8(bsxfun(@times,bsxfun(@minus,X1,xMin),1./xStep));
end
data=struct( 'X0',X0, 'X1',X1, 'wts0',wts0, 'wts1',wts1, ...
  'xMin',xMin, 'xStep',xStep, 'xType',xType );

% train decision tree classifier，%该决策树为二叉树 %好像没有指定深度，只有前面说的深度为1.
K=2*(N0+N1); 
thrs=zeros(K,1,xType);
hs=zeros(K,1,'single'); weights=hs; errs=hs;
fids=zeros(K,1,'uint32'); child=fids; depth=fids;
wtsAll0=cell(K,1); wtsAll0{1}=wts0;
wtsAll1=cell(K,1); wtsAll1{1}=wts1;

k=1; 
K=2;
   while( k < K )% k（应该是所有左孩子的序号，按从上到下，从左到右顺序！child存储的是左孩子在树上的真实节点序号，如child（2）=4，表示第二个左孩子是数中的第四个节点）
                  %K应该是所有节点序号，按从上到下，从左到右顺序
      % get node weights and prior
      wts0=wtsAll0{k}; wtsAll0{k}=[]; w0=sum(wts0);
      wts1=wtsAll1{k}; wtsAll1{k}=[]; w1=sum(wts1);
      w=w0+w1;
      prior=w1/w; %归一化后的所有正样本权值之和
      weights(k)=w;
      errs(k)=min(prior,1-prior);
      hs(k)=max(-4,min(4,.5*log(prior/(1-prior)))); %如何理解这句话？？？直接赋值.5*log(prior/(1-prior))不行吗？要保证hs在区间[-4,4]中
      

      % if nearly pure node or insufficient data don't train split 
      %pure node如何理解？？是depth(k)>=maxDepth吗？？depth初值为0，其值改变是在最后的if语句中，那如果那个if语句进不去，该怎么办？？
      if( prior<1e-3||prior>1-1e-3||depth(k)>=maxDepth||w<minWeight ) %限制了树的深度
        k=k+1;
        continue; 
      end

    % train best stump
      fidsSt=1:F; 
      if(fracFtrs<1), 
          fidsSt=randperm(F,floor(F*fracFtrs));
      end
      [errsSt,thrsSt] = binaryTreeTrain1(X0,X1,single(wts0/w),...
        single(wts1/w),nBins,prior,uint32(fidsSt-1),nThreads);
        %该处errsSt,thrsSt均为1*F，errSt是lowest error for each feature；thrsSt是each feature对应的门限值
        %binaryTreeTrain1函数是关键！！如何求解出的thrsSt？？？
      [aa,fid]=min(errsSt);%取出分类错误率最小的弱分类器，fid为索引号
      thr=single(thrsSt(fid))+.5; %+0.5？？因为thrsSt中，每个元素都是整数！！（.cpp文件中如是说！！，因为所有的数据先进行了量化处理）
      fid=fidsSt(fid);%这句有意义吗？？两个fid值不是一样的吗
	  disp(['选出的特征编号为：',num2str(fid)]);
      

      % split data and continue
      left0=X0(:,fid)<thr; 
      left1=X1(:,fid)<thr;
      if( (any(left0)||any(left1)) && (any(~left0)||any(~left1)) )
          %（any()函数为检测矩阵中是否有非0元素，有就返回1，否则返回0.所以只要左右子树各自都包含正负样本，即还有裂开的可能，则继续构造树）
          %
            thr = xMin(fid)+xStep(fid)*thr;%为什么不直接使用thr？？（因为之前的thr是量化值，要返回原来的真实数据的门限）
            child(k)=K;
            fids(k)=fid-1;
            thrs(k)=thr;
            wtsAll0{K}=wts0.*left0;
            wtsAll0{K+1}=wts0.*~left0;
            wtsAll1{K}=wts1.*left1;
            wtsAll1{K+1}=wts1.*~left1;
            depth(K:K+1)=depth(k)+1;
            K=K+2;
      end;
      k=k+1;
   end;

    K=K-1;

% create output model struct

    tree=struct('fids',fids(1:K),'thrs',thrs(1:K),'child',child(1:K),...
      'hs',hs(1:K),'weights',weights(1:K),'depth',depth(1:K));
    if(nargout>=3),
        err=sum(errs(1:K).*tree.weights.*(tree.child==0)); 
    end

end
