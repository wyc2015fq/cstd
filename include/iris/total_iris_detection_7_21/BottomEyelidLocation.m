function  [z, y] = BottomEyelidLocation(img, nr, wr, polar_nx, polar_ny)

load BottomEyelidTrainreferPoint_100

length_point = 0;           % 选取的在iris 内部的点的个数
num_point = 0;              % 判定是否有遮挡的计数

BottomEyelidROI=img;
BottomEyelidROI_rgb(:,:,1)=img;
BottomEyelidROI_rgb(:,:,2)=img;
BottomEyelidROI_rgb(:,:,3)=img;

[ROI_row ROI_col]=size(BottomEyelidROI);

row_a = [];
col_a = [];
I5 = horizontal_filter(img,'bottom');
% I4为double  I5为logical型

% 6_1 进行判定，对I4图像的pupil外iris内内部进行判断是否与下眼睑有遮挡
% 注意： 有遮挡时必定有很强的canny曲线检测到，也就是可以对I4进行(nr+10,wr+2)范围内进行I4==1统计计数
for jj = 5 : ROI_col-5
    for kk = round(2*ROI_row/3) : ROI_row
        diffdist=sqrt((kk - polar_nx)^2+(jj-polar_ny)^2);
        if (diffdist> nr + 10) && (I5(kk,jj) == 1) && (diffdist < wr + 2)
            num_point = num_point + 1;
        end
    end
end

if num_point <= 70              % 这里有个重要的参数设定啊
    z = 1;
    a = [1;1];
else
    % 每行选出一个边缘点   没有的边缘点 all_edgepoint为0
    for jj = 5 : ROI_col-5
        for kk = round(2*ROI_row/3) : ROI_row
            diffdist=sqrt((kk - polar_nx)^2+(jj-polar_ny)^2);
            if (diffdist> nr + 10) && (I5(kk,jj) == 1) && (diffdist < wr + 2)
                % diffabs(1,jj) = kk;
                % all_edgepoint(1,jj) = kk;
                row_a = [row_a,jj];
                col_a = [col_a,kk];
                BottomEyelidROI_rgb(kk,jj,:)=[0,255,0];
                length_point = length_point + 1;
                break;
            end
        end
    end

    a = [row_a;col_a];

    % y = polyfit(a(1, :), a(2, :), 2);
    y = my_polyfit(a(1, :), a(2, :), 2);
    canshu123(1, :) = y;
    x = 1 : 1 : ROI_col;
    z = canshu123(1, 1) * (x .* x) + canshu123(1, 2) * x + canshu123(1, 3);
    for xxx = 1 : 1 : ROI_col
        if(z(1, xxx) > ROI_row)
            z(1, xxx) = ROI_row;
        end
    end
end

