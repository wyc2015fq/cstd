# MTM：matlab实现2参数解析 - 去向前方的博客 - CSDN博客





2018年07月05日 16:20:51[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：218








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的matlab实现。 

想要复习的可以参考一下之前的文件： 
[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)
# 目录



- [前言](#前言)
- [目录](#目录)
- [正文](#正文)- [parseinputs](#parseinputs)

- [computeDFT](#computedft)




# 正文

接着上回的讲剩下的那两个子函数：

## parseinputs

```
解析输入的列表参数
解析输入传递到pmtm.m的参数并返回一个结构包括所有的参数传递给pmtm集合，可能是默认值也可能是用户定义的值。
%----------------------------------------------------------------------
function params = parseinputs(x,varargin)
%PARSEINPUTS Parse the inputs passed to pmtm.m and return a structure
%            containing all the parameters passed to PMTM set to either
%            default values or user defined values.
%
输入
x 输入的数据向量
vararginpmtm传给的输入参数列表，除了数据x
% Inputs:
%   x        - Input data vector.
%   varargin - Input parameter list passed to pmtm, except for x.
%

输出
params 是个结构体，包含了pmtm的输入参数列表，除了输入数据x，它
包含以下值域。
% Outputs:
%   params   - Structure containing pmtm's input parameter list, except for
%              the input data sequence, x; it contains the following fields:
nfft 评估psd的频率点数，默认是接近输入的2的n次方

%      nfft     - Number of frequency points to evaluate the PSD at; 

%                 the default is max(256,2^nextpow2(N)).

Fs 采样频率
%      Fs       - The sampling frequency; default is .
range 默认是单边，复信号是双边。

%      range    - default is 'onesided' or real signals and 'twosided' for 
%               - complex signals.
conflevel 置信度水平

%      conflevel- Confidence level (preferred syntax)

置信度水平，默认是0，95
%      ConfInt  - Confidence interval; default is .95. (legacy syntax)

谱估计时使用的方法，默认的是adaptive。
%      MTMethod - Algorithm used in MTM; default is 'adapt'.
E 包含有离散扁球序列的矩阵
%      E        - Matrix containing the discrete prolate spheroidal 
%                 sequences.
v 向量包含 dpss的中心

%      V        - Vector containing the concentration of the dpss.

NW 时间带宽积 
%      NW       - Time-bandwidth product; default is 4.
%
err——msd 反馈错误信息。
%   err_msg  - String containing an error message if an error occurred.
错误信息提示
if any(strcmp(varargin, 'whole'))
    warning(message('signal:pmtm:invalidRange', 'whole', 'twosided'));
elseif any(strcmp(varargin, 'half'))
    warning(message('signal:pmtm:invalidRange', 'half', 'onesided'));
end
初始化默认的参数值
% Set default parameter values.
N = size(x,1);
params  = [];

解析输入参数到NFFT，如果包含
如果没有指定e和v，则计算它们
% Parse the input arguments up to NFFT (exclusive). 
% If E and V are not specified, calculate them.
[E,V,NW,indx,nfft_temp,varargin] = getEV(N,varargin{:});

强制执行精确规则
% Cast to enforce Precision Rules
if (any([signal.internal.sigcheckfloattype(x,'single','pmtm','X') ...
    signal.internal.sigcheckfloattype(E,'single','pmtm','E') ...
    signal.internal.sigcheckfloattype(V,'single','pmtm','V')]))
  x = single(x);
  E = single(E);
  V = single(V);
end
NW 数值转化
NW = double(NW);
如果x是实数，且nfft的长度《=1
if isreal(x) && (length(nfft_temp) <= 1), 
   range = 'onesided';
else
   range = 'twosided'; 
end

注意：功率谱估计函数要求一个包含以下值域的结构体，任何对这个结构的改变，都必须在调用后做完。
% NOTE: The psdoptions function REQUIRES a structure with the following 
%       fields.  Any changes to the structure, such as adding/removing 
%       fields, should be done after the call to psdoptions.

params.nfft    = max(256,2^nextpow2(N));
params.Fs      = [];
params.range   = range;
params.centerdc = false;
params.conflevel = 'omitted';
遗漏设置
params.ConfInt = 'omitted';
params.MTMethod= 'adapt';
调用功率谱选项来解决遗留的以NFFT开始的输入参数列表
% Call psdoptions to handle the remaining input arg list starting with NFFT.
重写默认选项，使用用户特定的值。
% Overwrite default options with user specified options (if specified).
如果indx小于 参数的数量
if indx <= numel(varargin)
  检测相关输入参数是否合法。
  % Invalid character inputs for NW, NFFT, W, E,V and Fs is checked here
   [params,err_msg,err_msgobj] = psdoptions(isreal(x),params,varargin{indx:end});
   if err_msg, error(err_msgobj), end;     

   if ~strcmp(params.conflevel,'omitted') && ~strcmp(params.ConfInt, 'omitted')
       % user specified too many scalar inputs in conjunction with 'ConfidenceLevel'
       error(message('signal:pmtm:TooManyScalarNumericInputs'));
   end

   if length(params.nfft) > 1,
       if strcmpi(params.range,'onesided')
           warning(message('signal:pmtm:InconsistentRangeOption'));
       end
       params.range = 'twosided'`

#
##
 end


得到子函数的生成的dpss
% Add remaining fields to the return structure.
params.E  = E;
params.V  = V;
params.NW = NW;
```

# computeDFT

```
computeDFT Computes DFT using FFT or Goertzel
    This function is used to calculate the DFT of a signal using the FFT 
    or the Goertzel algorithm. 

    [XX,F] = computeDFT(XIN,NFFT) where NFFT is a scalar and computes the 
    DFT XX using FFT. F is the frequency points at which the XX is 
    computed and is of length NFFT.

    [XX,F] = computeDFT(XIN,F) where F is a vector with at least two 
    elements computes the DFT XX using the Goertzel algorithm. 

    [XX,F] = computeDFT(...,Fs) returns the frequency vector F (in hz)
    where Fs is the sampling frequency

    Inputs:
    XIN is the input signal
    NFFT if a scalar corresponds to the number of FFT points used to 
    calculate the DFT using FFT.
    NFFT if a vector corresponds to the frequency points at which the DFT
    is calculated using goertzel.
    FS is the s mp ing frequency
```




