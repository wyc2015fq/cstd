
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/ml/ml.hpp"

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "utils.h"

#define qDebug()  std::cout


int test_hsv_bin() {

  std::vector<string> filenames;
  LoadTextFileList("E:/OCR_Line/demo_images/list.txt", filenames);
  int k;
  for (k = 0; k < filenames.size(); ++k) {
    string fn = filenames[k];
    printf("%s\n", fn.c_str());
    cv::Mat srcImage = cv::imread(fn, IMREAD_COLOR);
    if (srcImage.empty()) {
      continue;
    }
    if (srcImage.cols>600) {
      double t = 600.*1. / srcImage.cols;
      cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
    }

    cv::Mat image(srcImage);
    cv::Mat frameHSV;
    cv::cvtColor(image, frameHSV, CV_BGR2HSV);

    cv::Mat dstTemp1(image.rows, image.cols, CV_8UC1);
    cv::inRange(frameHSV, Scalar(0, 0, 0), Scalar(180, 255, 100), dstTemp1);
    if (1) {
      cv::imshow("dstTemp1", dstTemp1);
      cv::imshow("srcImage", srcImage);
      cv::waitKey(0);
    }
  }
  return 0;
}

int bin_by_black(const Mat &srcImage, Mat& out) {
  cv::Mat image(srcImage);
  cv::Mat frameHSV;
  cv::cvtColor(image, frameHSV, CV_BGR2HSV);

  cv::Mat dstTemp1(image.rows, image.cols, CV_8UC1);
  cv::inRange(frameHSV, Scalar(50, 0, 90), Scalar(150, 50, 250), dstTemp1);
  out = dstTemp1;
  return 0;
}

bool rect_area_greater(const Rect& r1, const Rect& r2) {
  return r1.area() > r2.area();
}

int posDetect0(const Mat &inRGB, const Mat &in, Rect& out) {
  Mat threshold_R;
  Mat threshold_Inv;
  bin_by_black(inRGB, threshold_Inv);

  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));  //闭形态学的结构元素
  morphologyEx(threshold_Inv, threshold_Inv, CV_MOP_ERODE, element);

  imshow("threshold_R", threshold_Inv);
  vector< vector <Point> > contours;
  findContours(threshold_Inv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓

  vector< vector <Point> > ::iterator itc = contours.begin();
  vector<Rect> rects;
  while (itc != contours.end())
  {
    RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
    Rect r = mr.boundingRect();
    rects.push_back(r);
    ++itc;
  }
  Mat RGB = inRGB;
  if (rects.size() > 1) {
    std::sort(rects.begin(), rects.end(), rect_area_greater);
    out = rects[0];
    int n = (int)MIN(2, rects.size());
    for (int i = 1; i < n; ++i) {
      out = UnionRect(out, rects[i]);
    }
  }
  else if (rects.size() == 1){
    out = rects[0];
  }
  else {
    out = Rect(0, 0, in.cols, in.rows);
  }
  if (1) {
    int blockSize = 25;
    int constValue = 20;
    //cv::adaptiveThreshold(in, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);
    //imshow("local", local);
    rectangle(RGB, Rect(out), Scalar(0,0,255));
    imshow("inRGB", RGB);
    //imshow("threshold_R", threshold_Inv);
    cv::waitKey(0);
  }

  return 0;
}


