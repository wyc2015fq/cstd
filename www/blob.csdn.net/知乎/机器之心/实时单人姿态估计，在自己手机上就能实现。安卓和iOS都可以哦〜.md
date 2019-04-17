# 实时单人姿态估计，在自己手机上就能实现。安卓和iOS都可以哦〜 - 知乎
# 



> 本文介绍了如何使用[TensorFlow](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756338%26idx%3D4%26sn%3D026269e3e7b79f7b1595dd3f294228e2%26chksm%3D871a908cb06d199a1347190e24a78f506e01d358f0005ad11b1664f03fbd29920bf2652ee197%26token%3D1549755692%26lang%3Dzh_CN)在智能机上（包括安卓和iOS设备）执行实时单人姿态估计。

GitHub地址：https：[//github.com/edvardHua/PoseEstimationForMobile](https://link.zhihu.com/?target=//github.com/edvardHua/PoseEstimationForMobile)

该repo使用TensorFlow实现CPM和Hourglass模型。这里未使用常规的卷积，而是在模型内部使用了反向卷积（又叫Mobilenet V2），以便执行实时推断。
![](https://pic2.zhimg.com/v2-c4cee09adbeb51d7dc12850a63afb5c9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='558' height='122'></svg>)
注：你可以修改网络架构，来训练更高PCKh的模型。架构地址：https：[//github.com/edvardHua/PoseEstimationForMobile/tree/master/training/src](https://link.zhihu.com/?target=//github.com/edvardHua/PoseEstimationForMobile/tree/master/training/src)

该库包含：
- 训练CPM和Hourglass模型的代码;
- 安卓demo的源代码;
- IOS demo的源代码。

下面的gif是在Mi Mix2s上截取的（~60 FPS）
![](https://pic1.zhimg.com/v2-ef5fa937c16958776fa66fba4f61cd2c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='179' height='356'></svg>)
你可以下载以下apk，在自己的设备上进行测试。
![](https://pic4.zhimg.com/v2-cb3d46de9475ae6a5e08cdc33a4584eb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='473' height='84'></svg>)- PoseEstimation-Mace.apk：HTTPS：[//raw.githubusercontent.com/edvardHua/PoseEstimationForMobile/master/release/PoseEstimation-Mace.apk](https://link.zhihu.com/?target=//raw.githubusercontent.com/edvardHua/PoseEstimationForMobile/master/release/PoseEstimation-Mace.apk)
- PoseEstimation-TFlite.apk：HTTPS：[//raw.githubusercontent.com/edvardHua/PoseEstimationForMobile/master/release/PoseEstimation-TFlite.apk](https://link.zhihu.com/?target=//raw.githubusercontent.com/edvardHua/PoseEstimationForMobile/master/release/PoseEstimation-TFlite.apk)


## **训练**

**依赖项**
- Python3
- TensorFlow> = 1.4
- 锤

**数据集**

训练数据集：HTTPS：//[http://drive.google.com/open](https://link.zhihu.com/?target=http%3A//drive.google.com/open) ID = 1zahjQWhuKIYWRRI2ZlHzn65Ug_jIiC4l

将其解压缩，获取以下文件结构：


```
# root @ ubuntu in ~/hdd/ai_challenger
$ tree -L 1 ..
├── ai_challenger_train.json
├── ai_challenger_valid.json
├── train
└── valid
```


该训练数据集仅包含单人图像，数据来源是AI Challenger竞赛。共包含22446个训练样本和1500个测试样本。

该repo作者使用tf-pose-estimation库中的数据增强代码将标注迁移为COCO格式.tf-pose-estimate库：https：[//github.com/ildoonet/tf-pose-estimation](https://link.zhihu.com/?target=//github.com/ildoonet/tf-pose-estimation)

**超参数**

训练步骤中，使用实验文件夹中的cfg文件传输超参数。

以下是mv2_cpm.cfg文件的内容：


```
[Train]
model: 'mv2_cpm'
checkpoint: False
datapath: '/root/hdd/ai_challenger'
imgpath: '/root/hdd/'
visible_devices: '0, 1, 2'
multiprocessing_num: 8
max_epoch: 1000
lr: '0.001'
batchsize: 5
decay_rate: 0.95
input_width: 192
input_height: 192
n_kpoints: 14
scale: 2
modelpath: '/root/hdd/trained/mv2_cpm/models'
logpath: '/root/hdd/trained/mv2_cpm/log'
num_train_samples: 20000
per_update_tensorboard_step: 500
per_saved_model_step: 2000
pred_image_on_tensorboard: True
```


该cfg文件覆盖模型的所有参数，在network_mv2_cpm.py中仍有一些参数。

**使用nvidia-docker训练**

通过以下命令构建docker：


```
cd training/docker
docker build -t single-pose .
```


或者

`docker pull edvardhua/single-pose`

然后运行以下命令，训练模型：


```
nvidia-docker run -it -d \
-v <dataset_path>:/data5 -v <training_code_path>/training:/workspace \
-p 6006:6006 -e LOG_PATH=/root/hdd/trained/mv2_cpm/log \
-e PARAMETERS_FILE=experiments/mv2_cpm.cfg edvardhua/single-pose
```


此外，它还在端口6006上创建了tensorboard。确保安装了nvidia-docker。

**按一般方法训练**

1.安装依赖项：


```
cd training
pip3 install -r requirements.txt
```


还需要安装cocoapi（[https://github.com/cocodataset/cocoapi](https://link.zhihu.com/?target=https%3A//github.com/cocodataset/cocoapi)）。

2.编辑实验文件夹中的参数文件，它包含几乎所有超参数和训练中需要定义的其他配置。之后，传输参数文件，开始训练：


```
cd training
python3 src/train.py experiments/mv2_cpm.cfg
```


在3张英伟达1080Ti显卡上经过12个小时的训练后，该模型几乎收敛。以下是对应的张量板图。
![](https://pic2.zhimg.com/v2-d127f78e1bedfdc5fd9551c941cab0d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='238'></svg>)
**基准（PCKh）**

运行以下命令，评估PCKh值。


```
python3 src/benchmark.py --frozen_pb_path=hourglass/model-360000.pb \
--anno_json_path=/root/hdd/ai_challenger/ai_challenger_valid.json \
--img_path=/root/hdd \
--output_node_name=hourglass_out_3
```


**预训练模型**
- CPM：HTTPS：[//github.com/edvardHua/PoseEstimationForMobile/tree/master/release/cpm_model](https://link.zhihu.com/?target=//github.com/edvardHua/PoseEstimationForMobile/tree/master/release/cpm_model)
- 沙漏：HTTPS：[//github.com/edvardHua/PoseEstimationForMobile/tree/master/release/hourglass_model](https://link.zhihu.com/?target=//github.com/edvardHua/PoseEstimationForMobile/tree/master/release/hourglass_model)

**安卓演示**

由于mace框架，你可以使用GPU在安卓智能机上运行该模型。

按照以下命令将模型转换为mace格式：


```
cd <your-mace-path># You transer hourglass or cpm model by changing `yml` file.
python tools/converter.py convert --config=<PoseEstimationForMobilePath>/release/mace_ymls/cpm.yml
```


然后根据mace文档的说明，将模型集成到安卓设备中。

至于如何调用模型，解析输出，可以参见安卓源代码：HTTPS：[//github.com/edvardHua/PoseEstimationForMobile/tree/master/android_demo](https://link.zhihu.com/?target=//github.com/edvardHua/PoseEstimationForMobile/tree/master/android_demo)。

一些芯片的平均推断时间基准如下所示：
![](https://pic1.zhimg.com/v2-afa8377818ce6b3e3aadbd2095460e74_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='741' height='86'></svg>)
以下是该回购作者构建该演的环境：
- 操作系统：macOS 10.13.6（mace目前不支持windows）
- Android Studio：3.0.1
- NDK版本：r16

在构建mace-demo时，不同环境可能会遇到不同的错误。为避免这种情况，作者建议使用docker。


```
docker pull registry.cn-hangzhou.aliyuncs.com/xiaomimace/mace-dev-lite

docker run -it
 --privileged -d --name mace-dev 
 --net=host 
 -v to/you/path/PoseEstimationForMobile/android_demo/demo_mace:/demo_mace 
 registry.cn-hangzhou.aliyuncs.com/xiaomimace/mace-dev-lite

docker run -it --privileged -d --name mace-dev --net=host \
 -v to/you/path/PoseEstimationForMobile/android_demo/demo_mace:/demo_mace \
 registry.cn-hangzhou.aliyuncs.com/xiaomimace/mace-dev-lite
# Enter to docker
docker exec -it mace-dev bash
# Exec command inside the dockercd /demo_mace && ./gradlew build
```


或者将模型转换为tflite：


```
# Convert to frozen pb.cd training
python3 src/gen_frozen_pb.py \
--checkpoint=<you_training_model_path>/model-xxx --output_graph=<you_output_model_path>/model-xxx.pb \
--size=192 --model=mv2_cpm_2
# If you update tensorflow to 1.9, run following command.
python3 src/gen_tflite_coreml.py \
--frozen_pb=forzen_graph.pb \
--input_node_name='image' \
--output_node_name='Convolutional_Pose_Machine/stage_5_out' \
--output_path='./' \
--type=tflite
 # Convert to tflite.# See https://github.com/tensorflow/tensorflow/blob/master/tensorflow/docs_src/mobile/tflite/devguide.md for more information.
bazel-bin/tensorflow/contrib/lite/toco/toco \
--input_file=<you_output_model_path>/model-xxx.pb \
--output_file=<you_output_tflite_model_path>/mv2-cpm.tflite \
--input_format=TENSORFLOW_GRAPHDEF --output_format=TFLITE \
--inference_type=FLOAT \
--input_shape="1,192,192,3" \
--input_array='image' \
--output_array='Convolutional_Pose_Machine/stage_5_out'
```


然后，将tflite文件放在android_demo / app / src / main / assets中，修改ImageClassifierFloatInception.kt中的参数。


```
............// parameters need to modify in ImageClassifierFloatInception.kt/**     * Create ImageClassifierFloatInception instance     *     * @param imageSizeX Get the image size along the x axis.     * @param imageSizeY Get the image size along the y axis.     * @param outputW The output width of model     * @param outputH The output height of model     * @param modelPath Get the name of the model file stored in Assets.     * @param numBytesPerChannel Get the number of bytes that is used to store a single     * color channel value.     */
    fun create(
      activity: Activity,
      imageSizeX: Int = 192,
      imageSizeY: Int = 192,
      outputW: Int = 96,
      outputH: Int = 96,
      modelPath: String = "mv2-cpm.tflite",
      numBytesPerChannel: Int = 4
    ): ImageClassifierFloatInception =
      ImageClassifierFloatInception(
          activity,
          imageSizeX,
          imageSizeY,
          outputW,
          outputH,
          modelPath,
          numBytesPerChannel)............
```


最后，将该项目导入Android Studio，在智能机设备上运行。

**iOS演示**

首先，将模型转换为CoreML模型：


```
# Convert to frozen pb.cd training
python3 src/gen_frozen_pb.py \
--checkpoint=<you_training_model_path>/model-xxx --output_graph=<you_output_model_path>/model-xxx.pb \
--size=192 --model=mv2_cpm_2
# Run the following command to get mlmodel
python3 src/gen_tflite_coreml.py \
--frozen_pb=forzen_graph.pb \
--input_node_name='image' \
--output_node_name='Convolutional_Pose_Machine/stage_5_out' \
--output_path='./' \
--type=coreml
```


然后，按照PoseEstimation-CoreML中的说明来操作（[https://github.com/tucan9389/PoseEstimation-CoreML](https://link.zhihu.com/?target=https%3A//github.com/tucan9389/PoseEstimation-CoreML)）。


