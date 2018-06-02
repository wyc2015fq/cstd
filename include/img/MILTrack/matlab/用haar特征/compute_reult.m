function [X1_result,Y1_result] = compute_reult(YX_candidate,sample_w,sample_h,SelectKFeatures,I,SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2)
%函数功能：返回最佳位置(矩形左上角点坐标)
%YX_candidate为候选(矩形)位置的左上角点坐标
%sample_w和sample_h为(矩形)样本宽和高
%SelectKFeatures为选择出来的K个特征(包括位置、尺度、类型)
%SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2为选择出来的K个特征对应的特征均值和标准差


K = size(SelectKFeatures,1);
num_candidate = size(YX_candidate,1);
I = double(imadjust(I));%增加对比度
AllFeatures = zeros(num_candidate,K);
YX_all = YX_candidate;
for i = 1:num_candidate
    X1 = YX_all(i,2);
    Y1 = YX_all(i,1);
    X3 = X1 + sample_w - 1;
    Y3 = Y1 + sample_h - 1;
    patch_I = I(Y1:Y3,X1:X3);
    intg_im = ImageToIntg(patch_I,sample_w,sample_h);%求积分图
    for j = 1:K
        FeatureStPos = SelectKFeatures(j,1).StPos;
        FeatureScale = SelectKFeatures(j,1).Scale;
        FeatureType  = SelectKFeatures(j,1).Type;
        AllFeatures(i,j) = GetOneFeatureValue_2(FeatureType, FeatureStPos, FeatureScale, intg_im);
    end   
end


for j = 1:K%对于K个特征的每一个
    miu1 = SelectMIU1(j);delta1 = SelectDELTA1(j);
    miu2 = SelectMIU2(j);delta2 = SelectDELTA2(j);
    for i = 1:num_candidate%对于每一个候选位置
        tempFeature = AllFeatures(i,j);
        P_F_1 = (1/sqrt(2*pi)) * (1/(delta1+0.000000001)) * exp(-(tempFeature-miu1)*(tempFeature-miu1)/(2*delta1*delta1+0.000000001));
        P_F_0 = (1/sqrt(2*pi)) * (1/(delta2+0.000000001)) * exp(-(tempFeature-miu2)*(tempFeature-miu2)/(2*delta2*delta2+0.000000001));
        h_result(i,j) = log((P_F_1+0.000000001)/(P_F_0+0.000000001));%弱分类器输出
    end
end

H_result = sum(h_result,2);%强分类器输出
%P_y_x = 1/(1+exp(-H_result));
%[value,max_ind] = max(P_y_x);
[value,max_ind] = max(H_result);%若用上两式则可能-H_resul中的所有元素太大使P_y_x中的所有元素均为0

if length(max_ind) >=2
    ind = max_ind(1);
else
    ind = max_ind;
end
X1_result = YX_candidate(ind,2);
Y1_result = YX_candidate(ind,1); 