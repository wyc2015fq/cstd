
# prepare hdf5 data for training - 机器学习的小学生 - CSDN博客


2017年05月18日 20:47:52[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：461


**Matlab:**
```python
% hdf5_file_2
```
```python
clearvars;
load
```
```python
train_data_2.
```
```python
mat;
load
```
```python
mean_file.
```
```python
mat ;
```
```python
% load mean_file
```
```python
train_feat =
```
```python
[train_data{
```
```python
1
```
```python
};train_data{
```
```python
2
```
```python
};train_data{
```
```python
3
```
```python
};train_data{
```
```python
4
```
```python
}]
```
```python
;
test_feat = train_data
```
```python
{
```
```python
5
```
```python
}
```
```python
;
```
```python
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% check %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
```
```python
% imgName_temp1 = {train_feat.imgName};
```
```python
% imgName_temp2 = {test_feat.imgName};
```
```python
%
```
```python
% imgName2 = {imgName_temp1{:},imgName_temp2{:}};
```
```python
% % check all
```
```python
%
```
```python
% load datas.mat;
```
```python
%
```
```python
% imgName1 = {datas.imgName};
```
```python
%
```
```python
% result = setdiff(imgName1,imgName2); % 判断两个集合是否相等。
```
```python
% result0 = setdiff(imgName2,imgName1);
```
```python
%
```
```python
% test_imgName = {'AJ_Lamas_0001.jpg','nihao'};
```
```python
%
```
```python
% result2 = setdiff(test_imgName,imgName2);
```
```python
% result3 = setdiff(imgName2,test_imgName);
```
```python
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% end %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
```
```python
%% in order to load data quickly ,and don't use 'shuffle' in prototxt
```
```python
%% we shuffle first,and pass data into net
```
```python
ntrain =
```
```python
size
```
```python
(train_feat,
```
```python
1
```
```python
);
ntest  =
```
```python
size
```
```python
(test_feat,
```
```python
1
```
```python
);
```
```python
if
```
```python
~exist(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'file'
```
```python
)
    train_rand_ind = randperm(ntrain);
    test_rand_ind  = randperm(ntest);
    save(
```
```python
'rand_ind.mat'
```
```python
,
```
```python
'train_rand_ind'
```
```python
,
```
```python
'test_rand_ind'
```
```python
);
```
```python
else
```
```python
load
```
```python
rand_ind.
```
```python
mat;
```
```python
% load train_rand_ind ,test_rand_ind
```
```python
disp
```
```python
(
```
```python
'load rand_ind over ...'
```
```python
);
```
```python
end
```
```python
train_feat = train_feat(train_rand_ind);
test_feat = test_feat(test_rand_ind);
```
```python
% write to hdf5 file
```
```python
% follow demo.m
```
```python
imgSize =
```
```python
[
```
```python
227
```
```python
227
```
```python
]
```
```python
;
```
```python
% 227 * 227 for alexnet
```
```python
n_train_file =
```
```python
20
```
```python
;
n_test_file =
```
```python
fix
```
```python
(n_train_file /
```
```python
4
```
```python
);
train_each_size =
```
```python
floor
```
```python
(
```
```python
length
```
```python
(train_feat)/n_train_file);
test_each_size  =
```
```python
floor
```
```python
(
```
```python
length
```
```python
(test_feat)/n_test_file);
train_filePath =
```
```python
'./hdf5_file_2/train'
```
```python
;
test_filePath =
```
```python
'./hdf5_file_2/test'
```
```python
;
train_split = cell(n_train_file,
```
```python
1
```
```python
);
test_split = cell(n_test_file,
```
```python
1
```
```python
);
```
```python
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 1: train 0: test %%%%%%%%%%%%%%
```
```python
if
```
```python
0
```
```python
for
```
```python
i
```
```python
=
```
```python
1
```
```python
:n_train_file
        start_ind = (
```
```python
i
```
```python
-
```
```python
1
```
```python
) * train_each_size +
```
```python
1
```
```python
;
```
```python
if
```
```python
i
```
```python
== n_train_file
            end_ind = ntrain;
```
```python
else
```
```python
end_ind =
```
```python
i
```
```python
* train_each_size;
```
```python
end
```
```python
ind_range = start_ind : end_ind;
```
```python
% trainFeature : width * height * c * nsamples
```
```python
trainFeature =
```
```python
zeros
```
```python
(imgSize(
```
```python
1
```
```python
),imgSize(
```
```python
2
```
```python
),
```
```python
3
```
```python
,
```
```python
length
```
```python
(ind_range));
        trainLabel   =
```
```python
zeros
```
```python
(
```
```python
2
```
```python
,
```
```python
length
```
```python
(ind_range));
```
```python
% for headpose and smile
```
```python
t =
```
```python
1
```
```python
;
```
```python
for
```
```python
j
```
```python
= start_ind : end_ind
            imgPath = train_feat(
```
```python
j
```
```python
).imgPath;
            imgPath = strrep(imgPath,
```
```python
'imgs'
```
```python
,
```
```python
'alex_imgs'
```
```python
);
            img = imread(imgPath);
```
```python
% rgb
```
```python
img =
```
```python
permute
```
```python
(img,
```
```python
[
```
```python
2
```
```python
1
```
```python
3
```
```python
]
```
```python
);
            img = img(:,:,
```
```python
[
```
```python
3
```
```python
2
```
```python
1
```
```python
]
```
```python
);
```
```python
% rgb 2 bgr
```
```python
% 减去均值文件 mean_file
```
```python
img = double(img) - mean_file ;
```
```python
% mean_file 文件满足:width * hegith * c (bgr)
```
```python
trainFeature(:,:,:,t) = img;
            trainLabel(
```
```python
1
```
```python
,t) = train_feat(
```
```python
j
```
```python
).headpose +
```
```python
2
```
```python
;
```
```python
% for head pose label
```
```python
trainLabel(
```
```python
2
```
```python
,t) = train_feat(
```
```python
j
```
```python
).smile;
```
```python
% for smile label
```
```python
t = t +
```
```python
1
```
```python
;
```
```python
end
```
```python
fileName =
```
```python
[
```
```python
'train_part'
```
```python
num2str(i)
```
```python
'.h5'
```
```python
]
```
```python
;
```
```python
% train_part1 train_part2 .....
```
```python
%% save to hdf5
```
```python
filePath =
```
```python
[
```
```python
'./hdf5_file_2/train/'
```
```python
fileName]
```
```python
;
        h5create(filePath,
```
```python
'/data'
```
```python
,
```
```python
size
```
```python
(trainFeature),
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
        h5create(filePath,
```
```python
'/label'
```
```python
,
```
```python
size
```
```python
(trainLabel),
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
        h5write(filePath,
```
```python
'/data'
```
```python
,single(trainFeature));
        h5write(filePath,
```
```python
'/label'
```
```python
,single(trainLabel));
```
```python
end
```
```python
disp
```
```python
(
```
```python
'save trainFeature/trainLabel to hdf5 over ...'
```
```python
);
```
```python
else
```
```python
for
```
```python
i
```
```python
=
```
```python
1
```
```python
:n_test_file
        start_ind = (
```
```python
i
```
```python
-
```
```python
1
```
```python
) * test_each_size +
```
```python
1
```
```python
;
```
```python
if
```
```python
i
```
```python
== n_test_file
            end_ind = ntest;
```
```python
else
```
```python
end_ind =
```
```python
i
```
```python
* test_each_size;
```
```python
end
```
```python
ind_range = start_ind : end_ind;
```
```python
% trainFeature : width * height * c * nsamples
```
```python
testFeature =
```
```python
zeros
```
```python
(imgSize(
```
```python
1
```
```python
),imgSize(
```
```python
2
```
```python
),
```
```python
3
```
```python
,
```
```python
length
```
```python
(ind_range));
        testLabel   =
```
```python
zeros
```
```python
(
```
```python
2
```
```python
,
```
```python
length
```
```python
(ind_range));
```
```python
% for headpose and smile
```
```python
t =
```
```python
1
```
```python
;
```
```python
for
```
```python
j
```
```python
= start_ind : end_ind
            imgPath = test_feat(
```
```python
j
```
```python
).imgPath;
            imgPath = strrep(imgPath,
```
```python
'imgs'
```
```python
,
```
```python
'alex_imgs'
```
```python
);
            img = imread(imgPath);
```
```python
% rgb
```
```python
img =
```
```python
permute
```
```python
(img,
```
```python
[
```
```python
2
```
```python
1
```
```python
3
```
```python
]
```
```python
);
            img = img(:,:,
```
```python
[
```
```python
3
```
```python
2
```
```python
1
```
```python
]
```
```python
);
```
```python
% rgb 2 bgr
```
```python
% 减去均值文件 mean_file
```
```python
img = double(img) - mean_file ;
```
```python
% mean_file 文件满足:width * hegith * c (bgr)
```
```python
testFeature(:,:,:,t) = img;
            testLabel(
```
```python
1
```
```python
,t) = test_feat(
```
```python
j
```
```python
).headpose +
```
```python
2
```
```python
;
```
```python
% for head pose label
```
```python
testLabel(
```
```python
2
```
```python
,t) = test_feat(
```
```python
j
```
```python
).smile;
```
```python
% for smile
```
```python
t = t +
```
```python
1
```
```python
;
```
```python
end
```
```python
fileName =
```
```python
[
```
```python
'test_part'
```
```python
num2str(i)
```
```python
'.h5'
```
```python
]
```
```python
;
```
```python
% train_part1 train_part2 .....
```
```python
%% save to hdf5
```
```python
filePath =
```
```python
[
```
```python
'./hdf5_file_2/test/'
```
```python
fileName]
```
```python
;
        h5create(filePath,
```
```python
'/data'
```
```python
,
```
```python
size
```
```python
(testFeature),
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
        h5create(filePath,
```
```python
'/label'
```
```python
,
```
```python
size
```
```python
(testLabel),
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
        h5write(filePath,
```
```python
'/data'
```
```python
,single(testFeature));
        h5write(filePath,
```
```python
'/label'
```
```python
,single(testLabel));
```
```python
end
```
```python
disp
```
```python
(
```
```python
'save testFeature/testLabel to hdf5 over ...'
```
```python
);
```
```python
end
```
**Python:**

