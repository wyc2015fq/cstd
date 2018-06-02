function histogram = my_histogram_v(ymin,ymax,xmin,xmax,I,D)
T=100;
E=0.00000000001;
C=1;


I_hsv=rgb2hsv(I);
v=I_hsv(:,:,3);
v=v*255;
v=uint8(v);
[m,n]=size(v);

%求初始目标区域的直方图(开始)---------------------------------------------
t_bin_vector = zeros(256,1);

if ymin<1,ymin=1;end
if ymax>m,ymax=m;end
if xmin<1,xmin=1;end
if xmax>n,xmax=n;end

for i=ymin:ymax
    for j=xmin:xmax
        index = v(i,j)+1;%加1为了防止index为零
        t_bin_vector(index) = t_bin_vector(index) + 1;
    end
end
%求初始目标区域的直方图(结束)---------------------------------------------


%求目标周围区域的直方图(开始)---------------------------------------------
z_bin_vector = zeros(256,1);
b_bin_vector = zeros(256,1);
yymin=ymin-D;yymax=ymax+D;
xxmin=xmin-D;xxmax=xmax+D;


if yymin<1,yymin=1;end
if yymax>m,yymax=m;end
if xxmin<1,xxmin=1;end
if xxmax>n,xxmax=n;end

for i=yymin:yymax
    for j=xxmin:xxmax
        index = v(i,j)+1;
        z_bin_vector(index) = z_bin_vector(index) + 1;
    end
end

b_bin_vector=z_bin_vector-t_bin_vector;

%求目标周围区域的直方图(结束)---------------------------------------------


%求最后的颜色分布直方图(开始)---------------------------------------------
histogram = zeros(256,1);
for i=1:256
    if max(t_bin_vector(i,1),E)/max(b_bin_vector(i,1),E)>T;
        histogram(i,1)=(1/C)*T;
    end
    if max(t_bin_vector(i,1),E)/max(b_bin_vector(i,1),E)>1&&max(t_bin_vector(i,1),E)/max(b_bin_vector(i,1),E)<=T;
        histogram(i,1)=(1/C)*max(t_bin_vector(i,1),E)/max(b_bin_vector(i,1),E);
    else
        histogram(i,1)=0;
    end
end
%求最后的颜色分布直方图(结束)---------------------------------------------
