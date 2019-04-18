# python 基于机器学习—深度学习识别验证码 - wsp_1138886114的博客 - CSDN博客





2018年10月07日 11:32:53[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：974











### 文章目录
- [一、前言](#_1)
- [项目的整个识别流程：](#_3)
- [关于数据集](#_8)
- [二、验证码清理并生成训练集样本](#_16)
- [2.1 获取图像和标签](#21__17)
- [2.2 初步去除验证码的噪点—并且深度清理—保存到训练集](#22__34)
- [2.3 字符切割](#23__69)
- [三、特征提取](#_120)
- [3.1 这里我构建字体的HOG特征，作为图像特征](#31_HOG_121)
- [3.2 原文采用数字所占用的像素位置构建特征向量](#32__134)
- [四、模型训练](#_161)
- [4.1 原文采用 SVM中的SVC、以及random forest。](#41__SVMSVCrandom_forest_162)
- [五、模型预测](#_179)




### 一、前言

开发环境：`Anaconda | python 3.5 —pycharm / jupyter notebook`

##### 项目的整个识别流程：

① 验证码清理并生成训练集样本

② 验证码特征提取

③ 拟合识别模型

④ 识别模型测试
##### 关于数据集

数据集来自：[https://github.com/wzzzd/captcha_ml。验证码是数字+大写字母+小写字母的组合，即[0-9]+[A-Z]+[a-z]。文件名是验证码的正确数字标签，实例如下](https://github.com/wzzzd/captcha_ml%E3%80%82%E9%AA%8C%E8%AF%81%E7%A0%81%E6%98%AF%E6%95%B0%E5%AD%97+%E5%A4%A7%E5%86%99%E5%AD%97%E6%AF%8D+%E5%B0%8F%E5%86%99%E5%AD%97%E6%AF%8D%E7%9A%84%E7%BB%84%E5%90%88%EF%BC%8C%E5%8D%B3%5B0-9%5D+%5BA-Z%5D+%5Ba-z%5D%E3%80%82%E6%96%87%E4%BB%B6%E5%90%8D%E6%98%AF%E9%AA%8C%E8%AF%81%E7%A0%81%E7%9A%84%E6%AD%A3%E7%A1%AE%E6%95%B0%E5%AD%97%E6%A0%87%E7%AD%BE%EF%BC%8C%E5%AE%9E%E4%BE%8B%E5%A6%82%E4%B8%8B)
![在这里插入图片描述](https://img-blog.csdn.net/2018100710571862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用三个数据集：

①训练集（training set）:10000张验证码

②测试集（test set）：100张验证码

③验证集（validation set）：100张验证码
### 二、验证码清理并生成训练集样本

##### 2.1 获取图像和标签

```python
```python
"""
    :param input_path：                图像输入目录
    :return: image_array, image_label：获取的图像，标签
"""
def read_captcha(input_path):
    image_array = []
    image_label = []
    file_list = os.listdir(input_path)               # 获取captcha文件
    for file in file_list:
        image = Image.open(input_path+ '/' + file)   # 打开图片
        file_name = file.split(".")[0]               # 获取文件名，此为图片标签
        image_array.append(image)
        image_label.append(file_name)
    return image_array, image_label
```
```

##### 2.2 初步去除验证码的噪点—并且深度清理—保存到训练集

```python
```python
def image_transfer(image_arry):
    """
    :param image_arry:   图像list，每个元素为一副图像
    :return: image_clean:清理过后的图像list
    """
    image_clean = []
    for i, image in enumerate(image_arry):
        image = image.convert('L')             # 转换为灰度图像，即RGB通道从3变为1
        im2 = Image.new("L", image.size, 255)
        for y in range(image.size[1]):         # 遍历所有像素，将灰度超过阈值的像素转变为255（白）
            for x in range(image.size[0]):
                pix = image.getpixel((x, y))
                if int(pix) > threshold_grey:  # 灰度阈值
                    im2.putpixel((x, y), 255)
                else:
                    im2.putpixel((x, y), pix)
        image_clean.append(im2)
    return image_clean
------------------------------------------------------------------
#或者
import cv2

def image_transfer(image_arry)
    img_gray = cv2.cvtColor(image_arry, cv2.COLOR_BGR2GRAY)
    # 设定阈值（阈值大小影响去噪效果） 二值化去除噪点
    (_, thresh) = cv2.threshold(img_gray, 90, 255, cv2.THRESH_BINARY)
    return thresh
```
```

通过粗清理的办法，无法完全去除所有噪声点。此处引入了更细粒度的清理方法。

主要有3大步骤：

step 1：找出图像中所有的孤立点；

step 2：计算黑色点近邻9宫格中黑色点个数，若小于等于2个，那么认为该点为噪声点；

step 3：去除所有噪声点。
##### 2.3 字符切割

去除孤立点后，我们还是没法一下子就识别出这四个字符，需要对经过处理后的图片进行切分。

切割方式主要有一下步骤：

step 1：找出图片中所有分离图像的开始结束位置。遍历width&height，当每出现一个黑色点，记为该字符开始位置；当新的一列出现全白色点，那么记为结束位置。[(8, 9), (14, 22), (29, 38), (42, 50), (57, 66)]

step 2：尽管经过清理后，还是可能存在噪声点。在找到所有切割开始结束位置后，计算并选出（结束值-开始值）最大的切割位置。[(14, 22), (29, 38), (42, 50), (57, 66)]
```python
```python
def image_split(image):
    """
    :param image:单幅图像
    :return:     单幅图像被切割后的图像list
    """
    inletter = False    #找出每个字母开始位置
    foundletter = False #找出每个字母结束位置
    start = 0
    end = 0
    letters = []    #存储坐标
    for x in range(image.size[0]):
        for y in range(image.size[1]):
            pix = image.getpixel((x, y))
            if pix != True:
                inletter = True
        if foundletter == False and inletter == True:
            foundletter = True
            start = x
        if foundletter == True and inletter == False:
            foundletter = False
            end = x
            letters.append((start, end))
        inletter = False
 
    # 因为切割出来的图像有可能是噪声点
    # 筛选可能切割出来的噪声点,只保留开始结束位置差值最大的位置信息
    subtract_array = []    # 存储 结束-开始 值
    for each in letters:
        subtract_array.append(each[1]-each[0])
    reSet = sorted(subtract_array, key=lambda x:x, reverse=True)[0:image_character_num]
    letter_chioce = []    # 存储 最终选择的点坐标
    for each in letters:
        if int(each[1] - each[0]) in reSet:
            letter_chioce.append(each)
 
    image_split_array = []    #存储切割后的图像
    for letter in letter_chioce:
        im_split = image.crop((letter[0], 0, letter[1], image.size[1])) # (切割的起始横坐标，起始纵坐标，切割的宽度，切割的高度)
        im_split = im_split.resize((image_width, image_height)) # 转换格式
        image_split_array.append(im_split)
 
     return image_split_array[0:int(image_character_num)]
```
```

将按上述方法切分后的单个数字、字母，保存到新建的文件夹里，专门用来作为模型的训练集。

### 三、特征提取

##### 3.1 这里我构建字体的HOG特征，作为图像特征

关于HOG的使用方法：[https://www.programcreek.com/python/example/84776/cv2.HOGDescriptor](https://www.programcreek.com/python/example/84776/cv2.HOGDescriptor)

```python
```python
list_hog_fd = [] 
for feature in image_split_array:
    fd = hog(feature.reshape((28, 28)),     # hog 特征
             orientations=9, 
             pixels_per_cell=(14, 14), 
             cells_per_block=(1, 1), 
             visualise=False)
    list_hog_fd.append(fd)
hog_features = np.array(list_hog_fd, 'float64')
```
```

##### 3.2 原文采用数字所占用的像素位置构建特征向量

统计每个字符图像每一行像素值为黑色的总和（灰度值为0），加上每一列像素值为黑色的总和。因为我们切割后的图像大小为8*26（width*height），故特征个数为34=8+26。当然此处其实可以把单字符图像按像素值展开为一个208=8*26的向量，以此作为特征向量

```python
```python
def feature_transfer(image):
    """
    :param image （图像list）
    :return:feature （特征list）
    """
    image = image.resize((image_width, image_height)) #标准化图像格式
 
    feature = []                  # 存储计算特征
    for x in range(image_width):  # 计算行特征
        feature_width = 0
        for y in range(image_height):
            if image.getpixel((x, y)) == 0:
                feature_width += 1
        feature.append(feature_width)
 
    for y in range(image_height): # 计算列特征
        feature_height = 0
        for x in range(image_width):
            if image.getpixel((x, y)) == 0:
                feature_height += 1
        feature.append(feature_height)
    print("feature vector:",feature)
    return feature
```
```

### 四、模型训练

##### 4.1 原文采用 SVM中的SVC、以及random forest。

```python
```python
def trainModel(data, label):
    print("fit model >>>>>>>>>>>>>>>>>>>>>>")
 
    # svc_rbf = svm.SVC(decision_function_shape='ovo',kernel='rbf')    # rbf核svc
    # svc_linear = svm.SVC(decision_function_shape='ovo',kernel='linear')    #linear核svc
    rf = RandomForestClassifier(n_estimators=100, max_depth=10,min_samples_split=10, random_state=0)    #随机森林
    scores = cross_val_score(rf, data, label,cv=10)    #交叉检验，计算模型平均准确率
    print("rf: ",scores.mean())
    
    rf.fit(data, label)         # 拟合模型
    joblib.dump(rf, model_path) # 模型持久化，保存到本地
    print("model save success!")
 
    return rf
```
```

### 五、模型预测

① 读取将要识别的验证码文件

② 验证码粗清理。将灰度值小于阈值的像素值转化为255。

③ 验证码细清理。找出所有孤立的噪声点，并将该像素值转化为255。

④ 字符切割。找出所有字符的开始结束位置，并切割出4幅图像。

⑤ 图像特征提取。对于4幅图像中的每一幅，分别从行、列统计其灰度值为0（黑色）的和，构建4个特征向量。

⑥ 识别。读取训练好的模型，分别对4个特征向量进行识别，得到4个预测结果。

⑦ 输出。将识别出的4个字符结果，串起来，并输出到结果文件。
```python
```python
import os
from captcha_test.captcha_soc import image_process, image_feature, image_model, image_training
from sklearn.externals import joblib
from captcha_test.captcha_soc.config import *
 
 
#验证码数据清洗：转成灰度图（高斯去噪）-> 去噪 -> 二值化 -> 字符分割
def clean():
    image_array, image_label = image_process.read_captcha(test_data_path) 
    print("待测试的验证码数量：", len(image_array))
    image_clean = image_process.image_transfer(image_array) #转换成灰度图像，并去除背景噪声
    image_array = [] #[[im_1_1,im_1_2,im_1_3,im_1_4],[im_2_1,im_2_2,im_2_3,im_2_4],...]
    for each_image in image_clean:
        image_out = image_process.get_clear_bin_image(each_image) #转换为二值图片，并去除剩余噪声点
        split_result = image_process.image_split(image_out) #切割图片
        image_array.append(split_result)
    return image_array, image_label
 
 
#特征矩阵生成
def featrue_generate(image_array):
    feature = []
    for num, image in enumerate(image_array):
        feature_each_image = []
        for im_meta in image:
            fea_vector = image_feature.feature_transfer(im_meta)
            # print('label: ',image_label[num])
            # print(feature)
            feature_each_image.append(fea_vector)
            # print(fea_vector)
        # print(len(feature_each_image))
        if len(feature_each_image) == 0:
            feature_each_image = [[0]*(image_width+image_height)]*int(image_character_num)
        # print(feature_each_image)
        feature.append(feature_each_image)
    print("预测数据的长度:", len(feature))
    print("预测数据特征示例:", feature[0])
    return feature
 
 
#将结果写到文件
def write_to_file(predict_list):
    file_list = os.listdir(test_data_path)
    with open(output_path, 'w') as f:
        for num, line in enumerate(predict_list):
            if num == 0:
                f.write("file_name\tresult\n")
            f.write(file_list[num] + '\t' + line + '\n')
    print("结果输出到文件：", output_path)
 
def main():
    image_array, image_label = clean()         #验证码清理
    feature = featrue_generate(image_array)    #特征处理
    predict_list = []                          #预测
    acc = 0
    model = joblib.load(model_path)            #读取模型

    for num, line in enumerate(feature):
        predict_array = model.predict(line)
        predict = ''.join(predict_array)
        predict_list.append(predict)
        if predict == image_label[num]:
            acc += 1
        else:
            pass
            print("-----------------------")
            print("actual:",image_label[num])
            print("predict:", predict)
    print("测试集预测acc：", acc/len(image_label))
    write_to_file(predict_list)                #输出到文件
 
if __name__ == '__main__':
    main()
```
```

未完待续

特别鸣谢
[https://blog.csdn.net/Neleuska/article/details/80040304](https://blog.csdn.net/Neleuska/article/details/80040304)








