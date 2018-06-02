function [inte_image]=integral_image(image);
image_col=cumsum(image,2);
inte_image=cumsum(image_col,1);
%disp('完成积分图');