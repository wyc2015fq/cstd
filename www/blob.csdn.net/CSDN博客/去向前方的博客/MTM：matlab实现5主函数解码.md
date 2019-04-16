# MTM：matlab实现5主函数解码 - 去向前方的博客 - CSDN博客





2018年07月05日 20:35:24[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：130








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的matlab实现。 

想要复习的可以参考一下之前的文件： 
[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)

MTM：matlab实现2参数解析[MTM参数解析](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现3谱功率计算[MTM谱功率计算](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现4主函数解析[MTM 主函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80932452)
# 目录



- [前言](#前言)
- [目录](#目录)
- [MTM实现](#mtm实现)
- [函数computepsd](#函数computepsd)




# MTM实现

```
检查输入
narginchk(1,13);
len = length(varargin);
将输入的x转换为列向量
% convert vectors to column vectors for backwards compatibility
if isvector(x)
  x = x(:);
end
如果x的维度大于2，只支持向量和矩阵，则报错
if numel(size(x))>2
  error(message('signal:pmtm:MustBeVectorOr2DMatrix'));
end
是否丢掉最后一个窗
tf = strcmpi('droplasttaper',varargin);
指明第几个输入参数是丢窗
indx = find(tf==1);
drop 的值只能是真或假
if (~isempty(indx) && ~islogical(varargin{indx+1}))
    error(message('signal:pmtm:MustBeLogical', 'Droplasttaper'));
end
将这两个参数，移到最后
% If the 'droplasttaper' pv-pair is used, move it to the end of varargin
if (~isempty(indx) && (indx+1 ~= len))
    dummy = varargin(1:indx-1);
    dummy(indx:len-2) = varargin(indx+2:len);
    dummy(len-1:len) = varargin(indx:indx+1);
    varargin = dummy;
end
解析输入，这个函数在2种分析了
% Parse the inputs, set up default values, and return any error messages.
params = parseinputs(x,varargin{:});
强制使用精确规则
% Cast to enforce Precision Rules
% Already checked for invalid character inputs (NW, NFFT,Fs) in 'parseinputs->psdoptions'

params.nfft = double(params.nfft);
params.Fs = double(params.Fs);
使用mtm计算功率谱
% Compute the two-sided power spectrum via MTM.
[S,k,w] = mtm_spectrum(x,params);
生成频率点
% Generate the freq vector in correct units to avoid roundoff errors due to
% converting frequency units later.
nfft = params.nfft;
[~,ncol] = size(nfft);
计算单边或者双边功率谱或者均方谱
% Compute the 1-sided or 2-sided PSD [Power/freq] or mean-square [Power].
% Also, compute the corresponding freq vector & freq units.
第二个行之后的能量都*2/总频率 赫兹数
[Pxx,f,units] = computepsd(S,w,params.range,params.nfft,params.Fs,'psd');  
计算置信域，如果有需求的话。
% Calculate confidence limits ONLY when needed, since it can take a while.
强制双进度，在计算。
% Enforce double precision arithmetic on the calculations. chi2conf already
强制双进度计算
% enforces double precision arithmetic.
if ~strcmp(params.conflevel,'omitted') && (nargout==0 || nargout>2)
  % 'ConfidenceLevel' pv-pair specified
  Nchan = size(x,2);
  c = chi2conf(params.conflevel,k);
  Pxxc(:,2:2:2*Nchan) = double(Pxx)*c(2);
  Pxxc(:,1:2:2*Nchan) = double(Pxx)*c(1);
elseif (~strcmp(params.ConfInt,'omitted') && nargout==0) || nargout>2
  % using legacy syntax
  if ~strcmp(params.ConfInt,'omitted')
    c = chi2conf(params.ConfInt,k);
  else % use default value of .95
  k自由度卡方分布
    c = chi2conf(.95,k);
  end 
  多少个信号源     
  Nchan = size(x,2);
  置信度上下界
  Pxxc(:,2:2:2*Nchan) = double(Pxx)*c(2);
  Pxxc(:,1:2:2*Nchan) = double(Pxx)*c(1);
else
  Pxxc = [];
end

% Output
如果没有输出参数
if nargout==0 画出功率谱图
   % If no output arguments are specified plot the PSD w/ conf intervals.
   f = {f};单元化
   if strcmpi(units,'Hz'), f = [f {'Fs',params.Fs}]; end 单位是hz
   hpsd = dspdata.psd([Pxx Pxxc],f{:},'SpectrumType',params.range); hpsd结构化的数据

   % Create a spectrum object to store in the PSD object's metadata. 存储dsp数据
   hspec = spectrum.mtm(params.E,params.V,params.MTMethod);
   hpsd.Metadata.setsourcespectrum(hspec);

   if params.centerdc
     centerdc(hpsd);
   end

   hp = plot(hpsd);画图谱图
   if 3*size(x,2)==numel(hp)
     nChan = size(x,2);
     color = get(hp,'Color');
     for i=1:nChan
       set(hp(nChan+2*i-1), 'Color',color{i}, 'LineStyle','-.');
       set(hp(nChan+2*i), 'Color',color{i}, 'LineStyle','-.');
     end
   end     
   return
end

% center dc if needed
if params.centerdc
   [Pxx, f, Pxxc] = psdcenterdc(Pxx, f, Pxxc, params);
end中心化频率
返回行向量作为输出
% If the input is a vector and a row frequency vector was specified,
% return output as a row vector for backwards compatibility.
if ncol > 1 && nargout > 0 && isvector(x)
    Pxx = Pxx.';
    f = f.';
    % preserve (undocumented) behavior with legacy syntax.
    if strcmp(params.conflevel,'omitted') && nargout >= 3
        Pxxc = Pxxc.';
    end
end

if isa(Pxx,'single')
  % Cast to enforce precision rules.
  f = single(f);频率点
end  

if nargout==1
   varargout = {Pxx};输出pxx
elseif nargout==2
   varargout = {Pxx,f};输出pxx和f
elseif nargout==3
   if ~strcmp(params.conflevel,'omitted')
      % use preferred output order
      varargout = {Pxx,f,Pxxc};输出pxx和频率点和置信度
   else
      % use legacy output order
      varargout = {Pxx,Pxxc,f};
   end
end
```

# 函数computepsd

“` 

Compute the one-sided or two-sided PSD or Mean-Square. 

    [Pxx,W,UNITS] = computepsd(Sxx,W,RANGE,NFFT,Fs,ESTTYPE) where the 

    inputs and outputs are:
```
Inputs:
 Sxx   - Whole power spectrum [Power]; it can be a vector or a matrix.
         For matrices the operation is applied to each column.
 W     - Frequency vector in rad/sample or in Hz.
 RANGE - Determines if a 'onesided' or a 'twosided' Pxx and Sxx are
         returned.
 NFFT  - Number of frequency points.
 Fs    - Sampling Frequency.
 ESTTYPE - A string indicating the estimate type: 'psd', or 'ms' value.

Outputs:
 Pxx   - One-sided or two-sided PSD or MEAN-SQUARE (not scaled by Fs)
         depending on the input arguments RANGE and TYPE.
 W     - Frequency vector 0 to 2*Nyquist or 0 to Nyquist depending on
         range, units will be either rad/sample (if Fs is empty) or Hz
         (otherwise).
 UNITS -sample' or 'Hz'          `
         ```
```





