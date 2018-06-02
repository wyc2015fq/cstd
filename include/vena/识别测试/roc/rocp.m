function  [FAR FRR]=rocp(score,lable)
score=score(:);
lable=lable(:);
Nthr=100;
thresh=linspace(min(score),max(score),Nthr);%将min到max分成100个距离矩阵
acc=zeros(Nthr,1);
FAR=acc;
FRR=acc;
for i=1:Nthr
    thr=thresh(i);
    result=score>thr;
    FAR(i)=(sum((result==1)&lable==0))/sum(lable==0);%错误接受  FAR为错误接受数与类间测试总数  类间
    %lable为距离标签，类内为1，类间为0.
    %类间距离大于阈值，错误接受
    %类间距离小于阈值，错误拒绝

    FRR(i)=(sum((result==0)&lable==1))/sum(lable==1);%错误拒绝  FRR为错误拒绝数与类内测试总数  类内
end

%xlim([10^(-6) 10^(-2)]);
%ylim([10^(-0.2)     1]);
%plot((FAR),(FRR));

plot(FAR,FRR,'r');

xlabel('错误接受率FAR');
ylabel('错误拒绝率FRR');



