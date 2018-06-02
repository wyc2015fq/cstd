// 线积分卷积算法(Line Integral Convolution，LIC法)
// 是一种经典的矢量场可视化方法，由Cabral和Leedom 在1993年的SIGGRAPH大会上提出，
// 该算法可以有效地表征二维矢量场，既能清楚直观地反映出每点的速度方向，又能表达矢量场的细节。
// 该算法的提出对于矢量场可视化具有广泛而深远的意义，已被成功应用到很多领域。
// 其发展至今，已经涉及到噪声选取、信号滤波、卷积运算、纹理映射及计算机动画等多种技术，具有广阔的发展前景。
//  用卷积来表示矢量场的方向源于一种运动模糊的思想。LIC算法原理可概述为利用一维低通卷积核
// 双向对称地沿流线方向卷积白噪声纹理，最终合成矢量纹理。下图为LIC算法计算示意图。
// http://www.zhanpingliu.org/Research/FlowVis/LIC/LIC.htm

#define DISCRETE_FILTER_SIZE 2048      //卷积核的对应项，如果太小的话有些采样点就没有被卷积进去，影响图像效果
#define LOWPASS_FILTR_LENGTH 32.00000f
#define LINE_SQUARE_CLIP_MAX 100000.0f //只要大于等于0就行，等于0时会出现黑色线条
#define VECTOR_COMPONENT_MIN 0.050000f

/*----------------------------------------------------------------------------------------------*/


/*合成数据----------------------------------------------------*/
void SyntheszSaddle(int _row, int _col, float* pVectr)
{
	int row = 400, col = 400, index = 0;
	float vec_x = 0.0f, vec_y = 0.0f, vcMag = 0.0f, scale = 0.0f;
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			// 数据生成
			index = i * col + j;// 中心矢量场
			// index = (row - 1 - i) * col + j;// 马鞍矢量场
			vec_x = -(float)i / row + 0.5f;
			vec_y =  (float)j / col - 0.5f;

			// 数据规整
			vcMag = float(sqrt(vec_x * vec_x + vec_y * vec_y));
			scale = (vcMag < 0.001f) ? 0.0f : 1.0f / vcMag;
			vec_x *= scale;
			vec_y *= scale;

			pVectr[2*index]     = vec_x;
			pVectr[2*index + 1] = vec_y;
		}
	}
}
/*------------------------------------------------------------*/


/*数据规整------------------------------------------------------------------------------------------------------------*/
void NormalizVectrs(int w, int h, float* pVectr)
{
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			int   index = (j * w + i) << 1;
			float vcMag = float( sqrt( double(pVectr[index] * pVectr[index] + pVectr[index + 1] * pVectr[index + 1]) ) );
			float scale = (vcMag == 0.0f) ? 0.0f : 1.0f / vcMag;
			pVectr[index    ] *= scale;//矢量分量的放大倍数，越大，速度越大，程序运行速度会减慢，但效果会更清晰
			pVectr[index + 1] *= scale;
		}//for
	}//for
}
/*--------------------------------------------------------------------------------------------------------------------*/


/*生成白噪声----------------------------------------------------*/
void MakeWhiteNoise(int w, int h, unsigned char* pNoise)
{
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			int r = rand();
			r = ( (r & 0xff) + ( (r & 0xff00) >> 8 ) ) & 0xff;//
			pNoise[j * w + i] = (unsigned char) r;
		}
	}
}
/*--------------------------------------------------------------*/


/*卷积核LUTsiz = DISCRETE_FILTER_SIZE ----------------------*/
void GenBoxFiltrLUT(int LUTsiz, float* p_LUT0, float* p_LUT1)
{
	for(int i = 0; i < LUTsiz; i++)
	{
		p_LUT0[i] = p_LUT1[i] = (float)i;
	}
}
/*----------------------------------------------------------*/


/*写文件----------------------------------------------------------------------*/
void WriteImage2PPM(int w, int h, unsigned char* pImage, char* f_name)
{
	FILE* o_file;
	if( (o_file = fopen(f_name, "w") ) == NULL )
	{
		printf("Can't open output file\n");
		return;
	}

	fprintf(o_file, "P6\n%d %d\n255\n", w, h);

	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			unsigned char unchar = pImage[j * w + i];
			fprintf(o_file, "%c%c%c", unchar, unchar, unchar);
		}
	}

	fclose(o_file);
	o_file = NULL;
}
/*-----------------------------------------------------------------------------*/


