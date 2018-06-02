%%%%边缘检测%%%%%% 
function BW2=edgecrop(shuru1,kk)
%%shuru1为需要检测边缘的图像，kk为lvboo的参数值，该值影响很大，去掉小边缘
% [BW2,tv]=edge(double(shuru1),'sobel','vertical'); 
BW2=edge(shuru1);
% figure(2); 
% imshow(BW2)  
% se=strel('disk',2); 
% BW2=imclose(BW2,se);%闭运算 
% % % figure(3); 
% % % imshow(BW2) 
% %  BW2=bwmorph(BW2,'thin',Inf);%骨骼化 
BW2=lvboo(BW2,kk);%去出噪声 
% figure(4); 
% imshow(BW2) 