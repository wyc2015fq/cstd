% 寻找I_bw二值图像中为1的点的区域的个数以及大小  8连通
% I_bw = [0 0 0 0 0 0 0 0; 
%     0 1 0 1 1 0 0 0;
%     0 1 0 1 0 0 1 0;
%     0 0 1 0 0 1 1 0;
%     0 0 0 0 1 1 0 0;
%     0 0 0 0 0 0 0 0];
% AREA为设定的小于AREA的面积的连通域时 将之置为0
function I = my_conncomp(I_bw, AREA)

[row, col] = size(I_bw);
% 因为I_bw的周边行列均为0  这是前面算出的先验条件

% 从左上角开始扫描
I_ind1 = zeros(size(I_bw));   % 存储为1的标号
ind1 = 1;
for ii = 2:row-1
    for jj = 2:col-1
        if I_bw(ii,jj) == 1
            Top_left = I_ind1(ii-1,jj-1) + I_ind1(ii-1,jj) + I_ind1(ii-1,jj+1) + I_ind1(ii,jj-1);
            if Top_left
                vect = [I_ind1(ii-1,jj-1), I_ind1(ii-1,jj), I_ind1(ii-1,jj+1), I_ind1(ii,jj-1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind1(ii,jj) = min(vect);                
            else
                I_ind1(ii,jj) = ind1;
                ind1 = ind1+1;
            end
        end
    end
end

% 从右下角开始扫描
I_ind2 = zeros(size(I_bw));   % 存储为1的标号
ind2 = 1;
for ii = row-1 : -1 : 2
    for jj = col-1 : -1 : 2
        if I_bw(ii,jj) == 1
            Top_left = I_ind2(ii+1,jj-1) + I_ind2(ii+1,jj) + I_ind2(ii+1,jj+1) + I_ind2(ii,jj+1);
            if Top_left
                vect = [I_ind2(ii+1,jj-1), I_ind2(ii+1,jj), I_ind2(ii+1,jj+1), I_ind2(ii,jj+1)];
                for i = 1 : 4
                    if ~vect(i)
                        vect(i) = 100000;
                    end
                end
                I_ind2(ii,jj) = min(vect);                
            else
                I_ind2(ii,jj) = ind2;
                ind2 = ind2+1;
            end
        end
    end
end

if ind2 > ind1
    I_ind = I_ind1;
    ind = ind1-1;
else
    I_ind = I_ind2;
    ind = ind2-1;
end

I = I_bw;
% 找出每个连通域的面积大小
for ii = 1:ind
    [r, c] = find(I_ind == ii);
    area = length(r);
    if area <= AREA
        I(r,c) = 0;
    end
end