/*计算像素点------------------------------------------------------------------------------------------------*/
void FlowImagingLIC(int    w, int h,    float* pVectr, unsigned char* pNoise, unsigned char* pImage,
					float* p_LUT0, float* p_LUT1, float krnlen)
{
    int     vec_id = 0;
	int     advDir = 0;  //方向
	int     advcts = 0;	 //追踪的步数
	int     ADVCTS = int(krnlen * 3);//追踪的最大步数

	float   vctr_x = 0;  //速度x分量
	float   vctr_y = 0;  //速度y分量
	float   clp0_x = 0;  //当前点x坐标
	float   clp0_y = 0;  //当前点y坐标
	float   clp1_x = 0;  //下一点x坐标
	float   clp1_y = 0;  //下一点y坐标
	float   samp_x = 0;  //采样点x坐标
	float   samp_y = 0;  //采样点y坐标
	float   tmpLen = 0;  //临时长度
	float   segLen = 0;  //每段长度
	float   curLen = 0;  //当前的流线长度
	float   prvLen = 0;  //上一条流线长度
	float   W_ACUM = 0;  //计算权重之和
	float   texVal = 0;  //纹理的灰度值
	float   smpWgt = 0;  //当前采样点的权重值
	float   t_acum[2];   //输入纹理对应流线上的灰度值之和
	float   w_acum[2];   //权重之和
	float*  wgtLUT = NULL;//权重查找表
	float   len2ID = (DISCRETE_FILTER_SIZE - 1) / krnlen;//将曲线长度映射到卷积核的某一项
    //krnlen等于LOWPASS_FILTR_LENGTH
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			t_acum[0] = t_acum[1] = w_acum[0] = w_acum[1] = 0.0f;
            
			//追踪方向，正向流线与反向流线
			for(advDir = 0; advDir < 2; advDir++)
			{
				//**初始化追踪步数、追踪长度、种子点位置
				advcts = 0;
				curLen = 0.0f;
				clp0_x = i + 0.5f;
				clp0_y = j + 0.5f;

				//**获取相应的卷积核
				wgtLUT = (advDir == 0) ? p_LUT0 : p_LUT1;
                
				//**循环终止条件：流线追踪的足够长或者到达了涡流的中心
				while(curLen < krnlen && advcts < ADVCTS)
				{
					//**获得采样点的矢量数据
					vec_id = ( int(clp0_y) * w + int(clp0_x) ) << 1;
					vctr_x = pVectr[vec_id    ];
					vctr_y = pVectr[vec_id + 1];
                    
					//**若为关键点即一般情况下为涡流的中心时，跳出本次追踪
					if( vctr_x == 0 && vctr_y == 0)
					{
						t_acum[advDir] = (advcts == 0) ? 0.0f : t_acum[advDir];
					    w_acum[advDir] = (advcts == 0) ? 1.0f : w_acum[advDir];
						break;
					}
					
					//**正向追踪或反向追踪
					vctr_x = (advDir == 0) ? vctr_x : -vctr_x;
					vctr_y = (advDir == 0) ? vctr_y : -vctr_y;

					segLen = LINE_SQUARE_CLIP_MAX;
					segLen = (vctr_x < -VECTOR_COMPONENT_MIN) ? ( int(     clp0_x         ) - clp0_x ) / vctr_x : segLen;
					segLen = (vctr_x >  VECTOR_COMPONENT_MIN) ? ( int( int(clp0_x) + 1.5f ) - clp0_x ) / vctr_x : segLen;

					segLen = (vctr_y < -VECTOR_COMPONENT_MIN) ?
												  ( ( (tmpLen = ( int(     clp0_y         ) - clp0_y ) / vctr_y) < segLen) ? tmpLen : segLen) : segLen;
					segLen = (vctr_y >  VECTOR_COMPONENT_MIN) ?
						                          ( ( (tmpLen = ( int( int(clp0_y) + 1.5f ) - clp0_y ) / vctr_y) < segLen) ? tmpLen : segLen) : segLen;

					prvLen = curLen;
					curLen += segLen;
					segLen += 0.0004f;//如何不增加的话，还是会出现问题
					
					//**判断长度
					segLen = (curLen > krnlen) ? ( (curLen = krnlen) - prvLen ) : segLen;
                    
					//**获取下一个追踪点位置
					clp1_x = clp0_x + vctr_x * segLen;
					clp1_y = clp0_y + vctr_y * segLen;
					
					//**计算采样点位置
					samp_x = (clp0_x + clp1_x) * 0.5f;
					samp_y = (clp0_y + clp1_y) * 0.5f;

					///获取纹理采样点的灰度值->这里如果采用插值的话或者效果应该又是另一个样子
					texVal = pNoise[ int(samp_y) * w + int(samp_x) ];

					W_ACUM = wgtLUT[ int(curLen * len2ID) ];
					smpWgt = W_ACUM - w_acum[advDir];
					w_acum[advDir] = W_ACUM;
					t_acum[advDir] += texVal * smpWgt;

					advcts ++;
					clp0_x = clp1_x;
					clp0_y = clp1_y;
                    
					if( clp0_x < 0.0f || clp0_x >= w || clp0_y < 0.0f || clp0_y >= h)
					{
						break;
					}
				}//while
			}//for

			texVal = (t_acum[0] + t_acum[1]) / (w_acum[0] + w_acum[1]);

			texVal = (texVal <   0.0f) ?   0.0f : texVal;
			texVal = (texVal > 255.0f) ? 255.0f : texVal;

			pImage[j * w + i] = (unsigned char)texVal;
		}//for
	}//for
}
/*----------------------------------------------------------------------------------------------------------*/
void test_lic1()
{
  int SQUARE_FLOW_FIELD_SZ = 400;
  int            w = SQUARE_FLOW_FIELD_SZ;
  int            h = SQUARE_FLOW_FIELD_SZ;
  float*         pVectr = (float*        )malloc( sizeof(float) * w * h * 2 );
  float*         p_LUT0 = (float*        )malloc( sizeof(float) * DISCRETE_FILTER_SIZE);
  float*         p_LUT1 = (float*        )malloc( sizeof(float) * DISCRETE_FILTER_SIZE);
  unsigned char* pNoise = (unsigned char*)malloc( w * h     );
  unsigned char* pImage = (unsigned char*)malloc( w * h     );
  
  //SyntheszSaddle(h, w, pVectr);
  // NormalizVectrs(w, h, pVectr);//数据不规整一下的话，速度越小的地方噪声越强
  //MakeWhiteNoise(h, w, pNoise, w);
  //GenBoxFiltrLUT(DISCRETE_FILTER_SIZE, p_LUT0, p_LUT1);
  FlowImagingLIC(h, w, pVectr, pNoise, pImage, p_LUT0, p_LUT1, LOWPASS_FILTR_LENGTH);
  //WriteImage2PPM(w, h, pImage, "LIC.ppm");
  cvShowImage("pNoise", h, w, pNoise, w, 1);
  cvShowImage("pImage", h, w, pImage, w, 1);
  cvWaitKey(-1);
  

  free(pVectr);
  free(p_LUT0);
  free(p_LUT1);
  free(pNoise);
  free(pImage);
}


