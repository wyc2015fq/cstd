  %%%%%%%%%%%%%%%%%%%%%%%虹膜外圆定位%%%%%%%%%%%%%%%%%%%%%%%%%
function [iris_x, iris_y, iris_radius] = wai_yuan_detection...
    (pupil_x, pupil_y, pupil_radius, I_source)
    
% 极坐标原点，图像的中心点
x0 = pupil_x;
y0 = pupil_y; 
[pic_xsize, pic_ysize] = size(I_source);
Rmax = min(max(round(pic_xsize - x0), round(pic_ysize - y0)), max(x0, y0)) + 10;

% 专门的外圆极坐标变换
polarv = iris_to_polar_coordinate(I_source, x0, y0, Rmax, pic_xsize, pic_ysize);

polarv = uint8(polarv);

% 转化为边缘图像 
[p_row_sample, p_col_sample, N] = edge_detection_waiyuan(polarv, pupil_radius);

% % %%%%%%%%%%%%%%%%%胡克定律%%%%%%%%%%%%%%%%%%%%%%
[iris_radius, iris_x, iris_y] = Hooke_law_total(x0, y0, p_col_sample, p_row_sample, N, 100, 0.5, 'WaiYuan');
% [iris_radius, iris_x, iris_y] = Hooke_law_wai_yuan(x0, y0, p_col_sample, p_row_sample, N, 100, 0.5);

