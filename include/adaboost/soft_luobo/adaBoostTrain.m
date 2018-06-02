function model = adaBoostTrain( X0, X1, varargin )
% Train boosted decision tree classifier.
%
% Heavily optimized code for training Discrete or Real AdaBoost where the weak classifiers are decision trees. 
%With multi-core support enabled (see binaryTreeTrain.m), boosting 256 depth-2 trees over 5,000 features and 5,000 data points takes under 5 seconds, see example below. 
%Most of the training time is spent in binaryTreeTrain.m.
%
% For more information on how to quickly boost decision trees see:
%   [1] R. Appel, T. Fuchs, P. Doll醨, P. Perona; "Quickly Boosting  Decision Trees ?Pruning Underachieving Features Early," ICML 2013.
% The code here implements a simple brute-force strategy with the option to sample features used for training each node for additional speedups. Further gains using the ideas from the ICML paper are possible. 
%If you use this code please consider citing our ICML paper.
%
% USAGE
%  model = adaBoostTrain( X0, X1, [pBoost] )
%
% INPUTS
%  X0         - [N0xF] negative feature vectors
%  X1         - [N1xF] positive feature vectors
%  pBoost     - additional params (struct or name/value pairs)
%   .pTree      - ['REQ'] parameters for binaryTreeTrain
%   .nWeak      - [128] number of trees to learn
%   .discrete   - [1] train Discrete-AdaBoost or Real-AdaBoost
%   .verbose    - [0] if true print status information
%
% OUTPUTS
%  model      - learned boosted tree classifier w the following fields
%   .fids       - [K x nWeak] feature ids for each node
%   .thrs       - [K x nWeak] threshold corresponding to each fid
%   .child      - [K x nWeak] index of child for each node (1-indexed)
%   .hs         - [K x nWeak] log ratio (.5*log(p/(1-p)) at each node
%   .weights    - [K x nWeak] total sample weight at each node
%   .depth      - [K x nWeak] depth of each node
%   .errs       - [1 x nWeak] error for each tree (for debugging)
%   .losses     - [1 x nWeak] loss after every iteration (for debugging)
%   .treeDepth  - depth of all leaf nodes (or 0 if leaf depth varies)
%
% EXAMPLE
%  % output should be: 'Testing err=0.0145 fp=0.0165 fn=0.0125'
%  N=5000; F=5000; sep=.01; RandStream.getGlobalStream.reset();
%  [xTrn,hTrn,xTst,hTst]=demoGenData(N,N,2,F/10,sep,.5,0);
%  xTrn=repmat(single(xTrn),[1 10]); xTst=repmat(single(xTst),[1 10]);
%  pBoost=struct('nWeak',256,'verbose',16,'pTree',struct('maxDepth',2)); %应该指明自己指明pTree的类型
%  model = adaBoostTrain( xTrn(hTrn==1,:), xTrn(hTrn==2,:), pBoost );
%  fp = mean(adaBoostApply( xTst(hTst==1,:), model )>0);
%  fn = mean(adaBoostApply( xTst(hTst==2,:), model )<0);
%  fprintf('Testing err=%.4f fp=%.4f fn=%.4f\n',(fp+fn)/2,fp,fn);
%
% See also adaBoostApply, binaryTreeTrain, demoGenData
%
% Piotr's Image&Video Toolbox      Version 3.21
% Copyright 2013 Piotr Dollar.  [pdollar-at-caltech.edu]
% Please email me if you find bugs, or have suggestions or questions!
% Licensed under the Simplified BSD License [see external/bsd.txt]

% get additional parameters
%dfs={ 'pTree','REQ', 'nWeak',128, 'discrete',1, 'verbose',0 };
dfs={ 'pTree','REQ', 'nWeak','REQ', 'discrete','REQ', 'verbose','REQ'};%'REQ'表示某值是用户自定义，不是默认值
[pTree,nWeak,discrete,verbose]=getPrmDflt(varargin,dfs,1);
nThreads=[]; if(isfield(pTree,'nThreads')), nThreads=pTree.nThreads; end

% main loop
[N0,F]=size(X0); 
[N1,F1]=size(X1); 
assert(F==F1);
msg='Training AdaBoost: nWeak=%3i nFtrs=%i pos=%i neg=%i\n';
if(verbose), fprintf(msg,nWeak,F,N1,N0); start=clock; end
data=struct('X0',X0,'X1',X1);
H0=zeros(N0,1); 
H1=zeros(N1,1);
losses=zeros(1,nWeak); 
errs=losses;

for i=1:nWeak
  % train tree and classify each example
  disp(['第',num2str(i),'个弱分类器选取特征：']);
  [tree,data,err]=binaryTreeTrain(data,pTree);
  if(discrete),
      tree.hs=(tree.hs>0)*2-1;%保证取值为｛1，-1｝
  end
  h0 = binaryTreeApply(X0,tree,[],[],nThreads);
  h1 = binaryTreeApply(X1,tree,[],[],nThreads);
  
  % compute alpha and incorporate directly into tree model
  % 计算alpha，然后直接融合到tree模型中
  alpha=1; 
  if(discrete), 
     alpha=max(-5,min(5,.5*log((1-err)/err)));
  end
  if(verbose && alpha<=0), 
      nWeak=i-1; 
      disp(' stopping early');
      break;
  end
  
  tree.hs=tree.hs*alpha;
  
  % update cumulative scores H and weights
  H0=H0+h0*alpha; 
  data.wts0=exp( H0)/N0/2;%更新负样本权值
  H1=H1+h1*alpha; 
  data.wts1=exp(-H1)/N1/2;%更新正样本权值
  loss=sum(data.wts0)+sum(data.wts1);
  if(i==1), 
      trees=repmat(tree,nWeak,1);
      %（理解错了repmat函数，该函数是将tree复制nWeak*1块，赋值给trees！！不是将tree重排成nWeak*1的矩阵）
  end
  trees(i)=tree;
  errs(i)=err;
  losses(i)=loss;
  msg=' i=%4i alpha=%.3f err=%.3f loss=%.2e\n';
  if(mod(i,verbose)==0),
      fprintf(msg,i,alpha,err,loss); 
  end
  if(verbose && loss<1e-40),
      nWeak=i;
      disp(' stopping early');
      break;
  end
end

% create output model struct
k=0; 
for i=1:nWeak, 
    k=max(k,size(trees(i).fids,1));%每一棵树的fids的行数不是相同的吗？？怎么会有最大值之说！！
end
Z = @(type) zeros(k,nWeak,type);%@的用法：本处相当于定义了一个函数，function y = Z（type），自变量为type，Z为映射方式，y=zeros(k,nWeak,type)；

model=struct( 'fids',Z('uint32'), 'thrs',Z(data.xType), ...
  'child',Z('uint32'), 'hs',Z('single'), 'weights',Z('single'), ...
  'depth',Z('uint32'), 'errs',errs, 'losses',losses );

for i=1:nWeak, 
    T=trees(i); 
    k=size(T.fids,1);%行数
    model.fids(1:k,i)=T.fids;
    model.thrs(1:k,i)=T.thrs;
    model.child(1:k,i)=T.child;
    model.hs(1:k,i)=T.hs;
    model.weights(1:k,i)=T.weights; 
    model.depth(1:k,i)=T.depth;
end
disp(['训练得到',num2str(nWeak),'个弱分类器！！']);
depth = max(model.depth(:));
model.treeDepth = depth * uint32(all(model.depth(~model.child)==depth));

% output info to log
%msg='Done training err=%.4f fp=%.4f fn=%.4f (t=%.1fs).\n';
    %if(verbose), 
        %fp=mean(H0>0); %这是何意？？(false positive)
        %fn=mean(H1<0); %这应该是false negative！！
     % fprintf(msg,(fp+fn)/2,fp,fn,etime(clock,start));
    %end

end
