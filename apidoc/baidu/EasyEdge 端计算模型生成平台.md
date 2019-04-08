# EasyEdge 端计算模型生成平台

## 更新记录

| 时间      | 更新说明                                       |
| :-------- | :--------------------------------------------- |
| 2019.2.21 | 新增鉴权流程                                   |
| 2019.1.10 | 补充支持框架、网络模型的范围，补充针对芯片加速 |
| 2018.12.1 | 文档创建                                       |

## 产品介绍

EasyEdge是百度基于Paddle Mobile研发的端计算模型生成平台，能够帮助深度学习开发者将自建模型快速部署到设备端。只需上传模型，最快2分种即可生成端计算模型并获取SDK。

平台支持详情可参见下表。如需查看具体加速效果，可前往[平台页面](http://ai.baidu.com/easyedge/app)-上传原模型-了解芯片加速。

如需咨询，可加入官方QQ群813291890联系群管。

> 上传模型支持框架：Caffe、PyTorch、TensorFlow、PaddlePaddle

> 上传模型支持网络：VGG16、InceptionV3/V4、MobilenetV1、MobilenetV1-SSD、RestnetV1等13种

> AI芯片加速支持：华为HiSilicon NPU、英特尔Movidius VPU、高通Snapdragon GPU/DSP、苹果A-Bionic

**注**：✔️的数量代表支持加速的芯片种类数

|                 | Caffe | PyTorch | TensorFlow | PaddlePaddle |
| :-------------- | :---: | :-----: | :--------: | :----------: |
| InceptionV4     | ✔️✔️✔️✔️  |         |   ✔️✔️✔️✔️✔️    |      ✔️       |
| InceptionV3     | ✔️✔️✔️✔️✔️ |   ✔️✔️    |   ✔️✔️✔️✔️✔️    |      ✔️       |
| InceptionV2     | ✔️✔️✔️✔️✔️ |         |   ✔️✔️✔️✔️✔️    |      ✔️       |
| InceptionV1     | ✔️✔️✔️✔️✔️ |         |   ✔️✔️✔️✔️✔️    |      ✔️       |
| MobileNetV2     |  ✔️✔️✔️  |         |    ✔️✔️✔️     |      ✔️       |
| MobileNetV1     | ✔️✔️✔️✔️  |         |    ✔️✔️✔️✔️    |      ✔️       |
| ResNet101-V2    | ✔️✔️✔️✔️  |         |     ✔️✔️     |      ✔️       |
| ResNet50-V1     | ✔️✔️✔️✔️✔️ |         |    ✔️✔️✔️     |      ✔️       |
| SqueezeNetV1.1  | ✔️✔️✔️✔️✔️ |         |   ✔️✔️✔️✔️✔️    |      ✔️       |
| VGG19           | ✔️✔️✔️✔️✔️ |  ✔️✔️✔️✔️   |    ✔️✔️✔️✔️    |      ✔️       |
| VGG16           | ✔️✔️✔️✔️✔️ |  ✔️✔️✔️✔️   |    ✔️✔️✔️✔️    |      ✔️       |
| MobileNetV1-SSD |  ✔️✔️✔️  |         |            |      ✔️       |
| VGG16-SSD       |  ✔️✔️✔️  |         |            |      ✔️       |

## 使用流程

进入EasyEdge主页：[ ai.baidu.com/easyedge ](http://ai.baidu.com/easyedge)，点击“立即使用”即可使用平台。

## Step 1 - 上传深度学习模型

在目录【上传原模型】中：

1. 填写模型名称
2. 依次选择模型类型、模型框架、网络模型
3. 依次上传网络参数文件、网络结构文件、模型标签、其他配置（点击左侧划线处可查看上传要求）
4. 自定义分组并描述模型功能
5. 提交上传

![img](https://ai.bdstatic.com/file/DB86A25A56994F83AC0EAE20072490A2)

模型上传成功后，可以在【我的模型】中看到上传的模型。

## Step 2 - 生成端计算模型

成功上传模型后，在目录【生成端模型】中：

1. 选择模型
2. 选择并添加“芯片/硬件”、“操作系统”的组合，支持添加多种组合
3. 如需SDK支持离在线混合，需勾选同意EasyEdge在云端加密部署模型
4. 开始生成模型

![img](https://ai.bdstatic.com/file/4DE69E14D46F4967B0F79088C5E3B168)

在【我的模型】点击模型名称，即可查看端计算模型的生成状态。

## Step 3 - 获取离线SDK

若某版本端计算模型已成功生成，则可点击右侧操作中的“获取SDK”，跳转到【SDK列表】下载Demo体验或正式的SDK

## Step 4 - 激活离线SDK

1. 在【SDK列表】页面下载SDK后，点击“获取序列号激活”，打开【序列号列表】
2. 选择鉴权方式：如需开发APP可选择按产品线鉴权，否则需按设备鉴权
3. 点击“新增序列号”，填写相关信息
4. 用新增的序列号激活之前下载的SDK，参考开发文档：[Android](http://ai.baidu.com/docs#/EasyEdge-Android-SDK/top) | [iOS](http://ai.baidu.com/docs#/EasyEdge-iOS-SDK/top) | [Linux Python](http://ai.baidu.com/docs#/EasyEdge-LinuxPython-SDK/top)| [Linux CPP](http://ai.baidu.com/docs#/EasyEdge-LinuxCPP-SDK/top)

## 高级功能

### 评测模型

目前评测模型功能仅对“图像分类”类型的模型开放，在目录【评测端模型】中：

1. 选择模型和版本
2. 上传测试集 注意 1：同一模型生成的不同版本的端计算模型只能用一个测试集评测，一旦开始评测即不可修改 注意 2：需要将准备好的图片按标签整理在不同的文件夹里，文件夹以标签命名，标签必须与之前上传的模型标签文件完全一致 注意 3：评测集压缩包仅支持zip格式，压缩包大小需要在100M之内，解压后图片数不超过1000张
3. 开始评测模型

![img](https://ai.bdstatic.com/file/6500DBC8B2084F0886819511E130CCA8)

评测完成后可在【我的模型】查看生成端计算模型的详细评测结果。











# Linux Python SDK

# 简介

本文档介绍EasyEdge/EasyDL的Linux Python SDK的使用方法。

- 网络类型支持：图像分类，物体检测
- 硬件支持：
  - Intel Movidius MyRIAD2/MyRIAD X, Neural Compute Stick
  - CPU x86_64
- 操作系统支持
  - Linux
  - Windows (部分)

# Release Notes

| 时间       | 版本  | 说明             |
| :--------- | :---- | :--------------- |
| 2018.11.30 | 1.0.0 | 第一版！         |
| 2019.02.13 | 1.0.5 | paddlepaddle支持 |
| 2019.02.28 | 1.0.6 | 引擎功能完善     |

# 快速开始

## 1. 安装依赖

```
pip install numpy opencv-python requests future
```

根据引擎的不同，SDK依赖了不同的底层引擎。根据所需自行安装。

### （可选）安装paddlepaddle

```
pip install paddlepaddle==1.2.1
```

### （可选）安装NCS依赖

请参考Movidius文档安装ncsdk1.0

> https://movidius.github.io/ncsdk/install.html

请务必安装 ncsdk1.0，非2.0。 也即：`git clone http://github.com/Movidius/ncsdk && cd ncsdk && make install` 移除： -b ncsdk2

无需安装Toolkit，无需安装TensorFlow。

## 2. 安装easyedge python sdk包

```
pip install BaiduAI_EasyEdge_Movidius_SDK-1.0.0-py2.py3-none-any.whl
```

## 测试Demo

修改`demo_classification.py` 对应的模型文件夹和测试图片地址，运行：

```
python3 demo_classification.py
```

`demo_object_detection.py` 同理。

测试效果：

![img](http://aipe-easyedge.cdn.bcebos.com/doc/demo_object_detection_result.png)

# 接口说明

** 使用流程 **

```
pred = edge.Program()
pred.init(model_dir={模型文件夹路径}, device=edge.Device.MOVIDIUS, engine=edge.Engine.NCSDK)
pred.infer_image({numpy.ndarray的图片})
pred.close()
```

## 初始化

- 接口

```
    def init(self,
            model_dir,
            device=Device.LOCAL,
            config_file='conf.json',
            preprocess_file='preprocess_args.json',
            model_file='model',
            params_file='params',
            graph_file='graph.ncsmodel',
            label_file='label_list.txt',

            ):
       """
       Args:
           device: Device.LOCAL | Device.MOVIDIUS
           model_dir: str
               model dir
           preprocess_file: str
           model_file: str
           params_file: str
           graph_file: str
           label_file: str

       Raises:
           RuntimeError, IOError
       Returns:
           bool: True if success

       """
```

## 预测图像

- 接口

```
    def infer_image(self, img):
       """

       Args:
           img: np.ndarray
               must be BGR with HWC (opencv default)

       Returns:
           list

       """
```

- 返回格式: `[dict1, dict2, ...]`

图像分类

```
{
    "index": 736,
    "label": "'n03982430 pool table, billiard table, snooker table'",
    "confidence": 0.0
}
```

物体检测

```
{
    "y2": 0.91211,
    "label": "8: cat",
    "confidence": 1.0,
    "x2": 0.91504,
    "index": 8,
    "y1": 0.12671,
    "x1": 0.21289
}
```





# FAQ

**如以下回答未能解决您遇到的问题，欢迎加入官方QQ群813291890联系群管。**

### Q：什么是端计算模型？我是否需要使用这个平台？

A：端计算模型是能够在设备端上进行计算的模型。如果你已经有一个效果满意的深度学习模型，并希望快速把模型集成到设备端上，就可以使用EasyEdge快速生成端计算模型，并获取相应的SDK。

### Q：已经生成的端计算模型可以重新生成吗？

A：对同一个深度学习模型，同一“芯片/硬件”与“操作系统”组合的端计算模型只支持生成一次。

### Q：评测集上传失败怎么办？

A：1. 检查标签是否与之前上传的模型标签文件完全一致；2. 检查压缩包大小是否超过100M、图片总量是否超过1000张；3. 检查图片是否有损坏。

### Q：离在线混合版本的SDK支持多少次在线调用？

A：目前默认支持50,000次/天的在线调用。



