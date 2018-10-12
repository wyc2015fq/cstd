
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct KVNode
{
  int index;
  int pixelSum;
};
//经典线性霍夫变换缺点：运行耗时，不能定位线段的起始终止点。


void drawLine(cv::Mat& img, const Vec2f& lines, cv::Scalar& color, int thickness = 1, int lineType = 8)
{
  float rho = lines[0], theta = lines[1];
  Point pt1, pt2;
  double a = cos(theta), b = sin(theta);
  double x0 = a*rho, y0 = b*rho;
  pt1.x = cvRound(x0 + 2000 * (-b));  //把浮点数转化成整数
  pt1.y = cvRound(y0 + 2000 * (a));
  pt2.x = cvRound(x0 - 2000 * (-b));
  pt2.y = cvRound(y0 - 2000 * (a));
  line(img, pt1, pt2, color, thickness, lineType);
}


bool Compare(KVNode& aNode, KVNode& bNode)
{
  return (aNode.pixelSum > bNode.pixelSum);
}

 int HoughLinesByPoints(const Point* pts, int npt, int height, int width, float rho, float theta, int threshold, Vec2f* pLines, int linesMax)
{
	//tho表示距离刻度，theta表示角度刻度，pLines表示返回的直线数组指针,threshold表示直线的最少像素值，linesMax表示要返回的最大边数目。
	int* pAccm = NULL;
	KVNode* pKVNodeSort = NULL;
	float* tabSin = NULL;
	float* tabCos = NULL;
 
	int total = 0;
	int numAngle, numRho;
	int i, j;
	double scale = 0.0;
	float iRho = 1 / rho;
 
	numAngle = round(CV_PI / theta);
	numRho = round(((width + height) * 2 + 1) / rho);
 
	pAccm = new int[(numAngle + 2)*(numRho + 2)]{0};
	pKVNodeSort = new KVNode[numAngle*numRho];
 
	tabSin = new float[numAngle]{0.0f};
	tabCos = new float[numAngle]{0.0f};

	float ang = 0;//避免重复计算角度，事先计算好sinθi/ρ和cosθi/ρ
	for (int n = 0; n < numAngle; ang += theta, n++)
	{
		tabSin[n] = sinf(ang)*iRho;
		tabCos[n] = cosf(ang)*iRho;
	}
	//第一步：逐点进行或霍夫空间变换，并将结果写入累加器数组内
	for (int k = 0; k < npt; ++k)
  {
    int i = pts[k].y;
    int j = pts[k].x;
    for (int n = 0; n < numAngle; n++)
    {
      int r = round(j*tabCos[n] + i*tabSin[n]);
      r += (numRho - 1) / 2;
      ++pAccm[(n + 1)*(numRho + 2) + r + 1];
    }
  }
	//第二步：找到局部极大值，即做四方向的非极大值抑制
	for (int r = 0; r < numRho; ++r)
	{
		for (int n = 0; n < numAngle; ++n)
		{
			int base = (n + 1)*(numRho + 2) + r + 1;
			if (pAccm[base]>threshold && pAccm[base]>pAccm[base - 1] && pAccm[base] > pAccm[base + 1] && pAccm[base] > pAccm[base - numRho - 2] && pAccm[base] > pAccm[base + numRho + 2])
			{
				KVNode tempKVNode;
				tempKVNode.index = base;
				tempKVNode.pixelSum = pAccm[base];
				pKVNodeSort[total++] = tempKVNode;
			}
		}
	}
 
	//第三步：对累加器值按由大到小排序
	sort(pKVNodeSort, pKVNodeSort + total, Compare);
 
	//第四步：输出linesMax条直线，如果linesMax等于total,则输出所有直线
	linesMax = (linesMax <= total) ? linesMax : total;
	//pLines = new Vec2f[linesMax];
	scale = 1.0 / (numRho + 2);
  //linesMax = total;
	for (int i = 0; i < linesMax; ++i)
	{
		Vec2f tempVec2f;
		int index = pKVNodeSort[i].index;//分离出该极大值在霍夫空间中的位置
		int n = floor(index*scale) - 1;
		int r = index - (n + 1)*(numRho + 2) - 1;
		tempVec2f[0] = (r - (numRho - 1)*0.5f)*rho;
		tempVec2f[1] = n*theta;
		pLines[i] = tempVec2f;
    //pLines.push_back(tempVec2f);
	}
  //linesMax = pLines.size();

  delete[]tabSin;
  delete[]tabCos;
  delete[]pKVNodeSort;
  delete[]pAccm;

  return linesMax;
}

 //int width, height;        //图像的宽和高
