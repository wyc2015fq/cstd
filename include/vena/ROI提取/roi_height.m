function ROIH=roi_height(img,BW2)
%%%%%%该函数就是根据手指关节位置裁剪兴趣区域的高度
img=double(img);
cropimage=img;
%[L, N]=bwlabel(BW2,8);%%%%找出BW2中8邻域的连通域个数，输出L矩阵，个数为N。
[m,n]=size(BW2); %%%%求矩阵的行数m和列数n。
if mod(n,2)==0 % 边缘图像中点    n与2相除取余数
    mid=n/2;
else
    mid=(n+1)/2;
end
% 末梢关节
linesum=sum(cropimage,2); % 行累积，若是求每行之和，则为1
LN=length(linesum); % 行数目
LM=zeros(LN,1); %产生LN*1的零矩阵
ST=LM; % 用于保存窗口累积值
LM(1:50,1)=1; % 生成一个50个1，长度为LN的矩阵
for i=1:mid-49 % 确定末梢关节位置
    winsum=sum(LM.*linesum); % 窗口累积结果
    ST(i+49,1)=winsum; %保存在对应的位置
    LM(i,1)=0; % 窗口向下移动一行
    LM(i+50,1)=1;
end
[v, ind]=max(ST); % 寻找矩阵ST中的最大值v及其索引ind即为对应的末梢关节位置,ind为所在的行数
[m,n]=size(cropimage); % 初步裁剪的图像大小
r1=ind;
% 近端关节
[m,n]=size(cropimage); % 初步裁剪的图像大小
LN=length(linesum); % 行数目
LM=zeros(LN,1);
ST=LM; % 用于保存窗口累积值
scor=r1+80; % 近端关节搜索的起始坐标
LM=zeros(LN,1);
LM(scor:scor+49,1)=1; % 生成一个50个1，长度为LN的矩阵
for i=1:m-(scor-1)-49 % 确定近端关节位置
    winsum=sum(LM.*linesum);
    ST(i+scor+49-1,1)=winsum;
    LM(i+scor-1,1)=0;
    LM(i+scor+49,1)=1;
end
[v, ind]=max(ST);
[m,n]=size(cropimage);
r2=ind;
d1=r2-r1; % 近端关节和末梢关节的距离
d2=m-r2;  % 近端关节和图片最低行的距离
if r1<d1*0.3 % 为了保留更多信息，相应地将末梢关节位置上移，近端关节位置下移
    h1=5; % 该值有待考究
else
    h1=r1-d1*0.3;
end
if  d2<d1*0.1
    h2=m-5;
else
    h2=r2+d1*0.1;
end
if(ceil(h1)==0)
    ROIH=cropimage((ceil(h1)+1):ceil(h2),:); % 兴趣区域按高度裁剪
else
    ROIH=cropimage(ceil(h1):ceil(h2),:);
end