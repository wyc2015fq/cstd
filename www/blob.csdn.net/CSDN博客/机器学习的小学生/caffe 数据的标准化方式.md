
# caffe 数据的标准化方式 - 机器学习的小学生 - CSDN博客


2017年12月26日 10:22:45[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：687



## (1) 减去均值方式1
从caffe 给的 models/bvlc_reference_caffenet/下的训练协议train_val.prototxt
```python
transform_param {
    mirror: true
    crop_size:
```
```python
227
```
```python
mean_file:
```
```python
"data/ilsvrc12/imagenet_mean.binaryproto"
```
```python
}
```
```python
# mean pixel / channel-wise mean instead of mean image
```
```python
#  transform_param {
```
```python
#    crop_size: 227
```
```python
#    mean_value: 104
```
```python
#    mean_value: 117
```
```python
#    mean_value: 123
```
```python
#    mirror: true
```
```python
#  }
```
以及，给定的分类示例：
```python
mu = np.load(caffe_root +
```
```python
'python/caffe/imagenet/ilsvrc_2012_mean.npy'
```
```python
)
mu = mu.mean(
```
```python
1
```
```python
).mean(
```
```python
1
```
```python
)
```
```python
# average over pixels to obtain the mean (BGR) pixel values
```
```python
print
```
```python
'mean-subtracted values:'
```
```python
, zip(
```
```python
'BGR'
```
```python
, mu)
```
```python
# create transformer for the input called 'data'
```
```python
transformer = caffe.io.Transformer({
```
```python
'data'
```
```python
: net.blobs[
```
```python
'data'
```
```python
].data.shape})
transformer.set_transpose(
```
```python
'data'
```
```python
, (
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
,
```
```python
1
```
```python
))
```
```python
# move image channels to outermost dimension
```
```python
transformer.set_mean(
```
```python
'data'
```
```python
, mu)
```
```python
# subtract the dataset-mean value in each channel
```
```python
transformer.set_raw_scale(
```
```python
'data'
```
```python
,
```
```python
255
```
```python
)
```
```python
# rescale from [0, 1] to [0, 255]
```
```python
transformer.set_channel_swap(
```
```python
'data'
```
```python
, (
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
))
```
```python
# swap channels from RGB to BGR
```
我们看到，减去均值文件的方式是：
```python
img
```
```python
-
```
```python
[104,117,123]
```
```python
;
```
即三个通道，每个通道减去对应的值。
## (2) 减均值方式2
就是我们通常意义上的减去均值，即
```python
img
```
```python
-
```
```python
mean(img_all)
```
```python
;
```
img 和 mean(img_all)拥有同样的size。
## (3) 减去均值在除以标准差
假定图像中每一个位置的像素为一个随机变量，那么标准化的方式为：

$$
\frac{x-\mu}{\sigma}
$$

```python
img =
```
```python
img - mean(img_all);
```
```python
img =
```
```python
img ./ std(img_all);
```
三种处理数据的方式，在识别率上有啥差距呢？有空做一下调查。当然，对于图像来说，进行标准化是好的，曾经做过一个实验，使用标准化和未标准化数据相差大约10个百分点。

