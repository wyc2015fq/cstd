# 基于keras的图像多分类-flower - wsp_1138886114的博客 - CSDN博客





2018年10月24日 09:30:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：803








### 一、定义模型 [lenet.py](http://lenet.py)

```python
from keras.models import Sequential
from keras.layers.convolutional import Conv2D
from keras.layers.convolutional import MaxPooling2D
from keras.layers.core import Activation
from keras.layers.core import Flatten
from keras.layers.core import Dense
from keras import backend as K

class LeNet:
	@staticmethod
	def build(width, height, depth, classes):
		model = Sequential()
		inputShape = (height, width, depth)

		# using "channels first",（样本数，通道数，行或高，列或宽）通道在前,修改参数位置
		if K.image_data_format() == "channels_first":
			inputShape = (depth, height, width)

		"""
		CONV => RELU => POOL layers
		CONV => RELU => POOL layers
		FC => RELU layers
		softmax
		
		"""

		model.add(Conv2D(20, (5, 5), padding="same", input_shape=inputShape,name='Conv_1'))
		model.add(Activation("relu"))
		model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

		model.add(Conv2D(50, (5, 5), padding="same",name='Conv_2'))
		model.add(Activation("relu"))
		model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

		# first (and only) set of FC => RELU layers
		model.add(Flatten())
		model.add(Dense(500,name='Dense_1'))
		model.add(Activation("relu"))
		
		# softmax classifier
		model.add(Dense(classes,name='Dense_2'))
		model.add(Activation("softmax"))

		# return the constructed network architecture
		return model
```

### 二、定义训练模型 train_network.py

```python
# set the matplotlib backend so figures can be saved in the background
import matplotlib
matplotlib.use("Agg")

# import the necessary packages
from keras.preprocessing.image import ImageDataGenerator
from keras.optimizers import Adam
from sklearn.model_selection import train_test_split
from keras.preprocessing.image import img_to_array
from keras.utils import to_categorical
from pyimagesearch.lenet import LeNet
from imutils import paths
import matplotlib.pyplot as plt
import numpy as np
import argparse
import random
import cv2
import os


ap = argparse.ArgumentParser()
ap.add_argument("-d", "--dataset", type=str, default="training_set" ,
	help="path to input dataset")
ap.add_argument("-m", "--model", type=str, default="trained_model" ,
	help="path to output model")
ap.add_argument("-p", "--plot", type=str, default="plot.png" ,
	help="path to output loss/accuracy plot")
args = vars(ap.parse_args())


EPOCHS = 25
INIT_LR = 1e-3
BS = 32


data = []
labels = []
dir_labels = ()
num_class = 0


# finding the labels
print("[INFO] Finding Labels...")
for file in os.listdir(args["dataset"]) :
	temp_tuple=(file,'null')
	dir_labels=dir_labels+temp_tuple
	dir_labels=dir_labels[:-1]
	#print(dir_labels)
	num_class=num_class+1
	#print(num_class)

	
# grab the image paths and randomly shuffle them
print("[INFO] Loading Images...")
imagePaths = sorted(list(paths.list_images(args["dataset"])))
random.seed(42)
random.shuffle(imagePaths)


for imagePath in imagePaths:
	image = cv2.imread(imagePath)
	image = cv2.resize(image, (28, 28))
	image = img_to_array(image)
	data.append(image)

	label = imagePath.split(os.path.sep)[-2]	
	for i in range(num_class) :
		if label == dir_labels[i] :
			label = i
	labels.append(label)

data = np.array(data, dtype="float") / 255.0
labels = np.array(labels)



(trainX, testX, trainY, testY) = train_test_split(data,labels, test_size=0.25, random_state=42)
trainY = to_categorical(trainY, num_classes=num_class)
testY = to_categorical(testY, num_classes=num_class)


aug = ImageDataGenerator(rotation_range=30, width_shift_range=0.1,
	height_shift_range=0.1, shear_range=0.2, zoom_range=0.2,
	horizontal_flip=True, fill_mode="nearest")

print("[INFO] Compiling Model...")
model = LeNet.build(width=28, height=28, depth=3, classes=num_class)
opt = Adam(lr=INIT_LR, decay=INIT_LR / EPOCHS)
model.compile(loss="binary_crossentropy", optimizer=opt,
	metrics=["accuracy"])

print("[INFO] Training Network...")
H = model.fit_generator(aug.flow(trainX, trainY, batch_size=BS),
	validation_data=(testX, testY), steps_per_epoch=len(trainX) // BS,
	epochs=EPOCHS, verbose=1 )

print("[INFO] Saving Model...")
model_base=args["model"]+'.h5'
model.save(model_base)

# plot the training loss and accuracy
plt.style.use("ggplot")
plt.figure()
plt.plot(np.arange(0,EPOCHS), H.history["loss"], label="train_loss")
plt.plot(np.arange(0,EPOCHS), H.history["val_loss"], label="val_loss")
plt.plot(np.arange(0,EPOCHS), H.history["acc"], label="train_acc")
plt.plot(np.arange(0,EPOCHS), H.history["val_acc"], label="val_acc")
plt.title("Training Loss and Accuracy")
plt.xlabel("Epoch #")
plt.ylabel("Loss/Accuracy")
plt.legend(loc="lower left")
plt.savefig(args["plot"])

print("[INFO] Completed...")
```

