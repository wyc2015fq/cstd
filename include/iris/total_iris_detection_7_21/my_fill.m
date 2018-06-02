% 2011_7_20 盛先
% 填充二值化图像 当像素为0时，其四周均有一的存在则将其填充为1
function I = my_fill(I_bw)
% 空洞填充
% 先找连通域，然后连通域内的点如果有0点，则填充？？
% I2 =[0     0     0     0     0;
%      0     1     1     1     0;
%      0     1     0     1     0;
%      0     1     1     1     0;
%      0     0     0     0     0];

I_ind = my_connect_domain(I_bw);
I = I_bw;
[row, col] = size(I_bw); 
for ii = 2 : row-1
    for jj = 2 : col-1
        if ~I_bw(ii,jj)
            [aa,bb,left] = find(I_ind(ii, jj-1:-1:1),1,'first');
            [aa,bb,right] = find(I_ind(ii,jj+1:1:end),1,'first');
            [aa,bb,top] = find(I_ind(ii-1:-1:1,jj),1,'first');
            [aa,bb,bottom] = find(I_ind(ii+1:1:end,jj),1,'first');
            if (~isempty(left))&&(~isempty(right))&&(~isempty(top))&&(~isempty(bottom))
                if (left == right) && (left == top) && (left == bottom)
                    I(ii,jj) = 1;
                end
            end
        end
    end
end