int posDetect(const Mat &inRGB, const Mat &in, vector<Rect> & out)
{
	Mat threshold_R;
	Mat local;
  vector<RotatedRect> rects;
	OstuBeresenThreshold(in, threshold_R); //二值化
	//int blockSize = in.rows / 20;
	Mat imgInv(in.size(), in.type(), cv::Scalar(255));
  Mat threshold_Inv;
  threshold_Inv  = imgInv - threshold_R; //黑白色反转，即背景为黑色
  //std::vector<int> pos;
  //GetTextProjection(threshold_Inv, pos, 'H');
  //show_projection(pos, 'H');

	if (0) {
		int blockSize = 25;
		int constValue = 20;
		//cv::adaptiveThreshold(in, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);
		//imshow("local", local);
		imshow("threshold_R", threshold_Inv);
    cv::waitKey(0);
	}

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));  //闭形态学的结构元素
	morphologyEx(threshold_Inv, threshold_Inv, CV_MOP_CLOSE, element);

	vector< vector <Point> > contours;
	findContours(threshold_Inv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓
	
	vector< vector <Point> > ::iterator itc = contours.begin();

	if (0) {
		Mat out;
    inRGB.copyTo(out);
		int i;
		for (i = 0; i < contours.size(); ++i) {
			RotatedRect mr = minAreaRect(Mat(contours[i]));
			DrawRotatedRect(out, mr); //画黑色线条
		}
		imshow("Test_Rplanes", out);
		waitKey(0);
	}

  //对候选的轮廓进行进一步筛选
  if (0) {
    while (itc != contours.end())
    {
      RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
      if (!isEligible(mr))  //判断矩形轮廓是否符合要求
      {
        itc = contours.erase(itc);
      }
      else
      {
        rects.push_back(mr);
        ++itc;
      }
    }
  }
  else {
    while (itc != contours.end())
    {
      RotatedRect mr = minAreaRect(Mat(*itc)); //返回每个轮廓的最小有界矩形区域
      if (!isEligible2(mr, in.rows))  //判断矩形轮廓是否符合要求
      {
        itc = contours.erase(itc);
      }
      else
      {
        rects.push_back(mr);
        ++itc;
      }
    }
  }

	//测试是否找到了号码区域
	if (0) {
		Mat out;
		in.copyTo(out);
    for (int i = 0; i < rects.size(); ++i) {
      RotatedRect mr = rects[i];
      DrawRotatedRect(out, mr); //画黑色线条
    }
		imshow("Test_Rplane", out);
		waitKey(0);
	}
  Rect r0(0, 0, in.cols, in.rows);
  for (int i = 0; i < rects.size(); ++i) {
    Rect r = rects[i].boundingRect();
    r = InterRect(r, r0);
    //printf("%d %d\n", r.x, r.y);
    out.push_back(r);
  }
  return 0;
}

int normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area)
{
	float r, angle;

	angle = rects_optimal.angle;
	r = (float)rects_optimal.size.width / (float)(float)rects_optimal.size.height;
	if (r<1)
		angle = 90 + angle;
	Mat rotmat = getRotationMatrix2D(rects_optimal.center, angle, 1);//获得变形矩阵对象
	Mat img_rotated;
	warpAffine(intputImg, img_rotated, rotmat, intputImg.size(), CV_INTER_CUBIC);

	//裁剪图像
	Size rect_size = rects_optimal.size;

	if (r<1)
		std::swap(rect_size.width, rect_size.height);
	Mat  img_crop;
	getRectSubPix(img_rotated, rect_size, rects_optimal.center, img_crop);

	//用光照直方图调整所有裁剪得到的图像，使具有相同宽度和高度，适用于训练和分类
  if (0) {
    Mat resultResized;
    resultResized.create(20, 300, CV_8UC1);
    cv::resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

    resultResized.copyTo(output_area);
  }
  else {
    img_crop.copyTo(output_area);
  }
  return 0;
}

int seg_split(int npos, const int* pos, int minthd, int maxthd, std::vector<Range>& out) {
  std::vector<Range> x_char;
  Range r(0, npos);
  //char *flag = new char[img_threshold.cols];
  if (0) {
    int minv, maxv;
    minv = maxv = pos[0];
    for (int i = r.start; i < r.end; ++i) {
      if (pos[i] < minv) {
        minv = pos[i];
      }
      else if (pos[i] > maxv) {
        maxv = pos[i];
      }
    }
    double thd = 0.05;
    int thdv = minv + int(thd*(maxv - minv));
    for (int i = r.start; i < r.end; ++i)
    {
      if (pos[i] > thdv)
      {
        int start = i;
        for (; i < r.end && pos[i]>thdv; ++i);
        x_char.push_back(Range(start, i));
      }
    }
  }
  else {
    std::vector<int> minvec, maxvec;
    loc_minmax(npos, pos, -maxthd/2, maxthd/2, minvec, maxvec);
    double thd = 0.05;
    //show_projection(minvec, 'V');
    for (int i = r.start; i < r.end; ++i)
    {
      int thdv = minvec[i] + int(thd*(maxvec[i] - minvec[i]));
      if (pos[i] > thdv)
      {
        int start = i;
        for (; i < r.end && pos[i]>thdv; ++i);
        x_char.push_back(Range(start, i));
      }
    }
  }
  int k = 0;
  int counter = (int)x_char.size();
  while ((k = find_minwidth(counter, x_char.data(), minthd, maxthd))>0) {
    x_char[k - 1].end = x_char[k].end;
    for (int i = k + 1; i < counter; ++i) {
      x_char[i - 1] = x_char[i];
    }
    --counter;
  }
  for (int i = 0; i < counter; i++)
  {
    out.push_back(x_char[i]);
  }
  return counter;
}

