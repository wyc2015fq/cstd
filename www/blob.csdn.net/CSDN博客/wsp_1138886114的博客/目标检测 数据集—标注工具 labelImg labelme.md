# 目标检测 数据集—标注工具 labelImg/labelme - wsp_1138886114的博客 - CSDN博客





2018年12月15日 20:21:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1008











### 文章目录
- [一、使用 LabelImg生成目标检测数据集](#_LabelImg_5)
- [1.1、下载LabelImg](#11LabelImg_6)
- [1.2、配置环境](#12_9)
- [1.3、开始标注](#13_17)
- [二、使用 labelme生成mask掩码数据集](#_labelmemask_23)
- [2.1 安装与启动](#21__24)
- [2.2 标注与保存](#22__39)
- [2.3 实现labelme批量json_to_dataset方法](#23_labelmejson_to_dataset_52)



目标检测中，我们需要对原始图片数据集做标注，告诉机器我们需要的目标，然后让机器去学习，这个过程是非常重要的。我之前做目标检测时都是直接下载公开的数据集，比赛的数据集，因为简单方便。

如今自己需要另外训练自己的数据集，标准肯定少不了。LabelImg 这个工具 它的作用是在原始图像中标注目标物体位置并对每张图片生成相应的xml文件表示目标标准框的位置。

开发环境 `windows10 | Anaconda | python ==3.7.X`


### 一、使用 LabelImg生成目标检测数据集

##### 1.1、下载LabelImg

下载地址：[https://github.com/tzutalin/labelImg](https://github.com/tzutalin/labelImg) 下载，解压，得到LabelImg-master文件。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202454765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
##### 1.2、配置环境

终端进入LabelImg-master 目录下 ，依次执行如下命令即可

```python
conda install pyqt = 5
pyrcc5 -o resources.py resources.qrc
python labelImg.py
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215195839817.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

##### 1.3、开始标注

先查看图片，查看你需要标注哪几个位置，命好这几类名字（之后不能改）

Open Dir(图片目录)>> Change Save Dir(标注文件目录：最好与图片目录相同) >>Create \nRectBox(创建标注)>>选择类名>>Save(保存)>>Next Image(下一张)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202744150.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

为了加快标注，你可以进入 labelImg-master\data下，用Notepad++打开predefined_classes.txt文件，修改为刚才命好这几类名字。配合快捷键，一个小时大约标注100-150张（以我这个为例），这个时候你会感觉到想吐，这很正常，习惯就好，祝你好运！！！
### 二、使用 labelme生成mask掩码数据集

##### 2.1 安装与启动

mask数据集标注工具(labelme)： [https://github.com/wkentaro/labelme](https://github.com/wkentaro/labelme)

下载安装 [Anaconda](https://www.anaconda.com/download/)，然后在`prompt`里依次输入如下指令：

```python
conda create --name=labelme python=3.7（python=自己的Python版本）
activate labelme
conda install pyqt
pip install labelme
```

启动labelme的conda环境，即按照以下步骤启动:

```python
activate labelme
labelme
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106182931486.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

##### 2.2 标注与保存

点击`Create Polygons`创建目标，创建完点击`save`保存.json文件(弹窗选择保存位置)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106182615150.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)在画掩码过程中如有多个box、fruit… 命名规则为box1、box2…fruit1、fruit2…。最后生成的标签为一个label.png文件，这个文件只有一通道，在你标注时同一标签mask会被给予一个标签位，而mask要求不同的实例要放在不同的层中。最终训练索要得到的输入为一个 w *h*n 的ndarray，其中n为该图片中实例的个数。总而言之，画mask时就按照上述命名规则就好了，具体的过程已经在上述代码中实现。如图：
所有文件标注完，启动后找到目标文件夹（不要有中文路径及文件夹名），会报编码相关的错。运行如下代码：

```
labelme_json_to_dataset <文件名>.json
```

即可得到一个文件夹，有四个文件，`*.png`, `info.yaml` , `label.png`, `label_viz.png`。 其中label.png即是我们要的label_data。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106184426607.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106185128433.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
##### 2.3 实现labelme批量json_to_dataset方法

在C:\Anaconda3\envs\labelme\Lib\site-packages\labelme\cli（取决于你的安装环境）目录下`json_to_dataset.py`文件，这里面提供将json转成dataset的代码，所以我们只需要在这个基础上更改即可。

修改代码如下：

```

```

待续







