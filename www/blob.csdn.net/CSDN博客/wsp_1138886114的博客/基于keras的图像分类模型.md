# 基于keras的图像分类模型 - wsp_1138886114的博客 - CSDN博客





2018年10月15日 15:07:39[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：215











### 文章目录
- [一、获取图片数据](#_27)
- [1.1 读取图片文件 read_data.py](#11__read_datapy_28)
- [1.2 数据集整理 dataset.py](#12__datasetpy_68)
- [二、训练模型 train_model.py](#_train_modelpy_123)
- [三、模型测试 test_model.py](#_test_modelpy_236)



运行环境：`Anaconda | python3.7 --pycharm`

使用Anaconda 集成环境`pip install tensorflow , pip install keras`


keras的核心数据结构就是模型。模型是用来组织网络层的方式。

有`Sequential` 和 `Model` 两种模型。

在 ~/keras/keras.json 下修改最后一行backend对应的值即可。
```
{
"image_dim_ordering": "tf",
"epsilon": 1e-07,
"floatx": "float32",
"backend": "tensorflow"/"theano"    //二选一
}


两种模型传参顺序:

1. Sequential 模型:(conv_dim1,channels,conv_dim2,conv_dim3)
X_train = X_train.reshape(X_train.shape[0],1,img_rows,img_cols)
input_shape = (1,img_rows,img_cols)

2.model 模型 :(conv_dim1,conv_dim2,conv_dim3，channels)
X_train = X_train.reshape(X_train.shape[0],img_rows,img_cols,1)
input_shape = (img_rows,img_cols,1)
```

整个模型直接将所有图像数据导入训练，没有使用图像增强和数据生成器。整个模型的准确率比较低，有待改善。

### 一、获取图片数据

##### 1.1 读取图片文件 read_data.py

```python
import os 
import cv2
import numpy as np


def read_file(path): 
    img_list = [] 
    label_list = [] 
    dir_counter = 0

    #对路径下的所有子文件夹中的所有jpg文件进行读取并存入到一个list中 
    for child_dir in os.listdir(path): 
        child_path = path+"/"+child_dir 
        for dir_image in os.listdir(child_path):
            print(child_path+"/"+dir_image) 
            Suffix_name = ['.png','.jpg','.jpeg']
            if dir_image.endswith(tuple(Suffix_name)): 
                img = cv2.imread(child_path+"/"+dir_image) 
                img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
                #img = cv2.resize(img,(150,150)) 
                img_list.append(img) 
                label_list.append(dir_counter) 
        dir_counter += 1 
    img_list = np.array(img_list) 
    return img_list,label_list,dir_counter

#读取训练数据集的文件夹，把他们的名字返回给一个list
def read_name_list(path):
    name_list = []
    for child_dir in os.listdir(path):
        name_list.append(child_dir)
    return name_list


if __name__ == '__main__':
    img_list,label_lsit,counter = read_file('./face_reg/dataset_faces')
    print (counter)
```

##### 1.2 数据集整理 [dataset.py](http://dataset.py)

```python
import os
import cv2
import numpy as np
import keras
import random
from keras.models import Sequential
from keras.models import load_model
from keras.layers import BatchNormalization,Flatten
from keras.layers import Convolution2D,MaxPooling2D
from keras.layers import Activation,Dropout,Dense
from keras.optimizers import SGD,RMSprop
#from keras.preprocessing.image import ImageDataGenerator
from sklearn.model_selection import train_test_split


class DataSet(object):
    def __init__(self,path):
        self.num_classes = None
        self.X_train = None
        self.X_test = None
        self.Y_train = None
        self.Y_test = None
        self.extract_data(path)

    def extract_data(self,path):
        imgs,labels,counter = read_file(path)  #根据指定路径读取出图片、标签和类别数
        print("输出标记")
        print(labels)

        #拆分数据集
        X_train,X_test,y_train,y_test = train_test_split(imgs,labels,test_size=0.4,random_state=random.randint(0, 100))
        print("输出训练集标记:{0}和训练集长度:{1}:".format(y_train,len(X_train)))
        print("输出测试集标记:{0}和测试集长度:{1}".format(y_test,len(X_test)))
        print("输出和:",counter)

        # 格式化和标准化
        # 基于tensorflow的backend
        X_train = X_train.reshape(X_train.shape[0], 3, 150, 150) # X_train.shape[0] 训练集图片数量
        X_test = X_test.reshape(X_test.shape[0], 3, 150, 150)
        X_train = X_train.astype('float32')/255
        X_test = X_test.astype('float32')/255
        print(X_train[1])

        Y_train = keras.utils.to_categorical(y_train, num_classes=counter)
        Y_test = keras.utils.to_categorical(y_test, num_classes=counter)

        #将格式化后的数据赋值给类的属性上
        self.X_train = X_train
        self.X_test = X_test
        self.Y_train = Y_train
        self.Y_test = Y_test
        self.num_classes = counter
```

### 二、训练模型 train_model.py

```python
class Model(object):
    FILE_PATH = "./model_01.h5"            #模型进行存储和读取的地方
    def __init__(self):
        self.model = None
    def read_trainData(self,dataset):   #读取实例化后的DataSet类作为进行训练的数据源
        self.dataset = dataset


    """
    CNN模型:
    卷积 -> 批归一化 -> 激活 ->  池化

    卷积 -> 批归一化 -> 激活 ->  池化 -> dropout
    卷积 -> 批归一化 -> 激活 ->  池化 -> dropout

    展平 -> 全连接 -> 批归一化 -> 激活 -> dropout
        -> 全连接 -> 批归一化 ->  激活 -> dropout
        -> 全连接 -> 批归一化 ->  激活 -> summary
    """
    def build_model(self):
        self.model = Sequential()
        self.model.add(
            Convolution2D(
                filters=32,
                kernel_size=(5, 5),
                padding='same',
                dim_ordering='tf',
                input_shape=self.dataset.X_train.shape[1:])
        )
        self.model.add( BatchNormalization())
        self.model.add(Activation('relu'))
        self.model.add(
            MaxPooling2D(
                pool_size=(2, 2),
                strides=(2, 2),
                padding='same')
        )


        self.model.add(Convolution2D(filters=64, kernel_size=(5, 5), padding='same'))
        self.model.add(BatchNormalization())
        self.model.add(Activation('relu'))
        self.model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), padding='same'))
        self.model.add(Dropout(0.15))


        self.model.add(Convolution2D(filters=64, kernel_size=(5, 5), padding='same'))
        self.model.add(BatchNormalization())
        self.model.add(Activation('relu'))
        self.model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2), padding='same'))
        self.model.add(Dropout(0.15))


        self.model.add(Flatten())
        self.model.add(Dense(512))
        self.model.add(BatchNormalization())
        self.model.add(Activation('relu'))
        self.model.add(Dropout(0.5))

        self.model.add(Dense(128))
        self.model.add(BatchNormalization())
        self.model.add(Activation('relu'))
        self.model.add(Dropout(0.5))

        self.model.add(Dense(self.dataset.num_classes))
        self.model.add(BatchNormalization())
        self.model.add(Activation('softmax'))
        self.model.summary()

    """
    进行模型训练的函数
        optimizer 可选: RMSprop,Adagrad,adadelta
        loss：可选：categorical_crossentropy  squared_hinge
    """
    def train_model(self):
        sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
        self.model.compile(loss='categorical_crossentropy',optimizer='RMSprop',metrics=['accuracy'])
        self.model.fit(self.dataset.X_train,self.dataset.Y_train,epochs=12,batch_size=20)

    def evaluate_model(self):
        print('\nTesting---------------')
        loss, accuracy = self.model.evaluate(self.dataset.X_test, self.dataset.Y_test)
        print('test loss;', loss)
        print('test accuracy:', accuracy)

    def save(self, file_path=FILE_PATH):
        print('Model Saved.')
        self.model.save(file_path)

    def load(self, file_path=FILE_PATH):
        print('Model Loaded.')
        self.model = load_model(file_path)

    def predict(self,img):
        img = img.reshape((1, 150, 150,3))
        img = img.astype('float32')
        img = img/255.0
        result = self.model.predict_proba(img)  #测算一下该img属于某个label的概率
        max_index = np.argmax(result)           #找出概率最高的
        return max_index,result[0][max_index]   #第一个参数为概率最高的label的index,第二个参数为对应概率


if __name__ == '__main__':
    datast = DataSet('./face_dataset/All_faces') # 获取数据集
    model = Model()                              # 调用keras模型
    model.read_trainData(datast)                 # 读取训练数据
    model.build_model()                          # 创建训练网络
    model.train_model()                          # 训练模型
    model.evaluate_model()                       # 验证模型
    model.save()                                 # 保存模型
```

### 三、模型测试 test_model.py

```python
from read_data import read_file
from train_model import Model
import cv2
import os 
import numpy as np
from dataSet import DataSet
from keras import backend as K
from keras.utils import np_utils


K.clear_session()        #清空上一次会话

#读取训练数据集的文件夹，把他们的名字返回给一个list
def read_name_list(path):
    name_list = []
    for child_dir in os.listdir(path):
        name_list.append(child_dir)
    return name_list

#读取文件夹下子文件夹中所有图片进行识别
def test_onBatch(path):                          #传入需要分类图片的路径
    model= Model()
    model.load()
    index = 0
    img_list, label_list, counter = read_file(path)
    for img in img_list:
        picType,prob = model.predict(img)
        if picType != -1:
            index += 1
            name_list = read_name_list('./pictures/test')
            print(name_list)
            print (name_list[picType])
            cv2.imwrite("img_"+index+".png",img)   #对分类的图像保存
        else:
            print (" error")
    return index

if __name__ == '__main__':
    pro_predict=test_onBatch('./pictures/test/')    #传入需要分类图片的路径
```

整个模型情况如下：

```python
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
conv2d_1 (Conv2D)            (None, 3, 150, 32)        120032    
_________________________________________________________________
batch_normalization_1 (Batch (None, 3, 150, 32)        128       
_________________________________________________________________
activation_1 (Activation)    (None, 3, 150, 32)        0         
_________________________________________________________________
max_pooling2d_1 (MaxPooling2 (None, 2, 75, 32)         0         
_________________________________________________________________
conv2d_2 (Conv2D)            (None, 2, 75, 64)         51264     
_________________________________________________________________
batch_normalization_2 (Batch (None, 2, 75, 64)         256       
_________________________________________________________________
activation_2 (Activation)    (None, 2, 75, 64)         0         
_________________________________________________________________
max_pooling2d_2 (MaxPooling2 (None, 1, 38, 64)         0         
_________________________________________________________________
dropout_1 (Dropout)          (None, 1, 38, 64)         0         
_________________________________________________________________
conv2d_3 (Conv2D)            (None, 1, 38, 64)         102464    
_________________________________________________________________
batch_normalization_3 (Batch (None, 1, 38, 64)         256       
_________________________________________________________________
activation_3 (Activation)    (None, 1, 38, 64)         0         
_________________________________________________________________
max_pooling2d_3 (MaxPooling2 (None, 1, 19, 64)         0         
_________________________________________________________________
dropout_2 (Dropout)          (None, 1, 19, 64)         0         
_________________________________________________________________
flatten_1 (Flatten)          (None, 1216)              0         
_________________________________________________________________
dense_1 (Dense)              (None, 512)               623104    
_________________________________________________________________
batch_normalization_4 (Batch (None, 512)               2048      
_________________________________________________________________
activation_4 (Activation)    (None, 512)               0         
_________________________________________________________________
dropout_3 (Dropout)          (None, 512)               0         
_________________________________________________________________
dense_2 (Dense)              (None, 256)               131328    
_________________________________________________________________
batch_normalization_5 (Batch (None, 256)               1024      
_________________________________________________________________
activation_5 (Activation)    (None, 256)               0         
_________________________________________________________________
dropout_4 (Dropout)          (None, 256)               0         
_________________________________________________________________
dense_3 (Dense)              (None, 43)                11051     
_________________________________________________________________
batch_normalization_6 (Batch (None, 43)                172       
_________________________________________________________________
activation_6 (Activation)    (None, 43)                0         
=================================================================
Total params: 1,043,127
Trainable params: 1,041,185
Non-trainable params: 1,942
_________________________________________________________________
```
- 使用损失函数使用 "RMSprop"出现了过拟合，且模型准确率极低（训练准确率>>测试准确率）情况如下：

```
1357/1357 [==============================] - 24s 18ms/step - loss: 1.8120 - acc: 0.6374
 ------------evaluating---------------
test loss; 2.237347073581337
test accuracy: 0.5613259668837595
```
- 使用损失函数使用 "SGD"出现了过拟合，且模型准确率极低（训练准确率>>测试准确率）

```
1357/1357 [==============================] - 23s 17ms/step - loss: 1.6949 - acc: 0.5416
Epoch 12/12

905/905 [==============================] - 9s 10ms/step
test loss; 1.8758602150237362
test accuracy: 0.5237569061102788
```
- 由于准确率低模型没有进行预测

特别鸣谢：
[https://blog.csdn.net/u011268787/article/details/79891284](https://blog.csdn.net/u011268787/article/details/79891284)




