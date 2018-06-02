function plot_location(X1,Y1,X3,Y3,I_rgb,d2,t,RGB)

tempX1 = X1;tempY1 = Y1;
tempX2 = X3;tempY2 = Y1;
tempX3 = X3;tempY3 = Y3;
tempX4 = X1;tempY4 = Y3;

[pic_h,pic_w,D] = size(I_rgb);
if ndims(I_rgb) ==3
    I_temp(1:pic_h,1:pic_w,:) = I_rgb;
else
    I_temp(1:pic_h,1:pic_w,1) = I_rgb;
    I_temp(1:pic_h,1:pic_w,2) = I_rgb;
    I_temp(1:pic_h,1:pic_w,3) = I_rgb;
end
I_temp(tempY1,tempX1:tempX2,:) = 0;
I_temp(tempY3,tempX1:tempX2,:) = 0;
I_temp(tempY1:tempY3,tempX1,:) = 0;
I_temp(tempY1:tempY3,tempX2,:) = 0;

I_temp(tempY1,tempX1:tempX2,RGB) = 255;
I_temp(tempY3,tempX1:tempX2,RGB) = 255;
I_temp(tempY1:tempY3,tempX1,RGB) = 255;
I_temp(tempY1:tempY3,tempX2,RGB) = 255;

rusult_path=strcat('E:\result\',strtok(num2str(d2(t).name),'.'),'.jpg');%½á¹ûÐÎ×´
imwrite(I_temp,rusult_path);
pause(0.1);