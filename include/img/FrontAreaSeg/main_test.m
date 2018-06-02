% main
% edit: 2013.07.24
%% 前景区域提取参数设置
ImerodeThresh=10; % 腐蚀阈值
ImdilateThresh=15; % 膨胀阈值
AreaThresh=1500; % 面积阈值

BRsize=12; % 前景修复阈值

%% 聚类参数
RN=25;
RD=5;

%% 滤波参数
FilterLayerThresh=170; % 深度滤波
FilterAreaThresh=700; % 面积滤波
verson='Y'; % 选择进行腐蚀

BI=imread('G:\matlab_test\peopledetect\testlargedata\depthBW\depth00221.JPG_fg_dep.bmp'); % 前景二值图
DI=imread('G:\matlab_test\peopledetect\testlargedata\depth\depth00221.png'); % 深度图
DI=double(DI); % 类型转换 
CI=imread('G:\matlab_test\peopledetect\testlargedata\color\color00221.jpg'); % 彩色图
RDI=depthIrepair(DI,BRsize); % 前景修复
FrontI=FrontAreaSeg(BI,ImerodeThresh,ImdilateThresh,AreaThresh); % 前景区域提取
FRDI=RDI.*FrontI; % 前景区域
g=Rainclusteredit(FRDI,RN,RD); % 聚类
[FAI,LayerFilterI]=LayerAreaFilter(g,FilterLayerThresh,FilterAreaThresh,verson); % 滤波