### 三、定义预测模型

```python
# import the necessary packages
from keras.preprocessing.image import img_to_array
from keras.models import load_model
import numpy as np
import argparse
import imutils
import cv2
import os
import sys

# initialising 
dir_labels=()
dir_predict=()
num_class=0

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--dataset", required=False, default="training_set",
	help="path to input dataset")
ap.add_argument("-m", "--model", required=False, default="trained_model",
	help="path to trained model model")
ap.add_argument("-i", "--image", required=True,
	help="path to input image")
args = vars(ap.parse_args())

#findings the labels
for file in os.listdir(args["dataset"]) :
	temp_tuple=(file,'null')
	dir_labels=dir_labels+temp_tuple
	dir_labels=dir_labels[:-1]
	num_class=num_class+1

print("[INFO] Labels are ",dir_labels)

# load the image
print("[INFO] Loading Image...")
try :
	image = cv2.imread(args["image"])
	orig = image.copy()
except AttributeError :
	print("[INFO] Error in the test image... ")
	print('[INFO] Exiting...')
	sys.exit()

# pre-process the image for classification
image = cv2.resize(image, (28, 28))
image = image.astype("float") / 255.0
image = img_to_array(image)
image = np.expand_dims(image, axis=0)

# load the trained convolutional neural network
print("[INFO] Loading Network...")
model_base=args["model"]+'.h5'
model = load_model(model_base)

# classify the input image
dir_predict = model.predict(image)[0]
print(dir_labels)
print(dir_predict)
for i in range(num_class) :
	var = 0
	for j in range(num_class) :
		if(dir_predict[i]>=dir_predict[j]) :
			var=var+1
	if(var==num_class) :
		label=dir_labels[i]
		proba=dir_predict[i]
	elif(var==num_class-1) :
		label2=dir_labels[i]
		proba2=dir_predict[i]


label = "{}: {:.2f}%".format(label, proba * 100)

# draw the label on the image
output = imutils.resize(orig, width=400)
cv2.putText(output, label, (10, 25),  cv2.FONT_HERSHEY_SIMPLEX,
	0.7, (0, 255, 0), 2)

# show and save the output image
cv2.imshow("Output", output)
cv2.imwrite("Output.png",output)
cv2.waitKey(0)  #Press any key to exit the output image

print('[INFO] Exiting...')
```

数据集下载地址：

执行命令如下：

