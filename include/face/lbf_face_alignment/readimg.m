function [img,realshapes,imgpathlists]=readimg(dbnames,mode,num_points)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%函数功能：读取样本数据，形状，路径
%输入：
%dbnames：数据库名
%mode：模式
%num_points：特征点数
%输出：
%img：样本数据
%realshapes：样本真实形状
%imgpathlists：样本所在路径
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
work_path = pwd;%%当前工作路径
imgpathlists=[];%样本路径初始化
for i=1:length(dbnames)
    imgpathlistfile = strcat(work_path,'\',mode,'\', dbnames{i}, '\Path_Images.txt');%txt路径
    imgpathlist = textread(imgpathlistfile, '%s', 'delimiter', '\n');%读取样本路径
    imgpathlists=[imgpathlists;imgpathlist];%几个数据库样本串起来
    fprintf(strcat(num2str(dbnames{i}),':',num2str(length(imgpathlist)),'\n'));%打印当前数据库中样本个数，此行可删除不要。
end
%变量初始化
num_samples=length(imgpathlists);%总样本数
img=cell(1,num_samples);
realshapes=zeros(num_samples,num_points*2);

for i_sample=1:num_samples
    pic_path = imgpathlists{i_sample};%样本i路径
    img{i_sample} = im2uint8(imread(pic_path));%读取样本i数据并转成8位无符号型
    intershape=getshape(pic_path);%读取样本i真实的人脸形状
    realshapes(i_sample,:)=intershape(:);%把2列数据串接成一列    
end
end