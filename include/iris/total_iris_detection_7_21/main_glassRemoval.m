clc
clear all
%h = fspecial('gaussian',[3,3], 1.00)
fname='D:\pub\bin\iris\detect\S2001L01.BMP';
fname='D:\pub\bin\iris\test1pic\S2271L04.jpg';
img=imread(fname);
img_gray=img;
% m_o=[-1 -1 -1
%     -1 8 -1a
%     -1 -1 -1];
% [M N]=size(img_gray);
% ddd=conv2(img_gray,m_o);
% R=zeros(M,N);
% for i=30:M-30
%     for j=30:N-30
%    if img_gray(i,j)<100
%        R(i,j)=0;
%    else
%        R(i,j)=1;
%       % R(i-2:i+2,j-2:j+2)=1;
%    end
%     end
% end
%img_gray=rgb2gray(img);
img_result = glassRemoval(img_gray);
%imwrite(img_result, 'aaa.jpg');
imshow(img_result);
%imshow(R);


