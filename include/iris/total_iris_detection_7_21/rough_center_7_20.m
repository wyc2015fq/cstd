% x0,y0 分别为pupil的大致圆心，以及二值化后的图像
function [x0, y0] = rough_center_7_20(I)

I = im2double(I);
% 7_12 应该找到第一个波峰与第二个波峰之间的值，暂时定为30/256 = 0.1171 
I = im2bw(I, 0.1161);
% se = strel('disk',5);        
% I1 = imerode(~I,se);
I = myerode(~I,'se5');

% I = bwareaopen(I, 80, 4);   % 去除面积小于80个像素的噪点
% I = my_areaopen(I, 80);   % 去除面积小于80个像素的噪点
I = my_fill(I);        % 7_20 填充
% I2 = imfill(I, 8, 'holes');  % 填充

% 统计最后的为1的区域 找到最大区域
I_ind = my_connect_domain(I);
area_max = 0;
for ii = 1:max(max(I_ind))
    rc = find(I_ind == ii);
    area = length(rc);
    if area > area_max
        area_max = area;
        ind = ii;
    end
end

[r, c] = find(I_ind == ind);
x1 = r(1);
x2 = r(length(r));
y1 = c(1);
y2 = c(length(c));

[row, col] = size(I);
x0 = round((x1 + x2 + row) / 4);
y0 = round((y1 + y2 + col) / 4);