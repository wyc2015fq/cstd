# MTM：matlab实现4主函数 - 去向前方的博客 - CSDN博客





2018年07月05日 20:07:43[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：187








# 前言

之前讲了MTM（多锥形窗谱估计）的相关原理，现在来分析一下它的matlab实现。 

想要复习的可以参考一下之前的文件： 
[现代谱估计：多窗口谱](https://blog.csdn.net/lvsehaiyang1993/article/details/80361172)

想要复习一下如何实现的可以参考： 

MTM：matlab实现1[MTM：matlab实现1](https://blog.csdn.net/lvsehaiyang1993/article/details/80928212)

MTM：matlab实现2参数解析[MTM参数解析](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)

MTM：matlab实现3谱功率计算[MTM谱功率计算](https://blog.csdn.net/lvsehaiyang1993/article/details/80928955)
# 目录



- [前言](#前言)
- [目录](#目录)
- [主函数调用说明](#主函数调用说明)




# 主函数调用说明

```
function varargout = pmtm(x,varargin)
%PMTM   Power Spectral Density (PSD) estimate via the Thomson multitaper 
%   method (MTM).
%   Pxx = PMTM(X) returns the Power Spectral Density (PSD) estimate, Pxx,
%   of a discrete-time signal X. When X is a vector, it is converted to a
%   column vector and treated as a single channel.  When X is a matrix, the
%   PSD is computed independently for each column and stored in the
%   corresponding column of Pxx. Pxx is the distribution of power per unit
%   frequency. The frequency is expressed in units of radians/sample.
%
%   For real signals, PMTM returns the one-sided PSD by default; for 
%   complex signals, it returns the two-sided PSD.  Note that a one-sided 
%   PSD contains the total power of the input signal.
%
%   Pxx = PMTM(X,NW) specifies NW as the "time-bandwidth product" for the
%   discrete prolate spheroidal sequences (or Slepian sequences) used as 
%   data windows.  Typical choices for NW are 2, 5/2, 3, 7/2, or 4. If
%   empty or omitted, NW defaults to 4. By default, PMTM drops the last
%   taper because its corresponding eigenvalue is significantly smaller
%   than 1. Therefore, The number of tapers used to form Pxx is 2*NW-1.
%   
%   Pxx = PMTM(X,NW,NFFT) specifies the FFT length used to calculate the
%   PSD estimates.  For real X, Pxx has (NFFT/2+1) rows if NFFT is even,
%   and (NFFT+1)/2 rows if NFFT is odd.  For complex X, Pxx always has
%   length NFFT.  If NFFT is specified as empty, NFFT is set to either
%   256 or the next power of 2 greater than the length of X, whichever is
%   larger.
%
%   [Pxx,W] = PMTM(X,NW,NFFT) returns the vector of normalized angular 
%   frequencies, W, at which the PSD is estimated.  W has units of 
%   radians/sample.  For real signals, W spans the interval [0,Pi] when
%   NFFT is even and [0,Pi) when NFFT is odd.  For complex signals, W 
%   always spans the interval [0,2*Pi).
%
%   [Pxx,W] = PMTM(X,NW,W) computes the two-sided PSD at the normalized
%   angular frequencies contained in vector W. W must have at least two
%   elements.
%常规调用，输入为 x，nw，NFFT，fs
%   [Pxx,F] = PMTM(X,NW,NFFT,Fs) returns a PSD computed as a function of
%   physical frequency.  Fs is the sampling frequency specified in hertz.
%   If Fs is empty, it defaults to 1 Hz.
%
%   F is the vector of frequencies (in hertz) at which the PSD is
%   estimated.  For real signals, F spans the interval [0,Fs/2] when NFFT
%   is even and [0,Fs/2) when NFFT is odd.  For complex signals, F always
%   spans the interval [0,Fs).
%
%   [Pxx,F] = PMTM(X,NW,F,Fs) computes the two-sided PSD at the frequencies
%   contained in vector F.  F is a vector of frequencies in Hz with 2 or
%   more elements.
%
%   [Pxx,F] = PMTM(...,Fs,method) uses the algorithm specified in method 
%   for combining the individual spectral estimates:
%      'adapt'  - Thomson's adaptive non-linear combination (default).
%      'unity'  - linear combination with unity weights.
%      'eigen'  - linear combination with eigenvalue weights.
%
%   [Pxx,F] = PMTM(X,E,V,NFFT,Fs,method) is the PSD estimate, and frequency
%   vector from the data tapers in E and their concentrations V.  Type HELP
%   DPSS for a description of the matrix E and the vector V. By default,
%   PMTM drops the last eigenvector because its corresponding eigenvalue is
%   significantly smaller than 1.
%
%   [Pxx,F] = PMTM(X,DPSS_PARAMS,NFFT,Fs,method) uses the cell 
%   array DPSS_PARAMS containing the input arguments to DPSS (listed in
%   order, but excluding the first argument) to compute the data tapers. 
%   For example, PMTM(x,{3.5,'trace'},512,1000) calculates the prolate 
%   spheroidal sequences for NW=3.5, NFFT=512, and Fs=1000, and displays
%   the method that DPSS uses for this calculation. Type HELP DPSS for 
%   other options.
%
%   [Pxx,F] = PMTM(...,'DropLastTaper',DROPFLAG) specifies whether PMTM
%   should drop the last taper/eigenvector during the calculation. DROPFLAG
%   can be one of the following values: [ {true} | false ].
%       true  - the last taper/eigenvector is dropped 
%       false - the last taper/eigenvector is preserved
%
%   [Pxx,F,Pxxc] = PMTM(...,'ConfidenceLevel',P) returns the P*100%
%   confidence interval for Pxx, where P is a scalar between 0 and 1. The
%   default value for P is .95.  Confidence intervals are computed using a
%   chi-squared approach. Pxxc has twice as many columns as Pxx.
%   Odd-numbered columns contain the lower bounds of the confidence
%   intervals; even-numbered columns contain the upper bounds.  Thus,
%   Pxxc(M,2*N-1) is the lower bound and Pxxc(M,2*N) is the upper bound
%   corresponding to the estimate Pxx(M,N).
%
%   [...] = PMTM(X,...,FREQRANGE) returns the PSD over the specified range
%   of frequencies based upon the value of FREQRANGE:
%
%      'onesided' - returns the one-sided PSD of a real input signal X.
%         If NFFT is even, Pxx has length NFFT/2+1 and is computed over the
%         interval [0,pi].  If NFFT is odd, Pxx has length (NFFT+1)/2 and
%         is computed over the interval [0,pi). When Fs is specified, the
%         intervals become [0,Fs/2) and [0,Fs/2] for even and odd NFFT,
%         respectively.
%
%      'twosided' - returns the two-sided PSD for either real or complex
%         input X.  Pxx has length NFFT and is computed over the interval
%         [0,2*pi). When Fs is specified, the interval becomes [0,Fs).
%
%      'centered' - returns the centered two-sided PSD for either real or
%         complex X.  Pxx has length NFFT and is computed over the interval
%         (-pi, pi] for even length NFFT and (-pi, pi) for odd length NFFT.
%         When Fs is specified, the intervals become (-Fs/2, Fs/2] and
%         (-Fs/2, Fs/2) for even and odd length NFFT, respectively.
%
%      FREQRANGE may be placed in any position in the input argument list
%      after the second input argument, unless E and V are specified, in
%      which case FREQRANGE may be placed in any position after the third
%      input argument.  The default value of FREQRANGE is 'onesided' when X
%      is real and 'twosided' when X is complex.
%
%   PMTM(...) with no output arguments plots the PSD estimate (in decibels
%   per unit frequency) in the current figure window.
%
%   EXAMPLE:
%      Fs = 1000;   t = 0:1/Fs:.3;  
%      x = cos(2*pi*t*200)+randn(size(t)); % A cosine of 200Hz plus noise
%      pmtm(x,3.5,[],Fs);                  % Uses the default NFFT.
%
%   See also DPSS, PWELCH, PERIODOGRAM, PMUSIC, PBURG, PYULEAR, PCOV,
%   PMCOV, PEIG.

%   References: 
%     [1] Thomson, D.J."Spectrum estimation and harmonic analysis."
%         In Proceedings of the IEEE. Vol. 10 (1982). pp. 1055-1096.
%     [2] Percival, D.B. and Walden, A.T., "Spectral Analysis For Phy 
ical
%         Applications", Cambridge University Press, 1993, pp. 368-370. 

%   Copyright 1988-2014 The MathWorks, Inc.
```




