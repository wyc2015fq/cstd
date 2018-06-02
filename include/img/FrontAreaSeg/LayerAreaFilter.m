function [FAI,LayerFilterI]=LayerAreaFilter(g,LayerThresh,AreaThresh,verson)
% g:原始聚类图片
% LayerThresh：深度滤波阈值
% AreaThresh：面积滤波阈值
% verson：用于选择是否进行g的腐蚀
%         Y 表示进行腐蚀，N 表示不进行腐蚀
% 先深度滤波，后区域滤波，采用四连通区域
% edit time: 2013.07.02
Order_verson=1;
% Order_verson=1; 先深度滤波，后腐蚀，最后面积
% Order_verson=1；先腐蚀，后深度滤波，最后面积 

if Order_verson==1
    Tg=g;
    LayThresh=LayerThresh; % 深度阈值
    for gsx=1:size(Tg,1)  % 对g进行元素值大小滤波
        for gsy=1:size(Tg,2)
            if Tg(gsx,gsy)<LayThresh % 小于一定的数目，去除
                Tg(gsx,gsy)=0;
            end
        end
    end
    LayerFilterI=Tg;    
    if strcmp(verson,'Y') % 对 g 进行腐蚀
        Bg=LayerFilterI;
        Bg(LayerFilterI>0)=1; % 对 g 进行二值化
        Se=strel('square',6);
        IME=imerode(Bg,Se); % 腐蚀后的二值图
        Tg=IME.*LayerFilterI;
    end    
    if strcmp(verson,'N') % 不对 g 进行腐蚀
        Tg=LayerFilterI;
    end
end
if Order_verson==2
    if strcmp(verson,'Y') % 对 g 进行腐蚀
        Bg=g;
        Bg(g>0)=1; % 对 g 进行二值化
        Se=strel('square',6);
        IME=imerode(Bg,Se); % 腐蚀后的二值图
        Tg=IME.*g;
    end
    if strcmp(verson,'N') % 不对 g 进行腐蚀
        Tg=g;
    end
    LayThresh=LayerThresh; % 深度阈值
    for gsx=1:size(Tg,1)  % 对g进行元素值大小滤波
        for gsy=1:size(Tg,2)
            if Tg(gsx,gsy)<LayThresh % 小于一定的数目，去除
                Tg(gsx,gsy)=0;
            end
        end
    end
    LayerFilterI=Tg;
end

Tg(Tg~=0)=1; % 寻找非零点
[L N]= bwlabel(Tg,4); % 4 连通标记，L为标记后的边缘，N为标记区域数目
Temp=zeros(N,2); % 用于存储给个区域的点数
for ConNum=1:N % 统计各个区域的点数,2存放点数，1存放索引
    Counts=L==ConNum;
    Temp(ConNum,2)=sum(Counts(:));
    Temp(ConNum,1)=ConNum;
end
ThreshArea=AreaThresh;% 区域大小阈值，改变直接影响结果，和雨滴数目，g元素的大小决定这最终结果
AreaNum=Temp(:,2);
AreaNum(AreaNum<ThreshArea)=0; % 滤除区域点数小于ThreshArea的小区域
NumLeft=sum(AreaNum~=0); % 大区域数目
[value index]=dsort(AreaNum); % 将点数从大到小排列
Tempsave=zeros(size(L));
for EraNum=1:NumLeft % 标记流下来的区域
    Tempsave(L==index(EraNum))=1;
end
gArea=g.*Tempsave; % 对原始g进行剪切，找出符合大小的区域
FAI=gArea;
end