
# align images using landmarks - 机器学习的小学生 - CSDN博客


2018年11月01日 16:27:52[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：60


下面的代码中需要注意三个地方：
rate值的修改。
可视化参考形状。
选择变换的类型，例如  NonreflectiveSimilarity/Similarity/Affine
`clearall;
%% title
info_=[
    '文档的功能: 是实现对图像的标准化' ...
    ',标准化的方式采用将所有形状对齐的平均形状. \n'...
    '文档的创建时间为：2016/11/1/18：37 \n'  ...
    '修改文档的时间为：2018/11/01/15:38'];
%% content
datas = load('datas_for_raf.mat');
datas = datas.datas;
ref_model_size_h = floor(224);
ref_model_size_w = floor(224);
face_size = [224,224];
status = 'manu';
mean_shape0 = calc_meanshape_for_raf(datas, status);
isShow = true;
%for i=1:n_train
nlandmark = size(mean_shape0,1);
ndata = length(datas);
for i=1:ndata
    %for i = 1:1
    imgpath = datas(i).imgpath_original;
    img = imread(imgpath);
    %img = rgb2gray(img);
    if strcmp(status,'auto')
        landmark = datas(i).landmarks_auto;
    elseif strcmp(status,'manu')
        landmark = datas(i).landmarks_manu;
    end
    
    isshow = false;
    if isshow
        imshow(img);
        hold on;
        for j = 1:nlandmark
            plot(landmark(j,1),landmark(j,2),'.r','markersize',15);
            text(landmark(j,1),landmark(j,2),num2str(j));
        end
    end
    
    meanxy = mean(landmark,1);%landmark的中心
    % 将平均形状扩展到框架内
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%1. 修改下面的rate值%%%%%%%%%%%%%%%%%
    rate = [0.55,0.45]; % x, y
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    mean_shape = face_size .*rate .* mean_shape0;
    cen_point_x= mean_shape(3,1);
    cen_point_y= mean_shape(3,2);
    cen_point = face_size .*rate .* 0.5;
    cen_point_x = cen_point(1);
    cen_point_y = cen_point(2);
    len_x=abs(face_size(2)*0.5-cen_point_x);
    len_y=abs(face_size(1)*0.5-cen_point_y);
    mean_shape = mean_shape + [len_x,len_y];
    %mean_shape = disp_xy + face_size .* mean_shape;
    
    mean_shapexy = mean(mean_shape,1);%meanshape的中心
    
    % show
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% % 2.设置为true，观察映射后的形状。 %%%%%%%%%%%%%%%%%%%%%%
    isshow = true;
    if isshow
        img2 = repmat(uint8(128),ref_model_size_h,ref_model_size_w,3);
        drawshapes(img2, mean_shape);
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 3. NonreflectiveSimilarity/Similarity/Affine %%%%%%%%%%%%%%%%%
    %from meanshape to landmark，计算相似变换矩阵
    trans_mat = fitgeotrans(bsxfun(@minus,landmark,meanxy),bsxfun(@minus,mean_shape,mean_shapexy), 'Affine'); %affine %Similarity
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% 公式：（u,v)=(x,y,1)T
    % T=[sc -ss 0
    %	 ss  sc 0
    %    tx  ty 1];
    
    %% 将landmark转换到标准框架内
    m_landmark = bsxfun(@minus,landmark,meanxy);
    
    warp_landmark = transformPointsForward(trans_mat,m_landmark);
    
    for j = 1:nlandmark
        warp_landmark(j,1) = warp_landmark(j,1)+mean_shapexy(1);
        warp_landmark(j,2) = warp_landmark(j,2)+mean_shapexy(2);
    end
    %% 将图像转换到标准框架中
    %res_img=zeros(ref_model_size,ref_model_size);
    % 原理上 T(3,1)=T(3,2)=0
    T = trans_mat.T;
    T1 = T;
    T1(3,1) = T(3,1)+mean_shapexy(1)-(T(1,1)*meanxy(1)+T(2,1)*meanxy(2));
    T1(3,2) = T(3,2)+mean_shapexy(2)-(T(1,2)*meanxy(1)+T(2,2)*meanxy(2));
    trans_mat.T = T1;
    
    T2 = zeros(2,3);
    T2(1,1) = T(1,1);
    T2(1,2) = T(2,1);
    T2(2,1) = T(1,2);
    T2(2,2) = T(2,2);
    T2(1,3) = T(3,1)+mean_shapexy(1)-(T(1,1)*meanxy(1)+T(2,1)*meanxy(2));
    T2(2,3) = T(3,2)+mean_shapexy(2)-(T(1,2)*meanxy(1)+T(2,2)*meanxy(2));
    wsize = [ref_model_size_h ref_model_size_w];
    
    %matlab
    tic;
    %[size.h size.w]为目标图像的大小. [1 size.w] [1 size.h]为显示的x,y的范围。
    %RA = imref2d([wsize(1) wsize(2)], [1 wsize(2)], [1 wsize(1)]);
    RA = imref2d([wsize(1) wsize(2)], [1 wsize(2)], [1 wsize(1)]);
    [out,r] = imwarp(img, trans_mat, 'OutputView', RA);
    m_time=toc;
    %opencv
    tic;
    res_img = cv.warpAffine(img,T2,'DSize',wsize,'WarpInverse',false);
    o_time=toc;
    isShow = true;
    if isShow
        disp(['matlab相似变换的时间: ' num2str(m_time)]);
        disp(['opencv相似变换的时间: ' num2str(o_time)]);
        figure(2);
        %subplot(2,2,1);
        %imshow(img);
        %subplot(2,2,2);
        imshow(out,[],'border','tight');
        hold on;
        for j = 1:nlandmark
            plot(warp_landmark(j,1),warp_landmark(j,2),'.r','markersize',15);
        end
        %pause(2);
    end
    imgname = datas(i).imgname_aligned;
    if strcmp(status,'auto')
        new_path = 'imgs_auto';
    elseif strcmp(status,'manu')
        new_path = 'imgs_manu';
    end
    if ~exist(new_path,'dir')
        mkdir(new_path);
    end
    outfilename = [new_path '/' imgname];
    imwrite(out,outfilename);
    if mod(i,100)==0
        disp(i);
    end
    datas(i).warp_landmark = warp_landmark;
end
save('datas_for_raf_warp_landmark.mat','warp_landmark');
disp('save datas_for_raf_warp_landmark.mat over...');`calc_meanshape_for_raf.m
```python
function mean_shape = calc_meanshape_for_raf(data,status)
%% 计算数据集的平均形状
% status : auto or manu
%如果是cell类型，转化为struct
if iscell(data)
    data=[data{:}];
end
if strcmp(status,'auto')
    mean_shape = zeros(size(data(1).landmarks_auto));
elseif strcmp(status,'manu')
    mean_shape = zeros(size(data(1).landmarks_manu));
end
num_shapes = 0;
for i = 1:length(data)
    if strcmp(status,'auto')
        shape_i = double(data(i).landmarks_auto);
    elseif strcmp(status,'manu')
        shape_i = double(data(i).landmarks_manu);
    end
    if isempty(shape_i)
        continue;
    end
    shape_min = min(shape_i, [], 1);% min(shape_i,[],1)表示沿着行的最小值，
    shape_max = max(shape_i, [], 1);%
    
    % translate to origin point  原点化
    shape_i = bsxfun(@minus, shape_i, shape_min);
    
    % resize shape  尺度化
    shape_i = bsxfun(@rdivide, shape_i, shape_max - shape_min);
    
    mean_shape = mean_shape + shape_i;
    num_shapes = num_shapes + 1;
end
mean_shape = mean_shape ./ num_shapes;
%img = 255 * ones(800, 800, 3);
%drawshapes(img, 50 + 400 * mean_shape);
show = false;
if show
    img = 255 * ones(800, 800, 3);
    
    mean_shape = 400*mean_shape;
    cen_point_x= mean_shape(3,1);
    cen_point_y= mean_shape(3,2);
    
    len_x=abs(400-cen_point_x);
    len_y=abs(400-cen_point_y);
    
    mean_shape(:,1) = mean_shape(:,1) + len_x;
    mean_shape(:,2) = mean_shape(:,2) + len_y;
    
    drawshapes(img, mean_shape);
end
end
```
参考形状：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101191841406.jpg)
原图像：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101192704255.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)
Affine变换的结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101191947167.jpg)
Similarity变换后的图像：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101192539814.jpg)

