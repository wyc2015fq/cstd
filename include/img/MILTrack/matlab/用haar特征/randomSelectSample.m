function [YX_neg,YX_pos] = randomSelectSample(X,Y,sample_w,sample_h,pic_w,pic_h,beita1,beita2,gama,tempnum_neg,tempnum_pos,frame)
%函数功能：在一定范围内随机选择正样本和负样本，函数返回负样本和正样本的左上角点坐标
%(X,Y)为当前位置矩形框左上角点坐标
%sample_w和sample_h为(矩形)样本宽和高
%pic_w和pic_h为帧的宽和高
%[beita1，beita2]为到负样本左上角点坐标(X,Y)的距离
%[0，gama]为到正样本左上角点坐标(X,Y)的距离
%tempnum_neg和tempnum_pos为负样本和正样本数

%***********在距离坐标(X,Y)半径为[beita1，beita2]圆环范围内随机选择负样本(指的是样本矩形的左上角点)***********%
domainX1 = X - beita2;
domainY1 = Y - beita2;
domainX3 = X + beita2;
domainY3 = Y + beita2;

if domainX1 < 1,     domainX1 =     1;end
if domainY1 < 1,     domainY1 =     1;end
if domainX3 > pic_w, domainX3 = pic_w;end
if domainY3 > pic_h, domainY3 = pic_h;end

YX_neg_all = [];
for i = domainY1:domainY3
    for j = domainX1:domainX3
        dis = (Y - i)*(Y - i) + (X - j)*(X - j);
        tempY = i + sample_h -1;
        tempX = j + sample_w -1;
        if  dis <= beita2 * beita2 && dis >= beita1 * beita1 && tempY <= pic_h && tempX <= pic_w%要防止超出图像范围
            YX_neg_all = [YX_neg_all;[i j]];
        end
    end
end

neg_ind = unidrnd(size(YX_neg_all,1),tempnum_neg,1);%产生tempnum_neg个随机数，随机数的取值范围为1:size(YX_neg_all,1)
for j = 1:size(neg_ind,1)
    YX_neg(j,1:2) = YX_neg_all(neg_ind(j),:);
end

%***********在距离坐标(X,Y)半径为[0，gama]圆环范围内随机选择选择正样本***********%
if frame == 1
    YX_pos_all = [Y X];
else
    domainX1 = X - gama;
    domainY1 = Y - gama;
    domainX3 = X + gama;
    domainY3 = Y + gama;

    if domainX1 < 1,     domainX1 =     1;end
    if domainY1 < 1,     domainY1 =     1;end
    if domainX3 > pic_w, domainX3 = pic_w;end
    if domainY3 > pic_h, domainY3 = pic_h;end

    YX_pos_all = [];
    for i = domainY1:domainY3
        for j = domainX1:domainX3
            dis = (Y - i)*(Y - i) + (X - j)*(X - j);
            tempY = i + sample_h -1;
            tempX = j + sample_w -1;
            if  dis <= gama * gama && tempY <= pic_h && tempX <= pic_w%要防止超出图像范围
                YX_pos_all = [YX_pos_all;[i j]];
            end
        end
    end
end

if frame==1
    pos_ind = 1;
else
    pos_ind = unidrnd(size(YX_pos_all,1),tempnum_pos,1);%产生tempnum_pos个随机数，随机数的取值范围为1:size(YX_pos_all,1)
end
YX_pos(1,1:2) = [Y X];
if size(pos_ind,1)>=2
    for j = 2:size(pos_ind,1)
        YX_pos(j,1:2) = YX_pos_all(pos_ind(j),:);
    end
end


