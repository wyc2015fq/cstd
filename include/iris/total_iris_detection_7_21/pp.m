%%%%%%%%%%%%%%%%%%%%%%%%虹膜内圆定位%%%%%%%%%%%%%%%%%%%%%%%%%    
function [pupil_x, pupil_y, pupil_radius] = pp(img_gray)
   
    % 图像读取
    I_source = im2double(img_gray);    
    
    % 极坐标原点，图像的中心点
    [pic_xsize, pic_ysize] = size(I_source);
    [x0, y0] = rough_center_7_20(I_source);      % 得到pupil的大致中心点
    
    % 半径最大值
    Rmax_p = min(round(pic_xsize - x0), round(pic_ysize - y0)) - 1;    
    
    % 4_8 转化为极坐标下的图像
    polarv = pupil_to_polar_coordinate(I_source, x0, y0, Rmax_p);
    % figure,imshow(polarv)
    
    % 4_8 边缘检测， 每行最多对应一个边缘点, 并且最后得到内边缘点及其在极坐标下的位置
    [p_row, p_col, N] = edge_detection(polarv, Rmax_p); 

    % 胡克定律
    [pupil_radius, pupil_x, pupil_y] = Hooke_law_total(x0, y0, p_col, p_row, N, 100, 0.0051, 'NeiYuan');
%     [pupil_radius, pupil_x, pupil_y] = Hooke_law(x0, y0, p_col, p_row, N, 100, 0.0051);

end
