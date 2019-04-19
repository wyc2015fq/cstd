# 生成hdf5文件用于多标签训练 - 数据之美的博客 - CSDN博客
2017年02月26日 22:20:32[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：698
- 导入相关库
```python
import random
from PIL import Image
import numpy as np
import h5py
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- 文件配置
```python
IMAGE_DIR = ['image_train', 'image_test']
HDF5_FILE = ['hdf5_train.h5', 'hdf5_test.h5']
LIST_FILE = ['list_train.txt', 'list_test.txt']
```
- 1
- 2
- 3
- 1
- 2
- 3
- label配置
```python
LABELS = dict(
    # (kind_1, kind_2)
    A_0 = (0, 0),
    B_0 = (1, 0),
    A_1 = (0, 1),
    B_1 = (1, 1),
    A_2 = (0, 2),
    B_2 = (1, 2),
)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 生成hdf5文件
```python
print '\nplease wait...'
for kk, image_dir in enumerate(IMAGE_DIR):
    # 读取文件列表于file_list
    file_list = ...
    # 文件列表乱序
    random.shuffle(file_list)
    # 标签类别
    kind_index = ...
    # 图片大小为96*32，单通道
    datas = np.zeros((len(file_list), 1, 32, 96))
    # label大小为1*2
    labels = np.zeros((len(file_list), 2))
    for ii, _file in enumerate(file_list):
        # hdf5文件要求数据是float或者double格式
        # 同时caffe中Hdf5DataLayer不允许使用transform_param，
        # 所以要手动除以256
        datas[ii, :, :, :] = \
            np.array(Image.open(_file)).astype(np.float32) / 256
        labels[ii, :] = np.array(LABELS[kind_index ]).astype(np.int)
    # 写入hdf5文件
    with h5py.File(HDF5_FILE[kk], 'w') as f:
        f['data'] = datas
        f['labels'] = labels
        f.close()
    # 写入列表文件，可以有多个hdf5文件
    with open(LIST_FILE[kk], 'w') as f:
        f.write(os.path.abspath(HDF5_FILE[kk]) + '\n')
        f.close()
print '\ndone...'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
![](http://static.blog.csdn.net/images/save_snippets.png)
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 注意： 
- caffe中要求**1个hdf5文件大小不超过2GB**，所以如果数据量太大，建议生成多个hdf5文件
- 我用的5万张图片，大小一共30几兆，生成的hdf5文件是1.8GB
