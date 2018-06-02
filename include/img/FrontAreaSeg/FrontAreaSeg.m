function FrontRepairI=FrontAreaSeg(RDI,BI,ImerodeThresh,ImdilateThresh,AreaThresh)
% RDI: 简单修复后的深度图
% BI：前景二值图
% ImerodeThresh：腐蚀结构元素大小 , 建议 10
% ImdilateThresh：膨胀结构元素大小 ，建议 15
% AreaThresh: 区域面积阈值, 建议 1500
% FrontRepairI：简单修复后的前景深度图
% 思想：先进行腐蚀，后去除小块噪声区域，然后对剩下的人头区域进行膨胀，最后对应修复后的深度图
% edit time: 2013.07.05
RDI=double(RDI);
BI=double(BI);
% 腐蚀去除小区域
TI=BI;
TI(BI==255)=1; % 转换成0，1
Se=strel('square',ImerodeThresh); % 腐蚀的结构元素定义
IME=imerode(TI,Se); % 腐蚀后的二值图
[L CNum]= bwlabel(IME,4); % 4 连通标记，L为标记后的边缘，N为标记区域数目
NTemp=zeros(CNum,2); % 用于存储给个区域的点数
for ConNum=1:CNum % 统计各个区域的点数,2存放点数，1存放索引
    Counts=L==ConNum; % 寻找连通区域
    NTemp(ConNum,2)=sum(Counts(:)); % 对找到的连通区域进行像素统计
    NTemp(ConNum,1)=ConNum; % 保存
end
ThreshArea=AreaThresh; % 区域面积阈值
AreaNum=NTemp(:,2);
AreaNum(AreaNum<ThreshArea)=0; % 滤除区域点数小于ThreshArea的小区域
NumLeft=sum(AreaNum~=0); % 大区域数目
[value index]=dsort(AreaNum); % 将点数从大到小排列
Tempsave=zeros(size(L));
for EraNum=1:NumLeft % 标记留下来的区域
    Tempsave(L==index(EraNum))=1;
end

SI=strel('square',ImdilateThresh); % 膨胀结构元素
IME=imdilate(Tempsave,SI); % 膨胀后的二值图

FrontRepairI=IME.*double(RDI); % 简单修复后的前景区域
end