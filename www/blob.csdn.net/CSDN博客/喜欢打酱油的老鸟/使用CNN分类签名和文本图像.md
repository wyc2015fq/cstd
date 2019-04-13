
# 使用CNN分类签名和文本图像 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月24日 08:22:36[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：41


[https://www.toutiao.com/a6644821136204907022/](https://www.toutiao.com/a6644821136204907022/)
2019-01-10 18:59:42
正如我们所知道的任何具有法律重要性的文件，无论是合同，托运还是其他形式需要授权的，签名都是重要的一部分。签名提供身份证明和确认。本文提出的工作是关于签名和文本数据的分类。
分类模型使用Keras构建，Keras是TensorFlow的高级API，它是用于机器学习的开源库。该分类模型还可以帮助构建文档图像的签名检测模型。
**数据准备**
数据由两个类组成：Signature（签名）（类标签0）和Text（文本）（类标签1）。
文本图像的数据包含具有不同背景，高度，宽度和笔划粗细的独立单词的图像。文本图像不限于独特的语言，还涉及多语言文本。该数据包含1926个不同的图像。
签名图像数据包含1287个不同背景，高度，宽度和笔划粗细的签名图像。
数据已存储本地存储中。数据清理的初步步骤包括删除模糊图像，并用适当的填充和空白重新排列文本。为了增加数据的大小，执行了一些运行时数据扩充操作，如旋转、调整大小和缩放操作。数据集分为70％用于训练，30％用于验证。除此之外，还有单独的测试数据，在这些测试数据上进行测试以获得更精确的结果。
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/4b0568b8cc7c412c9a04d435aecdbfd0)
数据集中的图像样本
**分类模型**
深度卷积神经网络是使用序列模型构建的。有三个卷积层以及完全连接和输出层。最大池化大小与CNN参数设置为（2,2），内核大小设置为（3,3）。最初，滤波器的数量设置为32.在后续的卷积层中加倍滤波器的数量。
```python
# Layer Values
num_filters = 32 # No. of conv filters
max_pool_size = (2,2) # shape of max_pool
conv_kernel_size = (3, 3) # conv kernel shape
imag_shape = (100, 100, 3)
num_classes = 2
drop_prob = 0.5
```
![使用CNN分类签名和文本图像](http://p9.pstatp.com/large/pgc-image/af82489912f745eab18787a7991034d2)
使用的激活函数为ReLU，最后一层激活函数为Sigmoid。添加丢失层（dropout layer）dropout概率为0.5。该模型的架构如下。
```python
# Define model type
model = Sequential()
# 1st Layer
model.add(Conv2D(num_filters, conv_kernel_size[0], conv_kernel_size[1], input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
# 2nd Convolution Layer
model.add(Conv2D(num_filters*2, conv_kernel_size[0], conv_kernel_size[1], input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
# 3nd Convolution Layer
model.add(Conv2D(num_filters*4, conv_kernel_size[0], conv_kernel_size[1], input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
#Fully Connected Layer
model.add(Flatten())
model.add(Dense(128, activation='relu')) #Fully connected layer
# Dropout some neurons to reduce overfitting
model.add(Dropout(drop_prob))
#Readout Layer
model.add(Dense(num_classes, activation='sigmoid'))
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/3f52439538be4fba8476eb002a2e4b29)
接下来，使用binary_crossentropy作为精度和损失的评估指标，adam optimizer作为优化器来编译模型。
由于训练数据的大小是有限的，因此可以使用ImageDataGenerator()函数添加运行时图像增强。训练数据集中增加了旋转、缩放、缩放等图像增强功能。
```python
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
datagen = image.ImageDataGenerator(
 rotation_range=20,
 rescale=1./255,
 zoom_range=0.2,
 horizontal_flip=False)
datagen_v = image.ImageDataGenerator() #Generator for validation dataset
model.fit_generator(datagen.flow(train_images, train_labels, batch_size=32), 
 steps_per_epoch=len(train_images) / 32, epochs=num_epoch, 
 validation_data=datagen_v.flow(test_images, test_labels, batch_size=32), validation_steps= 3)
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/7c8a76628d5445d9b0765798d8ca8f12)
为了预测测试数据集模型的输出，使用了预测方法。然后使用sklearn.metrics从预测中计算出准确率、召回率和测试准确度。
```python
test_pred_t = model.predict(test_images_t)
test_label_t = np.argmax(test_labels_t, axis=1)
test_pred_t = np.argmax(test_pred_t,axis=1)
print("Precision ", precision_score(test_label_t,test_pred_t))
print("Recall ", recall_score(test_label_t,test_pred_t))
print("Accuracy ", accuracy_score(test_label_t,test_pred_t))
```
![使用CNN分类签名和文本图像](http://p3.pstatp.com/large/pgc-image/35296e9a36ca489a9110d83ad601914b)
增加图像增强和丢失层后的最终测试准确率为94.29％。签名图像的准确率为96.55％，召回率为97.22％。
下表通过添加augmentations和dropout来深入了解结果升级。
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/a169e7cc9e2a4aa48fcda13095f24be5)
各种实验结果
**训练和测试模型**
**数据**
通过对不同语言、不同背景下的签名图像和文本图像的采集，得到了相应的数据。如前所述，对数据进行相同的预处理。有两个类，签名和文本。
**1.setup.py**
```python
from setuptools import find_packages
from setuptools import setup
REQUIRED_PACKAGES = ['keras==2.2.4',
 'Keras-Preprocessing==1.0.5',
 'keras-utils==1.0.13',
 'Pillow==5.3.0',
 'h5py==2.9.0',
 'scikit-learn==0.20.2']
setup(
 name='trainer',
 version='0.1',
 author = 'Krishna Parekh',
 install_requires=REQUIRED_PACKAGES,
 packages=find_packages(),
 include_package_data=True,
 requires=[],
 description='CMLE Signature Classification'
)
```
![使用CNN分类签名和文本图像](http://p3.pstatp.com/large/pgc-image/6c0fdac9afce4c55b758f42fca90aac0)
**2.task.py**
task.py文件是模型的入口点。它包含运行模型时需要解析的参数列表。它还会调用模型和其他相关文件（如果有）。训练的模型以.hdf5格式保存。task.py文件的代码如下。
```python
CLASSIFICATION_MODEL = 'cl_model.hdf5'
def get_args():
 parser = argparse.ArgumentParser()
 # Input Arguments
 parser.add_argument(
 '--package-path',
 help = 'local path to training data',
 )
 parser.add_argument(
 '--job-dir',
 type = str,
 help = ' export models'
 )
 parser.add_argument(
 '--train-dir',
 help = 'local path to training data',
 required = True
 )
 args, _ = parser.parse_known_args()
 return args
def train_and_evaluate(args):
 f_train_i = BytesIO(file_io.read_file_to_string(args.train_dir+'/train_images.npy', binary_mode=True))
 train_images = np.load(f_train_i) # Loading training images
 f_train_l = BytesIO(file_io.read_file_to_string(args.train_dir+'/train_labels.npy', binary_mode=True))
 train_labels = np.load(f_train_l) # Loading training labels
 f_test_i = BytesIO(file_io.read_file_to_string(args.train_dir+'/test_images.npy', binary_mode=True))
 valid_images = np.load(f_test_i) # Loading Validation images
 f_test_l = BytesIO(file_io.read_file_to_string(args.train_dir+'/test_labels.npy', binary_mode=True))
 valid_labels = np.load(f_test_l) 
 
 Model = model.model_cnn()
 Model.summary()
 
 datagen = tf.keras.preprocessing.image.ImageDataGenerator(
 rotation_range=20,
 rescale=1./255,
 zoom_range=0.2,
 horizontal_flip=False) # Generator for training dataset
 
 datagen_v = tf.keras.preprocessing.image.ImageDataGenerator() #Generator for validation dataset
 
 Model.fit_generator(datagen.flow(train_images, train_labels, batch_size=32), 
 steps_per_epoch=len(train_images) / 32, epochs=3, 
 validation_data=datagen_v.flow(valid_images, valid_labels,
 batch_size=32), validation_steps= 3)
 
 Model.save('cl_model.hdf5')
 
 job_dir = args.job_dir+'/export'
 
 if job_dir.startswith("gs://"):
 Model.save(CLASSIFICATION_MODEL)
 
 
if __name__ == "__main__":
 args = get_args()
 arguments = args.__dict__
 train_and_evaluate(args)
```
![使用CNN分类签名和文本图像](http://p3.pstatp.com/large/pgc-image/06ea6e7c9edd4be482eb348acf301f6a)
注意：保存的模型为.hdf5格式。要部署模型，我们需要模型的.pb格式。为此，我们需要使用TensorFlow服务导出模型。
**3.model.py**
model.py包含要训练的实际模型。它将已编译的模型返回给调用函数。模型函数的代码如下所示。
```python
def model_cnn(imag_shape = (100, 100, 3)):
 # Layer Values
 num_filters = 32 # No. of conv filters
 max_pool_size = (2,2) # shape of max_pool
 conv_kernel_size = (3, 3) # conv kernel shape
 imag_shape = imag_shape
 num_classes = 2
 drop_prob = 0.5 # fraction to drop (0-1.0)
 
 # Define model type
 model = Sequential()
 
 # 1st Layer
 model.add(Conv2D(filters=num_filters, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
 model.add(MaxPooling2D(pool_size=max_pool_size))
 
 # 2nd Convolution Layer
 model.add(Conv2D(filters = num_filters*2, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
 model.add(MaxPooling2D(pool_size=max_pool_size))
 
 # 3nd Convolution Layer
 model.add(Conv2D(filters = num_filters*4, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
 model.add(MaxPooling2D(pool_size=max_pool_size))
 
 #Fully Connected Layer
 model.add(Flatten())
 model.add(Dense(128, activation='relu')) #Fully connected layer 
 
 # Dropout some neurons to reduce overfitting
 model.add(Dropout(drop_prob))
 
 #Readout Layer
 model.add(Dense(num_classes, activation='sigmoid'))
 
 # Set loss and measurement, optimizer, and metric used to evaluate loss
 model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
 
 return model
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/250c4d88c5f54e86aab4b99098d4fe99)
**4.utils.py**
该文件包含数据预处理的代码。传递包含图像文件的目录的位置，并生成适合模型输入的标记数据。生成数据集并将其保存在.npy文件中，然后将其用于模型培训。
```python
def generate_data(image_dir, image_width=100, image_height=100):
 directory = image_dir
 dataset_full = []
 print("directory", directory) 
 folders = next(os.walk(directory))
 count_fol = 0
 count_file = 0
 for fol in folders[1]:
 fol_path = directory+'/' + fol + "/"
 count_fol += 1
 files = next(os.walk(fol_path))[2]
 for f in files:
 file_lbl = []
 file_path = fol_path + f
 file_lbl.append(file_path)
 file_lbl.append(fol)
 count_file += 1
 dataset_full.append(file_lbl)
 selector = np.random.random((count_file))
 
 dataset_train = []
 dataset_test = []
 
 for i in range(len(selector)):
 if selector[i] > 1.0/3.0:
 #print(selector[i], "train")
 selector[i] = 1 # For training
 dataset_train.append(dataset_full[i])
 else:
 selector[i] = 0 # For testing
 dataset_test.append(dataset_full[i])
 
 # Now the classification 
 train_images = []
 train_labels = []
 image_rows = image_height # Size of the image
 image_cols = image_width
 
 le = LabelEncoder()
 
 print("dataset_train", len(dataset_train))
 print("dataset_test", len(dataset_test))
 
 for i in range(0, len(dataset_train)):
 img = image.load_img(dataset_train[i][0])
 img = img.resize((image_rows, image_cols), Image.ANTIALIAS)
 img_ar = np.array(img)
 lbl = dataset_train[i][1]
 train_images.append(img_ar)
 train_labels.append(lbl)
train_images = np.array(train_images)
 train_labels = le.fit_transform(train_labels) # Get numeric class labels
 
 test_images = []
 test_labels = ["" for _ in range(0, len(dataset_test))]
 
 for i in range(0, len(dataset_test)):
 img = image.load_img(dataset_test[i][0])
 img = img.resize((image_rows, image_cols), Image.ANTIALIAS)
 img_ar = np.array(img)
 lbl = dataset_test[i][1]
 test_images.append(img_ar)
 test_labels[i] = lbl
test_images = np.array(test_images)
 len(test_images)
 test_labels = le.fit_transform(test_labels)
 
return train_images, train_labels, test_images, test_labels
```
![使用CNN分类签名和文本图像](http://p3.pstatp.com/large/pgc-image/c7da379730ce4fd3a95fe6a91cd17e67)
**开始训练模型（本文后半部分是在Google云上进行的训练）**
```python
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Input
from tensorflow.keras.layers import LSTM, GRU
from tensorflow.keras.layers import Embedding
from tensorflow.keras.layers import Conv1D
from tensorflow.keras.layers import MaxPooling1D, Dense, Conv2D, Flatten, MaxPooling2D
from tensorflow.keras.layers import Input
from tensorflow.keras.models import Model
from tensorflow.keras.preprocessing import sequence
from tensorflow.keras.layers import Dropout
from tensorflow.keras.callbacks import ModelCheckpoint
import pandas as pd
import tqdm
import random 
import string
from sklearn.metrics import confusion_matrix
import numpy as np
import keras
import pickle
import time
import re
import datetime
import tensorflow as tf
import warnings
warnings.filterwarnings('ignore')
bucket_name='signature_classify'
file_location='sign_classify_f2/export/cl_model.hdf5'
tf_serving_model_out_location='gs://signature_classify/export/'
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/d4ce9888165f4df5b5f084fd38b43338)
![使用CNN分类签名和文本图像](http://p9.pstatp.com/large/pgc-image/ce7a9d889bcc404f89866633bb1cb7a0)
导出keras hdf5模型到Tensorflow SavedModel格式
**首先定义模型并加载权重**
```python
imag_shape = (100, 100, 3)
num_filters = 32 # No. of conv filters
max_pool_size = (2,2) # shape of max_pool
conv_kernel_size = (3, 3) # conv kernel shape
imag_shape = imag_shape
num_classes = 2
drop_prob = 0.5 # fraction to drop (0-1.0)
model = Sequential()
model.add(Conv2D(filters=num_filters, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
model.add(Conv2D(filters = num_filters*2, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
model.add(Conv2D(filters = num_filters*4, kernel_size=(conv_kernel_size), input_shape=imag_shape, activation='relu'))
model.add(MaxPooling2D(pool_size=max_pool_size))
model.add(Flatten())
model.add(Dense(128, activation='relu')) #Fully connected layer 
model.add(Dropout(drop_prob))
model.add(Dense(num_classes, activation='sigmoid'))
model.load_weights('cl_model.hdf5')
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/22b1838def274939b712fe0073390d19)
**使用下面的代码将模型导出为SavedModel格式**
```python
from tensorflow.keras import backend as K
from tensorflow.keras.models import model_from_config, Sequential
K.set_learning_phase(0) 
config = model.get_config()
weights = model.get_weights()
new_model = Sequential.from_config(config)
new_model.set_weights(weights)
from tensorflow.python.saved_model import builder as saved_model_builder
from tensorflow.python.saved_model.signature_def_utils_impl import build_signature_def, predict_signature_def
from tensorflow.python.saved_model import tag_constants, signature_constants
%rm -rf signature_classify/export/version_1/
export_path = 'signature_classify/export/version_1/'
builder = saved_model_builder.SavedModelBuilder(export_path)
signature = predict_signature_def(inputs={'images': new_model.input},
 outputs={'scores': new_model.output})
with K.get_session() as sess:
 builder.add_meta_graph_and_variables(sess=sess,
 tags=[tag_constants.SERVING],
 signature_def_map={'serving_default': signature})
 builder.save()
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/bb8841068a5541aaae8f1c550194ac64)
使用saved_model_cli api验证导出的SavedModel
```python
saved_model_cli show --dir signature_classify/export/version_1 --all
```
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/7aac0d59bf3f46329e25e6d1ad777141)
**预测模型**
```python
from PIL import Image
import cv2
#Resize the original image.
img=Image.open('image.jpg')
img = img.resize((100, 100), Image.ANTIALIAS)
img.save('new_image.jpg')
img=cv2.imread('new_image.jpg')
input_dict={'images': img.tolist()}
import json
with open('test_data.json', 'w') as outfile:
 json.dump(input_dict, outfile)
```
(100, 100, 3)
![使用CNN分类签名和文本图像](http://p1.pstatp.com/large/pgc-image/0b90e656a1b94748bc8a3f16dce962a2)

