# OpenCV实现手写体数字训练与识别 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年09月12日 10:39:06[gloomyfish](https://me.csdn.net/jia20003)阅读数：11620








# OpenCV实现手写体数字训练与识别

机器学习(ML)是OpenCV模块之一，对于常见的数字识别与英文字母识别都可以做到很高的识别率，完成这类应用的主要思想与方法是首选对训练图像数据完成预处理与特征提取，根据特征数据组成符合OpenCV要求的训练数据集与标记集，然后通过机器学习的KNN、SVM、ANN等方法完成训练，训练结束之后保存训练结果，对待检测的图像完成分割、二值化、ROI等操作之后，加载训练好的分类数据，就可以预言未知分类。

## 一：数据集

这里使用的数据集是mnist 手写体数字数据集、关于数据集的具体说明如下：
|数据集名称|说明|
|----|----|
|train-images-idx3-ubyte.gz|训练图像28x28大小，6万张|
|train-labels-idx1-ubyte.gz|每张图像的数字标记，6万条|
|t10k-images-idx3-ubyte.gz|测试数据集、1万张图像28x28|
|t10k-labels-idx1-ubyte.gz|测试数据集标记，表示图像数字|

上述数据集数据组成内部结构，图像是以灰度每个字节表示一个像素点的灰度值，图像的总数、宽与高的大小从开始位置读取，说明如下：
|开始移位|类型|值|描述|
|----|----|----|----|
|0000|4字节int类型|0x00000803(2051)|魔数|
|0004|4字节int类型|60000|图像数目|
|0008|4字节int类型|28|图像高度|
|00012|4字节int类型|28|图像宽度|

标记部分数据组成如下：
|开始移位|类型|值|描述|
|----|----|----|----|
|0000|4字节int类型|0x00000801(2049)|魔数|
|0004|4字节int类型|60000|标记数目|
|0008|1字节ubyte|??|对应图像数字|
|0009|1字节ubyte|??|对应图像数字|

- **读取图像数据集**

```cpp
Mat readImages(int opt) {
    int idx = 0;
    ifstream file;
    Mat img;
    if (opt == 0)
    {
        cout << "\n Training...";
        file.open("D:/vcprojects/images/mnist/train-images.idx3-ubyte", ios::binary);
    }
    else
    {
        cout << "\n Test...";
        file.open("D:/vcprojects/images/mnist/t10k-images.idx3-ubyte", ios::binary);
    }
    // check file
    if (!file.is_open())
    {
        cout << "\n File Not Found!";
        return img;
    }
    /*
    byte 0 - 3 : Magic Number(Not to be used)
    byte 4 - 7 : Total number of images in the dataset
    byte 8 - 11 : rows of each image in the dataset
    byte 12 - 15 : cols of each image in the dataset
    */
    int magic_number = 0;
    int number_of_images = 0;
    int height = 0;
    int width = 0;

    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);

    file.read((char*)&number_of_images, sizeof(number_of_images));
    number_of_images = reverseDigit(number_of_images);

    file.read((char*)&height, sizeof(height));
    height = reverseDigit(height);

    file.read((char*)&width, sizeof(width));
    width = reverseDigit(width);

    Mat train_images = Mat(number_of_images, height*width, CV_8UC1);
    cout << "\n No. of images:" << number_of_images <<endl;
    Mat digitImg = Mat::zeros(height, width, CV_8UC1);
    for (int i = 0; i < number_of_images; i++) {
        int index = 0;  
        for (int r = 0; r<height; ++r) {
            for (int c = 0; c<width; ++c) {
                unsigned char temp = 0;
                file.read((char*)&temp, sizeof(temp));
                index = r*width + c;
                train_images.at<uchar>(i, index) = (int)temp;
                digitImg.at<uchar>(r, c) = (int)temp;
            }
        }
        if (i < 100) {
            imwrite(format("D:/vcprojects/images/mnist/images/digit_%d.png", i), digitImg);
        }
    }
    train_images.convertTo(train_images, CV_32FC1);
    return train_images;
}
```
- **读取标记数据集**