```python
python train_network.py --dataset training_set --model trained_model --plot plo

输出：
Using TensorFlow backend.
[INFO] Finding Labels...
[INFO] Loading Images...
[INFO] Compiling Model...
[INFO] Training Network...
Epoch 1/25
2018-10-30 16:25:45.234853: I tensorflow/core/platform/cpu_feature_guard.cc:141] Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX2
101/101 [==============================] - 18s 175ms/step - loss: 0.4371 - acc: 0.8043 - val_loss: 0.4090 - val_acc: 0.8098
Epoch 2/25
101/101 [==============================] - 13s 128ms/step - loss: 0.3919 - acc: 0.8224 - val_loss: 0.3658 - val_acc: 0.8415
Epoch 3/25
101/101 [==============================] - 12s 119ms/step - loss: 0.3714 - acc: 0.8333 - val_loss: 0.3616 - val_acc: 0.8515
Epoch 4/25
101/101 [==============================] - 12s 121ms/step - loss: 0.3661 - acc: 0.8352 - val_loss: 0.3442 - val_acc: 0.8547
Epoch 5/25
101/101 [==============================] - 14s 140ms/step - loss: 0.3393 - acc: 0.8486 - val_loss: 0.3250 - val_acc: 0.8636
Epoch 6/25
101/101 [==============================] - 15s 144ms/step - loss: 0.3274 - acc: 0.8575 - val_loss: 0.3387 - val_acc: 0.8528
Epoch 7/25
101/101 [==============================] - 14s 134ms/step - loss: 0.3270 - acc: 0.8564 - val_loss: 0.3232 - val_acc: 0.8614
Epoch 8/25
101/101 [==============================] - 15s 145ms/step - loss: 0.3192 - acc: 0.8626 - val_loss: 0.3176 - val_acc: 0.8627
Epoch 9/25
101/101 [==============================] - 14s 143ms/step - loss: 0.3033 - acc: 0.8695 - val_loss: 0.3333 - val_acc: 0.8562
Epoch 10/25
101/101 [==============================] - 13s 126ms/step - loss: 0.3141 - acc: 0.8641 - val_loss: 0.3155 - val_acc: 0.8647
Epoch 11/25
101/101 [==============================] - 17s 166ms/step - loss: 0.3032 - acc: 0.8672 - val_loss: 0.3088 - val_acc: 0.8667
Epoch 12/25
101/101 [==============================] - 17s 166ms/step - loss: 0.2906 - acc: 0.8755 - val_loss: 0.3450 - val_acc: 0.8562
Epoch 13/25
101/101 [==============================] - 16s 155ms/step - loss: 0.2884 - acc: 0.8759 - val_loss: 0.3135 - val_acc: 0.8688
Epoch 14/25
101/101 [==============================] - 16s 155ms/step - loss: 0.2943 - acc: 0.8733 - val_loss: 0.3143 - val_acc: 0.8652
Epoch 15/25
101/101 [==============================] - 15s 149ms/step - loss: 0.2731 - acc: 0.8868 - val_loss: 0.2883 - val_acc: 0.8791
Epoch 16/25
101/101 [==============================] - 14s 135ms/step - loss: 0.2776 - acc: 0.8806 - val_loss: 0.3088 - val_acc: 0.8723
Epoch 17/25
101/101 [==============================] - 13s 132ms/step - loss: 0.2612 - acc: 0.8864 - val_loss: 0.3020 - val_acc: 0.8808
Epoch 18/25
101/101 [==============================] - 13s 132ms/step - loss: 0.2639 - acc: 0.8887 - val_loss: 0.2781 - val_acc: 0.8830
Epoch 19/25
101/101 [==============================] - 14s 141ms/step - loss: 0.2600 - acc: 0.8878 - val_loss: 0.2882 - val_acc: 0.8797
Epoch 20/25
101/101 [==============================] - 15s 144ms/step - loss: 0.2476 - acc: 0.8938 - val_loss: 0.3219 - val_acc: 0.8690
Epoch 21/25
101/101 [==============================] - 14s 139ms/step - loss: 0.2518 - acc: 0.8938 - val_loss: 0.2979 - val_acc: 0.8806
Epoch 22/25
101/101 [==============================] - 13s 125ms/step - loss: 0.2565 - acc: 0.8894 - val_loss: 0.2977 - val_acc: 0.8782
Epoch 23/25
101/101 [==============================] - 15s 145ms/step - loss: 0.2423 - acc: 0.8976 - val_loss: 0.3188 - val_acc: 0.8684
Epoch 24/25
101/101 [==============================] - 15s 151ms/step - loss: 0.2317 - acc: 0.9030 - val_loss: 0.2979 - val_acc: 0.8825
Epoch 25/25
101/101 [==============================] - 14s 139ms/step - loss: 0.2312 - acc: 0.9033 - val_loss: 0.2858 - val_acc: 0.8869
[INFO] Saving Model...
[INFO] Completed...
```



