
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/ml/ml.hpp"

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#define qDebug()  std::cout



using namespace cv;
using namespace std;


void OstuBeresenThreshold(const Mat &in, Mat &out) //输入为单通道
{

	double ostu_T = threshold(in, out, 0, 255, CV_THRESH_OTSU); //otsu获得全局阈值

	double min;
	double max;
	minMaxIdx(in, &min, &max);
	const double CI = 0.12;
	double beta = CI*(max - min + 1) / 128;
	double beta_lowT = (1 - beta)*ostu_T;
	double beta_highT = (1 + beta)*ostu_T;

	Mat doubleMatIn;
	in.copyTo(doubleMatIn);
	int rows = doubleMatIn.rows;
	int cols = doubleMatIn.cols;
	double Tbn;
	for (int i = 0; i < rows; ++i)
	{
		//获取第 i行首像素指针
		uchar * p = doubleMatIn.ptr<uchar>(i);
		uchar *outPtr = out.ptr<uchar>(i);

		//对第i 行的每个像素(byte)操作
		for (int j = 0; j < cols; ++j)
		{

			if (i <2 | i>rows - 3 | j<2 | j>rows - 3)
			{

				if (p[j] <= beta_lowT)
					outPtr[j] = 0;
				else
					outPtr[j] = 255;
			}
			else
			{
				Tbn = sum(doubleMatIn(Rect(i - 2, j - 2, 5, 5)))[0] / 25;  //窗口大小25*25
				if (p[j] < beta_lowT | (p[j] < Tbn && (beta_lowT <= p[j] && p[j] >= beta_highT)))
					outPtr[j] = 0;
				if (p[j] > beta_highT | (p[j] >= Tbn && (beta_lowT <= p[j] && p[j] >= beta_highT)))
					outPtr[j] = 255;
			}
		}
	}

}

bool isEligible(const RotatedRect &candidate)
{
	float error = 0.2;
	const float aspect = 4.5 / 0.3; //长宽比
	int min = 10 * aspect * 10; //最小区域
	int max = 50 * aspect * 50;  //最大区域
	float rmin = aspect - aspect*error; //考虑误差后的最小长宽比
	float rmax = aspect + aspect*error; //考虑误差后的最大长宽比

	int area = candidate.size.height * candidate.size.width;
	float r = (float)candidate.size.width / (float)candidate.size.height;
	if (r <1)
		r = 1 / r;

	if ((area < min || area > max) || (r< rmin || r > rmax)) //满足该条件才认为该candidate为车牌区域
		return false;
	else
		return true;
}

void DrawRotatedRect(Mat& out, RotatedRect& rrect) {
	Point2f vertices[4];
	rrect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(out, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 0));//画黑色线条
}

void posDetect(const Mat &in, vector<RotatedRect> & rects)
{
	Mat threshold_R;
	Mat local;
	OstuBeresenThreshold(in, threshold_R); //二值化
	//int blockSize = in.rows / 20;
	if (0) {
		int blockSize = 25;
		int constValue = 20;
		cv::adaptiveThreshold(in, local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, blockSize, constValue);
		imshow("local", local);
		imshow("threshold_R", threshold_R); cv::waitKey(0);
	}


	Mat imgInv(in.size(), in.type(), cv::Scalar(255));
	Mat threshold_Inv = imgInv - threshold_R; //黑白色反转，即背景为黑色

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));  //闭形态学的结构元素
	morphologyEx(threshold_Inv, threshold_Inv, CV_MOP_CLOSE, element);

	vector< vector <Point> > contours;
	findContours(threshold_Inv, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//只检测外轮廓
	
	vector< vector <Point> > ::iterator itc = contours.begin();

	if (0) {
		Mat out;
		in.copyTo(out);
		int i;
		for (i = 0; i < contours.size(); ++i) {
			RotatedRect mr = minAreaRect(Mat(contours[i]));
			DrawRotatedRect(out, mr); //画黑色线条
		}
		imshow("Test_Rplanes", out);
		waitKey(0);
	}

	//对候选的轮廓进行进一步筛选
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

	//测试是否找到了号码区域
	if (0) {
		Mat out;
		in.copyTo(out);
		DrawRotatedRect(out, rects[0]); //画黑色线条
		imshow("Test_Rplane", out);
		waitKey(0);
	}


}