```cpp
Mat readLabels(int opt) {
    int idx = 0;
    ifstream file;
    Mat img;
    if (opt == 0)
    {
        cout << "\n Training...";
        file.open("D:/vcprojects/images/mnist/train-labels.idx1-ubyte");
    }
    else
    {
        cout << "\n Test...";
        file.open("D:/vcprojects/images/mnist/t10k-labels.idx1-ubyte");
    }
    // check file
    if (!file.is_open())
    {
        cout << "\n File Not Found!";
        return img;
    }
    /*
    byte 0 - 3 : Magic Number(Not to be used)
    byte 4 - 7 : Total number of labels in the dataset
    */
    int magic_number = 0;
    int number_of_labels = 0;

    file.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseDigit(magic_number);
    file.read((char*)&number_of_labels, sizeof(number_of_labels));
    number_of_labels = reverseDigit(number_of_labels);

    cout << "\n No. of labels:" << number_of_labels << endl;
    Mat labels = Mat(number_of_labels, 1, CV_8UC1);
    for (long int i = 0; i<number_of_labels; ++i)
    {
        unsigned char temp = 0;
        file.read((char*)&temp, sizeof(temp));
        //printf("temp : %d\n ", temp);
        labels.at<uchar>(i, 0) = temp;
    }
    labels.convertTo(labels, CV_32SC1);
    return labels;
}
```

## 二：训练与测试

对上述数据集，我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们最终的识别率。

**KNN方式**

KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。训练的代码如下：

```
void knnTrain() {
    Mat train_images = readImages(0);
    Mat train_labels = readLabels(0);
    printf("\n read mnist train dataset successfully...\n");
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    knn->setDefaultK(5);
    knn->setIsClassifier(true);
    Ptr<ml::TrainData> tdata = ml::TrainData::create(train_images, ml::ROW_SAMPLE, train_labels);
    knn->train(tdata);
    knn->save("D:/vcprojects/images/mnist/knn_knowledge.yml");
}
```
- **测试代码如下：**

```cpp
void testMnist() {
    //Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>("D:/vcprojects/images/mnist/knn_knowledge.yml"); // SVM-POLY - 98%
    Ptr<ml::KNearest> knn = Algorithm::load<ml::KNearest>("D:/vcprojects/images/mnist/knn_knowledge.yml"); // KNN - 97%
    Mat train_images = readImages(1);
    Mat train_labels = readLabels(1);
    printf("\n read mnist test dataset successfully...\n");

    float total = train_images.rows;
    float correct = 0;
    Rect rect;
    rect.x = 0;
    rect.height = 1;
    rect.width = (28 * 28);
    for (int i = 0; i < total; i++) {
        int actual = train_labels.at<int>(i);
        rect.y = i;
        Mat oneImage = train_images(rect);
        //int digit = svm->predict(oneImage);
        Mat result;
        float predicted = knn->predict(oneImage, result);
        int digit = static_cast<int>(predicted);
        if (digit == actual) {
            correct++;
        }
    }
    printf("\n recognize rate : %.2f \n", correct / total);
}
```

**SVM方式**

SVM的全称是支掌向量机，本来是用来对数据进行二分类的预测与分析、后来扩展到可以对数据进行回归与多分类预测与分析，主要是把数据映射到高维数据空间、把靠近高维数据的部分称为支掌向量(SV)。SVM根据使用的核不同、参数不同，可以得到不同的分类与预测结果、所以在OpenCV中使用SVM做分类的时候，尽量推荐大家使用train_auto方法来训练、但是train_auto运行时间一般都会比较久，有时候可能长达数天。

## 三：应用

训练好的数据保存在本地，初始化加载，使用对象的识别方法就可以预测分类、进行对象识别。当然这么做，还需要对输入的手写数字图像进行二值化、分割、调整等预处理之后才可以传入进行预测。完整的步骤如下： 
![这里写图片描述](https://img-blog.csdn.net/20170912103418233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- **以下是两个测试图像识别结果：**
![这里写图片描述](https://img-blog.csdn.net/20170912103505066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170912103517272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- **注意点：**
最终要把图像Mat对象转换为CV_32FC1的灰度，否则可能报错！

欢迎继续关注本人博客，只分享干货！





