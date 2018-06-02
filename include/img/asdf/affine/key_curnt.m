function [Rot Tran]=key_curnt(Rk,Tk,Rp,Tp,all_point,featurePoint1,simplefaceTri,u0,v0,kx,ky,X22)
imageFile1=strcat('E:\pic\yaw\',num2str(1),'.bmp');  %%%前一帧
[im1, des1, loc1] = sift(imageFile1);
featurePoint1=[loc1(:,2),loc1(:,1)];
[in2Dabc1 new3Dabc1 w_3D1] = Interest2D3D(all_point,featurePoint1,simplefaceTri,u0,v0,kx,ky,X22,Rk,Tk);
featurePoint1=featurePoint1(in2Dabc1,:);   %对于前一帧如果点（sift检测出来的特征点）落在脸部区域，这些点都重新存入featurePoint1
featurePoint1=featurePoint1(in2Dabc1,:);   %对于前一帧如果点（sift检测出来的特征点）落在脸部区域，这些点都重新存入featurePoint1
w_3D1=w_3D1(in2Dabc1,:);
for ii=1:size(featurePoint1,1)
    x=featurePoint1(ii,1);
    y=featurePoint1(ii,2);
    [img_warp xx yy] = keyimgWarp(Rk,Tk,Rp,Tp,im1,x,y,w_3D1(ii,:));
%     [final_featurePoint] = track_warp (double(img_warp), double(img_current_gray), yy,xx,5);
    featurePoint(ii,:)=[yy,xx];
end 



t=0;
for i1 = 1 : size(new3Dabc1,1)
  
   if match(i1)>0;
      t=t+1;
      new3Dabc(t,:)=new3Dabc1(i1,:);
      w_3D1(t,:)=w_3D(i1,:);
      featurePoint(t,:)=featurePoint2(match(i1) ,:);
   end
end


X=[featurePoint, new3Dabc]';
results= test_RANSAC_RT(X);
temp=results.Theta;
Rot=temp(1:3,1:3);
Tran=temp(:,4);