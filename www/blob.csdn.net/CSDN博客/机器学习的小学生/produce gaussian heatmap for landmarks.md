
# produce gaussian heatmap for landmarks - 机器学习的小学生 - CSDN博客


2018年11月09日 21:20:38[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：174



```python
% 28 * 28 for conv3
% 14 * 14 for conv4
% 7 * 7 for conv5
close all;
datas = load('datas5224_pure3.mat');
datas = datas.datas;
f = @(x,meanx,nSigma) exp(-1/2*(x-meanx)'*nSigma*(x-meanx));
for i = 1:length(datas)
    landmark  = datas(i).after_landmark;
    
    imgpath2 = datas(i).imgpath2;
    %imgpath = strrep(imgpath,'flickr','flickr224');
    img = imread(imgpath2);
    subplot(141);
    imshow(img);
    hold on;
    for j = 1:size(landmark,1)
        plot(landmark(j,1),landmark(j,2),'.r','markersize',15);
        text(landmark(j,1),landmark(j,2),num2str(j),'color','w');
    end
    
    subplot(142);
    heatmap = compute_heatmap([28,28],imgSize,[landmark(8,:);landmark(12,:);landmark(15,:)]);
    imshow(heatmap,[]);
    
    subplot(143);
    heatmap = compute_heatmap([14,14],imgSize,[landmark(8,:);landmark(12,:);landmark(15,:)]);
    imshow(heatmap,[]);
    
    subplot(144);
    heatmap = compute_heatmap([7,7],imgSize,[landmark(8,:);landmark(12,:);landmark(15,:)]);
    imshow(heatmap,[]);
    
    break;
end
function heatmap = compute_heatmap(imgsize,imgSize,landmarks)
% imgsize: the heatmap size for dst
% imgSize: the heatmap size for src, examples: 224 * 224 for CNN
% landmarks
landmarks = round(landmarks .* (imgsize ./ imgSize));
%tau = max((imgSize ./ imgsize)) * 0.1;
tau = min((imgsize ./ imgSize)) * 20; % change those values
Sigma = [tau^2 0 ; 0 tau^2]; % not use
nSigma = [1.0/(tau^2) 0;0 1.0/(tau^2)]; % inverse of Sigma
% note that: x and meanx's shape: [2,1], not [1,2]
f = @(x,meanx,nSigma) exp(-1/2*(x-meanx)'*nSigma*(x-meanx));
heatmap = zeros(imgsize(1),imgsize(2));
for i = 1:size(landmarks,1) % nlandmark
    landmark = landmarks(i,:); % x,y
    for j = 1:imgsize(2) % x: w
        for k = 1:imgsize(1) % y: h
            seat = [j,k]; % (x,y)
            heatmap(k,j) = heatmap(k,j) + f(seat',landmark',nSigma);
        end
    end
end
activation_type = 'softmax';
if strcmp(activation_type,'sigmoid')
    % pass Sigmoid activation funcion
    heatmap = 1 ./ (1+exp(-heatmap));
elseif strcmp(activation_type,'softmax')
    % pass softmax function
    heatmap = exp(heatmap);
    heatmap = heatmap ./ sum(heatmap(:));
end
end
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181109212017570.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3h1bHVodWkxMjM=,size_16,color_FFFFFF,t_70)

