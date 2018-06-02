function [YX_neg,YX_pos] = randomSelectSample(X,Y,sample_w,sample_h,pic_w,pic_h,beita1,beita2,gama,tempnum_neg,tempnum_pos,frame)


%***********随机选择负样本***********%
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
        if  dis <= beita2 * beita2 && dis >= beita1 * beita1 && tempY <= pic_h && tempX <= pic_w
            YX_neg_all = [YX_neg_all;[i j]];
        end
    end
end
if length(YX_neg_all)==0,disp('跟踪区域尺度过大,导致不能产生负样本，跟踪暂停,请缩小跟踪区域尺度');pause;end

neg_ind = unidrnd(size(YX_neg_all,1),tempnum_neg,1);
for j = 1:size(neg_ind,1)
    YX_neg(j,1:2) = YX_neg_all(neg_ind(j),:);
end

%***********随机选择正样本***********%
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
            if  dis <= gama * gama && tempY <= pic_h && tempX <= pic_w
                YX_pos_all = [YX_pos_all;[i j]];
            end
        end
    end
end
if length(YX_pos_all)==0,disp('跟踪区域尺度过大,导致不能产生正样本，跟踪暂停,请缩小跟踪区域尺度');pause;end

if frame==1
    pos_ind = 1;
else
    pos_ind = unidrnd(size(YX_pos_all,1),tempnum_pos,1);
end
YX_pos(1,1:2) = [Y X];
if size(pos_ind,1)>=2
    for j = 2:size(pos_ind,1)
        YX_pos(j,1:2) = YX_pos_all(pos_ind(j),:);
    end
end


