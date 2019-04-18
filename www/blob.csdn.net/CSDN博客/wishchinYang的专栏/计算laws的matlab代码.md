# 计算laws的matlab代码 - wishchinYang的专栏 - CSDN博客
2013年07月17日 14:49:09[wishchin](https://me.csdn.net/wishchin)阅读数：1391
很简单的代码：不过花了codeforge上的10个点，自己写也早写出来了;
  代码如下：
文件：calLaws.m
```python
function [y,h_v,h_h]=calLaws(x,id,LocalEnergy)
for dim=1:2
    if dim==1
        FilterId=upper(id(1:2)); % Input argument "id" is undefined.
    else
        FilterId=upper(id(3:4));
    end
    if strcmp(FilterId,'L5'), h(dim,1:5)=[ 1  4  6  4  1];
    elseif strcmp(FilterId,'E5'), h(dim,1:5)=[-1 -2  0  2  1];
    elseif strcmp(FilterId,'S5'), h(dim,1:5)=[-1  0  2  0 -1];
    elseif strcmp(FilterId,'W5'), h(dim,1:5)=[-1  2  0 -2  1];
    elseif strcmp(FilterId,'R5'), h(dim,1:5)=[ 1 -4  6 -4  1];
    elseif strcmp(FilterId,'__'), h(dim,1:5)=[ 0  0  1  0  0];
    end
end
h_v=h(1,:);
h_h=h(2,:);
% Perform the filtering
y=filter2(h_v,filter2(h_h,x));
if exist('LocalEnergy')==1 
    y=avgsmth(abs(y),LocalEnergy);%LocalEnergy?????????5??????
end
```
文件 avgsmth.m
```python
function y=avgsmth(x,M,N)
% AVGSMTH  Smooth an image with an MxN averaging filter
%       y=avgsmth(x,M,N)
if nargin<3,%若输入参数小于三个！！！
  N=M;
end
w=ones(M,1)/M;
y=filter2(w,x);
w=ones(1,N)/N;
y=filter2(w,y);
```
