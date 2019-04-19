# Easy-PR学习笔记（三）：车牌区域精定位 - sinat_31425585的博客 - CSDN博客
2018年04月30日 12:32:45[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：421
由Easy-PR学习笔记（二）可知，基于边缘和颜色信息，可以获知一些候选的车牌区域，然后使用分类器，对候选区域做进一步确认，即可得到车牌的准确信息。
Easy-PR中采用SVM+HOG/直方图特征的方式来实施的，主要可以分为以下几个步骤：
**1、数据读取：**
数据读取实现部分位于util.cpp文件中的getFiles函数，使用io.h提供的文件操作接口，这样就可以获取folder文件夹目录下的所有图片对应绝对路径名。
```cpp
std::vector<std::string> Utils::getFiles(const std::string &folder,
                                         const bool all /* = true */) {
  std::vector<std::string> files;
  std::list<std::string> subfolders;
  subfolders.push_back(folder);
#ifdef OS_WINDOWS
  while (!subfolders.empty()) {
    std::string current_folder(subfolders.back());
    if (*(current_folder.end() - 1) != '/') {
      current_folder.append("/*");
    } else {
      current_folder.append("*");
    }
    subfolders.pop_back();
    struct _finddata_t file_info;
    auto file_handler = _findfirst(current_folder.c_str(), &file_info);
    while (file_handler != -1) {
      if (all &&
          (!strcmp(file_info.name, ".") || !strcmp(file_info.name, ".."))) {
        if (_findnext(file_handler, &file_info) != 0) break;
        continue;
      }
      if (file_info.attrib & _A_SUBDIR) {
        // it's a sub folder
        if (all) {
          // will search sub folder
          std::string folder(current_folder);
          folder.pop_back();
          folder.append(file_info.name);
          subfolders.push_back(folder.c_str());
        }
      } else {
        // it's a file
        std::string file_path;
        // current_folder.pop_back();
        file_path.assign(current_folder.c_str()).pop_back();
        file_path.append(file_info.name);
        files.push_back(file_path);
      }
      if (_findnext(file_handler, &file_info) != 0) break;
    }  // while
    _findclose(file_handler);
  }
  return files;
}
```
**2、特征提取：**
一个识别任务，与识别效果好坏直接关联的部分——特征设计。Easy-PR中提供了两个特征：HOG和直方图颜色特征，两种特征提取的实现部分位于feature.cpp文件中，HOG特征采用的是OpenCV提供的接口。
```cpp
void getHOGFeatures(const Mat& image, Mat& features) {
  //HOG descripter
  HOGDescriptor hog(cvSize(128, 64), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 3); //these parameters work well
	std::vector<float> descriptor;
  // resize input image to (128,64) for compute
	Size dsize = Size(128,64);
	Mat trainImg = Mat(dsize, CV_32S);
	resize(image, trainImg, dsize);
  // compute descripter
	hog.compute(trainImg, descriptor, Size(8, 8));
  // copy the result
	Mat mat_featrue(descriptor);
	mat_featrue.copyTo(features);
}
```
直方图颜色特征提取分为直方图特征和颜色特征两个部分：
```cpp
void getHistomPlusColoFeatures(const Mat& image, Mat& features) {
  // TODO
  Mat feature1, feature2;
  getHistogramFeatures(image, feature1);
  getColorFeatures(image, feature2);
  hconcat(feature1.reshape(1, 1), feature2.reshape(1, 1), features);
}
```
直方图特征分为两个部分：OTSU二值化+水平和垂直方向直方图（分别将二值图像做水平和垂直方向投影，并做直方图统计，然后将两个直方图进行合并）
```cpp
void getHistogramFeatures(const Mat& image, Mat& features) {
  Mat grayImage;
  cvtColor(image, grayImage, CV_RGB2GRAY);
  //grayImage = histeq(grayImage);
  Mat img_threshold;
  threshold(grayImage, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
  //Mat img_threshold = grayImage.clone();
  //spatial_ostu(img_threshold, 8, 2, getPlateType(image, false));
  features = getHistogram(img_threshold);
}
```
```cpp
Mat getHistogram(Mat in) {
  const int VERTICAL = 0;
  const int HORIZONTAL = 1;
  // Histogram features
  Mat vhist = ProjectedHistogram(in, VERTICAL);
  Mat hhist = ProjectedHistogram(in, HORIZONTAL);
  // Last 10 is the number of moments components
  int numCols = vhist.cols + hhist.cols;
  Mat out = Mat::zeros(1, numCols, CV_32F);
  int j = 0;
  for (int i = 0; i < vhist.cols; i++) {
    out.at<float>(j) = vhist.at<float>(i);
    j++;
  }
  for (int i = 0; i < hhist.cols; i++) {
    out.at<float>(j) = hhist.at<float>(i);
    j++;
  }
  return out;
}
```
颜色特征其实就是对HSV颜色模型中H分量进行归一化直方图统计：
```cpp
void getColorFeatures(const Mat& src, Mat& features) {
  Mat src_hsv;
  //grayImage = histeq(grayImage);
  cvtColor(src, src_hsv, CV_BGR2HSV);
  int channels = src_hsv.channels();
  int nRows = src_hsv.rows;
  // consider multi channel image
  int nCols = src_hsv.cols * channels;
  if (src_hsv.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
  }
  const int sz = 180;
  int h[sz] = { 0 };
  uchar* p;
  for (int i = 0; i < nRows; ++i) {
    p = src_hsv.ptr<uchar>(i);
    for (int j = 0; j < nCols; j += 3) {
      int H = int(p[j]);      // 0-180
      if (H > sz - 1) H = sz - 1;
      if (H < 0) H = 0;
      h[H]++;
    }
  }
  Mat mhist = Mat::zeros(1, sz, CV_32F);
  for (int j = 0; j < sz; j++) {
    mhist.at<float>(j) = (float)h[j];
  }
  // Normalize histogram
  double min, max;
  minMaxLoc(mhist, &min, &max);
  if (max > 0)
    mhist.convertTo(mhist, -1, 1.0f / max, 0);
  features = mhist;
}
```
然后，将直方图特征和颜色特征直接合并，就得到了直方图颜色特征。
**3、分类器训练**
这个部分就是将提取到的特征丢到SVM中进行训练，得到分类模型。首先，需要将得到的特征转换成SVM分类器能够直接使用的数据格式。
```cpp
cv::Ptr<cv::ml::TrainData> SvmTrain::tdata() {
  cv::Mat samples;
  std::vector<int> responses;
  for (auto f : train_file_list_) {
    auto image = cv::imread(f.file);
    if (!image.data) {
      fprintf(stdout, ">> Invalid image: %s  ignore.\n", f.file.c_str());
      continue;
    }
    cv::Mat feature;
    extractFeature(image, feature);
    feature = feature.reshape(1, 1);
    samples.push_back(feature);
    responses.push_back(int(f.label));
  }
  cv::Mat samples_, responses_;
  samples.convertTo(samples_, CV_32FC1);
  cv::Mat(responses).copyTo(responses_);
  return cv::ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE, responses_);
}
```
然后，将训练数据丢入SVM分类器，训练模型：
```cpp
void SvmTrain::train() {
  svm_ = cv::ml::SVM::create();
  svm_->setType(cv::ml::SVM::C_SVC);
  svm_->setKernel(cv::ml::SVM::RBF);
  svm_->setDegree(0.1);
  // 1.4 bug fix: old 1.4 ver gamma is 1
  svm_->setGamma(0.1); 
  svm_->setCoef0(0.1);
  svm_->setC(1);
  svm_->setNu(0.1);
  svm_->setP(0.1);
  svm_->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));
  this->prepare();
  if (train_file_list_.size() == 0) {
    fprintf(stdout, "No file found in the train folder!\n");
    fprintf(stdout, "You should create a folder named \"tmp\" in EasyPR main folder.\n");
    fprintf(stdout, "Copy train data folder(like \"SVM\") under \"tmp\". \n");
    return;
  }
  auto train_data = tdata();
  fprintf(stdout, ">> Training SVM model, please wait...\n");
  long start = utils::getTimestamp();
  svm_->trainAuto(train_data, 10, SVM::getDefaultGrid(SVM::C),
                  SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
                  SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
                  SVM::getDefaultGrid(SVM::DEGREE), true);
  //svm_->train(train_data);
  long end = utils::getTimestamp();
  fprintf(stdout, ">> Training done. Time elapse: %ldms\n", end - start);
  fprintf(stdout, ">> Saving model file...\n");
  svm_->save(svm_xml_);
  fprintf(stdout, ">> Your SVM Model was saved to %s\n", svm_xml_);
  fprintf(stdout, ">> Testing...\n");
  this->test();
  
}
```
**4、分类器测试；**
测试即让训练好的模型在测试集数据上跑一跑，看看模型的准确率。
```cpp
void SvmTrain::test() {
  // 1.4 bug fix: old 1.4 ver there is no null judge
  // if (NULL == svm_)
  LOAD_SVM_MODEL(svm_, svm_xml_);
  if (test_file_list_.empty()) {
    this->prepare();
  }
 
  double count_all = test_file_list_.size();
  double ptrue_rtrue = 0;
  double ptrue_rfalse = 0;
  double pfalse_rtrue = 0;
  double pfalse_rfalse = 0;
  for (auto item : test_file_list_) {
    auto image = cv::imread(item.file);
    if (!image.data) {
      std::cout << "no" << std::endl;
      continue;
    }
    cv::Mat feature;
    extractFeature(image, feature);
    auto predict = int(svm_->predict(feature));
    //std::cout << "predict: " << predict << std::endl;
    auto real = item.label;
    if (predict == kForward && real == kForward) ptrue_rtrue++;
    if (predict == kForward && real == kInverse) ptrue_rfalse++;
    if (predict == kInverse && real == kForward) pfalse_rtrue++;
    if (predict == kInverse && real == kInverse) pfalse_rfalse++;
  }
  std::cout << "count_all: " << count_all << std::endl;
  std::cout << "ptrue_rtrue: " << ptrue_rtrue << std::endl;
  std::cout << "ptrue_rfalse: " << ptrue_rfalse << std::endl;
  std::cout << "pfalse_rtrue: " << pfalse_rtrue << std::endl;
  std::cout << "pfalse_rfalse: " << pfalse_rfalse << std::endl;
  double precise = 0;
  if (ptrue_rtrue + ptrue_rfalse != 0) {
    precise = ptrue_rtrue / (ptrue_rtrue + ptrue_rfalse);
    std::cout << "precise: " << precise << std::endl;
  } else {
    std::cout << "precise: "
              << "NA" << std::endl;
  }
  double recall = 0;
  if (ptrue_rtrue + pfalse_rtrue != 0) {
    recall = ptrue_rtrue / (ptrue_rtrue + pfalse_rtrue);
    std::cout << "recall: " << recall << std::endl;
  } else {
    std::cout << "recall: "
              << "NA" << std::endl;
  }
  double Fsocre = 0;
  if (precise + recall != 0) {
    Fsocre = 2 * (precise * recall) / (precise + recall);
    std::cout << "Fsocre: " << Fsocre << std::endl;
  } else {
    std::cout << "Fsocre: "
              << "NA" << std::endl;
  }
}
```
这样就训练好了模型，在进行车牌精确定位时，需要载入训练好的模型，对候选矩形框区域进行预测即可。
```cpp
int PlateJudge::plateSetScore(CPlate& plate) {
    Mat features;
    extractFeature(plate.getPlateMat(), features);
    float score = svm_->predict(features, noArray(), cv::ml::StatModel::Flags::RAW_OUTPUT);
    //std::cout << "score:" << score << std::endl;
    if (0) {
      imshow("plate", plate.getPlateMat());
      waitKey(0);
      destroyWindow("plate");
    }
    // score is the distance of margin，below zero is plate, up is not
    // when score is below zero, the samll the value, the more possibliy to be a plate.
    plate.setPlateScore(score);
    if (score < 0.5) return 0;
    else return -1;      
  }
```
最后，使用NMS做进一步判断，其实就是Rect1交Rect2 除以 Rect1并Rect2
```cpp
float computeIOU(const Rect &rect1, const Rect &rect2) {
    Rect inter = interRect(rect1, rect2);
    Rect urect = mergeRect(rect1, rect2);
    float iou = (float) inter.area() / (float) urect.area();
    return iou;
  }
```
补充一下：
首先给出两个定义： 
（1）区域的交并比（IOU：Intersection over Union）：相交部分面积除以两个区域的并集； 
（2）NMS：非极大值抑制； 
    举个例子：
![](https://img-blog.csdn.net/20180715125702907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    IOU = 黄色区域/绿色区域
    交并比用来衡量两个候选区域的重叠度，如果交并比较大，就表示两个区域间存在较大重叠，我们就认为两个区域其实表示同一个物体位置，那么，必然两个候选区域中，有一个更加精确，另外一个可以直接忽略。 那么哪一个更加精确勒？
![](https://img-blog.csdn.net/201807151316117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    如图所示，我就将两个区域对应图像块丢到训练好的SVM分类器中进行分类，会得到两个区域为目标物体的概率，概率大的，我们认为该区域为目标区域的可能性较大，就将另外一个忽略掉（抑制掉），这就是非极大值抑制，保留最大概率（可能性）区域，抑制较小概率（可能性）区域。 当然这里可能不仅仅是两个重叠区域，可能是三个四个五个都行，做法是一样的。

