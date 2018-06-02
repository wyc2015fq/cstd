%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%该程序功能是提取单个图片感兴趣区域，单个图片实现效果较好。
%%提取步骤：
%1.读取图片，灰度变换，旋转90。
%2.初步裁剪图片，高斯滤波
%3.边缘检测
%4.根据边缘图像大致找到手指的左右边缘，从而初步找出背景，去掉背景
%5.利用写出的纠正角度函数，纠结图片的角度
%6.关节处高度截取：找出指关节处，再截取ROI高度。
%7.截取宽度，提取出ROI区域
%%%%%%%%%%%%%1.读取图片+灰度变换%%%%%%%%%%%%%%%%%%%%%%%%%
%clear all;
clc;
tic;
shuru=imread('b.BMP');   %%读取图片
%figure,imshow(shuru),title('原图像');
if ndims(shuru)==3
    shuru=rgb2gray(shuru);
end
[mm, nn]=size(shuru);
if mm<nn
    shuru=rot90(shuru);
end
%%%%%%%%%%%2.初步裁剪图片，高斯滤波%%%%%%
shuru=shuru(80:480,:);
shuru2 = shuru;
shuru=double(shuru);
h=fspecial('gaussian');
shuru1=filter2(h,shuru);
%%%%%%%%%%%%%%3.边缘检测%%%%%%%%%%%%%%%%
BW2=edgecrop(shuru1,50);
% imshow(BW2);
% figure
% imshow(BW2,[]);
[MM, NN]=size(BW2);
colall=[];
%%%%4.根据边缘图像大致找到手指的左右边缘，从而初步找出背景，去掉背景%%%%%
for i=1:MM
    COL=find(BW2(i,:)~=0);
    if length(COL)>1;
        col=[i COL(1) COL(2)];
        colall=[colall;col];
    end
end
oneing=zeros(MM,NN);
[result1,result2]=right_dis(colall);
if result1~=0&&result2~=0
    oneing(:,result1:result2)=1;
else
    oneing=ones(MM,NN);
end
shuru1=shuru1.*oneing;  %%去背景
shuru2=shuru1;
%%%%%%%%5.利用写出的纠正角度函数，纠结图片的角度%%%%%%%
%shuru1=correct_angle(shuru1);
% figure
% imshow(shuru1,[]);
% BW2=edgecrop(shuru1,100);
% figure
% imshow(BW2,[]);
%%%%%%%%6.关节处高度截取：找出指关节处，再截取ROI高度%%%%%%%
roih=roi_height(shuru1,BW2);
% figure
% imshow(roih,[]);
% title('检测到的高度');
% BW2=edgecrop(roih,50);
% figure
%imshow(BW2);
shuru=uint8(roih);
%%%%%%%%%%%%7.截取宽度，提取出ROI区域%%%%%%%%%%%%%%%%%%%%%
[min1, max1]=bianjie(BW2);
k=shuru(:,min1:max1);
%k=shuru(:,min1:max1);
%figure,imshow(k); title('ROI');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%尺寸灰度规一划(数字图像处理) 归一化大小根据论文设置%%%%%%%
[m,n]=size(k);
s=[double(64/n) 0 0;
    0 double(96/m) 0;
    0 0 1];
tform=maketform('affine',double(s));
% k=imtransform(k,tform,'XData',[1 64],'YData',[1 96]); %%双线性插值
% k=imtransform(k,tform,'bilinear'); %%双线性插值
k=imtransform(k,tform,'XData',[1 64],'YData',[1 96],'FillValue',0);%%填0补充
[m,n]=size(k);
%%%%%%%%%%%%%%%%%%%%%%%%%%
p=max(max(k(:,:)))-min(min(k(:,:)));
y=double(256/double(p));
k(:,:)=double((double(y)*double((k(:,:)-min(min(k(:,:)))))));  %%直方图归一化处理
shuchu=k;
figure,imshow(shuchu,[]),title('ROIguiyihua');
toc;
