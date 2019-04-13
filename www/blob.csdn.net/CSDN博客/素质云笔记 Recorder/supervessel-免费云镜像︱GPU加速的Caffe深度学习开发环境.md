
# supervessel-免费云镜像︱GPU加速的Caffe深度学习开发环境 - 素质云笔记-Recorder... - CSDN博客

2016年12月30日 11:18:43[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2247


# 开发环境介绍

在SuperVessel云上，我们为大家免费提供当前火热的caffe深度学习开发环境。SuperVessel的Caffe有如下优点：
1）	免去了繁琐的Caffe环境的安装配置，即申请即使用。
2）	集成了SuperVessel先进的GPU虚拟化技术，POWER8，GPU与cuDNN库三重加速的Caffe，极大的节约您的模型训练时间。
3）	环境集成了一些优秀的Caffe开源模型，如图片识别与人脸识别模型，帮助您更快的学习理解Caffe，助力您搭建有趣的深度学习应用。


Caffe深度学习开发环境为您提供：
1）	Ubuntu 14.04 PPC64 Little Endian操作系统
2）	CUDA ToolKit 7.0全套开发环境，包括cuDNN深度学习库的支持
3）	Caffe 2015.8.3更新版本，支持GPU加速，支持cuDNN加速，支持python接口（pycaffe）
4） 模型bvlc_reference_caffenet和人脸识别模型cnn_age_gender_models_and_data
5）	Caffe简单的模型训练数据
6）	OpenCV 2.4.9，并支持python接口（python-opencv）
7）	Atlas 3.10.2
8）	Boost 1.57
9）	其他Caffe依赖的软件包



# 启动预装的开发环境镜像


1） 在 SuperVessel 云的管理界面 的上方选择 "更多服务" -> "计算服务" -> "预装镜像服务"，或直接进入镜像市场：
![Gpu caffe 4.png](https://services.ptopenlab.com/mediawiki/images/0/0f/Gpu_caffe_4.png)
2） 在镜像市场中，选择 "GPU加速的Caffe深度学习开发环境"
![Gpu caffe 3.png](https://services.ptopenlab.com/mediawiki/images/thumb/2/28/Gpu_caffe_3.png/1000px-Gpu_caffe_3.png)
3） 选择 "启动" 按钮


# 远
# 程登录开发环境镜像
1） 登录开发环境需要先进行VPN登录。具体VPN的登录方法，详见：[VPN的使用](https://services.ptopenlab.com/mediawiki/index.php/VPN%E7%9A%84%E4%BD%BF%E7%94%A8)。
2） 登录该开发环境镜像所在的虚拟机。具体登录方法，详见：[登录虚拟机](https://services.ptopenlab.com/mediawiki/index.php/%E7%99%BB%E5%BD%95%E8%99%9A%E6%8B%9F%E6%9C%BA)。
3） 这个开发环境需要在root权限下运行Caffe。登录到虚拟机后，可以通过su命令切换到root（需要输入root用户密码）。
4） 该镜像的登陆用户名为opuser，初始密码为p0weruser。root用户初始密码为passw0rd。在初次登陆后会被要求强制修改密码。


# 测试Caffe开发环境
Caffe的安装目录在/home/opuser/caffe-master-20150813/，进入目录后，输入 make runtest，在打印中显示Cuda number of devices: 1，即表示GPU加速已经启用。后面会有很多的子项测试，通过了即为“OK”，如下所示：![Gpu caffe 1.png](https://services.ptopenlab.com/mediawiki/images/thumb/c/c3/Gpu_caffe_1.png/1000px-Gpu_caffe_1.png)

# 使用Caffe训练模型

我们使用mnist数据集来进行模型训练。mnist是手写数字识别的数据集，包含了很多手写数字的训练样本。我们已经为用户下载好mnist的数据集，用户只需在caffe的根目录直接训练即可： ./examples/mnist/train_lenet.sh，运行结果如下所示：![Gpu caffe 2.png](https://services.ptopenlab.com/mediawiki/images/thumb/7/7e/Gpu_caffe_2.png/1000px-Gpu_caffe_2.png)
该模型训练会迭代10000次停止。训练的过程中偶尔可能会出错退出，是此Caffe版本的一个bug，用户只要重新训练即可。
# 使用现有的模型分类

使用模型分类的例子，我们会以教程的形式给出，手把手教用户完成第一个深度学习的web应用，敬请期待！

本文来源于网络：https://services.ptopenlab.com/

