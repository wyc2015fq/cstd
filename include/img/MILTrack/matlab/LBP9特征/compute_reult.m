function [X1_result,Y1_result] = compute_reult(YX_candidate,sample_w,sample_h,SelectKFeatures,I,SelectMIU1,SelectDELTA1,SelectMIU2,SelectDELTA2)
%此函数与用haar特征的MIL的函数compute_reult有不同之处，不同的地方我用%￥￥￥￥￥￥￥￥￥￥￥标记出
K = size(SelectKFeatures,1);
num_candidate = size(YX_candidate,1);
AllFeatures = zeros(num_candidate,K);
YX_all = YX_candidate;


%以下￥￥￥￥￥￥￥￥￥￥￥
[pic_h,pic_w] = size(I);
ex_I = zeros(pic_h+9,pic_w+9);
ex_I(6:end-4,6:end-4) = imadjust(I);%去掉imadjust效果稍差
intg_im = zeros(pic_h+9,pic_w+9);%积分图，我放到了外面求
intg_im = ImageToIntg(ex_I,pic_w+9,pic_h+9);
patch_intg_im = zeros(sample_h+9,sample_w+9);%子积分图
%以上￥￥￥￥￥￥￥￥￥￥￥

for i = 1:num_candidate
    
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
    
    for j = 1:K
        FeatureStPos = SelectKFeatures(j,1).StPos;
        AllFeatures(i,j) = GetOneFeatureValue_2(FeatureStPos,patch_intg_im);%￥￥￥￥￥￥￥￥￥￥￥，此函数不同
    end  
end

for j = 1:K
    miu1 = SelectMIU1(j);delta1 = SelectDELTA1(j);
    miu2 = SelectMIU2(j);delta2 = SelectDELTA2(j);
    for i = 1:num_candidate
        tempFeature = AllFeatures(i,j);
        P_F_1 = (1/sqrt(2*pi)) * (1/(delta1+0.000000001)) * exp(-(tempFeature-miu1)*(tempFeature-miu1)/(2*delta1*delta1+0.000000001));
        P_F_0 = (1/sqrt(2*pi)) * (1/(delta2+0.000000001)) * exp(-(tempFeature-miu2)*(tempFeature-miu2)/(2*delta2*delta2+0.000000001));
        h_result(i,j) = log((P_F_1+0.000000001)/(P_F_0+0.000000001));
    end
end

H_result = sum(h_result,2);
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
