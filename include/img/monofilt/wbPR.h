
/******************************************
void fQuickSort(float* data, int* ind, int n)
功能：
	运用快速排序法对数组降序排列
参数：
	float* data - 排序数据		
	int* ind - 位置索引，即顺序
	int iLeft - 起始位置
	int iRight - 结束位置
*******************************************/
void fQuickSort(float* data, int* ind, int iLeft, int iRight)
{
	if (iLeft >= iRight)
		return;
	
	int mid = iLeft;

	//以最后一个元素为参照，大的放左边，小的放右边
	for (int i = iLeft; i < iRight; i++)
	{
		if (data[i] >= data[iRight])
		{			
			float temp0 = data[mid];
			data[mid] = data[i];
			data[i] = temp0;

			int temp1 = ind[mid];
			ind[mid] = ind[i];
			ind[i] = temp1;

			mid = mid + 1;
		} 
		
	}
	
	float temp2 = data[mid];
	data[mid] = data[iRight];
	data[iRight] = temp2;

	int temp3 = ind[mid];
	ind[mid] = ind[iRight];
	ind[iRight] = temp3;

	fQuickSort(data, ind, iLeft, mid-1);
	fQuickSort(data, ind, mid+1, iRight);
}

/******************************************
void wbPR(float** rgbImg, float** chImg, int rows, int cols)
功能：
	将rbg图按全反射理论算法(PR)做白平衡调整
参数：
	float** rgbImg - 图像的rgb数据，[rows][cols*3]		
	float** chImg - 输出数据，白平衡处理后的图像数据，[rows][cols*3]
	int rows - 图像的行数
	int cols - 图像的列数	
*******************************************/
int wbPR(int rows, int cols, uchar* rgbImg, int rgb_step, uchar* chImg, int ch_step)
{
	int i, j;

	//1）白点像素值
	int rw = 255;
	int gw = 255;
	int bw = 255;	

	//2）求最亮点的R、G、B值，为体现普遍性，本文求的是一定比率最亮点的平均值
	float rate = 0.15f;									//比率，在全图中取最亮点的比率
	float rmax = 0.0f;									//按比率rate，求最亮点的R分量的平均值
	float gmax = 0.0f;									//按比率rate，求最亮点的G分量的平均值
	float bmax = 0.0f;									//按比率rate，求最亮点的B分量的平均值
	float* grayImg = new float[rows * cols];					//记录rgbImg的灰度值
	float* tempImg = new float[rows * cols];			//将grayImg数据转化为一维数组，方便排序
  int gray_step = cols;
  int temp_step = cols;
	int* ind = new int[rows * cols];					//记录顺序
	for (i = 0; i < rows*cols; i++)						//赋初值
	{
		ind[i] = i;
	}
	
	//rgb2gray(rgbImg, grayImg, rows, cols);				//rgb转gray

	for (i = 0; i < rows; i++)
	{
    int r, g, b;
    uchar* rgbLine = rgbImg + i*rgb_step;
    float* grayLine = grayImg + i*gray_step;
		for (j = 0; j < cols; j++)
		{
      r = rgbLine[j*3+0];
      g = rgbLine[j*3+1];
      b = rgbLine[j*3+2];
			grayLine[j] = (float)((r+g+g+b)>>2);
		}
	}

	for (i = 0; i < rows; i++)
	{
    float* grayLine = grayImg + i*gray_step;
    float* tempLine = tempImg + i*temp_step;
		for (j = 0; j < cols; j++)
		{
			tempLine[j] = grayLine[j];
		}
	}


	// fSort(tempImg, ind, rows*cols);		//降序排列
	fQuickSort(tempImg, ind, 0, rows*cols-1);

	//求rmax、gmax、bmax
	for (i = 0; i < int(rate*rows*cols); i++)
	{
		int row, col;									//行、列定位
		row = int(ind[i] / cols);						//行
		col = ind[i] - cols * row;						//列
		rmax = rmax + rgbImg[row*rgb_step+3*col];
		gmax = gmax + rgbImg[row*rgb_step+3*col+1];
		bmax = bmax + rgbImg[row*rgb_step+3*col+2];
	}
	rmax = rmax / (int(rate*rows*cols));
	gmax = gmax / (int(rate*rows*cols));
	bmax = bmax / (int(rate*rows*cols));


	//3）调整原图像R、G、B分量值，得出调整后的图像数据
	for (i = 0; i < rows; i++)
	{
    uchar* rgbLine = rgbImg + i*rgb_step;
    uchar* chLine = chImg + i*ch_step;
		for (j = 0; j < cols; j++)
		{
			chLine[3*j+0] = (uchar)(rgbLine[3*j+0] * rw / rmax);
			chLine[3*j+1] = (uchar)(rgbLine[3*j+1] * gw / gmax);
			chLine[3*j+2] = (uchar)(rgbLine[3*j+2] * bw / bmax);
		}
	}

	//4）释放内存
	delete[] grayImg;
	grayImg = NULL;

	delete[] tempImg;
	tempImg = NULL;

	delete[] ind;
	ind = NULL;
  return 0;
}

