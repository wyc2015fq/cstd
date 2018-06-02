function [I, nx, ny, nr] = nei_yuan(Op_x, Op_y, Rp, I_source, pic_xsize, pic_ysize)
% 4_9 划出内圆  以及其中心点 nx, ny，半径nr

% 定义内圆 pupil极坐标图像大小 与 相位间距（分360份）
np = 360;  dp = pi / 180;
Op_x = round(Op_x);
Op_y = round(Op_y);
% Rp_p = Rp(m-1);
Rp_p = Rp;
I = I_source;
I(Op_x, Op_y) = 255;

for i = 0 : np
    p = dp * i;
    % 求直角坐标中对应的点
    x = round(Op_x + Rp_p * cos(p));
    y = round(Op_y + Rp_p * sin(p));
    if x < 1 || x > pic_xsize || y < 1 || y > pic_ysize
        continue;
    else
        I(x,y) = 255;
    end
end
% 内圆中心点和半径
nx = Op_x;
ny = Op_y;
nr = Rp_p;
% figure; imshow(I);