int char_segment(const Mat &inputImg, vector<Rect> &dst_rect)
{
	Mat img_threshold;
	Mat whiteImg(inputImg.size(), inputImg.type(), cv::Scalar(255));
	Mat in_Inv = whiteImg - inputImg;

	// threshold(in_Inv ,img_threshold , 140,255 ,CV_THRESH_BINARY ); //反转黑白色
	threshold(in_Inv, img_threshold, 0, 255, CV_THRESH_OTSU); //大津法二值化
  //imshow("img_threshold", img_threshold); 
  std::vector<int> pos;
  std::vector<Range> x_char;
  GetTextProjection(img_threshold, pos, 'V');
  //SmoothPos121(pos.size(), pos.data());
  //show_projection(pos, 'V');
  //waitKey();
  int minthd = img_threshold.rows / 2;
  int maxthd = int(img_threshold.rows * 0.9);

  int npos = (int)pos.size();
  pos[0] = pos[npos - 1] = 0;

  int counter = seg_split(npos, pos.data(), minthd, maxthd, x_char);

	for (int i = 0; i < counter; i++)
	{
    Rect r(x_char[i].start, 0, x_char[i].size(), img_threshold.rows);
    dst_rect.push_back(r);
	}

	//delete[]flag;
	// imwrite("b.jpg" , img_threshold);
  return 0;
}

cv::Mat resetsize(const cv::Mat& in) {
  if (in.cols > 700 || in.cols >600)
  {
    Mat resizeR(450, 600, CV_8UC3);
    cv::resize(in, resizeR, resizeR.size());

    return resizeR;
  }
  return in;
}

void on_pushButton_2_clicked(const cv::Mat& imgSrc)
{
    Mat srcImage = resetsize(imgSrc);
    //Mat imgSrc_test = imread("E:/PracticeOfQt/Id_recognition/id_cardBai.jpg" ,1); //用于测试，实际应用只需将下面替换成imgSrc
    Mat imgRplane = getRplane(srcImage); //获得原始图像R分量

    vector <Rect>  rects1;
    vector<vector<Rect>>  rectvec;
    //Rect rc;
    //posDetect0(srcImage, imgRplane, rc);
    //return;
    //srcImage = srcImage(rc);
    imgRplane = getRplane(srcImage);
    posDetect(srcImage, imgRplane, rects1);  //获得身份证号码区域
    partitionLine(rects1, rectvec);
    for (int k = 0; k < rectvec.size(); ++k) {
      vector <Rect>&  rects = rectvec[k];
      for (int j = 0; j < rects.size(); ++j) {
        //normalPosArea(imgRplane, rects[j], outputMat); //获得身份证号码字符矩阵
        //if (k == 2 && j == 1)
        {
          Rect rect = rects[j];
          printf("j = %d %3d %3d\n", j, rect.y, rect.x);
          Mat outputMat = imgRplane(rect);
          vector<Rect> char_mat;  //获得切割得的字符矩阵
          cv::rectangle(srcImage, rect, cv::Scalar(255, 0, 0), 2);
          char_segment(outputMat, char_mat);
          for (int i = 0; i < char_mat.size(); ++i) {
            char buf[256];
            Rect r0 = char_mat[i];
            r0.x += rect.x;
            r0.y += rect.y;
            snprintf(buf, 256, "%d", i);
            cv::rectangle(srcImage, r0, cv::Scalar(0, 0, 255), 1);
            //printf("%d %d\n", char_mat[i].width(), char_mat[i].height());
            //imshow("char", srcImage(r0)); waitKey(-1);            cv::destroyWindow("char");
          }
        }
      }
      //cv::waitKey(-1);
    }
    imshow("srcImage", srcImage);
    cv::waitKey(-1);
    //getAnnXML();  //该函数只需执行一次，获得训练矩阵和标签矩阵，保存于xml文件中
#if 0
    cv::ANN_MLP ann;
    ann_train(ann, 10, 24); //10为输出层结点,也等于输出的类别，24为隐藏层结点

    vector<int> char_result;
    classify( ann ,char_mat ,char_result);

    getParityBit(char_result); //最后一位易出错，直接由前17位计算最后一位

    //设置界面信息
    if(char_result[16]%2 == 1)  //根据倒数第2位区分男女
    {
        ui->radioButton->setChecked(true);
    }
    else
        ui->radioButton_2->setChecked(true);

    std::string id = "";
    for(int i = 0; i < char_result.size();++i)
    {
        if (char_result[i] == 10)
            id += "X";
        else
            id += std::string::number(char_result[i], 10); //将int转换为std::string
    }
    ui->lineEdit_2->setText(id);

    std::string birthday;
    for(int i = 6; i < 14;++i)
        birthday += id[i];
    ui->lineEdit->setText(birthday);
#endif
}

