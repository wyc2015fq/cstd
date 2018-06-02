%%带距离的三核滤波器
function [B newM]=meansure2(T,M)

x1=30;
x2=360;

%%
fn=5;
alf=1.7;
dist=2;%%需要调整的参数

H1=fspecial('gaussian',fn,alf);
fn2=3*fn+2*dist;
H=zeros(fn,fn2);
H(:,1:5)=H1;
H(:,(fn+dist+1):(2*fn+dist))=-2*H1;
H(:,(fn2-fn+1):(fn2))=H1;

D=ones(5,30);
%%把图像划分成5*30有交叠的子区域
t1=2;
n1=13;
t2=2;
n2=165;
k=0;
n3=n1*n2;
B=zeros(n3,1);
newM=zeros(n3,1);
for i=1:n1
    for j=1:n2
        k=k+1;
        a1=1+(i-1)*t1;
        a2=a1+4;
        b1=1+(j-1)*t2;
        b2=b1+29;
        A1=T(a1:a2,b1:b2);
        c1=floor((30-fn2)/2);
        A2=A1(:,(1+c1):(c1+fn2));
        A3 = A2.*H;
        e1=sum(sum(A3));
        if (e1>0)
            B(k)=1;
        else
            B(k)=0;
        end
        M1=M(a1:a2,b1:b2);
        M2=M1&D;
        e2=sum(sum(M2));
        if (e2>60)
            newM(k)=1;
        else
            newM(k)=0;
        end
    end
end