void normalPosArea(const Mat &intputImg, RotatedRect &rects_optimal, Mat& output_area)
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
	Mat resultResized;
	resultResized.create(20, 300, CV_8UC1);
	cv::resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);

	resultResized.copyTo(output_area);
}

Mat getRplane(const Mat &in)
{
	vector<Mat> splitBGR(in.channels()); //容器大小为通道数3
	split(in, splitBGR);
	//return splitBGR[2];  //R分量

	if (in.cols > 700 | in.cols >600)
	{
		Mat resizeR(450, 600, CV_8UC1);
		cv::resize(splitBGR[2], resizeR, resizeR.size());

		return resizeR;
	}
	else
		return splitBGR[2];

}

Mat getRplane2(const Mat &in)
{
	Mat gray;
	Mat color_boost;
	cv::decolor(in, gray, color_boost);
	//return splitBGR[2];  //R分量
	imshow("gray", gray);
	imshow("color_boost", color_boost);
	cv::waitKey(-1);

	if (in.cols > 700 | in.cols >600)
	{
		Mat resizeR(450, 600, CV_8UC1);
		cv::resize(gray, resizeR, resizeR.size());

		return resizeR;
	}
	else
		return gray;

}

void char_segment(const Mat &inputImg, vector<Mat> &dst_mat)
{
	Mat img_threshold;

	Mat whiteImg(inputImg.size(), inputImg.type(), cv::Scalar(255));
	Mat in_Inv = whiteImg - inputImg;

	// threshold(in_Inv ,img_threshold , 140,255 ,CV_THRESH_BINARY ); //反转黑白色
	threshold(in_Inv, img_threshold, 0, 255, CV_THRESH_OTSU); //大津法二值化

	int x_char[19] = { 0 };
	short counter = 1;
	short num = 0;
	char *flag = new char[img_threshold.cols];

	for (int j = 0; j < img_threshold.cols; ++j)
	{
		flag[j] = true;
		for (int i = 0; i < img_threshold.rows; ++i)
		{
			if (img_threshold.at<uchar>(i, j) != 0)
			{
				flag[j] = false;
				break;
			}

		}
	}

	for (int i = 0; i < img_threshold.cols - 2; ++i)
	{
		if (flag[i] == true)
		{
			x_char[counter] += i;
			num++;
			if (flag[i + 1] == false && flag[i + 2] == false)
			{
				x_char[counter] = x_char[counter] / num;
				num = 0;
				counter++;
			}
		}
	}
	x_char[18] = img_threshold.cols;

	for (int i = 0; i < 18; i++)
	{
		dst_mat.push_back(Mat(in_Inv, Rect(x_char[i], 0, x_char[i + 1] - x_char[i], img_threshold.rows)));
	}

	delete[]flag;
	// imwrite("b.jpg" , img_threshold);
}

void on_pushButton_2_clicked(const cv::Mat& imgSrc)
{
    //Mat imgSrc_test = imread("E:/PracticeOfQt/Id_recognition/id_cardBai.jpg" ,1); //用于测试，实际应用只需将下面替换成imgSrc
    Mat imgRplane = getRplane(imgSrc); //获得原始图像R分量

    vector <RotatedRect>  rects;
    posDetect(imgRplane ,rects);  //获得身份证号码区域

    Mat outputMat;
    normalPosArea(imgRplane ,rects[0],outputMat); //获得身份证号码字符矩阵

    vector<Mat> char_mat;  //获得切割得的字符矩阵
    char_segment(outputMat , char_mat);

	int i;
	for (i = 0; i < char_mat.size(); ++i) {
		char buf[256];
		snprintf(buf, 256, "%d", i);
		imshow(buf, char_mat[i]);
	}
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


	out = Mat::zeros(1, feat.size(), CV_32F);
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


int test_Id_recognition()
{
	cv::Mat imgSrc;
	const char* ch = "D:/code/pudn/ocr/Id_recognition/id_card9.jpg";
	ch = "E:/OCR_Line/demo_images/002.jpg";
	imgSrc = cv::imread(ch, 1);  //使用打开的文件对话框获得的路径，由imread读出
	on_pushButton_2_clicked(imgSrc);
	cv::imshow("asdf", imgSrc);
	cv::waitKey(-1);
	return 0;
}

