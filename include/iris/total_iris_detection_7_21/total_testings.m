%function results = total_testings

pathstr = 'D:\pub\bin\iris\test1pic\';

DD = dir([pathstr '*.jpg']); % 已经去除specular highlight的图片
pic_num = length(DD);
results = {pic_num};

for i = 1:1:pic_num
    fname = strcat([pathstr DD(i).name]);
    Pic = imread(fname);

    % 去除reflection
    Iris = specular_remove(Pic);
    Iris1 = medfilt2(Iris,[5,5]);

    % 找pupil，pp算法，得到原始图像的虹膜内圆
    [nx, ny, nr] = pp(Iris1);

    % TopEyelidLocation_5_27 应该得到上眼睑的曲线
    z_top = TopEyelidLocation(Iris, nr, nx, ny);

    % 将上眼睑和上眼睑以上的部分检测出来，用白色255表示
    ROI_rgb = Iris;
    [ROI_row, ROI_col] = size(ROI_rgb);
    for ii = 1:ROI_col
        if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
            continue;
        end
        ROI_rgb(1:ceil(z_top(ii)),ii,:) = 255;
    end

    % pp算法，得到原始图像的虹膜外圆
    [wx, wy, wr] = wai_yuan_detection(nx, ny, nr, ROI_rgb);

    z_bottom = BottomEyelidLocation(Iris, nr, wr, nx, ny);

    % 画出内外圆,  以及掩膜
    [v_dw, mask_iris] = iris_detect_show(Pic, z_top, z_bottom, nx, ny, nr, wx, wy, wr);

    % 将 iris部分表示出来  归一化
    pic_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, Pic);
    mask_normalize = Daugmannormalized(nx, ny, nr, wx, wy, wr, mask_iris);
    %     figure,imshow(pic_normalize); figure,imshow(mask_iris);

    fresult = strcat(pwd, '\V2_d1_mask\', DD(i).name);
    imwrite(mask_normalize, fresult);
    fresult = strcat(pwd, '\V2_d1_norm\', DD(i).name);
    imwrite(pic_normalize, fresult);
end
