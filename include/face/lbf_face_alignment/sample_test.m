clc ; clear; %close all;
load train_model_new.mat
work_path = pwd;
max_stage = 5;%迭代最大阶段数
Radius = [0.4 0.3 0.2 0.15 0.1];
%dbnames={'Helen','LFPW','IBUG'};%选择要测试的数据库
dbnames={'LFPW'};%选择要测试的数据库
num_points=size(meanshape,2)/2;max_depth=5;

disp('step1:读pts及图片');
%[imgTr,realshapes,imgpathlists]=resizeimg(dbnames,'test');
[imgTe,realshapes,imgpathlists]=readimg(dbnames,'test',num_points);%测试样本读取

disp('step2:形状初始化');
num_pic=length(imgTe);
imgTe_gray=cell(1,num_pic);
detec_bbox=zeros(num_pic,4);
for i_sample = 1:num_pic
    img = imgTe{i_sample};
    if ndims(img)==3, imgTe_gray{i_sample}=rgb2gray(img);
    else imgTe_gray{i_sample}=img;end
    realshape=realshapes(i_sample,:);realshape=reshape(realshape,num_points,2);
    %{
    %V_J检测人脸
    [x1_f,y1_f,x3_f,y3_f] = Viola_Jones_face_detect(work_path,imgTe{i_sample},imgpathlists{i_sample}); %检测人脸位置
    if isempty(x1_f),fprintf('%d\n',i_sample);continue;end
    detec_bbox(i_sample,:)=[x1_f,x3_f,y1_f,y3_f];
    %}
end

%
%若为其他数据库，不能用此段代码，只能用上面的V_J检测函数
detec_bbox=[];
for i_db=1:length(dbnames)
    %加载事先保存的V_J检测器检测到的人脸框
    if strcmp(dbnames{i_db},'Helen')
        load helen_test_detec_bbox.mat
    elseif strcmp(dbnames{i_db},'LFPW')
        load lfpw_test_detec_bbox.mat
    elseif strcmp(dbnames{i_db},'IBUG')
        load ibug_test_detec_bbox.mat
    else
        error('请使用V_J function，并注释掉此段代码!');
    end 
    detec_bbox=[detec_bbox;bbox];
end
%}
for i_sample = 1:num_pic
    shapes_intial(i_sample,:)=resetshape(meanshape,detec_bbox(i_sample,:));%形状初始化
end
disp('step3:模型回归');
t0=cputime;
for i_sample = 1:num_pic
    
    shape_curstage=shapes_intial(i_sample,:);
    %显示初始化后的形状
    %subplot(1,2,1),imshow(imgTe{i_sample});hold on;plot(shape_curstage(1:num_points),shape_curstage(num_points+1:end),'r.');title('初始形状');hold off;
    bbox3=max(shape_curstage(1:num_points))-min(shape_curstage(1:num_points));%当前人脸宽
    bbox4=max(shape_curstage(1+num_points:end))-min(shape_curstage(1+num_points:end));%当前人脸高
    realshape=realshapes(i_sample,:);realshape=reshape(realshape,num_points,2);%人脸真实形状
    
    for i_stage = 1:max_stage
        %BF
        [Pt Pt_new]=isample_traverse(forestmodel,Radius,shape_curstage,i_stage,num_points,NumOfTree,imgTe_gray{i_sample},bbox3,bbox4,max_depth);%遍历树，得到全局二值特征
        %tic;
        Delta=Pt*Wt{i_stage};%回归形状增量
        %toc;
        %{
        tic;
        Delta_new=0;
        for i_Pt=1:length(Pt_new)
          Delta_new=Delta_new+Wt{i_stage}(16*(i_Pt-1)+Pt_new(i_Pt),:);
        end
        toc;
        %}
        Delta=[Delta(1:num_points)*bbox3,Delta(num_points+1:end)*bbox4];%根据相应的人脸尺寸，还原形状增量
        %更新人脸形状
        shape_curstage=shape_curstage+Delta;
        shape_curstage=reshape(shape_curstage,num_points,2);
        %更新人脸尺寸
        minx=min(shape_curstage(:,1));maxx=max(shape_curstage(:,1));
        miny=min(shape_curstage(:,2));maxy=max(shape_curstage(:,2));
        bbox3=maxx-minx;bbox4=maxy-miny;%宽，高
        error_of_image(i_sample,i_stage) = compute_error(realshape, shape_curstage);%计算误差
        shape_curstage=shape_curstage(:)';%转成行向量
    end
    
    errors(i_sample)=error_of_image(i_sample,max_stage);
    
    %fprintf('%f\n',error_of_image(i_sample,max_stage));%显示单张图片的误差
    Shape_result=reshape(shape_curstage,num_points,2);
    %显示最终的形状
    %subplot(1,2,2),imshow(imgTe{i_sample});hold on;plot(Shape_result(:,1),Shape_result(:,2),'g.');title('结果形状');hold off;pause;
end 
t1=cputime-t0;
    %
    %计算正确率的一段代码
    factor=0.1;%设定一个定位标准
    precision_rate=length(find(errors<factor))/length(errors)*100;%计算此标准下的正确率
    fprintf('%f\n',precision_rate);%显示正确率
    %}

disp('step4:计算误差');
for i_stage = 1:max_stage
    MRSE = 100*mean(error_of_image(:,i_stage));
    MRSE_display = sprintf('Mean Root Square Error for %d Test Samples: %f',i_sample, MRSE);
    disp(MRSE_display);
end
disp(strcat('算法速度:',num2str(floor(num_pic/t1)),'fps'));    %matlab上显示算法速度