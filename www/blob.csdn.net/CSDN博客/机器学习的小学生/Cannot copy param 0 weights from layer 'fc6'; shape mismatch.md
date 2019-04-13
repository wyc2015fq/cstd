
# Cannot copy param 0 weights from layer 'fc6'; shape mismatch. - 机器学习的小学生 - CSDN博客


2017年05月17日 10:21:58[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：4483个人分类：[Caffe																](https://blog.csdn.net/xuluhui123/article/category/6777566)



问题：
Cannot copy param 0 weights from layer ‘fc6’; shape mismatch.**Source param shape**is 4096 9216 (37748736);**target param shape**is 4096 2304 (9437184). To learn this layer’s parameters from scratch rather than copying from a saved net, rename the layer.
表示训练好的模型的参数（model文件）和协议中该层的参数不匹配（prototxt文件）。
源文件为model 文件，目标文件为 prototxt
发生的情况：在我们进行网络微调时，同样的网络结构，但是输入图片的大小不一样，就会在网络的第一个全连接层出现问题。就像提示的那样，如果想从头开始训练该层网络，那么对其进行重新命名。就会因网络权重的拷贝过程中，提示不匹配的问题。
利用Caffe 网络结构可视化可视化工具：
[http://ethereon.github.io/netscope/\#/editor](http://ethereon.github.io/netscope/#/editor)
为了方便数据形状的可视化，我们在数据层添加transform_param…crop_size，那么可视化工具会计算推测各个输出的数据的形状，也很方便的参看权重的形状，虽然HDF5Data层没有transform_param参数：
```python
layer
```
```python
{
```
```python
name
```
```python
:
```
```python
"data"
```
```python
type:
```
```python
"HDF5Data"
```
```python
top:
```
```python
"data"
```
```python
top:
```
```python
"label"
```
```python
include {
    phase: TRAIN
```
```python
}
```
```python
hdf5_data_param
```
```python
{
```
```python
source
```
```python
:
```
```python
"hdf5_file/train/train_list.txt"
```
```python
batch_size:
```
```python
42
```
```python
}
```
```python
transform_param
```
```python
{
```
```python
crop_size
```
```python
:
```
```python
128
```
```python
}
}
```

