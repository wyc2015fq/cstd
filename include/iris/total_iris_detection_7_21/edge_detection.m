function [p_row, p_col, Number_sample_edge] = edge_detection(polarv, Rmax)
% p_col 为最后在内圆上取点的极坐标列坐标
% p_row 为最后在内圆上取点的极坐标行坐标
% Number_sample_edge 为最后在内圆上取点的个数
% polarv 为极坐标图像, Rmax为极坐标半径最大值

% 边缘检测及显示
polarv = im2bw(polarv,0.1161);
% polarv1 = bwareaopen(polarv, 400, 4);    % 7_20
polarv = my_areaopen(polarv, 400);
% I_edge = edge(polarv,'canny','vertical');   % 7_20

scaling = 1.0;    sigma = 1;    vert = 1.0;    horz = 0.0;
% generate the edge image
I_edge = canny(polarv, sigma, scaling, vert, horz, 1.9, 1.5);    % edge detection 4-19
I_edge = im2bw(I_edge,0.3);

%%%%%%%%%%%%%%%%求瞳孔边缘点%%%%%%%%%%%%%%%%%%%%
edge_number = 0;          % 4_11 edge_number 为取的边缘点个数， 也就是取的角度个数
for i = 1 : 135         % 4_11 忽略行的上下一部分边缘 每行也就是每个角度上只取一个边缘点
    for j = 1 : Rmax
        if(I_edge(i,j) == 1)
            edge_number = edge_number + 1;
            edge_location_row(edge_number) = i;  % 4_8 行所在极坐标上的位置
            edge_location_col(edge_number) = j;  % 4_8 列所在极坐标上的位置
            break;
        end
    end
end
for i = 225 : 360  % 4_11 忽略行的上下一部分边缘 每行也就是每个角度上只取一个边缘点
    for j = 1 : Rmax
        if(I_edge(i,j) == 1)
            edge_number = edge_number + 1;
            edge_location_row(edge_number) = i;  % 4_8 行所在极坐标上的位置
            edge_location_col(edge_number) = j;  % 4_8 列所在极坐标上的位置
            break;
        end
    end
end

% 计算断点
duan_dian = [];  % 存放连续线段的坐标点 以行为标准 第二步得到的p_row_y2上下两点距离大于2则为一个断点
line_length = [];  % 存放当前线段的长度
point = 1;
duan_dian(point) = 1;
for i = 2 : edge_number
    if abs((edge_location_col(i) - edge_location_col(i-1))) > 1
        point = point + 1;
        duan_dian(point) = i;
        line_length(point -1) = i - duan_dian(point-1);
    end
end
duan_dian(point+1) = edge_number + 1;
line_length(point) = edge_number + 1 - duan_dian(point);
N = 0;  % 4-13 选中的边缘点
for i = 1 : length(line_length)
    if line_length(i) > 25     % 8 为基本长度，小于8长度的线段进行抛弃
        for j = duan_dian(i) : duan_dian(i+1)-1
            N = N + 1;
            Final_good_edge_row(N) = edge_location_row(j);
            Final_good_edge_col(N) = edge_location_col(j);
        end
    end
end

% 取内边缘点
Number_sample_edge = 0;
canshu2=5;
if N < canshu2
    i_cs = 1;
else
    i_cs = canshu2;
end
for i = i_cs : canshu2 : N         % 20 是可调的  主要用于改变sample点的多少
    Number_sample_edge = Number_sample_edge + 1;
    p_col(Number_sample_edge) = Final_good_edge_col(i);
    p_row(Number_sample_edge) = Final_good_edge_row(i);
end


