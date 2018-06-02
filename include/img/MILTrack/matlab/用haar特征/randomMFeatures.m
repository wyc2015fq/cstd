function  MFeatures = randomMFeatures(sample_w,sample_h,M)
%函数功能：从特征池中随机选择M个随机特征(包括位置、尺度和类型)

%要根据不同的样本大小设置最小的特征尺度，也就是说忽略相对较小尺度的特征
temp = max(sample_h,sample_w);
if temp>=100
    min_scale = 10; %特征尺度的最小单位
elseif temp>=70
    min_scale = 7;
elseif temp>=50
    min_scale = 5;
else
    min_scale = 2;
end
temp = min(sample_h,sample_w);
if temp <=30
    min_scale = 2;
elseif temp <=50;
    min_scale = 5;
end
    
for m = 1:M
    temp1 =0;
    temp2 = 0;
    FeatureType    = unidrnd(5,1,1) - 1;
    while(temp1 < 1)
        FeatureStPos.y = unidrnd(sample_h-min_scale-1,1,1);%随机产生正整数1:sample_h-min_scale-1中的一个
        temp1 = sample_h-FeatureStPos.y-min_scale;
    end
    while(temp2 < 1)
        FeatureStPos.x = unidrnd(sample_w-min_scale-1,1,1);
        temp2 = sample_w-FeatureStPos.x-min_scale;
    end
    if(FeatureType == 0)
        FeatureScale.y = unidrnd(sample_h-FeatureStPos.y-min_scale,1,1)+min_scale;%随机产生正整数min_scale+1:sample_h-FeatureStPos.y中的一个
        FeatureScale.x = floor((1/2) * (unidrnd(sample_w-FeatureStPos.x-min_scale,1,1)+min_scale));
    elseif(FeatureType == 1)
        FeatureScale.y = floor((1/2) * (unidrnd(sample_h-FeatureStPos.y-min_scale,1,1)+min_scale));%随机产生正整数min_scale+1:sample_h-FeatureStPos.y中的一个
        FeatureScale.x = unidrnd(sample_w-FeatureStPos.x-min_scale,1,1)+min_scale;
    elseif(FeatureType == 2)
        FeatureScale.y = unidrnd(sample_h-FeatureStPos.y-min_scale,1,1)+min_scale;
        FeatureScale.x = floor((1/3) * (unidrnd(sample_w-FeatureStPos.x-min_scale,1,1)+min_scale));
    elseif(FeatureType == 3)
        FeatureScale.y = floor((1/2) * (unidrnd(sample_h-FeatureStPos.y-min_scale,1,1)+min_scale));
        FeatureScale.x = floor((1/2) * (unidrnd(sample_w-FeatureStPos.x-min_scale,1,1)+min_scale));
    elseif(FeatureType == 4)
        FeatureScale.y = floor((1/3) * (unidrnd(sample_h-FeatureStPos.y-min_scale,1,1)+min_scale));
        FeatureScale.x = unidrnd(sample_w-FeatureStPos.x-min_scale,1,1)+min_scale;

    end
    MFeatures(m,1).StPos = FeatureStPos;
    MFeatures(m,1).Scale = FeatureScale;
    MFeatures(m,1).Type  = FeatureType;
end
