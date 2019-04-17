# Facebook开源MaskR-CNN的PyTorch1.0基准，比mmdetection快且省内存 - 知乎
# 



选自GitHub，机器之心编辑，参与：刘晓坤。

> 近日，Facebook AI Research 开源了 Faster R-CNN 和 Mask R-CNN 的 [PyTorch 1.0](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749442%26idx%3D1%26sn%3Dca773e5a904e4a024e52ae6b998cf97f%26chksm%3D871afe7cb06d776a52a4f8ace7f39cf3130bb2f7a4e256e7acf18bba8b06f9ab02ead7fb085c%26scene%3D21%23wechat_redirect) 实现基准：MaskRCNN-Benchmark。相比 Detectron 和 mmdetection，MaskRCNN-Benchmark 的性能相当，并拥有更快的训练速度和更低的 GPU 内存占用。

项目地址：[https://github.com/facebookresearch/maskrcnn-benchmark](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/maskrcnn-benchmark)
![](https://pic1.zhimg.com/v2-26ccf702633d0baa0d857582d7a52efc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='913' height='687'></svg>)MaskRCNN-Benchmark 目标检测示例
## **Detectron 和 mmdetection**

[Detectron](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650736547%26idx%3D3%26sn%3Da326f17f832cec4827596291930e76dd%26chksm%3D871ac3ddb06d4acb3ffc56ffd56b340d5f15b7180a8aff5a38ea41c3c4e41e8d96da4c5a0a08%26scene%3D21%23wechat_redirect) 是 Facebook AI Research 实现顶尖目标检测算法（包括 Mask R-CNN）的软件系统。该系统是基于 Python 和深度学习框架 Caffe 2 而构建的。Detectron 目前包含以下目标检测算法的实现：Mask R-CNN 、RetinaNet、Faster R-CNN、RPN、Fast R-CNN、R-FCN。

[mmdetection](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750181%26idx%3D2%26sn%3D227bdb9e5166f0b44b478b41a77bbcd5%26chksm%3D871af89bb06d718ddecc91d6924ce25377e13c9004e71de4a78cab67b6deedb73e939aab807b%26scene%3D21%23wechat_redirect) 是商汤和港中文近日联合开源的基于 PyTorch 的开源目标检测工具包。该工具包支持 Mask R-CNN 等多种流行的检测框架，读者可在 PyTorch 环境下测试不同的预训练模型及训练新的检测分割模型。和 Detectron 对比，mmdetection 的性能稍高、训练速度稍快、所需显存稍小。

mmdetection 第一个版本中实现了 RPN、Fast R-CNN、Faster R-CNN、Mask R-CNN，近期还计划放出 RetinaNet 和 Cascade R-CNN。但更重要的是，基于 PyTorch 和基于 Caffe2 的 code 相比，易用性是有代差的。成功安装 Detectron 的时间，大概可以装好一打的 mmdetection。

## **MaskRCNN-Benchmark 项目亮点：**
- PyTorch 1.0：相当或者超越 Detectron 准确率的 RPN、Faster R-CNN、Mask R-CNN 实现；
- 非常快：训练速度是 Detectron 的两倍，是 mmdection 的 1.3 倍。
- 节省内存：在训练过程中使用的 GPU 内存比 mmdetection 少大约 500MB；
- 使用多 GPU 训练和推理；
- 批量化推理：可以在每 GPU 每批量上使用多张图像进行推理；
- 支持 CPU 推理：可以在推理时间内于 CPU 上运行。
- 提供几乎所有参考 Mask R-CNN 和 Faster R-CNN 配置的预训练模型，具有 1x 的 schedule。

## **MaskRCNN-Benchmark Model Zoo 基线模型性能数据**

地址：[https://github.com/facebookresearch/maskrcnn-benchmark/blob/master/MODEL_ZOO.md](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/maskrcnn-benchmark/blob/master/MODEL_ZOO.md)

**硬件**

8 NVIDIA V100 GPUs

**软件：**
- PyTorch version: 1.0.0a0+dd2c487
- CUDA 9.2
- CUDNN 7.1
- NCCL 2.2.13-1

**端到端 Mask R-CNN 和 Faster R-CNN 基线模型**

所有的基线模型都使用了和 Detectron 相同的实验设置，检测模型权重使用 Caffe2 中的 ImageNet 权重初始化，这和 Detectron 是一样的。预训练模型通过下表中的 model id 链接获取。
![](https://pic4.zhimg.com/v2-82e0cc88b63a03d0dcffe3c5f9a4799f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='645'></svg>)
## **和 Detectron、mmdetection 的性能对比**

**训练速度**

下表中的数据单位是秒/迭代，越低越好。（mmdetection 中备注的硬件和 maskrcnn_benchmark 是不同的）
![](https://pic3.zhimg.com/v2-d9c7d14cf4d5c43bce687f362099d62a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='923' height='380'></svg>)
**训练内存（越低越好）**
![](https://pic4.zhimg.com/v2-fc4a426fd21db31aafb24a62c5f2f39b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='925' height='371'></svg>)
**推理准确率（越高越好）**
![](https://pic1.zhimg.com/v2-2179260cdb8e6513833f87517bb01ef8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='924' height='370'></svg>)
## **Webcam 和 Jupyter notebook demo**

该项目提供了一个简单的 webcam demo，展示如何使用 maskrcnn_benchmark 进行推理：


```
cd demo# by default, it runs on the GPU# for best results, use min-image-size 800
python webcam.py --min-image-size 800# can also run it on the CPU
python webcam.py --min-image-size 300 MODEL.DEVICE cpu# or change the model that you want to use
python webcam.py --config-file ../configs/caffe2/e2e_mask_rcnn_R_101_FPN_1x_caffe2.py --min-image-size 300 MODEL.DEVICE cpu# in order to see the probability heatmaps, pass --show-mask-heatmaps
python webcam.py --min-image-size 300 --show-mask-heatmaps MODEL.DEVICE cpu
```


**安装**

教程地址：[https://github.com/facebookresearch/maskrcnn-benchmark/blob/master/INSTALL.md](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/maskrcnn-benchmark/blob/master/INSTALL.md)




**安装要求：**
- PyTorch 1.0 的每日测试版本，安装说明：[https://pytorch.org/get-started/locally/](https://link.zhihu.com/?target=https%3A//pytorch.org/get-started/locally/)
- torchvision
- cocoapi
- yacs
- （可选）OpenCV（用于 webcam demo）


```
# maskrnn_benchmark and coco api dependencies
pip install ninja yacs cython
# follow PyTorch installation in https://pytorch.org/get-started/locally/# we give the instructions for CUDA 9.0
conda install pytorch-nightly -c pytorch
# install torchvisioncd ~/github
git clone git@github.com:pytorch/vision.gitcd vision
python setup.py install
# install pycocotoolscd ~/github
git clone git@github.com:cocodataset/cocoapi.gitcd cocoapi/PythonAPI
python setup.py build_ext install
# install PyTorch Detectioncd ~/github
git clone git@github.com:facebookresearch/maskrcnn-benchmark.gitcd maskrcnn-benchmark# the following will install the lib with# symbolic links, so that you can modify# the files if you want and won't need to# re-build it
python setup.py build develop
```


## **通过几行代码进行推理**

该项目提供了一个 helper 类来简化编写使用预训练模型进行推理的流程，只要在 demo 文件夹下运行以下代码：


```
from maskrcnn_benchmark.config import cfgfrom predictor import COCODemo

config_file = "../configs/caffe2/e2e_mask_rcnn_R_50_FPN_1x_caffe2.yaml"# update the config options with the config file
cfg.merge_from_file(config_file)# manual override some options
cfg.merge_from_list(["MODEL.DEVICE", "cpu"])

coco_demo = COCODemo(
    cfg,
    min_image_size=800,
    confidence_threshold=0.7,
)# load image and then run prediction
image = ...
predictions = coco_demo.run_on_opencv_image(image)
```


## **在 COCO 数据集上执行训练**

为了运行以下示例，你首先需要安装 maskrcnn_benchmark。你还需要下载 COCO 数据集，推荐按以下方式符号链接 COCO 数据集的路径到 datasets/。我们使用来自 Detectron 的 GitHub 的 minival 和 valminusminival 集合。


```
# symlink the coco datasetcd ~/github/maskrcnn-benchmark
mkdir -p datasets/coco
ln -s /path_to_coco_dataset/annotations datasets/coco/annotations
ln -s /path_to_coco_dataset/train2014 datasets/coco/train2014
ln -s /path_to_coco_dataset/test2014 datasets/coco/test2014
ln -s /path_to_coco_dataset/val2014 datasets/coco/val2014
```


你也可以配置你自己的到数据集的路径。为此，你需要让 maskrcnn_benchmark/config/paths_catalog.py 指向你的数据集保存的位置。你也可以创建一个新的 paths_catalog.py 文件，其实现了相同的两个类，并在训练过程中将它作为一个配置参数 PATHS_CATALOG 传递。

**单 GPU 训练**

`python /path_to_maskrnn_benchmark/tools/train_net.py --config-file "/path/to/config/file.yaml"`

**多 GPU 训练**

该项目使用内部的 torch.distributed.launch 以启动多 GPU 训练。这个来自 PyTorch 的效用函数可以产生我们想要使用 GPU 数目的 Python 进程，并且每个 Python 进程只需要使用一个 GPU。


```
export NGPUS=8
python -m torch.distributed.launch --nproc_per_node=$NGPUS /path_to_maskrcnn_benchmark/tools/train_net.py --
```


## **添加你自己的数据集**

该项目添加了对 COCO 类型数据集的支持，为在新数据集上训练添加支持可以通过以下方法实现：


```
from maskrcnn_benchmark.structures.bounding_box import BoxList
class MyDataset(object):
    def __init__(self, ...):
        # as you would do normallydef __getitem__(self, idx):
        # load the image as a PIL Image
        image = ...# load the bounding boxes as a list of list of boxes# in this case, for illustrative purposes, we use# x1, y1, x2, y2 order.
        boxes = [[0, 0, 10, 10], [10, 20, 50, 50]]
        # and labels
        labels = torch.tensor([10, 20])

        # create a BoxList from the boxes
        boxlist = Boxlist(boxes, size=image.size, mode="xyxy")
        # add the labels to the boxlist
        boxlist.add_field("labels", labels)

        if self.transforms:
            image, boxlist = self.transforms(image, boxlist)

        # return the image, the boxlist and the idx in your datasetreturn image, boxlist, idx

    def get_img_info(self, idx):
        # get img_height and img_width. This is used if# we want to split the batches according to the asp
```


就这样。你可以添加额外的字段到 boxlist，例如 segmentation masks（使用 structures.segmentation_mask.SegmentationMask），或甚至是你自己的实例类型。如果想了解 COCO 数据集实现的完整过程，可以查看：[https://github.com/facebookresearch/maskrcnn-benchmark/blob/master/maskrcnn_benchmark/data/datasets/coco.py](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/maskrcnn-benchmark/blob/master/maskrcnn_benchmark/data/datasets/coco.py)


