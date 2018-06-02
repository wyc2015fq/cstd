% 输入图像Pic得到对应iris部分的极坐标下的图像pic_normalize（360*30）
% 对应的pic_normalize的掩膜为mask_normalize，0为无效点，1为有效点
function [pic_normalize, mask_normalize] = total_testings_one_pic(Pic)

% 去除reflection
Iris = specular_remove(Pic);
% 中值滤波
Iris1 = medfilt2(Iris,[5,5]);

% 找pupil，pp算法，得到原始图像的虹膜内圆，对应直角坐标下的圆心(nx,ny),半径nr
[nx, ny, nr] = pp(Iris1);

% 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
z_top = TopEyelidLocation(Iris, nr, nx, ny);

% 将上眼睑和上眼睑以上的部分检测出来，用白色255表示,可以在外圆检测时去除一大部分误检点
ROI_rgb = Iris;
[ROI_row, ROI_col] = size(ROI_rgb);
for ii = 1:ROI_col
    if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
        continue;
    end
    ROI_rgb(1:ceil(z_top(ii)),ii) = 255;
end

% pp算法，得到原始图像的虹膜外圆，对应直角坐标下的圆心(wx,wy),半径wr
[wx, wy, wr] = wai_yuan_detection(nx, ny, nr, ROI_rgb);

% 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
% 如果下眼睑在外圆半径外，则不进行检测，因为对后面的iris提取不起作用
z_bottom = BottomEyelidLocation(Iris, nr, wr, nx, ny);

% 画出直角坐标系下 检测好的内外圆以及上下眼睑v_dw,  以及掩膜mask_iris
[v_dw, mask_iris] = iris_detect_show(Pic, z_top, z_bottom, nx, ny, nr, wx, wy, wr);

% 将 iris部分表示出来  归一化 Daugman的方法
pic_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, Pic);
mask_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, mask_iris);

