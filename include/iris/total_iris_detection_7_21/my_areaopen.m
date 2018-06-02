% 2011_7_20 盛先
% 寻找I_bw二值图像中为1的点的区域的个数以及大小8连通
% I_bw = [0 0 0 0 0 0 0 0; 
%     0 1 0 1 1 0 0 0;
%     0 1 0 1 0 0 1 0;
%     0 0 1 0 0 1 1 0;
%     0 0 0 0 1 1 0 0;
%     0 0 0 0 0 0 0 0];
% AREA为设定的小于AREA的面积的连通域时 将之置为0
function I = my_areaopen(I_bw, AREA)

I_ind = my_connect_domain(I_bw);
I = I_bw;
% 找出每个连通域的面积大小
for ii = 1:max(max(I_ind))
    [r, c] = find(I_ind == ii);
    area = length(r);
    if area <= AREA
        I(r,c) = 0;
    end
end

