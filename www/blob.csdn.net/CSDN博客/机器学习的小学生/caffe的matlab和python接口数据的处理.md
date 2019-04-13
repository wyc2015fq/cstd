
# caffe的matlab和python接口数据的处理 - 机器学习的小学生 - CSDN博客


2017年05月13日 15:47:59[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：516


**Python:**
```python
arr = np.load(
```
```python
'ilsvrc_2012_mean.npy'
```
```python
)
```
```python
print
```
```python
arr.shape
```
```python
# (3,256,256) channel * height * width
```
```python
########## 数据的转置 ###############
```
```python
arr = np.transpose(arr,[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
0
```
```python
])
```
```python
# height * width *　channel
```
```python
print
```
```python
arr.shape
```
```python
########## 通道的交换 ###############
```
```python
arr = arr[:,:,[
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
]]
```
```python
print
```
```python
arr.shape
```
```python
################## test lena ##########
```
```python
img = imread(
```
```python
'lena.jpg'
```
```python
)
img = img[:,:,(
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
)]
```
```python
# change from rgb to gbr
```
```python
img = img[:,:,(
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
)]
```
```python
# change back
```
```python
plt.imshow(arr)
plt.show()
```
**Matlab:**
```python
img = imread(
```
```python
'lena.jpg'
```
```python
);
```
```python
% height * width * channel[rgb]
```
```python
%%%%%%%%%%%%%%%%% [h,w,c] to [w,h,c] %%%%%%%%%%%%%
```
```python
%%%%%%%%%%%% 因为matlab是按列存储的,caffe和python是按行存储的
```
```python
imgforwd =
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
```
```python
%%%%%%%%%%%%%%% rgb to bgr %%%%%%%%%%%%%%
```
```python
%%%%%%%%%%%因为caffe利用了opencv，opencv图像的存储就是bgr
```
```python
imgforwd = imgforwd(:,:,
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
%%%%%%%%%%% 逆向回来 %%%%%%%%%%%%
```
```python
imgback =
```
```python
permute
```
```python
(imgforwd,
```
```python
[
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
]
```
```python
);
imgback = imgback(:,:,
```
```python
[
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
]
```
```python
);
figure;
subplot(
```
```python
121
```
```python
),imshow(imgforwd);
subplot(
```
```python
122
```
```python
),imshow(imgback);
```
![这里写图片描述](https://img-blog.csdn.net/20170513155853228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