float sumMatValue(const Mat &image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}

Mat projectHistogram(const Mat &img, int t)
{
	Mat lowData;
	cv::resize(img, lowData, Size(8, 16)); //缩放到8*16

	int sz = (t) ? lowData.rows : lowData.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j < sz; j++)
	{
		Mat data = (t) ? lowData.row(j) : lowData.col(j);
		mhist.at<float>(j) = countNonZero(data);
	}

	double min, max;
	minMaxLoc(mhist, &min, &max);

	if (max > 0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);

	return mhist;
}

void calcGradientFeat(const Mat &imgSrc, Mat &out)
{
	vector <float>  feat;
	Mat image;

	//cvtColor(imgSrc,image,CV_BGR2GRAY);
	cv::resize(imgSrc, image, Size(8, 16));

	// 计算x方向和y方向上的滤波
	float mask[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t(); // 转置
	Mat sobelX, sobelY;

	filter2D(image, sobelX, CV_32F, x_mask);
	filter2D(image, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);

	// 将图像划分为4*2共8个格子，计算每个格子里灰度值总和的百分比
	for (int i = 0; i < image.rows; i = i + 4)
	{
		for (int j = 0; j < image.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY = sobelY(Rect(j, i, 4, 4));
			feat.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}

	//计算第2个特征
	Mat imageGray;
	//cvtColor(imgSrc,imageGray,CV_BGR2GRAY);
	cv::resize(imgSrc, imageGray, Size(4, 8));
	Mat p = imageGray.reshape(1, 1);
	p.convertTo(p, CV_32FC1);
	for (int i = 0; i<p.cols; i++)
	{
		feat.push_back(p.at<float>(i));
	}

	//增加水平直方图和垂直直方图
	Mat vhist = projectHistogram(imgSrc, 1); //水平直方图
	Mat hhist = projectHistogram(imgSrc, 0);  //垂直直方图
	for (int i = 0; i<vhist.cols; i++)
	{
		feat.push_back(vhist.at<float>(i));
	}
	for (int i = 0; i<hhist.cols; i++)
	{
		feat.push_back(hhist.at<float>(i));
	}


	out = Mat::zeros(1, (int)feat.size(), CV_32F);
	for (int i = 0; i<feat.size(); i++)
	{
		out.at<float>(i) = feat[i];
	}
}

void getAnnXML() // 此函数仅需运行一次，目的是获得训练矩阵和标签矩阵，保存于ann_xml.xml中
{
    FileStorage fs("ann_xml.xml", FileStorage::WRITE);
    if (!fs.isOpened())
    {
        qDebug() << "failed to open " <<"/n";
    }

    Mat  trainData;
    Mat classes = Mat::zeros(1,550,CV_8UC1);   //1700*48维，也即每个样本有48个特征值
    char path[60];
    Mat img_read;
    for (int i = 0;i<10 ;i++)  //第i类
    {
        for (int j=1 ; j< 51 ; ++j)  //i类中第j个，即总共有11类字符，每类有50个样本
        {
            sprintf( path ,"E:/PracticeOfQt/Id_recognition/Number_char/%d/%d (%d).png" , i,i,j);
            img_read = imread(path , 0);
            Mat dst_feature;
            calcGradientFeat(img_read, dst_feature); //计算每个样本的特征矢量
            trainData.push_back(dst_feature);
            classes.at<uchar>(i*50 + j -1) =  i;
        }
    }
    fs<<"TrainingData"<<trainData;
    fs<<"classes"<<classes;
    fs.release();

}

#if 0
void ann_train(CvANN_MLP &ann, int numCharacters, int nlayers)
{
    Mat trainData ,classes;
    FileStorage fs;
    fs.open("ann_xml.xml" , FileStorage::READ);

    fs["TrainingData"] >>trainData;
    fs["classes"] >>classes;

    Mat layerSizes(1,3,CV_32SC1);     //3层神经网络
    layerSizes.at<int>( 0 ) = trainData.cols;   //输入层的神经元结点数，设置为24
    layerSizes.at<int>( 1 ) = nlayers; //1个隐藏层的神经元结点数，设置为24
    layerSizes.at<int>( 2 ) = numCharacters; //输出层的神经元结点数为:10
    ann.create(layerSizes , CvANN_MLP::SIGMOID_SYM ,1,1);  //初始化ann
    CvANN_MLP_TrainParams param;
    param.term_crit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5000,0.01);

    Mat trainClasses;
    trainClasses.create(trainData.rows , numCharacters ,CV_32FC1);
    for (int i =0;i< trainData.rows; i++)
    {
        for (int k=0 ; k< trainClasses.cols ; k++ )
        {
            if ( k == (int)classes.at<uchar> (i))
            {
                trainClasses.at<float>(i,k)  = 1 ;
            }
            else
                trainClasses.at<float>(i,k)  = 0;
        }

    }

    //Mat weights(1 , trainData.rows , CV_32FC1 ,Scalar::all(1) );
    ann.train( trainData ,trainClasses , Mat() ,Mat() ,param);
}

void classify(CvANN_MLP &ann, vector<Mat> &char_Mat, vector<int> &char_result)
{
    char_result.resize(char_Mat.size());
    for (int i=0;i<char_Mat.size(); ++i)
    {
        Mat output(1 ,10 , CV_32FC1); //1*10矩阵

        Mat char_feature;
        calcGradientFeat(char_Mat[i] ,char_feature);
        ann.predict(char_feature ,output);
        Point maxLoc;
        double maxVal;
        minMaxLoc(output , 0 ,&maxVal , 0 ,&maxLoc);

        char_result[i] =  maxLoc.x;

    }
}
#endif

void getParityBit(vector<int> &char_result)
{
    int mod = 0;
    int wights[17]={ 7,9,10,5,8,4 ,2,1,6,3,7,9,10,5,8,4,2};
    for(int i =0; i < 17 ;++i)
        mod += char_result[i]*wights[i];//乘相应系数求和

    mod = mod%11; //对11求余

    int value[11]= {1,0,10,9,8,7,6,5,4,3,2};
    char_result[17] = value[mod];
}


int test_ocr_detect()
{
  if (0) {
    cv::Mat imgSrc;
    const char* ch = "D:/code/pudn/ocr/Id_recognition/id_card9.jpg";
    ch = "E:/OCR_Line/demo_images/012.jpg";
    imgSrc = cv::imread(ch, 1);  //使用打开的文件对话框获得的路径，由imread读出
    on_pushButton_2_clicked(imgSrc);
    //cv::imshow("asdf", imgSrc);
    cv::waitKey(-1);
  }
  if (1) {
    std::vector<string> filenames;
    LoadTextFileList("E:/OCR_Line/std/list.txt", filenames);
    int k;
    for (k = 0; k < filenames.size(); ++k) {
      string fn = filenames[k];
      printf("%s\n", fn.c_str());
      cv::Mat srcImage = cv::imread(fn, IMREAD_COLOR);
      if (srcImage.empty()) {
        continue;
      }
      if (srcImage.cols > 600) {
        double t = 600.*1. / srcImage.cols;
        cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
      }
      on_pushButton_2_clicked(srcImage);
      //cv::imshow("asdf", imgSrc);
      //cv::waitKey(-1);
    }
  }
	return 0;
}

