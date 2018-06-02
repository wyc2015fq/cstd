
clc;
%clear all;
path='D:\pub\bin\iris\testPic\feat2\';
path1='D:\pub\bin\iris\testPic\feat\';
pa = dir([path '*.bmp']);
n=length(pa);

width=15;disx=1;disy=0;delta=0.8;
if 1
    om=zeros(n,30*360, 'uint8');
    ma=zeros(n,30*360, 'uint8');
    z=makekernel(width, disx, disy, delta);
    z1=makekernel(width, -disx, disy, delta);
    ordifilterall=z1-z;
    for ii=1:n
        id(ii)=str2num(pa(ii).name(2:5))*10 + ('R'==pa(ii).name(6));
        im=imread([path pa(ii).name]);
        imma=imread([path1 pa(ii).name])>0;
        fa=makeom(im, ordifilterall);
        %fa = fa & imma;
        % figure(1); imshow(fa);    figure(2); imshow(fa);    figure(3); imshow(imma);
        om(ii,:)=fa(:)';
        ma(ii,:)=imma(:)';
    end
end

istrain=zeros(1, n);
cnt=zeros(1, max(id)-min(id)+10);
for ii=1:n
    jj=id(ii)-min(id)+1;
    if cnt(jj)<2
        cnt(jj)=cnt(jj)+1;
        istrain(ii)=1;
    end
end
clear mask;
clear om0;
clear om1;
correct_cnt=0;
all_cnt=0;
set0=find(0==istrain);
set1=find(1==istrain);
id0=id(set0);id1=id(set1);
om0=om(set0,:);
om1=om(set1,:);
ma0=ma(set0,:);
ma1=ma(set1,:);
n0=length(set0);
n1=length(set1);
if 1
    for ii=1:n0
        fa=repmat(om0(ii,:), n1, 1);
        fm=repmat(ma0(ii,:), n1, 1);
        tt=xor(fa, om1);
        tt(find(fm==0))=1;
        tt(find(ma1==0))=1;
        dis=sum(tt, 2);
        [mindis iidt]=min(dis);
        iid(ii)=iidt;
        disp(ii);
    end
end
correct_cnt=sum(id0==id1(iid))/n0


