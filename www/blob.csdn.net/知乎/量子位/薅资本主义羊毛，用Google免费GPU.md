# 薅资本主义羊毛，用Google免费GPU - 知乎
# 



> 夏乙 编译自 Medium
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-98f278e684cd35de003f08fbc3a33221_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='940' height='301'></svg>)
英伟达Tesla K80 GPU，2599美元一块，还很难买到；搭载K80的AWS EC2 P2实例，用起来也很肉疼。

高大上的GPU，不花钱也能用上。Google的一项免费云端机器学习服务，最近也用上了Tesla K80。

这么好的羊毛，当然要薅起来啊！

这项资本主义社会的伟大创造，就是Google Colab，全名Colaboratory。你可以用它来提高Python技能，也可以用Keras、TensorFlow、PyTorch、OpenCV等等流行的深度学习库来练练手，开发深度学习应用。

地址在这里，Google还贴心地写了中文版简介：[https://colab.research.google.com/notebook](https://link.zhihu.com/?target=https%3A//colab.research.google.com/notebook)

即便如此，据说还有人不会用？

号称喜欢训练深度神经网络的作者fuat，就详详细细的写了一份Google Colab免费GPU试用指南~

## **准备工作**

## **在Google Drive上创建文件夹**
![](https://pic2.zhimg.com/v2-ee8d87900cf8575b2dedc2bfcdbf3efd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='326'></svg>)
Colab用的数据都存储在Google Drive云端硬盘上，所以，我们需要先指定要在Google Drive上用的文件夹。

比如说，可以在Google Drive上创建一个“app”文件夹，或者其他什么名字，也可以选择Colab笔记本默认的文件夹。
![](https://pic2.zhimg.com/v2-590ab9d223bda06ac738edccd48d7625_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='928' height='472'></svg>)
## **新建Colab笔记本**

在刚刚创建的app文件夹里点击右键，选择“More”，然后从菜单里选择“Colaboratory”，这样就新建出了一个Colab笔记本。
![](https://pic3.zhimg.com/v2-f81c533f8526419188db3b41f0b24e66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='903' height='619'></svg>)
点击笔记本的名字，可以重命名。
![](https://pic1.zhimg.com/v2-123ba40672a0a8edac465b324e74de1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='391' height='95'></svg>)
## **设置免费GPU**

这一步，要改变笔记本所用的默认硬件。在笔记本里点Edit>Notebook settings（编辑>笔记本设置），或者Runtime>Change runtime type（运行时>改变运行时类型），然后在Hardware accelerator（硬件加速器）一栏选择GPU。
![](https://pic2.zhimg.com/v2-0679d299bcfca7ef8fc63c77b889e599_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='370' height='284'></svg>)
然后，Google Colab就可以用了。

## **用Colab运行基本Python代码**
![](https://pic4.zhimg.com/v2-efb8df491ec313d1c0dc143bcef95e87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='196'></svg>)
我们来运行一些Python Numpy教程里的基本数据类型代码。
![](https://pic2.zhimg.com/v2-e02f8f1c567338e6e1a31a18b1207ff9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='311'></svg>)
这些代码来自斯坦福大学卷积神经网络与视觉识别课程(CS231n)的Python Numpy教程，
地址：[http://cs231n.github.io/python-numpy-tutorial/](https://link.zhihu.com/?target=http%3A//cs231n.github.io/python-numpy-tutorial/)

运行结果如你所料。

## **用Colab运行.py文件**

先运行下面这些代码，来安装必要的库、执行授权。


```
!apt-get install -y -qq software-properties-common python-software-properties module-init-tools
!add-apt-repository -y ppa:alessandro-strada/ppa 2>&1 > /dev/null
!apt-get update -qq 2>&1 > /dev/null
!apt-get -y install -qq google-drive-ocamlfuse fuse
from google.colab import auth
auth.authenticate_user()
from oauth2client.client import GoogleCredentials
creds = GoogleCredentials.get_application_default()
import getpass
!google-drive-ocamlfuse -headless -id={creds.client_id} -secret={creds.client_secret} < /dev/null 2>&1 | grep URL
vcode = getpass.getpass()
!echo {vcode} | google-drive-ocamlfuse -headless -id={creds.client_id} -secret={creds.client_secret}
```


运行的时候应该会看到下图所示的结果：
![](https://pic4.zhimg.com/v2-78b1173a35abccb3f6a19a76a78305e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='845' height='268'></svg>)
看见那个链接之后，点击它，复制验证码并粘贴到文本框里。

授权完成后，就可以挂载Google Drive了：


```
!mkdir -p drive
!google-drive-ocamlfuse drive
```


安装Keras：

`!pip install -q keras`

将mnist_cnn.py文件上传到位于Google云端硬盘上的应用文件夹：
![](https://pic3.zhimg.com/v2-6228cba4706b1ae6307c9ef362b920fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='478' height='283'></svg>)
运行下面的代码，用MNIST数据集训练一个简单的卷积神经网络：

`!python3 drive/app/mnist_cnn.py `
![](https://pic4.zhimg.com/v2-d2c5e2e1e73a4bc96269bc49b209794f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='575'></svg>)
从结果中可以看到，每个epoch只需要11秒。

## **下载泰坦尼克数据集(.csv File)，显示前5行**

想按照链接下载.csv文件到app文件夹，只需运行：

`!wget https://raw.githubusercontent.com/vincentarelbundock/Rdatasets/master/csv/datasets/Titanic.csv -P drive/app`

也可以直接将.csv文件上传到app文件夹：
![](https://pic1.zhimg.com/v2-32cb3375882f3ee630f735b8fffbeb04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='948' height='237'></svg>)
然后读取app文件夹中的.csv文件，显示前5行：


```
import pandas as pd
titanic = pd.read_csv(“drive/app/Titanic.csv”)
titanic.head(5)
```

![](https://pic2.zhimg.com/v2-7e865302cb60c5e0ae5621a05246b6ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='773' height='277'></svg>)
## **Tips**

## **1. 如何安装库？**

安装Keras：


```
!pip install -q keras
import keras
```


安装PyTorch：


```
!pip install -q http://download.pytorch.org/whl/cu75/torch-0.2.0.post3-cp27-cp27mu-manylinux1_x86_64.whl torchvision
import torch
```


安装OpenCV：


```
!apt-get -qq install -y libsm6 libxext6 && pip install -q -U opencv-python
import cv2
```


安装XGBoost：


```
!pip install -q xgboost==0.4a30
import xgboost
```


安装GraphViz：


```
!apt-get -qq install -y graphviz && pip install -q pydot
import pydot
```


安装7zip Reader：


```
!apt-get -qq install -y libarchive-dev && pip install -q -U libarchive
import libarchive
```


安装其他库：

用`!pip install`或者`!apt-get install`命令。

## **2. GPU在干活么？**

要查看你在Colab里是不是真的在用GPU，可以运行以下代码来交叉检查：


```
import tensorflow as tf
tf.test.gpu_device_name()
```

![](https://pic2.zhimg.com/v2-acbfa4aa1ae9b33ca907226e3e0fd24d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='463' height='145'></svg>)
如果显示上图左侧的结果，就是在用CPU，显示右侧结果就是在用GPU。

## **3. 我在用哪个GPU？**


```
from tensorflow.python.client import device_lib
device_lib.list_local_devices()
```


其实现在，Colab只提供Tesla K80，所以你会看到下图这样的结果：
![](https://pic4.zhimg.com/v2-7ce081aafb7c0c938413a92ffc369023_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='813' height='275'></svg>)
## **4. RAM有多大？**

`!cat /proc/meminfo `
![](https://pic3.zhimg.com/v2-dc1b8c1ef6ed28c20f563d8d40901cee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='242' height='215'></svg>)
## **5. CPU呢？**

`!cat /proc/cpuinfo `
![](https://pic2.zhimg.com/v2-e717305a52032bac8c1c1b7fbd8781a5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='405'></svg>)
## **总结一下吧**

好好学习，认真薅毛。

量子位温馨提示：自备梯子。

[https://medium.com/deep-learning-turkey/google-colab-free-gpu-tutorial-e113627b9f5d](https://link.zhihu.com/?target=https%3A//medium.com/deep-learning-turkey/google-colab-free-gpu-tutorial-e113627b9f5d)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


