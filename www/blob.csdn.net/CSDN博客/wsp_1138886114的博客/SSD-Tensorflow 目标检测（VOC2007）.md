# SSD-Tensorflow 目标检测（VOC2007） - wsp_1138886114的博客 - CSDN博客





2018年12月13日 21:10:00[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：944











### 文章目录
- [一、准备--模型与数据集](#_1)
- [二、训练--加载模型训练](#_42)
- [三、预测--加载模型对图片进行测试](#_73)




### 一、准备–模型与数据集

**第一步**：下载代码与数据集
- 下载SSD框架本地，解压。SSD源码下载: [https://github.com/balancap/SSD-Tensorflow](https://github.com/balancap/SSD-Tensorflow)
- 下载数据集 [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/)
- 其他数据集下载：
[http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar) (可能失效)
[http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar)
[http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213195157610.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
**第二步**：在解压出来的主目录下依次创建`tfrecords_、train_model`文件夹
- `tfrecords_` : 用来存储tensorflow特定格式的训练数据
- `train_model` : 用来存储训练模型记录与参数
- 将下载的数据集解压放置工程目录下，文件目录如图所示：

（SegmentationClass与SegmentationObjec这两个文件夹用来做图像分割的可以不放进来）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213191819787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
**第三步**：生成`.tfrecords`文件
- 修改67行SAMPLES_PER_FILES的个数；（一个`.tfrecords`文件包含几个`.xml`文件）

修改83行读取方式为`'rb'`；(若不修改会报错)
- 打开`tf_convert_data.py`文件，

依次点击：`run` , `Edit Configuration`，在面板中依次执行如下：
**3** Name:`tf_convert_data.py`
**4** Script path :`your_filepath`最好是绝对路径
**6** 默认即可，当然如果你有GPU可以更改为使用GPU
**7** 即当前工程运行环境，可在setting里面修改
**5** 在`Parameters`中填入以下内容： 设置好点击Apply,OK。
```
--dataset_name=pascalvoc
--dataset_dir=./VOC2007/
--output_name=voc_2007_train
--output_dir=./tfrecords_
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213195116691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
- 运行`tf_convert_data.py`文件，在面板中得到成功信息，可以在tfrecords_文件夹下看到生成的`.tfrecords`文件；截图如下：

**第四步**：修改训练步数epoch——打开`train_ssd_network.py`文件
- 修改154行训练步数，None会无限训练下去；

### 二、训练–加载模型训练

**第五步**: 我用的是vgg_16模型——[下载请点击](https://pan.baidu.com/s/1q2LAjwp2IIGSVkP56HPiJg)，提取码：e9n8。下载完成解压后存入checkpoint文件中；

重新训练模型——打开`train_ssd_network.py`文件，依次点击：run、Edit Configuration，在Parameters中填入以下内容，再运行train_ssd_network.py文件

```python
"""
若是从头开始训练，注释掉如下三项

--checkpoint_path              不提供初始化模型,让模型自己随机初始化权重，从头训练
--checkpoint_exclude_scopes    从头开始训练不需要此项
--trainable_scopes             从头开始训练不需要此项
"""

--train_dir=./train_model/     # 训练生成模型的存放路径
--dataset_dir=./tfrecords_/    # 数据存放路径
--dataset_name=pascalvoc_2007
--dataset_split_name=train
--model_name=ssd_300_vgg       # 加载的模型的名字
--checkpoint_path=./checkpoints/vgg_16.ckpt  #加载预训练模型的路径
--checkpoint_model_scope=vgg_16 #所加载模型里面的作用域名
--checkpoint_exclude_scopes=ssd_300_vgg/conv6,ssd_300_vgg/conv7,ssd_300_vgg/block8,ssd_300_vgg/block9,ssd_300_vgg/block10,ssd_300_vgg/block11,ssd_300_vgg/block4_box,ssd_300_vgg/block7_box,ssd_300_vgg/block8_box,ssd_300_vgg/block9_box,ssd_300_vgg/block10_box,ssd_300_vgg/block11_box
--trainable_scopes=ssd_300_vgg/conv6,ssd_300_vgg/conv7,ssd_300_vgg/block8,ssd_300_vgg/block9,ssd_300_vgg/block10,ssd_300_vgg/block11,ssd_300_vgg/block4_box,ssd_300_vgg/block7_box,ssd_300_vgg/block8_box,ssd_300_vgg/block9_box,ssd_300_vgg/block10_box,ssd_300_vgg/block11_box
--save_summaries_secs=60      # 每60s保存一下日志
--save_interval_secs=600      # 每600s保存一下模型
--weight_decay=0.0005         # 正则化的权值衰减的系数
--optimizer=adam              # 选取的最优化函数
--learning_rate=0.001         # 学习率
--learning_rate_decay_factor=0.94   # 学习率的衰减因子
--batch_size=24                     # 可以小一点，不然可能会报错（显存不够用）
--gpu_memory_fraction=0.9           # 指定占用gpu内存的百分比
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213210942135.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 三、预测–加载模型对图片进行测试
- 
**第一** 打开checkpoints文件，解压缩ssd_300_vgg.ckpt.zip文件到checkpoints目录下面。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215221347960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
**第二** 打开`notebooks`的`ssd_tests.ipyb`文件（使用Jupyter或者ipython)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215221427713.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

进入http://localhost:8892/tree服务器，打开 ssd_notebook.ipynb.ipynb文件- 
**第三** 配置一些路径：第4个cell中`from notebooks import visualization`改成`import notebooks`，不改的话目录结构不对，会报错。

- 
**第四** 测试图片：改变path = '…/demo/'成自己的图片所在路径（或者将自己准备的图片放入）

- 
**第五** 从头到尾运行所有cell即可。

- 
最后一个 cell 代码如下：


```python
# Test on some demo image and visualize output.
path = '../demo/'
image_names = sorted(os.listdir(path))                    #获取测试文件夹所有图片

img = mpimg.imread(path + image_names[-5])                 #测试倒数第五张图片
rclasses, rscores, rbboxes =  process_image(img)

#visualization.bboxes_draw_on_img(img, rclasses, rscores, rbboxes, visualization.colors_plasma)
visualization.plt_bboxes(img, rclasses, rscores, rbboxes) #展示倒数第五张图片效果
```

当然可以使用循环测试一批图片，代码改为下：

```python
# Test on some demo image and visualize output.
path = '../demo/'
image_names = sorted(os.listdir(path))         # 获取测试文件夹所有图片

i = 0
for it  in image_names:
    img = mpimg.imread(path + it)
    i+=1
    if i>4: break                               # 测试4张图片
    rclasses, rscores, rbboxes =  process_image(img)
    visualization.plt_bboxes(img, rclasses, rscores, rbboxes)
```

效果展示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215224614742.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
为了便于管理，我在Anaconda中创建了额外四个运行环境（如上图），但是也带来了一系列的麻烦，在这次测试中出现了一系列问题：

若是各位遇到下列问题，正好给大家一个参考，若是各位没有遇到，略过。
- 
**报错1**：在虚拟环境中的jupyter notebook启动后无法导入tensorflow
**解决**：激活当前环境，执行`conda install jupyter`[详情请点击](http://www.cnblogs.com/jiangxinyang/p/9606494.html)

- 
**报错2**： No module named matplotlib
**解决**：激活当前环境，执行`conda install matplotlib`

- 
**报错3**： No module named PIL
**解决**：激活当前环境，执行`pip install Pillow`

- 
**报错4**： name ‘visualization’ is not defined
**解决**：`import visualization`












