function [h_candidateWeakClf,MIU1,DELTA1,MIU2,DELTA2] = candidateWeakClf(YX_neg,YX_pos,sample_w,sample_h,MFeatures,I,MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f,v,frame)
%函数功能：返回M个候选弱分类器，以及更新特征值的均值和标准差
%YX_neg和YX_pos存储负样本和正样本坐标(矩形左上角点)
%pic_w和pic_h为帧的宽和高
%MFeatures为随机选择的M个特征，包括位置、类型、尺度
%I为当前输入帧
%MIU1_last_f,DELTA1_last_f,MIU2_last_f,DELTA2_last_f存储服从高斯分布的特征值的均值和标准差
%v为学习率(均值和标准差的更新率)


M = size(MFeatures,1);
tempnum_neg = size(YX_neg,1);
tempnum_pos = size(YX_pos,1);
totalnum = tempnum_neg + tempnum_pos;%总样本数
AllFeatures = zeros(totalnum,M);%存储所有样本的所有特征值
YX_all = [YX_neg;YX_pos];
I = double(imadjust(I));%增加对比度，
for i = 1:totalnum%对于每一个样本
    X1 = YX_all(i,2);
    Y1 = YX_all(i,1);
    X3 = X1 + sample_w - 1;
    Y3 = Y1 + sample_h - 1;
    patch_I = I(Y1:Y3,X1:X3);
    intg_im = ImageToIntg(patch_I,sample_w,sample_h);%求积分图
    for j = 1:M%对于每一个特征
        FeatureStPos = MFeatures(j,1).StPos;
        FeatureScale = MFeatures(j,1).Scale;
        FeatureType  = MFeatures(j,1).Type;
        AllFeatures(i,j) = GetOneFeatureValue_2(FeatureType, FeatureStPos, FeatureScale, intg_im);%计算特征值
    end   
end
   
%以下几行计算特征均值和标准差(每个特征对于所有正样本或所有负样本的均值和标准,要遍历所有特征)
if frame ==1,v=0;end%第一帧时的更新率为0
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



