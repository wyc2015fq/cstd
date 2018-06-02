function  [MFeatures,NineBlocks_MFeatures] = randomMFeatures(sample_w,sample_h,M,K)
%此函数与用haar特征的MIL的函数randomMFeatures不同
%函数返回MFeatures为所有候选特征的中心坐标
%NineBlocks_MFeatures存储每个子图像中的特征中心索引
Label     = zeros(sample_h,sample_w);%已选位置的标记
for m = 1:M
    
    FeatureStPos.y = unidrnd(sample_h,1,1)+5;%产生1个随机数，范围在6:sample_h+5
    FeatureStPos.x = unidrnd(sample_w,1,1)+5;
    while((Label(FeatureStPos.y-5,FeatureStPos.x-5)) ~=0 )%防止重复选择
        FeatureStPos.y = unidrnd(sample_h,1,1)+5;
        FeatureStPos.x = unidrnd(sample_w,1,1)+5;
    end
    MFeatures(m,1).StPos = FeatureStPos;
    Label(FeatureStPos.y-5,FeatureStPos.x-5) = m;%用m标记已选特征的中心位置
end

%要将尺度为sample_h和sample_w的样本分为9块.注意以下的9和3与此算法中用到的特征中的9和3不是一回事,这里是为了防止大多数特征的中心都集
%中在局部(如图像左上角)，这样会很容易导致偏跟或者失跟踪，所以将图像分为9个子图像，使每个子图像内都有一些特征的中心
sub_h = floor(sample_h/3);%子图像尺度
sub_w = floor(sample_w/3);

block_num = 1;%子图像索引
NineBlocks_MFeatures ={};%存储每个子图像中的特征中心，是9*1的ceil矩阵
for i = 1:3
    for j = 1:3
        temp_y1 = i*sub_h - (sub_h - 1);%(temp_y1,temp_x1)为子图像左上角点坐标，(temp_y3,temp_x3)为子图像右下角点坐标
        temp_x1 = j*sub_w - (sub_w - 1);
        
        if i<=2
            temp_y3 = i*sub_h;
        else
            temp_y3 = sample_h;
        end
        
        if j<=2
            temp_x3 = j*sub_w;
        else
            temp_x3 = sample_w;
        end

        sub_Label = Label(temp_y1:temp_y3,temp_x1:temp_x3);%sub_Label为子图像中的已选特征中心的标记
        ind = find(sub_Label~=0);
        temp = (sub_Label(ind))';
        NineBlocks_MFeatures{block_num,1} = temp;  
        block_num = block_num + 1;
    end
end





