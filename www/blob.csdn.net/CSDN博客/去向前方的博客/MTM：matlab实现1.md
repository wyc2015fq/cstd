# MTM：matlab实现1 - 去向前方的博客 - CSDN博客





2018年07月05日 15:48:08[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：137








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的matlab实现。 

想要复习的可以参考一下之前的文件： 
[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)
# 正文

首先是基本的子函数

## findNFFT

从传递过来的参数里面，找到特定的NFFT或者频率向量，返回给调用函数

```matlab
------------------------------------------------------------------
function nfftTemp = findNFFT(varargin)
% FINDNFFT Finds the specified NFFT or frequency vector from the optional
% arguments passed

nfftTemp = [];
for cnt = 1:length(varargin)
    if isnumeric(varargin{cnt}), 
        nfftTemp = varargin{cnt};
        找到对应的参数后，立马退出。
        break;
    end
end
    end
end
```

## getEV

函数[E,V,NW,indx,nfft_temp,varargin]=getEV(N,varargin) 

将输入参数解析为 (但不包括) Nfft. 如果e和v未指定, 则计算 E 和 V

```
%----------------------------------------------------------------------
function [E,V,NW,indx,nfft_temp,varargin] = getEV(N,varargin)
% GETEV  Parse the input arguments up to, but not including, Nfft and 
%        calculate E and V if not specified.
%
输入：
N 输入数据x的长度
% Inputs:
%   N        - Length of the input data sequence, x.

varargin -传递给pmtm的输入参数列表，除了x

%   varargin - Input parameter list passed to pmtm, except for x.
%

输出
% Outputs:
e 离散扁球序列矩阵
%   E        - Matrix containing the discrete prolate spheroidal 
%              sequences (dpss).
v  包含dpss中心的向量



%   V        - Vector containing the concentration of the dpss.

NW 时间窗口是4，默认是4
%   NW       - Time-bandwidth product; default is 4.

指示输入参数列表开始选项的索引
%   indx     - Index indicating starting location of options in pmtm's 
%              input argument list.

特定的NFFT点数
%   nfft_temp - NFFT or Frequency vector specified. Empty if not specified 
设置默认的或者初始化输出变量，避免早返回

% Define defaults & initialize output variables (in case of early return).
NW      = 4;
indx    = 1;  % Index where the options begin in the input arg list
nfft_temp = [];
判断是否输入droplasttaper参数
tf = strcmpi('droplasttaper',varargin);
查找tf==1的位置。
loc = find(tf==1);
如果找到了droplasttaper，
默认是dlt=1，丢掉最后一帧。
if ~isempty(loc)
    dlt = varargin{loc+1};     % droplasttaper
    varargin = varargin(1:loc-1);
else
    dlt = true;              % default value
end

第二种输入参数给pmtm可以是以下的类型：
1（x,Nw,...）标量
x，矩阵e，向量v
x，元胞数组包含dpss的所有参数
一个特定的功率谱选项

% The 2nd input arg to pmtm can be a
%    1. (X,NW,...)            scalar
%    2. (X,E,V,...)           matrix E, hence, 3rd input must be a vector (V) 
%    3. (X,{dpss_params},...) cell containing the input argument list to dpss 
%    4. a string specifying a psdoption

满足下列条件，则NW=varargin『1』.
if ~isempty(varargin) && ~ischar(varargin{1})
   if ~isempty(varargin{1})
       NW = varargin{1};
   end

   indx = 2;
   if iscell(NW),           % NW is a cell array => dpss_params
      if (NW{1}<1.25 && dlt)
          error(message('signal:pmtm:insufficientTimebandwidthproduct', 'NW', '1.25', 'Droplasttaper', 'true'));
      end 
      if (NW{1}<0.75 && ~dlt)
          error(message('signal:pmtm:insufficientTimebandwidthproduct', 'NW', '0.75', 'Droplasttaper', 'false'));          
      end    
   生成对应的E,V值
      [E,V] = dpss(N,NW{:}); 
      numvec = length(V);
      if dlt
           if numvec > 2
               E = E(:,1:numvec-1);
               V = V(1:numvec-1);
           else
               error(message('signal:pmtm:inadequateNumtapers', '3', 'Droplasttaper', 'true'));
           end
      else
           if numvec < 2
               error(message('signal:pmtm:inadequateNumtapers', '2', 'Droplasttaper', 'false'));
           end
      end
      NW值
      NW = NW{1};
      if nargin > 2, nfft_temp = findNFFT(varargin{2:end}); end
        E，v提前设计好。
   elseif length(NW)>1,     % NW is the matrix E (==>V must be specified)
      E = NW;
      if length(varargin)<2,
         error(message('signal:pmtm:MustProvideVWithE', 'V', 'E'));
      else
         V = varargin{2};
         if nargin > 3, nfft_temp = findNFFT(varargin{3:end}); end
      end
      numvec = length(V);
      if size(E,2)~=numvec
         error(message('signal:pmtm:MismatchEV', 'E', 'V'));
      end     
      NW = size(E,2)/2;  
      indx = 3; % Update index of beginning of options in the input arg list      
      if dlt
          if (numvec < 3)
               error(message('signal:pmtm:inadequateNumtapers', '3', 'Droplasttaper', 'true'));
          else
              E = E(:,1:numvec-1);
              V = V(1:numvec-1);
          end
      else
          if(numvec < 2)
               error(message('signal:pmtm:inadequateNumtapers', '2', 'Droplasttaper', 'false'));
          end
      end
如果NW是个标量
   else                      % NW is a scalar
       if (NW<1.25 && dlt)
          error(message('signal:pmtm:insufficientTimebandwidthproduct', 'NW', '1.25', 'Droplasttaper', 'true'));
       end
       if (NW<0.75 && ~dlt)
          error(message('signal:pmtm:insufficientTimebandwidthproduct', 'NW', '0.75', 'Droplasttaper', 'false'));
       end
       % Get the dpss, one way or another:
       [E,V] = dpss(N,NW);
       numvec = length(V);
       if dlt
           if numvec > 2
               E = E(:,1:numvec-1);
               V = V(1:numvec-1);
           else
               error(message('signal:pmtm:inadequateNumtapers', '3', 'Droplasttaper', 'true'));
           end
       else
           if numvec < 2
               error(message('signal:pmtm:inadequateNumtapers', '2', 'Droplasttaper', 'false'));
           end
       end
       if nargin > 2, nfft_temp = findNFFT(varargin{2:end}); end
   end
else
   % Get the dpss, one way or another:
   [E,V] = dpss(N,NW);
   numvec = length(V);
      if dlt
           if numvec > 2
               E = E(:,1:numvec-1);
               V = V(1:numvec-1);
           else
               error(message('signal:pmtm:inadequateNumtapers', '3', 'Droplasttaper', 'true'));
           end
      else
           if numvec < 2
               error(message('signal:pmtm:inadequateNumtapers', '2', 'Droplasttaper', 'false'));
           end
      end
   nfft_temp = [];
end
```




