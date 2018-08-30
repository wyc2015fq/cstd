clc ; clear; close all;
max_stage = 5;%迭代最大阶段数
NumOfTree=1;%森林树棵数
max_depth=5;%树的深度
TrDataAugment=1;%训练数据扩大，防止树过拟合及获得更好泛化能力
num_features=500;%特征数
num_points = 68;%特征点数
Radius = [0.4 0.3 0.2 0.15 0.1];%设定半径
dbnames={'AFW','LFPW','Helen'};%选择需要训练的数据库
work_path=pwd;

disp('step1:读pts及图片');
%[imgTr,realshapes,imgpathlists]=resizeimg(dbnames,'train');
[imgTr,realshapes,imgpathlists]=readimg(dbnames,'train',num_points);%imgTr：训练样本；realshapes：真实的人脸形状；imgpathlists：存放路径
num_ptsfiles=size(realshapes,1);%pts个数
num_samples=size(imgTr,2); %训练样本个数

disp('step2:求平均形状');
meanshape=sum(realshapes)./num_ptsfiles;%平均形状

shape_ref =repmat(realshapes,TrDataAugment,1);%扩大真实的样本形状
num_samples_Aug=num_samples*TrDataAugment;%扩大后的样本数
clear realshapes;%清除变量

%求出每个样本瞳孔间距(即inter-pupil dist)
for i_sample=1:num_samples
    shape_gt=shape_ref(i_sample,:);%当前样本i的形状
    shape_gt=reshape(shape_gt,num_points,2);%转成68行2列
    dist_pupils(i_sample,1)=norm((mean(shape_gt(37:42, :)) - mean(shape_gt(43:48, :))));%样本i瞳孔间距
end
dist_pupils=repmat(dist_pupils,TrDataAugment,1);

disp('step3:确立初始形状');
%{
%V_J检测
for i_sample = 1:num_samples
[x1_f,y1_f,x3_f,y3_f] = Viola_Jones_face_detect(work_path,imgTr{i_sample},imgpathlists{i_sample}); %VJ检测人脸位置
if isempty(x1_f),fprintf('%d\n',i_sample);continue;end
detec_bbox(i_sample,:)=[x1_f,x3_f,y1_f,y3_f];
end
%}
load train_detec_bbox.mat %这个保存的是V_J检测三个库的人脸框，若只训练一个库，则用上面的V_J函数。
if TrDataAugment>1,detec_bbox =repmat(detec_bbox,TrDataAugment,1);end %检测框扩大
shape_curstage=randomshape(shape_ref,detec_bbox,num_samples);%初始形状

for i_stage = 1:max_stage
    disp('step4:确定形状偏差');
    %DeltaS:形状偏差；Deltas：归一化的形状偏差；Delta：回归的形状增量
    DeltaS=shape_ref-shape_curstage;%形状偏差：真实形状-当前形状
    width=max(shape_curstage(:,1:num_points)')-min(shape_curstage(:,1:num_points)');%人脸宽
    height=max(shape_curstage(:,num_points+1:end)')-min(shape_curstage(:,num_points+1:end)');%人脸高
    Deltas=[DeltaS(:,1:num_points)./repmat(width',1,num_points),DeltaS(:,num_points+1:end)./repmat(height',1,num_points)];%按相应人脸尺寸，形状偏差作归一化
    %Deltas=[bsxfun(@rdivide,DeltaS(:,1:num_points)',width);bsxfun(@rdivide,DeltaS(:,num_points+1:end)',height)]';%形状偏差作归一化

    radius=Radius(i_stage);%当前阶段半径
    disp('step5:建立森林，求回归矩阵，求形状增量');

    %样本森林模型
    %Delta:形状增量;W:回归矩阵;
    [Delta,W,Forest_Model]=Get_Forest_Model(radius,imgTr,shape_curstage,num_points,num_samples_Aug,NumOfTree,max_depth,Deltas,num_samples,num_features);
    forestmodel{i_stage}=Forest_Model;
    Wt{i_stage}=W;
    Delta=[Delta(:,1:num_points).*repmat(width',1,num_points),Delta(:,num_points+1:end).*repmat(height',1,num_points)];%按相应人脸尺寸，形状偏差还原
    %Delta=[bsxfun(@times,Delta(:,1:num_points)',width);bsxfun(@times,Delta(:,num_points+1:end)',height)]';
    shape_curstage=shape_curstage+Delta;

    %计算误差
    deltashapes=DeltaS-Delta;%回归后的形状偏差
    deltashapes_x=deltashapes(:,1:num_points);%x方向偏差
    deltashapes_y=deltashapes(:,num_points+1:end);%y方向偏差
    error_per_image=mean(sqrt(deltashapes_x.^2+deltashapes_y.^2), 2)./dist_pupils;%单张图片的均方根误差
    MRSE = 100*mean(error_per_image);%所有图片的均方根误差
    MRSE_display = sprintf('Mean Root Square Error for %d Training Samples: %f', num_samples_Aug, MRSE);disp(MRSE_display);%显示均方根误差
end
save('train_model_new.mat','forestmodel','Wt','meanshape','NumOfTree','max_depth');
