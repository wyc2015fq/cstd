clc;
%clear all;
fname='D:\pub\bin\iris\test1pic\S2271L04.jpg';
fname='D:\pub\bin\iris\test1pic\ttt.bmp';
fname='D:\pub\bin\iris\test1pic\S2005L05.jpg';

pathstr = 'D:\pub\bin\iris\test1pic\';
pathstr = 'D:\pub\bin\iris\testPic\';

DD = dir([pathstr '*.jpg']); % 已经去除specular highlight的图片
pic_num = length(DD);
results = {pic_num};

pic_num=100;
z_bottom=0;

for i = 1:1:pic_num
    fname0 = DD(i).name;
    if 0
        fname0='S2094R08.jpg';
        fname0='S2320L02.jpg';
        fname0='S2409R04.jpg';
    end
    fname = strcat([pathstr fname0]);
    disp([num2str(i) ' ' fname]);
    Iris = imread(fname);
    ss=size(Iris, 1)/200;
    %Iris = imresize(Iris, [200, 200]);
    ss=1;
    Pic=Iris;
    Pic0=Iris;
    % 输入图像Pic得到对应iris部分的极坐标下的图像pic_normalize（360*30）
    % 对应的pic_normalize的掩膜为mask_normalize，0为无效点，1为有效点
    % 去除reflection
    %Iris = specular_remove(Pic);
    Iris = imresize(Iris, size(Iris)/2);
    Iris = glassRemoval(Iris);

    % 中值滤波
    % Iris1 = medfilt2(Iris,[5,5]);
    %Iris1 = Iris;

    Pic=Iris;
    Iris1 = Iris;

    % 找pupil，pp算法，得到原始图像的虹膜内圆，对应直角坐标下的圆心(nx,ny),半径nr
    [nx, ny, nr] = pp(Iris1);

    nr=nr*2;
    nx=nx*2;
    ny=ny*2;
    if 1
        % 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
        [z_top y_top]= TopEyelidLocation(Iris, nr, nx, ny);

        % 将上眼睑和上眼睑以上的部分检测出来，用白色255表示,可以在外圆检测时去除一大部分误检点
        ROI_rgb = Pic0;
        [ROI_row, ROI_col] = size(ROI_rgb);
        for ii = 1:ROI_col
            if (round(z_top(ii)) <= 0) || (round(z_top(ii)) > ROI_row)
                continue;
            end
            ROI_rgb(1:ceil(z_top(ii)),ii) = 255;
        end

        % pp算法，得到原始图像的虹膜外圆，对应直角坐标下的圆心(wx,wy),半径wr
        [wx, wy, wr] = wai_yuan_detection(nx/2, ny/2, nr/2, Iris1);

        wx=wx*2;
        wy=wy*2;
        wr=wr*2;
        % 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
        % 如果下眼睑在外圆半径外，则不进行检测，因为对后面的iris提取不起作用
        %[z_bottom, y_bottom] = BottomEyelidLocation(Pic0, nr, wr, nx, ny);
    end

    % 画出直角坐标系下 检测好的内外圆以及上下眼睑v_dw,  以及掩膜mask_iris
    [v_dw, mask_iris] = iris_detect_show(Pic0, z_top, z_bottom, nx, ny, nr, wx, wy, wr);
    %[nx, ny, nr, wx, wy, wr]*ss  [y_top y_bottom]
    if 0
        v_nom = Daugmannormalized(nx, ny, nr, wx, wy, wr, Pic0);
        imshow(v_nom);
    end
    if 0
        imshow(v_dw);
    else
        outname = strcat([pathstr 'out_2701\' DD(i).name]);
        imwrite(v_dw, outname);
    end
end

