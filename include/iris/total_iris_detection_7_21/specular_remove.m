% specular highlight remove
% 先用 %5的像素thres 二值化图像,并计算二值化区域面积，因为图像高亮点的
% 变化比较小，所以逐步减小thres直到， 相邻面积比小于一定值，判定二值化
% 结束。 然后将所得二值化图像 进行膨胀，得到二值化图
% 然后将高亮点区域用周围像素点加权填充。
function I_inpaint = specular_remove(I)
    [row,col] = size(I);
    % 统计灰度图中5% 的高亮点像素范围
    All_num_pix_I = row * col;
    [n, xout] = imhist(I);
    n_fanzhuan = fliplr(n);  % 将向量 n 翻转  255的像素个数放前面
    n_leijihe = zeros(1,length(xout));  % 累计和，存放与总像素数的比
    for ii = 1:length(xout)
        n_leijihe(ii) = sum(n_fanzhuan(1:ii))/All_num_pix_I;
    end
    ind = find(n_leijihe>0.05,1,'first');
    thres = 256 - ind;
    [aa, area_before] = bw_pic(I, thres);

    for threshold = thres-2 : -2 : 1
        [I_bw_last, area_last] = bw_pic(I, threshold);
%         if area_last/area_before < 1.05
        if (area_last-area_before)/area_before > 0.03 && (area_last-area_before)>10
%         if (area_last-area_before)>10
            I_bw = I_bw_last;
            break;
        end
        area_before = area_last;
    end

%     I_bw1 = bwareaopen(I_bw, 400, 8); % 去除很小的点
    I_bw1 = my_areaopen(I_bw, 400);   % 7_20 去除很小的点
    I_bw = I_bw - I_bw1; % 去除很大的点，为了提高速度
    
%     se = strel('disk', 2);            % 7_20
%     I_bw2 = imdilate(I_bw, se);
    I_bw2 = mydilate(I_bw,'se2');
    % 因为虹膜都在图片中心处，所以把二值化图像周边变为0,方便后续处理
    for ii= 1 : 15 
        for jj = 1:col
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = row - 14 :row
        for jj = 1:col
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = 1 : row
        for jj = 1:15
            I_bw2(ii,jj) = 0;
        end
    end
    for ii = 1 : row
        for jj = col-14 : col
            I_bw2(ii,jj) = 0;
        end
    end

    I_inpaint = I;
    I = double(I);
    for ii = 5 : row - 5
        for jj = 5 : col - 5
            if I_bw2(ii, jj) == 1 % 找到sqecular点
                % 找到此点的四周四个点（left,right,top,down）
                % 找left邻近点
                for iii =  ii : -1 : 1
                    if sum(I_bw2(iii-2:iii-1, jj)) == 0 && I_bw2(iii, jj) == 1
                        top = iii-1;
                        break;
                    end
                end
                for iii = ii : 1 : row
                    if sum(I_bw2(iii+1:iii+2, jj)) == 0 && I_bw2(iii, jj) == 1
                        down = iii+1;
                        break;
                    end
                end
                for jjj = jj : -1 : 1
                    if sum(I_bw2(ii, jjj-2:jjj-1)) == 0 && I_bw2(ii, jjj) == 1
                        left = jjj-1;
                        break;
                    end
                end
                for jjj = jj : 1 : col
                    if sum(I_bw2(ii, jjj+1:jjj+2)) == 0 && I_bw2(ii, jjj) == 1
                        right = jjj+1;
                        break;
                    end
                end
                % 注意此时的I的取值范围，要不会是int8型，很可能出现错误
                I_inpaint(ii,jj) = floor((I(ii,left)*(right-jj) + I(ii,right)*(jj-left))/(2*(right-left))...
                    + (I(top,jj)*(down-ii) + I(down,jj)*(ii-top))/(2*(down-top)));         
            end
        end
    end
    