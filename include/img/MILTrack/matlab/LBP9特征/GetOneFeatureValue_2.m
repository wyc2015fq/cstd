function feature = GetOneFeatureValue_2(FeatureStPos,InterData )
%此函数与用haar特征的MIL的函数GetOneFeatureValue_2不同
%函数的输入参数：InterData的尺度是高度：sample_h+9,宽度：sample_w+9，
%               FeatureStPos为点坐标(此坐标要做为9*9矩阵块的中心坐标)
%输出feature为特征值

yc = FeatureStPos.y;%9*9块的中心坐标
xc = FeatureStPos.x;

col1 = [-3;-3;-3;0;0;0;3;3;3];
col2 = [-3; 0; 3;-3; 0; 3;-3; 0; 3];

YX = [yc - col1,xc - col2];%9*9块中每一个3*3子块的中心坐标
blockValues = zeros(9,1);

LTopY = YX(:,1) - 2;%积分图用到的9*9矩阵块中9个3*3子块的左上角坐标
LTopX = YX(:,2) - 2;

RTopY = YX(:,1) - 2;%积分图用到的9*9矩阵块中9个3*3子块的右上角坐标
RTopX = YX(:,2) + 1;

LBtmY = YX(:,1) + 1;%积分图用到的9*9矩阵块中9个3*3子块的左下角坐标
LBtmX = YX(:,2) - 2;

RBtmY = YX(:,1) + 1;%积分图用到的9*9矩阵块中9个3*3子块的右下角坐标
RBtmX = YX(:,2) + 1;

for i =1:9%计算9个3*3子块对应的各自像素和
    blockValues(i,1) = InterData(RBtmY(i),RBtmX(i)) + InterData(LTopY(i),LTopX(i)) - InterData(RTopY(i),RTopX(i)) - InterData(LBtmY(i),LBtmX(i));      
end

Label = ((blockValues - blockValues(5,1)) >= 0);%外边8个3*3子块各自的blockValues值大于中心3*3的blockValues值则标记为1，否则标记为0

%特征值为2^7 * blockValues(1,1) + 2^6 * blockValues(2,1) + 2^5 * blockValues(3,1) + 2^4 * blockValues(4,1) + 2^3 * blockValues(6,1) + 2^2 * blockValues(7,1) + 2^1 * blockValues(8,1) +  2^0 *blockValues(9,1) ;
feature  = 128 * blockValues(1,1) + 64 * blockValues(2,1) + 32 * blockValues(3,1) + 16 * blockValues(4,1) + ...
           8 * blockValues(6,1) + 4 * blockValues(7,1) + 2 * blockValues(8,1) +  blockValues(9,1) ;
