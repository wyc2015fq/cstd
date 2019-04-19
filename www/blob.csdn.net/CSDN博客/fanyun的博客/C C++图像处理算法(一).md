# C/C++图像处理算法(一) - fanyun的博客 - CSDN博客
2018年08月26日 14:27:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2082
图像处理算法1总结如下：
```cpp
//添加椒盐噪声
void salt(Mat& src,int number)
{
	for (int i = 0; i < number; i++)
	{
		int r = static_cast<int>(rng.uniform(0, src.rows));
		int c = static_cast<int>(rng.uniform(0, src.cols));
		int k = (static_cast<int>(rng.uniform(0, 1000))&1);
		if(k==1)
		    src.at<uchar>(r, c) = 255;
		else
			src.at<uchar>(r, c) = 0;
	}
	return;
}
```
```cpp
/*
* @ drt ：高斯方差
* @ Medium ：高斯均值
*/
int Get_Gauss(int Medium, int drt)
{
	//产生高斯样本，以U为均值，D为均方差
	double sum = 0;
	for (int i = 0; i<12; i++) sum += rand() / 32767.00;
	//计算机中rand()函数为－32767～＋32767（2^15-1）
	//故sum＋为0～1之间的均匀随机变量
	return int(Medium + drt*(sum - 6));
	//产生均值为U，标准差为D的高斯分布的样本，并返回
}
/*
* variance ：高斯噪声的方差
*/
 
//添加高斯噪声
void ImgAddGaussNoise1(const uchar *srcimgbuff, uchar * dstImgbuff, int srcwith, int srcheigh, int chanels)
{
	assert(srcimgbuff != NULL && srcwith > 0 && srcheigh > 0);
	int bytecount = srcwith * srcheigh * chanels;
 
	for (size_t i = 0; i < bytecount; i++)
	{
		dstImgbuff[i] += Get_Gauss(20, 0.02);
	}
}
```
```cpp
//中值求取
void Media(Mat* src, int indexrows, int indexcols, int* meanv, int ker)
{
	int lo = (ker - 1) / 2;
	vector<int>moreo;
	for (int i = indexrows - lo; i <= indexrows + lo; i++)
	{
		for (int j = indexcols - lo; j <= indexcols + lo; j++)
		{
			moreo.push_back(src->at<uchar>(i, j));
		}
	}
	sort(moreo.begin(), moreo.end());
	*meanv = moreo.at(ker * ker / 2);
	return;
}
```
```cpp
//均值求取
void Meanvalue(Mat* src, int indexrows, int indexcols, float* meanv, int ker)
{
	int lo = (ker - 1) / 2;
	float total = 0;
	for (int i = indexrows - lo; i <= indexrows + lo; i++)
	{
		for (int j = indexcols - lo; j <= indexcols + lo; j++)
		{
			total += src->at<uchar>(i, j);
		}
	}
	*meanv = total / (ker * ker);
	return;
}
```
```cpp
//像素方差
void Variance(Mat& src, vector<test>& hierachy, int ker)
{
	int row = src.rows;
	int col = src.cols;
	int lo = (ker - 1) / 2;
	for (int ir = lo; ir < row - lo; ir++)
	{
		for (int jc = lo; jc < col - lo; jc++)
		{
			float means;
			int var;
			//计算均值
			Meanvalue(&src, ir, jc, &means, ker);
			Vvalue(&src, ir, jc, &var, ker, means);
			test temp;
			temp.menval = var;
			temp.x = ir;
			temp.y = jc;
			hierachy.push_back(temp);
		}
	}
	return;
}
```
```cpp
//局部方差求取
void Vvalue(Mat* src, int indexrows, int indexcols, int* vall, int ker, float mean)
{
	int lo = (ker - 1) / 2;
	float total = 0;
	for (int i = indexrows - lo; i <= indexrows + lo; i++)
	{
		for (int j = indexcols - lo; j <= indexcols + lo; j++)
		{
			total += pow((src->at<uchar>(i, j) - mean), 2);
		}
	}
	*vall = static_cast<int>(total);
	return;
}
```
```cpp
//STL排序方式
bool SortByM1(const test &v1, const test &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{
	return v1.menval < v2.menval;//升序排列  
}
```
```cpp
//SSIM 结构相似比
Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	/***************************** INITS **********************************/
	int d = CV_32F;
 
	Mat I1, I2;
	i1.convertTo(I1, d);           // cannot calculate on one byte large values
	i2.convertTo(I2, d);
 
	int num = I1.channels();
	//cv::imshow("123", I1);
	//cv::waitKey();
 
	Mat I2_2 = I2.mul(I2);        // I2^2
	Mat I1_2 = I1.mul(I1);        // I1^2
	Mat I1_I2 = I1.mul(I2);        // I1 * I2
 
								   /*************************** END INITS **********************************/
 
	Mat mu1, mu2;   // PRELIMINARY COMPUTING
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);
 
	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);
 
	Mat sigma1_2, sigma2_2, sigma12;
 
	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;
 
	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;
 
	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;
 
	///////////////////////////////// FORMULA ////////////////////////////////
	Mat t1, t2, t3;
 
	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
 
	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
 
	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;
 
	Scalar mssim = mean(ssim_map); // mssim = average of ssim map
	return mssim;
}
```
