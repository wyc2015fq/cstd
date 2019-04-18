# PythonOpenCV--Rtrees随机森林 - wishchinYang的专栏 - CSDN博客
2014年08月11日 10:55:07[wishchin](https://me.csdn.net/wishchin)阅读数：1789

```python
360确实很个性，哈哈,你个貔貅，只吃不吐！
Rtrees介绍！参考链接：http://docs.opencv.org/modules/ml/doc/random_trees.html
```
原文链接：Python opencv实现的手写字符串识别--SVM 、神经网络、 K近邻、 Boosting、Rtrees
```python
代码copy：
```
```python
#http://www.360doc.com/content/14/0811/10/13997450_400977867.shtml
#code from!
#dataset:http://yann.lecun.com/exdb/mnist/
from cv2.cv import *
import cv2
import os
import struct
import numpy
class_n = 10
number_of_training_set = 2000 #0 for all, 60,000 max
number_of_test_set = 0 #0 for all, 10,000 max
#trainimagepath = r'.\data\train-images.idx3-ubyte'
trainimagepath = r'E:\Develope\EclipseWorks\MeachinLearning\Ch16_RandomTrees\data\train-images.idx3-ubyte'
#trainlabelpath = r'.\data\train-labels.idx1-ubyte'
#testimagepath = r'.\data\t10k-images.idx3-ubyte'
#testlabelpath = r'.\data\t10k-labels.idx1-ubyte'
trainlabelpath = r'E:\Develope\EclipseWorks\MeachinLearning\Ch16_RandomTrees\data\train-labels.idx1-ubyte'
testimagepath = r'E:\Develope\EclipseWorks\MeachinLearning\Ch16_RandomTrees\data\t10k-images.idx3-ubyte'
testlabelpath = r'E:\Develope\EclipseWorks\MeachinLearning\Ch16_RandomTrees\data\t10k-labels.idx1-ubyte'
def evalfun(method, y_val, test_labels, test_number_of_images):
    count = 0
    for item in range(test_number_of_images):
        if y_val[item] == test_labels[item]:
            count += 1
    print method + ':' + str(float(count)/test_number_of_images)
def unroll_samples(samples):
    sample_n, var_n = samples.shape
    new_samples = numpy.zeros((sample_n * class_n, var_n+1), numpy.float32)
    new_samples[:,:-1] = numpy.repeat(samples, class_n, axis=0)
    new_samples[:,-1] = numpy.tile(numpy.arange(class_n), sample_n)
    return new_samples
def unroll_responses(responses):
    sample_n = len(responses)
    new_responses = numpy.zeros(sample_n*class_n, numpy.int32)
    resp_idx = numpy.int32( responses + numpy.arange(sample_n)*class_n )
    new_responses[resp_idx] = 1
    return new_responses
def readImage(filepath, re_size, number_of_images_set):
    
    f = open(filepath, 'rb')
    byte = f.read(4)
    magic_number = struct.unpack('>i',byte)[0]
    byte = f.read(4)
    number_of_images = struct.unpack('>i',byte)[0]
    if number_of_images_set != 0:
        number_of_images = number_of_images_set
    #number_of_images = 30000
    byte = f.read(4)
    number_of_rows = struct.unpack('>i',byte)[0]
    byte = f.read(4)
    number_of_cols = struct.unpack('>i',byte)[0]
    images_readed_count = 0
    images = numpy.array([], dtype = numpy.float32)
    while images_readed_count < number_of_images:
##        print 'starting read image' + str(images_readed_count) + \
##              ' of ' + str(number_of_images)
        byte = f.read(number_of_rows * number_of_cols)
        pixels = [numpy.float32(struct.unpack('B', item)[0])/numpy.float32(255) for item in byte]
        pixels = numpy.resize(pixels, (number_of_rows, number_of_cols))
        left = number_of_cols
        right = 0
        top = number_of_rows
        bottom = 0
        for i in range(number_of_rows):
            for j in range(number_of_cols):
                if pixels[i, j] > 0:
                    if j < left:
                        left = j
                    if j > right:
                        right = j
                    if i < top:
                        top = i
                    if i > bottom:
                        bottom = i
        if (bottom-top) > (right-left):
            length = bottom - top
        else:
            length = right - left
        midx = float(right + left)/2; midy = float(bottom + top)/2
        left = int(midx - float(length)/2)
        right = int(midx + float(length)/2) + 1
        top = int(midy - float(length)/2)
        bottom = int(midy + float(length)/2) + 1
        left = left if left > 0 else 0
        right = right if right < number_of_cols else number_of_cols
        top = top if top > 0 else 0
        bottom = bottom if bottom < number_of_rows else number_of_rows
        pixels = pixels[left:right, top:bottom]
##        print str(int(midx - float(length)/2)) + ':' + \
##              str(int(midx + float(length)/2)) + ',' + \
##              str(int(midy - float(length)/2)) + ':' + \
##              str(int(midy + float(length)/2))
        pixels = numpy.resize(cv2.resize(pixels, re_size), (1, re_size[0]*re_size[1]))
        if images.size == 0:
            images = numpy.array(pixels, dtype = numpy.float32)
        else:
            images = numpy.append(images, pixels, axis = 0)
        images_readed_count += 1
    f.close()
    return number_of_images, images
def readLabel(filepath, number_of_images_set):
    f = open(filepath, 'rb')
    byte = f.read(4)
    magic_number = struct.unpack('>i',byte)[0]
    byte = f.read(4)
    number_of_images = struct.unpack('>i',byte)[0]
    if number_of_images_set != 0:
        number_of_images = number_of_images_set
    #number_of_images = 10000
    
    images_readed_count = 0
    labels = numpy.array([], dtype = numpy.float32)
    while images_readed_count < number_of_images:
        byte = f.read(1)
        labels = numpy.append(labels, numpy.float32(struct.unpack('B', byte)[0]))
        images_readed_count += 1
    f.close()
    return number_of_images, labels
##[self.model.predict(s) for s in samples]
## params = dict( kernel_type = cv2.SVM_LINEAR,
##                       svm_type = cv2.SVM_C_SVC,
##                       C = 1 )
##
##samples = numpy.array(numpy.random.random((4,2)), dtype = numpy.float32)
##y_train = numpy.array([1.,0.,0.,1.], dtype = numpy.float32)
##
##clf = SVM()
##clf.train(samples, y_train)
##y_val = clf.predict(samples)
def ANN_Pridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
    ########ANN#########
    modelnn = cv2.ANN_MLP()
    sample_n, var_n = train_images.shape
    new_train_labels = unroll_responses(train_labels).reshape(-1, class_n)
    layer_sizes = numpy.int32([var_n, 100, class_n])
    modelnn.create(layer_sizes)
    params = dict( term_crit = (cv2.TERM_CRITERIA_COUNT, 300, 0.01),
                   train_method = cv2.ANN_MLP_TRAIN_PARAMS_BACKPROP,
                   bp_dw_scale = 0.001,
                   bp_moment_scale = 0.0 )
    modelnn.train(train_images, numpy.float32(new_train_labels), None, params = params)
    ret, resp = modelnn.predict(test_images)
    y_val_nn = resp.argmax(-1)
    evalfun('nn', y_val_nn, test_labels, test_number_of_images)
    
    return []
def KNearestPridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
    
    ########KNearest#########
    modelknn = cv2.KNearest()
    modelknn.train(train_images,train_labels)
    y_val_knn= modelknn.find_nearest(test_images, k = 3)
    count_knn = 0
    for item in range(test_number_of_images):
        if y_val_knn[1][item][0] == test_labels[item]:
            count_knn += 1
    print 'knn:'+str(float(count_knn)/test_number_of_images)
   
    return []
def SVM_Pridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
   
    #######SVM##########
    modelsvm = cv2.SVM()
    modelsvm.train(train_images, train_labels)#, params = params
    y_val_svm = [modelsvm.predict(test_image) for test_image in test_images]
    evalfun('svm', y_val_svm, test_labels, test_number_of_images)
    
    return []
def RTrees_Pridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
   
    #######RTrees##########
    modelRTtree = cv2.RTrees()
    sample_n, var_n = train_images.shape
    var_types = numpy.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL], numpy.uint8)
    params = dict(max_depth=10)
    modelRTtree.train(train_images, cv2.CV_ROW_SAMPLE, train_labels, varType = var_types, params = params)
    y_val_RTtree = numpy.float32( [modelRTtree.predict(s) for s in test_images] )
    evalfun('RTtree', y_val_RTtree, test_labels, test_number_of_images)
  
    return []
def Boost_Pridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
   
    #######Boost#########
    modelBoost = cv2.Boost()
    sample_n, var_n = train_images.shape
    new_train_images = unroll_samples(train_images)
    new_train_labels = unroll_responses(train_labels)
    var_types = numpy.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL, cv2.CV_VAR_CATEGORICAL], numpy.uint8)
    params = dict(max_depth=5) #, use_surrogates=False)
    modelBoost.train(new_train_images, cv2.CV_ROW_SAMPLE, new_train_labels, varType = var_types, params=params)
    new_test_images = unroll_samples(test_images)
    y_val_Boost = numpy.array( [modelBoost.predict(s, returnSum = True) for s in new_test_images] )
    y_val_Boost = y_val_Boost.reshape(-1, class_n).argmax(1)
    evalfun('Boost', y_val_Boost, test_labels, test_number_of_images)
    
    return []
def ML_Pridict():
    to_size = (8,8)
    number_of_training_set = 2000
    train_number_of_images, train_images = readImage(trainimagepath, to_size, number_of_training_set)
    train_number_of_images, train_labels = readLabel(trainlabelpath, number_of_training_set)
    ##train_images = train_images * 255
    ##train_images = cv2.normalize(train_images)
    
    number_of_test_set = 0
    test_number_of_images, test_images = readImage(testimagepath, to_size, number_of_test_set)
    test_number_of_images, test_labels = readLabel(testlabelpath, number_of_test_set)
    print 'loaded images and labels.'
    ########ANN#########
    modelnn = cv2.ANN_MLP()
    sample_n, var_n = train_images.shape
    new_train_labels = unroll_responses(train_labels).reshape(-1, class_n)
    layer_sizes = numpy.int32([var_n, 100, class_n])
    modelnn.create(layer_sizes)
    params = dict( term_crit = (cv2.TERM_CRITERIA_COUNT, 300, 0.01),
                   train_method = cv2.ANN_MLP_TRAIN_PARAMS_BACKPROP,
                   bp_dw_scale = 0.001,
                   bp_moment_scale = 0.0 )
    modelnn.train(train_images, numpy.float32(new_train_labels), None, params = params)
    ret, resp = modelnn.predict(test_images)
    y_val_nn = resp.argmax(-1)
    evalfun('nn', y_val_nn, test_labels, test_number_of_images)
    
    ########KNearest#########
    modelknn = cv2.KNearest()
    modelknn.train(train_images,train_labels)
    y_val_knn= modelknn.find_nearest(test_images, k = 3)
    count_knn = 0
    for item in range(test_number_of_images):
        if y_val_knn[1][item][0] == test_labels[item]:
            count_knn += 1
    print 'knn:'+str(float(count_knn)/test_number_of_images)
    #######SVM##########
    modelsvm = cv2.SVM()
    modelsvm.train(train_images, train_labels)#, params = params
    y_val_svm = [modelsvm.predict(test_image) for test_image in test_images]
    evalfun('svm', y_val_svm, test_labels, test_number_of_images)
    
    #######RTrees##########
    modelRTtree = cv2.RTrees()
    sample_n, var_n = train_images.shape
    var_types = numpy.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL], numpy.uint8)
    params = dict(max_depth=10)
    modelRTtree.train(train_images, cv2.CV_ROW_SAMPLE, train_labels, varType = var_types, params = params)
    y_val_RTtree = numpy.float32( [modelRTtree.predict(s) for s in test_images] )
    evalfun('RTtree', y_val_RTtree, test_labels, test_number_of_images)
    #######Boost#########
    modelBoost = cv2.Boost()
    sample_n, var_n = train_images.shape
    new_train_images = unroll_samples(train_images)
    new_train_labels = unroll_responses(train_labels)
    var_types = numpy.array([cv2.CV_VAR_NUMERICAL] * var_n + [cv2.CV_VAR_CATEGORICAL, cv2.CV_VAR_CATEGORICAL], numpy.uint8)
    params = dict(max_depth=5) #, use_surrogates=False)
    modelBoost.train(new_train_images, cv2.CV_ROW_SAMPLE, new_train_labels, varType = var_types, params=params)
    new_test_images = unroll_samples(test_images)
    y_val_Boost = numpy.array( [modelBoost.predict(s, returnSum = True) for s in new_test_images] )
    y_val_Boost = y_val_Boost.reshape(-1, class_n).argmax(1)
    evalfun('Boost', y_val_Boost, test_labels, test_number_of_images)
    
    return []
```
运行结果：
```python
tree.SVM_Pridict()
loaded images and labels.
svm:0.6251
```
