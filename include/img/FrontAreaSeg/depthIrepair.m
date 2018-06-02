function RI=depthIrepair(DI,BRsize)
% 深度图简易填充
% 以深度图中0值点为中心，划取(2*BRsize+1) x (2*BRsize+1)大小的块，求取其中非零均值作为中心点的深度值
% DI : 原始深度图
% BRsize : 块半径，这里限制了块大小，所以BRsize应该小于或者等于12
% 对于某些边角空洞，可能无法填充
% edit time: 2013.07.05
if BRsize>12
    error('块大小超过程序所设定的上限，块大小应该小于或者等于12');
end
if BRsize<0
    error('块大小不符合要求，块大小应该大于0');
end

DI=double(DI); % 将深度图转换为double类型
[m,n]=size(DI); % 原始深度图大小
TDI=ones(size(DI)+26); % 定义一个矩阵用于寻找深度图中空洞像素的坐标
TDI(13:m+12,13:n+12)=DI; 
index=find(TDI==0); % 寻找原始深度图中的空洞像素坐标
[xc,yc]=ind2sub(size(TDI),index); % 寻找原始深度图中的空洞像素坐标
N=length(xc); % 空洞像素数目
TDIT=TDI; % 定义一个操作矩阵
TDIT(TDI==1)=0; % 将所有的深度值为1的像素点全部赋值为0
for p=1:N  %　对于所有空洞像素
    IP=TDIT(xc(p)-BRsize:xc(p)+BRsize,yc(p)-BRsize:yc(p)+BRsize); % 块区域划定
    A=sum(IP(:)); % 求取块区域所有深度值    
    AB=IP~=0; % 寻找非零点
    B=sum(AB(:)); % 非零点个数
    if B==0 % 如果区域中没有非零点，为了避免分母为零，令B=1
        B=1;
    end
    TDIT(xc(p),yc(p))=round(A/B); % 非零点均值
end
RI=TDIT(13:m+12,13:n+12); % 返回简易填充后的深度图，大小和原始深度图一致
end