/*----------------------------------------------------------------------------------------------*/
void test_lic()
{
  int SQUARE_FLOW_FIELD_SZ = 400;
  int            w = SQUARE_FLOW_FIELD_SZ;
  int            h = SQUARE_FLOW_FIELD_SZ;
  float*         pVectr = (float*        )malloc( sizeof(float        ) * w * h * 2 );
  float*         p_LUT0 = (float*        )malloc( sizeof(float        ) * DISCRETE_FILTER_SIZE);
  float*         p_LUT1 = (float*        )malloc( sizeof(float        ) * DISCRETE_FILTER_SIZE);
  unsigned char* pNoise = (unsigned char*)malloc( sizeof(unsigned char) * w * h     );
  unsigned char* pImage = (unsigned char*)malloc( sizeof(unsigned char) * w * h     );
  
  SyntheszSaddle(w, h, pVectr);
  // NormalizVectrs(w, h, pVectr);//数据不规整一下的话，速度越小的地方噪声越强
  MakeWhiteNoise(w, h, pNoise);
  GenBoxFiltrLUT(DISCRETE_FILTER_SIZE, p_LUT0, p_LUT1);
  FlowImagingLIC(w, h, pVectr, pNoise, pImage, p_LUT0, p_LUT1, LOWPASS_FILTR_LENGTH);
  //WriteImage2PPM(w, h, pImage, "LIC.ppm");
  
  cvShowImage("pNoise", h, w, pNoise, w, 1);
  cvShowImage("pImage", h, w, pImage, w, 1);
  cvWaitKey(-1);

  free(pVectr);  pVectr = NULL;
  free(p_LUT0);  p_LUT0 = NULL;
  free(p_LUT1);  p_LUT1 = NULL;
  free(pNoise);  pNoise = NULL;
  free(pImage);  pImage = NULL;
}