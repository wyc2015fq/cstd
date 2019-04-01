# 连接词识别的Level Building 算法

2011年12月04日 12:09:36 [xiaoding133](https://me.csdn.net/xiaoding133) 阅读数：1899



看了一篇论文，A Level Building Dynamic Time Warping Algorithm for Connected Word Recognition，IEEE TRANSACTIONS ON ACOUSTICS, SPEECH, AND SIGNAL PROCESSING, VOL. ASSP-29, NO. 2, APRIL 1981，上面描述了Level Building 算法用于连接词识别，还讲了该算法与 Two level DP 算法的联系。。。。而Two level Dp算法已经可以很好的实现出来，但Level Building  算法在操作的过程中遇到了一点点小问题。。。。。代码如下，希望大家能指出来。。。。。

```matlab
N=size(T,2);%测试序列的帧数
K=length(DataSet);%参考模板个数
L=7;%连接词最大数目
%Algorithm S2
%Word level Matching 
%Initialize
D=inf(N,N,K);
for e=1:N %起点标号
    for b=e-40+1:e-8+1 %终点
        if b>0
            for v=1:K
                dist=dtw(T(:,b:e), DataSet{v}.mfcc);
                 D(b,e,v)=dist;
            end
        end
    end
end

%Initialize初始化 第一级
F(1,1:N,1:K)=-1;

for e=2:N
      mindstk=inf;
      v1=-1; %最优词索引
     for v=1:K
         mindist=inf;
         k=-1;
         for b=1:e-1
             if e-b+1>=8&e-b+1<=40
             if D(b,e,v)<mindist
                 mindist=D(b,e,v);
                 k=e; %保存上一个结束点
             end
             end
         end
         Dst(1,1,v)=inf;
         Dst(1,e,v)=mindist;
         F(1,e,v)=k;
         
         if Dst(1,e,v)<mindstk
             mindstk=Dst(1,e,v);
             v1=v;
         end
     end
    
      B(1,e)=mindstk; %第一级的最小距离
      W(1,e)=v1; %保存第一级最优词序号
     if v1>0
     DF(1,e)=F(1,e,v1); % DF保存第一级最小路径测试序列对应的帧号
     else
     DF(1,e)=0; 
     end
   
end


% Start Phrase level Matching 
for l=2:L             %层循环
    
    for e=l+1:N   %测试模板帧
      mindstk=inf;
      v1=-1; %最优词索引
         for v=1:K       %所有的参考模板
            mindist=inf;
             k=-1;
          for b=2:e-1
             if e-b+1>=8&e-b+1<=40
             if D(b,e,v)+B(l-1,b-1)<mindist
                 mindist=D(b,e,v)+B(l-1,b-1);
                 k=b-1; %保存上一个结束点
             end
             end
          end
         Dst(l,e,v)=mindist;
         F(l,e,v)=k;
             
         if Dst(l,e,v)<mindstk
             mindstk=Dst(l,e,v);
             v1=v;
          end
         
         end
         
         
      B(l,e)=mindstk; %第l级的最小距离
      W(l,e)=v1; %保存第l级最优词序号
      if v1>0
      DF(l,e)=F(l,e,v1); % DF保存第l级最小路径测试序列对应的帧号
     else
     DF(l,e)=0; 
     end
         
    end
end

%Find Best Length 省略。。
Lr=7; %假设bset Length为7
eL=N;
words=[];
while Lr>0
     words=[W(Lr,eL) words];
     eL=DF(Lr,eL);
     Lr=Lr-1;
end
disp(words-1); %输出结果
```


用连续数字串：1344507.wav 做实验，识别输出的结果为：1   1     3     4     5     0     7

 

2012.12.18 修正的LevelBuilding 算法：

```matlab
function [Pr,Br,Wr] = levelBuilding(hmms,O)
%   hmms:HMM模型集合
%   O:观测序列
% 返回值： Pr：最佳累积距离；Br:最佳累积距离对应的帧；Wr:最佳词序列

    T= size(O,1);	%语音帧数,观测序列长度
    K=length(hmms);%HMM模型个数
    L=7;  %最大层数，即词数
%At level l=1
%Initialization
      P=-inf(L,T,K);
      B=zeros(L,T,K);
      Pr=-inf(L,T);  %保存最优概率
      Br=zeros(L,T);
      Wr=zeros(L,T);
     delta=cell(1,K);
      fai=cell(1,K); %记录状态
      init=cell(1,K);
     trans=cell(1,K);
      %初始化
      for i=1:K
        hmm=hmms{i}.hmm; %第i个隐马尔可夫模型的状态数
       delta{i}=zeros(T,hmm.N); %第i个模型的token
        %初始化第i个模型的第一个状态的概率，转换为对数形式
       fai{i}= zeros(T,hmm.N);
       init{i}=hmm.init;    
       trans{i}=hmm.trans;
       ind1  = find(init{i}>0);
       ind0  = find(init{i}<=0);
       init{i}(ind0) = -inf;
       init{i}(ind1) = log(init{i}(ind1));
       %初始化第i个模型的的转移概率
      ind1 = find(trans{i}>0);
      ind0 = find(trans{i}<=0);
      trans{i}(ind0) = -inf;
      trans{i}(ind1) = log(trans{i}(ind1));
      end
     x = O(1,:);
     
   for k=1:K
      for i=1:hmms{k}.hmm.N %每个HMM的状态数
	  delta{k}(1,i) = init{k}(i) + log(mixture(hmms{k}.hmm.mix(i),x)); %初始化前向概率矩阵,初始状态概率乘以t=1时观测序列的输出概率
      end
   end
   
  delta1=delta; %保存初始值，以免迭代覆盖
  
for t=2:T %从第二个观测序列开始 
        
     for  k=1:K 
         for j = 1:hmms{k}.hmm.N  %对该模型的所有状态
	      [delta{k}(t,j), fai{k}(t,j)]= max(delta{k}(t-1,:) + trans{k}(:,j)');%找出t-1时刻所有状态最大的前向概率与转移的矩阵kk
	      x = O(t,:);
	      delta{k}(t,j) = delta{k}(t,j) + log(mixture(hmms{k}.hmm.mix(j),x)); %记录第k个词的最大距离
         end    
         P(1,t,k)=delta{k}(t,hmms{k}.hmm.N);
         B(1,t,k)=0; %保存帧号还是0？？
     end
     [pm,q]=max(P(1,t,1:K));
      Pr(1,t)=pm;
      Br(1,t)=B(1,t,q);
      Wr(1,t)=q;
end
%%%%%%%%%%%%%%%%%%%%%%level=1 End%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%level=2 开始。。。。。
%Initialization
 delta2(1,1:4)=0;  belta(1,1:4)=0;
for l=2:L
   
    for t=2:T %从第二个观测序列开始 
     
        temp=-inf; wi=0;
        for k=1:K
            
            if trans{k}(1,1)+delta2(t-1,1)>temp
                temp=trans{k}(1,1)+delta2(t-1,1);
                wi=k;
            end
        end
             %[pb,ind]=max(trans{1:K}(1,1)+delta2(t-1,1));
             x=O(t,:);
             [va,ind]=max([Pr(l-1,t-1) temp]);
             delta2(t,1)=va+log(mixture(hmms{wi}.hmm.mix(1),x));
             if ind==1 
                 belta(t,1)=t-1;
             else
                 belta(t,1)=belta(t-1,1);
             end
             %%递归
             for k=1:K
                  for j = 1:hmms{k}.hmm.N  %对该模型的所有状态
                     
                  
                      [vm,vd]=max(delta2(t-1,:)+ trans{k}(:,j)');
                      x=O(t,:);
                     delta2(t,j)=vm+log(mixture(hmms{k}.hmm.mix(j),x));
                     belta(t,j)=belta(t-1,vd);
                  end  
                  
              P(l,t,k)=delta2(t,hmms{k}.hmm.N);
              B(l,t,k)=belta(t,hmms{k}.hmm.N); %保存帧号还是0？？
             end   
             %%结束递归
    end
     [pm,q]=max(P(l,t,1:K));
      Pr(l,t)=pm;
      Br(l,t)=B(l,t,q);
      Wr(l,t)=q;
    
end

%  disp(delta2);

end
```


 