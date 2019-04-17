# cascade rcnn检测代码 - Snoopy_Dream - CSDN博客





2018年06月22日 20:54:05[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：972
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[cascade rcnn目标检测](https://blog.csdn.net/column/details/25672.html)












```python
% Cascade-RCNN
% Copyright (c) 2018 The Regents of the University of California
% see cascade-rcnn/LICENSE for details
% Written by Zhaowei Cai [zwcai-at-ucsd.edu]
% Please email me if you find bugs, or have suggestions or questions!

clear all; close all;
addpath('../../matlab/'); addpath('../../utils/');
cocoDir = '/home/ygx/cocoapi'; % 修改成自己的
addpath(genpath([cocoDir '/MatlabAPI']));

%% experimental setup %%
% network
root_dir = './res50-15s-800-fpn-cascade/';
%root_dir = '/home/ygx/cascade-rcnn/examples/voc/res50-18s-600-rfcn-cascade/';
binary_file = [root_dir 'cascadercnn_voc_iter_35000.caffemodel'];
assert(exist(binary_file, 'file') ~= 0); 
definition_file = [root_dir 'deploy.prototxt'];
assert(exist(definition_file, 'file') ~= 0);
use_gpu = true;
if ~use_gpu
  caffe.set_mode_cpu();
else
  caffe.set_mode_gpu(); gpu_id = 0;
  caffe.set_device(gpu_id);
end
net = caffe.Net(definition_file, binary_file, 'test');

% dataset %%%add by ygx
dataType = '2007';
fileFolder=fullfile('/home/ygx/cascade-rcnn/data/VOCdevkit_car/VOC2007/test_a');%文件夹名plane
dirOutput=dir(fullfile(fileFolder,'*'));%如果存在不同类型的文件，用‘*’读取所有，如果读取特定类型文件，'.'加上文件类型，例如用‘.jpg’
fileNames={dirOutput.name}'; 
len=length(fileNames);
fileNames=fileNames(3:len);
num_cls = 1;
obj_ids = 2:num_cls+1;
nImg = len-2;
%class_name = {'第一个','第2个','第3个','第4个','第5个','第6 个'}
% architecture
if(~isempty(strfind(root_dir, 'cascade'))), CASCADE = 1;
else CASCADE = 0; end
if(~isempty(strfind(root_dir, 'fpn'))), FPN = 1;
else FPN = 0; end
if (~CASCADE)
  % baseline model
  if (FPN)
    proposal_blob_names = {'proposals_to_all'};
  else
    proposal_blob_names = {'proposals'};
  end
  bbox_blob_names = {'output_bbox_1st'};
  cls_prob_blob_names = {'cls_prob_1st'};
  output_names = {'1st'};
else
  % cascade-rcnn model
  if (FPN)
    proposal_blob_names = {'proposals_to_all','proposals_to_all_2nd',...
        'proposals_to_all_3rd','proposals_to_all_2nd','proposals_to_all_3rd'};
  else
    proposal_blob_names = {'proposals','proposals_2nd','proposals_3rd',...
        'proposals_2nd','proposals_3rd'};
  end
  bbox_blob_names = {'output_bbox_1st','output_bbox_2nd','output_bbox_3rd',...
      'output_bbox_2nd','output_bbox_3rd'};
  cls_prob_blob_names = {'cls_prob_1st','cls_prob_2nd','cls_prob_3rd',...
      'cls_prob_2nd_avg','cls_prob_3rd_avg'};
  output_names = {'1st','2nd','3rd','2nd_avg','3rd_avg'};
end
num_outputs = numel(proposal_blob_names);
assert(num_outputs==numel(bbox_blob_names));
assert(num_outputs==numel(cls_prob_blob_names));
assert(num_outputs==numel(output_names));

% detection configuration
detect_final_boxes = cell(nImg,num_outputs);
det_thr = 0.5; % threoshold
max_per_img = 100; % max number of detections
pNms.type = 'maxg'; pNms.overlap = 0.5; pNms.ovrDnm = 'union'; % NMS

% saveing ID and evaluation log
comp_id = 'tmp'; % specify a unique ID if you want to archive the results
log_id = '280k';
eval_log = sprintf('%scoco_eval_%s_%s.txt',root_dir,log_id,dataType); 

% image pre-processing
if (FPN)
    shortSize = 416;%800; 
    longSize = 1056;%1312;%!!!!!!!
else
    shortSize = 600; 
    longSize = 1000; 
end
mu0 = ones(1,1,3); mu0(:,:,1:3) = [104 117 123];
usedtime=0; 

%%%%%%%%%%%下面是自己加的
fp1 = fopen('/home/ygx/cascade-rcnn/data/VOCdevkit_car/3rd_avg_35000.txt','wt');
fp2 = fopen('/home/ygx/cascade-rcnn/data/VOCdevkit_car/3rd_only_35000.txt','wt');
fprintf(fp1,'name,coordinate\n');
fprintf(fp2,'name,coordinate\n');
%% running %%
for kk = 1:nImg
  img= imread(sprintf('/home/ygx/cascade-rcnn/data/VOCdevkit_car/VOC2007/test_a/%s',fileNames{kk}));
  orgImg = img;
  if (size(img,3)==1), img = repmat(img,[1 1 3]); end
  [orgH,orgW,~] = size(img);
  
  rzRatio = shortSize/min(orgH,orgW);
  imgH = min(rzRatio*orgH,longSize); imgW = min(rzRatio*orgW,longSize);
  imgH = round(imgH/32)*32; imgW = round(imgW/32)*32; % must be the multiple of 32
  hwRatios = [imgH imgW]./[orgH orgW];
  img = imresize(img,[imgH imgW]); 
  mu = repmat(mu0,[imgH,imgW,1]);
  img = single(img(:,:,[3 2 1]));
  img = bsxfun(@minus,img,mu);
  img = permute(img, [2 1 3]);

  % network forward
  tic; outputs = net.forward({img}); pertime=toc;
  usedtime=usedtime+pertime; avgtime=usedtime/kk;
    
  for nn = 1:num_outputs
    detect_boxes = cell(num_cls,1); 
    tmp = squeeze(net.blobs(bbox_blob_names{nn}).get_data()); 
    tmp = tmp'; tmp = tmp(:,2:end);
    tmp(:,[1,3]) = tmp(:,[1,3])./hwRatios(2);
    tmp(:,[2,4]) = tmp(:,[2,4])./hwRatios(1);
    % clipping bbs to image boarders
    tmp(:,[1,2]) = max(0,tmp(:,[1,2]));
    tmp(:,3) = min(tmp(:,3),orgW); tmp(:,4) = min(tmp(:,4),orgH);
    tmp(:,[3,4]) = tmp(:,[3,4])-tmp(:,[1,2])+1;
    output_bboxs = double(tmp);  
    
    tmp = squeeze(net.blobs(cls_prob_blob_names{nn}).get_data()); 
    cls_prob = tmp'; 
    
    tmp = squeeze(net.blobs(proposal_blob_names{nn}).get_data());
    tmp = tmp'; tmp = tmp(:,2:end); 
    tmp(:,[3,4]) = tmp(:,[3,4])-tmp(:,[1,2])+1;
    proposals = tmp;
    
    keep_id = find(proposals(:,3)~=0 & proposals(:,4)~=0);
    proposals = proposals(keep_id,:); 
    output_bboxs = output_bboxs(keep_id,:); cls_prob = cls_prob(keep_id,:);
% 0000.jpg,1,1_1_2_2;3_1_2_2
% 0000.jpg,2,4_2_9_5  格式如左上，后期
    for i = 1:num_cls
      id = obj_ids(i);        %其实就是类似与加1的操作，因为背景类的原因
      prob = cls_prob(:,id); %prob = cls_prob(:,1表示是回背景类)        
      bbset = double([output_bboxs prob]);
      if (det_thr>0)
          keep_id = find(prob>=det_thr); bbset = bbset(keep_id,:);
      end
      bbset=bbNms(bbset,pNms);
      if (max_per_img>0 && size(bbset,1)>max_per_img)
          rank_scores = sort(bbset(:,5),'descend');
          keep_id = find(bbset(:,5)>=rank_scores(max_per_img));
          bbset = bbset(keep_id,:);
      end
      b=size(bbset);
      %numbox表示一幅图被检测出目标的个数
      numbox=b(1);
      %只保留3rd_avg也是最后一个最精确的一个
%       if (nn==3)
%           filenames=fileNames(kk);
%           filename=filenames{1};
%           fprintf(fp2,'%s,',filename);%图名.jpg,
%           %%%%fprintf(fp2,'%s,',char(class_name(i)));  %%类别名
%           if (numbox==0)%如果什么都没检测出来，就写上0 0 1 1
%               %fprintf(fp2, '%d_%d_%d_%d_%d',0,0,1,1,0);
%               fprintf(fp1, '%d_%d_%d_%d;',0,0,1,1);
%               fprintf(fp2,'\n');
%           else
%               for j=1:numbox
%                   bb=bbset(j,1:5);
%                   %fprintf(fp2, '%d_%d_%d_%d_%f',int16(bb(1)),int16(bb(2)),int16(bb(3)),int16(bb(4)),bb(5));
%                   fprintf(fp1, '%d_%d_%d_%d;',int16(bb(1)),int16(bb(2)),int16(bb(3)),int16(bb(4)));
%                   if (j==numbox)
%                       fprintf(fp2,'\n');
%                   else
%                       fprintf(fp2,';');
%                   end
%               end
%           end
%       end      
      
      if (nn==5)
          filenames=fileNames(kk);
          filename=filenames{1};
          fprintf(fp1,'%s,',filename);%图名.jpg,
          %%%fprintf(fp1,'%s,',char(class_name(i)));   %%类别名
          if (numbox==0)
              %fprintf(fp1, '%d_%d_%d_%d_%d',0,0,1,1,0);
              fprintf(fp1, '%d_%d_%d_%d',0,0,1,1);
              fprintf(fp1,'\n');
          else
              for k=1:numbox
                  bb=bbset(k,1:5);
                  %fprintf(fp1, '%d_%d_%d_%d_%f',int16(bb(1)),int16(bb(2)),int16(bb(3)),int16(bb(4)),bb(5));
                  fprintf(fp1, '%d_%d_%d_%d',int16(bb(1)),int16(bb(2)),int16(bb(3)),int16(bb(4)));
                  if (k==numbox)
                      fprintf(fp1,'\n');
                  else
                      fprintf(fp1,';');
                  end
              end
          end
      end    
    end
  end
  if (mod(kk,100)==0), fprintf('idx %i/%i, avgtime=%.4fs\n',kk,nImg,avgtime); end
end

%% evaluation %%

diary('off');

caffe.reset_all();
```