static int icvHoughLinesProbabilistic(const Point* pts, int npt, int height, int width,
  float rho, float theta, int threshold,
  int lineLength, int lineGap,
  vector<Vec4i>& lines, int linesMax)
{
   //accum为累加器矩阵，mask为掩码矩阵
   Mat accum, mask;
   vector<float> trigtab;    //用于存储事先计算好的正弦和余弦值
                             //开辟一段内存空间

   int numangle, numrho;     //角度和距离的离散数量
   float ang;
   int r, n, count;
   CvPoint pt;
   float irho = 1 / rho;     //距离分辨率的倒数
   CvRNG rng = cvRNG(-1);    //随机数
   const float* ttab;        //向量trigtab的地址指针
   uchar* mdata0;    //矩阵mask的地址指针
                     //确保输入图像的正确性
   //CV_Assert(CV_IS_MAT(image) && CV_MAT_TYPE(image->type) == CV_8UC1);

                            //由角度和距离分辨率，得到角度和距离的离散数量
   numangle = cvRound(CV_PI / theta);
   numrho = cvRound(((width + height) * 2 + 1) / rho);
   //创建累加器矩阵，即霍夫空间
   accum.create(numangle, numrho, CV_32SC1);
   //创建掩码矩阵，大小与输入图像相同
   mask.create(height, width, CV_8UC1);
   //定义trigtab的大小，因为要存储正弦和余弦值，所以长度为角度离散数的2倍
   trigtab.resize(numangle * 2);
   //累加器矩阵清零
   mask = cv::Scalar(1);
   accum = cv::Scalar(0);
   //避免重复计算，事先计算好所需的所有正弦和余弦值
   for (ang = 0, n = 0; n < numangle; ang += theta, n++)
   {
     trigtab[n * 2] = (float)(cos(ang) * irho);
     trigtab[n * 2 + 1] = (float)(sin(ang) * irho);
   }
   //赋值首地址
   ttab = &trigtab[0];
   mdata0 = mask.data;

   // stage 1. collect non-zero image points
   //收集图像中的所有非零点，因为输入图像是边缘图像，所以非零点就是边缘点
   if (0) {
     for (int i = 0; i < npt; ++i)
     {
       //提取出输入图像和掩码矩阵的每行地址指针
       Point pt = pts[i];
       if (pt.x > 0 && pt.y > 0 && pt.x < width - 1 && pt.y < height - 1) {
         uchar* mdata = mdata0 + pt.y*width + pt.x;
         uchar* mdata1 = mdata - width;
         uchar* mdata2 = mdata + width;
         //掩码的相应位置清0
         mdata[0] = 1;
         mdata[1] = 1;
         mdata[-1] = 1;

         mdata1[0] = 1;
         mdata1[1] = 1;
         mdata1[-1] = 1;

         mdata2[0] = 1;
         mdata2[1] = 1;
         mdata2[-1] = 1;
       }
     }
   }
   //终止写序列，seq为所有边缘点坐标位置的序列
   count = npt;    //得到边缘点的数量
   CvPoint* seq = (CvPoint*)malloc(npt * sizeof(CvPoint));
   memcpy(seq, pts, npt * sizeof(CvPoint));
   // stage 2. process all the points in random order
   //随机处理所有的边缘点
   for (; count > 0; count--)
   {
     // choose random point out of the remaining ones
     //步骤1，在剩下的边缘点中随机选择一个点，idx为不大于count的随机数
     int idx = cvRandInt(&rng) % count;
     //max_val为累加器的最大值，max_n为最大值所对应的角度
     int max_val = threshold - 1, max_n = 0;
     //由随机数idx在序列中提取出所对应的坐标点
     CvPoint* point = seq + idx;
     //定义直线的两个端点
     CvPoint line_end[2] = { { 0,0 },{ 0,0 } };
     float a, b;
     //累加器的地址指针，也就是霍夫空间的地址指针
     int* adata = (int*)accum.data;
     int i, j, k, x0, y0, dx0, dy0, xflag;
     int good_line;
     const int shift = 16;
     //提取出坐标点的横、纵坐标
     i = point->y;
     j = point->x;

     // "remove" it by overriding it with the last element
     //用序列中的最后一个元素覆盖掉刚才提取出来的随机坐标点
     *point = *(CvPoint*)(seq + count - 1);

     // check if it has been excluded already (i.e. belongs to some other line)
     //检测这个坐标点是否已经计算过，也就是它已经属于其他直线
     //因为计算过的坐标点会在掩码矩阵mask的相对应位置清零
     if (!mdata0[i*width + j])    //该坐标点被处理过
       continue;    //不做任何处理，继续主循环

                    // update accumulator, find the most probable line
                    //步骤2，更新累加器矩阵，找到最有可能的直线
     for (n = 0; n < numangle; n++, adata += numrho)
     {
       //由角度计算距离
       r = cvRound(j * ttab[n * 2] + i * ttab[n * 2 + 1]);
       r += (numrho - 1) / 2;
       //在累加器矩阵的相应位置上数值加1，并赋值给val
       int val = ++adata[r];
       //更新最大值，并得到它的角度
       if (max_val < val)
       {
         max_val = val;
         max_n = n;
       }
     }

     // if it is too "weak" candidate, continue with another point
     //步骤3，如果上面得到的最大值小于阈值，则放弃该点，继续下一个点的计算
     if (max_val < threshold)
       continue;

     // from the current point walk in each direction
     // along the found line and extract the line segment
     //步骤4，从当前点出发，沿着它所在直线的方向前进，直到达到端点为止
     a = -ttab[max_n * 2 + 1];    //a=-sinθ
     b = ttab[max_n * 2];    //b=cosθ
                             //当前点的横、纵坐标值
     x0 = j;
     y0 = i;
     //确定当前点所在直线的角度是在45度～135度之间，还是在0～45或135度～180度之间
     if (fabs(a) > fabs(b))    //在45度～135度之间
     {
       xflag = 1;    //置标识位，标识直线的粗略方向
                     //确定横、纵坐标的位移量
       dx0 = a > 0 ? 1 : -1;
       dy0 = cvRound(b*(1 << shift) / fabs(a));
       //确定纵坐标
       y0 = (y0 << shift) + (1 << (shift - 1));
     }
     else    //在0～45或135度～180度之间
     {
       xflag = 0;   //清标识位
                    //确定横、纵坐标的位移量
       dy0 = b > 0 ? 1 : -1;
       dx0 = cvRound(a*(1 << shift) / fabs(b));
       //确定横坐标
       x0 = (x0 << shift) + (1 << (shift - 1));
     }
     //搜索直线的两个端点
     for (k = 0; k < 2; k++)
     {
       //gap表示两条直线的间隙，x和y为搜索位置，dx和dy为位移量
       int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;
       //搜索第二个端点的时候，反方向位移
       if (k > 0)
         dx = -dx, dy = -dy;

       // walk along the line using fixed-point arithmetics,
       // stop at the image border or in case of too big gap
       //沿着直线的方向位移，直到到达图像的边界或大的间隙为止
       for (;; x += dx, y += dy)
       {
         uchar* mdata;
         int i1, j1;
         //确定新的位移后的坐标位置
         if (xflag)
         {
           j1 = x;
           i1 = y >> shift;
         }
         else
         {
           j1 = x >> shift;
           i1 = y;
         }
         //如果到达了图像的边界，停止位移，退出循环
         if (j1 < 0 || j1 >= width || i1 < 0 || i1 >= height)
           break;
         //定位位移后掩码矩阵位置
         mdata = mdata0 + i1*width + j1;

         // for each non-zero point:
         //    update line end,
         //    clear the mask element
         //    reset the gap
         //该掩码不为0，说明该点可能是在直线上
         if (*mdata)
         {
           gap = 0;    //设置间隙为0
                       //更新直线的端点位置
           line_end[k].y = i1;
           line_end[k].x = j1;
         }
         //掩码为0，说明不是直线，但仍继续位移，直到间隙大于所设置的阈值为止
         else if (++gap > lineGap)    //间隙加1
           break;
       }
     }
     //步骤5，由检测到的直线的两个端点粗略计算直线的长度
     //当直线长度大于所设置的阈值时，good_line为1，否则为0
     good_line = abs(line_end[1].x - line_end[0].x) >= lineLength ||
       abs(line_end[1].y - line_end[0].y) >= lineLength;
     //再次搜索端点，目的是更新累加器矩阵和更新掩码矩阵，以备下一次循环使用
     for (k = 0; k < 2; k++)
     {
       int x = x0, y = y0, dx = dx0, dy = dy0;

       if (k > 0)
         dx = -dx, dy = -dy;

       // walk along the line using fixed-point arithmetics,
       // stop at the image border or in case of too big gap
       for (;; x += dx, y += dy)
       {
         uchar* mdata;
         int i1, j1;

         if (xflag)
         {
           j1 = x;
           i1 = y >> shift;
         }
         else
         {
           j1 = x >> shift;
           i1 = y;
         }

         mdata = mdata0 + i1*width + j1;

         // for each non-zero point:
         //    update line end,
         //    clear the mask element
         //    reset the gap
         if (*mdata)
         {
           //if语句的作用是清除那些已经判定是好的直线上的点对应的累加器的值，避免再次利用这些累加值
           if (good_line)    //在第一次搜索中已经确定是好的直线
           {
             //得到累加器矩阵地址指针
             adata = (int*)accum.data;
             for (n = 0; n < numangle; n++, adata += numrho)
             {
               r = cvRound(j1 * ttab[n * 2] + i1 * ttab[n * 2 + 1]);
               r += (numrho - 1) / 2;
               adata[r]--;    //相应的累加器减1
             }
           }
           //搜索过的位置，不管是好的直线，还是坏的直线，掩码相应位置都清0，这样下次就不会再重复搜索这些位置了，从而达到减小计算边缘点的目的
           *mdata = 0;
         }
         //如果已经到达了直线的端点，则退出循环
         if (i1 == line_end[k].y && j1 == line_end[k].x)
           break;
       }
     }
     //如果是好的直线
     if (good_line)
     {
       Vec4i lr(line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y );
       //把两个端点压入序列中
       //cvSeqPush(lines, &lr);
       lines.push_back(lr);
       //如果检测到的直线数量大于阈值，则退出该函数
       if (lines.size() >= linesMax)
         return linesMax;
     }
   }
   free(seq);
   return lines.size();
 }


