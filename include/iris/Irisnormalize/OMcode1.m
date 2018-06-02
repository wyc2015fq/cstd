function [mixB newM]=OMcode1(T,M)


%%
%%把图像划分成16*64有交叠的子区域4*75=300
x0=16;
y0=64;
n1=4;
n2=75;

x=[1 5 10 15];
y=zeros(1,n2);
for i=1:n2
    y(i)=1+4*(i-1);
end

%%
fn=5;
alf=1.7;
H0=fspecial('gaussian',fn,alf);


mixB0=[];
newM=[];

for i=1:n1
    for j=1:n2
        x1=x(i);
        x2=x(i)+x0-1;
        y1=y(j);
        y2=y(j)+y0-1;
        subT=T((x1:x2),(y1:y2));
        subM=M((x1:x2),(y1:y2));
        newM=[newM,subM];
        %%带距离的两核滤波器
        for k=1:6
            dist=k-1;
            fn2=2*fn+dist;
            H=zeros(fn,fn2);
            H(:,1:fn)=H0;
            H(:,(fn2-4):fn2)=-H0;
            
            B=imfilter(subT,H);                
            mixB0=[mixB0,B];            
        end
        %%带距离的三核滤波器
        for k=1:6
            dist=k-1;
            fn2=3*fn+2*dist;
            H=zeros(fn,fn2);
            H(:,1:fn)=H0;
            H(:,(fn+dist+1):(2*fn+dist))=-2*H0;
            H(:,(fn2-fn+1):(fn2))=H0;
            
            B=imfilter(subT,H);
            mixB0=[mixB0,B];  
        end
    end
end

%%二值化
[m1 m2]=size(mixB0);
mixB=zeros(m1,m2);
for i=1:m1
    for j=1:m2
        if mixB0(i,j)>0
           mixB(i,j)=1;
        end
    end
end


