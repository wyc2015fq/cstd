function distance=cmpdis(im1,im2,mask1,mask2)
%%im1,im2为归一化的测试图片和训练图片定序特征图
%%mask1,mask2为测试图片和训练图片的mask原始图片
mask1=255-mask1;%反转mask
mask2=255-mask2;
trmask=(find(mask1==255));
temask=(find(mask2==255));
result=xor(im1,im2);
result(trmask)=1;
result(temask)=1;
result=result(:);
%归一化处理
normval=length(result);
distance=sum(result)/normval;


