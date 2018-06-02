% 4_8 转化为极坐标下的图像
% 4-13 直接的从(x0,y0)竖直向下 逆时针旋转为对应的 theta角度
function polarv = pupil_to_polar_coordinate(pic, x0, y0, Rmax)

% 定义内圆 pupil极坐标图像大小 与 相位间距（分360份）
np = 360;  dp = pi / 180;
[pic_xsize, pic_ysize] = size(pic);

polarv = zeros(np, Rmax);
pic = double(pic);
for i = 0 : np - 1
    for j = 1 : Rmax
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize - 1;
            end
        end

        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize - 1;
            end
        end
        % 在直角坐标中的周围四点
        x1 = floor(x);
        y1 = floor(y);
        x2 = x1+1;
        y2 = y1+1;
        % 处理越界情况
        [x1, x2, y1, y2] = value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize);
        %在X和Y方向分别进行线性插值
        e = pic(x1, y1) * (x - x1) + pic(x2, y1) * (x2 - x);
        f = pic(x1, y2) * (x - x1) + pic(x2, y2) * (x2 - x);
        polarv(i+1, j) = abs(e * (y - y1) + f * (y2 - y));
    end
end
