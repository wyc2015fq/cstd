# matlab laplacian of gaussian(拉普拉斯高斯) 图像滤波 - 家家的专栏 - CSDN博客





2011年08月24日 17:14:39[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：5147








 示例代码

function e = log_edge(a)

%a = imread('img1.bmp');

sigma=0.15;    %sigma=0.5;

[m,n]=size(a);

e=repmat(logical(uint8(0)),m,n);

rr=2:m-1;

cc=2:n-1;

fsize=ceil(sigma*7)*6+1;   %fsize=ceil(sigma*3)*2+1;

op=fspecial('log',fsize,sigma);

op=op-sum(op(:))/prod(size(op));

b=filter2(op,a);

thresh=1.75*mean2(abs(b));   %thresh=.75*mean2(abs(b));

[rx,cx]=find(b(rr,cc)<0&b(rr,cc+1)>0&abs(b(rr,cc)-b(rr,cc+1))>thresh);

e((rx+1)+cx*m)=1;

[rx,cx]=find(b(rr,cc-1)>0&b(rr,cc)<0&abs(b(rr,cc-1)-b(rr,cc))>thresh);

e((rx+1)+cx*m)=1;

[rx,cx]=find(b(rr,cc)<0&b(rr+1,cc)>0&abs(b(rr,cc)-b(rr+1,cc))>thresh);

e((rx+1)+cx*m)=1;

[rx,cx]=find(b(rr-1,cc)>0&b(rr,cc)<0&abs(b(rr-1,cc)-b(rr,cc))>thresh);

e((rx+1)+cx*m)=1;

[rz,cz]=find(b(rr,cc)==0);

if isempty(rz)

    zero=(rz+1)+cz*m;

    zz=find(b(zero-1)<0&b(zero+1)>0&abs(b(zero-1)-b(zero+1))>2*thresh);

    e(zero(zz))=1;

    zz=find(b(zero-1)>0&b(zero+1)<0&abs(b(zero-1)-b(zero+1))>2*thresh);

    e(zero(zz))=1;

    zz=find(b(zero-m)<0&b(zero+m)>0&abs(b(zero-m)-b(zero+m))>2*thresh);

    e(zero(zz))=1;

    zz=find(b(zero-m)>0&b(zero+m)<0&abs(b(zero-m)-b(zero+m))>2*thresh);

    e(zero(zz))=1;

end



