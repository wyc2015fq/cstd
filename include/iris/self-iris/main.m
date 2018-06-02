%%%%图像通过构造的多种定序滤波后的输出图像%%%%%%%%%
%%%%将之前已经构造的多种定序滤波load进行，文件ordifilter2L，ordifilter3L后缀名23表示核个数,L表示水平方向%%%%%%%
%%%%%函数返回值allfimg为一个元包，包含所有定序滤波的图片
function allfimg=main(im)
%%%测试该函数%%%%%%%%%%%
% clear all;
% clc;
% im=imread('1.bmp');
% im=double(im);
%%%%%%%%%%%%%%%%%%%%%%%  
file='C:\Documents and Settings\Administrator\桌面\需传送文件\OMkernel\';
sita=cell(1,1);
sita{1}='L'; 
allfimg=[];
for k=1:2  %两种核
    allimg=[];   
OM=load(strcat(file,'ordifilter',num2str(k+1),sita{1}));
OM=OM.ordifilterall;
[m n]=size(OM);
fimg=cell(m,n);
allimging=cell(m*n,1);
for i=1:m
    for j=1:n
        fimg{i,j}=makeom(im,OM{i,j});%%%滤波后的图片%%
        allimging{(i-1)*n+j,1}=fimg{i,j};
    end
end
allimg=[allimg; allimging];
allfimg=[allfimg;allimg];
end
%%测试图片滤波后图像%%
% for i=1:length(allfimg)
%     savefile=strcat('D:\我的文档\om1\',num2str(i),'.bmp');
%     tt=allfimg{i,1};
%    imwrite(tt,savefile);
% end
%%%%%%%%%%%%%%%%%%%%



