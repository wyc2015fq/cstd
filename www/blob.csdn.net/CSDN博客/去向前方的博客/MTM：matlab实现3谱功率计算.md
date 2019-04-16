# MTM：matlab实现3谱功率计算 - 去向前方的博客 - CSDN博客





2018年07月05日 19:17:05[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：192








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的matlab实现。 

想要复习的可以参考一下之前的文件： 
[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)

MTM：matlab实现2参数解析[MTM参数解析](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)
# 目录


- [前言](#前言)
- [目录](#目录)
- [正文](#正文)



# 正文

子函数的讲解即将收尾，现在是最后一个，多窗口谱计算函数。 

输入 

x 数据向量 

params，pmtm传递的参数，除了输入x 

它包含以下值域 

nfft，评估功率谱的具体频率点 

Fs 采样频率 

range 单边还是双边 

ConfInt 置信间隔 默认0.95 

mtmmethod多窗口谱方法，默认自适应 

E，dpss矩阵 

V 包含dpss序列中心的向量 

NW 时间带宽乘积 

输出 

s MTM计算出的功率谱 

k 构建pxx的加窗数 

w dft计算的频率点
```
%----------------------------------------------------------------------
function [S,k,w] = mtm_spectrum(x,params)
%MTM_SPECTRUM Compute the power spectrum via MTM.
%
% Inputs:
%   x      - Input data vector.
%   params - Structure containing pmtm's input parameter list, except for
%            the input data sequence, x; it contains the following fields:
%      nfft     - Number of frequency points to evaluate the PSD at; 
%                 the default is max(256,2^nextpow2(N)).
%      Fs       - The sampling frequency; default is 1.
%      range    - default is 'onesided' or real signals and 'twosided' for 
%               - complex signals.
%      ConfInt  - Confidence interval; default is .95.
%      MTMethod - Algorithm used in MTM; default is 'adapt'.
%      E        - Matrix containing the discrete prolate spheroidal 
%                 sequences (dpss).
%      V        - Vector containing the concentration of the dpss.
%      NW       - Time-bandwidth product; default is 4.
%
% Outputs:
%   S      - Power spectrum computed via MTM.
%   k      - Number of sequences used to form Pxx.
%   w      - Frequency vector for which DFT is calculated


从输入中提取相关的参数
% Extract some parameters from the input structure for convenience.
nfft = params.nfft;
E  = params.E;
V  = params.V;
Fs = params.Fs;
如果没有采样频率，则默认为归一化频率2pi
if isempty(Fs)
  Fs = 2*pi;
end
列向量计算MTM，每一列是一个单独的采样。
行数即采样点数，列数即独立信号个数
N = size(x,1);
Nchan = size(x,2);
采用的多窗口的窗口个数
k = length(V);
如果nfft不是一个整数而是一个列向量，则代表它是想要计算的点。
if length(nfft) > 1, 
    isfreqVector = true;     
    nfft_mod = length(nfft);
else 
    isfreqVector = false;
    nfft_mod = nfft;
end
如果x是single，则信号功率谱也是single
if isa(x,'single') || isa(E,'single')
  S = zeros(nfft_mod, Nchan, 'single');
else
  S = zeros(nfft_mod, Nchan);
end
对于每一个信号源
for chan=1:Nchan
计算加窗 离散傅里叶
    % Compute the windowed DFTs.
    如果指定输入频率向量或者不指定且N采样个数小于设定的NFFT点数
    if (~isfreqVector && N<=nfft) || isfreqVector 
       计算每一列的加窗fft变换
        % Compute DFT using FFT or Goertzel
        使用基本函数，快速计算加窗后的离散傅里叶变换
        [Xx,w] = computeDFT(bsxfun(@times,E(:,1:k),x(:,chan)),nfft(:),Fs); 
         sk是xx的能量值   
        Sk = abs(Xx).^2;

    else % Wrap the data modulo nfft if N > nfft. Note we cannot use datawrap 
        % and FFT because datawrap does not support matrices
        % use CZT to compute DFT on nfft evenly spaced samples around the
        % unit circle:
        Sk = abs(czt(bsxfun(@times,E(:,1:k),x(:,chan)),nfft(:))).^2;
获得对应的频率点
        w = psdfreqvec('npts',nfft,'Fs',Fs);
    end
计算多窗口谱估计，在0：NFFt上计算整个特征谱。
    % Compute the MTM spectral estimates, compute the whole spectrum 0:nfft.
    switch params.MTMethod,
自适应的情况：
    case 'adapt'
设置决定自适应的权重的代数
       % Set up the iteration to determine the adaptive weights: 

        初始功率谱，第一列的平方和/数量
       sig2=x(:,chan)'*x(:,chan)/N;      
        % Power
        加窗1和加窗2的平均值作为初始值
       Schan=(Sk(:,1)+Sk(:,2))/2;    % Initial spectrum estimate   
       S1=zeros(nfft_mod,1);  

       % The algorithm converges so fast that results are
       % usually 'indistinguishable' after about three iterations.

       % This version uses the equations from [2] (P&W pp 368-370).

       % Set tolerance for acceptance of spectral estimate:
       tol=.0005*sig2/nfft_mod;
       a=bsxfun(@times,sig2,(1-V));

       % Do the iteration:
       while sum(abs(Schan-S1)/nfft_mod)>tol
          % calculate weights
          b=(Schan*ones(1,k))./(Schan*V'+ones(nfft_mod,1)*a'); 
          % calculate new spectral estimate
          wk=(b.^2).*(ones(nfft_mod,1)*V');
          S1=sum(wk'.*Sk')./ sum(wk,2)';
          S1=S1';
          Stemp=S1; S1=Schan; Schan=Stemp;  % swap S and S1
       end
    case {'unity','eigen'}
       % Compute the averaged estimate: simple arithmetic averaging is used. 
       % The Sk can also be weighted by the eigenvalues, as in Park et al. 
       % Eqn. 9.; note that the eqn. apparently has a typo; as the weights
       % should be V and not 1/V.
       if strcmp(params.MTMethod,'eigen')
          wt = V(:);    % Park estimate
       else
          wt = ones(k,1);
       end
       Schan = Sk*wt/k;
    end
    S(:,chan) = Schan;
end
```





