function [h_candidateWeakClf,MIU1,DELTA1,MIU2,DELTA2] = candidateWeakClf(YX_neg,YX_pos,sample_w,sample_h,MFeatures,I,MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f,v,frame)
%此函数与用haar特征的MIL的函数compute_reult有不同之处，不同的地方我用%￥￥￥￥￥￥￥￥￥￥￥标记出
M = size(MFeatures,1);
tempnum_neg = size(YX_neg,1);
tempnum_pos = size(YX_pos,1);
totalnum = tempnum_neg + tempnum_pos;
AllFeatures = zeros(totalnum,M);%存储所有样本的所有特征值
YX_all = [YX_neg;YX_pos];

%以下￥￥￥￥￥￥￥￥￥￥￥
[pic_h,pic_w] = size(I);
ex_I = zeros(pic_h+9,pic_w+9);
ex_I(6:end-4,6:end-4) = imadjust(I);%去掉imadjust效果稍差
intg_im = zeros(pic_h+9,pic_w+9);%积分图，我放到了外面求
intg_im = ImageToIntg(ex_I,pic_w+9,pic_h+9);
patch_intg_im = zeros(sample_h+9,sample_w+9);%子积分图
%以上￥￥￥￥￥￥￥￥￥￥￥

for i = 1:totalnum
    X1 = YX_all(i,2);
    Y1 = YX_all(i,1);
    X3 = X1 + sample_w - 1;
    Y3 = Y1 + sample_h - 1;
    
    %以下￥￥￥￥￥￥￥￥￥￥￥
    temp_Y1 = Y1+5 -5;%可简化
    temp_Y3 = Y3+5 +4;
    temp_X1 = X1+5 -5;
    temp_X3 = X3+5 +4;
    patch_intg_im = intg_im(temp_Y1:temp_Y3,temp_X1:temp_X3);
    %以上￥￥￥￥￥￥￥￥￥￥￥
    
    for j = 1:M
        FeatureStPos = MFeatures(j,1).StPos;
        AllFeatures(i,j) = GetOneFeatureValue_2(FeatureStPos, patch_intg_im);%￥￥￥￥￥￥￥￥￥￥￥，此函数不同
    end   
end
   
if frame ==1,v=0;end
MIU2   = v*MIU2_last_f   + (1-v)*sum(AllFeatures(1:tempnum_neg,:),1)/tempnum_neg;%对于负样本
temp = repmat(MIU2,tempnum_neg,1);
DELTA2 = v*DELTA2_last_f + (1-v)*sqrt(sum((AllFeatures(1:tempnum_neg,:)-temp).^2,1)/tempnum_neg);
MIU1   = v*MIU1_last_f   + (1-v)*sum(AllFeatures(1+tempnum_neg:end,:),1)/tempnum_pos;%对于正样本
temp = repmat(MIU1,tempnum_pos,1);
DELTA1 = v*DELTA1_last_f + (1-v)*sqrt(sum((AllFeatures(1+tempnum_neg:end,:)-temp).^2,1)/tempnum_pos);

h_candidateWeakClf = zeros(totalnum,M);%存储所有样本的候选弱分类器
for j = 1:M
    miu1 = MIU1(j);delta1 = DELTA1(j);
    miu2 = MIU2(j);delta2 = DELTA2(j);
    for i = 1:totalnum
        tempFeature = AllFeatures(i,j);
        P_F_1 = (1/sqrt(2*pi)) * (1/(delta1+0.000000001)) * exp(-(tempFeature-miu1)*(tempFeature-miu1)/(2*delta1*delta1+0.000000001));
        P_F_0 = (1/sqrt(2*pi)) * (1/(delta2+0.000000001)) * exp(-(tempFeature-miu2)*(tempFeature-miu2)/(2*delta2*delta2+0.000000001));
        h_candidateWeakClf(i,j) = log((P_F_1+0.000000001)/(P_F_0+0.000000001));
    end
end



