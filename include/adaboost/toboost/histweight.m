function [yresult,xresult,disval]=histweight(input,n,weight)
%input 是一个输入的向量
%n为将这个向量划分为n个区间，统计每个区间的分布直方图
%weight为输入向量的样本权重值向量，如果没有weight，该向量直方图统计分布可以直接用调用hist.m函数
lengthin=length(input);
 maxin=max(input);
 minin=min(input);
 disval=(maxin-minin)/n;
 for i=1:n
     xresult(i)=minin+disval*i;
 end
 yresult=zeros(1,n);
    for j=1:lengthin  
        if input(j)<xresult(1)
           yresulting=1*weight(j);
           yresult(1)=yresult(1)+yresulting;
        else  
           i=floor((input(j)-xresult(1))/disval)+1;
           yresulting2=1*weight(j);
           yresult(i)=yresult(i)+yresulting2;
        end       
    end