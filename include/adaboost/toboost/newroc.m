function  AUC=newroc(output,Y,weight)
%%output表示每个样本在直方图分布上的最终得分，Y表示每个样本的标签，即正负样本标号
%%%%函数举例%%%%
% clear all;
% clc;
% % output=[1 2 3 4];
% % Y=[1 0 1 0];
% % weight=[0.25 0.25 0.25 0.25];
% output=rand(1,10);
% Y=[1  1 1 1 1 0 0 0 0 1];
% weight=[ 1/(12) 1/(12) 1/(12) 1/(12) 1/(12) 1/8 1/8 1/8 1/8 1/(12) ];
%%%%%%%%%
% [noutput,index]=sort(output);
% nY=Y(index);
% nweight=weight(index);
maxout=max(output);
minout=min(output);
thresh=minout:(maxout-minout)/10:maxout;    %%%%阈值的选取只选择了十个值
% sort_output = sort(output);
% thresh = ([minout-1 sort_output] + [sort_output maxout+1])*0.5;%阈值选择相邻两个数的平均值
posi= Y==1;
neg= Y==0;
FRR=[];
FAR=[];
for i=1:length(thresh)
       score=output-thresh(i);
       preout=(score>0);
%        preouting=preout'-nY;
       preouting=preout'-Y;
       rwrong_label=(preouting==1);
       awrong_label=(preouting==-1);
%        rwrong_weight=rwrong_label.*nweight;
%        awrong_weight=awrong_label.*nweight;
       rwrong_weight=rwrong_label.*weight;
       awrong_weight=awrong_label.*weight;
       frr=sum(rwrong_weight);
       FRR=[FRR frr];
       far=sum(awrong_weight);
       FAR=[FAR far];
end
 %%%%%%%
%  FAR=log(FAR);
[FAR,INDEX]=sort(FAR);
FRR=FRR(INDEX);
% FAR=[0 FAR 1];
% FRR=[0 FRR 1];
% AUC=trapz(FAR,FRR)
% plot(FAR,FRR)
% for i=1:length(FRR)-1
%     dleta(i)=FAR(i+1)-FAR(i);
% end
% dleta(length(FRR))=0;
% AUC=dleta.*FRR(1:end);
% AUC=sum(AUC);
% NEWFRR=FRR(2:end)-FRR(1);
% AUC=(FAR(2:end)-FAR(1:end-1))*[FRR(1) NEWFRR(2:end)+NEWFRR(1:end-1)]'/2;%
% % 正比例函数的面积
% new= FRR(1:end-1)-FRR(end);
% sumnew= new(1:end-1)+new(2:end);
% AUC=(FAR(2:end)-FAR(1:end-1))*[sumnew FRR(end-1)-FRR(end)]'/2;%反比例函数的面积
FRR=FRR-FRR(end);
AUC=sum((FAR(2:end)-FAR(1:end-1)).*(FRR(2:end)+FRR(1:end-1))/2);




      
      
      