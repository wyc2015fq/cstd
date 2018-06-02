% 用于外圆的极坐标转化
function polarv = iris_to_polar_coordinate(pic, x0, y0, Rmax, pic_xsize, pic_ysize)

% 定义极坐标图像大小 与 相位间距（分360份）
np = 446;  dp = pi / 180; 

polarv = zeros(np, Rmax);
pic = double(pic);
num_i = 1;
for i = 1 : 2 : 45          % 在0到44°间隔为2°，进行变化    
    for j = 1 : Rmax
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));       
    end
    num_i = num_i + 1;
end 
for i = 46 : 0.5 : 134          % 在0到44°间隔为2°，进行变化    
    for j = 1 : Rmax
%         if num_i == 112 && j == 1
%                num_i  = 112;
%         end
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));
    end
    num_i = num_i + 1;
end
for i = 135 : 2 : 225          % 在0到44°间隔为2°，进行变化    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));        
    end
    num_i = num_i + 1;
end
for i = 226 : 0.5 : 314          % 在0到44°间隔为2°，进行变化    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));       
    end
     num_i = num_i + 1;
end
for i = 315 : 2 : 360          % 在0到44°间隔为2°，进行变化    
    for j = 1 : Rmax
%         if i == 161 && j == 101
%             i = 161;
%         end
        theta = dp * i;
        % 求直角坐标中对应的点
        x = x0 + j*cos(theta);
        y = y0 + j*sin(theta);
        if x < 0
            x = 1;
        else if x > pic_xsize
                x = pic_xsize-1;
            end
        end                
        if y < 0
            y = 1;
        else if y > pic_ysize
                y = pic_ysize-1;
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
        polarv(num_i, j) = round(abs(e * (y - y1) + f * (y2 - y)));        
    end
    num_i = num_i + 1;
end
% 显示极坐标下的图像
% figure; imshow(polarv,[]);  