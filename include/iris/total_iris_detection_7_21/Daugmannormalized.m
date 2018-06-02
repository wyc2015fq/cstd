function v_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, I)
% nx, ny, nr 为对应的内圆的直角坐标以及其半径大小
% wx, wy, wr 为对应的外圆的直角坐标以及其半径大小
% v_normalize 为对应的内圆与外圆边缘的中间部分的极坐标图像，其中心点为？？

[pic_ysize, pic_xsize] = size(I);
np = 360;
R = 30;     % R 代表把 虹膜分成多少份  4-19
%v_normalize = zeros(R, np);
%求瞳孔和虹膜边缘点
xi = zeros(1, np);
yi = zeros(1, np);
xo = zeros(1, np);
yo = zeros(1, np);
%第一种情况：ny = wy
if (ny == wy) && (nx == wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + wr * cos(i_pi));
        yo(i+1) = round(wy + wr * sin(i_pi));
    end
end
if (ny == wy) && (nx < wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a_OIA = pi / 2 + i_pi;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        a_OAI = asin(IO * sin(a_OIA) / wr);
        a_IOA = pi - a_OIA - a_OAI;
        IA = sqrt(wr^2 + IO^2 - 2 * wr * IO * cos(a_IOA));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
if (ny == wy) && (nx > wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a_OIA = pi / 2 - i_pi;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        a_OAI = asin(IO * sin(a_OIA) / wr);
        a_IOA = pi - a_OIA - a_OAI;
        IA = sqrt(wr^2 + IO^2 - 2 * wr * IO * cos(a_IOA));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
%第二种情况：(ny > wy) && (nx >= wx)
if (ny > wy) && (nx >= wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a = atan(abs((nx - wx) / (ny - wy)));
        a_OIA = pi - a - i_pi;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        a_OAI = asin(IO * sin(a_OIA) / wr);
        a_IOA = pi - a_OIA - a_OAI;
        IA = sqrt(wr^2 + IO^2 - 2 * wr * IO * cos(a_IOA));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
%第三种情况：(ny > wy) && (nx <= wx)
if (ny > wy) && (nx <= wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a = atan(abs((nx - wx) / (ny - wy)));
        a_OIA = pi - i_pi + a;
        a_OAI = asin(nr * sin(a_OIA) / wr);
        a_IOA = pi - a_OIA - a_OAI;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        IA = sqrt(IO^2 + wr^2 - 2 * wr * IO * cos(a_IOA));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
%第四种情况：(ny < wy) && (nx >= wx)
if (ny < wy) && (nx >= wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a = atan(abs((nx - wx) / (ny - wy)));
        a_AIO = i_pi - a;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        a_IAO = asin(IO * sin(a_AIO) / wr);
        a_AOI = pi - a_AIO - a_IAO;
        IA = sqrt(IO^2 + wr^2 - 2 * IO * wr * cos(a_AOI));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
%第五种情况：(ny < wy) && (nx <= wx)
if (ny < wy) && (nx <= wx)
    for i = 0 : np - 1
        i_pi = i * pi / 180;
        a = atan(abs((nx - wx) / (ny - wy)));
        a_AIO = a + i_pi;
        IO = sqrt((nx - wx)^2 + (ny - wy)^2);
        a_IAO = asin(IO * sin(a_AIO) / wr);
        a_IOA = pi - a_AIO - a_IAO;
        IA = sqrt(IO^2 + wr^2 - 2 * wr * IO * cos(a_IOA));
        xi(i+1) = round(nx + nr * cos(i_pi));
        yi(i+1) = round(ny + nr * sin(i_pi));
        xo(i+1) = round(wx + IA * cos(i_pi));
        yo(i+1) = round(wy + IA * sin(i_pi));
    end
end
% vv = I;
% figure, imshow(vv);
% for i = 1 : np
%     vv(xi(i), yi(i)) = 1;  % xi,yi 为内圆边缘点
%     vv(xo(i), yo(i)) = 1;  % xo,yo 为外圆边缘点
% end
% figure, imshow(vv);
%  R = 100;
for i = 0 : R - 1
    r = i / (R - 1);
    for j = 0 : np - 1
        %求直角坐标中对应的点
        x = (1 - r) * xi(j+1) + r * xo(j+1);
        y = (1 - r) * yi(j+1) + r * yo(j+1);
        %在直角坐标中的周围四点
        x1 = floor(x);
        y1 = floor(y);
        x2 = x1 + 1;
        y2 = y1 + 1;
        %处理越界情况
        if x1 < 1
            x1 = 1;
        end
        if x2 < 2
            x2 = 2;
        end
        if y1 < 1
            y1 = 1;
        end
        if y2 < 2
            y2 = 2;
        end
        if x1 > pic_xsize - 1
            x1 = pic_xsize - 1;
        end
        if x2 > pic_xsize
            x2 = pic_xsize;
        end
        if y1 > pic_ysize - 1
            y1 = pic_ysize - 1;
        end
        if y2 > pic_ysize
            y2 = pic_ysize;
        end
        %在X和Y方向分别进行线性插值
        e = I(x1, y1) * (x - x1) + I(x2, y1) * (x2 - x);
        f = I(x1, y2) * (x - x1) + I(x2, y2) * (x2 - x);
        v_normalize(i+1, j+1) = e * (y - y1) + f * (y2 - y);
    end
end
%显示极坐标下的图像
% v_normalize = mat2gray(v_normalize);
% figure;imshow(v_normalize);
return
