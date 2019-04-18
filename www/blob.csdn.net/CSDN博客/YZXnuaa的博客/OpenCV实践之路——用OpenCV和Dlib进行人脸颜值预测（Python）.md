# OpenCV实践之路——用OpenCV和Dlib进行人脸颜值预测（Python） - YZXnuaa的博客 - CSDN博客
2018年06月19日 11:44:14[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：194
### 本项目部分基于这个博客[Computer Vision for Predicting Facial Attractiveness](http://www.learnopencv.com/computer-vision-for-predicting-facial-attractiveness/)。按照上面这个博客复现起来比较麻烦（当然是对我而言），而且我想要做的是对于任意一张人脸进行检测之后都可以进行颜值估计，但是他给出的代码还需要进行改动不少才行。现在就把自己的复现过程记录下来。
本文中只展示了部分关键代码。完整代码和测试用的数据可以在[GitHub](https://github.com/LiuXiaolong19920720/predict-facial-attractiveness)上获取。
### 不同点
- 在提取人脸关键点的时候，原文中提到用的是[CLM framework](https://github.com/TadasBaltrusaitis/CLM-framework)，本文用的是之前配置好的Dlib。
- 实现了对自选图片中人脸的颜值估计
### 用到的Python库
- numpy
- opencv
- dlib
- sklearn
### 主要步骤
- 训练模型
- 人脸关键点提取
- 特征生成
- 颜值预测
#### 1. 训练模型
训练一个模型最重要的就是输入数据和模型的选取。这里训练模型用到的输入数据是给出的features_All.txt和rating.txt。这是[华南理工大学](http://www.hcii-lab.net/data/SCUT-FBP/EN/introduce.html)人机智能交互实验室给出的数据集中提取出来的特征和标签。一共用了500幅人脸图像。
```
root = 'E:/Github/predict-facial-attractiveness/'
features = np.loadtxt(root + 'data/features_ALL.txt', delimiter=',')
#features = preprocessing.scale(features)
features_train = features[0:-50]
features_test = features[-50:]
pca = decomposition.PCA(n_components=20)
pca.fit(features_train)
features_train = pca.transform(features_train)
features_test = pca.transform(features_test)
ratings = np.loadtxt(root + 'data/ratings.txt', delimiter=',')
#ratings = preprocessing.scale(ratings)
ratings_train = ratings[0:-50]
ratings_test = ratings[-50:]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 值得注意的是，这里用PCA算法对特征进行了压缩和降维。降维之后特征变成了20维，也就是说特征一共有500行，每行是一个人的特征向量，每个特征向量有20个元素。
根据[Computer Vision for Predicting Facial Attractiveness](http://www.learnopencv.com/computer-vision-for-predicting-facial-attractiveness/)一文中给出的结果，我们选取表现最好的模型之一——随机森林作为此次复现用的算法模型。
```python
regr = RandomForestRegressor(n_estimators=50, max_depth=None, min_samples_split=1, random_state=0)
regr = regr.fit(features_train, ratings_train)
#一定要把conpress设为true或者其他的值，没有设置会输出很多的*.pkl
joblib.dump(regr, root + 'model/my_face_rating.pkl',compress=1) 
print "Generate Model Successfully!"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 用随机森林训练模型。因为想要预测自选图片的人脸颜值，所以要把模型保存下来以备后用。
#### 2. 人脸关键点提取
在之前的博客[OpenCV实践之路——用dlib库进行人脸检测与人脸标记](http://blog.csdn.net/xingchenbingbuyu/article/details/51116354)中已经提到过如何进行人脸关键点提取，这里的工作基于之前的内容，不同之处在于要把提取到的关键点按照模型可以用的方式进行保存。部分代码如下：
```
f = open(root + 'data/landmarks.txt','w')
for i in range(len(rects)):  
    使用predictor进行人脸关键点识别  
    landmarks = numpy.matrix([[p.x,p.y] for p in predictor(im,rects[i]).parts()])  
    im = im.copy()  
    #使用enumerate 函数遍历序列中的元素以及它们的下标  
    for idx,point in enumerate(landmarks):  
        pos = (point[0,0],point[0,1])  
        f.write(str(point[0,0]))
        f.write(',')
        f.write(str(point[0,1]))
        f.write(',')
        #cv2.putText(im,str(idx),pos,                  
        #6.绘制特征点  
        cv2.circle(im,pos,3,color=(0,255,0))  
    f.write('\n')   
print "landmarks,get!"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
### 3. 特征生成
特征生成直接用了原始代码，只对路径进行了一些改动。这一部分我暂时也不太明朗，所以不敢多说。有兴趣的还是直接看代码吧。篇幅原因，这里只是部分代码，特征生成的完整代码在[这里](https://github.com/LiuXiaolong19920720/predict-facial-attractiveness/blob/master/source/generateFeatures.py)。
```python
def generateFeatures(pointIndices1, pointIndices2, pointIndices3, pointIndices4, allLandmarkCoordinates):
    size = allLandmarkCoordinates.shape
    allFeatures = numpy.zeros((size[0], len(pointIndices1)))
    for x in range(0, size[0]):
        landmarkCoordinates = allLandmarkCoordinates[x, :]
        ratios = [];
        for i in range(0, len(pointIndices1)):
            x1 = landmarkCoordinates[2*(pointIndices1[i]-1)]
            y1 = landmarkCoordinates[2*pointIndices1[i] - 1]
            x2 = landmarkCoordinates[2*(pointIndices2[i]-1)]
            y2 = landmarkCoordinates[2*pointIndices2[i] - 1]
            x3 = landmarkCoordinates[2*(pointIndices3[i]-1)]
            y3 = landmarkCoordinates[2*pointIndices3[i] - 1]
            x4 = landmarkCoordinates[2*(pointIndices4[i]-1)]
            y4 = landmarkCoordinates[2*pointIndices4[i] - 1]
            points = [x1, y1, x2, y2, x3, y3, x4, y4]
            ratios.append(facialRatio(points))
        allFeatures[x, :] = numpy.asarray(ratios)
    return allFeatures
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
#### 4. 颜值预测
到了这一步，我们就可以用第一步得到的模型和上一步得到的特征进行颜值的估计了。
```python
#use your own path
root = 'E:/Github/predict-facial-attractiveness/'
clf = joblib.load(root+'model/my_face_rating.pkl')
features = np.loadtxt(root + 'data/features_ALL.txt', delimiter=',')
my_features = np.loadtxt(root + 'data/my_features.txt', delimiter=',')
pca = decomposition.PCA(n_components=20)
pca.fit(features)
predictions = np.zeros([6,1]);
for i in range(0, 6):
    features_test = features[i, :]
    features_test = pca.transform(features_test)
    #regr = linear_model.LinearRegression()
    #regr.fit(features_train, ratings_train)
    predictions[i] = clf.predict(features_test)
#predictions = clf.predict(features)
print predictions
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 这里有一点需要注意：一定要用原来的特征进行PCA的fit，因为自己的图片提取的只有六个特征，而PCA要压缩成20维，根本没法压缩，所以先用原本的特征进行PCA的fit，这样即使只有一个特征，也可以降维到20列。否则会出错。
仍然用这张图片测试
![](https://img-blog.csdn.net/20160411000937910)
结果如下图所示，可能是因为训练的时候用的都是亚洲女性的图片，所以测试欧美女性的时候有一些偏差，本结果只是测试结果，不说明任何问题。
![这里写图片描述](https://img-blog.csdn.net/20161013142307734)
- 满分5分
