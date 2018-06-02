function [v_dw, mask_iris] = iris_detect_show(v_dw, z_top, z_bottom, nx, ny, nr, wx, wy, wr)
% 显示最终的检查结果
[ROI_row, ROI_col] = size(v_dw);
mask_iris = true(ROI_row, ROI_col);    % 直角坐标系下的掩膜 0表示无效点，1为有效点

for ii = 1:ROI_col
    if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
        continue;
    end
    v_dw(ceil(z_top(ii)),ii) = 255;
    mask_iris(1:ceil(z_top(ii)),ii) = 0;
end

for ii = 1:length(z_bottom)
    if round(z_bottom(ii)) <= 0 || (round(z_bottom(ii)) > ROI_row)
        continue;
    end
    v_dw(ceil(z_bottom(ii)),ii) = 255;
    mask_iris(ceil(z_bottom(ii)):end,ii) = 0;
end

for ii = 1:ROI_row
    for jj = 1:ROI_col
        if (ii-nx)*(ii-nx) + (jj-ny)*(jj-ny) <= nr*nr
            mask_iris(ii,jj) = 0;
        elseif (ii-wx)*(ii-wx) + (jj-wy)*(jj-wy) >= wr*wr
            mask_iris(ii,jj) = 0;
        end
    end
end

v_dw = DrawCircle(v_dw, [nx, ny, nr]);
v_dw = DrawCircle(v_dw, [wx, wy, wr]);

%v_dw = draw_yuan(nx, ny, nr, v_dw);
%v_dw = draw_yuan(wx, wy, wr, v_dw);
